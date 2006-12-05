/////////////////////////////////////////////////////////////////////////////
// $Id: MxIPC.cpp 899 2004-12-09 19:38:35Z felfert $
//
// Author:      Fritz Elfert
// Created:     06/13/04 22:44:20
// Copyright:   Copyright 2004 Millenux GmbH
// Licence:     LGPL
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma implementation "MxIPC.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/process.h>
#include <wx/txtstrm.h>
#include <wx/filename.h>
#include <wx/regex.h>
#include <wx/timer.h>

#include "MxIPC.h"

#ifdef MYTRACETAG
# undef MYTRACETAG
#endif
#define MYTRACETAG wxT("MxIPC")

class AsyncProcess : public wxProcess
{
public:
    AsyncProcess(const wxString& wdir, const wxString& cmd)
        : wxProcess(), m_sDir(wdir), m_sCmd(cmd)
    { Redirect(); }

    virtual void OnTerminate(int pid, int status);
    virtual bool CheckInput();

    bool Start();
    bool IsRunning();
    bool Kill();
    bool GetMessage(wxString &ret);
    bool Print(wxString);
    int GetStatus();

private:
    long m_iPid;
    int m_iStatus;
    bool m_bTerminated;

    wxString m_sOutBuf;
    wxString m_sErrBuf;
    wxString m_sCmd;
    wxString m_sDir;

    wxStopWatch m_cIoTimer;
    wxMutex m_cMsgMutex;
    wxArrayString m_aMessages;
};

bool AsyncProcess::CheckInput()
{
    bool loopi = IsInputAvailable();
    bool loope = IsErrorAvailable();
    bool ret;
    char c;

    m_cIoTimer.Start((loopi||loope) ? 0 : 2000);
    while ((loopi || loope) && (m_cIoTimer.Time() < 1000)) {
        if (IsInputAvailable()) {
            c = GetInputStream()->GetC();
            m_cIoTimer.Start(0);
            switch (c) {
                case '\r':
                    break;
                case '\n':
                    m_cMsgMutex.Lock();
                    m_aMessages.Add(m_sOutBuf);
                    m_cMsgMutex.Unlock();
                    m_sOutBuf = wxT("");
                    loopi = false;
                    break;
                default:
                    m_sOutBuf += c;
                    break;
            }
        }
        if (IsErrorAvailable()) {
            c = GetErrorStream()->GetC();
            m_cIoTimer.Start(0);
            switch (c) {
                case '\r':
                    break;
                case '\n':
                    m_cMsgMutex.Lock();
                    m_aMessages.Add(m_sErrBuf);
                    m_cMsgMutex.Unlock();
                    m_sErrBuf = wxT("");
                    loope = false;
                    break;
                default:
                    m_sErrBuf += c;
                    break;
            }
        }
    }
    m_cIoTimer.Pause();
    if (m_cIoTimer.Time() >= 1000) {
        if (!m_sErrBuf.IsEmpty()) {
            m_cMsgMutex.Lock();
            m_aMessages.Add(m_sErrBuf);
            m_cMsgMutex.Unlock();
            m_sErrBuf = wxT("");
            m_cIoTimer.Start(0);
            m_cIoTimer.Pause();
        }
        if (!m_sOutBuf.IsEmpty()) {
            m_cMsgMutex.Lock();
            m_aMessages.Add(m_sOutBuf);
            m_cMsgMutex.Unlock();
            m_sOutBuf = wxT("");
            m_cIoTimer.Start(0);
            m_cIoTimer.Pause();
        }
    }

    if (wxLog::IsAllowedTraceMask(MYTRACETAG)) {
        m_cMsgMutex.Lock();
        size_t cnt = m_aMessages.GetCount();
        ::wxLogTrace(MYTRACETAG, wxT("Messages: %d"), cnt);
        for (size_t i = 0; i < cnt; i++)
            ::wxLogTrace(MYTRACETAG, wxT("  m[%d]='%s'"), i, m_aMessages[i].c_str());
        m_cMsgMutex.Unlock();
    }
    return (m_cIoTimer.Time() < 1000);
}

