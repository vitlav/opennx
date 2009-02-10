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

#include "SessionList.h"
#include "MySession.h"
#include "LogDialog.h"
#include "opennxApp.h"
#include "pwcrypt.h"

#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/listctrl.h>
#include <wx/process.h>

#ifdef __UNIX__
#include <sys/types.h>
#include <signal.h>
#endif

#include "osdep.h"

#include "trace.h"
ENABLE_TRACE;

class SessionTraverser : public wxDirTraverser
{
    public:
        SessionTraverser(wxArrayString& dirs) : m_dirs(dirs) { }

        virtual wxDirTraverseResult OnFile(const wxString& WXUNUSED(filename))
        {
            return wxDIR_CONTINUE;
        }

        virtual wxDirTraverseResult OnDir(const wxString& dirpath)
        {
            wxString name = wxFileName::FileName(dirpath).GetFullName();
            m_dirs.Add(name);
            return wxDIR_IGNORE;
        }

    private:
        wxArrayString& m_dirs;
};

class RmRfTraverser : public wxDirTraverser
{
    public:
        RmRfTraverser() { }

        ~RmRfTraverser()
        {
            int n = m_aFiles.GetCount() - 1;
            while (n >= 0) {
                ::wxLogTrace(MYTRACETAG, wxT("Removing file %s"), m_aFiles[n].c_str());
                ::wxRemoveFile(m_aFiles[n--]);
            }
            n = m_aDirs.GetCount() - 1;
            while (n >= 0) {
                ::wxLogTrace(MYTRACETAG, wxT("Removing dir %s"), m_aDirs[n].c_str());
                ::wxRmdir(m_aDirs[n--]);
            }
        }

        virtual wxDirTraverseResult OnFile(const wxString& filename)
        {
            ::wxLogTrace(MYTRACETAG, wxT("going to delete file %s"), filename.c_str());
            m_aFiles.Add(filename);
            return wxDIR_CONTINUE;
        }

        virtual wxDirTraverseResult OnDir(const wxString& dirpath)
        {
            ::wxLogTrace(MYTRACETAG, wxT("going to delete dir %s"), dirpath.c_str());
            m_aDirs.Add(dirpath);
            return wxDIR_CONTINUE;
        }

    private:
        wxArrayString m_aDirs;
        wxArrayString m_aFiles;
};

WX_DECLARE_STRING_HASH_MAP(MySession, SessionHash);

SessionList::SessionList(wxString dir, wxListCtrl* ctrl)
    : wxThreadHelper()
    , m_dirName(dir)
    , m_listctrl(ctrl)
    , m_dir(NULL)
    , m_reValid(false)
{
    m_sessions = new SessionHash();
    m_re = new wxRegEx();
    if (m_re->Compile(wxT("(([TF])-)?([SC])-(.*)-([[:digit:]]+)-([[:xdigit:]]{32})$"), wxRE_ADVANCED))
        m_reValid = true;
    if (m_reValid && (!m_dirName.IsEmpty())) {
        Create(
#ifdef __OPENBSD__
                32768
#endif
              );
        m_thread->Run();
    }
}

SessionList::~SessionList()
{
    m_thread->Delete();
    while (m_thread->IsRunning())
        wxThread::Sleep(100);
    if (m_dir != NULL)
        delete m_dir;
    delete m_re;
    m_sessions->clear();
    delete m_sessions;
}

void SessionList::SetDir(wxString dir)
{
    m_dirName = dir;
    if (m_reValid && (!m_dirName.IsEmpty())) {
        if (m_thread == NULL) {
            Create();
            m_thread->Run();
        } else {
            m_csDir.Enter();
            if (m_dir)
                delete m_dir;
            m_dir = NULL;
            m_csDir.Leave();
        }
    }
}

