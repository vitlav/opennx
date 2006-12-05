/////////////////////////////////////////////////////////////////////////////
// Name:        X11PropertyDialog.cpp
// Purpose:     
// Author:      Fritz Elfert
// Modified by: 
// Created:     06/25/04 00:45:06
// RCS-ID:      $Id: X11PropertyDialog.cpp 821 2004-08-08 00:32:08Z felfert $
// Copyright:   Copyright 2004 Millenux GmbH
// Licence:     LGPL
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma implementation "X11PropertyDialog.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include "X11PropertyDialog.h"
#include "MxXmlConfig.h"
#include "Icon.h"

////@begin XPM images
////@end XPM images

/*!
 * X11PropertyDialog type definition
 */

IMPLEMENT_CLASS( X11PropertyDialog, wxDialog )

/*!
 * X11PropertyDialog event table definition
 */

BEGIN_EVENT_TABLE( X11PropertyDialog, wxDialog )

////@begin X11PropertyDialog event table entries
    EVT_RADIOBUTTON( XRCID("ID_RADIOBUTTON_X11_CONSOLE"), X11PropertyDialog::OnRadiobuttonX11ConsoleSelected )

    EVT_RADIOBUTTON( XRCID("ID_RADIOBUTTON_X11_XCLIENTS"), X11PropertyDialog::OnRadiobuttonX11XclientsSelected )

    EVT_RADIOBUTTON( XRCID("ID_RADIOBUTTON_X11_CUSTOMCMD"), X11PropertyDialog::OnRadiobuttonX11CustomcmdSelected )

    EVT_RADIOBUTTON( XRCID("ID_RADIOBUTTON_X11_VDESKTOP"), X11PropertyDialog::OnRadiobuttonX11VdesktopSelected )

    EVT_RADIOBUTTON( XRCID("ID_RADIOBUTTON_X11_WIN_FLOATING"), X11PropertyDialog::OnRadiobuttonX11WinFloatingSelected )

    EVT_BUTTON( wxID_OK, X11PropertyDialog::OnOkClick )

////@end X11PropertyDialog event table entries

END_EVENT_TABLE()

/*!
 * X11PropertyDialog constructors
 */

X11PropertyDialog::X11PropertyDialog( )
    : m_pCfg(NULL)
{
}

