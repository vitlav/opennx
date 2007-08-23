// $Id$
//
// Copyright (C) 2006 The OpenNX Team
// Author: Fritz Elfert
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Library General Public License as
// published by the Free Software Foundation; either version 2 of the
// License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this program; if not, write to the
// Free Software Foundation, Inc.,
// 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "WinShare.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/defs.h"
#endif

#include "WinShare.h"
#include <wx/utils.h>
#include <wx/dynlib.h>
#include <wx/filename.h>
#include <wx/log.h>
#include <wx/arrimpl.cpp>

static wxString MYTRACETAG(wxFileName::FileName(wxT(__FILE__)).GetName());

#ifdef __UNIX__

#include <cups/cups.h>
#include <libsmbclient.h>

typedef int (*SMBC_init)(smbc_get_auth_data_fn fn, int debug);
typedef int (*SMBC_opendir)(const char *url);
typedef struct smbc_dirent* (*SMBC_readdir)(unsigned int dh);
typedef int (*SMBC_closedir)(int dh);

// dummy typedefs for windows funcptrs
typedef void (*NT_NetShareEnum)(void);
typedef void (*NT_NetApiBufferFree)(void);
typedef void (*W9X_NetShareEnum)(void);

typedef int (*FP_cupsGetDests)(cups_dest_t ** dests);
typedef const char * (*FP_cupsServer)();

// authentication-callback, used by libsmbclient
// we always return empty username and password which means "do anonymous logon"
static void
smbc_auth_fn(const char *, const char *, char *, int,
        char *username, int unmaxlen, char *password, int pwmaxlen)
{
    *username = '\0'; *password = '\0';
}

#else // __UNIX__
// Windows

#include <lm.h>

struct share_info_1 {
    char		shi1_netname[LM20_NNLEN+1];
    char		shi1_pad1;
    unsigned short	shi1_type;
    char FAR *		shi1_remark;
};  /* share_info_1 */

typedef NET_API_STATUS (__stdcall *NT_NetShareEnum)(LPWSTR svname, DWORD level, LPBYTE* bufptr, DWORD prefmaxlen,
        LPDWORD entriesread, LPDWORD totalentries, LPDWORD resume_handle);
typedef NET_API_STATUS (__stdcall *NT_NetApiBufferFree)(LPVOID buffer);
typedef NET_API_STATUS (__stdcall *W9X_NetShareEnum)(const char *svname, short sLevel, char *pBuffer,
        unsigned short cbBuffer, unsigned short *pcEntriesRead,
        unsigned short *pcTotalAvail);
// dummy typedefs for libsmbclient
typedef void (*SMBC_init)(void);
typedef void (*SMBC_opendir)(void);
typedef void* (*SMBC_readdir)(void);
typedef void (*SMBC_closedir)(void);
// dummy typedefs for cups
typedef void cups_dest_t;
typedef int (*FP_cupsGetDests)(cups_dest_t ** dests);
typedef const char * (*FP_cupsServer)();

#endif // __UNIX__

WX_DEFINE_OBJARRAY(ArrayOfShares);

class DllData
{
    public:
        typedef enum {
            SmbClientWinNT,
            SmbClientWin95,
            SmbClientUnix,
            CupsClientUnix,
        } ClientType;

        DllData(ClientType);
        ~DllData();

        ArrayOfShares GetShares();
        bool IsAvailable();

    private:
        bool isNT;
        bool isSMBC;
        wxDllType handle;
        NT_NetShareEnum NT_enum;
        NT_NetApiBufferFree NT_free;
        W9X_NetShareEnum W9X_enum;
        SMBC_init C_init;
        SMBC_opendir C_opendir;
        SMBC_readdir C_readdir;
        SMBC_closedir C_closedir;
        FP_cupsGetDests cupsGetDests;
        FP_cupsServer cupsServer;
};

