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

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
#ifdef __WXMSW__
#include <shlobj.h>
#endif

#ifdef __UNIX__
#include <unistd.h>
#endif
#include <wx/cmdline.h>
#include <wx/xrc/xmlres.h>
#include <wx/image.h>
#include <wx/config.h>
#include <wx/fs_zip.h>
#include "wx/fs_mem.h"
#include <wx/sysopt.h>
#include <wx/tokenzr.h>

#include "resource.h"
#include "opennxApp.h"
#include "SessionAdmin.h"
#include "SessionProperties.h"
#include "LoginDialog.h"
#include "MyWizard.h"
#include "MyXmlConfig.h"

#include "memres.h"

#ifdef MYTRACETAG
# undef MYTRACETAG
#endif
#define MYTRACETAG wxT("opennxApp")

// Create a new application object: this macro will allow wxWindows to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also declares the accessor function
// wxGetApp() which will return the reference of the right type (i.e. opennxApp and
// not wxApp)
IMPLEMENT_APP(opennxApp);

    wxString
opennxApp::LoadFileFromResource(const wxString &loc, bool bUseLocale /* = true */)
{
    bool tryloop = true;
    wxString ret;
    wxString cloc = bUseLocale ? m_cLocale.GetCanonicalName() : wxT("");
    wxFileSystem fs;
    wxFSFile *f;

    do {
        wxString tryloc = loc;

        if (!cloc.IsEmpty()) {
            tryloc = wxFileName(loc).GetPath(wxPATH_GET_SEPARATOR|wxPATH_GET_VOLUME)
                + cloc + wxT("_") + wxFileName(loc).GetFullName();
            cloc = cloc.BeforeLast(_T('_'));
        } else
            tryloop = false;

        // try plain loc first
        f = fs.OpenFile(tryloc);
        
        if (!f)
            f = fs.OpenFile(GetResourcePrefix() + tryloc);
        
        if (f) {
            wxInputStream *is = f->GetStream();
            size_t size = is->GetSize();
            char *buf = new char[size+2];
            is->Read(buf, size);
            delete f;
            buf[size] = buf[size+1] = 0;
            ret = wxConvLocal.cMB2WX(buf);
            delete []buf;
        }
    } while (ret.IsEmpty() && tryloop);
    return ret;
}

static const wxChar *desktopDirs[] = {
    wxT("Desktop"), wxT("KDesktop"), wxT(".gnome-desktop"), NULL
};

    bool
opennxApp::CreateDesktopEntry(MyXmlConfig *cfg)
{
    wxString appDir;
    wxConfigBase::Get()->Read(wxT("Config/SystemNxDir"), &appDir);
#ifdef __WXMSW__
    TCHAR dtPath[MAX_PATH];
    if (SHGetSpecialFolderPath(NULL, dtPath, CSIDL_DESKTOPDIRECTORY, FALSE)) {
        wxString linkPath = wxString::Format(wxT("%s\\%s.lnk"), dtPath, cfg->sGetName().c_str());
        wxString targetPath = wxString::Format(wxT("%s\\bin\\opennx.exe"), appDir.c_str());
        wxString desc = _("Launch NX Session");
        wxString args = wxString::Format(wxT("--session=\"%s\""), cfg->sGetFileName().c_str());
        HRESULT hres;
        IShellLink* psl;

        hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER,
                IID_IShellLink, (LPVOID *) &psl);
        if (SUCCEEDED(hres)) {
            IPersistFile* ppf;
            psl->SetPath(targetPath.c_str());
            psl->SetWorkingDirectory(appDir.c_str());
            psl->SetDescription(desc.c_str());
            psl->SetArguments(args.c_str());
            psl->SetIconLocation(targetPath, 1);
            hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf);
            if (SUCCEEDED(hres)) {
                hres = ppf->Save(wxConvLocal.cWX2WC(linkPath), TRUE);
                ppf->Release();
            }
            psl->Release();
        }
    }
