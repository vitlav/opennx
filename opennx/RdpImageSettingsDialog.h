/////////////////////////////////////////////////////////////////////////////
// Name:        RdpImageSettingsDialog.h
// Purpose:     
// Author:      Fritz Elfert
// Modified by: 
// Created:     06/13/04 22:48:38
// RCS-ID:      $Id: RdpImageSettingsDialog.h 799 2004-08-04 04:19:35Z felfert $
// Copyright:   Copyright 2004 Millenux GmbH
// Licence:     LGPL
/////////////////////////////////////////////////////////////////////////////

#ifndef _RDPIMAGESETTINGSDIALOG_H_
#define _RDPIMAGESETTINGSDIALOG_H_

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "RdpImageSettingsDialog.cpp"
#endif

/*!
 * Includes
 */

////@begin includes
#include "RdpImageSettingsDialog_symbols.h"
#include "wx/xrc/xmlres.h"
#include "wx/valgen.h"
#include "wx/statline.h"
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
#define ID_DIALOG_IMAGE_RDP 10085
#define SYMBOL_RDPIMAGESETTINGSDIALOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_RDPIMAGESETTINGSDIALOG_TITLE _("RDP Image Settings - MXclient")
#define SYMBOL_RDPIMAGESETTINGSDIALOG_IDNAME ID_DIALOG_IMAGE_RDP
#define SYMBOL_RDPIMAGESETTINGSDIALOG_SIZE wxDefaultSize
#define SYMBOL_RDPIMAGESETTINGSDIALOG_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * RdpImageSettingsDialog class declaration
 */

class RdpImageSettingsDialog: public wxDialog
{    
    DECLARE_CLASS( RdpImageSettingsDialog )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    RdpImageSettingsDialog( );
    RdpImageSettingsDialog( wxWindow* parent, wxWindowID id = SYMBOL_RDPIMAGESETTINGSDIALOG_IDNAME, const wxString& caption = SYMBOL_RDPIMAGESETTINGSDIALOG_TITLE, const wxPoint& pos = SYMBOL_RDPIMAGESETTINGSDIALOG_POSITION, const wxSize& size = SYMBOL_RDPIMAGESETTINGSDIALOG_SIZE, long style = SYMBOL_RDPIMAGESETTINGSDIALOG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_RDPIMAGESETTINGSDIALOG_IDNAME, const wxString& caption = SYMBOL_RDPIMAGESETTINGSDIALOG_TITLE, const wxPoint& pos = SYMBOL_RDPIMAGESETTINGSDIALOG_POSITION, const wxSize& size = SYMBOL_RDPIMAGESETTINGSDIALOG_SIZE, long style = SYMBOL_RDPIMAGESETTINGSDIALOG_STYLE );
    void SetConfig(MxXmlConfig *);

private:
    /// Creates the controls and sizers
    void CreateControls();

////@begin RdpImageSettingsDialog event handler declarations

    /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_RDP_ENCODING
    void OnRadiobuttonRdpEncodingSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_RDP_PLAINX
    void OnRadiobuttonRdpPlainxSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
    void OnOkClick( wxCommandEvent& event );

////@end RdpImageSettingsDialog event handler declarations

////@begin RdpImageSettingsDialog member function declarations


    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end RdpImageSettingsDialog member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

    void UpdateDialogConstraints();

////@begin RdpImageSettingsDialog member variables
    wxRadioButton* m_pCtrlRdpEncoding;
    wxCheckBox* m_pCtrlRdpCompressed;
    wxRadioButton* m_pCtrlPlainX;
    bool m_bRdpPlainX;
    bool m_bRdpEncoding;
    bool m_bRdpCompressed;
////@end RdpImageSettingsDialog member variables

    MxXmlConfig *m_pCfg;
};

#endif
    // _RDPIMAGESETTINGSDIALOG_H_
