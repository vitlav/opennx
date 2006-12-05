/////////////////////////////////////////////////////////////////////////////
// Name:        RdpImageSettingsDialog.cpp
// Purpose:     
// Author:      Fritz Elfert
// Modified by: 
// Created:     06/13/04 22:48:38
// RCS-ID:      $Id: RdpImageSettingsDialog.cpp 811 2004-08-05 20:54:14Z felfert $
// Copyright:   Copyright 2004 Millenux GmbH
// Licence:     LGPL
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma implementation "RdpImageSettingsDialog.h"
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

#include "RdpImageSettingsDialog.h"
#include "MxXmlConfig.h"
#include "Icon.h"

////@begin XPM images
////@end XPM images

/*!
 * RdpImageSettingsDialog type definition
 */

IMPLEMENT_CLASS( RdpImageSettingsDialog, wxDialog )

/*!
 * RdpImageSettingsDialog event table definition
 */

BEGIN_EVENT_TABLE( RdpImageSettingsDialog, wxDialog )

////@begin RdpImageSettingsDialog event table entries
    EVT_RADIOBUTTON( XRCID("ID_RADIOBUTTON_RDP_ENCODING"), RdpImageSettingsDialog::OnRadiobuttonRdpEncodingSelected )

    EVT_RADIOBUTTON( XRCID("ID_RADIOBUTTON_RDP_PLAINX"), RdpImageSettingsDialog::OnRadiobuttonRdpPlainxSelected )

    EVT_BUTTON( wxID_OK, RdpImageSettingsDialog::OnOkClick )

////@end RdpImageSettingsDialog event table entries

END_EVENT_TABLE()

/*!
 * RdpImageSettingsDialog constructors
 */

RdpImageSettingsDialog::RdpImageSettingsDialog( )
    : m_pCfg(NULL)
{
}