bool AsyncProcess::Print(wxString s)
{
    wxOutputStream *os = GetOutputStream();
    if (os) {
        wxTextOutputStream tos(*os);
        tos << s.c_str();
        return true;
    }
    return false;
}

void AsyncProcess::OnTerminate(int pid, int status)
{
    while (CheckInput())
        ;
    wxLogTrace(MYTRACETAG, wxT("Process %u terminated with exit code %d."), pid, status);
    m_iStatus = status;
    m_bTerminated = true;
}

bool AsyncProcess::Start()
{
    bool ret = false;

    if (!m_sCmd.IsEmpty()) {
        wxString cwd = ::wxGetCwd();
        wxFileName::SetCwd(m_sDir);
        m_sOutBuf = wxT("");
        m_sErrBuf = wxT("");
        m_aMessages.Empty();
        m_iPid = ::wxExecute(m_sCmd, wxEXEC_ASYNC, this);
        ret = (m_iPid != 0);
        wxFileName::SetCwd(cwd);
        m_bTerminated = !ret;
    }
    return ret;
}

int AsyncProcess::GetStatus()
{
    return m_iStatus;
}

bool AsyncProcess::IsRunning()
{
    return CheckInput() || !m_bTerminated;
}

bool AsyncProcess::Kill()
{
    bool ret = false;
    if (m_iPid) {
        switch (wxProcess::Kill(m_iPid, wxSIGKILL)) {
            case wxKILL_OK:
            case wxKILL_NO_PROCESS:
                ret = true;
        }
    }
    return ret;
}

bool AsyncProcess::GetMessage(wxString &ret)
{
    m_cMsgMutex.Lock();
    if (m_aMessages.GetCount() > 0) {
        ret = m_aMessages[0];
        m_aMessages.RemoveAt(0);
    } else
        ret = wxT("");
    m_cMsgMutex.Unlock();
    return (!ret.IsEmpty());
}

MxIPC::MxIPC()
    : m_pProcess(NULL)
    , m_bIsSsh(false)
    , m_iLineCount(0)
    , m_sMessage(wxT(""))
{
#ifdef __WXMSW__
    m_MsgSession = ::RegisterWindowMessage(wxT("NX_SESSION_MESSAGE"));
    m_MsgKill = ::RegisterWindowMessage(wxT("NxKill"));
#endif
    m_re = new wxRegEx();
    m_re->Compile(wxT("\\(yes/no\\)\\?"), wxRE_ADVANCED|wxRE_ICASE);
}

MxIPC::~MxIPC()
{
    if (m_pProcess) {
        if (m_pProcess->IsRunning())
            m_pProcess->Detach();
        else
            delete m_pProcess;
        m_pProcess = NULL;
    }
    delete m_re;
}

bool MxIPC::IsRunning()
{
    return (m_pProcess && m_pProcess->IsRunning());
}

int MxIPC::GetResult()
{
    return m_pProcess ? m_pProcess->GetStatus() : -1;
}

bool MxIPC::Kill()
{
    bool ret (m_pProcess && m_pProcess->Kill());
    m_pProcess = NULL;
    return ret;
}

void MxIPC::Print(wxString s)
{
    if (m_pProcess)
        m_pProcess->Print(s);
}

bool MxIPC::SshProcess(wxString dir, wxString cmd)
{
    m_bIsSsh = true;
    bool ret = false;
    m_iLineCount = 0;
    m_sMessage = wxT("");
    m_pProcess = new AsyncProcess(dir, cmd);
    ret = m_pProcess->Start();
    if (!ret) {
        delete m_pProcess;
        m_pProcess = NULL;
    }
    return ret;
}

static int parseCode(wxString &buf)
{
    int ret = -1;
    long n;
    if (buf.Find(wxT("NX> ")) == 0) {
        int idx = 0;
        while (buf.Mid(4+idx, 1).IsNumber()) {
            if (ret == -1)
                ret = 0;
            buf.Mid(4+idx, 1).ToLong(&n);
            ret *= 10;
            ret += n;
            idx++;
        }
    }
    return ret;
}

    const MxIPC::ChatActions
