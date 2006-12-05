/////////////////////////////////////////////////////////////////////////////
// Name:        SessionAdmin.cpp
// Purpose:     
// Author:      Fritz Elfert
// Modified by: 
// Created:     06/13/04 22:54:55
// RCS-ID:      $Id: SessionAdmin.cpp 900 2004-12-09 21:23:05Z felfert $
// Copyright:   Copyright 2004 Millenux GmbH
// Licence:     LGPL
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma implementation "SessionAdmin.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include <wx/filename.h>
#include <wx/config.h>

#include "SessionAdmin.h"
#include "SessionList.h"
#include "LoginDialog.h"
#include "AboutDialog.h"
#include "Icon.h"

////@begin XPM images
////@end XPM images

/*!
 * SessionAdmin type definition
 */

IMPLEMENT_CLASS( SessionAdmin, wxFrame )

/*!
 * SessionAdmin event table definition
 */

BEGIN_EVENT_TABLE( SessionAdmin, wxFrame )

////@begin SessionAdmin event table entries
    EVT_MENU( XRCID("ID_MENU_SESSION_NEW"), SessionAdmin::OnMenuSessionNewClick )

    EVT_MENU( XRCID("ID_MENU_FILE_CHDIR"), SessionAdmin::OnMenuFileChdirClick )

    EVT_MENU( XRCID("ID_MENU_FILE_EXIT"), SessionAdmin::OnMenuFileExitClick )

    EVT_MENU( XRCID("ID_MENU_SESSION_TERMINATE"), SessionAdmin::OnMenuSessionTerminateClick )

    EVT_MENU( XRCID("ID_MENU_SESSION_PSTATS"), SessionAdmin::OnMenuSessionPstatsClick )

    EVT_MENU( XRCID("ID_MENU_SESSION_FSTATS"), SessionAdmin::OnMenuSessionFstatsClick )

    EVT_MENU( XRCID("ID_MENU_SESSION_LOG"), SessionAdmin::OnMenuSessionLogClick )

    EVT_MENU( XRCID("ID_MENU_SESSION_REMOVE"), SessionAdmin::OnMenuSessionRemoveClick )

    EVT_MENU( XRCID("ID_MENU_SESSION_KILL"), SessionAdmin::OnMenuSessionKillClick )

    EVT_MENU( XRCID("ID_MENU_REFRESH"), SessionAdmin::OnMenuRefreshClick )

    EVT_MENU( XRCID("ID_MENU_HELP_ABOUT"), SessionAdmin::OnMenuHelpAboutClick )

    EVT_MENU( XRCID("ID_TOOL_SESSION_NEW"), SessionAdmin::OnToolSessionNewClick )

    EVT_MENU( XRCID("ID_TOOL_SESSION_TERMINATE"), SessionAdmin::OnToolSessionTerminateClick )

    EVT_MENU( XRCID("ID_TOOL_SESSION_PSTATS"), SessionAdmin::OnToolSessionPstatsClick )

    EVT_MENU( XRCID("ID_TOOL_SESSION_FSTATS"), SessionAdmin::OnToolSessionFstatsClick )

    EVT_MENU( XRCID("ID_TOOL_SESSION_LOG"), SessionAdmin::OnToolSessionLogClick )

    EVT_MENU( XRCID("ID_TOOL_REFRESH"), SessionAdmin::OnToolRefreshClick )

    EVT_MENU( XRCID("ID_TOOL_SESSION_REMOVE"), SessionAdmin::OnToolSessionRemoveClick )

    EVT_MENU( XRCID("ID_TOOL_SESSION_KILL"), SessionAdmin::OnToolSessionKillClick )

    EVT_LIST_ITEM_SELECTED( XRCID("ID_LISTCTRL"), SessionAdmin::OnListctrlSelected )
    EVT_LIST_ITEM_DESELECTED( XRCID("ID_LISTCTRL"), SessionAdmin::OnListctrlDeselected )

////@end SessionAdmin event table entries

END_EVENT_TABLE()

/*!
 * SessionAdmin constructors
 */

SessionAdmin::SessionAdmin()
{
}

SessionAdmin::SessionAdmin( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Create( parent, id, caption, pos, size, style );
}

SessionAdmin::~SessionAdmin()
{
    delete m_sessions;
}

/*!
 * SessionAdmin creator
 */

