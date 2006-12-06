/////////////////////////////////////////////////////////////////////////////
// Name:        X11PropertyDialog.h
// Purpose:     
// Author:      Fritz Elfert
// Modified by: 
// Created:     06/25/04 00:45:06
// RCS-ID:      $Id$
// Copyright:   Copyright 2004 Millenux GmbH
// Licence:     LGPL
/////////////////////////////////////////////////////////////////////////////

#ifndef _X11PROPERTYDIALOG_H_
#define _X11PROPERTYDIALOG_H_

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "X11PropertyDialog.cpp"
#endif

/*!
 * Includes
 */

////@begin includes
#include "X11PropertyDialog_symbols.h"
#include "wx/xrc/xmlres.h"
#include "wx/valgen.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations
class MxXmlConfig;

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_DIALOG_SETTINGS_X11 10090
#define SYMBOL_X11PROPERTYDIALOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_X11PROPERTYDIALOG_TITLE _("X11 Session Settings - MXclient")
#define SYMBOL_X11PROPERTYDIALOG_IDNAME ID_DIALOG_SETTINGS_X11
#define SYMBOL_X11PROPERTYDIALOG_SIZE wxSize(228, 150)
#define SYMBOL_X11PROPERTYDIALOG_POSITION wxDefaultPosition
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
 * X11PropertyDialog class declaration
 */

class X11PropertyDialog: public wxDialog
{    
    DECLARE_CLASS( X11PropertyDialog )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    X11PropertyDialog( );
    X11PropertyDialog( wxWindow* parent, wxWindowID id = SYMBOL_X11PROPERTYDIALOG_IDNAME, const wxString& caption = SYMBOL_X11PROPERTYDIALOG_TITLE, const wxPoint& pos = SYMBOL_X11PROPERTYDIALOG_POSITION, const wxSize& size = SYMBOL_X11PROPERTYDIALOG_SIZE, long style = SYMBOL_X11PROPERTYDIALOG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_X11PROPERTYDIALOG_IDNAME, const wxString& caption = SYMBOL_X11PROPERTYDIALOG_TITLE, const wxPoint& pos = SYMBOL_X11PROPERTYDIALOG_POSITION, const wxSize& size = SYMBOL_X11PROPERTYDIALOG_SIZE, long style = SYMBOL_X11PROPERTYDIALOG_STYLE );

    void SetConfig(MxXmlConfig *);

private:
    /// Creates the controls and sizers
    void CreateControls();
    void UpdateDialogConstraints(bool);

////@begin X11PropertyDialog event handler declarations

    /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_X11_CONSOLE
    void OnRadiobuttonX11ConsoleSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_X11_XCLIENTS
    void OnRadiobuttonX11XclientsSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_X11_CUSTOMCMD
    void OnRadiobuttonX11CustomcmdSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_X11_VDESKTOP
    void OnRadiobuttonX11VdesktopSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_X11_WIN_FLOATING
    void OnRadiobuttonX11WinFloatingSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
    void OnOkClick( wxCommandEvent& event );

////@end X11PropertyDialog event handler declarations

public:
////@begin X11PropertyDialog member function declarations


    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end X11PropertyDialog member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin X11PropertyDialog member variables
    wxRadioButton* m_pCtrlRunCustomCommand;
    wxTextCtrl* m_pCtrlCustomCommand;
    wxRadioButton* m_pCtrlWinFloating;
    wxCheckBox* m_pCtrlDisableTaint;
    bool m_bRunConsole;
    bool m_bRunXclients;
    bool m_bRunCustom;
    bool m_bVirtualDesktop;
    bool m_bFloatingWindow;
    bool m_bDisableTaint;
    wxString m_sCustomCommand;
////@end X11PropertyDialog member variables

    MxXmlConfig *m_pCfg;
};

#endif
    // _X11PROPERTYDIALOG_H_
