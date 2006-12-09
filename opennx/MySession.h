#ifndef _MYSESSION_H_
#define _MYSESSION_H_

#include <wx/regex.h>

class MySession
{
public:
    friend class SessionList;

    enum sType {
        None,
        Client,
        Server,
    };

    enum sStatus {
        Unknown,
        Failed,
        Terminated,
        Running,
    };

    MySession() { }
    MySession(wxString dir, wxString status, wxString stype, wxString host, int port, wxString md5);
    MySession(const MySession &src);
    virtual ~MySession();

    wxString GetCreationTime();
    wxString GetPIDStr() { return wxString::Format(_T("%d"), m_pid); }
    wxString GetPortStr() { return wxString::Format(_T("%d"), m_port); }

    bool Create(const wxString cfgFileName, const wxString password);
    bool GetPidFromFile();
    void CheckState();

    MySession &operator =(const MySession &src);

private:

    int m_port;
    long m_pid;
    enum sType m_type;
    enum sStatus m_status;
    wxString m_host;
    wxString m_md5;
    wxString m_dir;
    bool m_touched;

private:
    wxRegEx m_rePID;
};

#endif