DllData::DllData(ClientType ct)
{
    isNT = false;
    isSMBC = false;
    handle = NULL;
    switch (ct) {
        case SmbClientUnix:
            {
                wxLogNull lognull;
                wxDynamicLibrary dll(wxT("libsmbclient"));
                if (dll.IsLoaded()) {
                    C_init = (SMBC_init)dll.GetSymbol(wxT("smbc_init"));
                    C_opendir = (SMBC_opendir)dll.GetSymbol(wxT("smbc_opendir"));
                    C_readdir = (SMBC_readdir)dll.GetSymbol(wxT("smbc_readdir"));
                    C_closedir = (SMBC_closedir)dll.GetSymbol(wxT("smbc_closedir"));
                    handle = dll.Detach();
                }
                isSMBC = true;
            }
            break;
        case SmbClientWinNT:
            {
                wxLogNull lognull;
                wxDynamicLibrary dll(wxT("netapi32"));
                if (dll.IsLoaded()) {
                    NT_enum = (NT_NetShareEnum)dll.GetSymbol(wxT("NetShareEnum"));
                    NT_free = (NT_NetApiBufferFree)dll.GetSymbol(wxT("NetApiBufferFree"));
                    handle = dll.Detach();
                    isNT = true;
                }
            }
            break;
        case SmbClientWin95:
            {
                wxLogNull lognull;
                wxDynamicLibrary dll(wxT("svrapi32"));
                if (dll.IsLoaded()) {
                    W9X_enum = (W9X_NetShareEnum)dll.GetSymbol(wxT("NetShareEnum"));
                    handle = dll.Detach();
                }
            }
            break;
        case CupsClientUnix:
            {
                wxLogNull lognull;
                wxDynamicLibrary dll(wxT("libcups"));
                if (dll.IsLoaded()) {
                    cupsGetDests = (FP_cupsGetDests)dll.GetSymbol(wxT("cupsGetDests"));
                    cupsServer = (FP_cupsServer)dll.GetSymbol(wxT("cupsServer"));
                    handle = dll.Detach();
                }
            }
            break;
    }
}

DllData::~DllData()
{
    if (handle)
        wxDynamicLibrary::Unload(handle);
}

ArrayOfShares DllData::GetShares()
{
    ArrayOfShares sa;

    if (!handle)
        return sa;

#ifdef __UNIX__
    if (isSMBC) {
        // Unix, use libsmbclient
        if (C_init(smbc_auth_fn, 0) == 0) {
            int d = C_opendir("smb://127.0.0.1/");
            if (d >= 0) {
                struct smbc_dirent *e;
                while ((e = C_readdir(d))) {
                    SharedResource r;
                    switch (e->smbc_type) {
                        case SMBC_FILE_SHARE:
                        case SMBC_PRINTER_SHARE:
                            r.name = wxConvLocal.cMB2WX(e->name);
                            r.description = wxConvLocal.cMB2WX(e->comment);
                            r.sharetype = (e->smbc_type == SMBC_FILE_SHARE) ?
                                SharedResource::SHARE_SMB_DISK : SharedResource::SHARE_SMB_PRINTER;
                            sa.Add(r);
                            break;
                    }
                }
                C_closedir(d);
            }
        }
    } else {
        // Unix, use libcups
        ::wxLogTrace(MYTRACETAG, wxT("Retrieving CUPS destinations"));
        cups_dest_t *dests = NULL;
        int ndests = cupsGetDests(&dests);
        for (int i = 0; i < ndests; i++) {
            SharedResource r;
            r.name = wxConvLocal.cMB2WX(dests[i].name);
            r.description = wxConvLocal.cMB2WX(dests[i].instance);
            r.sharetype = SharedResource::SHARE_CUPS_PRINTER;
            sa.Add(r);
        }
    }
#else // __UNIX__

    // Windows specific stuff
    if (isNT) {
        // NT, Win2K, XP
        PSHARE_INFO_1 BufPtr, p;
        NET_API_STATUS res;
        DWORD er = 0, tr = 0, resume = 0, i;
        do {
            res = NT_enum(NULL, 1, (LPBYTE *)&BufPtr, (DWORD)-1, &er, &tr, &resume);
            if (res == ERROR_SUCCESS || res == ERROR_MORE_DATA) {
                p = BufPtr;
                for (i = 1; i <= er; i++) {
                    SharedResource r;
                    switch (p->shi1_type) {
                        case STYPE_DISKTREE:
                        case STYPE_PRINTQ:
                            r.name = wxConvLocal.cWC2WX((const wchar_t*)p->shi1_netname);
                            r.description = wxConvLocal.cWC2WX((const wchar_t*)p->shi1_remark);
                            r.sharetype = (p->shi1_type == STYPE_DISKTREE) ?
                                SharedResource::SHARE_SMB_DISK : SharedResource::SHARE_SMB_PRINTER;
                            if (r.name != wxT("print$"))
                                sa.Add(r);
                            break;
                    }
                    p++;
                }
                NT_free(BufPtr);
            }
        } while (res == ERROR_MORE_DATA);
    } else {
        // Win 95, 98, Me
        char *buf = (char *)malloc(sizeof(share_info_1));
        unsigned short cbBuffer = 0;
        unsigned short nEntries = 0;
        unsigned short nTotal = 0;

        if (buf) {
            // First, get number of total entries
            NET_API_STATUS nStatus = W9X_enum(NULL, 1, buf, cbBuffer, &nEntries, &nTotal);
            if (nTotal) {
                free(buf);
                cbBuffer = nTotal * sizeof(share_info_1);
                buf = (char *)malloc(cbBuffer);
                if (buf) {
                    nStatus = W9X_enum(NULL, 1, buf, cbBuffer, &nEntries, &nTotal);
                    if ((nStatus == NERR_Success) || (nStatus == ERROR_MORE_DATA)) {
                        struct share_info_1 *p = (struct share_info_1 *)buf;
                        for (int i = 0; (i < nEntries); i++) {
                            if (p) {
                                SharedResource r;
                                switch (p->shi1_type) {
                                    case STYPE_DISKTREE:
                                    case STYPE_PRINTQ:
                                        r.name = wxConvUTF8.cMB2WX(p->shi1_netname);
                                        r.description = wxConvUTF8.cMB2WX(p->shi1_remark);
                                        r.sharetype = (p->shi1_type == STYPE_DISKTREE) ?
                                            SharedResource::SHARE_SMB_DISK : SharedResource::SHARE_SMB_PRINTER;
                                        if (r.name != wxT("print$"))
                                            sa.Add(r);
                                        break;
                                }
                                p++;
                            }
                        }
                    }
                    free(buf);
                }
            }
        }
    }
#endif // __UNIX__

    return sa;
}