RdpImageSettingsDialog::RdpImageSettingsDialog( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
    : m_pCfg(NULL)
{
    Create(parent, id, caption, pos, size, style);
}

void RdpImageSettingsDialog::SetConfig(MxXmlConfig *cfg)
{
    m_pCfg = cfg;
}

/*!
 * RdpImageSettingsDialog creator
 */

bool RdpImageSettingsDialog::Create( wxWindow* parent, wxWindowID WXUNUSED(id), const wxString& WXUNUSED(caption), const wxPoint& WXUNUSED(pos), const wxSize& WXUNUSED(size), long WXUNUSED(style) )
{
////@begin RdpImageSettingsDialog member initialisation
    m_pCtrlRdpEncoding = NULL;
    m_pCtrlRdpCompressed = NULL;
    m_pCtrlPlainX = NULL;
////@end RdpImageSettingsDialog member initialisation

    wxASSERT_MSG(m_pCfg, _T("RdpImageSettingsDialog::Create: No configuration"));
    if (m_pCfg) {
        switch (m_pCfg->iGetRdpImageCompression()) {
            case 0:
                m_bRdpPlainX = true;
                m_bRdpEncoding = false;
                break;
            case 1:
                m_bRdpPlainX = false;
                m_bRdpEncoding = true;
                m_bRdpCompressed = true;
                break;
            case 2:
                m_bRdpPlainX = false;
                m_bRdpEncoding = true;
                m_bRdpCompressed = false;
                break;
        }
    }

////@begin RdpImageSettingsDialog creation
    SetExtraStyle(GetExtraStyle()|wxWS_EX_VALIDATE_RECURSIVELY|wxWS_EX_BLOCK_EVENTS);
    SetParent(parent);
    CreateControls();
    SetIcon(this->GetIconResource(wxT("res/nx.png")));
    Centre();
////@end RdpImageSettingsDialog creation

    UpdateDialogConstraints();
    return TRUE;
}

/*!
 * Control creation for RdpImageSettingsDialog
 */

void RdpImageSettingsDialog::CreateControls()
{    
////@begin RdpImageSettingsDialog content construction

    wxXmlResource::Get()->LoadDialog(this, GetParent(), _T("ID_DIALOG_IMAGE_RDP"));
    if (FindWindow(XRCID("ID_RADIOBUTTON_RDP_ENCODING")))
        m_pCtrlRdpEncoding = wxDynamicCast(FindWindow(XRCID("ID_RADIOBUTTON_RDP_ENCODING")), wxRadioButton);
    if (FindWindow(XRCID("ID_CHECKBOX_RDP_COMPRESS")))
        m_pCtrlRdpCompressed = wxDynamicCast(FindWindow(XRCID("ID_CHECKBOX_RDP_COMPRESS")), wxCheckBox);
    if (FindWindow(XRCID("ID_RADIOBUTTON_RDP_PLAINX")))
        m_pCtrlPlainX = wxDynamicCast(FindWindow(XRCID("ID_RADIOBUTTON_RDP_PLAINX")), wxRadioButton);

    // Set validators
    if (FindWindow(XRCID("ID_RADIOBUTTON_RDP_ENCODING")))
        FindWindow(XRCID("ID_RADIOBUTTON_RDP_ENCODING"))->SetValidator( wxGenericValidator(& m_bRdpEncoding) );
    if (FindWindow(XRCID("ID_CHECKBOX_RDP_COMPRESS")))
        FindWindow(XRCID("ID_CHECKBOX_RDP_COMPRESS"))->SetValidator( wxGenericValidator(& m_bRdpCompressed) );
    if (FindWindow(XRCID("ID_RADIOBUTTON_RDP_PLAINX")))
        FindWindow(XRCID("ID_RADIOBUTTON_RDP_PLAINX"))->SetValidator( wxGenericValidator(& m_bRdpPlainX) );
////@end RdpImageSettingsDialog content construction

    // Create custom windows not generated automatically here.

////@begin RdpImageSettingsDialog content initialisation

////@end RdpImageSettingsDialog content initialisation
}

/*!
 * Should we show tooltips?
 */

bool RdpImageSettingsDialog::ShowToolTips()
{
    return TRUE;
}

/*!
 * Get bitmap resources
 */

wxBitmap RdpImageSettingsDialog::GetBitmapResource( const wxString& WXUNUSED(name) )
{
    // Bitmap retrieval
////@begin RdpImageSettingsDialog bitmap retrieval
    return wxNullBitmap;
////@end RdpImageSettingsDialog bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon RdpImageSettingsDialog::GetIconResource( const wxString& name )
{
    // Icon retrieval
    return CreateIconFromFile(name);
}

/*!
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_RDP_PLAINX
 */

void RdpImageSettingsDialog::OnRadiobuttonRdpPlainxSelected( wxCommandEvent& event )
{
    m_bRdpPlainX = true;
    m_bRdpEncoding = false;
    m_pCtrlRdpEncoding->SetValue(false);
    UpdateDialogConstraints();
    event.Skip();
}

/*!
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_RDP_ENCODING
 */

void RdpImageSettingsDialog::OnRadiobuttonRdpEncodingSelected( wxCommandEvent& event )
{
    m_bRdpPlainX = false;
    m_bRdpEncoding = true;
    m_pCtrlPlainX->SetValue(false);
    UpdateDialogConstraints();
    event.Skip();
}

void RdpImageSettingsDialog::UpdateDialogConstraints()
{
    m_pCtrlRdpCompressed->Enable(m_bRdpEncoding);
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void RdpImageSettingsDialog::OnOkClick( wxCommandEvent& event )
{
    wxASSERT_MSG(m_pCfg, _T("RdpImageSettingsDialog::OnOkClick: No configuration"));
    if (m_pCfg) {
        TransferDataFromWindow();
        m_pCfg->iSetRdpImageCompression(m_bRdpPlainX ? 0 : (m_bRdpCompressed ? 1 : 2));
    }
    event.Skip();
}


