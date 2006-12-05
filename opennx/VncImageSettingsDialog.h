/////////////////////////////////////////////////////////////////////////////
// Name:        VncImageSettingsDialog.h
// Purpose:     
// Author:      Fritz Elfert
// Modified by: 
// Created:     06/13/04 22:48:26
// RCS-ID:      $Id: VncImageSettingsDialog.h 799 2004-08-04 04:19:35Z felfert $
// Copyright:   Copyright 2004 Millenux GmbH
// Licence:     LGPL
/////////////////////////////////////////////////////////////////////////////

#ifndef _VNCIMAGESETTINGSDIALOG_H_
#define _VNCIMAGESETTINGSDIALOG_H_

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "VncImageSettingsDialog.cpp"
#endif

/*!
 * Includes
 */

////@begin includes
#include "VncImageSettingsDialog_symbols.h"
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
#define ID_DIALOG_IMAGE_VNC 10078
#define SYMBOL_VNCIMAGESETTINGSDIALOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_VNCIMAGESETTINGSDIALOG_TITLE _("VNC Image Settings - MXclient")
#define SYMBOL_VNCIMAGESETTINGSDIALOG_IDNAME ID_DIALOG_IMAGE_VNC
#define SYMBOL_VNCIMAGESETTINGSDIALOG_SIZE wxDefaultSize
#define SYMBOL_VNCIMAGESETTINGSDIALOG_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * VncImageSettingsDialog class declaration
 */

class VncImageSettingsDialog: public wxDialog
{    
    DECLARE_CLASS( VncImageSettingsDialog )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    VncImageSettingsDialog( );
    VncImageSettingsDialog( wxWindow* parent, wxWindowID id = SYMBOL_VNCIMAGESETTINGSDIALOG_IDNAME, const wxString& caption = SYMBOL_VNCIMAGESETTINGSDIALOG_TITLE, const wxPoint& pos = SYMBOL_VNCIMAGESETTINGSDIALOG_POSITION, const wxSize& size = SYMBOL_VNCIMAGESETTINGSDIALOG_SIZE, long style = SYMBOL_VNCIMAGESETTINGSDIALOG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_VNCIMAGESETTINGSDIALOG_IDNAME, const wxString& caption = SYMBOL_VNCIMAGESETTINGSDIALOG_TITLE, const wxPoint& pos = SYMBOL_VNCIMAGESETTINGSDIALOG_POSITION, const wxSize& size = SYMBOL_VNCIMAGESETTINGSDIALOG_SIZE, long style = SYMBOL_VNCIMAGESETTINGSDIALOG_STYLE );
    void SetConfig(MxXmlConfig *);

private:
    /// Creates the controls and sizers
    void CreateControls();

////@begin VncImageSettingsDialog event handler declarations

    /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_VNC_HEXTILE
    void OnRadiobuttonVncHextileSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_VNC_TIGHT
    void OnRadiobuttonVncTightSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_VNC_PLAINX
    void OnRadiobuttonVncPlainxSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
    void OnOkClick( wxCommandEvent& event );

////@end VncImageSettingsDialog event handler declarations

////@begin VncImageSettingsDialog member function declarations


    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end VncImageSettingsDialog member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

    void UpdateDialogConstraints();

////@begin VncImageSettingsDialog member variables
    wxRadioButton* m_pCtrlUseHextile;
    wxRadioButton* m_pCtrlUseTight;
    wxCheckBox* m_pCtrlVncJpeg;
    wxRadioButton* m_pCtrlUsePlainX;
    bool m_bUseHextile;
    bool m_bUsePlainX;
    bool m_bUseTight;
    bool m_bUseTightJpeg;
////@end VncImageSettingsDialog member variables

    MxXmlConfig *m_pCfg;
};

#endif
    // _VNCIMAGESETTINGSDIALOG_H_