#endif
#ifdef __UNIX__
    wxString dtEntry = wxT("[Desktop Entry]\n");
    dtEntry += wxT("Encoding=UTF-8\n");
    dtEntry += wxT("Type=Application\n");
    dtEntry += wxT("MimeType=\n");
    dtEntry += wxT("StartupNotify=true\n");
    dtEntry += wxT("Terminal=false\n");
    dtEntry += wxT("TerminalOptions=\n");
    dtEntry += wxT("Comment=Launch NX Session\n");
    dtEntry += wxT("Comment[de]=Starte NX Sitzung\n");
    dtEntry += wxT("Name=") + cfg->sGetName() + wxT("\n");
    dtEntry += wxT("GenericName=OpenNX Client\n");
    dtEntry += wxT("GenericName[de]=OpenNX Client\n");
    dtEntry += wxT("Exec=") + appDir + wxT("/bin/opennx --session=\"")
        + cfg->sGetFileName() + wxT("\"\n");
    dtEntry += wxT("Icon=") + appDir + wxT("/share/icons/nx-desktop.png\n");

    const wxChar **p = desktopDirs;
    while (*p) {
        wxString path = wxGetHomeDir() + wxT("/") + *p;
        if (::wxDirExists(path)) {
            wxFile f;
            wxString fn = path + wxT("/") + cfg->sGetName() + wxT(".desktop");
            ::wxLogTrace(MYTRACETAG, wxT("Creating '%s'"), fn.c_str());
            if (f.Create(fn, true, wxS_IRUSR|wxS_IWUSR|wxS_IRGRP|wxS_IROTH)) {
                f.Write(dtEntry);
                f.Close();
            }
        }
        p++;
    }
#endif
}

    bool
opennxApp::RemoveDesktopEntry(MyXmlConfig *cfg)
{
#ifdef __WXMSW__
    TCHAR dtPath[MAX_PATH];
    if (SHGetSpecialFolderPath(NULL, dtPath, CSIDL_DESKTOPDIRECTORY, FALSE))
        wxRemoveFile(wxString::Format(_T("%s\\%s.lnk"), dtPath, cfg->sGetName().mb_str()));
#endif
#ifdef __UNIX__
    const wxChar **p = desktopDirs;

    while (*p) {
        ::wxRemoveFile(wxString::Format(_T("%s/%s/%s.desktop"),
                    ::wxGetHomeDir().c_str(), *p,cfg->sGetName().c_str()));
        p++;
    }
#endif
    ::wxLogTrace(MYTRACETAG, wxT("Removing '%s'"), cfg->sGetFileName().c_str());
    ::wxRemoveFile(cfg->sGetFileName());
}

