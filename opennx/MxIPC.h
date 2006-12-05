/////////////////////////////////////////////////////////////////////////////
// Name:        MxIPC.h
// Purpose:     
// Author:      Fritz Elfert
// Modified by: 
// Created:     06/13/04 22:44:20
// RCS-ID:      $Id: MxIPC.h 883 2004-11-04 21:15:18Z felfert $
// Copyright:   Copyright 2004 Millenux GmbH
// Licence:     LGPL
/////////////////////////////////////////////////////////////////////////////

#ifndef _MXIPC_H_
#define _MXIPC_H_

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "MxIPC.cpp"
#endif

class AsyncProcess;
class wxRegEx;

class MxIPC
{
public:
    typedef enum {
        ActionNone,
        ActionStatus,
        ActionNotice,
        ActionWarning,
        ActionError,
        ActionPromptYesNo,
        ActionSendUsername,
        ActionSendPassword,
        ActionNextCommand,
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
    } ChatActions;

    MxIPC();
    virtual ~MxIPC();
    bool SshProcess(wxString, wxString);
    bool IsRunning();
    bool Kill();
    const ChatActions SshChat(wxString &);
    void Print(wxString s);
    int GetResult();

private:
#ifdef __WXMSW__
    unsigned int m_MsgSession;
    unsigned int m_MsgKill;
#endif
    AsyncProcess *m_pProcess;
    wxRegEx *m_re;
    bool m_bIsSsh;
    int m_iSshPid;
    int m_iLineCount;
    wxString m_sMessage;
};

#endif
// _MXIPC_
