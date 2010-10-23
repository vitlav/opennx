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

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

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
#include "MyXmlConfig.h"
#include "Icon.h"
#include "opennxApp.h"

#include <wx/cshelp.h>

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
    EVT_COMMAND_SCROLL_THUMBRELEASE( XRCID("ID_SLIDER_RDP_COLORS"), RdpImageSettingsDialog::OnSliderRdpColorsScrollThumbRelease )

    EVT_RADIOBUTTON( XRCID("ID_RADIOBUTTON_RDP_IMGRDP"), RdpImageSettingsDialog::OnRadiobuttonRdpImgrdpSelected )

    EVT_RADIOBUTTON( XRCID("ID_RADIOBUTTON_RDP_IMGRGB"), RdpImageSettingsDialog::OnRadiobuttonRdpImgrgbSelected )

    EVT_RADIOBUTTON( XRCID("ID_RADIOBUTTON_RDP_IMGPLAINX"), RdpImageSettingsDialog::OnRadiobuttonRdpImgplainxSelected )

    EVT_BUTTON( wxID_OK, RdpImageSettingsDialog::OnOkClick )

////@end RdpImageSettingsDialog event table entries

    EVT_MENU(wxID_CONTEXT_HELP, RdpImageSettingsDialog::OnContextHelp)

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

void RdpImageSettingsDialog::SetConfig(MyXmlConfig *cfg)
{
    m_pCfg = cfg;
}

/*!
 * RdpImageSettingsDialog creator
 */

bool RdpImageSettingsDialog::Create( wxWindow* parent, wxWindowID WXUNUSED(id), const wxString& WXUNUSED(caption), const wxPoint& WXUNUSED(pos), const wxSize& WXUNUSED(size), long WXUNUSED(style) )
{
////@begin RdpImageSettingsDialog member initialisation
    m_iRdpColors = 0;
    m_pCtrlRdpEncoding = NULL;
    m_pCtrlRdpCompressed = NULL;
    m_pCtrlPlainX = NULL;
////@end RdpImageSettingsDialog member initialisation

    wxASSERT_MSG(m_pCfg, _T("RdpImageSettingsDialog::Create: No configuration"));
    if (m_pCfg) {
        switch (m_pCfg->iGetRdpImageCompression()) {
            case 0:
                m_bRdpEncoding = true;
                m_bRdpCompressed = false;
                m_bRdpRgb = true;
                m_bRdpPlainX = false;
                break;
            case 1:
                m_bRdpEncoding = true;
                m_bRdpCompressed = true;
                m_bRdpRgb = false;
                m_bRdpPlainX = false;
                break;
            case 2:
                m_bRdpEncoding = true;
                m_bRdpCompressed = false;
                m_bRdpRgb = false;
                m_bRdpPlainX = false;
                break;
            case 3:
                m_bRdpEncoding = false;
                m_bRdpCompressed = false;
                m_bRdpRgb = false;
                m_bRdpPlainX = true;
                break;
        }
        m_bRdpCache = m_pCfg->bGetRdpCache();
        m_iRdpColors = m_pCfg->iGetRdpColors();
    }

////@begin RdpImageSettingsDialog creation
    SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY|wxWS_EX_BLOCK_EVENTS|wxDIALOG_EX_CONTEXTHELP);
    SetParent(parent);
    CreateControls();
    SetIcon(GetIconResource(wxT("res/nx.png")));
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end RdpImageSettingsDialog creation

    UpdateDialogConstraints();
    ::wxGetApp().EnableContextHelp(this);
    return TRUE;
}

/*!
 * Control creation for RdpImageSettingsDialog
 */

