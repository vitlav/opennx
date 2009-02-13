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

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma implementation "SessionList.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "ConnectDialog.h"
#include "MySession.h"
#include "MyXmlConfig.h"
#include "MyIPC.h"
#include "ResumeDialog.h"
#include "WinShare.h"
#include "opennxApp.h"
#include "osdep.h"
#include "pwcrypt.h"

#include <wx/filename.h>
#include <wx/regex.h>
#include <wx/wfstream.h>
#include <wx/txtstrm.h>
#include <wx/socket.h>
#include <wx/config.h>
#include <wx/utils.h>
#include <wx/tokenzr.h>
#include <wx/regex.h>
#include <wx/protocol/http.h>
#include <wx/sckstrm.h>
#include <wx/dir.h>
#include <wx/process.h>
#include <wx/textfile.h>
#ifdef __VISUALC__
# include <fstream.h>
#else
# include <fstream>
#endif
#ifndef __WXMSW__
# include <grp.h>
#endif

#include "trace.h"
ENABLE_TRACE;

static const int CUPS_PORT_OFFSET  = 2000;
static const int SMB_PORT_OFFSET   = 3000;
static const int PROXY_PORT_OFFSET = 4000;
static const int X_PORT_OFFSET     = 6000;
static const int SOUND_PORT_OFFSET = 7000;
static const int KBD_PORT_OFFSET   = 8000;

#define NX_PROTO wxT(NX_PROTOCOL_VERSION)

#ifdef __WXMSW__
wxString cygPath(const wxString &dir, const wxString &file = wxEmptyString)
{
    wxFileName fn(dir);
    wxString ret = wxT("/cygdrive/");
    ret += fn.GetVolume().Lower();
    ret += fn.GetShortPath().AfterFirst(wxT(':'));
    if (!file.IsEmpty())
        ret << wxT("\\") << file;
    ret.Replace(wxT("\\"), wxT("/"));
    return ret;
}

class FontpathTraverser : public wxDirTraverser
{
    public:
        wxString GetFontPath() { return m_sFontPath; }

        virtual wxDirTraverseResult OnFile(const wxString& WXUNUSED(filename))
        {
            return wxDIR_CONTINUE;
        }

        virtual wxDirTraverseResult OnDir(const wxString& dirname)
        {
            if (!m_sFontPath.IsEmpty())
                m_sFontPath += wxT(",");
            m_sFontPath += cygPath(dirname);
            return wxDIR_IGNORE;
        }

    private:
        wxString m_sFontPath;

};
#endif

class RunLog : public wxLogChain
{
    public:
        RunLog(wxLog *logger) :wxLogChain(logger) { }

        void DoLog(wxLogLevel level, const wxChar *szString, time_t t)
        {
            PassMessages(level <= wxLOG_Message);
            wxLogChain::DoLog((level > wxLOG_Message) ? wxLOG_Message : level, szString, t);
        }
};

class SessionCleaner : public wxDirTraverser
{
    public:
        SessionCleaner(const wxString &toplevel)
        {
            m_sTopLevel = toplevel;
            wxString s = wxFileName::GetPathSeparator();
            if (!toplevel.EndsWith(s))
                m_sTopLevel += s;
            wxString r = m_sTopLevel + wxT("temp") + s + wxT("([0-9]+)");
            r.Replace(wxT("\\"), wxT("\\\\"));
            m_cRegex.Compile(r, wxRE_ADVANCED);
            wxASSERT(m_cRegex.IsValid());
        }

        ~SessionCleaner()
        {
            int n = m_aFiles.GetCount() - 1;
            while (n >= 0)
                ::wxRemoveFile(m_aFiles[n--]);
            n = m_aDirs.GetCount() - 1;
            while (n >= 0)
                ::wxRmdir(m_aDirs[n--]);
        }

        virtual wxDirTraverseResult OnFile(const wxString &name)
        {
            for (size_t i = 0; i < m_aDirs.GetCount(); i++) {
                if (name.StartsWith(m_aDirs[i])) {
                    ::wxLogTrace(MYTRACETAG, wxT("adding file '%s'"), name.c_str());
                    m_aFiles.Add(name);
                    return wxDIR_CONTINUE;
                }
            }
            return wxDIR_CONTINUE;
        }

        virtual wxDirTraverseResult OnDir(const wxString &name)
        {
            if (name.StartsWith(m_sTopLevel + wxT("S-"))) {
                ::wxLogTrace(MYTRACETAG, wxT("Session dir: '%s'"), name.c_str());
                wxTextFile tf(name + wxFileName::GetPathSeparator() + wxT("session"));
                if (tf.Exists()) {
                    wxString line;
                    if (tf.Open()) {
                        for (line = tf.GetFirstLine(); !tf.Eof(); line = tf.GetNextLine()) {
                            if (line.Contains(wxT("mode with pid"))) {
                                long pid;
                                line.AfterFirst(wxT('\'')).BeforeLast(wxT('\'')).ToLong(&pid);
                                ::wxLogTrace(MYTRACETAG, wxT("Proxy-PID: %d"), pid);
                                if (!wxProcess::Exists(pid)) {
                                    ::wxLogTrace(MYTRACETAG, wxT("PID does not exist, adding '%s'"), name.c_str());
                                    m_aDirs.Add(name);
                                    return wxDIR_CONTINUE;
                                }
                            }
                        }
                    }
                }
                ::wxLogTrace(MYTRACETAG, wxT("Keeping '%s'"), name.c_str());
            }
            if (name.StartsWith(m_sTopLevel + wxT("D-"))) {
                ::wxLogTrace(MYTRACETAG, wxT("Service dir: '%s'"), name.c_str());
                wxTextFile tf(name + wxFileName::GetPathSeparator() + wxT("pid"));
                if (tf.Exists()) {
                    long pid;
                    if (tf.Open()) {
                        tf.GetFirstLine().ToLong(&pid);
                        ::wxLogTrace(MYTRACETAG, wxT("Service-PID: %d"), pid);
                        if (!wxProcess::Exists(pid)) {
                            ::wxLogTrace(MYTRACETAG, wxT("PID does not exist, adding '%s'"), name.c_str());
                            m_aDirs.Add(name);
                            return wxDIR_CONTINUE;
                        }
                    }
                }
                ::wxLogTrace(MYTRACETAG, wxT("Keeping '%s'"), name.c_str());
            }
            if (name.StartsWith(m_sTopLevel + wxT("F-"))) {
                ::wxLogTrace(MYTRACETAG, wxT("Failed session dir: '%s'"), name.c_str());
                m_aDirs.Add(name);
                return wxDIR_CONTINUE;
            }
            if (m_cRegex.Matches(name)) {
                ::wxLogTrace(MYTRACETAG, wxT("Temp dir: '%s'"), name.c_str());
                long mpid = 0;
                long spid = 0;
                m_cRegex.GetMatch(name, 1).ToLong(&mpid);
                ::wxLogTrace(MYTRACETAG, wxT("Main PID: %d"), mpid);
                wxTextFile tf(name + wxFileName::GetPathSeparator() + wxT("sshlog"));
                if (tf.Exists()) {
                    if (tf.Open()) {
                        wxString line = tf.GetFirstLine();
                        line.AfterLast(wxT(':')).Strip(wxString::both).ToLong(&spid);
                        ::wxLogTrace(MYTRACETAG, wxT("Ssh-PID: %d"), spid);
                    }
                }
                if (mpid && spid && (!wxProcess::Exists(mpid)) && (!wxProcess::Exists(spid))) {
                    ::wxLogTrace(MYTRACETAG, wxT("PIDs do not exist, adding '%s'"), name.c_str());
                    m_aDirs.Add(name);
                    return wxDIR_CONTINUE;
                }
                ::wxLogTrace(MYTRACETAG, wxT("Keeping '%s'"), name.c_str());
                return wxDIR_IGNORE;
            }
            if (name.StartsWith(m_sTopLevel + wxT("temp")))
                return wxDIR_CONTINUE;
            return wxDIR_IGNORE;
        }

    private:
        wxArrayString m_aDirs;
        wxArrayString m_aFiles;
        wxString m_sTopLevel;
        wxRegEx m_cRegex;
};

