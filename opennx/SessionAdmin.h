/////////////////////////////////////////////////////////////////////////////
// Name:        SessionAdmin.h
// Purpose:     
// Author:      Fritz Elfert
// Modified by: 
// Created:     06/13/04 22:54:56
// RCS-ID:      $Id$
// Copyright:   Copyright 2004 Millenux GmbH
// Licence:     LGPL
/////////////////////////////////////////////////////////////////////////////

#ifndef _SESSIONADMIN_H_
#define _SESSIONADMIN_H_

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "SessionAdmin.cpp"
#endif

/*!
 * Includes
 */

////@begin includes
#include "SessionAdmin_symbols.h"
#include "wx/xrc/xmlres.h"
#include "wx/frame.h"
#include "wx/toolbar.h"
#include "wx/listctrl.h"
////@end includes

/*!
 * Forward declarations
 */

class SessionList;
////@begin forward declarations
class wxListCtrl;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_FRAME_ADMIN 10103
#define SYMBOL_SESSIONADMIN_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxMINIMIZE_BOX|wxMAXIMIZE_BOX|wxCLOSE_BOX
#define SYMBOL_SESSIONADMIN_TITLE _("Session Administrator - OpenNX")
#define SYMBOL_SESSIONADMIN_IDNAME ID_FRAME_ADMIN
#define SYMBOL_SESSIONADMIN_SIZE wxDefaultSize
#define SYMBOL_SESSIONADMIN_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * SessionAdmin class declaration
 */

class SessionAdmin: public wxFrame
{    
    DECLARE_CLASS( SessionAdmin )
    DECLARE_EVENT_TABLE()

public:

    void SessionToolsEnable(bool enable);
    /// Constructors
    SessionAdmin( );
    SessionAdmin( wxWindow* parent, wxWindowID id = SYMBOL_SESSIONADMIN_IDNAME, const wxString& caption = SYMBOL_SESSIONADMIN_TITLE, const wxPoint& pos = SYMBOL_SESSIONADMIN_POSITION, const wxSize& size = SYMBOL_SESSIONADMIN_SIZE, long style = SYMBOL_SESSIONADMIN_STYLE );
    virtual ~SessionAdmin();

    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_SESSIONADMIN_IDNAME, const wxString& caption = SYMBOL_SESSIONADMIN_TITLE, const wxPoint& pos = SYMBOL_SESSIONADMIN_POSITION, const wxSize& size = SYMBOL_SESSIONADMIN_SIZE, long style = SYMBOL_SESSIONADMIN_STYLE );

    /// Creates the controls and sizers
    void CreateControls();

////@begin SessionAdmin event handler declarations

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENU_SESSION_NEW
    void OnMenuSessionNewClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENU_FILE_CHDIR
    void OnMenuFileChdirClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENU_FILE_EXIT
    void OnMenuFileExitClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENU_SESSION_TERMINATE
    void OnMenuSessionTerminateClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENU_SESSION_PSTATS
    void OnMenuSessionPstatsClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENU_SESSION_FSTATS
    void OnMenuSessionFstatsClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENU_SESSION_LOG
    void OnMenuSessionLogClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENU_SESSION_REMOVE
    void OnMenuSessionRemoveClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENU_SESSION_KILL
    void OnMenuSessionKillClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENU_REFRESH
    void OnMenuRefreshClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENU_HELP_ABOUT
    void OnMenuHelpAboutClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOL_SESSION_NEW
    void OnToolSessionNewClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOL_SESSION_TERMINATE
    void OnToolSessionTerminateClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOL_SESSION_PSTATS
    void OnToolSessionPstatsClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOL_SESSION_FSTATS
    void OnToolSessionFstatsClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOL_SESSION_LOG
    void OnToolSessionLogClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOL_REFRESH
    void OnToolRefreshClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOL_SESSION_REMOVE
    void OnToolSessionRemoveClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOL_SESSION_KILL
    void OnToolSessionKillClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_LIST_ITEM_SELECTED event handler for ID_LISTCTRL
    void OnListctrlSelected( wxListEvent& event );

    /// wxEVT_COMMAND_LIST_ITEM_DESELECTED event handler for ID_LISTCTRL
    void OnListctrlDeselected( wxListEvent& event );

////@end SessionAdmin event handler declarations

////@begin SessionAdmin member function declarations

    wxString GetNxDirectory() const { return m_NxDirectory ; }
    void SetNxDirectory(wxString value) { m_NxDirectory = value ; }

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end SessionAdmin member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

private:

////@begin SessionAdmin member variables
    wxListCtrl* m_SessionListCtrl;
    wxString m_NxDirectory;
////@end SessionAdmin member variables

    SessionList *m_sessions;
};

#endif
    // _SESSIONADMIN_H_
