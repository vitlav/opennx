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

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma implementation "VncImageSettingsDialog.h"
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

#include "VncImageSettingsDialog.h"
#include "MyXmlConfig.h"
#include "Icon.h"

////@begin XPM images
////@end XPM images

/*!
 * VncImageSettingsDialog type definition
 */

IMPLEMENT_CLASS( VncImageSettingsDialog, wxDialog )

/*!
 * VncImageSettingsDialog event table definition
 */

BEGIN_EVENT_TABLE( VncImageSettingsDialog, wxDialog )

////@begin VncImageSettingsDialog event table entries
    EVT_RADIOBUTTON( XRCID("ID_RADIOBUTTON_VNC_HEXTILE"), VncImageSettingsDialog::OnRadiobuttonVncHextileSelected )

    EVT_RADIOBUTTON( XRCID("ID_RADIOBUTTON_VNC_TIGHT"), VncImageSettingsDialog::OnRadiobuttonVncTightSelected )

    EVT_RADIOBUTTON( XRCID("ID_RADIOBUTTON_VNC_PLAINX"), VncImageSettingsDialog::OnRadiobuttonVncPlainxSelected )

    EVT_BUTTON( wxID_OK, VncImageSettingsDialog::OnOkClick )

////@end VncImageSettingsDialog event table entries

END_EVENT_TABLE()

/*!
 * VncImageSettingsDialog constructors
 */

VncImageSettingsDialog::VncImageSettingsDialog( )
    : m_pCfg(NULL)
{
}