DECLARE_EVENT_TYPE(wxEVT_SESSION, -6);
DEFINE_EVENT_TYPE(wxEVT_SESSION);

/**
 * This class is a helper for watching a file until a specific
 * string has appeared in it. When found, it triggers an event.
 */
class SessionWatch : public wxThreadHelper
{
    public:
        SessionWatch(wxEvtHandler *handler, const wxString &logfile, const wxString &search)
            : wxThreadHelper()
        {
            m_pHandler = handler;
            m_sLog = logfile;
            m_sSearch = search;
            if (Create() == wxTHREAD_NO_ERROR)
                GetThread()->Run();
        }

    private:
        virtual wxThread::ExitCode Entry() {
            wxStopWatch sw;
            wxTextFile tf(m_sLog);
            long timeout = 5000;
            bool bFound = false;
            while ((!bFound) && (sw.Time() < timeout)) {
                if (tf.Exists()) {
                    if (timeout == 5000)
                        timeout = sw.Time() + 10000;
                    if (!tf.Open())
                        break;
                    wxString line;
                    for (line = tf.GetFirstLine(); !tf.Eof(); line = tf.GetNextLine()) {
                        if (line.Contains(m_sSearch)) {
                            bFound = true;
                            break;
                        }
                    }
                    tf.Close();
                }
                wxThread::Sleep(500);
            }
            if (m_pHandler) {
                wxCommandEvent upevent(wxEVT_SESSION, wxID_ANY);
                upevent.SetInt(bFound ? 1 : 0);
                m_pHandler->ProcessEvent(upevent);
            }
            return 0;
        }

        wxEvtHandler *m_pHandler;
        wxString m_sLog;
        wxString m_sSearch;
};

/**
 * Our specialization ow wxHTTP. Neded, because wxHTTP stops
 * parsing headers when recognizing an error status header (4xx, 5xx ...)
 * but we want it to continue parsing in order to get the Server: header.
 */
class MyHTTP : public wxHTTP {
    public:
        MyHTTP() : wxHTTP() {}
        wxInputStream *GetInputStream(const wxString& path)
        {
            wxSocketInputStream *inp_stream;
            wxString new_path;
            m_perr = wxPROTO_CONNERR;
            if (!m_addr)
                return NULL;

            // We set m_connected back to false so wxSocketBase will know what to do.
#ifdef __WXMAC__
            wxSocketClient::Connect(*m_addr , false );
            wxSocketClient::WaitOnConnect(10);

            if (!wxSocketClient::IsConnected())
                return NULL;
#else
            if (!wxProtocol::Connect(*m_addr))
                return NULL;
#endif

            if (!BuildRequest(path, m_post_buf.empty() ? wxHTTP_GET : wxHTTP_POST))
                return NULL;

            inp_stream = new wxSocketInputStream(*this);

            Notify(false);
            SetFlags(wxSOCKET_BLOCK | wxSOCKET_WAITALL);

            return inp_stream;
        }

    protected:
        bool BuildRequest(const wxString& path, wxHTTP_Req req)
        {
            bool ret = wxHTTP::BuildRequest(path, req);
            ::wxLogTrace(MYTRACETAG, wxT("calling ParseHeaders()"));
            ParseHeaders();
            return ret;
        }

};

IMPLEMENT_CLASS(MySession, wxEvtHandler);

BEGIN_EVENT_TABLE(MySession, wxEvtHandler)
    EVT_COMMAND(wxID_ANY, wxEVT_NXSSH, MySession::OnSshEvent)
    EVT_COMMAND(wxID_ANY, wxEVT_SESSION, MySession::OnSessionEvent)
    EVT_COMMAND(wxID_ANY, wxEVT_NXSERVICE, MySession::OnServiceEvent)
END_EVENT_TABLE();

MySession::MySession(wxString dir, wxString status, wxString stype, wxString host, int port, wxString md5)
    : m_pSshLog(NULL)
    , m_pNxSsh(NULL)
    , m_pCfg(NULL)
    , m_pDlg(NULL)
    , m_pParent(NULL)
    , m_pSessionWatch(NULL)
    , m_bTouched(true)
    , m_iPort(port)
    , m_lPid(0)
    , m_eSessionType(None)
    , m_eSessionStatus(Unknown)
    , m_sHost(host)
    , m_sMd5(md5)
    , m_sDir(dir)
{
    m_bValid = false;
    if (stype == wxT("C"))
        m_eSessionType = Server;
    if (stype == wxT("S"))
        m_eSessionType = Client;

    if (status == wxT("T"))
        m_eSessionStatus = Terminated;
    if (status == wxT("F"))
        m_eSessionStatus = Failed;

    m_rePID.Compile(wxT("([[:digit:]]+)"));

    CheckState();
}

MySession::MySession(const MySession &src)
    : m_pSshLog(NULL)
    , m_pNxSsh(NULL)
    , m_pCfg(NULL)
    , m_pDlg(NULL)
    , m_pParent(NULL)
    , m_pSessionWatch(NULL)
{
    m_sHost = src.m_sHost;
    m_iPort = src.m_iPort;
    m_lPid = src.m_lPid;
    m_sMd5 = src.m_sMd5;
    m_eSessionType = src.m_eSessionType;
    m_eSessionStatus = src.m_eSessionStatus;
    m_sDir = src.m_sDir;
    m_bTouched = src.m_bTouched;
    m_rePID.Compile(wxT("([[:digit:]]+)"));
}

MySession::MySession()
    : m_pSshLog(NULL)
    , m_pNxSsh(NULL)
    , m_pCfg(NULL)
    , m_pDlg(NULL)
    , m_pParent(NULL)
    , m_pSessionWatch(NULL)
{
}

MySession & MySession::operator =(const MySession &src)
{
    m_pSshLog = NULL;
    m_pCfg = NULL;
    m_pDlg = NULL;
    m_pNxSsh = NULL;
    m_pParent = NULL;
    m_pSessionWatch = NULL;
    m_sHost = src.m_sHost;
    m_iPort = src.m_iPort;
    m_lPid = src.m_lPid;
    m_sMd5 = src.m_sMd5;
    m_eSessionType = src.m_eSessionType;
    m_eSessionStatus = src.m_eSessionStatus;
    m_sDir = src.m_sDir;
    m_bTouched = src.m_bTouched;
    m_rePID.Compile(wxT("([[:digit:]]+)"));
    return *this;
}

MySession::~MySession()
{
    ::wxLogTrace(MYTRACETAG, wxT("~MySession"));
    if (m_pSshLog) {
        m_pSshLog->Flush();
        delete m_pSshLog;
    }
    m_pSshLog = NULL;
    if (m_pSessionWatch) {
        delete m_pSessionWatch;
        m_pSessionWatch = NULL;
    }
    delete wxLog::SetActiveTarget(NULL);
}

    wxString
MySession::sGetCreationTime()
{
    if (!m_bValid)
        return _("unknown");
    wxDateTime ctime;
    wxFileName fn(m_sDir, wxT("session"));
    fn.GetTimes(NULL, NULL, &ctime);
    return ctime.Format();
}

    bool
MySession::bGetPidFromFile()
{
    m_lPid = 0;
    if (!m_bValid)
       return false;
    wxFileInputStream input(m_sDir + wxFileName::GetPathSeparator() + wxT("session"));
    wxTextInputStream text(input);
    int cnt = 0;

    while ((!input.Eof()) && (cnt < 100)) {
        wxString line = text.ReadLine();
        int idx = line.Find(wxT("pid"));

        if (idx != -1 &&
                line.Find(wxT("NXAGENT")) == -1 &&
                line.Find(wxT("Agent")) == -1)
        {
            line = line.Mid(idx + 4);
            if (m_rePID.Matches(line))
                m_rePID.GetMatch(line, 1).ToLong(&m_lPid);
        }
        cnt++;
    }
    return (m_lPid > 0);
}

    void
