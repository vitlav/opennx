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

#include <wx/bmpcbox.h>

#include "MyValidator.h"
#include "MyXmlConfig.h"
#include "WinShare.h"
#include "SmbShareProperties.h"
#include "Icon.h"

////@begin XPM images
////@end XPM images

static wxString MYTRACETAG(wxFileName::FileName(wxT(__FILE__)).GetName());

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
    , m_bUseSmb(false)
    , m_bUseCups(false)
{
}

SmbShareProperties::SmbShareProperties( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
    : m_iCurrentShare(-1)
    , m_bUseSmb(false)
    , m_bUseCups(false)
{
    Create(parent, id, caption, pos, size, style);
}

void SmbShareProperties::SetConfig(MyXmlConfig *cfg)
{
    m_pCfg = cfg;
}

void SmbShareProperties::SetCurrentShare(int cs)
{
    m_iCurrentShare = cs;
}

void SmbShareProperties::SetUse(bool useSmb, bool useCups)
{
    m_bUseSmb = useSmb;
    m_bUseCups = useCups;
}

/*!
 * SmbShareProperties creator
 */

bool SmbShareProperties::Create( wxWindow* parent, wxWindowID WXUNUSED(id), const wxString& WXUNUSED(caption), const wxPoint& WXUNUSED(pos), const wxSize& WXUNUSED(size), long WXUNUSED(style) )
{
////@begin SmbShareProperties member initialisation
    m_pCtrlLocalShares = NULL;
    m_pCtrlCupsOptions = NULL;
    m_pCtrlSmbOptions = NULL;
    m_pCtrlMountPoint = NULL;
    m_pCtrlUsername = NULL;
    m_pCtrlPassword = NULL;
////@end SmbShareProperties member initialisation

////@begin SmbShareProperties creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    SetParent(parent);
    CreateControls();
    SetIcon(GetIconResource(wxT("res/nx.png")));
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end SmbShareProperties creation
    return TRUE;
}

static int
cmpShare(SharedResource *a, SharedResource *b)
{
    int ret = a->sharetype - b->sharetype;
    return (ret != 0) ? ret : a->name.Cmp(b->name);
}

/*!
 * Control creation for SmbShareProperties
 */

void SmbShareProperties::CreateControls()
{    
////@begin SmbShareProperties content construction
    if (!wxXmlResource::Get()->LoadDialog(this, GetParent(), _T("ID_DIALOG_SHARE_ADD")))
        wxLogError(wxT("Missing wxXmlResource::Get()->Load() in OnInit()?"));
    m_pCtrlLocalShares = XRCCTRL(*this, "ID_COMBOBOX_SHARE_LOCALNAME", wxBitmapComboBox);
    m_pCtrlCupsOptions = XRCCTRL(*this, "ID_PANEL_CUPSOTIONS", wxPanel);
    m_pCtrlSmbOptions = XRCCTRL(*this, "ID_PANEL_SMBOPTIONS", wxPanel);
    m_pCtrlMountPoint = XRCCTRL(*this, "ID_TEXTCTRL_SHARE_MOUNTPOINT", wxTextCtrl);
    m_pCtrlUsername = XRCCTRL(*this, "ID_TEXTCTRL_SHARE_USERNAME", wxTextCtrl);
    m_pCtrlPassword = XRCCTRL(*this, "ID_TEXTCTRL_SHARE_PASSWORD", wxTextCtrl);
    // Set validators
    if (FindWindow(XRCID("ID_TEXTCTRL_SHARE_MOUNTPOINT")))
        FindWindow(XRCID("ID_TEXTCTRL_SHARE_MOUNTPOINT"))->SetValidator( MyValidator(& m_sMountPoint) );
    if (FindWindow(XRCID("ID_TEXTCTRL_SHARE_USERNAME")))
        FindWindow(XRCID("ID_TEXTCTRL_SHARE_USERNAME"))->SetValidator( MyValidator(& m_sUsername) );
    if (FindWindow(XRCID("ID_TEXTCTRL_SHARE_PASSWORD")))
        FindWindow(XRCID("ID_TEXTCTRL_SHARE_PASSWORD"))->SetValidator( MyValidator(& m_sPassword) );
////@end SmbShareProperties content construction

    // Create custom windows not generated automatically here.

////@begin SmbShareProperties content initialisation
////@end SmbShareProperties content initialisation

    Layout();
    m_pCtrlCupsOptions->SetMinSize(m_pCtrlSmbOptions->GetBestSize());
    m_pCtrlSmbOptions->Show(false);
    m_pCtrlCupsOptions->Show(true);
    Layout();
    m_pCtrlCupsOptions->Show(false);
    m_pCtrlSmbOptions->Show(true);
    Layout();
    
    if (m_iCurrentShare != -1) {
        ShareGroup sg = m_pCfg->aGetShareGroups().Item(m_iCurrentShare);
        wxBitmap bm = wxNullBitmap;
        m_pCtrlLocalShares->Append(sg.m_sName, bm, &sg);
        m_pCtrlLocalShares->SetSelection(0);
        m_pCtrlLocalShares->Enable(false);
        m_sMountPoint = sg.m_sMountPoint;
        m_sUsername = sg.m_sUsername;
        m_sPassword = sg.m_sPassword;
        SetTitle(_("Modify shared resource - OpenNX"));
    } else {
        if (m_bUseSmb) {
            SmbClient sc;
            m_aShares = sc.GetShares();
        }
        if (m_bUseCups) {
            CupsClient cc;
            ArrayOfShares acs = cc.GetShares();
            WX_APPEND_ARRAY(m_aShares, acs);
        }

        for (size_t i = 0; i < m_aShares.GetCount(); i++) {
            wxBitmap bm;
            switch (m_aShares[i].sharetype) {
                case SharedResource::SHARE_SMB_DISK:
                    bm = GetBitmapResource(wxT("res/smbfolder.png"));
                    break;
                case SharedResource::SHARE_SMB_PRINTER:
                    bm = GetBitmapResource(wxT("res/smbprinter.png"));
                    break;
                case SharedResource::SHARE_CUPS_PRINTER:
                    bm = GetBitmapResource(wxT("res/cupsprinter.png"));
                    break;
                default:
                    bm = wxNullBitmap;
                    break;
            }
            m_pCtrlLocalShares->Append(m_aShares[i].name, bm, &(m_aShares[i]));
        }

        if (m_aShares.GetCount() > 0) {
            m_pCtrlLocalShares->SetSelection(0);
            switch (m_aShares[0].sharetype) {
                case SharedResource::SHARE_SMB_DISK:
                case SharedResource::SHARE_SMB_PRINTER:
                    m_sMountPoint = _T("$(SHARES)/") + m_aShares[0].name;
                    break;
                case SharedResource::SHARE_CUPS_PRINTER:
                    m_pCtrlSmbOptions->Show(false);
                    m_pCtrlCupsOptions->Show(true);
                    break;
            }
        } else {
            ::wxLogMessage(_("No shares found"));
            m_pCtrlLocalShares->Enable(false);
            m_pCtrlMountPoint->Enable(false);
            m_pCtrlUsername->Enable(false);
            m_pCtrlPassword->Enable(false);
        }
    }
}

/*!
 * wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_COMBOBOX_SHARE_LOCALNAME
 */

void SmbShareProperties::OnComboboxShareLocalnameSelected( wxCommandEvent& event )
{
    SharedResource *res = wxDynamicCast(m_pCtrlLocalShares->GetClientData(event.GetInt()), SharedResource);
    switch (res->sharetype) {
        case SharedResource::SHARE_SMB_DISK:
        case SharedResource::SHARE_SMB_PRINTER:
            m_pCtrlCupsOptions->Show(false);
            m_pCtrlSmbOptions->Show(true);
            m_sMountPoint = wxT("$(SHARES)/") + res->name;
            m_pCtrlMountPoint->SetValue(m_sMountPoint);
            break;
        case SharedResource::SHARE_CUPS_PRINTER:
            m_pCtrlSmbOptions->Show(false);
            m_pCtrlCupsOptions->GetSizer()->Layout();
            m_pCtrlCupsOptions->Show(true);
            break;
    }
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

wxBitmap SmbShareProperties::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
    return CreateBitmapFromFile(name);
}

/*!
 * Get icon resources
 */

wxIcon SmbShareProperties::GetIconResource( const wxString& name )
{
    // Icon retrieval
    return CreateIconFromFile(name);
}
