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
#include <wx/msgdlg.h>
#include <wx/config.h>
#include <wx/wfstream.h>
#include <wx/sysopt.h>

#include "watchReaderApp.h"
#include "LibOpenSC.h"

#include "trace.h"
ENABLE_TRACE;
DECLARE_TRACETAGS;

IMPLEMENT_APP( watchReaderApp )
IMPLEMENT_CLASS( watchReaderApp, wxApp )

BEGIN_EVENT_TABLE(watchReaderApp, wxApp)
END_EVENT_TABLE()


watchReaderApp::watchReaderApp()
{
    SetAppName(wxT("OpenNX"));
    wxConfig *cfg;
#ifdef __WXMSW__
    cfg = new wxConfig(wxT("OpenNX"), wxT("InnoviData"));
#else
    cfg = new wxConfig(wxT("OpenNX"), wxT("InnoviData"), wxT(".opennx"), wxT("opennx.conf"));
#endif
    wxConfigBase::Set(cfg);

#ifndef __WXMSW__
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
#endif
}


void watchReaderApp::OnInitCmdLine(wxCmdLineParser& parser)
{
    // Init standard options (--help, --verbose);
    wxApp::OnInitCmdLine(parser);
    parser.AddOption(wxT("r"), wxT("reader"), _("Number of smart card reader to watch for."),
            wxCMD_LINE_VAL_NUMBER, wxCMD_LINE_OPTION_MANDATORY);
    parser.AddOption(wxT("p"), wxT("pid"), _("Process ID of the nxssh process."),
            wxCMD_LINE_VAL_NUMBER, wxCMD_LINE_OPTION_MANDATORY);
}

bool watchReaderApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
    if (!wxApp::OnCmdLineParsed(parser))
        return false;
    long tmp;
    if (parser.Found(wxT("r"), &tmp))
        m_iReader = tmp;
    if (parser.Found(wxT("p"), &tmp))
        m_lSshPid = tmp;
    return true;
}

bool watchReaderApp::OnInit()
{    
    initWxTraceTags();
    wxString tmp;
    wxConfigBase::Get()->Read(wxT("Config/SystemNxDir"), &tmp);
    m_cLocale.AddCatalogLookupPathPrefix(tmp + wxFileName::GetPathSeparator()
            + wxT("share") + wxFileName::GetPathSeparator() + wxT("locale"));
    m_cLocale.AddCatalogLookupPathPrefix(wxT("locale"));
    m_cLocale.Init();
    m_cLocale.AddCatalog(wxT("opennx"));

    // Win: Don't remap bitmaps to system colors
    wxSystemOptions::SetOption(wxT("msw.remap"), 0);

    if (!wxApp::OnInit())
        return false;

    LibOpenSC opensc;
    if (opensc.WatchHotRemove(m_iReader, m_lSshPid))
        wxMessageBox(
                _("OpenNX session has been suspended, because\nthe authenticating smart card has been removed."),
                _("Smart card removed"), wxOK|wxICON_INFORMATION);
    SetExitOnFrameDelete(true);
    return false;
}


int watchReaderApp::OnExit()
{
    return wxApp::OnExit();
}

