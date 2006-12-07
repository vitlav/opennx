/////////////////////////////////////////////////////////////////////////////
// Name:        VncPropertyDialog.h
// Purpose:     
// Author:      Fritz Elfert
// Modified by: 
// Created:     06/13/04 22:47:28
// RCS-ID:      $Id$
// Copyright:   Copyright 2004 Millenux GmbH
// Licence:     LGPL
/////////////////////////////////////////////////////////////////////////////

#ifndef _VNCPROPERTYDIALOG_H_
#define _VNCPROPERTYDIALOG_H_

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "VncPropertyDialog.cpp"
#endif

/*!
 * Includes
 */

////@begin includes
#include "VncPropertyDialog_symbols.h"
#include "wx/xrc/xmlres.h"
#include "wx/valtext.h"
#include "wx/valgen.h"
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
#define ID_DIALOG_SETTINGS_VNC 10057
#define SYMBOL_VNCPROPERTYDIALOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_VNCPROPERTYDIALOG_TITLE _("VNC Session Settings - OpenNX")
#define SYMBOL_VNCPROPERTYDIALOG_IDNAME ID_DIALOG_SETTINGS_VNC
#define SYMBOL_VNCPROPERTYDIALOG_SIZE wxSize(200, 150)
#define SYMBOL_VNCPROPERTYDIALOG_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * VncPropertyDialog class declaration
 */

class VncPropertyDialog: public wxDialog
{    
    DECLARE_CLASS( VncPropertyDialog )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    VncPropertyDialog( );
    VncPropertyDialog( wxWindow* parent, wxWindowID id = SYMBOL_VNCPROPERTYDIALOG_IDNAME, const wxString& caption = SYMBOL_VNCPROPERTYDIALOG_TITLE, const wxPoint& pos = SYMBOL_VNCPROPERTYDIALOG_POSITION, const wxSize& size = SYMBOL_VNCPROPERTYDIALOG_SIZE, long style = SYMBOL_VNCPROPERTYDIALOG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_VNCPROPERTYDIALOG_IDNAME, const wxString& caption = SYMBOL_VNCPROPERTYDIALOG_TITLE, const wxPoint& pos = SYMBOL_VNCPROPERTYDIALOG_POSITION, const wxSize& size = SYMBOL_VNCPROPERTYDIALOG_SIZE, long style = SYMBOL_VNCPROPERTYDIALOG_STYLE );

    void SetConfig(MxXmlConfig *);

private:
    /// Creates the controls and sizers
    void CreateControls();

////@begin VncPropertyDialog event handler declarations

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
    void OnOkClick( wxCommandEvent& event );

////@end VncPropertyDialog event handler declarations

public:
////@begin VncPropertyDialog member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end VncPropertyDialog member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

private:
////@begin VncPropertyDialog member variables
    wxTextCtrl* m_pCtrlPassword;
    wxCheckBox* m_pCtrlRememberPassword;
    wxString m_sPassword;
    wxString m_sHostname;
    int m_iDisplayNumber;
    bool m_bRememberPassword;
////@end VncPropertyDialog member variables

    MxXmlConfig *m_pCfg;

};

#endif
    // _VNCPROPERTYDIALOG_H_
