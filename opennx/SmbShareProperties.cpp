/////////////////////////////////////////////////////////////////////////////
// Name:        SmbShareProperties.cpp
// Purpose:     
// Author:      Fritz Elfert
// Modified by: 
// Created:     08/01/04 16:35:50
// RCS-ID:      $Id: SmbShareProperties.cpp 811 2004-08-05 20:54:14Z felfert $
// Copyright:   Copyright 2004 Millenux GmbH
// Licence:     LGPL
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma implementation "SmbShareProperties.h"
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

#include "MxValidator.h"
#include "MxXmlConfig.h"
#include "WinShare.h"
#include "SmbShareProperties.h"
#include "Icon.h"

////@begin XPM images
////@end XPM images

/*!
 * SmbShareProperties type definition
 */

IMPLEMENT_DYNAMIC_CLASS( SmbShareProperties, wxDialog )

/*!
 * SmbShareProperties event table definition
 */

BEGIN_EVENT_TABLE( SmbShareProperties, wxDialog )

////@begin SmbShareProperties event table entries
    EVT_COMBOBOX( XRCID("ID_COMBOBOX_SHARE_LOCALNAME"), SmbShareProperties::OnComboboxShareLocalnameSelected )

    EVT_TEXT( XRCID("ID_TEXTCTRL_SHARE_MOUNTPOINT"), SmbShareProperties::OnTextctrlShareMountpointUpdated )

    EVT_TEXT( XRCID("ID_TEXTCTRL_SHARE_USERNAME"), SmbShareProperties::OnTextctrlShareUsernameUpdated )

    EVT_TEXT( XRCID("ID_TEXTCTRL_SHARE_PASSWORD"), SmbShareProperties::OnTextctrlSharePasswordUpdated )

    EVT_BUTTON( wxID_OK, SmbShareProperties::OnOkClick )

////@end SmbShareProperties event table entries

END_EVENT_TABLE()

/*!
 * SmbShareProperties constructors
 */

SmbShareProperties::SmbShareProperties( )
    : m_iCurrentShare(-1)
{
}

