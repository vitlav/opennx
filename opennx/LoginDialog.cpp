/////////////////////////////////////////////////////////////////////////////
// $Id: LoginDialog.cpp 883 2004-11-04 21:15:18Z felfert $
//
// Author:      Fritz Elfert
// Created:     06/13/04 22:44:20
// Copyright:   Copyright 2004 Millenux GmbH
// Licence:     LGPL
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma implementation "LoginDialog.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/config.h>
#include <wx/dir.h>
#include <wx/xml/xml.h>

////@begin includes
////@end includes

#include "LoginDialog.h"
#include "MxXmlConfig.h"
#include "SessionProperties.h"
#include "MxSession.h"
#include "Icon.h"

////@begin XPM images
////@end XPM images

#ifdef MYTRACETAG
# undef MYTRACETAG
#endif
#define MYTRACETAG wxT("LoginDialog")

/*!
 * LoginDialog type definition
 */

IMPLEMENT_CLASS( LoginDialog, wxDialog )

    /*!
     * LoginDialog event table definition
     */

BEGIN_EVENT_TABLE( LoginDialog, wxDialog )

    ////@begin LoginDialog event table entries
    EVT_COMBOBOX( XRCID("ID_COMBOBOX_SESSION"), LoginDialog::OnComboboxSessionSelected )

    EVT_CHECKBOX( XRCID("ID_CHECKBOX_SMARTCARD"), LoginDialog::OnCheckboxSmartcardClick )

    EVT_BUTTON( XRCID("ID_BUTTON_CONFIGURE"), LoginDialog::OnButtonConfigureClick )

EVT_BUTTON( wxID_OK, LoginDialog::OnOkClick )

    ////@end LoginDialog event table entries

END_EVENT_TABLE()

    /*!
     * LoginDialog constructors
     */

    LoginDialog::LoginDialog( )
: m_pCurrentCfg(NULL)
{
}

    LoginDialog::LoginDialog( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
: m_pCurrentCfg(NULL)
{
    Create(parent, id, caption, pos, size, style);
}

LoginDialog::~LoginDialog()
{
    if (m_pCurrentCfg)
        delete m_pCurrentCfg;
}

void LoginDialog::ReadConfigDirectory()
{
    wxString cfgdir;
    wxConfigBase::Get()->Read(wxT("Config/UserMxDir"), &cfgdir);
    cfgdir = cfgdir + wxFileName::GetPathSeparator() + wxT("config");
    m_aConfigFiles.Empty();
    m_aSessionNames.Empty();

    wxDir::GetAllFiles(cfgdir, &m_aConfigFiles, wxT("*.conf"), wxDIR_FILES);
    size_t i;
    if (m_pCurrentCfg)
        delete m_pCurrentCfg;
    m_pCurrentCfg = NULL;
    for (i = 0; i < m_aConfigFiles.GetCount(); i++) {
        MxXmlConfig cfg(m_aConfigFiles[i]);
        if (cfg.IsValid()) {
            if (m_sSessionName.IsEmpty())
                m_sSessionName = cfg.sGetName();
            m_aSessionNames.Add(cfg.sGetName());
            if (cfg.sGetName() == m_sSessionName) {
                m_pCurrentCfg = new MxXmlConfig(m_aConfigFiles[i]);
                m_sUsername = cfg.sGetUsername();
                m_sPassword = cfg.sGetPassword();
                m_bUseSmartCard = cfg.bGetUseSmartCard();
            }
        }
    }
    if (m_aSessionNames.GetCount() == 0) {
        m_aSessionNames.Add(wxT(""));
        m_sSessionName = wxT("");
    } else {
        if (!m_pCurrentCfg) {
            m_sSessionName = wxT("");
            ReadConfigDirectory();
        }
    }
}

/*!
 * LoginDialog creator
 */

bool LoginDialog::Create( wxWindow* parent, wxWindowID WXUNUSED(id), const wxString& WXUNUSED(caption), const wxPoint& WXUNUSED(pos), const wxSize& WXUNUSED(size), long WXUNUSED(style) )
{
    ////@begin LoginDialog member initialisation
    m_pCtrlUsername = NULL;
    m_pCtrlPassword = NULL;
    m_pCtrlSessionName = NULL;
    m_pCtrlUseSmartCard = NULL;
    ////@end LoginDialog member initialisation

    ////@begin LoginDialog creation
    SetExtraStyle(GetExtraStyle()|wxWS_EX_BLOCK_EVENTS);
    SetParent(parent);
    CreateControls();
    SetIcon(this->GetIconResource(wxT("res/nx.png")));
    GetSizer()->Fit(this);
    GetSizer()->SetSizeHints(this);
    Centre();
    ////@end LoginDialog creation
    return TRUE;
}

/*!
 * Control creation for LoginDialog
 */

void LoginDialog::CreateControls()
{

    ReadConfigDirectory();

    ////@begin LoginDialog content construction

    wxXmlResource::Get()->LoadDialog(this, GetParent(), wxT("ID_DIALOG_LOGIN"));
    if (FindWindow(XRCID("ID_TEXTCTRL_USERNAME")))
        m_pCtrlUsername = wxDynamicCast(FindWindow(XRCID("ID_TEXTCTRL_USERNAME")), wxTextCtrl);
    if (FindWindow(XRCID("ID_TEXTCTRL_PASSWORD")))
        m_pCtrlPassword = wxDynamicCast(FindWindow(XRCID("ID_TEXTCTRL_PASSWORD")), wxTextCtrl);
    if (FindWindow(XRCID("ID_COMBOBOX_SESSION")))
        m_pCtrlSessionName = wxDynamicCast(FindWindow(XRCID("ID_COMBOBOX_SESSION")), wxComboBox);
    if (FindWindow(XRCID("ID_CHECKBOX_SMARTCARD")))
        m_pCtrlUseSmartCard = wxDynamicCast(FindWindow(XRCID("ID_CHECKBOX_SMARTCARD")), wxCheckBox);

    // Set validators
    if (FindWindow(XRCID("ID_TEXTCTRL_USERNAME")))
        FindWindow(XRCID("ID_TEXTCTRL_USERNAME"))->SetValidator( wxGenericValidator(& m_sUsername) );
    if (FindWindow(XRCID("ID_TEXTCTRL_PASSWORD")))
        FindWindow(XRCID("ID_TEXTCTRL_PASSWORD"))->SetValidator( wxGenericValidator(& m_sPassword) );
    if (FindWindow(XRCID("ID_COMBOBOX_SESSION")))
        FindWindow(XRCID("ID_COMBOBOX_SESSION"))->SetValidator( wxGenericValidator(& m_sSessionName) );
    if (FindWindow(XRCID("ID_CHECKBOX_SMARTCARD")))
        FindWindow(XRCID("ID_CHECKBOX_SMARTCARD"))->SetValidator( wxGenericValidator(& m_bUseSmartCard) );
    ////@end LoginDialog content construction

    // Create custom windows not generated automatically here.

    ////@begin LoginDialog content initialisation

    ////@end LoginDialog content initialisation

    m_pCtrlSessionName->Append(m_aSessionNames);
}

/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_SMARTCARD
 */

void LoginDialog::OnCheckboxSmartcardClick( wxCommandEvent& event )
{
    // Insert custom code here
    event.Skip();
}


/*!
 * Should we show tooltips?
 */

bool LoginDialog::ShowToolTips()
{
    return TRUE;
}

/*!
 * Get bitmap resources
 */

wxBitmap LoginDialog::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
    return CreateBitmapFromFile(name);
}

