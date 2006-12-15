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

#ifndef _MYSESSION_H_
#define _MYSESSION_H_

#include <wx/regex.h>

class ConnectDialog;
class MyIPC;
class MyXmlConfig;

class MySession : public wxEvtHandler
{
    DECLARE_CLASS(MySession);
    DECLARE_EVENT_TABLE();

public:
    typedef enum {
        None,
        Client,
        Server,
    } tSessionType;

    typedef enum {
        Unknown,
        Failed,
        Terminated,
        Running,
    } tSessionStatus;

    MySession() : m_pRunLog(NULL) { }
    MySession(wxString dir, wxString status, wxString stype, wxString host, int port, wxString md5);
    MySession(const MySession &src);
    virtual ~MySession();

    // session admin
    wxString sGetCreationTime();
    wxString sGetPID() { return wxString::Format(_T("%d"), m_lPid); }
    wxString sGetPort() { return wxString::Format(_T("%d"), m_iPort); }
    const wxString& sGetHost() { return m_sHost; }
    const wxString& sGetMd5() { return m_sMd5; }
    const wxString& sGetDir() { return m_sDir; }
    tSessionStatus eGetSessionStatus() { return m_eSessionStatus; }
    tSessionType eGetSessionType() { return m_eSessionType; }
    void bSetTouched(bool b) { m_bTouched = b; }
    bool bGetTouched() { return m_bTouched; }
    bool bGetPidFromFile();
    void CheckState();
    MySession &operator =(const MySession &src);

    // connection setup
    bool Create(const wxString cfgFileName, const wxString password);

private:
    // locals for connection setup
    typedef enum {
        STATE_HELLO,
        STATE_SHELLMODE,
        STATE_AUTHMODE,
        STATE_LOGIN,
        STATE_LIST_SESSIONS,
        STATE_PARSE_SESSIONS,
        STATE_START_SESSION,
        STATE_RESUME_SESSION,
        STATE_FINISH,
    } tConnectState;

    wxArrayString m_aParseBuffer;
    wxString getXauthCookie();
    wxString getXauthPath();
    int getFirstFreePort();
    void startProxy();
    void parseSessions();
    long intver(const wxString&);
    virtual void OnSshEvent(wxCommandEvent &);

    tConnectState m_eConnectState;
    bool m_bGotError;
    bool m_bSslTunneling;
    bool m_bSessionRunning;
    bool m_bCollectSessions;
    bool m_Abort;
    int m_iProgress;
    wxLog *m_pRunLog;
    MyIPC *m_pNxSsh;
    MyXmlConfig *m_pCfg;
    ConnectDialog *m_pDlg;
    wxString m_sClearPassword;
    wxString m_sSessionID;
    wxString m_sSessionType;
    wxString m_sSessionCache;
    wxString m_sSessionDisplay;
    wxString m_sProxyCookie;
    wxString m_sProxyIP;
    wxString m_sProxyPort;
    wxString m_sSubscription;
    wxString m_sAgentCookie;
    wxString m_sOptFilename;
    wxString m_sUserDir;
    wxString m_sSysDir;
    wxString m_sXauthCookie;
    wxString m_sResumeName;
    wxString m_sResumeType;
    wxString m_sResumeId;

    // locals for admin tool
    bool m_bTouched;
    int m_iPort;
    long m_lPid;
    tSessionType m_eSessionType;
    tSessionStatus m_eSessionStatus;
    wxString m_sHost;
    wxString m_sMd5;
    wxString m_sDir;
    wxRegEx m_rePID;
};

#endif
