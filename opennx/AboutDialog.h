/////////////////////////////////////////////////////////////////////////////
// Name:        AboutDialog.h
// Purpose:     
// Author:      Fritz Elfert
// Modified by: 
// Created:     08/08/04 18:31:09
// RCS-ID:      
// Copyright:   Copyright 2004 Millenux GmbH
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _ABOUTDIALOG_H_
#define _ABOUTDIALOG_H_

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "AboutDialog.cpp"
#endif

/*!
 * Includes
 */

////@begin includes
#include "AboutDialog_symbols.h"
#include "wx/xrc/xmlres.h"
#include "wx/html/htmlwin.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class extHtmlWindow;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_DIALOG_ABOUT 10036
#define SYMBOL_ABOUTDIALOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_ABOUTDIALOG_TITLE _("About OpenNX")
#define SYMBOL_ABOUTDIALOG_IDNAME ID_DIALOG_ABOUT
#define SYMBOL_ABOUTDIALOG_SIZE wxDefaultSize
#define SYMBOL_ABOUTDIALOG_POSITION wxDefaultPosition
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
 * AboutDialog class declaration
 */

class AboutDialog: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( AboutDialog )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    AboutDialog( );
    AboutDialog( wxWindow* parent, wxWindowID id = SYMBOL_ABOUTDIALOG_IDNAME, const wxString& caption = SYMBOL_ABOUTDIALOG_TITLE, const wxPoint& pos = SYMBOL_ABOUTDIALOG_POSITION, const wxSize& size = SYMBOL_ABOUTDIALOG_SIZE, long style = SYMBOL_ABOUTDIALOG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_ABOUTDIALOG_IDNAME, const wxString& caption = SYMBOL_ABOUTDIALOG_TITLE, const wxPoint& pos = SYMBOL_ABOUTDIALOG_POSITION, const wxSize& size = SYMBOL_ABOUTDIALOG_SIZE, long style = SYMBOL_ABOUTDIALOG_STYLE );

    /// Creates the controls and sizers
    void CreateControls();

////@begin AboutDialog event handler declarations

////@end AboutDialog event handler declarations

////@begin AboutDialog member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end AboutDialog member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin AboutDialog member variables
    extHtmlWindow* m_pHtmlWindow;
////@end AboutDialog member variables
};

#endif
    // _ABOUTDIALOG_H_
