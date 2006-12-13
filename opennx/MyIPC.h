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

#ifndef _MYIPC_H_
#define _MYIPC_H_

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "MyIPC.cpp"
#endif

#include <wx/event.h>

DECLARE_EVENT_TYPE(wxEVT_NXSSH, -1);

class AsyncProcess;
class wxRegEx;

class MyIPC : public wxEvtHandler
{
    DECLARE_CLASS(MyIPC);
    DECLARE_EVENT_TABLE();

public:
    typedef enum {
        ActionNone,
        ActionStatus,
        ActionLog,
        ActionWarning,
        ActionError,
        ActionPromptYesNo,
        ActionSendUsername,
        ActionSendPassword,
        ActionNextCommand,
        ActionWelcome,
        ActionPassphraseDialog,
        ActionSetSessionID,
        ActionSetProxyCookie,
        ActionSetProxyIP,
        ActionSetSessionType,
        ActionSetSessionCache,
        ActionSetSessionDisplay,
        ActionSetAgentCookie,
        ActionSetSslTunneling,
        ActionSetSubscription,
        ActionExit,
        ActionChildDied,
        ActionStartProxy,
        ActionSessionRunning,
        ActionSessionListStart,
        ActionSessionListEnd,
    } tSessionEvents;

    MyIPC();
    virtual ~MyIPC();
    bool SshProcess(const wxString&, const wxString&, wxEvtHandler *);
    bool IsRunning();
    bool Kill();
    void Print(const wxString &s, bool doLog = true);
    int GetResult();

private:
    int parseCode(const wxString &);
    virtual void OnOutReceived(wxCommandEvent &);
    virtual void OnErrReceived(wxCommandEvent &);
    virtual void OnTerminate(wxCommandEvent &);

#ifdef __WXMSW__
    unsigned int m_MsgSession;
    unsigned int m_MsgKill;
#endif
    AsyncProcess *m_pProcess;
    wxEvtHandler *m_pEvtHandler;
    wxRegEx *m_re;
    bool m_bIsSsh;
    int m_iSshPid;
    int m_iOutCollect;
    int m_iErrCollect;
    wxString m_sOutMessage;
    wxString m_sErrMessage;
};

#endif
// _MYIPC_
