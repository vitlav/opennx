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
#include <wx/dynlib.h>

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
    if (stype == _T("C"))
        m_type = Server;
    if (stype == _T("S"))
        m_type = Client;

    if (status == _T("T"))
        m_status = Terminated;
    if (status == _T("F"))
        m_status = Failed;

    m_rePID.Compile(_T("([[:digit:]]+)"));

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
    m_rePID.Compile(_T("([[:digit:]]+)"));
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
    m_rePID.Compile(_T("([[:digit:]]+)"));
    return *this;
}

MxSession::~MxSession()
{
    ::wxLogDebug(_T("~MxSession: %s"), m_md5.c_str());
}

wxString MxSession::GetCreationTime()
{
    wxDateTime ctime;
    wxFileName fn(m_dir, _T("session"));
    fn.GetTimes(NULL, NULL, &ctime);
    return ctime.Format();
}

bool MxSession::GetPidFromFile()
{
    wxFileInputStream input(m_dir + wxFileName::GetPathSeparator() + _T("session"));
    wxTextInputStream text(input);
    int cnt = 0;

    while ((!input.Eof()) && (cnt < 100)) {
        wxString line = text.ReadLine();
        int idx = line.Find(_T("pid"));

        if (idx != -1 &&
            line.Find(_T("NXAGENT")) == -1 &&
            line.Find(_T("Agent")) == -1)
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

    wxFFile fi(m_dir + wxFileName::GetPathSeparator() + _T("session"));
    wxFFileInputStream input(fi);

    if (fi.Length() > 2048)
        input.SeekI(2048, wxFromEnd);
    wxTextInputStream text(input);
    while (!input.Eof()) {
        wxString line = text.ReadLine();
        if (line.IsEmpty())
            continue;
        if (line.Find(_T("Info: Shutting down the link and exiting")) != -1) {
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

bool MxSession::Create(const wxString cfgFileName)
{
    wxString cmd = _T("-nx -x -2 -o 'RhostsAuthentication no' -o 'PasswordAuthentication no' -o 'RSAAuthentication no' -o 'RhostsRSAAuthentication no' -o 'PubkeyAuthentication yes' ");

    MxXmlConfig cfg(cfgFileName);
    if (cfg.IsValid()) {
        wxString appDir;
        wxConfigBase::Get()->Read(_T("Config/SystemMxDir"), &appDir);
        wxFileName fn(appDir);
        
        int proxyPort = -1;
        cmd += wxString::Format(_T("-p %d "), cfg.iGetServerPort());
        if (cfg.bGetUseSmartCard())
            cmd += _T("-I 0 ");
        else
            cmd += wxString::Format(_T("-i %s/share/client.id_dsa.key "), fn.GetShortPath().c_str());
        if (cfg.bGetEnableSSL()) {
            proxyPort = getFirstFreePort();
            cmd += wxString::Format(_T("-D %d "), proxyPort);
        }
        MxIPC ssh;

        cmd += wxString::Format(_T("nx@%s"), cfg.sGetServerHost().c_str());
        cmd = wxString::Format(_T("%s/bin/nxssh "), fn.GetShortPath().c_str()) + cmd;
        wxLogDebug(_T("nxssh args: %s"), cmd.c_str());

        if (ssh.SshProcess(appDir + _T("/bin"), cmd)) {
            bool gotError = false;
            wxString tmp;

            ConnectDialog d(NULL);
            d.SetStatusText(wxString::Format(_T("Connecting to %s ..."), cfg.sGetServerHost().c_str()));
            d.Show(true);
            while (ssh.IsRunning() && (!d.bGetAbort()) && (!gotError)) {
                int p = ssh.SshChat(tmp);
                switch (p) {
                    case -1:
                        break;
                    case -2:
                        break;
                    case -3:
                        if (wxMessageBox(tmp, _T("Warning"), wxYES_NO) == wxYES)
                            ssh.Print(_T("yes\n"));
                        else
                            ssh.Print(_T("no\n"));
                        break;
                    case -99:
                        gotError = true;
                        break;
                    default:
                        if (p >= 0) {
                            d.SetStatusText(tmp);
                            d.SetProgress(p);
                        }
                }
                d.Update();
                wxSafeYield(&d);
            }
            if (d.bGetAbort()) {
                ssh.Kill();
            }
            if (gotError)
                wxMessageBox(tmp, _T("NXSSH Error"));
            return true;
        }
    }
    return false;
}
