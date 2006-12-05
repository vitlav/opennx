/////////////////////////////////////////////////////////////////////////////
// Name:        SmbShareProperties.h
// Purpose:     
// Author:      Fritz Elfert
// Modified by: 
// Created:     08/01/04 16:35:50
// RCS-ID:      $Id: SmbShareProperties.h 799 2004-08-04 04:19:35Z felfert $
// Copyright:   Copyright 2004 Millenux GmbH
// Licence:     LGPL
/////////////////////////////////////////////////////////////////////////////

#ifndef _SMBSHAREPROPERTIES_H_
#define _SMBSHAREPROPERTIES_H_

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "SmbShareProperties.cpp"
#endif

/*!
 * Includes
 */

////@begin includes
#include "SmbShareProperties_symbols.h"
#include "wx/xrc/xmlres.h"
#include "wx/statline.h"
////@end includes

#include "WinShare.h"

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
#define ID_DIALOG_SHARE_ADD 10098
#define SYMBOL_SMBSHAREPROPERTIES_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_SMBSHAREPROPERTIES_TITLE _("New SMB Share - MXclient")
#define SYMBOL_SMBSHAREPROPERTIES_IDNAME ID_DIALOG_SHARE_ADD
#define SYMBOL_SMBSHAREPROPERTIES_SIZE wxSize(228, 150)
#define SYMBOL_SMBSHAREPROPERTIES_POSITION wxDefaultPosition
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
 * SmbShareProperties class declaration
 */

class SmbShareProperties: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( SmbShareProperties )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    SmbShareProperties( );
    SmbShareProperties( wxWindow* parent, wxWindowID id = SYMBOL_SMBSHAREPROPERTIES_IDNAME, const wxString& caption = SYMBOL_SMBSHAREPROPERTIES_TITLE, const wxPoint& pos = SYMBOL_SMBSHAREPROPERTIES_POSITION, const wxSize& size = SYMBOL_SMBSHAREPROPERTIES_SIZE, long style = SYMBOL_SMBSHAREPROPERTIES_STYLE );
    void SetConfig(MxXmlConfig *);
    void SetCurrentShare(int);

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_SMBSHAREPROPERTIES_IDNAME, const wxString& caption = SYMBOL_SMBSHAREPROPERTIES_TITLE, const wxPoint& pos = SYMBOL_SMBSHAREPROPERTIES_POSITION, const wxSize& size = SYMBOL_SMBSHAREPROPERTIES_SIZE, long style = SYMBOL_SMBSHAREPROPERTIES_STYLE );

private:
    /// Creates the controls and sizers
    void CreateControls();

////@begin SmbShareProperties event handler declarations

    /// wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_COMBOBOX_SHARE_LOCALNAME
    void OnComboboxShareLocalnameSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXTCTRL_SHARE_MOUNTPOINT
    void OnTextctrlShareMountpointUpdated( wxCommandEvent& event );

    /// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXTCTRL_SHARE_USERNAME
    void OnTextctrlShareUsernameUpdated( wxCommandEvent& event );

    /// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXTCTRL_SHARE_PASSWORD
    void OnTextctrlSharePasswordUpdated( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
    void OnOkClick( wxCommandEvent& event );

////@end SmbShareProperties event handler declarations

////@begin SmbShareProperties member function declarations


    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end SmbShareProperties member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin SmbShareProperties member variables
    wxComboBox* m_pCtrlLocalShares;
    wxTextCtrl* m_pCtrlMountPoint;
    wxTextCtrl* m_pCtrlUsername;
    wxTextCtrl* m_pCtrlPassword;
    wxString m_sMountPoint;
    wxString m_sUsername;
    wxString m_sPassword;
////@end SmbShareProperties member variables

    int m_iCurrentShare;
    ArrayOfShares m_aShares;
    MxXmlConfig *m_pCfg;
};

#endif
    // _SMBSHAREPROPERTIES_H_