opennxApp::opennxApp()
    : m_pCfg(NULL)
{

    SetAppName(wxT("OpenNX"));
#ifdef __WXMSW__
    m_pCfg = new wxConfig(wxT("OpenNX"), wxT("Millenux"));
#else
    m_pCfg = new wxConfig(wxT("OpenNX"), wxT("Millenux"), wxT(".opennx"));
#endif
    wxConfigBase::Set(m_pCfg);

#ifdef __WXMSW__
    DWORD dummy;
    DWORD viSize;
    LPVOID vi;
    TCHAR mySelf[MAX_PATH];
    VS_FIXEDFILEINFO *vsFFI;
    UINT vsFFIlen;
    m_sVersion = wxT("?.?");

    if (GetModuleFileName(NULL, mySelf, sizeof(mySelf))) {
        viSize = GetFileVersionInfoSize(mySelf, &dummy);
        if (viSize) {
            vi = (LPVOID)malloc(viSize);
            if (vi) {
                if (GetFileVersionInfo(mySelf, dummy, viSize, vi)) {
                    if (VerQueryValue(vi, wxT("\\"), (LPVOID *)&vsFFI, &vsFFIlen)) {
                        m_sVersion = wxString::Format(wxT("%d.%d"), HIWORD(vsFFI->dwFileVersionMS),
                                LOWORD(vsFFI->dwFileVersionMS));
                        if (vsFFI->dwFileVersionLS)
                            m_sVersion += wxString::Format(wxT(".%d"), HIWORD(vsFFI->dwFileVersionLS));
                        if (LOWORD(vsFFI->dwFileVersionLS))
                            m_sVersion += wxString::Format(wxT(".%d"), LOWORD(vsFFI->dwFileVersionLS));
                    }

                }
                free(vi);
            }
        }
    }
#else
    m_sVersion = wxT(PACKAGE_VERSION);
#endif

    wxString tmp;
    if (!wxConfigBase::Get()->Read(wxT("Config/UserNxDir"), &tmp)) {
        tmp = ::wxGetHomeDir() + wxFileName::GetPathSeparator() + wxT(".nx");
        if (!wxFileName(tmp).DirExists())
            wxFileName(tmp).Mkdir();
        wxConfigBase::Get()->Write(wxT("Config/UserNxDir"), tmp);
        wxFileName::Mkdir(tmp +  wxFileName::GetPathSeparator() + wxT("config"), 0750, wxPATH_MKDIR_FULL);
    }

    wxFileName fn;
    if (!wxConfigBase::Get()->Read(wxT("Config/SystemNxDir"), &tmp)) {
#define NotImplemented
#ifdef __WXMSW__
        int ret = GetModuleFileName(NULL, tmp.GetWriteBuf(1024), 1024);
        tmp.UngetWriteBuf(ret);
        fn.Assign(tmp);
# undef NotImplemented
#endif
#ifdef __OPENBSD__
        // FIXME: How to get one's own exe path?
        fn.Assign(wxT("/usr/local/NX/bin/opennx"));
# undef NotImplemented
#endif
#ifdef __linux__
        // Get executable path from /proc/self/exe
        char ldst[PATH_MAX+1];
        int ret = readlink("/proc/self/exe", ldst, PATH_MAX);
        if (ret == -1)
            ret = 0;
        ldst[ret] = '\0';
        if (ret == 0)
            fn.Assign(wxT("/usr/NX/bin/opennx"));
        else
            fn.Assign(wxConvLocal.cMB2WX(ldst));
# undef NotImplemented
#endif
#ifdef NotImplemented
# error Missing Implementation for this OS
#endif
        m_sSelfPath = fn.GetFullPath();
        fn.RemoveLastDir();
        wxConfigBase::Get()->Write(wxT("Config/SystemNxDir"),
                fn.GetPath(wxPATH_GET_VOLUME));
    }
    wxConfigBase::Get()->Flush();
    wxConfigBase::Get()->Read(wxT("Config/SystemNxDir"), &tmp);

#ifdef __UNIX__
    wxString lpath;
    if (::wxGetEnv(wxT("LD_LIBRARY_PATH"), &lpath))
        lpath += wxT(":");
    lpath = tmp + wxT("/lib");
    if (!::wxSetEnv(wxT("LD_LIBRARY_PATH"), lpath))
        ::wxLogSysError(wxT("Can not set LD_LIBRARY_PATH"));
#endif

    wxString traceTags;
    if (::wxGetEnv(wxT("WXTRACE"), &traceTags)) {
        wxStringTokenizer t(traceTags, wxT(",:"));
        while (t.HasMoreTokens()) {
            wxString tag = t.GetNextToken();
            ::wxLogDebug(wxT("Trace for '%s' enabled"), tag.c_str());
            wxLog::AddTraceMask(tag);
        }
    }
}

opennxApp::~opennxApp()
{
    if (m_pCfg)
        delete m_pCfg;
}