MxIPC::SshChat(wxString &ret)
{
    wxString tmp;
    int ccode, i;
    long n;
    bool gotText = m_pProcess->GetMessage(tmp);

    ret = wxT("");
    if (m_bIsSsh && (gotText || IsRunning())) {
        if (gotText) {
            wxLogTrace(MYTRACETAG, wxT("sshOUT: '%s'"), tmp.c_str());
            ccode = parseCode(tmp);
            wxLogTrace(MYTRACETAG, wxT("ccode: %d"), ccode);
            switch (ccode) {
                case -1:
                    // No code found
                    ret = tmp;
                    if (tmp.Contains(wxT("usage:")))
                        return ActionError;
                    if (tmp.Contains(wxT("Connection refused")))
                        return ActionError;
                    if (tmp.Contains(wxT("no address associated")))
                        return ActionError;
                    if (tmp.Contains(wxT("Warning:")))
                        return ActionWarning;
                    if (m_iLineCount) {
                        m_iLineCount--;
                        m_sMessage += wxT("\n");
                        m_sMessage += tmp;
                        if (m_iLineCount == 0) {
                            m_re->Replace(&m_sMessage, wxT(""));
                            ret = m_sMessage + wxT("?");
                            return ActionPromptYesNo;
                        }
                    }
                    return ActionNone;
                case 100:
                    // Server version & licence
                    ret = tmp.Mid(8);
                    return ActionStatus;
                case 101:
                    // request username
                    return ActionSendUsername;
                case 102:
                    // request password
                    return ActionSendPassword;
                case 103:
                    // Welcome message
                    ret = tmp.Mid(8);
                    return ActionStatus;
                case 105:
                    // request for next command
                    return ActionNextCommand;
                case 106:
                    // Result: server parameters
                    // fall thru
                case 110:
                    // Result: server status (running|stopped)
                    // fall thru
                case 113:
                    // Announce: changing password
                    // fall thru
                case 114:
                    // Announce: password changed
                    // fall thru
                case 122:
                    // Result: service started
                    // fall thru
                case 123:
                    // Result: service stopped
                    // fall thru
                case 127:
                    // Result: session list
                    // fall thru
                case 146:
                    // Result: user list
                    // fall thru
                case 147:
                    // Result: server capacity reached
                    // fall thru
                case 148:
                    // Result: server capacity not reached
                    return ActionNone;
                case 134:
                    // Accepted protocol: <version>
                    ret = _("Protocol accepted");
                    return ActionStatus;

                    /* Codes 200 - 299 are from local nxssh */
                case 200:
                    // Connected to adress ...
                    // fall thru

#ifdef OBSOLETE_CODE
                    // Apparently gone
                case 201:
                    ret = wxT("NX Server not installed or the server access has been disabled");
                    return ActionError;
#endif

                case 202:
                    // Authenticating user ...
                    ret = tmp.Mid(8);
                    return ActionStatus;
                case 203:
                    // NXSSH started with pid: ...
                    ret = tmp.AfterFirst(wxT(':')).Strip(wxString::both);
                    if (ret.IsNumber()) {
                        ret.ToLong(&n);
                        m_iSshPid = n;
                    }
                    ret = tmp.Mid(8);
                    return ActionNotice;
                case 204:
                    // Authentication failed
                    ret = _("Authentication failed");
                    return ActionError;
                case 205:
                    // ?? generic
                    m_iLineCount = 2;
                    m_sMessage = tmp.Mid(8);
                    return ActionNone;
                case 208:
                    // Using auth method: ...
                    ret = tmp.Mid(8);
                    return ActionNotice;
                case 209:
                    // Remote host authentication has changed
                    ret = tmp.Mid(8);
                    return ActionWarning;
                case 210:
                    // Enter passwhrase for key ...
                    ret = tmp.Mid(8);
                    return ActionPassphraseDialog;
                case 211:
                    // The authenticity of host ... can't be verified ... (multiline message)
                    m_iLineCount = 2;
                    m_sMessage = tmp.Mid(8);
                    return ActionNone;
                case 280:
                case 282:
                case 285:
                case 286:
                case 287:
                case 288:
                case 289:
                    // Debug messages
                    ret = tmp.Mid(8);
                    return ActionNotice;
                case 290:
                case 291:
                case 292:
                case 294:
                case 295:
                case 296:
                case 297:
                case 298:
                case 299:
                    // Subchannel system ?
                    return ActionNone;

#ifdef OBSOLETE_CODE
                    // Apparently gone
                case 300:
                case 301:
                    ret = wxT("Connection error");
                    return -99;
                case 302:
                case 303:
                    ret = wxT("Connection refused");
                    return -99;
                case 304:
                case 305:
                    ret = wxT("Host not found");
                    return -99;
                case 306:
                    ret = wxT("Network unreachable");
                    return -99;
                case 307:
                case 310:
                    ret = wxT("Host key verification failed");
                    return -99;
#endif

                case 404:
                    // Error: wrong password or login
                    // fall thru
                case 500:
                    // Generic error
                    // fall thru
                case 503:
                    // Unknown command
                    // fall thru
                case 504:
                    // Session startup failed
                    // fall thru
                case 537:
                    // Passwords did not match
                    // fall thru
                case 599:
                    // Session limit exceeded
                    ret = tmp.Mid(8);
                    return ActionError;

                case 700:
                    // Session ID
                    ret = tmp.AfterFirst(wxT(':')).Strip(wxString::both);
                    return ActionSetSessionID;
                case 701:
                    // Session proxy cookie
                    ret = tmp.AfterFirst(wxT(':')).Strip(wxString::both);
                    return ActionSetProxyCookie;
                case 702:
                    // Session proxy IP
                    ret = tmp.AfterFirst(wxT(':')).Strip(wxString::both);
                    return ActionSetProxyIP;
                case 703:
                    // Session type
                    ret = tmp.AfterFirst(wxT(':')).Strip(wxString::both);
                    return ActionSetSessionType;
                case 704:
                    // Session cache
                    ret = tmp.AfterFirst(wxT(':')).Strip(wxString::both);
                    return ActionSetSessionCache;
                case 705:
                    // Session display
                    ret = tmp.AfterFirst(wxT(':')).Strip(wxString::both);
                    return ActionSetSessionDisplay;
                case 706:
                    // Session agent cookie
                    ret = tmp.AfterFirst(wxT(':')).Strip(wxString::both);
                    return ActionSetAgentCookie;
                case 707:
                    // Session ssl tunneling
                    ret = tmp.AfterFirst(wxT(':')).Strip(wxString::both);
                    return ActionSetSslTunneling;
                case 708:
                    ret = tmp.AfterFirst(wxT(':')).Strip(wxString::both);
                    return ActionSetSubscription;
                case 710:
                    // session running
                    return ActionStartProxy;
                case 716:
                    // terminationg session on user request
                    // fall thru
                case 718:
                    // session restore successful
                    // fall thru
                case 719:
                    // SMB running
                    // fall thru
                case 1000:
                    // nxnode version
                    // fall thru
                case 1001:
                    // Bye
                    // fall thru
                case 1002:
                    // Commit
                    // fall thru
                    ret = tmp.Mid(8);
                    return ActionNotice;
                case 1004:
                    // Session status: failed
                    // fall thru
                    ret = tmp.Mid(8);
                    return ActionError;
                case 1005:
                    // Session status: suspended
                    // fall thru
                    ret = tmp.Mid(8);
                    return ActionNotice;
                case 1006:
                    // Session status: running|closed
                    // fall thru
                case 1009:
                    // Session status: terminating
                    ret = tmp.Mid(8);
                    return ActionNotice;
                case 999:
                    // Terminating
                    return ActionExit;
                default:
                    ret = wxString::Format(_("Unexpected response: '%s'"), tmp.c_str());
                    return ActionError;
            }
        }
        return ActionNone;
    }
    while (m_pProcess->GetMessage(tmp)) {
        if (!ret.IsEmpty())
            ret += wxT('\n');
        ret += tmp;
    }
    return ActionChildDied;
}
