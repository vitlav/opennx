/////////////////////////////////////////////////////////////////////////////
// Name:        RdpPropertyDialog.h
// Purpose:     
// Author:      Fritz Elfert
// Modified by: 
// Created:     06/13/04 22:47:58
// RCS-ID:      $Id$
// Copyright:   Copyright 2004 Millenux GmbH
// Licence:     LGPL
/////////////////////////////////////////////////////////////////////////////

#ifndef _RDPPROPERTYDIALOG_H_
#define _RDPPROPERTYDIALOG_H_

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "RdpPropertyDialog.cpp"
#endif

/*!
 * Includes
 */

////@begin includes
#include "RdpPropertyDialog_symbols.h"
#include "wx/xrc/xmlres.h"
#include "wx/valgen.h"
#include "wx/valtext.h"
////@end includes

/*!
 * Forward declarations
 */

class MxXmlConfig;

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_DIALOG_SETTINGS_RDP 10065
#define SYMBOL_RDPPROPERTYDIALOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_RDPPROPERTYDIALOG_TITLE _("RDP Session Settings - OpenNX")
#define SYMBOL_RDPPROPERTYDIALOG_IDNAME ID_DIALOG_SETTINGS_RDP
#define SYMBOL_RDPPROPERTYDIALOG_SIZE wxSize(200, 150)
#define SYMBOL_RDPPROPERTYDIALOG_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * RdpPropertyDialog class declaration
 */

class RdpPropertyDialog: public wxDialog
{    
    DECLARE_CLASS( RdpPropertyDialog )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    RdpPropertyDialog( );
    RdpPropertyDialog( wxWindow* parent, wxWindowID id = SYMBOL_RDPPROPERTYDIALOG_IDNAME, const wxString& caption = SYMBOL_RDPPROPERTYDIALOG_TITLE, const wxPoint& pos = SYMBOL_RDPPROPERTYDIALOG_POSITION, const wxSize& size = SYMBOL_RDPPROPERTYDIALOG_SIZE, long style = SYMBOL_RDPPROPERTYDIALOG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_RDPPROPERTYDIALOG_IDNAME, const wxString& caption = SYMBOL_RDPPROPERTYDIALOG_TITLE, const wxPoint& pos = SYMBOL_RDPPROPERTYDIALOG_POSITION, const wxSize& size = SYMBOL_RDPPROPERTYDIALOG_SIZE, long style = SYMBOL_RDPPROPERTYDIALOG_STYLE );

    void SetConfig(MxXmlConfig *);
    void UpdateDialogConstraints();

private:
    /// Creates the controls and sizers
    void CreateControls();

////@begin RdpPropertyDialog event handler declarations

    /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_RDP_WINLOGON
    void OnRadiobuttonRdpWinlogonSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_RDP_MXAUTH
    void OnRadiobuttonRdpMxauthSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_RDP_AUTOLOGIN
    void OnRadiobuttonRdpAutologinSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_RDP_DESKTOP
    void OnRadiobuttonRdpDesktopSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_RDP_RUNAPP
    void OnRadiobuttonRdpRunappSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
    void OnOkClick( wxCommandEvent& event );

////@end RdpPropertyDialog event handler declarations

public:
////@begin RdpPropertyDialog member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end RdpPropertyDialog member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

private:
////@begin RdpPropertyDialog member variables
    wxRadioButton* m_pCtrlAutologin;
    wxTextCtrl* m_pCtrlUsername;
    wxTextCtrl* m_pCtrlPassword;
    wxCheckBox* m_pCtrlRememberPassword;
    wxRadioButton* m_pCtrlRunApplication;
    wxTextCtrl* m_pCtrlApplicationString;
private:
    wxString m_sHostname;
    wxString m_sUsername;
    wxString m_sPassword;
    wxString m_sRunCommand;
    bool m_bAutoLogin;
    bool m_bRememberPassword;
    bool m_bShowWinLogon;
    bool m_bUseMxAuth;
    bool m_bRunDesktop;
    bool m_bRunApplication;
    wxString m_sRdpDomain;
////@end RdpPropertyDialog member variables

    MxXmlConfig *m_pCfg;
};

#endif
    // _RDPPROPERTYDIALOG_H_
