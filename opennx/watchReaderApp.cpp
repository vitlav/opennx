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
}

bool watchReaderApp::OnInit()
{    
    // Win: Don't remap bitmaps to system colors
    wxSystemOptions::SetOption(wxT("msw.remap"), 0);

    if (!wxApp::OnInit())
        return false;

    LibOpenSC opensc;
    opensc.WatchHotRemove(m_iReader, m_lSshPid);
    SetExitOnFrameDelete(true);
    return false;
}


int watchReaderApp::OnExit()
{
	return wxApp::OnExit();
}