void SessionList::ScanDir()
{
    wxCriticalSectionLocker dlocker(m_csDir);

    if (m_dir == NULL) {
        if (m_dirName.IsEmpty())
            return;
        if (!wxDir::Exists(m_dirName)) {
            m_dirName = wxEmptyString;
            return;
        }
        m_dir = new wxDir(m_dirName);
    }
    //::wxLogTrace(MYTRACETAG, wxT("SessionList: scanning %s"), m_dirName.c_str());

    // get the names of all session directories
    wxArrayString sdirs;
    SessionTraverser traverser(sdirs);
    m_dir->Traverse(traverser);
    size_t cnt = sdirs.GetCount();
    size_t i;

    // Format of session dir name:
    //
    // ([TF]-)?[SC]-(.*)-[:digit:]+-[:xdigit:]{32}
    //
    // 1. element is session status
    // "T-" resp. "F-" stands for terminated or failed respectively.
    // If it is missing, the session is undefined (probably running).
    //
    // 2. element is session type
    // S- resp. C- stands for server resp. client session type
    //
    // 3. element is host name of the NX server
    // 4. element is port name
    // 5. element is an md5sum. (//FE: For what data?)
    //

    bool changed = false;
    SessionHash::iterator it;
    size_t oldcount = m_sessions->size();
    for (it = m_sessions->begin(); it != m_sessions->end(); ++it)
        it->second.bSetTouched(false);

    for (i = 0; i < cnt; i++) {
        wxString tmp = sdirs[i];
        if (m_re->Matches(tmp)) {
            wxString md5 = m_re->GetMatch(tmp,6);

            it = m_sessions->find(md5);
            if (it == m_sessions->end()) {
                // New session found
                long port;

                m_re->GetMatch(tmp,5).ToLong(&port);
                ::wxLogTrace(MYTRACETAG,
                        wxT("State='%s', Type='%s', Host='%s', Port=%d, MD5='%s'"),
                        m_re->GetMatch(tmp,2).c_str(), m_re->GetMatch(tmp,3).c_str(),
                        m_re->GetMatch(tmp,4).c_str(), port, md5.c_str());
                // Create new hash entry
                MySession s(m_dirName + wxFileName::GetPathSeparator() + tmp,
                        m_re->GetMatch(tmp,2), m_re->GetMatch(tmp,3),
                        m_re->GetMatch(tmp,4), port, md5);
                (*m_sessions)[md5] = s;
                if (m_listctrl != NULL) {
                    long idx;

                    wxMutexGuiEnter();

                    // Hostname
                    idx = m_listctrl->InsertItem(0, s.sGetHost(), 0);
                    // Port
                    m_listctrl->SetItem(idx, 1, s.sGetPort());
                    // Session ID
                    m_listctrl->SetItem(idx, 2, s.sGetMd5());
                    // Creation Time
                    m_listctrl->SetItem(idx, 3, s.sGetCreationTime());
                    // PID
                    m_listctrl->SetItem(idx, 4, s.sGetPID());
                    // Status
                    switch (s.eGetSessionStatus()) {
                        case MySession::Terminated:
                            m_listctrl->SetItem(idx, 5, _("terminated"));
                            break;
                        case MySession::Failed:
                            m_listctrl->SetItem(idx, 5, _("failed"));
                            break;
                        case MySession::Running:
                            m_listctrl->SetItem(idx, 5, _("running"));
                            break;
                        default:
                            m_listctrl->SetItem(idx, 5, _("unknown"));
                            break;
                    }
                    // Type
                    m_listctrl->SetItem(idx, 6,
                            (s.eGetSessionType() == MySession::Server)
                            ? _("Server") : _("Client"));

                    // Data
                    m_listctrl->SetItemData(idx,
                            (long)&(m_sessions->find(md5)->second));
                    wxMutexGuiLeave();

                    changed = true;
                }
            } else {
                // Existing session found, mark it
                it->second.bSetTouched(true);
            }
        }
    }
    bool finished = false;

    while (!finished) {
        finished = true;
        for (it = m_sessions->begin(); it != m_sessions->end(); ++it) {
            if (it->second.bGetTouched()) {
                MySession::tSessionStatus s = it->second.eGetSessionStatus();
                it->second.CheckState();
                if (it->second.eGetSessionStatus() != s) {
                    wxMutexGuiEnter();
                    long idx = m_listctrl->FindItem(-1, (long)&it->second);
                    ::wxLogTrace(MYTRACETAG, wxT("state changed: %d"), idx);
                    if (idx != -1) {
                        switch (it->second.eGetSessionStatus()) {
                            case MySession::Terminated:
                                m_listctrl->SetItem(idx, 5, _("terminated"));
                                break;
                            case MySession::Failed:
                                m_listctrl->SetItem(idx, 5, _("failed"));
                                break;
                            case MySession::Running:
                                m_listctrl->SetItem(idx, 5, _("running"));
                                break;
                            default:
                                m_listctrl->SetItem(idx, 5, _("unknown"));
                                break;
                        }
                        m_listctrl->SetItem(idx, 3, it->second.sGetCreationTime());
                        m_listctrl->SetItem(idx, 4, it->second.sGetPID());
                        changed = true;
                    }
                    wxMutexGuiLeave();
                }
            } else {
                ::wxLogTrace(MYTRACETAG, wxT("Session '%s' disappeared"),
                        it->second.sGetMd5().c_str());
                finished = false;
                if (m_listctrl != NULL) {
                    wxMutexGuiEnter();
                    long item = m_listctrl->FindItem(-1, (long)&it->second);
                    if (item != -1) {
                        m_listctrl->DeleteItem(item);
                        changed = true;
                    }
                    wxMutexGuiLeave();
                }
                m_sessions->erase(it);
                break;
            }
        }
    }
    if (changed && (m_listctrl != NULL)) {
        wxMutexGuiEnter();
        for (int i = 0; i < m_listctrl->GetColumnCount(); i++) 
            m_listctrl->SetColumnWidth(i, wxLIST_AUTOSIZE);
        m_listctrl->Update();
        wxMutexGuiLeave();
    }
    if (m_sessions->size() != oldcount)
        ::wxLogTrace(MYTRACETAG, wxT("SessionList: Now %d sessions"), m_sessions->size());
}

    void
