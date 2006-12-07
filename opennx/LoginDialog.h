/////////////////////////////////////////////////////////////////////////////
// Name:        LoginDialog.h
// Purpose:     
// Author:      Fritz Elfert
// Modified by: 
// Created:     06/13/04 22:44:20
// RCS-ID:      $Id$
// Copyright:   Copyright 2004 Millenux GmbH
// Licence:     LGPL
/////////////////////////////////////////////////////////////////////////////

#ifndef _LOGINDIALOG_H_
#define _LOGINDIALOG_H_

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "LoginDialog.cpp"
#endif

/*!
 * Includes
 */

////@begin includes
#include "LoginDialog_symbols.h"
#include "wx/xrc/xmlres.h"
#include "wx/valgen.h"
#include "wx/statline.h"
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
#define ID_DIALOG_LOGIN 10000
#define SYMBOL_LOGINDIALOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_LOGINDIALOG_TITLE _("Login - OpenNX")
#define SYMBOL_LOGINDIALOG_IDNAME ID_DIALOG_LOGIN
#define SYMBOL_LOGINDIALOG_SIZE wxSize(200, 150)
#define SYMBOL_LOGINDIALOG_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * LoginDialog class declaration
 */

class LoginDialog: public wxDialog
{    
    DECLARE_CLASS( LoginDialog )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    LoginDialog( );
    LoginDialog( wxWindow* parent, wxWindowID id = SYMBOL_LOGINDIALOG_IDNAME, const wxString& caption = SYMBOL_LOGINDIALOG_TITLE, const wxPoint& pos = SYMBOL_LOGINDIALOG_POSITION, const wxSize& size = SYMBOL_LOGINDIALOG_SIZE, long style = SYMBOL_LOGINDIALOG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_LOGINDIALOG_IDNAME, const wxString& caption = SYMBOL_LOGINDIALOG_TITLE, const wxPoint& pos = SYMBOL_LOGINDIALOG_POSITION, const wxSize& size = SYMBOL_LOGINDIALOG_SIZE, long style = SYMBOL_LOGINDIALOG_STYLE );

    ~LoginDialog();

private:
    /// Creates the controls and sizers
    void CreateControls();

////@begin LoginDialog event handler declarations

    /// wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_COMBOBOX_SESSION
    void OnComboboxSessionSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_SMARTCARD
    void OnCheckboxSmartcardClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_GUESTLOGIN
    void OnCheckboxGuestloginClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_CONFIGURE
    void OnButtonConfigureClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
    void OnOkClick( wxCommandEvent& event );

////@end LoginDialog event handler declarations

public:
////@begin LoginDialog member function declarations

    wxString GetSessionName() const { return m_sSessionName ; }
    void SetSessionName(wxString value) { m_sSessionName = value ; }

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end LoginDialog member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

private:
    void ReadConfigDirectory();

////@begin LoginDialog member variables
    wxTextCtrl* m_pCtrlUsername;
    wxTextCtrl* m_pCtrlPassword;
    wxComboBox* m_pCtrlSessionName;
    wxCheckBox* m_pCtrlUseSmartCard;
    wxCheckBox* m_pCtrlGuestLogin;
private:
    wxString m_sUsername;
    wxString m_sPassword;
    bool m_bUseSmartCard;
    wxString m_sSessionName;
    bool m_bGuestLogin;
    wxString m_sTmpUsername;
    wxString m_sTmpPassword;
////@end LoginDialog member variables

    MxXmlConfig *m_pCurrentCfg;
    wxArrayString m_aConfigFiles;
    wxArrayString m_aSessionNames;
};

#endif
    // _LOGINDIALOG_H_