bool DllData::IsAvailable()
{
    bool ret = false;
    ::wxLogTrace(MYTRACETAG, wxT("DllData(SMB) IsAvailable called"));
#ifdef __UNIX__
    if (handle) {
        if (isSMBC) {
            // Probe Samba
            if (C_init(smbc_auth_fn, 0) == 0) {
                ::wxLogTrace(MYTRACETAG, wxT("C_init success"));
                int d = C_opendir("smb://127.0.0.1/");
                if (d >= 0) {
                    ret = true;
                    C_closedir(d);
                }
            } else
                ::wxLogTrace(MYTRACETAG, wxT("C_init returned error"));
        } else {
            // Probe Cups
            return (cupsServer() != NULL);
        }
    }
#endif
    return ret;
}

    SmbClient::SmbClient()
: dllPrivate(NULL)
{
    int vMajor, vMinor;

    int osType = ::wxGetOsVersion(&vMajor, &vMinor);
    if (osType & (wxOS_UNIX | wxOS_MAC_OSX_DARWIN))
        dllPrivate = new DllData(DllData::SmbClientUnix);
    else if (osType & wxOS_WINDOWS) {
        switch (osType) {
            case wxOS_WINDOWS_NT:
                dllPrivate = new DllData(DllData::SmbClientWinNT);
                break;
            case wxOS_WINDOWS_9X:
                dllPrivate = new DllData(DllData::SmbClientWin95);
                break;
        }
    }
}

SmbClient::~SmbClient()
{
    if (dllPrivate)
        delete dllPrivate;
}

    ArrayOfShares
SmbClient::GetShares()
{
    if (dllPrivate)
        m_shares = dllPrivate->GetShares();
    ::wxLogTrace(MYTRACETAG, wxT("# of SMB shares: %d"), m_shares.GetCount());
    return m_shares;
}

    bool
SmbClient::IsAvailable()
{
#ifdef __UNIX__
    return dllPrivate && dllPrivate->IsAvailable();
#else
    return true;
#endif
}

    CupsClient::CupsClient()
: dllPrivate(NULL)
{
#ifdef __UNIX__
    dllPrivate = new DllData(DllData::CupsClientUnix);
#endif
}

CupsClient::~CupsClient()
{
    if (dllPrivate)
        delete dllPrivate;
}

    ArrayOfShares
CupsClient::GetShares()
{
#ifdef __UNIX__
    if (dllPrivate)
        m_shares = dllPrivate->GetShares();
#endif
    ::wxLogTrace(MYTRACETAG, wxT("# of CUPS shares: %d"), m_shares.GetCount());
    return m_shares;
}

    bool
CupsClient::IsAvailable()
{
    return dllPrivate && dllPrivate->IsAvailable();
}

