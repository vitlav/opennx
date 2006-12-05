/////////////////////////////////////////////////////////////////////////////
// Name:        ConnectDialog.h
// Purpose:     
// Author:      Fritz Elfert
// Modified by: 
// Created:     09/28/04 12:57:18
// RCS-ID:      
// Copyright:   Copyright 2004 Millenux GmbH
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _CONNECTDIALOG_H_
#define _CONNECTDIALOG_H_

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "ConnectDialog.cpp"
#endif

/*!
 * Includes
 */

////@begin includes
#include "ConnectDialog_symbols.h"
#include "wx/xrc/xmlres.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_DIALOG_CONNECT 10060
#define SYMBOL_CONNECTDIALOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_CONNECTDIALOG_TITLE _("Connecting")
#define SYMBOL_CONNECTDIALOG_IDNAME ID_DIALOG_CONNECT
#define SYMBOL_CONNECTDIALOG_SIZE wxSize(400, 300)
#define SYMBOL_CONNECTDIALOG_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif
#ifndef wxFIXED_MINSIZE
#define wxFIXED_MINSIZE 0
#endif

/*!
 * ConnectDialog class declaration
 */

class ConnectDialog: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( ConnectDialog )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    ConnectDialog( );
    ConnectDialog( wxWindow* parent, wxWindowID id = SYMBOL_CONNECTDIALOG_IDNAME, const wxString& caption = SYMBOL_CONNECTDIALOG_TITLE, const wxPoint& pos = SYMBOL_CONNECTDIALOG_POSITION, const wxSize& size = SYMBOL_CONNECTDIALOG_SIZE, long style = SYMBOL_CONNECTDIALOG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_CONNECTDIALOG_IDNAME, const wxString& caption = SYMBOL_CONNECTDIALOG_TITLE, const wxPoint& pos = SYMBOL_CONNECTDIALOG_POSITION, const wxSize& size = SYMBOL_CONNECTDIALOG_SIZE, long style = SYMBOL_CONNECTDIALOG_STYLE );

    /// Creates the controls and sizers
    void CreateControls();

    void SetStatusText(wxString);
    void SetProgress(int);
    bool bGetAbort() {return m_bAbort;}

////@begin ConnectDialog event handler declarations

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL
    void OnCancelClick( wxCommandEvent& event );

////@end ConnectDialog event handler declarations

////@begin ConnectDialog member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end ConnectDialog member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

private:

////@begin ConnectDialog member variables
    wxStaticText* m_pCtrlStatus;
    wxGauge* m_pCtrlProgress;
    bool m_bAbort;
////@end ConnectDialog member variables
};

#endif
    // _CONNECTDIALOG_H_
