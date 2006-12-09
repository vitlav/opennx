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
#define MYTRACETAG wxT("MySession")

MySession::MySession(wxString dir, wxString status, wxString stype, wxString host, int port, wxString md5)
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

MySession::MySession(const MySession &src)
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

MySession & MySession::operator =(const MySession &src)
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

MySession::~MySession()
{
    ::wxLogTrace(MYTRACETAG, wxT("~MySession: md5='%s'"), m_md5.c_str());
}

wxString MySession::GetCreationTime()
{
    wxDateTime ctime;
    wxFileName fn(m_dir, wxT("session"));
    fn.GetTimes(NULL, NULL, &ctime);
    return ctime.Format();
}

bool MySession::GetPidFromFile()
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

void MySession::CheckState()
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

bool MySession::Create(const wxString cfgFileName, const wxString password)
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
    wxString cmd;

    MyXmlConfig cfg(cfgFileName);
    if (cfg.IsValid()) {
        wxString appDir;
        wxString userDir;
        wxConfigBase::Get()->Read(wxT("Config/SystemNxDir"), &appDir);
        wxConfigBase::Get()->Read(wxT("Config/SystemNxDir"), &userDir);
        wxFileName fn(appDir);

        cmd << wxT("-nx -x -2")
            << wxT(" -o 'RhostsAuthentication no'")
            << wxT(" -o 'PasswordAuthentication no'")
            << wxT(" -o 'RSAAuthentication no'")
            << wxT(" -o 'RhostsRSAAuthentication no'")
            << wxT(" -o 'PubkeyAuthentication yes'");

        // FE. nxssh is a fork of a stone-old openssh and therefore complains
        // about newer options in .ssh/config e.g.: ForwardDynamic.
        // Therefore we disable the use of a cfg at all.
        cmd << wxT(" -F /dev/null ");

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

        MyIPC ssh;
        cmd += wxString::Format(wxT("nx@%s"), cfg.sGetServerHost().c_str());
        cmd = wxString::Format(wxT("%s/bin/nxssh "), fn.GetShortPath().c_str()) + cmd;

        if (ssh.SshProcess(appDir + wxT("/bin"), cmd)) {
            bool gotError = false;
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
                    case MyIPC::ActionNone:
                        break;
                    case MyIPC::ActionStatus:
                        d.SetStatusText(tmp);
                        break;
                    case MyIPC::ActionNotice:
                        ::wxLogMessage(tmp);
                        break;
                    case MyIPC::ActionWarning:
                        ::wxLogWarning(tmp);
                        break;
                    case MyIPC::ActionError:
                        ::wxLogError(tmp);
                        gotError = true;
                        break;
                    case MyIPC::ActionPromptYesNo:
                        if (wxMessageBox(tmp, _("Warning"), wxYES_NO|wxICON_EXCLAMATION) == wxYES)
                            ssh.Print(wxT("yes"));
                        else
                            ssh.Print(wxT("no"));
                        break;
                    case MyIPC::ActionSendUsername:
                        d.SetStatusText(_("Sending username"));
                        ssh.Print(cfg.sGetUsername());
                        break;
                    case MyIPC::ActionSendPassword:
                        d.SetStatusText(_("Sending password"));
                        ssh.Print(password);
                        break;
                    case MyIPC::ActionNextCommand:
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
                    case MyIPC::ActionPassphraseDialog:
                        ::wxMessageBox(wxT("NOT IMPLMENTED"),
                                wxT("iMyIPC::ActionPassphraseDialog"));
                        ssh.Print(wxT("notimplemented"));
                        break;
                    case MyIPC::ActionSetSessionID:
                        sSessionID = tmp;
                        break;
                    case MyIPC::ActionSetProxyCookie:
                        sProxyCookie = tmp;
                        break;
                    case MyIPC::ActionSetProxyIP:
                        sProxyIP = tmp;
                        break;
                    case MyIPC::ActionSetSessionType:
                        sSessionType = tmp;
                        break;
                    case MyIPC::ActionSetSessionCache:
                        sSessionCache = tmp;
                        break;
                    case MyIPC::ActionSetSessionDisplay:
                        sSessionDisplay = tmp;
                        break;
                    case MyIPC::ActionSetAgentCookie:
                        sAgentCookie = tmp;
                        break;
                    case MyIPC::ActionSetSslTunneling:
                        bSslTunneling = (tmp == wxT("1"));
                        break;
                    case MyIPC::ActionSetSubscription:
                        sSubscription = tmp;
                        break;
                    case MyIPC::ActionExit:
                        return true;
                    case MyIPC::ActionChildDied:
                        if (!tmp.IsEmpty())
                            ::wxLogError(tmp);
                        ::wxLogError(_("Called command was: ") + cmd);
                        ::wxLogError(_("nxssh terminated with status %d."), ssh.GetResult());
                        gotError = true;
                        break;
                    case MyIPC::ActionStartProxy:
                        {
                            wxString o;
                            o << wxT("nx,cookie=") << sProxyCookie
                              << wxT(",session=session")
                              << wxT(",root=") << userDir
                              << wxT(",product=") << sSubscription
                              << wxT(",id=") << sSessionID;
#if 0
                            if (bSslTunneling)
                                o += wxString::Format(wxT("listen=20000:%s"), sSessionDisplay.c_str());
                            else
                                o += wxString::Format(wxT("connect=%s:%s"),
                                        cfg.sGetServerHost().c_str(), sSessionDisplay.c_str());
#endif
                            wxString fname = userDir + wxFileName::GetPathSeparator();
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
                                    wxConfigBase::Get()->Read(wxT("Config/SystemNxDir"), &pcmd);
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