MySession::CheckState()
{
    wxString logfilename = m_sDir + wxFileName::GetPathSeparator() + wxT("session");
    m_eSessionStatus = Unknown;

    if (!wxFile::Exists(logfilename))
        return;
    if ((m_eSessionStatus == Terminated) || (m_eSessionStatus == Failed))
        return;

    wxFile fi(logfilename);
    if (!fi.IsOpened())
        return;

    m_bValid = true;
    wxFileInputStream input(fi);

    wxTextInputStream text(input);
    while (!input.Eof()) {
        wxString line = text.ReadLine();
        if (line.StartsWith(wxT("Session: Session terminated"))) {
            m_eSessionStatus = Terminated;
            break;
        }

    }
    if (bGetPidFromFile()) {
        if (wxProcess::Exists(m_lPid))
            m_eSessionStatus = Running;
    }
}

    int
MySession::getFirstFreePort(int startPort)
{
    int port = startPort;
    wxIPV4address a;

    a.LocalHost();
    while (port < 65535) {
        a.Service(port);
        wxSocketServer ss(a);
        if (ss.Ok())
            return port;
        port++;
    }
    return 0;
}

    wxString
MySession::getXauthCookie(int display /* = 0 */)
{
#ifdef __UNIX__
    wxString dpy;
    if (wxGetEnv(wxT("DISPLAY"), &dpy) && (!dpy.IsEmpty())) {
        wxString cmd = wxT("xauth list :");
        cmd << dpy.AfterFirst(wxT(':'));
        wxArrayString clines;
        if (::wxExecute(cmd, clines) == 0) {
            size_t count = clines.GetCount();
            for (size_t i = 0; i < count; i++) {
                wxString line = clines[i];
                if (line.Contains(wxT("MIT-MAGIC-COOKIE-1"))) {
                    wxStringTokenizer t(line, wxT("\t "));
                    int ecount = 0;
                    while (t.HasMoreTokens()) {
                        wxString tok = t.GetNextToken();
                        if (++ecount == 3)
                            return tok;
                    }
                }
            }
        }
    }
#endif
#ifdef __WXMSW__
    // On windows we *create* the cookie instead
    wxString cookie = md5sum(::wxGetUserId() + wxDateTime::Now().Format());
    wxString domain = wxString::Format(wxT("%s:%d"),
            ::wxGetFullHostName().c_str(), display);
    wxFileName fn(m_sSysDir, wxEmptyString);
    fn.AppendDir(wxT("bin"));
    fn.SetName(wxT("nxauth.exe"));
    wxString cmd = fn.GetShortPath();
    cmd << wxT(" -i");
    cmd << wxT(" -f");
    cmd << wxT(" ") << getXauthPath();
    cmd << wxT(" add");
    cmd << wxT(" ") << domain;
    cmd << wxT(" MIT-MAGIC-COOKIE-1");
    cmd << wxT(" ") << cookie;
    return ::wxExecute(cmd) ? wxString() : cookie;
#endif
    return wxString();
}

    wxString
MySession::getXauthPath()
{
#ifdef __UNIX__
    wxString cmd = wxT("xauth info");
    wxArrayString clines;
    if (::wxExecute(cmd, clines) == 0) {
        size_t count = clines.GetCount();
        for (size_t i = 0; i < count; i++) {
            wxString line = clines[i];
            if (line.StartsWith(wxT("Authority file:"))) {
                wxStringTokenizer t(line, wxT(":"));
                int ecount = 0;
                while (t.HasMoreTokens()) {
                    wxString tok = t.GetNextToken();
                    if (++ecount == 2)
                        return tok.Strip(wxString::both);
                }
            }
        }
    }
#endif
#ifdef __WXMSW__
    return cygPath(m_sUserDir, wxT(".Xauthority"));
#endif
    return wxString();
}

wxString
MySession::formatOptFilename()
{
#ifdef __WXMSW__
    return cygPath(m_sOptFilename);
#else
    return m_sOptFilename;
#endif
}

    void
MySession::OnSessionEvent(wxCommandEvent &event)
{
    if (event.GetInt())
        m_bSessionEstablished = true;
    else
        m_bGotError = true;
}

    void
MySession::OnServiceEvent(wxCommandEvent &event)
{
    MyIPC::tServiceEvents e = wx_static_cast(MyIPC::tServiceEvents, event.GetInt());
    switch (e) {
        case MyIPC::ServiceTerminated:
            break;
        case MyIPC::ServiceEsdPort:
            event.GetString().AfterLast(wxT(' ')).ToLong(&m_lEsdPort);
            ::wxLogTrace(MYTRACETAG, wxT("esdport=%d"), m_lEsdPort);
            break;
        case MyIPC::ServiceEsdStarted:
            m_bEsdRunning = true;
            ::wxLogTrace(MYTRACETAG, wxT("esd started"));
            break;
        case MyIPC::ServiceEsdRunning:
            {
                wxString s = event.GetString();
                wxRegEx re(wxT("^\\d+\\s+audio\\s+(\\S+)\\s+is running"), wxRE_ADVANCED);
                wxASSERT(re.IsValid());
                if (re.Matches(s)) {
                    wxString portfile = m_sUserDir;
                    portfile << wxFileName::GetPathSeparator()
                        << re.GetMatch(s, 1)
                        << wxFileName::GetPathSeparator()
                        << wxT("port");
                    wxTextFile tf(portfile);
                    if (tf.Open()) {
                        tf.GetFirstLine().ToLong(&m_lEsdPort);
                        tf.Close();
                        ::wxLogTrace(MYTRACETAG, wxT("esdport=%d"), m_lEsdPort);
                    }
                }
            }
            m_bEsdRunning = true;
            break;
    }
}

    void