bool SessionAdmin::Create( wxWindow* parent, wxWindowID WXUNUSED(id), const wxString& WXUNUSED(caption), const wxPoint& WXUNUSED(pos), const wxSize& WXUNUSED(size), long WXUNUSED(style) )
{
////@begin SessionAdmin member initialisation
    m_SessionListCtrl = NULL;
////@end SessionAdmin member initialisation

    wxConfigBase::Get()->Read(_T("Config/UserMxDir"), &m_MxDirectory);

////@begin SessionAdmin creation
    SetParent(parent);
    CreateControls();
    SetIcon(this->GetIconResource(wxT("res/mxclient-admin.png")));
    Centre();
////@end SessionAdmin creation

    m_sessions = new SessionList(m_MxDirectory, m_SessionListCtrl);
    return TRUE;
}

/*!
 * Control creation for SessionAdmin
 */

void SessionAdmin::CreateControls()
{    
////@begin SessionAdmin content construction

    wxXmlResource::Get()->LoadFrame(this, GetParent(), _T("ID_FRAME_ADMIN"));
    if (FindWindow(XRCID("ID_LISTCTRL")))
        m_SessionListCtrl = wxDynamicCast(FindWindow(XRCID("ID_LISTCTRL")), wxListCtrl);
////@end SessionAdmin content construction

    // Create custom windows not generated automatically here.

////@begin SessionAdmin content initialisation

////@end SessionAdmin content initialisation

    SessionToolsEnable(false);
    m_SessionListCtrl->InsertColumn(0, _("Server"));
    m_SessionListCtrl->InsertColumn(1, _("Port"));
    m_SessionListCtrl->InsertColumn(2, _("Session ID"));
    m_SessionListCtrl->InsertColumn(3, _("Creation Date"));
    m_SessionListCtrl->InsertColumn(4, _("PID"));
    m_SessionListCtrl->InsertColumn(5, _("Status"));
    m_SessionListCtrl->InsertColumn(6, _("Type"));
    for (int i = 0; i < m_SessionListCtrl->GetColumnCount(); i++) 
        m_SessionListCtrl->SetColumnWidth(i, wxLIST_AUTOSIZE_USEHEADER);
}


void SessionAdmin::SessionToolsEnable(bool enable)
{
    GetToolBar()->EnableTool(XRCID("ID_TOOL_SESSION_TERMINATE"), enable);
    GetToolBar()->EnableTool(XRCID("ID_TOOL_SESSION_PSTATS"), enable);
    GetToolBar()->EnableTool(XRCID("ID_TOOL_SESSION_FSTATS"), enable);
    GetToolBar()->EnableTool(XRCID("ID_TOOL_SESSION_LOG"), enable);
    GetToolBar()->EnableTool(XRCID("ID_TOOL_SESSION_REMOVE"), enable);
    GetToolBar()->EnableTool(XRCID("ID_TOOL_SESSION_KILL"), enable);
    GetMenuBar()->FindItem(XRCID("ID_MENU_SESSION_TERMINATE"))->Enable(enable);
    GetMenuBar()->FindItem(XRCID("ID_MENU_SESSION_PSTATS"))->Enable(enable);
    GetMenuBar()->FindItem(XRCID("ID_MENU_SESSION_FSTATS"))->Enable(enable);
    GetMenuBar()->FindItem(XRCID("ID_MENU_SESSION_LOG"))->Enable(enable);
    GetMenuBar()->FindItem(XRCID("ID_MENU_SESSION_REMOVE"))->Enable(enable);
    GetMenuBar()->FindItem(XRCID("ID_MENU_SESSION_KILL"))->Enable(enable);
}

/*!
 * Should we show tooltips?
 */

bool SessionAdmin::ShowToolTips()
{
    return TRUE;
}

/*!
 * Get bitmap resources
 */

wxBitmap SessionAdmin::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
    return CreateBitmapFromFile(name);
}

/*!
 * Get icon resources
 */

wxIcon SessionAdmin::GetIconResource( const wxString& name )
{
    // Icon retrieval
    return CreateIconFromFile(name);
}

/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOL_SESSION_NEW
 */

void SessionAdmin::OnToolSessionNewClick( wxCommandEvent& event )
{
    OnMenuSessionNewClick(event);
}
/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENU_FILE_CHDIR
 */

void SessionAdmin::OnMenuFileChdirClick( wxCommandEvent& event )
{
    const wxString& dir = wxDirSelector(_("Choose new MX session directory."),
        m_MxDirectory, wxDD_DEFAULT_STYLE, wxDefaultPosition, this);
    if (!dir.IsEmpty()) {
        m_MxDirectory = dir;
        m_sessions->SetDir(dir);
    }
    event.Skip();
}

