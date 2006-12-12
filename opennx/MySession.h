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

    MySession() { }
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
        // STATE_LISTSESSIONS,
        // STATE_PARSE_SESSIONS,
        STATE_START_SESSION,
        STATE_FINISH,
    } tConnectState;

    wxString getXauthCookie();
    int getFirstFreePort();
    virtual void OnSshEvent(wxCommandEvent &);

    tConnectState m_eConnectState;
    bool m_bGotError;
    bool m_bSslTunneling;
    bool m_bSessionRunning;
    int m_iProgress;
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
    wxString m_sXauthCookie;

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