VncImageSettingsDialog::VncImageSettingsDialog( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
    : m_pCfg(NULL)
{
    Create(parent, id, caption, pos, size, style);
}

void VncImageSettingsDialog::SetConfig(MyXmlConfig *cfg)
{
    m_pCfg = cfg;
}

/*!
 * VncImageSettingsDialog creator
 */

bool VncImageSettingsDialog::Create( wxWindow* parent, wxWindowID WXUNUSED(id), const wxString& WXUNUSED(caption), const wxPoint& WXUNUSED(pos), const wxSize& WXUNUSED(size), long WXUNUSED(style) )
{
////@begin VncImageSettingsDialog member initialisation
    m_pCtrlUseHextile = NULL;
    m_pCtrlUseTight = NULL;
    m_pCtrlVncJpeg = NULL;
    m_pCtrlUsePlainX = NULL;
////@end VncImageSettingsDialog member initialisation

    wxASSERT_MSG(m_pCfg, _T("VncImageSettingsDialog::Create: No configuration"));
    if (m_pCfg) {
        m_bUseTightJpeg = m_pCfg->bGetUseTightJpeg();
        switch (m_pCfg->iGetVncImageEncoding()) {
            case 0:
                m_bUseHextile = true;
                m_bUseTight = false;
                m_bUsePlainX = false;
                break;
            case 1:
                m_bUseHextile = false;
                m_bUseTight = true;
                m_bUsePlainX = false;
                break;
            case 2:
                m_bUseHextile = false;
                m_bUseTight = false;
                m_bUsePlainX = true;
                break;
        }
    }

////@begin VncImageSettingsDialog creation
    SetExtraStyle(GetExtraStyle()|wxWS_EX_BLOCK_EVENTS);
    SetParent(parent);
    CreateControls();
    SetIcon(GetIconResource(wxT("res/nx.png")));
    Centre();
////@end VncImageSettingsDialog creation
    return TRUE;
}

/*!
 * Control creation for VncImageSettingsDialog
 */

void VncImageSettingsDialog::CreateControls()
{    
////@begin VncImageSettingsDialog content construction
    if (!wxXmlResource::Get()->LoadDialog(this, GetParent(), _T("ID_DIALOG_IMAGE_VNC")))
        wxLogError(wxT("Missing wxXmlResource::Get()->Load() in OnInit()?"));
    m_pCtrlUseHextile = XRCCTRL(*this, "ID_RADIOBUTTON_VNC_HEXTILE", wxRadioButton);
    m_pCtrlUseTight = XRCCTRL(*this, "ID_RADIOBUTTON_VNC_TIGHT", wxRadioButton);
    m_pCtrlVncJpeg = XRCCTRL(*this, "ID_CHECKBOX_VNC_JPEG", wxCheckBox);
    m_pCtrlUsePlainX = XRCCTRL(*this, "ID_RADIOBUTTON_VNC_PLAINX", wxRadioButton);
    // Set validators
    if (FindWindow(XRCID("ID_RADIOBUTTON_VNC_HEXTILE")))
        FindWindow(XRCID("ID_RADIOBUTTON_VNC_HEXTILE"))->SetValidator( wxGenericValidator(& m_bUseHextile) );
    if (FindWindow(XRCID("ID_RADIOBUTTON_VNC_TIGHT")))
        FindWindow(XRCID("ID_RADIOBUTTON_VNC_TIGHT"))->SetValidator( wxGenericValidator(& m_bUseTight) );
    if (FindWindow(XRCID("ID_CHECKBOX_VNC_JPEG")))
        FindWindow(XRCID("ID_CHECKBOX_VNC_JPEG"))->SetValidator( wxGenericValidator(& m_bUseTightJpeg) );
    if (FindWindow(XRCID("ID_RADIOBUTTON_VNC_PLAINX")))
        FindWindow(XRCID("ID_RADIOBUTTON_VNC_PLAINX"))->SetValidator( wxGenericValidator(& m_bUsePlainX) );
////@end VncImageSettingsDialog content construction

    // Create custom windows not generated automatically here.

////@begin VncImageSettingsDialog content initialisation
////@end VncImageSettingsDialog content initialisation
    UpdateDialogConstraints();
}

/*!
 * Should we show tooltips?
 */

bool VncImageSettingsDialog::ShowToolTips()
{
    return TRUE;
}

/*!
 * Get bitmap resources
 */

wxBitmap VncImageSettingsDialog::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin VncImageSettingsDialog bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end VncImageSettingsDialog bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon VncImageSettingsDialog::GetIconResource( const wxString& name )
{
    // Icon retrieval
    return CreateIconFromFile(name);
}

/*!
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_VNC_HEXTILE
 */

void VncImageSettingsDialog::OnRadiobuttonVncHextileSelected( wxCommandEvent& event )
{
    m_bUseHextile = true;
    m_bUseTight = false;
    m_bUsePlainX = false;
    m_pCtrlUseTight->SetValue(false);
    m_pCtrlUsePlainX->SetValue(false);
    UpdateDialogConstraints();
    event.Skip();
}

/*!
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_VNC_PLAINX
 */

void VncImageSettingsDialog::OnRadiobuttonVncPlainxSelected( wxCommandEvent& event )
{
    m_bUseHextile = false;
    m_bUseTight = false;
    m_bUsePlainX = true;
    m_pCtrlUseHextile->SetValue(false);
    m_pCtrlUseTight->SetValue(false);
    UpdateDialogConstraints();
    event.Skip();
}

/*!
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_VNC_TIGHT
 */

void VncImageSettingsDialog::OnRadiobuttonVncTightSelected( wxCommandEvent& event )
{
    m_bUseHextile = false;
    m_bUseTight = true;
    m_bUsePlainX = false;
    m_pCtrlUseHextile->SetValue(false);
    m_pCtrlUsePlainX->SetValue(false);
    UpdateDialogConstraints();
    event.Skip();
}

void VncImageSettingsDialog::UpdateDialogConstraints()
{
    m_pCtrlVncJpeg->Enable(m_bUseTight);
}
/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void VncImageSettingsDialog::OnOkClick( wxCommandEvent& event )
{
    wxASSERT_MSG(m_pCfg, _T("VncImageSettingsDialog::OnOkClick: No configuration"));
    if (m_pCfg) {
        TransferDataFromWindow();
        m_pCfg->iSetVncImageEncoding(m_bUsePlainX ? 2 : (m_bUseTight ? 1 : 0));
        m_pCfg->bSetUseTightJpeg(m_bUseTightJpeg);
    }
    event.Skip();
}