SmbShareProperties::SmbShareProperties( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
    : m_iCurrentShare(-1)
{
    Create(parent, id, caption, pos, size, style);
}

void SmbShareProperties::SetConfig(MxXmlConfig *cfg)
{
    m_pCfg = cfg;
}

void SmbShareProperties::SetCurrentShare(int cs)
{
    m_iCurrentShare = cs;
}

/*!
 * SmbShareProperties creator
 */

bool SmbShareProperties::Create( wxWindow* parent, wxWindowID WXUNUSED(id), const wxString& WXUNUSED(caption), const wxPoint& WXUNUSED(pos), const wxSize& WXUNUSED(size), long WXUNUSED(style) )
{
////@begin SmbShareProperties member initialisation
    m_pCtrlLocalShares = NULL;
    m_pCtrlMountPoint = NULL;
    m_pCtrlUsername = NULL;
    m_pCtrlPassword = NULL;
////@end SmbShareProperties member initialisation

////@begin SmbShareProperties creation
    SetExtraStyle(GetExtraStyle()|wxWS_EX_BLOCK_EVENTS);
    SetParent(parent);
    CreateControls();
    SetIcon(this->GetIconResource(wxT("res/nx.png")));
    GetSizer()->Fit(this);
    GetSizer()->SetSizeHints(this);
    Centre();
////@end SmbShareProperties creation
    return TRUE;
}

/*!
 * Control creation for SmbShareProperties
 */

void SmbShareProperties::CreateControls()
{    
////@begin SmbShareProperties content construction

    wxXmlResource::Get()->LoadDialog(this, GetParent(), _T("ID_DIALOG_SHARE_ADD"));
    if (FindWindow(XRCID("ID_COMBOBOX_SHARE_LOCALNAME")))
        m_pCtrlLocalShares = wxDynamicCast(FindWindow(XRCID("ID_COMBOBOX_SHARE_LOCALNAME")), wxComboBox);
    if (FindWindow(XRCID("ID_TEXTCTRL_SHARE_MOUNTPOINT")))
        m_pCtrlMountPoint = wxDynamicCast(FindWindow(XRCID("ID_TEXTCTRL_SHARE_MOUNTPOINT")), wxTextCtrl);
    if (FindWindow(XRCID("ID_TEXTCTRL_SHARE_USERNAME")))
        m_pCtrlUsername = wxDynamicCast(FindWindow(XRCID("ID_TEXTCTRL_SHARE_USERNAME")), wxTextCtrl);
    if (FindWindow(XRCID("ID_TEXTCTRL_SHARE_PASSWORD")))
        m_pCtrlPassword = wxDynamicCast(FindWindow(XRCID("ID_TEXTCTRL_SHARE_PASSWORD")), wxTextCtrl);

    // Set validators
    if (FindWindow(XRCID("ID_TEXTCTRL_SHARE_MOUNTPOINT")))
        FindWindow(XRCID("ID_TEXTCTRL_SHARE_MOUNTPOINT"))->SetValidator( MxValidator(& m_sMountPoint) );
    if (FindWindow(XRCID("ID_TEXTCTRL_SHARE_USERNAME")))
        FindWindow(XRCID("ID_TEXTCTRL_SHARE_USERNAME"))->SetValidator( MxValidator(& m_sUsername) );
    if (FindWindow(XRCID("ID_TEXTCTRL_SHARE_PASSWORD")))
        FindWindow(XRCID("ID_TEXTCTRL_SHARE_PASSWORD"))->SetValidator( MxValidator(& m_sPassword) );
////@end SmbShareProperties content construction

    // Create custom windows not generated automatically here.

////@begin SmbShareProperties content initialisation

////@end SmbShareProperties content initialisation

    if (m_iCurrentShare != -1) {
        ShareGroup sg = m_pCfg->aGetShareGroups().Item(m_iCurrentShare);
        m_pCtrlLocalShares->Append(sg.m_sName, &sg);
        m_pCtrlLocalShares->SetSelection(0);
        m_pCtrlLocalShares->Enable(false);
        m_sMountPoint = sg.m_sMountPoint;
        m_sUsername = sg.m_sUsername;
        m_sPassword = sg.m_sPassword;
        SetTitle(_("Modify SMB Share - MXclient"));
    } else {
        WinShare s;
        
        m_aShares = s.GetShares();
        for (size_t i = 0; i < m_aShares.GetCount(); i++)
            m_pCtrlLocalShares->Append(m_aShares[i].name, &(m_aShares[i]));
        if (m_aShares.GetCount() > 0) {
            m_pCtrlLocalShares->SetSelection(0);
            m_sMountPoint = _T("$(SHARES)/");
            m_sMountPoint += wxDynamicCast(m_pCtrlLocalShares->GetClientData(0), SharedResource)->name;
        }
    }
}

/*!
 * wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_COMBOBOX_SHARE_LOCALNAME
 */

void SmbShareProperties::OnComboboxShareLocalnameSelected( wxCommandEvent& event )
{
    m_sMountPoint = _T("$(SHARES)/");
    m_sMountPoint += wxDynamicCast(m_pCtrlLocalShares->GetClientData(event.GetInt()), SharedResource)->name;
    m_pCtrlMountPoint->SetValue(m_sMountPoint);
    event.Skip();
}

/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXTCTRL_SHARE_MOUNTPOINT
 */

void SmbShareProperties::OnTextctrlShareMountpointUpdated( wxCommandEvent& event )
{
    // Insert custom code here
    event.Skip();
}

/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXTCTRL_SHARE_USERNAME
 */

void SmbShareProperties::OnTextctrlShareUsernameUpdated( wxCommandEvent& event )
{
    // Insert custom code here
    event.Skip();
}

/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXTCTRL_SHARE_PASSWORD
 */

void SmbShareProperties::OnTextctrlSharePasswordUpdated( wxCommandEvent& event )
{
    // Insert custom code here
    event.Skip();
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void SmbShareProperties::OnOkClick( wxCommandEvent& event )
{
    ArrayOfShareGroups sg = m_pCfg->aGetShareGroups();
    TransferDataFromWindow();
    for (size_t i = 0; i < sg.GetCount(); i++) {
        if ((sg[i].m_sMountPoint == m_sMountPoint) && ((int)i != m_iCurrentShare)) {
            wxMessageBox(_("Mountpoint is already used. Please chose another mountpoint"),
                _("Invalid mountpoint"), wxICON_ERROR|wxOK);
            if (m_iCurrentShare != -1)
                m_pCtrlMountPoint->SetValue(sg[m_iCurrentShare].m_sMountPoint);
            return;
        }
    }
    wxString groupName = m_sMountPoint + _T("_Shared");
    if (m_iCurrentShare != -1) {
        if (groupName != sg[m_iCurrentShare].m_sGroupName) {
            wxArrayString as = m_pCfg->aGetUsedShareGroups();
            int aidx = as.Index(sg[m_iCurrentShare].m_sGroupName);
            as.RemoveAt(aidx);
            as.Insert(groupName, aidx);
            m_pCfg->aSetUsedShareGroups(as);
        }
        sg[m_iCurrentShare].m_sGroupName = groupName;
        sg[m_iCurrentShare].m_sMountPoint = m_sMountPoint;
        sg[m_iCurrentShare].m_sUsername = m_sUsername;
        sg[m_iCurrentShare].m_sPassword = m_sPassword;
        m_pCfg->aSetShareGroups(sg);
    } else {
        ShareGroup g;
        int shidx = m_pCtrlLocalShares->GetSelection();

        g.m_bMountNow = true;
        g.m_iType = wxDynamicCast(m_pCtrlLocalShares->GetClientData(shidx), SharedResource)->sharetype;
        g.m_sGroupName = groupName;
        g.m_sMountPoint = m_sMountPoint;
        g.m_sName = m_pCtrlLocalShares->GetValue();
        g.m_sUsername = m_sUsername;
        g.m_sPassword = m_sPassword;

        sg.Add(g);
        m_pCfg->aSetShareGroups(sg);
        wxArrayString as = m_pCfg->aGetUsedShareGroups();
        as.Add(g.m_sGroupName);
        m_pCfg->aSetUsedShareGroups(as);
        m_pCfg->iSetUsedShareGroups(m_pCfg->iGetUsedShareGroups() + 1);
    }
    event.Skip();
}

/*!
 * Should we show tooltips?
 */

bool SmbShareProperties::ShowToolTips()
{
    return TRUE;
}

/*!
 * Get bitmap resources
 */

wxBitmap SmbShareProperties::GetBitmapResource( const wxString& WXUNUSED(name) )
{
    // Bitmap retrieval
////@begin SmbShareProperties bitmap retrieval
    return wxNullBitmap;
////@end SmbShareProperties bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon SmbShareProperties::GetIconResource( const wxString& name )
{
    // Icon retrieval
    return CreateIconFromFile(name);
}