SessionList::ShowSessionLog(int idx)
{
    MySession *s = wxDynamicCast((void *)m_listctrl->GetItemData(idx), MySession);
    if (s) {
        LogDialog d(NULL);
        d.ReadLogFile(s->sGetDir() + wxFileName::GetPathSeparator() + wxT("session"));
        d.ShowModal();
    }
}

    void
SessionList::ShowSessionStats(int idx, bool full)
{
    MySession *s = wxDynamicCast((void *)m_listctrl->GetItemData(idx), MySession);
    if (s) {
        wxFileName fn(s->sGetDir(), wxT("stats"));
        long spid = s->lGetPID();
        wxString md5stats1 = Md5OfFile(fn.GetFullPath());
        wxString md5stats2 = md5stats1;
        bool ok = false;
#ifdef __UNIX__
        ok = (kill((pid_t)spid, full ? SIGUSR1 : SIGUSR2) == 0);
#endif
        if (ok) {
            // Wait until file has changed;
            ::wxLogTrace(MYTRACETAG, wxT("wait for change of stats"));
            while (ok) {
                md5stats2 = Md5OfFile(fn.GetFullPath());
                if (md5stats1 != md5stats2)
                    break;
                ::wxGetApp().Yield(true);
                wxThread::Sleep(100);
            }
            // Wait until file stopped changing
            wxThread::Sleep(100);
            ::wxLogTrace(MYTRACETAG, wxT("wait for settling stats"));
            while (ok) {
                md5stats1 = Md5OfFile(fn.GetFullPath());
                if (md5stats1 == md5stats2)
                    break;
                md5stats2 = md5stats1;
                ::wxGetApp().Yield(true);
                wxThread::Sleep(100);
            }
            if (ok) {
                LogDialog d(NULL);
                d.SetTitle(full ?
                        _("Full session statistics - OpenNX") :
                        _("Partial session statistics - OpenNX"));
                d.ReadLogFile(s->sGetDir() + wxFileName::GetPathSeparator() + wxT("stats"));
                d.ShowModal();
            }
        }
    }
}

    bool
SessionList::IsRunning(int idx)
{
    MySession *s = wxDynamicCast((void *)m_listctrl->GetItemData(idx), MySession);
    if (s)
        return (s->eGetSessionStatus() == MySession::Running);
    return false;
}

    void
SessionList::TerminateSession(int idx)
{
    MySession *s = wxDynamicCast((void *)m_listctrl->GetItemData(idx), MySession);
    if (s)
        close_sid(s->sGetMd5().mb_str());
}

    void
SessionList::KillSession(int idx)
{
    MySession *s = wxDynamicCast((void *)m_listctrl->GetItemData(idx), MySession);
    if (s)
        wxProcess::Kill(s->lGetPID());
}

    void
SessionList::CleanupDir(int idx)
{
    MySession *s = wxDynamicCast((void *)m_listctrl->GetItemData(idx), MySession);
    if (s) {
        wxString dir = s->sGetDir();
        ::wxLogTrace(MYTRACETAG, wxT("CleanupDir '%s'"), dir.c_str());
        if (!dir.IsEmpty()) {
            {
                wxDir d(dir);
                RmRfTraverser t;
                d.Traverse(t);
            }
            ::wxRmdir(dir);
        }
    }
}

    wxThread::ExitCode
SessionList::Entry()
{
    int cnt = 0;
    while (!m_thread->TestDestroy()) {
        if (cnt-- == 0) {
            ScanDir();
            cnt = 20;
        }
        wxThread::Sleep(100);
    }
    return 0;
}
