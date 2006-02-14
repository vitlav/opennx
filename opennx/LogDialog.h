/////////////////////////////////////////////////////////////////////////////
// Name:        LogDialog.h
// Purpose:     
// Author:      Fritz Elfert
// Modified by: 
// Created:     06/17/04 03:34:48
// RCS-ID:      $Id: LogDialog.h 901 2004-12-10 21:59:33Z felfert $
// Copyright:   Copyright 2004 Millenux GmbH
// Licence:     LGPL
/////////////////////////////////////////////////////////////////////////////

#ifndef _LOGDIALOG_H_
#define _LOGDIALOG_H_

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "LogDialog.cpp"
#endif

/*!
 * Includes
 */

////@begin includes
#include "LogDialog_symbols.h"
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
#define ID_DIALOG_LOG 10104
#define SYMBOL_LOGDIALOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxMAXIMIZE_BOX|wxMINIMIZE_BOX
#define SYMBOL_LOGDIALOG_TITLE _("Session Log - MXclient")
#define SYMBOL_LOGDIALOG_IDNAME ID_DIALOG_LOG
#define SYMBOL_LOGDIALOG_SIZE wxSize(400, 300)
#define SYMBOL_LOGDIALOG_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * LogDialog class declaration
 */

class LogDialog: public wxDialog
{    
    DECLARE_CLASS( LogDialog )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    LogDialog( );
    LogDialog( wxWindow* parent, wxWindowID id = SYMBOL_LOGDIALOG_IDNAME, const wxString& caption = SYMBOL_LOGDIALOG_TITLE, const wxPoint& pos = SYMBOL_LOGDIALOG_POSITION, const wxSize& size = SYMBOL_LOGDIALOG_SIZE, long style = SYMBOL_LOGDIALOG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_LOGDIALOG_IDNAME, const wxString& caption = SYMBOL_LOGDIALOG_TITLE, const wxPoint& pos = SYMBOL_LOGDIALOG_POSITION, const wxSize& size = SYMBOL_LOGDIALOG_SIZE, long style = SYMBOL_LOGDIALOG_STYLE );

    /// Creates the controls and sizers
    void CreateControls();

    void SetFileName(wxString);
    void ReadLogFile(wxString);

////@begin LogDialog event handler declarations

////@end LogDialog event handler declarations

private:

////@begin LogDialog member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end LogDialog member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin LogDialog member variables
    wxTextCtrl* m_TextCtrl;
////@end LogDialog member variables

    wxString m_sFileName;
};

#endif
    // _LOGDIALOG_H_
