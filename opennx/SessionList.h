/////////////////////////////////////////////////////////////////////////////
// Name:        SessionList.h
// Purpose:     
// Author:      Fritz Elfert
// Modified by: 
// Created:     06/13/04 22:54:56
// RCS-ID:      $Id: SessionList.h 900 2004-12-09 21:23:05Z felfert $
// Copyright:   Copyright 2004 Millenux GmbH
// Licence:     LGPL
/////////////////////////////////////////////////////////////////////////////

#ifndef _SESSIONLIST_H_
#define _SESSIONLIST_H_

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "SessionList.cpp"
#endif

#include <wx/thread.h>

class wxDir;
class wxRegEx;
class wxListCtrl;
class SessionHash;

class SessionList : public wxThreadHelper
{
public:
    SessionList(wxString dir = _T(""), wxListCtrl *ctrl = NULL);
    virtual ~SessionList();

    virtual wxThread::ExitCode Entry();
    
    void SetDir(wxString dir);
    //wxString GetDir() { return m_dirName; }

    void ScanDir();
    void ShowSesssionLog(int);

private:
    wxCriticalSection m_csDir;
    wxString m_dirName;
    wxListCtrl *m_listctrl;
    SessionHash *m_sessions;
    wxDir *m_dir;
    wxRegEx *m_re;
    bool m_reValid;
};

#endif // _SESSIONLIST_H_