MySession::OnSshEvent(wxCommandEvent &event)
{
    MyIPC::tSessionEvents e = wx_static_cast(MyIPC::tSessionEvents, event.GetInt());
    wxString msg(event.GetString());
    wxString scmd;

    switch (e) {
        case MyIPC::ActionNone:
            break;
        case MyIPC::ActionStatus:
            m_pDlg->SetStatusText(msg);
            break;
        case MyIPC::ActionLog:
            m_pDlg->SetProgress(m_iProgress++);
            if (m_bCollectSessions)
                m_aParseBuffer.Add(msg);
            if (m_bCollectConfig) {
                m_sConfigBuffer << msg << wxT("\n");
                if (m_sConfigBuffer.Length() >= m_nSessionPushLength) {
                    ::wxLogInfo(wxT("session override finished"));
                    m_bCollectConfig = false;
                    m_pCfg->LoadFromString(m_sConfigBuffer, true);
                    if (m_pCfg->IsWritable())
                        m_pCfg->SaveToFile();
                }
            }
            if (m_pSshLog) {
                wxLog *oldLog = wxLog::SetActiveTarget(m_pSshLog);
                ::wxLogMessage(msg);
                wxLog::SetActiveTarget(oldLog);
            }
            break;
        case MyIPC::ActionWarning:
            ::wxLogWarning(msg);
            break;
        case MyIPC::ActionError:
            ::wxLogError(msg);
            m_bGotError = true;
            break;
        case MyIPC::ActionPromptYesNo:
            {
                wxMessageDialog d(m_pParent, msg,
                        _("Warning - OpenNX"), wxYES_NO|wxICON_EXCLAMATION);
                if (d.ShowModal() == wxID_YES)
                    printSsh(wxT("yes"));
                else {
                    printSsh(wxT("no"));
                    m_bGotError = true;
                }
            }
            break;
        case MyIPC::ActionKeyChangedYesNo:
            {
                msg << _("\nDo you want to delete the key and retry ?");
                wxMessageDialog d(m_pParent, msg,
                        _("Warning - OpenNX"), wxYES_NO|wxICON_EXCLAMATION);
                if (d.ShowModal() == wxID_YES)
                    m_bRemoveKey = true;
                m_bGotError = true;
            }
            break;
        case MyIPC::ActionOffendingKey:
            m_sOffendingKey = msg;
            break;
        case MyIPC::ActionSendUsername:
            {
                // At this point key-based auth is finished and
                // thus the key-file isn't needed anymore
                wxLogNull logdummy;
                wxFileName fn;
                fn.Assign(m_sTempDir, wxT("keylog"));
                ::wxRemoveFile(fn.GetFullPath());
            }
            m_pDlg->SetStatusText(_("Sending username"));
            printSsh(m_pCfg->sGetSessionUser());
            break;
        case MyIPC::ActionSendPassword:
            m_pDlg->SetStatusText(_("Authenticating"));
            if (m_pCfg->bGetRememberPassword())
                printSsh(m_pCfg->sGetSessionPassword(), false);
            else
                printSsh(m_sClearPassword, false);
            break;
        case MyIPC::ActionWelcome:
            m_pDlg->SetStatusText(_("Authentication successful"));
            m_eConnectState = STATE_LIST_SESSIONS;
            break;
        case MyIPC::ActionSessionPushLength:
            // Session file length: 213
            msg.Mid(21).ToULong(&m_nSessionPushLength);
            break;
        case MyIPC::ActionSessionPushStart:
            m_sConfigBuffer.Empty();
            m_bCollectConfig = true;
            ::wxLogInfo(wxT("receiving %d bytes of session override"),
                    m_nSessionPushLength);
            break;
        case MyIPC::ActionNextCommand:
            m_iProgress += 4;
            m_pDlg->SetProgress(m_iProgress);
            switch (m_eConnectState) {
                case STATE_HELLO:
                    m_pDlg->SetStatusText(_("Authenticating"));
                    printSsh(wxT("hello NXCLIENT - Version ") NX_PROTO);
                    m_eConnectState = STATE_SHELLMODE;
                    break;
                case STATE_SHELLMODE:
                    printSsh(wxT("SET SHELL_MODE SHELL"));
                    m_eConnectState = STATE_AUTHMODE;
                    break;
                case STATE_AUTHMODE:
                    printSsh(wxT("SET AUTH_MODE PASSWORD"));
                    m_eConnectState = STATE_LOGIN;
                    break;
                case STATE_LOGIN:
                    printSsh(wxT("login"));
                    break;
                case STATE_LIST_SESSIONS:
                    m_pDlg->SetStatusText(_("Query server-side sessions"));
                    scmd = wxT("listsession") + m_pCfg->sGetListParams(intver(NX_PROTO));
                    printSsh(scmd);
                    m_eConnectState = STATE_PARSE_SESSIONS;
                    break;
                case STATE_PARSE_SESSIONS:
                    printSsh(wxEmptyString);
                    break;
                case STATE_START_SESSION:
                    scmd = wxT("startsession ");
                    scmd << m_pCfg->sGetSessionParams(intver(NX_PROTO), true, m_sClearPassword);
                    printSsh(scmd);
                    m_eConnectState = STATE_FINISH;
                    break;
                case STATE_RESUME_SESSION:
                    scmd = wxT("restoresession ");
                    scmd << m_pCfg->sGetSessionParams(intver(NX_PROTO), false, m_sClearPassword)
                        << wxT(" --session=\"") << m_sResumeName
                        << wxT("\" --type=\"") << m_sResumeType
                        << wxT("\" --id=\"") << m_sResumeId << wxT("\"");
                    printSsh(scmd);
                    m_eConnectState = STATE_FINISH;
                    break;
                case STATE_FINISH:
                    break;
            }
            break;
        case MyIPC::ActionPinDialog:
            printSsh(wxGetPasswordFromUser(
                        _("Enter PIN for Smart Card access."), _("Smart Card PIN")), false);
            break;
        case MyIPC::ActionPassphraseDialog:
            scmd = ::wxGetPasswordFromUser(::wxGetTranslation(msg), _("Enter passphrase"));
            if (scmd.IsEmpty()) {
                msg = _("Empty passphrase");
                m_bGotError = true;
            }
            printSsh(scmd, false);
            break;
        case MyIPC::ActionSetSessionID:
            m_pDlg->SetStatusText(_("Negotiating session parameter"));
            m_sSessionID = msg;
            break;
        case MyIPC::ActionSetProxyCookie:
            m_pDlg->SetStatusText(_("Negotiating session parameter"));
            m_sProxyCookie = msg;
            break;
        case MyIPC::ActionSetProxyIP:
            m_pDlg->SetStatusText(_("Negotiating session parameter"));
            m_sProxyIP = msg;
            break;
        case MyIPC::ActionSetSessionType:
            m_pDlg->SetStatusText(_("Negotiating session parameter"));
            m_sSessionType = msg;
            break;
        case MyIPC::ActionSetSessionCache:
            m_pDlg->SetStatusText(_("Negotiating session parameter"));
            m_sSessionCache = msg;
            break;
        case MyIPC::ActionSetSessionDisplay:
            m_pDlg->SetStatusText(_("Negotiating session parameter"));
            m_sSessionDisplay = msg;
            break;
        case MyIPC::ActionSetAgentCookie:
            m_pDlg->SetStatusText(_("Negotiating session parameter"));
            m_sAgentCookie = msg;
            break;
        case MyIPC::ActionSetSslTunneling:
            m_pDlg->SetStatusText(_("Negotiating session parameter"));
            m_bSslTunneling = (msg == wxT("1"));
            break;
        case MyIPC::ActionSetSubscription:
            m_pDlg->SetStatusText(_("Negotiating session parameter"));
            m_sSubscription = msg;
            break;
        case MyIPC::ActionSetSmbPort:
            m_pDlg->SetStatusText(_("Negotiating session parameter"));
            m_sSmbPort = msg;
            break;
        case MyIPC::ActionExit:
            if (m_eConnectState == STATE_FINISH) {
                m_pDlg->SetStatusText(_("Starting session"));
                msg = wxT("NX> 299 Switch connection to: ");
                if (intver(NX_PROTO) > 0x00020000) {
                    msg << wxT("NX mode: encrypted options: nx,options=")
                        << formatOptFilename() << wxT(":") << m_sSessionDisplay;
                } else {
                    msg << m_sProxyIP << wxT(":") << m_sProxyPort
                        << wxT(" cookie: ") << m_sProxyCookie;
                }
                printSsh(msg);
            } else {
                m_bSessionRunning = true;
            }
            break;
        case MyIPC::ActionTerminated:
            if ((m_eConnectState <= STATE_PARSE_SESSIONS) && (!m_bGotError) && (!m_bRemoveKey) && (m_sOffendingKey.IsEmpty())) {
                msg = _("Unexpected termination of nxssh");
                ::wxLogError(msg);
                m_bGotError = true;
            }
            break;
        case MyIPC::ActionStartProxy:
            if (m_eConnectState == STATE_FINISH) {
                m_pDlg->SetStatusText(_("Starting session"));
                startSharing();
                startProxy();
            }
            break;
        case MyIPC::ActionSessionRunning:
            m_bSessionRunning = true;
            {
                wxString slog = m_sSessionDir;
                slog << wxFileName::GetPathSeparator() << wxT("session");
                m_pSessionWatch = new SessionWatch(this, slog,
                        wxT("Session: Session started at"));
            }
            break;
        case MyIPC::ActionSessionListStart:
            // Server starts sending session list
            ::wxLogInfo(wxT("receiving session list .."));
            m_aParseBuffer.Empty();
            m_bCollectSessions = true;
            break;
        case MyIPC::ActionSessionListEnd:
            // Server has sent list of running & suspended sessions
            m_bCollectSessions = false;
            ::wxLogInfo(wxT("received end of session list"));
            parseSessions(event.GetExtraLong() == 148);
            break;
    }
}

long
MySession::intver(const wxString &ver)
{
    long ret = 0;
    wxStringTokenizer t(ver, wxT("."));
    while (t.HasMoreTokens()) {
        long n;
        t.GetNextToken().ToLong(&n);
        ret = (ret << 8) + n;
    }
    ::wxLogTrace(MYTRACETAG, wxT("protocol version: %08x"), ret);
    return ret;
}

    void
MySession::printSsh(const wxString &s, bool doLog /* = true */)
{
    if (m_pNxSsh) {
        ::wxLogTrace(MYTRACETAG, wxT("sending '%s'"), (doLog ? s.c_str() : wxT("********")));
        m_pNxSsh->Print(s, doLog);
    }
}

    void
