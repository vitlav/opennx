/////////////////////////////////////////////////////////////////////////////
// Name:        RdpPropertyDialog.cpp
// Purpose:     
// Author:      Fritz Elfert
// Modified by: 
// Created:     06/13/04 22:47:58
// RCS-ID:      $Id$
// Copyright:   Copyright 2004 Millenux GmbH
// Licence:     LGPL
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma implementation "RdpPropertyDialog.h"
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

#include "RdpPropertyDialog.h"
#include "Icon.h"
#include "MxXmlConfig.h"
#include "MxValidator.h"

////@begin XPM images
////@end XPM images

/*!
 * RdpPropertyDialog type definition
 */

IMPLEMENT_CLASS( RdpPropertyDialog, wxDialog )

/*!
 * RdpPropertyDialog event table definition
 */

BEGIN_EVENT_TABLE( RdpPropertyDialog, wxDialog )

////@begin RdpPropertyDialog event table entries
    EVT_RADIOBUTTON( XRCID("ID_RADIOBUTTON_RDP_WINLOGON"), RdpPropertyDialog::OnRadiobuttonRdpWinlogonSelected )

    EVT_RADIOBUTTON( XRCID("ID_RADIOBUTTON_RDP_MXAUTH"), RdpPropertyDialog::OnRadiobuttonRdpMxauthSelected )

    EVT_RADIOBUTTON( XRCID("ID_RADIOBUTTON_RDP_AUTOLOGIN"), RdpPropertyDialog::OnRadiobuttonRdpAutologinSelected )

    EVT_RADIOBUTTON( XRCID("ID_RADIOBUTTON_RDP_DESKTOP"), RdpPropertyDialog::OnRadiobuttonRdpDesktopSelected )

    EVT_RADIOBUTTON( XRCID("ID_RADIOBUTTON_RDP_RUNAPP"), RdpPropertyDialog::OnRadiobuttonRdpRunappSelected )

    EVT_BUTTON( wxID_OK, RdpPropertyDialog::OnOkClick )

////@end RdpPropertyDialog event table entries

END_EVENT_TABLE()

/*!
 * RdpPropertyDialog constructors
 */

RdpPropertyDialog::RdpPropertyDialog( )
    : m_pCfg(NULL)
{
}

