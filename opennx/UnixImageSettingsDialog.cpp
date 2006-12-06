/////////////////////////////////////////////////////////////////////////////
// Name:        UnixImageSettingsDialog.cpp
// Purpose:     
// Author:      Fritz Elfert
// Modified by: 
// Created:     06/25/04 00:38:55
// RCS-ID:      $Id$
// Copyright:   Copyright 2004 Millenux GmbH
// Licence:     LGPL
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma implementation "UnixImageSettingsDialog.h"
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

#include "UnixImageSettingsDialog.h"
#include "MxXmlConfig.h"
#include "Icon.h"

////@begin XPM images
////@end XPM images

/*!
 * UnixImageSettingsDialog type definition
 */

IMPLEMENT_CLASS( UnixImageSettingsDialog, wxDialog )

/*!
 * UnixImageSettingsDialog event table definition
 */

BEGIN_EVENT_TABLE( UnixImageSettingsDialog, wxDialog )

////@begin UnixImageSettingsDialog event table entries
    EVT_BUTTON( wxID_OK, UnixImageSettingsDialog::OnOkClick )

////@end UnixImageSettingsDialog event table entries

END_EVENT_TABLE()

/*!
 * UnixImageSettingsDialog constructors
 */

UnixImageSettingsDialog::UnixImageSettingsDialog( )
    : m_pCfg(NULL)
{
}