MySession::parseSessions(bool moreAllowed)
{
    size_t n = m_aParseBuffer.GetCount();
    wxRegEx re(
            wxT("^(\\d+)\\s+([a-z-]+)\\s+([0-9A-F]{32})\\s+([A-Z-]{8})\\s+(\\d+)\\s+(\\d+x\\d+)\\s+(\\w+)\\s+(\\w+)\\s*"),
            wxRE_ADVANCED);
    wxASSERT(re.IsValid());
    ResumeDialog d(NULL);
    bool bFound = false;
    d.SetPreferredSession(m_pCfg->sGetName());
    for (size_t i = 0; i < n; i++) {
        wxString line = m_aParseBuffer[i];
        if (re.Matches(line) && (re.GetMatchCount() == 9)) {
            wxString sPort(re.GetMatch(line, 1));
            wxString sType(re.GetMatch(line, 2));
            wxString sId(re.GetMatch(line, 3));
            wxString sOpts(re.GetMatch(line, 4));
            wxString sColors(re.GetMatch(line, 5));
            wxString sSize(re.GetMatch(line, 6));
            wxString sState(re.GetMatch(line, 7));
            wxString sName(re.GetMatch(line, 8));
            d.AddSession(sName, sState, sType, sSize, sColors, sPort, sOpts, sId);
            bFound = true;
        }
    }
    if (bFound) {
        d.EnableNew(moreAllowed);
        switch (d.ShowModal()) {
            case wxID_OK:
                switch (d.GetMode()) {
                    case ResumeDialog::Resume:
                        wxLogInfo(wxT("RESUME"));
                        m_sResumeName = d.GetSelectedName();
                        m_sResumeType = d.GetSelectedType();
                        m_sResumeId = d.GetSelectedId();
                        m_eConnectState = STATE_RESUME_SESSION;
                        break;
                    case ResumeDialog::Takeover:
                        wxLogInfo(wxT("TAKEOVER"));
                        m_sResumeName = d.GetSelectedName();
                        m_sResumeType = d.GetSelectedType();
                        m_sResumeId = d.GetSelectedId();
                        m_eConnectState = STATE_RESUME_SESSION;
                        break;
                    case ResumeDialog::New:
                        m_eConnectState = STATE_START_SESSION;
                        break;
                }
                break;
            case wxID_CANCEL:
                printSsh(wxT("bye"));
                break;
        }
    } else {
        if (moreAllowed)
            m_eConnectState = STATE_START_SESSION;
        else {
            printSsh(wxT("bye"));
            wxMessageDialog d(m_pParent,
                    _("You have reached your session limit. No more sessions allowed"),
                    _("Error - OpenNX"), wxOK);
            d.ShowModal();
            m_bGotError = true;
        }
    }
}

    void
MySession::startSharing()
{
    ArrayOfShareGroups sg = m_pCfg->aGetShareGroups();
    wxArrayString used = m_pCfg->aGetUsedShareGroups();
    CupsClient cc;
    SmbClient sc;
    ArrayOfShares sa;
    if (cc.IsAvailable())
        sa = cc.GetShares();
    if (sc.IsAvailable())
        WX_APPEND_ARRAY(sa, sc.GetShares());

    long cupsport;
    long smbport;
    m_sSessionDisplay.ToLong(&cupsport);
    m_sSessionDisplay.ToLong(&smbport);
    cupsport += CUPS_PORT_OFFSET;
    smbport += SMB_PORT_OFFSET;
    wxString cupspw = decodeString(wxConfigBase::Get()->Read(wxT("Config/CupsPasswd"), wxEmptyString));
    for (size_t i = 0; i < sg.GetCount(); i++) {
        if (used.Index(sg[i].m_sGroupName) == wxNOT_FOUND)
            continue;
        bool bAvailable = false;
        wxString sn = sg[i].m_sShareName;
        ::wxLogTrace(MYTRACETAG, wxT("startSharing: considering share '%s'"), sn.c_str());
        for (size_t j = 0; j < sa.GetCount(); j++) {
            if ((sa[j].sharetype == sg[i].m_eType) && (sa[j].name == sn)) {
                bAvailable = true;
                break;
            }
        }
        ::wxLogTrace(MYTRACETAG, wxT("'%s' is %savailable"), sn.c_str(),
                (bAvailable ? wxEmptyString : wxT("not ")));
        if (!bAvailable)
            continue;
        wxString shcmd;
        switch (sg[i].m_eType) {
            case SharedResource::SHARE_UNKNOWN:
                break;
            case SharedResource::SHARE_SMB_DISK:
                shcmd = wxT("addmount");
                shcmd
                   << wxT(" --port=\"") << smbport << wxT("\"") 
                   << wxT(" --username=\"")
                   << MyXmlConfig::UrlEsc(sg[i].m_sUsername) << wxT("\"") 
                   << wxT(" --password=\"")
                   << MyXmlConfig::UrlEsc(sg[i].m_sPassword) << wxT("\"") 
                   << wxT(" --share=\"") << MyXmlConfig::UrlEsc(sn) << wxT("\"") 
                   << wxT(" --computername=\"")
                   << MyXmlConfig::UrlEsc(::wxGetFullHostName()) << wxT("\"") 
                   << wxT(" --session_id=\"") << m_sSessionID.Right(32) << wxT("\"") 
                   << wxT(" --dir=\"")
                   << MyXmlConfig::UrlEsc(sg[i].m_sAlias) << wxT("\"");
                printSsh(shcmd);
                break;
            case SharedResource::SHARE_SMB_PRINTER:
                shcmd = wxT("addprinter");
                shcmd << wxT(" --type=\"smb\"")
                   << wxT(" --username=\"")
                   << MyXmlConfig::UrlEsc(sg[i].m_sUsername) << wxT("\"") 
                   << wxT(" --password=\"")
                   << MyXmlConfig::UrlEsc(sg[i].m_sPassword) << wxT("\"") 
                   << wxT(" --port=\"") << (int)smbport << wxT("\"") 
                   << wxT(" --share=\"") << MyXmlConfig::UrlEsc(sn) << wxT("\"") 
                   << wxT(" --computername=\"")
                   << MyXmlConfig::UrlEsc(::wxGetHostName()) << wxT("\"") 
                   << wxT(" --session_id=\"") << m_sSessionID.Right(32) << wxT("\"") 
                   << wxT(" --model=\"") << sg[i].m_sDriver << wxT("\"");
                if (sg[i].m_bDefault)
                    shcmd << wxT(" --defaultprinter=\"1\"");
                if (sg[i].m_bPublic)
                    shcmd << wxT(" --public=\"1\"");
                printSsh(shcmd);
                break;
            case SharedResource::SHARE_CUPS_PRINTER:
                shcmd = wxT("addprinter");
                shcmd << wxT(" --type=\"ipp\"")
                   << wxT(" --username=\"") << MyXmlConfig::UrlEsc(sg[i].m_sUsername) << wxT("\"") 
                   << wxT(" --port=\"") << cupsport << wxT("\"") 
                   << wxT(" --session_id=\"") << m_sSessionID.Right(32) << wxT("\"") 
                   << wxT(" --printer=\"") << MyXmlConfig::UrlEsc(sn) << wxT("\"") 
                   << wxT(" --password=\"") << cupspw << wxT("\"")
                   << wxT(" --model=\"cups%20printer\"");
                if (sg[i].m_bDefault)
                    shcmd << wxT(" --defaultprinter=\"1\"");
                if (sg[i].m_bPublic)
                    shcmd << wxT(" --public=\"1\"");
                printSsh(shcmd);
                break;
        }
    }
}

    void
MySession::startXserver()
{
#ifdef __WXMSW__
    int display = getFirstFreePort(X_PORT_OFFSET);
::wxLogTrace(MYTRACETAG, wxT("startXServer first free port is %d"), display);
    if (0 == display) {
#warning error-handling
        return;
    }
    display -= X_PORT_OFFSET;
    wxString dpyStr = wxT(":");
    dpyStr << display;
    m_sXauthCookie = getXauthCookie(display);

    wxFileName fn(m_sSysDir, wxT("nxwin.exe"));
    fn.AppendDir(wxT("bin"));

    wxString wxWinCmd = fn.GetShortPath();
    wxWinCmd << wxT(" -nowinkill");
    wxWinCmd << wxT(" -clipboard");
    wxWinCmd << wxT(" -noloadxkb");
    wxWinCmd << wxT(" -agent");
    //wxWinCmd << wxT(" -hide");
    wxWinCmd << wxT(" -noreset");
    wxWinCmd << wxT(" -nolisten tcp");
    wxWinCmd << wxT(" -auth ") << getXauthPath();

    {
        wxLogNull l;
        fn.AssignDir(m_sSysDir);
        fn.AppendDir(wxT("share"));
        fn.AppendDir(wxT("fonts"));
        wxDir d(fn.GetShortPath());
        FontpathTraverser t;
        d.Traverse(t);
        wxWinCmd << wxT(" -fp ") << t.GetFontPath();
    }

    wxWinCmd << wxT(" -screen 0 800x600");

    wxString user;
    if (m_pCfg->bGetGuestMode()) {
        user = m_pCfg->sGetGuestUser();
        if (user.IsEmpty())
            user << wxT("guest");
    } else
        user = m_pCfg->sGetUsername();
    wxWinCmd << wxT(" -name ") << user << wxT("@") << m_pCfg->sGetServerHost() << wxT(":");
    // wxWinCmd << wxT(" -nokeyhook");

    wxWinCmd << wxT(" ") << dpyStr;
::wxLogTrace(MYTRACETAG, wxT("startXServer executing %s"), wxWinCmd.c_str());
    ::wxExecute(wxWinCmd, wxEXEC_ASYNC);
#endif
}

    void
