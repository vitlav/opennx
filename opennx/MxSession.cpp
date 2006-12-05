/////////////////////////////////////////////////////////////////////////////
// $Id: MxSession.cpp 886 2004-11-09 10:41:53Z felfert $
//
// Author:      Fritz Elfert
// Created:     06/13/04 22:54:56
// Copyright:   Copyright 2004 Millenux GmbH
// Licence:     LGPL
/////////////////////////////////////////////////////////////////////////////

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
#include "MxSession.h"
#include "MxXmlConfig.h"
#include "MxIPC.h"

#include <wx/filename.h>
#include <wx/regex.h>
#include <wx/wfstream.h>
#include <wx/txtstrm.h>
#include <wx/socket.h>
#include <wx/config.h>
//#include <wx/dynlib.h>
#include <wx/utils.h>
#include <wx/tokenzr.h>

#ifdef MYTRACETAG
# undef MYTRACETAG
#endif
#define MYTRACETAG wxT("MxSession")

MxSession::MxSession(wxString dir, wxString status, wxString stype, wxString host, int port, wxString md5)
    : m_host(host)
    , m_port(port)
    , m_pid(0)
    , m_md5(md5)
    , m_type(None)
    , m_status(Unknown)
    , m_dir(dir)
    , m_touched(true)
{
    if (stype == wxT("C"))
        m_type = Server;
    if (stype == wxT("S"))
        m_type = Client;

    if (status == wxT("T"))
        m_status = Terminated;
    if (status == wxT("F"))
        m_status = Failed;

    m_rePID.Compile(wxT("([[:digit:]]+)"));

    CheckState();
    GetPidFromFile();
}

MxSession::MxSession(const MxSession &src)
{
    m_host = src.m_host;
    m_port = src.m_port;
    m_pid = src.m_pid;
    m_md5 = src.m_md5;
    m_type = src.m_type;
    m_status = src.m_status;
    m_dir = src.m_dir;
    m_touched = src.m_touched;
    m_rePID.Compile(wxT("([[:digit:]]+)"));
}

MxSession & MxSession::operator =(const MxSession &src)
{
    m_host = src.m_host;
    m_port = src.m_port;
    m_pid = src.m_pid;
    m_md5 = src.m_md5;
    m_type = src.m_type;
    m_status = src.m_status;
    m_dir = src.m_dir;
    m_touched = src.m_touched;
    m_rePID.Compile(wxT("([[:digit:]]+)"));
    return *this;
}

MxSession::~MxSession()
{
    ::wxLogTrace(MYTRACETAG, wxT("~MxSession: md5='%s'"), m_md5.c_str());
}

wxString MxSession::GetCreationTime()
{
    wxDateTime ctime;
    wxFileName fn(m_dir, wxT("session"));
    fn.GetTimes(NULL, NULL, &ctime);
    return ctime.Format();
}

bool MxSession::GetPidFromFile()
{
    wxFileInputStream input(m_dir + wxFileName::GetPathSeparator() + wxT("session"));
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
                m_rePID.GetMatch(line,1).ToLong(&m_pid);
        }
        cnt++;
    }
    return (m_pid > 0);
}

void MxSession::CheckState()
{
    if ((m_status == Terminated) || (m_status == Failed))
        return;

    wxFFile fi(m_dir + wxFileName::GetPathSeparator() + wxT("session"));
    wxFFileInputStream input(fi);

    if (fi.Length() > 2048)
        input.SeekI(2048, wxFromEnd);
    wxTextInputStream text(input);
    while (!input.Eof()) {
        wxString line = text.ReadLine();
        if (line.IsEmpty())
            continue;
        if (line.Find(wxT("Info: Shutting down the link and exiting")) != -1) {
            m_status = Terminated;
            break;
        }

    }
}

static int getFirstFreePort()
{
    int port = 6324;
    wxIPV4address a;

    a.LocalHost();
    while (port < 65535) {
        a.Service(port);
        wxSocketServer ss(a);
        if (ss.Ok())
            return port;
    }
    return 0;
}

typedef enum {
    STATE_HELLO,
    STATE_SHELLMODE,
    STATE_START_LOGIN,
    STATE_START_SESSION,
} SessionState;

