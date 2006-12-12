/////////////////////////////////////////////////////////////////////////////
// Name:        MyIPC.h
// Purpose:     
// Author:      Fritz Elfert
// Modified by: 
// Created:     06/13/04 22:44:20
// RCS-ID:      $Id$
// Copyright:   Copyright 2004 Millenux GmbH
// Licence:     LGPL
/////////////////////////////////////////////////////////////////////////////

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
    } tSessionEvents;

    MyIPC();
    virtual ~MyIPC();
    bool SshProcess(const wxString&, const wxString&, wxEvtHandler *);
    bool IsRunning();
    bool Kill();
    void Print(const wxString &s);
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