X11PropertyDialog::X11PropertyDialog( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
    : m_pCfg(NULL)
{
    Create(parent, id, caption, pos, size, style);
}

void X11PropertyDialog::SetConfig(MxXmlConfig *cfg)
{
    m_pCfg = cfg;
}

/*!
 * X11PropertyDialog creator
 */

bool X11PropertyDialog::Create( wxWindow* parent, wxWindowID WXUNUSED(id), const wxString& WXUNUSED(caption), const wxPoint& WXUNUSED(pos), const wxSize& WXUNUSED(size), long WXUNUSED(style) )
{
////@begin X11PropertyDialog member initialisation
    m_pCtrlRunCustomCommand = NULL;
    m_pCtrlCustomCommand = NULL;
    m_pCtrlWinFloating = NULL;
    m_pCtrlDisableTaint = NULL;
////@end X11PropertyDialog member initialisation
    wxASSERT_MSG(m_pCfg, _T("X11PropertyDialog::Create: No configuration"));
    if (m_pCfg) {
        m_bRunConsole = m_pCfg->bGetRunConsole();
        m_bRunXclients = m_pCfg->bGetRunXclients();
        m_bRunCustom = ((!m_bRunConsole) && (!m_bRunXclients));
        m_sCustomCommand = m_pCfg->sGetCommandLine();
        m_bVirtualDesktop = m_pCfg->bGetVirtualDesktop();
        m_bFloatingWindow = !m_bVirtualDesktop;
        m_bDisableTaint = m_pCfg->bGetDisableTaint();
    }

////@begin X11PropertyDialog creation
    SetExtraStyle(GetExtraStyle()|wxWS_EX_BLOCK_EVENTS);
    SetParent(parent);
    CreateControls();
    SetIcon(this->GetIconResource(wxT("res/nx.png")));
    Centre();
////@end X11PropertyDialog creation
    return TRUE;
}

/*!
 * Control creation for X11PropertyDialog
 */

void X11PropertyDialog::CreateControls()
{    
////@begin X11PropertyDialog content construction

    wxXmlResource::Get()->LoadDialog(this, GetParent(), _T("ID_DIALOG_SETTINGS_X11"));
    if (FindWindow(XRCID("ID_RADIOBUTTON_X11_CUSTOMCMD")))
        m_pCtrlRunCustomCommand = wxDynamicCast(FindWindow(XRCID("ID_RADIOBUTTON_X11_CUSTOMCMD")), wxRadioButton);
    if (FindWindow(XRCID("ID_TEXTCTRL_X11_CUSTOMCMD")))
        m_pCtrlCustomCommand = wxDynamicCast(FindWindow(XRCID("ID_TEXTCTRL_X11_CUSTOMCMD")), wxTextCtrl);
    if (FindWindow(XRCID("ID_RADIOBUTTON_X11_WIN_FLOATING")))
        m_pCtrlWinFloating = wxDynamicCast(FindWindow(XRCID("ID_RADIOBUTTON_X11_WIN_FLOATING")), wxRadioButton);
    if (FindWindow(XRCID("ID_CHECKBOX_X11_DISABLE_TAINT")))
        m_pCtrlDisableTaint = wxDynamicCast(FindWindow(XRCID("ID_CHECKBOX_X11_DISABLE_TAINT")), wxCheckBox);

    // Set validators
    if (FindWindow(XRCID("ID_RADIOBUTTON_X11_CONSOLE")))
        FindWindow(XRCID("ID_RADIOBUTTON_X11_CONSOLE"))->SetValidator( wxGenericValidator(& m_bRunConsole) );
    if (FindWindow(XRCID("ID_RADIOBUTTON_X11_XCLIENTS")))
        FindWindow(XRCID("ID_RADIOBUTTON_X11_XCLIENTS"))->SetValidator( wxGenericValidator(& m_bRunXclients) );
    if (FindWindow(XRCID("ID_RADIOBUTTON_X11_CUSTOMCMD")))
        FindWindow(XRCID("ID_RADIOBUTTON_X11_CUSTOMCMD"))->SetValidator( wxGenericValidator(& m_bRunCustom) );
    if (FindWindow(XRCID("ID_TEXTCTRL_X11_CUSTOMCMD")))
        FindWindow(XRCID("ID_TEXTCTRL_X11_CUSTOMCMD"))->SetValidator( wxGenericValidator(& m_sCustomCommand) );
    if (FindWindow(XRCID("ID_RADIOBUTTON_X11_VDESKTOP")))
        FindWindow(XRCID("ID_RADIOBUTTON_X11_VDESKTOP"))->SetValidator( wxGenericValidator(& m_bVirtualDesktop) );
    if (FindWindow(XRCID("ID_RADIOBUTTON_X11_WIN_FLOATING")))
        FindWindow(XRCID("ID_RADIOBUTTON_X11_WIN_FLOATING"))->SetValidator( wxGenericValidator(& m_bFloatingWindow) );
    if (FindWindow(XRCID("ID_CHECKBOX_X11_DISABLE_TAINT")))
        FindWindow(XRCID("ID_CHECKBOX_X11_DISABLE_TAINT"))->SetValidator( wxGenericValidator(& m_bDisableTaint) );
////@end X11PropertyDialog content construction

    // Create custom windows not generated automatically here.

////@begin X11PropertyDialog content initialisation

////@end X11PropertyDialog content initialisation
    UpdateDialogConstraints(false);
}

/*!
 * Should we show tooltips?
 */

bool X11PropertyDialog::ShowToolTips()
{
    return TRUE;
}

/*!
 * Get bitmap resources
 */

wxBitmap X11PropertyDialog::GetBitmapResource( const wxString& WXUNUSED(name) )
{
    // Bitmap retrieval
////@begin X11PropertyDialog bitmap retrieval
    return wxNullBitmap;
////@end X11PropertyDialog bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon X11PropertyDialog::GetIconResource( const wxString& name )
{
    // Icon retrieval
    return CreateIconFromFile(name);
}

/*!
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_X11_CONSOLE
 */

void X11PropertyDialog::OnRadiobuttonX11ConsoleSelected( wxCommandEvent& event )
{
    UpdateDialogConstraints(true);
    event.Skip();
}

/*!
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_X11_XCLIENTS
 */

void X11PropertyDialog::OnRadiobuttonX11XclientsSelected( wxCommandEvent& event )
{
    UpdateDialogConstraints(true);
    event.Skip();
}

/*!
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_X11_CUSTOMCMD
 */

void X11PropertyDialog::OnRadiobuttonX11CustomcmdSelected( wxCommandEvent& event )
{
    UpdateDialogConstraints(true);
    event.Skip();
}

/*!
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_X11_VDESKTOP
 */

void X11PropertyDialog::OnRadiobuttonX11VdesktopSelected( wxCommandEvent& event )
{
    UpdateDialogConstraints(true);
    event.Skip();
}

/*!
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_X11_WIN_FLOATING
 */

void X11PropertyDialog::OnRadiobuttonX11WinFloatingSelected( wxCommandEvent& event )
{
    UpdateDialogConstraints(true);
    event.Skip();
}

void X11PropertyDialog::UpdateDialogConstraints(bool getValues)
{
    if (getValues)
        TransferDataFromWindow();

    m_pCtrlCustomCommand->Enable(m_bRunCustom);
    m_pCtrlDisableTaint->Enable(m_bFloatingWindow);
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void X11PropertyDialog::OnOkClick( wxCommandEvent& event )
{
    wxASSERT_MSG(m_pCfg, _T("X11PropertyDialog::OnOkClick: No configuration"));
    if (m_pCfg) {
        m_pCfg->bSetRunConsole(m_bRunConsole);
        m_pCfg->bSetRunXclients(m_bRunXclients);
        m_pCfg->sSetCommandLine(m_sCustomCommand);
        m_pCfg->bSetVirtualDesktop(m_bVirtualDesktop);
        m_pCfg->bSetDisableTaint(m_bDisableTaint);
    }
    event.Skip();
}