MySession::startProxy()
{
    ::wxLogTrace(MYTRACETAG, wxT("MySession::startProxy() called"));
    long cupsport = wxConfigBase::Get()->Read(wxT("Config/CupsPort"), -1);
    wxString popts;
    popts << wxT("nx,cookie=") << m_sProxyCookie
        << wxT(",root=") << m_sUserDir
        << m_pCfg->sGetProxyParams(intver(NX_PROTO))
        << wxT(",product=") << m_sSubscription;
    if (!m_sSmbPort.IsEmpty())
        popts << wxT(",samba=") << m_sSmbPort;
    if ((getActiveCupsPrinters().GetCount() > 0) && (isCupsRunning()))
        popts << wxT(",cups=") << cupsport;
    if (m_lEsdPort != 0)
        popts << wxT(",media=") << m_lEsdPort;
    popts
        << wxT(",encryption=") << (m_bSslTunneling ? 1 : 0)
        << wxT(",session=session")
#ifdef __UNIX__
        << wxT(",client=linux")
#else
        << wxT(",client=winnt")
#endif
        << wxT(",id=") << m_sSessionID;
    if (intver(NX_PROTO) <= 0x00020000) {
        if (m_bSslTunneling) {
            m_sProxyIP = wxT("127.0.0.1");
            m_sProxyPort = wxString::Format(wxT("%d"), getFirstFreePort(PROXY_PORT_OFFSET));
            popts << wxT(",listen=") << m_sProxyPort;
        } else {
            popts << wxT(",connect=") << m_sProxyIP;
        }
    }
    popts << wxT(":") << m_sSessionDisplay;
    m_sSessionDir = m_sUserDir;
    m_sSessionDir << wxFileName::GetPathSeparator()
        << wxT("S-") << m_sSessionID;
    {
        if (!wxFileName::Mkdir(m_sSessionDir, 0700, wxPATH_MKDIR_FULL)) {
            ::wxLogSysError(_("Could not create session directory\n%s\n"),
                    m_sSessionDir.c_str());
            m_bGotError = true;
        }
        m_sOptFilename = m_sSessionDir;
        m_sOptFilename << wxFileName::GetPathSeparator() << wxT("options");
        wxFile f;
        if (f.Open(m_sOptFilename, wxFile::write, wxS_IRUSR|wxS_IWUSR)) {
            f.Write(popts + wxT("\n"));
            f.Close();
            ::wxLogInfo(wxT("Session options='%s'\n"), popts.c_str());
            wxString pcmd;
            wxConfigBase::Get()->Read(wxT("Config/SystemNxDir"), &pcmd);
            pcmd << wxFileName::GetPathSeparator() << wxT("bin")
                << wxFileName::GetPathSeparator() << wxT("nxproxy -S nx,options=")
                << m_sOptFilename << wxT(":") << m_sSessionDisplay;
            printSsh(wxT("bye"));
            if (intver(NX_PROTO) <= 0x00020000)
                ::wxExecute(pcmd);
        } else {
            ::wxLogSysError(_("Could not write session options\n%s\n"),
                    m_sOptFilename.c_str());
            m_bGotError = true;
        }
    }
}

    ArrayOfShareGroups
MySession::getActiveCupsPrinters()
{
    ArrayOfShareGroups ret;
    CupsClient cc;
    if (cc.IsAvailable()) {
        ArrayOfShares sa = cc.GetShares();
        ret = m_pCfg->aGetShareGroups();
        wxArrayString used = m_pCfg->aGetUsedShareGroups();
        for (size_t i = ret.GetCount(); i > 0; i--) {
            size_t idx = i - 1;
            if (used.Index(ret[idx].m_sGroupName) == wxNOT_FOUND) {
                ::wxLogTrace(MYTRACETAG, wxT("removing[%d] '%s'"),
                        idx, ret[idx].m_sShareName.c_str());
                ret.RemoveAt(idx);
                continue;
            }
            if (ret[idx].m_eType != SharedResource::SHARE_CUPS_PRINTER) {
                ::wxLogTrace(MYTRACETAG, wxT("removing[%d] '%s'"),
                        idx, ret[idx].m_sShareName.c_str());
                ret.RemoveAt(idx);
                continue;
            }
            bool bAvailable = false;
            ::wxLogTrace(MYTRACETAG, wxT("Considering CUPS printer '%s' %d"),
                    ret[idx].m_sShareName.c_str(), idx);
            for (size_t j = 0; j < sa.GetCount(); j++) {
                if (sa[j].name == ret[idx].m_sShareName) {
                    bAvailable = true;
                    break;
                }
            }
            ::wxLogTrace(MYTRACETAG, wxT("'%s' is %savailable"),
                    ret[idx].m_sShareName.c_str(), (bAvailable ? wxEmptyString : wxT("not ")));
            if (!bAvailable) {
                ::wxLogTrace(MYTRACETAG, wxT("removing[%d] '%s'"),
                        idx, ret[idx].m_sShareName.c_str());
                ret.RemoveAt(idx);
            }
        }
    }
    ::wxLogTrace(MYTRACETAG, wxT("# of active printers: %d"), ret.GetCount());
    return ret;
}

    bool
MySession::isCupsRunning()
{
    if (m_bCupsRunning)
        return true;
    long cupsport = wxConfigBase::Get()->Read(wxT("Config/CupsPort"), -1);
    bool ret = false;
    if (cupsport > 0) {
        // Try connecting to cupsd
        MyHTTP http;
        http.Connect(wxT("127.0.0.1"), cupsport);
        wxInputStream *is = http.GetInputStream(wxT("/"));
        int res = http.GetResponse();
        wxString svr = http.GetHeader(wxT("server"));
        ::wxLogTrace(MYTRACETAG, wxT("isCupsRunning RC=%d SVR=%s"), res, svr.c_str());
        if (((res == 200) || (res == 401)) && svr.Contains(wxT("CUPS")))
            ret = true;
        delete is;
    }
    m_bCupsRunning = ret;
    ::wxLogTrace(MYTRACETAG, wxT("isCupsRunning returning %s"), (ret ? wxT("true") : wxT("false")));
    return ret;
}

    bool
