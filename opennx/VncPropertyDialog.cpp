/////////////////////////////////////////////////////////////////////////////
// Name:        VncPropertyDialog.cpp
// Purpose:     
// Author:      Fritz Elfert
// Modified by: 
// Created:     06/13/04 22:47:28
// RCS-ID:      $Id: VncPropertyDialog.cpp 811 2004-08-05 20:54:14Z felfert $
// Copyright:   Copyright 2004 Millenux GmbH
// Licence:     LGPL
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma implementation "VncPropertyDialog.h"
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

#include "VncPropertyDialog.h"
#include "Icon.h"
#include "MxXmlConfig.h"

////@begin XPM images
////@end XPM images

/*!
 * VncPropertyDialog type definition
 */

IMPLEMENT_CLASS( VncPropertyDialog, wxDialog )

/*!
 * VncPropertyDialog event table definition
 */

BEGIN_EVENT_TABLE( VncPropertyDialog, wxDialog )

////@begin VncPropertyDialog event table entries
    EVT_BUTTON( wxID_OK, VncPropertyDialog::OnOkClick )

////@end VncPropertyDialog event table entries

END_EVENT_TABLE()

/*!
 * VncPropertyDialog constructors
 */

VncPropertyDialog::VncPropertyDialog( )
    : m_pCfg(NULL)
{
}

VncPropertyDialog::VncPropertyDialog( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
    : m_pCfg(NULL)
{
    Create(parent, id, caption, pos, size, style);
}

void VncPropertyDialog::SetConfig(MxXmlConfig *cfg)
{
    m_pCfg = cfg;
}

/*!
 * VncPropertyDialog creator
 */

bool VncPropertyDialog::Create( wxWindow* parent, wxWindowID WXUNUSED(id), const wxString& WXUNUSED(caption), const wxPoint& WXUNUSED(pos), const wxSize& WXUNUSED(size), long WXUNUSED(style) )
{
////@begin VncPropertyDialog member initialisation
    m_pCtrlPassword = NULL;
    m_pCtrlRememberPassword = NULL;
////@end VncPropertyDialog member initialisation

    wxASSERT_MSG(m_pCfg, _T("VncPropertyDialog::Create: No configuration"));
    if (m_pCfg) {
        m_bRememberPassword = m_pCfg->bGetVncRememberPassword();
        m_iDisplayNumber = m_pCfg->iGetVncDisplayNumber();
        m_sHostname = m_pCfg->sGetVncHostName();
        m_sPassword = m_pCfg->sGetVncPassword();
    }

////@begin VncPropertyDialog creation
    SetExtraStyle(GetExtraStyle()|wxWS_EX_BLOCK_EVENTS);
    SetParent(parent);
    CreateControls();
    SetIcon(this->GetIconResource(wxT("res/nx.png")));
    GetSizer()->Fit(this);
    GetSizer()->SetSizeHints(this);
    Centre();
////@end VncPropertyDialog creation
    return TRUE;
}

/*!
 * Control creation for VncPropertyDialog
 */

void VncPropertyDialog::CreateControls()
{    
////@begin VncPropertyDialog content construction

    wxXmlResource::Get()->LoadDialog(this, GetParent(), _T("ID_DIALOG_SETTINGS_VNC"));
    if (FindWindow(XRCID("ID_TEXTCTRL_VNC_PASSWD")))
        m_pCtrlPassword = wxDynamicCast(FindWindow(XRCID("ID_TEXTCTRL_VNC_PASSWD")), wxTextCtrl);
    if (FindWindow(XRCID("ID_CHECKBOX_VNC_REMEMBER_PWD")))
        m_pCtrlRememberPassword = wxDynamicCast(FindWindow(XRCID("ID_CHECKBOX_VNC_REMEMBER_PWD")), wxCheckBox);

    // Set validators
    if (FindWindow(XRCID("ID_TEXTCTRL_VNC_HOST")))
        FindWindow(XRCID("ID_TEXTCTRL_VNC_HOST"))->SetValidator( wxTextValidator(wxFILTER_NONE, & m_sHostname) );
    if (FindWindow(XRCID("ID_TEXTCTRL_VNC_DPY")))
        FindWindow(XRCID("ID_TEXTCTRL_VNC_DPY"))->SetValidator( wxGenericValidator(& m_iDisplayNumber) );
    if (FindWindow(XRCID("ID_TEXTCTRL_VNC_PASSWD")))
        FindWindow(XRCID("ID_TEXTCTRL_VNC_PASSWD"))->SetValidator( wxGenericValidator(& m_sPassword) );
    if (FindWindow(XRCID("ID_CHECKBOX_VNC_REMEMBER_PWD")))
        FindWindow(XRCID("ID_CHECKBOX_VNC_REMEMBER_PWD"))->SetValidator( wxGenericValidator(& m_bRememberPassword) );
////@end VncPropertyDialog content construction

    // Create custom windows not generated automatically here.

////@begin VncPropertyDialog content initialisation

////@end VncPropertyDialog content initialisation
}

/*!
 * Should we show tooltips?
 */

bool VncPropertyDialog::ShowToolTips()
{
    return TRUE;
}

/*!
 * Get bitmap resources
 */

wxBitmap VncPropertyDialog::GetBitmapResource( const wxString& WXUNUSED(name) )
{
    // Bitmap retrieval
////@begin VncPropertyDialog bitmap retrieval
    return wxNullBitmap;
////@end VncPropertyDialog bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon VncPropertyDialog::GetIconResource( const wxString& name )
{
    // Icon retrieval
    return CreateIconFromFile(name);
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void VncPropertyDialog::OnOkClick( wxCommandEvent& event )
{
    wxASSERT_MSG(m_pCfg, _T("VncPropertyDialog::OnOkClick: No configuration"));
    if (m_pCfg) {
        TransferDataFromWindow();
        m_pCfg->bSetVncRememberPassword(m_bRememberPassword);
        m_pCfg->iSetVncDisplayNumber(m_iDisplayNumber);
        m_pCfg->sSetVncHostName(m_sHostname);
        m_pCfg->sSetVncPassword(m_sPassword);
    }
    event.Skip();
}