void RdpImageSettingsDialog::CreateControls()
{    
////@begin RdpImageSettingsDialog content construction
    if (!wxXmlResource::Get()->LoadDialog(this, GetParent(), _T("ID_DIALOG_IMAGE_RDP")))
        wxLogError(wxT("Missing wxXmlResource::Get()->Load() in OnInit()?"));
    m_pCtrlRdpEncoding = XRCCTRL(*this, "ID_RADIOBUTTON_RDP_IMGRDP", wxRadioButton);
    m_pCtrlRdpCompressed = XRCCTRL(*this, "ID_CHECKBOX_RDP_IMGRDPCOMP", wxCheckBox);
    m_pCtrlPlainX = XRCCTRL(*this, "ID_RADIOBUTTON_RDP_IMGPLAINX", wxRadioButton);
    // Set validators
    if (FindWindow(XRCID("ID_SLIDER_RDP_COLORS")))
        FindWindow(XRCID("ID_SLIDER_RDP_COLORS"))->SetValidator( wxGenericValidator(& m_iRdpColors) );
    if (FindWindow(XRCID("ID_RADIOBUTTON_RDP_IMGRDP")))
        FindWindow(XRCID("ID_RADIOBUTTON_RDP_IMGRDP"))->SetValidator( wxGenericValidator(& m_bRdpEncoding) );
    if (FindWindow(XRCID("ID_CHECKBOX_RDP_IMGRDPCOMP")))
        FindWindow(XRCID("ID_CHECKBOX_RDP_IMGRDPCOMP"))->SetValidator( wxGenericValidator(& m_bRdpCompressed) );
    if (FindWindow(XRCID("ID_RADIOBUTTON_RDP_IMGRGB")))
        FindWindow(XRCID("ID_RADIOBUTTON_RDP_IMGRGB"))->SetValidator( wxGenericValidator(& m_bRdpRgb) );
    if (FindWindow(XRCID("ID_RADIOBUTTON_RDP_IMGPLAINX")))
        FindWindow(XRCID("ID_RADIOBUTTON_RDP_IMGPLAINX"))->SetValidator( wxGenericValidator(& m_bRdpPlainX) );
    if (FindWindow(XRCID("ID_CHECKBOX_RDP_IMGCACHE")))
        FindWindow(XRCID("ID_CHECKBOX_RDP_IMGCACHE"))->SetValidator( wxGenericValidator(& m_bRdpCache) );
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

wxBitmap RdpImageSettingsDialog::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
    return CreateBitmapFromFile(name);
}

/*!
 * Get icon resources
 */

wxIcon RdpImageSettingsDialog::GetIconResource( const wxString& name )
{
    // Icon retrieval
    return CreateIconFromFile(name);
}

void RdpImageSettingsDialog::OnContextHelp(wxCommandEvent &)
{
    wxContextHelp contextHelp(this);
}

/*!
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_RDP_PLAINX
 */

void RdpImageSettingsDialog::OnRadiobuttonRdpImgplainxSelected( wxCommandEvent& event )
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

void RdpImageSettingsDialog::OnRadiobuttonRdpImgrdpSelected( wxCommandEvent& event )
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
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_RDP_IMGRGB
 */

void RdpImageSettingsDialog::OnRadiobuttonRdpImgrgbSelected( wxCommandEvent& event )
{
    m_bRdpPlainX = false;
    m_bRdpEncoding = false;
    m_pCtrlPlainX->SetValue(false);
    m_pCtrlRdpEncoding->SetValue(false);
    UpdateDialogConstraints();
    event.Skip();
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void RdpImageSettingsDialog::OnOkClick( wxCommandEvent& event )
{
    wxASSERT_MSG(m_pCfg, _T("RdpImageSettingsDialog::OnOkClick: No configuration"));
    if (m_pCfg) {
        TransferDataFromWindow();
        if (m_bRdpEncoding)
            m_pCfg->iSetRdpImageCompression(m_bRdpCompressed ? 1 : 2);
        else
            m_pCfg->iSetRdpImageCompression(m_bRdpPlainX ? 3 : 0);
        m_pCfg->iSetRdpColors(m_iRdpColors);
        m_pCfg->bSetRdpCache(m_bRdpCache);
    }
    event.Skip();
}


/*!
 * wxEVT_SCROLL_THUMBRELEASE event handler for ID_SLIDER_RDP_COLORS
 */

void RdpImageSettingsDialog::OnSliderRdpColorsScrollThumbRelease( wxScrollEvent& event )
{
    wxSlider *sl = wxDynamicCast(event.GetEventObject(), wxSlider);
    // Stick slider to nearest integer position
    int p = event.GetPosition();
    sl->SetValue((p > 0) ?  p - 1 : p + 1);
    sl->SetValue(p);
    event.Skip();
}