MySession::prepareCups()
{
    long cupsport = wxConfigBase::Get()->Read(wxT("Config/CupsPort"), -1);
    if (cupsport == -1) {
        cupsport = getFirstFreePort(20000);
        wxConfigBase::Get()->Write(wxT("Config/CupsPort"), cupsport);
    }
    ::wxLogTrace(MYTRACETAG, wxT("Check for cupsd running at port %d"), cupsport);
    if (isCupsRunning())
        return true;

    wxString tmp;
    wxString sCupsDir = m_sUserDir;
    sCupsDir << wxFileName::GetPathSeparator() << wxT("cups");

    wxFileName::Mkdir(sCupsDir, 0700, wxPATH_MKDIR_FULL);
    sCupsDir << wxFileName::GetPathSeparator();
    wxFileName::Mkdir(sCupsDir + wxT("spool"), 0700, wxPATH_MKDIR_FULL);
    wxFileName::Mkdir(sCupsDir + wxT("certs"), 0700, wxPATH_MKDIR_FULL);
    wxFileName::Mkdir(sCupsDir + wxT("spool") +
            wxFileName::GetPathSeparator() + wxT("tmp"), 0700, wxPATH_MKDIR_FULL);
    {
        CupsClient cl;
        if (cl.IsAvailable()) {
            ArrayOfShares sa = cl.GetShares();
            if (sa.GetCount() > 0) {
                wxFileOutputStream fos(sCupsDir + wxT("printers.conf"));
                wxTextOutputStream tos(fos);
                for (size_t i = 0; i < sa.GetCount(); i++) {
                    tos << wxT("<Printer ") << sa[i].name << wxT(">") << endl;
                    tos << wxT("  Info ") << sa[i].description << endl;
                    tos << wxT("  Location local computer") << endl;
                    tos << wxT("  DeviceURI http://localhost:") << m_pCfg->iGetCupsPort()
                        << wxT("/printers/") << sa[i].name << endl;
                    tos << wxT("  State Idle") << endl;
                    tos << wxT("  Accepting Yes") << endl;
                    tos << wxT("  JobSheets none none") << endl;
                    tos << wxT("  QuotaPeriod 0") << endl;
                    tos << wxT("  PageLimit 0") << endl;
                    tos << wxT("  KLimit 0") << endl;
                    tos << wxT("</Printer>") << endl;
                }
            }
        } else
            return false;
    }

    tmp = m_sSysDir;
    tmp << wxFileName::GetPathSeparator() << wxT("share")
        << wxFileName::GetPathSeparator() << wxT("cups")
        << wxFileName::GetPathSeparator();
    ::wxCopyFile(tmp + wxT("mime.convs"), sCupsDir + wxT("mime.convs"));
    ::wxCopyFile(tmp + wxT("mime.types"), sCupsDir + wxT("mime.types"));
    {
        wxFileOutputStream fos(sCupsDir + wxT("cupsd.conf"));
        wxTextOutputStream tos(fos);
        tos << wxT("TempDir ") << sCupsDir << wxT("spool")
            << wxFileName::GetPathSeparator() << wxT("tmp") << endl;
        tos << wxT("RequestRoot ") << sCupsDir << wxT("spool") << endl;
        tos << wxT("ServerRoot ") << m_sUserDir
            << wxFileName::GetPathSeparator() << wxT("cups") << endl;
        tos << wxT("ErrorLog ") << sCupsDir << wxT("error_log") << endl;
        tos << wxT("LogLevel info") << endl;
        tos << wxT("Port ") << (int)cupsport << endl;
        tos << wxT("Browsing Off") << endl;
        tos << wxT("ServerName localhost") << endl;
        tos << wxT("ServerTokens full") << endl;
        tos << wxT("<Location />") << endl;
        tos << wxT("  Order Deny,Allow") << endl;
        tos << wxT("  Deny From All") << endl;
        tos << wxT("  Require user ") << ::wxGetUserId() << endl;
        tos << wxT("  AuthType Digest") << endl;
        tos << wxT("  Allow from 127.0.0.1") << endl;
        tos << wxT("</Location>") << endl;
    }
    {
        wxFileOutputStream fos(sCupsDir + wxT("passwd.md5"));
        wxTextOutputStream tos(fos);
#ifdef __WXMSW__
        tos << ::wxGetUserId() << wxT(":") << ::wxGetUserId();
#else
        struct group *g = getgrgid(getgid());
        wxString sGroupId = wxConvLocal.cMB2WX(g->gr_name);
        tos << ::wxGetUserId() << wxT(":") << sGroupId;
#endif
        wxDateTime now = wxDateTime::Now();
        wxString cupspw = ::wxGetUserId();
        cupspw << wxT("NX")
            << now.FormatISODate()
            << wxT("T")
            << now.FormatISOTime();
        cupspw = md5sum(cupspw).Left(16);
        wxConfigBase::Get()->Write(wxT("Config/CupsPasswd"), encodeString(cupspw));
        tos << wxT(":") << md5sum(::wxGetUserId()+ wxT(":CUPS:") + cupspw) << endl;
    }
    wxString cmd = m_pCfg->sGetCupsPath();
    cmd << wxT(" -c ") << sCupsDir << wxT("cupsd.conf");
    ::wxLogTrace(MYTRACETAG, wxT("Starting '%s'"), cmd.c_str());
    if (::wxExecute(cmd, wxEXEC_ASYNC|wxEXEC_MAKE_GROUP_LEADER) <= 0)
        return false;
    wxThread::Sleep(500);
    return isCupsRunning();
}

    void
MySession::cleanupOldSessions()
{
    wxDir ud;
    ::wxLogTrace(MYTRACETAG, wxT("Cleaning up old session data"));
    if (ud.Open(m_sUserDir)) {
        SessionCleaner sc(m_sUserDir);
        ud.Traverse(sc);
    }
}

    void
MySession::clearSshKeys(const wxString &keyloc)
{
    ::wxLogTrace(MYTRACETAG, wxT("Clearing keys for %s at %s"),
            m_pCfg->sGetServerHost().c_str(), keyloc.c_str());
    wxString keyfile = keyloc.BeforeLast(wxT(':'));
    long n;
    if (keyloc.AfterLast(wxT(':')).ToLong(&n)) {
        n--;
        wxTextFile tf(keyfile);
        if (tf.Exists()) {
            if (tf.Open()) {
                ::wxLogTrace(MYTRACETAG, wxT("Removing '%s'"), tf[n].c_str());
                tf.RemoveLine(n);
                wxIPV4address ip;
                if (ip.Hostname(m_pCfg->sGetServerHost())) {
                    wxString ipnum = ip.IPAddress();
                    wxString line;
                    for (line = tf.GetFirstLine(); !tf.Eof(); line = tf.GetNextLine()) {
                        if (line.Contains(ipnum)) {
                            ::wxLogTrace(MYTRACETAG, wxT("Removing '%s'"), line.c_str());
                            tf.RemoveLine(tf.GetCurrentLine());
                        }
                    }
                }
                tf.Write();
            }
        }
    }
}

    bool
