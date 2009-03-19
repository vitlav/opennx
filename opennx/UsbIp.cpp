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
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "UsbIp.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/defs.h"
#endif

#include "UsbIp.h"

#include <wx/log.h>
#include <wx/socket.h>

#include "trace.h"
ENABLE_TRACE;

IMPLEMENT_CLASS(UsbIp, wxEvtHandler);

BEGIN_EVENT_TABLE(UsbIp, wxEvtHandler)
    EVT_SOCKET(wxSOCKET_INPUT, UsbIp::OnInput)
    EVT_SOCKET(wxSOCKET_OUTPUT, UsbIp::OnOutput)
    EVT_SOCKET(wxSOCKET_CONNECTION, UsbIp::OnConnect)
    EVT_SOCKET(wxSOCKET_LOST, UsbIp::OnLost)
END_EVENT_TABLE();

UsbIp::UsbIp()
{
    wxUNIXaddress addr;
    addr.Filename(wxT("/tmp/xyzsock"));
    // for testing during development, use hardcoded path for now
#warning Replace hardcoded socket path
    m_pSocketClient = new wxSocketClient();
    m_pSocketClient->SetNotify(
            wxSOCKET_INPUT_FLAG|wxSOCKET_OUTPUT_FLAG|wxSOCKET_CONNECTION_FLAG|wxSOCKET_LOST_FLAG);
    m_pSocketClient->Notify(true);
    m_pSocketClient->SetTimeout(10); // 10 sec.
    m_pSocketClient->Connect(addr, false);
}

UsbIp::~UsbIp()
{
    if (m_pSocketClient)
        m_pSocketClient->Destroy();
    m_pSocketClient = NULL;
}

void UsbIp::OnInput(wxSocketEvent &event)
{
}

void UsbIp::OnOutput(wxSocketEvent &event)
{
}

void UsbIp::OnConnect(wxSocketEvent &event)
{
}

void UsbIp::OnLost(wxSocketEvent &event)
{
}

#ifdef SUPPORT_USBIP
#endif
