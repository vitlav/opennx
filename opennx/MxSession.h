#ifndef _MXSESSION_H_
#define _MXSESSION_H_

#include <wx/regex.h>

class MxSession
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

    MxSession() { }
    MxSession(wxString dir, wxString status, wxString stype, wxString host, int port, wxString md5);
    MxSession(const MxSession &src);
    virtual ~MxSession();

    wxString GetCreationTime();
    wxString GetPIDStr() { return wxString::Format(_T("%d"), m_pid); }
    wxString GetPortStr() { return wxString::Format(_T("%d"), m_port); }

    bool Create(const wxString cfgFileName);
    bool GetPidFromFile();
    void CheckState();

    MxSession &operator =(const MxSession &src);

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
