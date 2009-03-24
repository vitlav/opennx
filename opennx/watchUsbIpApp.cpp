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

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/cmdline.h>
#include <wx/xrc/xmlres.h>
#include <wx/msgdlg.h>
#include <wx/config.h>
#include <wx/fs_zip.h>
#include "wx/fs_mem.h"
#include <wx/wfstream.h>
#include <wx/mimetype.h>
#include <wx/sysopt.h>
#include <wx/socket.h>

#include "watchUsbIpApp.h"
#include "Icon.h"
#include "LibUSB.h"
#include "xh_richtext.h"
#include "UsbIp.h"

#include "memres.h"

#include "trace.h"
ENABLE_TRACE;
DECLARE_TRACETAGS;

IMPLEMENT_APP( watchUsbIpApp )
IMPLEMENT_CLASS( watchUsbIpApp, wxApp )

BEGIN_EVENT_TABLE(watchUsbIpApp, wxApp)
END_EVENT_TABLE()


watchUsbIpApp::watchUsbIpApp()
{
    SetAppName(wxT("OpenNX"));
    wxConfig *cfg;
#ifdef __WXMSW__
    cfg = new wxConfig(wxT("OpenNX"), wxT("InnoviData"));
#else
    cfg = new wxConfig(wxT("OpenNX"), wxT("InnoviData"), wxT(".opennx"), wxT("opennx.conf"));
#endif
    wxConfigBase::Set(cfg);

    wxLogNull dummy;
    // Try to get KDE language settings and set locale accordingly
    wxFileInputStream fis(::wxGetHomeDir() +
            wxFileName::GetPathSeparator() + wxT(".kde") + 
            wxFileName::GetPathSeparator() + wxT("share") + 
            wxFileName::GetPathSeparator() + wxT("config") + 
            wxFileName::GetPathSeparator() + wxT("kdeglobals"));
    if (fis.IsOk()) {
        wxFileConfig cfg(fis);
        wxString country = cfg.Read(wxT("Locale/Country"), wxEmptyString);
        wxString lang = cfg.Read(wxT("Locale/Language"), wxEmptyString);
        if ((!lang.IsEmpty()) && (!country.IsEmpty()))
            ::wxSetEnv(wxT("LANG"), lang + wxT("_") + country.Upper() + wxT(".UTF-8"));
    }
}


void watchUsbIpApp::OnInitCmdLine(wxCmdLineParser& parser)
{
    // Init standard options (--help, --verbose);
    wxApp::OnInitCmdLine(parser);
    parser.AddOption(wxT("s"), wxT("sessionid"), _("NX SessionID to watch for."),
            wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
    parser.AddOption(wxT("c"), wxT("config"), _("Path of session config file."),
            wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
    parser.AddOption(wxT("p"), wxT("pid"), _("Process ID of the nxssh process."),
            wxCMD_LINE_VAL_NUMBER, wxCMD_LINE_OPTION_MANDATORY);
}

bool watchUsbIpApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
    if (!wxApp::OnCmdLineParsed(parser))
        return false;
    parser.Found(wxT("s"), &m_sSessionID);
    parser.Found(wxT("c"), &m_sSessionConfig);
    parser.Found(wxT("p"), &m_lSshPid);
}

bool watchUsbIpApp::OnInit()
{    
    wxString tmp;
    wxConfigBase::Get()->Read(wxT("Config/SystemNxDir"), &tmp);
    m_cLocale.AddCatalogLookupPathPrefix(tmp + wxFileName::GetPathSeparator()
            + wxT("share") + wxFileName::GetPathSeparator() + wxT("locale"));
    m_cLocale.AddCatalogLookupPathPrefix(wxT("locale"));
    m_cLocale.Init();
    m_cLocale.AddCatalog(wxT("opennx"));

    // Win: Don't remap bitmaps to system colors
    wxSystemOptions::SetOption(wxT("msw.remap"), 0);
    // WinXP: Don't draw themed gradients on notebook pages
    wxSystemOptions::SetOption(wxT("msw.notebook.themed-background"), 0);

    wxFileSystem::AddHandler(new wxZipFSHandler);
    wxFileSystem::AddHandler(new wxMemoryFSHandler);
    wxInitAllImageHandlers();
    wxBitmap::InitStandardHandlers();
    wxXmlResource::Get()->InitAllHandlers();
    wxXmlResource::Get()->AddHandler(new wxRichTextCtrlXmlHandler());

    // This enable socket-I/O from other threads.
    wxSocketBase::Initialize();

    bool resok = false;
    wxString optionalRsc = tmp + wxFileName::GetPathSeparator() + wxT("share")
        + wxFileName::GetPathSeparator() + wxT("opennx.rsc");
    if (wxFileName::FileExists(optionalRsc)) {
        wxFile rf(optionalRsc);
        if (rf.IsOpened()) {
            unsigned char *resptr = (unsigned char *)malloc(rf.Length());
            if (resptr) {
                if (rf.Read(resptr, rf.Length()) == rf.Length()) {
                    wxMemoryFSHandler::AddFileWithMimeType(wxT("memrsc"), resptr, rf.Length(), wxT("application/zip"));
                    {
                        // The following code eliminates a stupid error dialog which shows up
                        // if some .desktop entires (in KDE or GNOME applink dirs) are dangling symlinks.
                        wxLogNull lognull;
                        wxTheMimeTypesManager->GetFileTypeFromExtension(wxT("zip"));
                    }
                    resok = true;
                }
                free(resptr);
            }
        }
    }
    if (!resok) {
        const unsigned char *resptr = get_mem_res();
        if (resptr) {
            wxMemoryFSHandler::AddFileWithMimeType(wxT("memrsc"), resptr, cnt_mem_res, wxT("application/zip"));
            {
                // The following code eliminates a stupid error dialog which shows up
                // if some .desktop entires (in KDE or GNOME applink dirs) are dangling symlinks.
                wxLogNull lognull;
                wxTheMimeTypesManager->GetFileTypeFromExtension(wxT("zip"));
            }
            free_mem_res(resptr);
        }
        resok = true;
    }
    if (!resok) {
        wxLogFatalError(wxT("Could not load application resource."));
        return false;
    }

    m_sResourcePrefix = wxT("memory:memrsc#zip:");
    if (!wxXmlResource::Get()->Load(m_sResourcePrefix + wxT("res/opennx.xrc")))
        return false;

    if (!wxApp::OnInit())
        return false;

    SetExitOnFrameDelete(true);
    return false;
}


int watchUsbIpApp::OnExit()
{
    return wxApp::OnExit();
}

