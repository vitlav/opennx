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
    bool sGetOut(wxString &ret);
    bool sGetErr(wxString &ret);
    bool Print(wxString);

private:
    long m_iPid;
    int m_iStatus;
    bool m_bTerminated;

    wxString m_sOut;
    wxString m_sErr;
    wxString m_sOutBuf;
    wxString m_sErrBuf;
    wxString m_sCmd;
    wxString m_sDir;

    wxStopWatch m_cOutTimer;
    wxStopWatch m_cErrTimer;
};

bool AsyncProcess::CheckInput()
{
    bool ret = false;

    if (IsInputAvailable()) {
        char c = GetInputStream()->GetC();
        switch (c) {
            case '\r':
                break;
            case '\n':
                m_sOut = m_sOutBuf;
                m_sOutBuf = _T("");
                ret = true;
                break;
            default:
                m_sOutBuf += c;
                m_cOutTimer.Start(0);
                break;
        }
    }

    if (IsErrorAvailable()) {
        char c = GetErrorStream()->GetC();
        switch (c) {
            case '\r':
                break;
            case '\n':
                m_sErr = m_sErrBuf;
                m_sErrBuf = _T("");
                ret = true;
                break;
            default:
                m_sErrBuf += c;
                m_cErrTimer.Start(0);
                break;
        }
    }

    if (!ret) {
        if ((m_cErrTimer.Time() > 1000) && (!m_sErrBuf.IsEmpty())) {
            m_sErr = m_sErrBuf;
            m_sErrBuf = _T("");
            return true;
        }
        if ((m_cOutTimer.Time() > 1000) && (!m_sOutBuf.IsEmpty())) {
            m_sOut = m_sOutBuf;
            m_sOutBuf = _T("");
            return true;
        }
    }
    return ret;
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
    wxLogDebug(_T("Process %u ('%s') terminated with exit code %d."),
                pid, m_sCmd.c_str(), status);
    m_iStatus = status;
    m_bTerminated = true;
    delete this;
}

bool AsyncProcess::Start()
{
    bool ret = false;

    if (!m_sCmd.IsEmpty()) {
        wxString cwd = ::wxGetCwd();
        wxFileName::SetCwd(m_sDir);
        m_iPid = ::wxExecute(m_sCmd, wxEXEC_ASYNC, this);
        ret = (m_iPid != 0);
        wxFileName::SetCwd(cwd);
        m_bTerminated = !ret;
        m_sOut = _T("");
        m_sErr = _T("");
        m_sOutBuf = _T("");
        m_sErrBuf = _T("");
    }
    return ret;
}

bool AsyncProcess::IsRunning()
{
    CheckInput();
    return !m_bTerminated;
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

bool AsyncProcess::sGetOut(wxString &ret)
{
    ret = m_sOut;
    m_sOut = _T("");
    return (!ret.IsEmpty());
}

bool AsyncProcess::sGetErr(wxString &ret)
{
    ret = m_sErr;
    m_sErr = _T("");
    return (!ret.IsEmpty());
}

MxIPC::MxIPC()
    : m_pProcess(NULL)
    , m_bIsSsh(false)
    , m_iLineCount(0)
    , m_sMessage(_T(""))
{
#ifdef __WXMSW__
  m_MsgSession = ::RegisterWindowMessage(_T("NX_SESSION_MESSAGE"));
  m_MsgKill = ::RegisterWindowMessage(_T("NxKill"));
#endif
  m_re = new wxRegEx();
  m_re->Compile(_T("\\(yes/no\\)\\?"), wxRE_ADVANCED|wxRE_ICASE);
}

MxIPC::~MxIPC()
{
    if (m_pProcess)
        delete m_pProcess;
    delete m_re;
}

bool MxIPC::IsRunning()
{
    return (m_pProcess && m_pProcess->IsRunning());
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
    m_sMessage = _T("");
    m_pProcess = new AsyncProcess(dir, cmd);
    ret = m_pProcess->Start();
    if (!ret) {
        delete m_pProcess;
        m_pProcess = NULL;
    }
    return ret;
}

int MxIPC::SshChat(wxString &ret)
{
    if (m_bIsSsh && m_pProcess) {
        wxString tmp;
        bool gotText = false;

        if (m_pProcess->sGetOut(tmp)) {
            gotText = true;
            wxLogDebug(_T("sshOUT: '%s'"), tmp.c_str());
        } else {
            if (m_pProcess->sGetErr(tmp)) {
                gotText = true;
                wxLogDebug(_T("sshERR: '%s'"), tmp.c_str());
            }
        }
        if (gotText) {
            int ccode = 0;
            int i;
            long n;
            if (tmp.Find(_T("NX> ")) == 0) {
                int idx = 0;
                while (tmp.Mid(4+idx, 1).IsNumber()) {
                    tmp.Mid(4+idx, 1).ToLong(&n);
                    ccode *= 10;
                    ccode += n;
                    idx++;
                }
            }
            switch (ccode) {
                case 200:
                    ret = _T("Connected");
                    return 10;
                case 201:
                    ret = _T("NX Server not installed or the server access has been disabled");
                    return -99;
                case 203:
                    i = tmp.Find(_T("pid: "));
                    if (i >= 0) {
                        tmp.Mid(i+5).ToLong(&n);
                        m_iSshPid = n;
                    }   
                    return -1;
                case 204:
                    ret = _T("NX Server has refused the connection");
                    return -99;
                case 205:
                    m_iLineCount = 2;
                    m_sMessage = tmp.Mid(8);
                    return -1;
                case 300:
                case 301:
                    ret = _T("Connection error");
                    return -99;
                case 302:
                case 303:
                    ret = _T("Connection refused");
                    return -99;
                case 304:
                case 305:
                    ret = _T("Host not found");
                    return -99;
                case 306:
                    ret = _T("Network unreachable");
                    return -99;
                case 307:
                case 310:
                    ret = _T("Host key verification failed");
                    return -99;
                case 999:
                    // Terminated??
                    ret = _T("??terminated??");
                    return -99;
                default:
                    if (m_iLineCount) {
                        m_iLineCount--;
                        m_sMessage += _T("\n");
                        m_sMessage += tmp;
                        if (m_iLineCount == 0) {
                            m_re->Replace(&m_sMessage, _T(""));
                            ret = m_sMessage + _T("?");
                            return -3;
                        }
                        return -1;
                    }
                    if (ccode >= 400 && ccode <= 699) {
                        ret = tmp;
                        return -99;
                    }
                    return -1;
            }
        }
        return -2;
    }
    return -1;
}