UnixImageSettingsDialog::UnixImageSettingsDialog( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
    : m_pCfg(NULL)
{
    Create(parent, id, caption, pos, size, style);
}

void UnixImageSettingsDialog::SetConfig(MxXmlConfig *cfg)
{
    m_pCfg = cfg;
}

/*!
 * UnixImageSettingsDialog creator
 */

bool UnixImageSettingsDialog::Create( wxWindow* parent, wxWindowID WXUNUSED(id), const wxString& WXUNUSED(caption), const wxPoint& WXUNUSED(pos), const wxSize& WXUNUSED(size), long WXUNUSED(style) )
{
////@begin UnixImageSettingsDialog member initialisation
    m_pCtrlUseJpegQuality = NULL;
    m_pCtrlJpegQuality = NULL;
////@end UnixImageSettingsDialog member initialisation

    wxASSERT_MSG(m_pCfg, _T("UnixImageSettingsDialog::Create: No configuration"));
    if (m_pCfg) {
        m_bUsePNG = m_pCfg->bGetUsePNG();
        m_bUsePlainX = m_pCfg->bGetUsePlainX();
        m_bUseJpeg = m_pCfg->bGetUseJpeg();
        m_bUseJpegQuality = m_pCfg->bGetUseJpegQuality();
        m_bDisableRender = m_pCfg->bGetDisableRender();
        m_iJpegQuality = m_pCfg->iGetJpegQuality();
    }

////@begin UnixImageSettingsDialog creation
    SetExtraStyle(GetExtraStyle()|wxWS_EX_BLOCK_EVENTS);
    SetParent(parent);
    CreateControls();
    SetIcon(GetIconResource(wxT("res/nx.png")));
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end UnixImageSettingsDialog creation
    return TRUE;
}

/*!
 * Control creation for UnixImageSettingsDialog
 */

void UnixImageSettingsDialog::CreateControls()
{    
////@begin UnixImageSettingsDialog content construction
    if (!wxXmlResource::Get()->LoadDialog(this, GetParent(), _T("ID_DIALOG_IMAGE_X11")))
        wxLogError(wxT("Missing wxXmlResource::Get()->Load() in OnInit()?"));
    m_pCtrlUseJpegQuality = XRCCTRL(*this, "ID_CHECKBOX_X11_JPEG_CUSTOMQUALITY", wxCheckBox);
    m_pCtrlJpegQuality = XRCCTRL(*this, "ID_SLIDER_X11_JPEG_QALITY", wxSlider);
    // Set validators
    if (FindWindow(XRCID("ID_RADIOBUTTON_X11_PNG")))
        FindWindow(XRCID("ID_RADIOBUTTON_X11_PNG"))->SetValidator( wxGenericValidator(& m_bImageEncodingPNG) );
    if (FindWindow(XRCID("ID_RADIOBUTTON_X11_PLAIN")))
        FindWindow(XRCID("ID_RADIOBUTTON_X11_PLAIN"))->SetValidator( wxGenericValidator(& m_bImageEncodingPlainX) );
    if (FindWindow(XRCID("ID_RADIOBUTTON_X11_JPEG")))
        FindWindow(XRCID("ID_RADIOBUTTON_X11_JPEG"))->SetValidator( wxGenericValidator(& m_bImageEncodingJpeg) );
    if (FindWindow(XRCID("ID_CHECKBOX_X11_JPEG_CUSTOMQUALITY")))
        FindWindow(XRCID("ID_CHECKBOX_X11_JPEG_CUSTOMQUALITY"))->SetValidator( wxGenericValidator(& m_bUseJpegQuality) );
    if (FindWindow(XRCID("ID_SLIDER_X11_JPEG_QALITY")))
        FindWindow(XRCID("ID_SLIDER_X11_JPEG_QALITY"))->SetValidator( wxGenericValidator(& m_iJpegQuality) );
    if (FindWindow(XRCID("ID_CHECKBOX_PERF_DISABLERENDER")))
        FindWindow(XRCID("ID_CHECKBOX_PERF_DISABLERENDER"))->SetValidator( wxGenericValidator(& m_bDisableRender) );
    if (FindWindow(XRCID("ID_CHECKBOX_PERF_DISABLEBACKINGSTORE")))
        FindWindow(XRCID("ID_CHECKBOX_PERF_DISABLEBACKINGSTORE"))->SetValidator( wxGenericValidator(& m_bDisableBackingstore) );
    if (FindWindow(XRCID("ID_CHECKBOX_PERF_DISABLECOMPOSITE")))
        FindWindow(XRCID("ID_CHECKBOX_PERF_DISABLECOMPOSITE"))->SetValidator( wxGenericValidator(& m_bDisableComposite) );
    if (FindWindow(XRCID("ID_CHECKBOX_PERF_DISABLESHMEM")))
        FindWindow(XRCID("ID_CHECKBOX_PERF_DISABLESHMEM"))->SetValidator( wxGenericValidator(& m_bDisableShmem) );
    if (FindWindow(XRCID("ID_CHECKBOX_PERF_DISABLESHPIX")))
        FindWindow(XRCID("ID_CHECKBOX_PERF_DISABLESHPIX"))->SetValidator( wxGenericValidator(& m_bDisableShpix) );
////@end UnixImageSettingsDialog content construction

    // Create custom windows not generated automatically here.

////@begin UnixImageSettingsDialog content initialisation
////@end UnixImageSettingsDialog content initialisation
}

/*!
 * Should we show tooltips?
 */

bool UnixImageSettingsDialog::ShowToolTips()
{
    return TRUE;
}

/*!
 * Get bitmap resources
 */

wxBitmap UnixImageSettingsDialog::GetBitmapResource( const wxString& WXUNUSED(name) )
{
    // Bitmap retrieval
////@begin UnixImageSettingsDialog bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end UnixImageSettingsDialog bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon UnixImageSettingsDialog::GetIconResource( const wxString& name )
{
    // Icon retrieval
    return CreateIconFromFile(name);
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void UnixImageSettingsDialog::OnOkClick( wxCommandEvent& event )
{
    wxASSERT_MSG(m_pCfg, _T("UnixImageSettingsDialog::OnOkClick: No configuration"));
    if (m_pCfg) {
        TransferDataFromWindow();
        m_pCfg->bSetUsePNG(m_bUsePNG);
        m_pCfg->bSetUsePlainX(m_bUsePlainX);
        m_pCfg->bSetUseJpeg(m_bUseJpeg);
        m_pCfg->bSetUseJpegQuality(m_bUseJpegQuality);
        m_pCfg->bSetDisableRender(m_bDisableRender);
        m_pCfg->iSetJpegQuality(m_iJpegQuality);
    }
    event.Skip();
}