/*!
 * Get icon resources
 */

wxIcon LoginDialog::GetIconResource( const wxString& name )
{
    // Icon retrieval
    return CreateIconFromFile(name);
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_CONFIGURE
 */

void LoginDialog::OnButtonConfigureClick( wxCommandEvent& event )
{
    // Insert custom code here
    SessionProperties d;
    d.SetConfig(m_pCurrentCfg);
    wxString fn = m_pCurrentCfg->sGetFileName();
    d.Create(this);
    switch (d.ShowModal()) {
        case wxID_CANCEL:
            delete m_pCurrentCfg;
            m_pCurrentCfg = new MxXmlConfig(fn);
            break;
        case wxID_CLEAR:
            ::wxLogTrace(MYTRACETAG, wxT("deleting '%s'"), fn.c_str());
            ::wxRemoveFile(fn);
            m_sSessionName.Empty();
            m_pCtrlSessionName->Clear();
            ReadConfigDirectory();
            m_pCtrlSessionName->Append(m_aSessionNames);
            if (!m_sSessionName.IsEmpty())
                m_pCtrlSessionName->SetValue(m_sSessionName);
            break;
        case wxID_OK:
            m_bUseSmartCard = m_pCurrentCfg->bGetUseSmartCard();
            m_pCtrlUseSmartCard->SetValue(m_bUseSmartCard);
            if (!m_pCurrentCfg->SaveToFile())
                wxMessageBox(wxString::Format(_("Could not save session to\n%s"),
                            m_pCurrentCfg->sGetFileName().c_str()), _("Error saving - MXclient"),
                        wxICON_ERROR | wxOK);
            wxConfigBase::Get()->Write(wxT("Config/UserMxDir"), d.GetsUserMxDir());
            wxConfigBase::Get()->Write(wxT("Config/SystemMxDir"), d.GetsSystemMxDir());
            break;
    }
    event.Skip();
}

/*!
 * wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_COMBOBOX_SESSION
 */

void LoginDialog::OnComboboxSessionSelected( wxCommandEvent& event )
{
    wxString sessionName = event.GetString();
    if (m_pCurrentCfg)
        delete m_pCurrentCfg;
    for (size_t i = 0; i < m_aConfigFiles.GetCount(); i++) {
        MxXmlConfig cfg(m_aConfigFiles[i]);
        if (cfg.IsValid()) {
            if (cfg.sGetName() == sessionName) {
                m_pCurrentCfg = new MxXmlConfig(m_aConfigFiles[i]);
                m_pCtrlUsername->SetValue(cfg.sGetUsername());
                m_pCtrlPassword->SetValue(cfg.sGetPassword());
                m_pCtrlUseSmartCard->SetValue(cfg.bGetUseSmartCard());
            }
        }
    }
    event.Skip();
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void LoginDialog::OnOkClick( wxCommandEvent& event )
{
    if (m_pCurrentCfg) {
        TransferDataFromWindow();
        m_pCurrentCfg->sSetUsername(m_sUsername);
        if (m_pCurrentCfg->bGetRememberPassword())
            m_pCurrentCfg->sSetPassword(m_sPassword);
        m_pCurrentCfg->bSetUseSmartCard(m_bUseSmartCard);
        if (m_bUseSmartCard)
            m_pCurrentCfg->bSetEnableSSL(true);

        if (!m_pCurrentCfg->SaveToFile())
            wxMessageBox(wxString::Format(_("Could not save session to\n%s"),
                        m_pCurrentCfg->sGetFileName().c_str()), _("Error saving - MXclient"),
                    wxICON_ERROR | wxOK);
        else {
            MxSession s;
            s.Create(m_pCurrentCfg->sGetFileName(), m_sPassword);
        }
    }
    event.Skip();
}