void opennxApp::OnInitCmdLine(wxCmdLineParser& parser)
{
    // Init standard options (--help, --verbose);
    wxApp::OnInitCmdLine(parser);


    parser.AddSwitch(wxT(""), wxT("admin"),
            _("Start the session administration tool."));
    parser.AddOption(wxT(""), wxT("session"),
            _("Run a session importing configuration settings from FILENAME."));
    parser.AddSwitch(wxT(""), wxT("wizard"),
            _("Guide the user through the steps to configure a session."));
}

bool opennxApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
    if (!wxApp::OnCmdLineParsed(parser))
        return false;

    m_eMode = MODE_CLIENT;
    if (parser.Found(wxT("admin")))
        m_eMode = MODE_ADMIN;
    if (parser.Found(wxT("wizard")))
        m_eMode = MODE_WIZARD;
    (void)parser.Found(wxT("session"), &m_sSessionName);
    return true;
}

// 'Main program' equivalent: the program execution "starts" here
bool opennxApp::OnInit()
{
    m_cLocale.AddCatalogLookupPathPrefix(wxT("locale"));
    m_cLocale.Init();
    m_cLocale.AddCatalog(wxT("opennx"));

    // Don't remap bitmaps to system colors
    wxSystemOptions::SetOption(wxT("msw.remap"), 0);

    // Call to base class needed for initializing command line processing
    if (!wxApp::OnInit())
        return false;

    wxFileSystem::AddHandler(new wxZipFSHandler);
    wxFileSystem::AddHandler(new wxMemoryFSHandler);
    wxInitAllImageHandlers();
    wxBitmap::InitStandardHandlers();
    wxXmlResource::Get()->InitAllHandlers();

    const unsigned char *resptr = get_mem_res();
    wxMemoryFSHandler::AddFile(wxT("memrsc"), resptr, cnt_mem_res);
    free_mem_res(resptr);
    m_sResourcePrefix = wxT("memory:memrsc#zip:");
    if (!wxXmlResource::Get()->Load(m_sResourcePrefix + wxT("res/opennx.xrc"))) {
        wxMemoryFSHandler::RemoveFile(wxT("memrsc"));
        return false;
    }

    if (m_eMode == MODE_ADMIN) {
        SessionAdmin *sa = new SessionAdmin(NULL);
        // If we return true, the global config will
        // be deleted by the framework, so we set it to NULL here
        // to prevent double free.
        m_pCfg = NULL;
        sa->Show();
        SetTopWindow(sa);
        return true;
    }

    if ((m_eMode == MODE_CLIENT) && m_sSessionName.IsEmpty()) {
        if (!wxConfigBase::Get()->Read(wxT("Config/LastSession"), &m_sSessionName))
            m_eMode = MODE_WIZARD;
    } else {
        if (!m_sSessionName.IsEmpty()) {
            MyXmlConfig cfg(m_sSessionName);
            if (cfg.IsValid())
                m_sSessionName = cfg.sGetName();
        }
    }

    if (m_eMode == MODE_WIZARD) {
        MyWizard wz(NULL);
        if (!wz.Run()) {
            wxMemoryFSHandler::RemoveFile(wxT("memrsc"));
            return false;
        }
        m_sSessionName = wz.sGetConfigName();
    }

    LoginDialog d;
    d.SetSessionName(m_sSessionName);
    d.Create(NULL);
    if (d.ShowModal() == wxID_OK) {
        m_sSessionName = d.GetSessionName();
        if (!m_sSessionName.IsEmpty())
            wxConfigBase::Get()->Write(wxT("Config/LastSession"), m_sSessionName);
    }

    // success: wxApp::OnRun() will be called which will enter the main message
    // loop and the application will run. We returne FALSE here, so that the
    // application exits if the dialog is destroyed.
    wxMemoryFSHandler::RemoveFile(wxT("memrsc"));
    return false;
}

/*!
 * Cleanup for opennxApp
 */
int opennxApp::OnExit()
{
    wxMemoryFSHandler::RemoveFile(wxT("memrsc"));
    return wxApp::OnExit();
}
