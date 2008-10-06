// $Id$
//
// Copyright (C) 2006 The OpenNX Team
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
#include "wx/bmpcbox.h"
#include "wx/valgen.h"
#include "wx/statline.h"
////@end includes

#include "WinShare.h"
#include "MyXmlConfig.h"

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxBitmapComboBox;
////@end forward declarations
class MyXmlConfig;
class SharedResource;

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_DIALOG_SHARE_ADD 10098
#define SYMBOL_SMBSHAREPROPERTIES_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_SMBSHAREPROPERTIES_TITLE _("New shared resource - OpenNX")
#define SYMBOL_SMBSHAREPROPERTIES_IDNAME ID_DIALOG_SHARE_ADD
#define SYMBOL_SMBSHAREPROPERTIES_SIZE wxSize(114, 75)
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
    void SetConfig(MyXmlConfig *);
    void SetCurrentShare(int);
    void SetUse(bool, bool);

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_SMBSHAREPROPERTIES_IDNAME, const wxString& caption = SYMBOL_SMBSHAREPROPERTIES_TITLE, const wxPoint& pos = SYMBOL_SMBSHAREPROPERTIES_POSITION, const wxSize& size = SYMBOL_SMBSHAREPROPERTIES_SIZE, long style = SYMBOL_SMBSHAREPROPERTIES_STYLE );

private:
    void askForDefault(ArrayOfShareGroups &, ShareGroup &);

    /// Creates the controls and sizers
    void CreateControls();

////@begin SmbShareProperties event handler declarations

    /// wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_COMBOBOX_SHARE_LOCALNAME
    void OnComboboxShareLocalnameSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXTCTRL_SMBPRINT_USERNAME
    void OnTextctrlSmbprintUsernameUpdated( wxCommandEvent& event );

    /// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXTCTRL_SMBPRINT_PASSWORD
    void OnTextctrlSmbprintPasswordUpdated( wxCommandEvent& event );

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
    wxBitmapComboBox* m_pCtrlLocalShares;
    wxPanel* m_pCtrlSmbPrintOptions;
    wxComboBox* m_pCtrlSmbDriver;
    wxRadioButton* m_pCtrlSmbPrivate;
    wxRadioButton* m_pCtrlSmbPublic;
    wxTextCtrl* m_pCtrlSmbPrintUsername;
    wxTextCtrl* m_pCtrlSmbPrintPassword;
    wxPanel* m_pCtrlCupsOptions;
    wxRadioButton* m_pCtrlCupsPrivate;
    wxRadioButton* m_pCtrlCupsPublic;
    wxPanel* m_pCtrlSmbDiskOptions;
    wxTextCtrl* m_pCtrlMountPoint;
    wxTextCtrl* m_pCtrlUsername;
    wxTextCtrl* m_pCtrlPassword;
private:
    wxString m_sMountPoint;
    wxString m_sSmbDiskUsername;
    wxString m_sSmbDiskPassword;
    bool m_bCupsPublic;
    wxString m_sCupsDriver;
    wxString m_sSmbDriver;
    wxString m_sSmbPrintUsername;
    wxString m_sSmbPrintPassword;
    bool m_bSmbPublic;
////@end SmbShareProperties member variables

    int m_iCurrentShare;
    ArrayOfShares m_aShares;
    MyXmlConfig *m_pCfg;
    bool m_bUseSmb;
    bool m_bUseCups;
};

#endif
    // _SMBSHAREPROPERTIES_H_