/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENU_FILE_EXIT
 */

void SessionAdmin::OnMenuFileExitClick( wxCommandEvent& event )
{
    // Insert custom code here
    Close();
    event.Skip();
}

/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENU_SESSION_TERMINATE
 */

void SessionAdmin::OnToolSessionTerminateClick( wxCommandEvent& event )
{
    OnMenuSessionTerminateClick(event);
}

/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENU_SESSION_PSTATS
 */

void SessionAdmin::OnToolSessionPstatsClick( wxCommandEvent& event )
{
    OnMenuSessionPstatsClick(event);
}

/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENU_SESSION_FSTATS
 */

void SessionAdmin::OnToolSessionFstatsClick( wxCommandEvent& event )
{
    OnMenuSessionFstatsClick(event);
}

/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENU_SESSION_LOG
 */

void SessionAdmin::OnToolSessionLogClick( wxCommandEvent& event )
{
    OnMenuSessionLogClick(event);
}

/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENU_SESSION_REMOVE
 */

void SessionAdmin::OnToolSessionRemoveClick( wxCommandEvent& event )
{
    OnMenuSessionRemoveClick(event);
}

/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENU_SESSION_KILL
 */

void SessionAdmin::OnToolSessionKillClick( wxCommandEvent& event )
{
    OnMenuSessionKillClick(event);
}

/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENU_OPTIONS_REFRESH
 */

void SessionAdmin::OnToolRefreshClick( wxCommandEvent& event )
{
    OnMenuRefreshClick(event);
}

/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENU_HELP_ABOUT
 */

void SessionAdmin::OnMenuHelpAboutClick( wxCommandEvent& event )
{
    AboutDialog d(this);
    d.ShowModal();
    event.Skip();
}

/*!
 * wxEVT_COMMAND_LIST_ITEM_SELECTED event handler for ID_LISTCTRL
 */

void SessionAdmin::OnListctrlSelected( wxListEvent& event )
{
    SessionToolsEnable(true);
    event.Skip();
}

/*!
 * wxEVT_COMMAND_LIST_ITEM_DESELECTED event handler for ID_LISTCTRL
 */

void SessionAdmin::OnListctrlDeselected( wxListEvent& event )
{
    SessionToolsEnable(false);
    event.Skip();
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENU_SESSION_NEW
 */

void SessionAdmin::OnMenuSessionNewClick( wxCommandEvent& event )
{
    // Insert custom code here
    LoginDialog d(this);
    d.ShowModal();
    event.Skip();
}

/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENU_SESSION_TERMINATE
 */

void SessionAdmin::OnMenuSessionTerminateClick( wxCommandEvent& event )
{
    long item = item = m_SessionListCtrl->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (item != -1) {
        m_SessionListCtrl->SetItem(item, 5, _("terminated"));
        m_SessionListCtrl->SetColumnWidth(5, wxLIST_AUTOSIZE);
    }
    event.Skip();
}

/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENU_SESSION_PSTATS
 */

void SessionAdmin::OnMenuSessionPstatsClick( wxCommandEvent& event )
{
    // Insert custom code here
    event.Skip();
}

/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENU_SESSION_FSTATS
 */

void SessionAdmin::OnMenuSessionFstatsClick( wxCommandEvent& event )
{
    // Insert custom code here
    event.Skip();
}

/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENU_SESSION_LOG
 */

void SessionAdmin::OnMenuSessionLogClick( wxCommandEvent& event )
{
    long item = item = m_SessionListCtrl->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (item != -1) {
        m_sessions->ShowSesssionLog(item);
    }
    // Insert custom code here
    event.Skip();
}

/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENU_SESSION_REMOVE
 */

void SessionAdmin::OnMenuSessionRemoveClick( wxCommandEvent& event )
{
    long item = item = m_SessionListCtrl->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (item != -1)
        m_SessionListCtrl->DeleteItem(item);
    event.Skip();
}

/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENU_SESSION_KILL
 */

void SessionAdmin::OnMenuSessionKillClick( wxCommandEvent& event )
{
    // Insert custom code here
    event.Skip();
}

/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENU_REFRESH
 */

void SessionAdmin::OnMenuRefreshClick( wxCommandEvent& event )
{
    // Insert custom code here
    event.Skip();
}


