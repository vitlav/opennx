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
    void ShowSessionLog(int);
    void ShowSessionStats(int, bool);
    void CleanupDir(int);
    void TerminateSession(int);
    void KillSession(int);
    bool IsRunning(int);

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