bool MxSession::Create(const wxString cfgFileName, const wxString password)
{
    wxString sSessionID;
    wxString sSessionType;
    wxString sSessionCache;
    wxString sSessionDisplay;
    wxString sProxyCookie;
    wxString sProxyIP;
    wxString sSubscription;
    wxString sAgentCookie;
    bool bSslTunneling = false;
    wxString cmd = wxT("-nx -x -2 -o 'RhostsAuthentication no' -o 'PasswordAuthentication no' -o 'RSAAuthentication no' -o 'RhostsRSAAuthentication no' -o 'PubkeyAuthentication yes' ");

    MxXmlConfig cfg(cfgFileName);
    if (cfg.IsValid()) {
        wxString appDir;
        wxConfigBase::Get()->Read(wxT("Config/SystemMxDir"), &appDir);
        wxFileName fn(appDir);

        // FE. nxssh is a fork of a stone-old openssh and therefore complains
        // about newer options in .ssh/config e.g.: ForwardDynamic.
        // Therefore we disable the use of a cfg at all.
        cmd += wxT("-F /dev/null ");

        int proxyPort = -1;
        cmd += wxString::Format(wxT("-p %d "), cfg.iGetServerPort());
        if (cfg.bGetUseSmartCard())
            cmd += wxT("-I 0 ");
        else
            cmd += wxString::Format(wxT("-i %s/share/client.id_dsa.key "), fn.GetShortPath().c_str());
        if (cfg.bGetEnableSSL()) {
            proxyPort = getFirstFreePort();
            cmd += wxString::Format(wxT("-D %d "), proxyPort);
        }
#ifdef __UNIX__
        // retrieve our xauth cookie
        wxString x11_auth_cookie;
        wxString dpy;
        if (wxGetEnv(wxT("DISPLAY"), &dpy) && (!dpy.IsEmpty())) {
            wxString cmd = wxString::Format(wxT("xauth list %s"), dpy.AfterFirst(wxT(':')).c_str());
            wxArrayString clines;
            if (::wxExecute(cmd, clines) == 0) {
                size_t count = clines.GetCount();
                for (size_t i = 0; (i < count) && x11_auth_cookie.IsEmpty(); i++) {
                    wxString line = clines[i];
                    if (line.Contains(wxT("MIT-MAGIC-COOKIE-1"))) {
                        wxStringTokenizer t(line, wxT("\t "));
                        int ecount = 0;
                        while (t.HasMoreTokens() && x11_auth_cookie.IsEmpty()) {
                            wxString tok = t.GetNextToken();
                            if (++ecount == 3)
                                x11_auth_cookie = tok;
                        }
                    }
                }
            }
        }
#endif

        MxIPC ssh;
        cmd += wxString::Format(wxT("nx@%s"), cfg.sGetServerHost().c_str());
        cmd = wxString::Format(wxT("%s/bin/nxssh "), fn.GetShortPath().c_str()) + cmd;

        if (ssh.SshProcess(appDir + wxT("/bin"), cmd)) {
            bool gotError = false;
            int p;
            int state = 0;
            wxString tmp;
            wxString scmd;

            ConnectDialog d(NULL);
            d.SetStatusText(wxString::Format(wxT("Connecting to %s ..."),
                        cfg.sGetServerHost().c_str()));
            d.Show(true);
            while (!(d.bGetAbort() || gotError)) {
                int p = ssh.SshChat(tmp);
                ::wxLogTrace(MYTRACETAG, wxT("chat1 ret=%d, tmp='%s'"), p, tmp.c_str());
                switch (p) {
                    case MxIPC::ActionNone:
                        break;
                    case MxIPC::ActionStatus:
                        d.SetStatusText(tmp);
                        break;
                    case MxIPC::ActionNotice:
                        ::wxLogMessage(tmp);
                        break;
                    case MxIPC::ActionWarning:
                        ::wxLogWarning(tmp);
                        break;
                    case MxIPC::ActionError:
                        ::wxLogError(tmp);
                        gotError = true;
                        break;
                    case MxIPC::ActionPromptYesNo:
                        if (wxMessageBox(tmp, _("Warning"), wxYES_NO|wxICON_EXCLAMATION) == wxYES)
                            ssh.Print(wxT("yes"));
                        else
                            ssh.Print(wxT("no"));
                        break;
                    case MxIPC::ActionSendUsername:
                        d.SetStatusText(_("Sending username"));
                        ssh.Print(cfg.sGetUsername());
                        break;
                    case MxIPC::ActionSendPassword:
                        d.SetStatusText(_("Sending password"));
                        ssh.Print(password);
                        break;
                    case MxIPC::ActionNextCommand:
                        p += 10;
                        d.SetProgress(p);
                        switch (++state) {
                            case STATE_HELLO:
                                ssh.Print(wxT("HELLO NXCLIENT - Version 1.4.0"));
                                break;
                            case STATE_SHELLMODE:
                                ssh.Print(wxT("SET SHELL_MODE SHELL"));
                                break;
                            case STATE_START_LOGIN:
                                ssh.Print(wxT("login"));
                                break;
                            case STATE_START_SESSION:
                                scmd = wxT("startsession ") +
                                    cfg.sGetSessionParams(wxT(""));
#ifdef __UNIX__
                                if (!x11_auth_cookie.IsEmpty())
                                    scmd += wxString::Format(wxT(" --cookie=\"%s\""),
                                            x11_auth_cookie.c_str());
#endif
                                ssh.Print(scmd);
                                break;
                        }
                        break;
                    case MxIPC::ActionPassphraseDialog:
                        ::wxMessageBox(wxT("NOT IMPLMENTED"),
                                wxT("iMxIPC::ActionPassphraseDialog"));
                        ssh.Print(wxT("notimplemented"));
                        break;
                    case MxIPC::ActionSetSessionID:
                        sSessionID = tmp;
                        break;
                    case MxIPC::ActionSetProxyCookie:
                        sProxyCookie = tmp;
                        break;
                    case MxIPC::ActionSetProxyIP:
                        sProxyIP = tmp;
                        break;
                    case MxIPC::ActionSetSessionType:
                        sSessionType = tmp;
                        break;
                    case MxIPC::ActionSetSessionCache:
                        sSessionCache = tmp;
                        break;
                    case MxIPC::ActionSetSessionDisplay:
                        sSessionDisplay = tmp;
                        break;
                    case MxIPC::ActionSetAgentCookie:
                        sAgentCookie = tmp;
                        break;
                    case MxIPC::ActionSetSslTunneling:
                        bSslTunneling = (tmp == wxT("1"));
                        break;
                    case MxIPC::ActionSetSubscription:
                        sSubscription = tmp;
                        break;
                    case MxIPC::ActionExit:
                        return true;
                    case MxIPC::ActionChildDied:
                        if (!tmp.IsEmpty())
                            ::wxLogError(tmp);
                        ::wxLogError(_("Called command was: ") + cmd);
                        ::wxLogError(_("nxssh terminated with status %d."), ssh.GetResult());
                        gotError = true;
                        break;
                    case MxIPC::ActionStartProxy:
                        {
                            wxString fname;
                            wxConfigBase::Get()->Read(wxT("Config/UserMxDir"), &fname);
                            wxString o = wxString::Format(wxT("cookie=%s,root=%s,session=%s,id=%s,"),
                                    sProxyCookie.c_str(), fname.c_str(), cfg.sGetName().c_str(),
                                    sSessionID.c_str());
                            if (bSslTunneling)
                                o += wxString::Format(wxT("listen=20000:%s"), sSessionDisplay.c_str());
                            else
                                o += wxString::Format(wxT("connect=%s:%s"),
                                        cfg.sGetServerHost().c_str(), sSessionDisplay.c_str());
                            fname += wxFileName::GetPathSeparator();
                            fname += wxString::Format(wxT("S-%s"), sSessionID.c_str());
                            {
                                wxFileName::Mkdir(fname, 0777, wxPATH_MKDIR_FULL);
                                fname += wxFileName::GetPathSeparator();
                                fname += wxT("options");
                                wxFile f(fname, wxFile::write_excl);
                                if (f.IsOpened()) {
                                    f.Write(o);
                                    f.Close();
                                    wxString pcmd;
                                    wxConfigBase::Get()->Read(wxT("Config/SystemMxDir"), &pcmd);
                                    pcmd += wxFileName::GetPathSeparator();
                                    pcmd += wxT("bin");
                                    pcmd += wxFileName::GetPathSeparator();
                                    pcmd += wxString::Format(wxT("nxproxy -S options=%s:%s"),
                                            fname.c_str(), sSessionDisplay.c_str());
                                    ::wxExecute(pcmd, wxEXEC_SYNC);
                                }
                            }
                            ssh.Print(wxT("exit"));
                            return true;
                        }
                        break;
                }
                d.Update();
                wxSafeYield(&d);
            }
            if (d.bGetAbort()) {
                ssh.Kill();
            }
            return true;
        } else {
            ::wxLogError(_("Called command was: ") + cmd);
            ::wxLogError(_("Could not start nxssh."));
            return true;
        }
    }
    return false;
}
