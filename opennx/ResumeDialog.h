// $Id$
//
// Copyright (C) 2006 The OpenNX team
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

#ifndef _RESUMEDIALOG_H_
#define _RESUMEDIALOG_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "ResumeDialog.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "ResumeDialog_symbols.h"
#include "wx/xrc/xmlres.h"
#include "wx/listctrl.h"
#include "wx/statline.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxListCtrl;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_RESUMEDIALOG 10050
#define SYMBOL_RESUMEDIALOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_RESUMEDIALOG_TITLE _("ResumeDialog")
#define SYMBOL_RESUMEDIALOG_IDNAME ID_RESUMEDIALOG
#define SYMBOL_RESUMEDIALOG_SIZE wxSize(400, 300)
#define SYMBOL_RESUMEDIALOG_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * ResumeDialog class declaration
 */

class ResumeDialog: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( ResumeDialog )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    ResumeDialog( );
    ResumeDialog( wxWindow* parent, wxWindowID id = SYMBOL_RESUMEDIALOG_IDNAME, const wxString& caption = SYMBOL_RESUMEDIALOG_TITLE, const wxPoint& pos = SYMBOL_RESUMEDIALOG_POSITION, const wxSize& size = SYMBOL_RESUMEDIALOG_SIZE, long style = SYMBOL_RESUMEDIALOG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_RESUMEDIALOG_IDNAME, const wxString& caption = SYMBOL_RESUMEDIALOG_TITLE, const wxPoint& pos = SYMBOL_RESUMEDIALOG_POSITION, const wxSize& size = SYMBOL_RESUMEDIALOG_SIZE, long style = SYMBOL_RESUMEDIALOG_STYLE );

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

    void SetPreferredSession(const wxString &name) { m_sPreferredSession = name; };
    void AddSession(const wxString&, const wxString&, const wxString&, const wxString&,
            const wxString&, const wxString&, const wxString&, const wxString&);

private:

////@begin ResumeDialog event handler declarations

    /// wxEVT_COMMAND_LIST_ITEM_SELECTED event handler for ID_LISTCTRL_SESSIONS
    void OnListctrlSessionsSelected( wxListEvent& event );

    /// wxEVT_COMMAND_LIST_ITEM_DESELECTED event handler for ID_LISTCTRL_SESSIONS
    void OnListctrlSessionsDeselected( wxListEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_TAKEOVER
    void OnButtonTakeoverClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_RESUME
    void OnButtonResumeClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_NEW
    void OnNEWClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_ABORT
    void OnABORTClick( wxCommandEvent& event );

////@end ResumeDialog event handler declarations

////@begin ResumeDialog member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end ResumeDialog member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin ResumeDialog member variables
    wxListCtrl* m_pCtrlSessions;
////@end ResumeDialog member variables

    wxString m_sPreferredSession;
};

#endif
    // _RESUMEDIALOG_H_