RdpPropertyDialog::RdpPropertyDialog( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
    : m_pCfg(NULL)
{
    Create(parent, id, caption, pos, size, style);
}

void RdpPropertyDialog::SetConfig(MxXmlConfig *cfg)
{
    m_pCfg = cfg;
}

/*!
 * RdpPropertyDialog creator
 */

bool RdpPropertyDialog::Create( wxWindow* parent, wxWindowID WXUNUSED(id), const wxString& WXUNUSED(caption), const wxPoint& WXUNUSED(pos), const wxSize& WXUNUSED(size), long WXUNUSED(style) )
{
////@begin RdpPropertyDialog member initialisation
    m_pCtrlAutologin = NULL;
    m_pCtrlUsername = NULL;
    m_pCtrlPassword = NULL;
    m_pCtrlRememberPassword = NULL;
    m_pCtrlRunApplication = NULL;
    m_pCtrlApplicationString = NULL;
////@end RdpPropertyDialog member initialisation
    wxASSERT_MSG(m_pCfg, _T("RdpPropertyDialog::Create: No configuration"));
    if (m_pCfg) {
        m_bRememberPassword = m_pCfg->bGetRdpRememberPassword();
        m_bRunApplication = m_pCfg->bGetRdpRunApplication();
        m_bRunDesktop = !m_bRunApplication;
        m_sHostname = m_pCfg->sGetRdpHostName();
        m_sUsername = m_pCfg->sGetRdpUsername();
        m_sPassword = m_pCfg->sGetRdpPassword();
        m_sRunCommand = m_pCfg->sGetRdpApplication();
        int atype = m_pCfg->iGetRdpAuthType();
        switch (atype) {
            case 0:
                m_bShowWinLogon = false;
                m_bAutoLogin = true;
                m_bUseMxAuth = false;
                break;
            case 1:
                m_bShowWinLogon = true;
                m_bAutoLogin = false;
                m_bUseMxAuth = false;
                break;
        }

    }

////@begin RdpPropertyDialog creation
    SetExtraStyle(GetExtraStyle()|wxWS_EX_BLOCK_EVENTS);
    SetParent(parent);
    CreateControls();
    SetIcon(this->GetIconResource(wxT("res/nx.png")));
    GetSizer()->Fit(this);
    GetSizer()->SetSizeHints(this);
    Centre();
////@end RdpPropertyDialog creation
    return TRUE;
}

/*!
 * Control creation for RdpPropertyDialog
 */

void RdpPropertyDialog::CreateControls()
{    
////@begin RdpPropertyDialog content construction

    wxXmlResource::Get()->LoadDialog(this, GetParent(), _T("ID_DIALOG_SETTINGS_RDP"));
    if (FindWindow(XRCID("ID_RADIOBUTTON_RDP_AUTOLOGIN")))
        m_pCtrlAutologin = wxDynamicCast(FindWindow(XRCID("ID_RADIOBUTTON_RDP_AUTOLOGIN")), wxRadioButton);
    if (FindWindow(XRCID("ID_TEXTCTRL_RDP_USERNAME")))
        m_pCtrlUsername = wxDynamicCast(FindWindow(XRCID("ID_TEXTCTRL_RDP_USERNAME")), wxTextCtrl);
    if (FindWindow(XRCID("ID_TEXTCTRL_RDP_PASSWORD")))
        m_pCtrlPassword = wxDynamicCast(FindWindow(XRCID("ID_TEXTCTRL_RDP_PASSWORD")), wxTextCtrl);
    if (FindWindow(XRCID("ID_CHECKBOX_RDP_REMEMBER_PWD")))
        m_pCtrlRememberPassword = wxDynamicCast(FindWindow(XRCID("ID_CHECKBOX_RDP_REMEMBER_PWD")), wxCheckBox);
    if (FindWindow(XRCID("ID_RADIOBUTTON_RDP_RUNAPP")))
        m_pCtrlRunApplication = wxDynamicCast(FindWindow(XRCID("ID_RADIOBUTTON_RDP_RUNAPP")), wxRadioButton);
    if (FindWindow(XRCID("ID_TEXTCTRL_RDP_APPLICATION")))
        m_pCtrlApplicationString = wxDynamicCast(FindWindow(XRCID("ID_TEXTCTRL_RDP_APPLICATION")), wxTextCtrl);

    // Set validators
    if (FindWindow(XRCID("ID_TEXTCTRL_RDP_HOST")))
        FindWindow(XRCID("ID_TEXTCTRL_RDP_HOST"))->SetValidator( wxGenericValidator(& m_sHostname) );
    if (FindWindow(XRCID("ID_RADIOBUTTON_RDP_WINLOGON")))
        FindWindow(XRCID("ID_RADIOBUTTON_RDP_WINLOGON"))->SetValidator( wxGenericValidator(& m_bShowWinLogon) );
    if (FindWindow(XRCID("ID_RADIOBUTTON_RDP_MXAUTH")))
        FindWindow(XRCID("ID_RADIOBUTTON_RDP_MXAUTH"))->SetValidator( wxGenericValidator(& m_bUseMxAuth) );
    if (FindWindow(XRCID("ID_RADIOBUTTON_RDP_AUTOLOGIN")))
        FindWindow(XRCID("ID_RADIOBUTTON_RDP_AUTOLOGIN"))->SetValidator( wxGenericValidator(& m_bAutoLogin) );
    if (FindWindow(XRCID("ID_TEXTCTRL_RDP_USERNAME")))
        FindWindow(XRCID("ID_TEXTCTRL_RDP_USERNAME"))->SetValidator( wxGenericValidator(& m_sUsername) );
    if (FindWindow(XRCID("ID_TEXTCTRL_RDP_PASSWORD")))
        FindWindow(XRCID("ID_TEXTCTRL_RDP_PASSWORD"))->SetValidator( wxGenericValidator(& m_sPassword) );
    if (FindWindow(XRCID("ID_CHECKBOX_RDP_REMEMBER_PWD")))
        FindWindow(XRCID("ID_CHECKBOX_RDP_REMEMBER_PWD"))->SetValidator( wxGenericValidator(& m_bRememberPassword) );
    if (FindWindow(XRCID("ID_RADIOBUTTON_RDP_DESKTOP")))
        FindWindow(XRCID("ID_RADIOBUTTON_RDP_DESKTOP"))->SetValidator( wxGenericValidator(& m_bRunDesktop) );
    if (FindWindow(XRCID("ID_RADIOBUTTON_RDP_RUNAPP")))
        FindWindow(XRCID("ID_RADIOBUTTON_RDP_RUNAPP"))->SetValidator( wxGenericValidator(& m_bRunApplication) );
    if (FindWindow(XRCID("ID_TEXTCTRL_RDP_APPLICATION")))
        FindWindow(XRCID("ID_TEXTCTRL_RDP_APPLICATION"))->SetValidator( wxGenericValidator(& m_sRunCommand) );
////@end RdpPropertyDialog content construction

    // Create custom windows not generated automatically here.

////@begin RdpPropertyDialog content initialisation

////@end RdpPropertyDialog content initialisation
    UpdateDialogConstraints();
}

/*!
 * Should we show tooltips?
 */

bool RdpPropertyDialog::ShowToolTips()
{
    return TRUE;
}

/*!
 * Get bitmap resources
 */

wxBitmap RdpPropertyDialog::GetBitmapResource( const wxString& WXUNUSED(name) )
{
    // Bitmap retrieval
////@begin RdpPropertyDialog bitmap retrieval
    return wxNullBitmap;
////@end RdpPropertyDialog bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon RdpPropertyDialog::GetIconResource( const wxString& name )
{
    // Icon retrieval
    return CreateIconFromFile(name);
}

/*!
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_RDP_AUTOLOGIN
 */

void RdpPropertyDialog::OnRadiobuttonRdpAutologinSelected( wxCommandEvent& event )
{
    m_bAutoLogin = true;
    UpdateDialogConstraints();
    event.Skip();
}

/*!
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_RDP_MXAUTH
 */

void RdpPropertyDialog::OnRadiobuttonRdpMxauthSelected( wxCommandEvent& event )
{
    m_bAutoLogin = false;
    UpdateDialogConstraints();
    event.Skip();
}

/*!
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_RDP_WINLOGON
 */

void RdpPropertyDialog::OnRadiobuttonRdpWinlogonSelected( wxCommandEvent& event )
{
    m_bAutoLogin = false;
    UpdateDialogConstraints();
    event.Skip();
}

/*!
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_RDP_DESKTOP
 */

void RdpPropertyDialog::OnRadiobuttonRdpDesktopSelected( wxCommandEvent& event )
{
    m_bRunApplication = false;
    UpdateDialogConstraints();
    event.Skip();
}

/*!
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_RDP_RUNAPP
 */

void RdpPropertyDialog::OnRadiobuttonRdpRunappSelected( wxCommandEvent& event )
{
    m_bRunApplication = true;
    UpdateDialogConstraints();
    event.Skip();
}

void RdpPropertyDialog::UpdateDialogConstraints()
{
    m_pCtrlUsername->Enable(m_bAutoLogin);
    m_pCtrlPassword->Enable(m_bAutoLogin);
    m_pCtrlRememberPassword->Enable(m_bAutoLogin);
    m_pCtrlApplicationString->Enable(m_bRunApplication);
}
/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void RdpPropertyDialog::OnOkClick( wxCommandEvent& event )
{
    wxASSERT_MSG(m_pCfg, _T("RdpPropertyDialog::OnOkClick: No configuration"));
    if (m_pCfg) {
        TransferDataFromWindow();
        m_pCfg->bSetRdpRememberPassword(m_bRememberPassword);
        m_pCfg->bSetRdpRunApplication(m_bRunApplication);
        m_pCfg->sSetRdpHostName(m_sHostname);
        m_pCfg->sSetRdpUsername(m_sUsername);
        m_pCfg->sSetRdpPassword(m_sPassword);
        m_pCfg->sSetRdpApplication(m_sRunCommand);
        int atype = 0;
        if (m_bShowWinLogon)
            atype = 1;
        if (m_bAutoLogin)
            atype = m_bUseMxAuth ? 2 : 0;
        m_pCfg->iSetRdpAuthType(atype);
    }
    event.Skip();
}


