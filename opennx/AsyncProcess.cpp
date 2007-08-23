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

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma implementation "AsyncProcess.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <wx/filename.h>
#include <wx/txtstrm.h>

#include "AsyncProcess.h"

static wxString MYTRACETAG(wxFileName::FileName(wxT(__FILE__)).GetName());

DEFINE_EVENT_TYPE(wxEVT_PROCESS_STDOUT);
DEFINE_EVENT_TYPE(wxEVT_PROCESS_STDERR);
DEFINE_EVENT_TYPE(wxEVT_PROCESS_EXIT);

IMPLEMENT_DYNAMIC_CLASS(AsyncProcess, wxProcess)

AsyncProcess::AsyncProcess()
    : wxProcess(),
    wxThreadHelper(),
    m_pEvtHandler(NULL)
{
    Redirect();
}

AsyncProcess::AsyncProcess(const wxString& cmd, const wxString &wdir, wxEvtHandler *h /* = NULL */)
    : wxProcess(),
    wxThreadHelper(),
    m_pEvtHandler(h),
    m_sCmd(cmd),
    m_sDir(wdir)
{
    Redirect();
}

AsyncProcess::AsyncProcess(const wxString& cmd, wxEvtHandler *h /* = NULL */)
    : wxProcess(),
    wxThreadHelper(),
    m_pEvtHandler(h),
    m_sCmd(cmd),
    m_sDir(::wxGetCwd())
{
    Redirect();
}

AsyncProcess::~AsyncProcess()
{
    ::wxLogTrace(MYTRACETAG, wxT("~AsyncProcess"));
    if (m_thread && m_thread->IsRunning()) {
        m_thread->Delete();
        while (m_thread->IsRunning())
            wxThread::Sleep(100);
        m_thread = NULL;
    }
    m_pEvtHandler = NULL;
}

    wxThread::ExitCode
AsyncProcess::Entry()
{
    ::wxLogTrace(MYTRACETAG, wxT("IoThread starting"));
    while (!m_thread->TestDestroy()) {
        while (IsInputAvailable()) {
            char c = GetInputStream()->GetC();
            if (m_pEvtHandler) {
                m_cOutWatch.Start();
                switch (c) {
                    case '\r':
                        break;
                    case '\n':
                        {
                            wxCommandEvent event(wxEVT_PROCESS_STDOUT, wxID_ANY);
                            event.SetString(m_sOutBuf);
                            event.SetClientData(this);
                            m_pEvtHandler->AddPendingEvent(event);
                            m_sOutBuf.Empty();
                        }
                        break;
                    default:
                        m_sOutBuf += c;
                        break;
                }
            }
        }
        if (m_pEvtHandler) {
            // If no LF received within a second, send buffer anyway
            if ((m_cOutWatch.Time() > 1000) && (!m_sOutBuf.IsEmpty())) {
                ::wxLogTrace(MYTRACETAG, wxT("IoThread outwatch timed out"));
                wxCommandEvent event(wxEVT_PROCESS_STDOUT, wxID_ANY);
                event.SetString(m_sOutBuf);
                event.SetClientData(this);
                m_pEvtHandler->AddPendingEvent(event);
                m_sOutBuf.Empty();
            }
        }
        while (IsErrorAvailable()) {
            char c = GetErrorStream()->GetC();
            if (m_pEvtHandler) {
                m_cErrWatch.Start();
                switch (c) {
                    case '\r':
                        break;
                    case '\n':
                        {
                            wxCommandEvent event(wxEVT_PROCESS_STDERR, wxID_ANY);
                            event.SetString(m_sErrBuf);
                            event.SetClientData(this);
                            m_pEvtHandler->AddPendingEvent(event);
                            m_sErrBuf.Empty();
                        }
                        break;
                    default:
                        m_sErrBuf += c;
                        break;
                }
            }
        }
        if (m_pEvtHandler) {
            // If no LF received within a second, send buffer anyway
            if ((m_cErrWatch.Time() > 1000) && (!m_sErrBuf.IsEmpty())) {
                ::wxLogTrace(MYTRACETAG, wxT("IoThread errwatch timed out"));
                wxCommandEvent event(wxEVT_PROCESS_STDERR, wxID_ANY);
                event.SetString(m_sErrBuf);
                event.SetClientData(this);
                m_pEvtHandler->AddPendingEvent(event);
                m_sErrBuf.Empty();
            }
        }
        wxThread::Sleep(10);
    }
    ::wxLogTrace(MYTRACETAG, wxT("IoThread exiting"));
    return 0;
}

    bool