MySession::Create(MyXmlConfig &cfgpar, const wxString password, wxWindow *parent)
{
    m_sClearPassword = password;
    m_bSessionRunning = false;
    m_bCupsRunning = false;
    m_bEsdRunning = false;
    m_lEsdPort = 0;
    m_bSessionEstablished = false;
    m_bCollectSessions = false;
    m_bCollectConfig = false;
    m_sSessionID = wxEmptyString;
    m_pParent = parent;
    MyXmlConfig cfg(cfgpar.sGetFileName());
    m_pCfg = &cfg;
    if (cfg.IsValid()) {
        // Copy misc values from login dialog
        m_pCfg->bSetUseSmartCard(cfgpar.bGetUseSmartCard());
        m_pCfg->bSetEnableSSL(cfgpar.bGetEnableSSL());
        m_pCfg->bSetGuestMode(cfgpar.bGetGuestMode());

        wxConfigBase::Get()->Read(wxT("Config/SystemNxDir"), &m_sSysDir);
        wxConfigBase::Get()->Read(wxT("Config/UserNxDir"), &m_sUserDir);
        wxFileName fn(m_sSysDir, wxEmptyString);

        fn.AppendDir(wxT("bin"));
#ifdef __WXMSW__
        fn.SetName(wxT("nxssh.exe"));
#else
        fn.SetName(wxT("nxssh"));
#endif
        wxString nxsshcmd = fn.GetShortPath();
        nxsshcmd << wxT(" -nx -x -2")
            << wxT(" -p ") << m_pCfg->iGetServerPort()
            << wxT(" -o 'RhostsAuthentication no'")
            << wxT(" -o 'PasswordAuthentication no'")
            << wxT(" -o 'RSAAuthentication no'")
            << wxT(" -o 'RhostsRSAAuthentication no'")
            << wxT(" -o 'PubkeyAuthentication yes'");
        m_sTempDir = m_sUserDir;
        m_sTempDir << wxFileName::GetPathSeparator() << wxT("temp")
            << wxFileName::GetPathSeparator() << ::wxGetProcessId();
        wxFileName::Mkdir(m_sTempDir, 0700, wxPATH_MKDIR_FULL);

        wxString logfn = m_sTempDir +
            wxFileName::GetPathSeparator() + wxT("runlog");
#ifdef __VISUALC__
        ofstream *log = new ofstream();
#else
        std::ofstream *log = new std::ofstream();
#endif
        log->open(logfn.mb_str());
        if (!wxLog::GetTraceMasks().GetCount())
            new RunLog(new wxLogStream(log));

        logfn = m_sTempDir +
            wxFileName::GetPathSeparator() + wxT("sshlog");
#ifdef __VISUALC__
        log = new ofstream();
#else
        log = new std::ofstream();
#endif
        log->open(logfn.mb_str());
        m_pSshLog = new wxLogStream(log);
        wxLog::SetLogLevel(wxLOG_Max);

        if (m_pCfg->bGetRemoveOldSessionFiles())
            cleanupOldSessions();

        if (m_pCfg->bGetUseSmartCard()) {
            nxsshcmd << wxT(" -I 0"); // We always use SC reader 0
        } else {
            if (m_pCfg->sGetSshKey().IsEmpty()) {
                fn.Assign(m_sSysDir, wxT("server.id_dsa.key"));
                fn.AppendDir(wxT("share"));
                fn.AppendDir(wxT("keys"));
            } else {
                fn.Assign(m_sTempDir, wxT("keylog"));
                wxFile f;
                ::wxRemoveFile(fn.GetFullPath());
                if (f.Open(fn.GetFullPath(), wxFile::write_excl, wxS_IRUSR|wxS_IWUSR)) {
                    f.Write(m_pCfg->sGetSshKey());
                    f.Close();
                } else {
                    ::wxLogSysError(_("Could not write %s"), fn.GetFullPath().c_str());
                    return false;
                }
            }
            nxsshcmd << wxT(" -i ") << fn.GetShortPath();
        }

        if (m_pCfg->bGetUseProxy()) {
            if (!m_pCfg->sGetProxyHost().IsEmpty())
                nxsshcmd << wxT(" -P ") << m_pCfg->sGetProxyHost() << wxT(":") << m_pCfg->iGetProxyPort();
            else if (!m_pCfg->sGetProxyCommand().IsEmpty())
                nxsshcmd << wxT(" -o 'ProxyCommand ") << m_pCfg->sGetProxyCommand() << wxT("'");
        }
        if (m_pCfg->bGetEnableSSL())
            nxsshcmd << wxT(" -B");
        nxsshcmd << wxT(" -E") << wxT(" nx@") << m_pCfg->sGetServerHost();

        fn.Assign(wxFileName::GetHomeDir());
        ::wxSetEnv(wxT("NX_HOME"), fn.GetShortPath());
        ::wxLogInfo(wxT("env: NX_HOME='%s'"), fn.GetShortPath().c_str());
        ::wxSetEnv(wxT("NX_ROOT"), m_sUserDir);
        ::wxLogInfo(wxT("env: NX_ROOT='%s'"), m_sUserDir.c_str());
        ::wxSetEnv(wxT("NX_SYSTEM"), m_sSysDir);
        ::wxLogInfo(wxT("env: NX_SYSTEM='%s'"), m_sSysDir.c_str());
        ::wxSetEnv(wxT("NX_CLIENT"), ::wxGetApp().GetSelfPath());
        ::wxLogInfo(wxT("env: NX_CLIENT='%s'"), ::wxGetApp().GetSelfPath().c_str());
        ::wxSetEnv(wxT("NX_VERSION"), NX_PROTO);
        ::wxLogInfo(wxT("env: NX_VERSION='") NX_PROTO wxT("'"));
        ::wxSetEnv(wxT("XAUTHORITY"), getXauthPath());
        ::wxLogInfo(wxT("env: XAUTHORITY='%s'"), getXauthPath().c_str());
#ifdef __UNIX__
        // NX needs TEMP or NX_TEMP to be set to the same dir
        // where .X11-unix resides (typically /tmp)
        wxString stmp = wxConvLocal.cMB2WX(x11_socket_path);
        if (!stmp.IsEmpty()) {
            fn.Assign(stmp);
            fn.RemoveLastDir();
            fn.SetName(wxEmptyString);
            ::wxSetEnv(wxT("NX_TEMP"), fn.GetShortPath());
            ::wxLogInfo(wxT("env: NX_TEMP='%s'"), fn.GetShortPath().c_str());
        } else {
            ::wxSetEnv(wxT("NX_TEMP"), wxT("/tmp"));
            ::wxLogInfo(wxT("env: NX_TEMP='/tmp'"));
        }
#else
        ::wxSetEnv(wxT("NX_TEMP"), cygPath(m_sTempDir));
        ::wxLogInfo(wxT("env: NX_TEMP='%s'"), cygPath(m_sTempDir).c_str());
#endif

#ifdef __WXMSW__
        startXserver();
#endif

        fn.Assign(m_sSysDir, wxT("bin"));
        m_iProgress = 0;
        ConnectDialog dlg(m_pParent);
        m_pDlg = &dlg;
        dlg.Show(true);
        dlg.SetStatusText(wxString::Format(_("Connecting to %s ..."),
                    m_pCfg->sGetServerHost().c_str()));

        if (m_pCfg->bGetEnableMultimedia()) {
            wxFileName fn(m_sSysDir, wxEmptyString);
            fn.AppendDir(wxT("bin"));
            fn.SetName(wxT("nxservice"));
            dlg.SetStatusText(_("Preparing multimedia service ..."));
            MyIPC ipc;
            if (ipc.ServiceProcess(fn.GetFullPath() + wxT(" --list audio"), this)) {
                while (ipc.IsRunning() && (!dlg.bGetAbort())) {
                    ::wxGetApp().Yield(true);
                    wxLog::FlushActive();
                }
                if (dlg.bGetAbort())
                    ipc.Kill();
            } else
                ::wxLogWarning(_("Could not query multimedia support"));
            if ((!dlg.bGetAbort()) && (!m_bEsdRunning)) {
                ::wxLogTrace(MYTRACETAG, wxT("Starting nxesd"));
                if (ipc.ServiceProcess(fn.GetFullPath() + wxT(" --start audio"), this)) {
                    while (ipc.IsRunning() && (!dlg.bGetAbort()) && (!m_bEsdRunning)) {
                        ::wxGetApp().Yield(true);
                        wxLog::FlushActive();
                    }
                }
                if (dlg.bGetAbort())
                    ipc.Kill();
                if (!m_bEsdRunning)
                    ::wxLogWarning(_("Could not start multimedia support"));
            }
            dlg.SetStatusText(wxString::Format(_("Connecting to %s ..."),
                        m_pCfg->sGetServerHost().c_str()));
        }
        if (dlg.bGetAbort())
            return false;
        if (getActiveCupsPrinters().GetCount() > 0) {
            dlg.SetStatusText(_("Preparing CUPS service ..."));
            prepareCups();
            dlg.SetStatusText(wxString::Format(_("Connecting to %s ..."),
                        m_pCfg->sGetServerHost().c_str()));
        }

        MyIPC nxssh;
        m_pNxSsh = &nxssh;

        ::wxLogInfo(wxT("Starting %s"), nxsshcmd.c_str());
        do {
            m_bRemoveKey = false;
            m_sOffendingKey = wxEmptyString;
            if (nxssh.SshProcess(nxsshcmd, fn.GetShortPath(), this)) {
                m_bGotError = false;
                m_eConnectState = STATE_HELLO;
                while (!(dlg.bGetAbort() || m_bGotError ||
                            (m_bSessionRunning && m_bSessionEstablished))) {
                    wxLog::FlushActive();
                    ::wxGetApp().Yield(true);
                }
                if (dlg.bGetAbort() || m_bGotError) {
                    if (m_bRemoveKey) {
                        while (m_sOffendingKey.IsEmpty()) {
                            wxLog::FlushActive();
                            ::wxGetApp().Yield(true);
                        }
                    } else {
                        nxssh.Kill();
                        return false;
                    }
                }
                wxThread::Sleep(500);
            } else {
                ::wxLogError(_("Called command was: ") + nxsshcmd);
                ::wxLogError(_("Could not start nxssh."));
                return false;
            }
            if (m_bRemoveKey)
                clearSshKeys(m_sOffendingKey);
        } while (m_bRemoveKey);
        return true;
    }
    return false;
}