AsyncProcess::IsRunning() {
    bool ret = ((m_pid > 0) && Exists(m_pid));
    //::wxLogTrace(MYTRACETAG, wxT("IsRunning returning %d"), ret);
    return ret;
}

    bool
AsyncProcess::Print(const wxString &s, bool doLog)
{
    wxOutputStream *os = GetOutputStream();
    if (os) {
        if (doLog)
            ::wxLogTrace(MYTRACETAG, wxT("Sending: '%s'"), s.c_str());
        else
            ::wxLogTrace(MYTRACETAG, wxT("Sending (hidden): '************'"));
        wxTextOutputStream tos(*os);
        tos << s << wxT("\n");
        return true;
    }
    return false;
}

    void
AsyncProcess::OnTerminate(int pid, int status)
{
    ::wxLogTrace(MYTRACETAG, wxT("Process %u terminated with exit code %d."), pid, status);
    wxLog::FlushActive();
    m_iStatus = status;
    if (m_thread && m_thread->IsRunning()) {
        while ((m_cOutWatch.Time() < 2000) || (m_cErrWatch.Time() < 2000))
            wxThread::Sleep(100);
        m_thread->Delete();
        while (m_thread->IsRunning())
            wxThread::Sleep(100);
        m_thread = NULL;
    }
    if (m_pEvtHandler) {
        wxCommandEvent event(wxEVT_PROCESS_EXIT, wxID_ANY);
        event.SetInt(m_iStatus);
        event.SetClientData(this);
        m_pEvtHandler->AddPendingEvent(event);
    }
}

    bool
AsyncProcess::Start()
{
    bool ret = false;

    if (!m_sCmd.IsEmpty()) {
        wxString cwd = ::wxGetCwd();
        ::wxLogTrace(MYTRACETAG, wxT("Starting '%s'"), m_sCmd.c_str());
        if (!m_sDir.IsEmpty())
            wxFileName::SetCwd(m_sDir);
        m_sOutBuf.Empty();
        m_sErrBuf.Empty();
        m_pid = ::wxExecute(m_sCmd, wxEXEC_ASYNC, this);
        ::wxLogTrace(MYTRACETAG, wxT("wxExecute returned %d"), m_pid);
        ret = (m_pid > 0);
        if (!m_sDir.IsEmpty())
            wxFileName::SetCwd(cwd);
        if (ret) {
            ::wxLogTrace(MYTRACETAG, wxT(">Create()"));
            if (IsRunning())
                Create();
            ::wxLogTrace(MYTRACETAG, wxT("<Create()"));
            if (IsRunning())
                m_thread->Run();
            ::wxLogTrace(MYTRACETAG, wxT("Run()"));
        }
    }
    return ret;
}

    void
AsyncProcess::Detach()
{
    ::wxLogTrace(MYTRACETAG, wxT("Detach() called"));
    m_pEvtHandler = NULL;
    wxProcess::Detach();
    if (m_thread && m_thread->IsRunning()) {
        m_thread->Delete();
        while (m_thread->IsRunning())
            wxThread::Sleep(100);
        m_thread = NULL;
    }
}

    bool
AsyncProcess::Kill()
{
    ::wxLogTrace(MYTRACETAG, wxT("Kill() called"));
    bool ret = false;
    Detach();
    if (GetPid() > 0) {
        switch (wxProcess::Kill(GetPid(), wxSIGKILL)) {
            case wxKILL_OK:
            case wxKILL_NO_PROCESS:
                ret = true;
                break;
            case wxKILL_BAD_SIGNAL:
            case wxKILL_ACCESS_DENIED:
            case wxKILL_ERROR:
                break;
        }
    }
    return ret;
}
