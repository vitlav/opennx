// $Id$
//
// Copyright (C) 2006 The OpenNX team
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

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "KeyDialog.h"
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

////@begin includes
////@end includes

#include "KeyDialog.h"

////@begin XPM images
////@end XPM images

/*!
 * KeyDialog type definition
 */

IMPLEMENT_DYNAMIC_CLASS( KeyDialog, wxDialog )

/*!
 * KeyDialog event table definition
 */

BEGIN_EVENT_TABLE( KeyDialog, wxDialog )

////@begin KeyDialog event table entries
    EVT_TEXT( XRCID("ID_TEXTCTRL_SSHKEY"), KeyDialog::OnTextctrlSshkeyUpdated )

    EVT_BUTTON( XRCID("ID_BUTTON_IMPORT"), KeyDialog::OnButtonImportClick )

    EVT_BUTTON( wxID_DEFAULT, KeyDialog::OnDEFAULTClick )

    EVT_BUTTON( wxID_SAVE, KeyDialog::OnSAVEClick )

////@end KeyDialog event table entries

END_EVENT_TABLE()

/*!
 * KeyDialog constructors
 */

KeyDialog::KeyDialog( )
{
    Init();
}

KeyDialog::KeyDialog( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}

/*!
 * KeyDialog creator
 */

bool KeyDialog::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin KeyDialog creation
    SetExtraStyle(GetExtraStyle()|wxWS_EX_BLOCK_EVENTS);
    SetParent(parent);
    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end KeyDialog creation
    return true;
}

/*!
 * Member initialisation 
 */

void KeyDialog::Init()
{
////@begin KeyDialog member initialisation
    m_sSshKey = wxT("");
    m_bCharTyped = false;
    m_pCtrlSshKey = NULL;
    m_pCtrlSave = NULL;
////@end KeyDialog member initialisation
}
/*!
 * Control creation for KeyDialog
 */

void KeyDialog::CreateControls()
{   
#if 0 
////@begin KeyDialog content construction
    if (!wxXmlResource::Get()->LoadDialog(this, GetParent(), _T("ID_KEYDIALOG")))
        wxLogError(wxT("Missing wxXmlResource::Get()->Load() in OnInit()?"));
    m_pCtrlSshKey = XRCCTRL(*this, "ID_TEXTCTRL_SSHKEY", wxTextCtrl);
    m_pCtrlSave = XRCCTRL(*this, "wxID_SAVE", wxButton);
    // Set validators
    if (FindWindow(XRCID("ID_TEXTCTRL_SSHKEY")))
        FindWindow(XRCID("ID_TEXTCTRL_SSHKEY"))->SetValidator( wxGenericValidator(& m_sSshKey) );
    // Connect events and objects
    m_pCtrlSshKey->Connect(ID_TEXTCTRL_SSHKEY, wxEVT_CHAR, wxKeyEventHandler(KeyDialog::OnChar), NULL, this);
////@end KeyDialog content construction
#else
    if (!wxXmlResource::Get()->LoadDialog(this, GetParent(), _T("ID_KEYDIALOG")))
        wxLogError(wxT("Missing wxXmlResource::Get()->Load() in OnInit()?"));
    m_pCtrlSshKey = XRCCTRL(*this, "ID_TEXTCTRL_SSHKEY", wxTextCtrl);
    m_pCtrlSave = XRCCTRL(*this, "wxID_SAVE", wxButton);
    // Set validators
    if (FindWindow(XRCID("ID_TEXTCTRL_SSHKEY")))
        FindWindow(XRCID("ID_TEXTCTRL_SSHKEY"))->SetValidator( wxGenericValidator(& m_sSshKey) );
    // Connect events and objects
    m_pCtrlSshKey->Connect(XRCID("ID_TEXTCTRL_SSHKEY"), wxEVT_CHAR, wxKeyEventHandler(KeyDialog::OnChar), NULL, this);
#endif

    // Create custom windows not generated automatically here.
////@begin KeyDialog content initialisation
////@end KeyDialog content initialisation
}

/*!
 * Should we show tooltips?
 */

bool KeyDialog::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap KeyDialog::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin KeyDialog bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end KeyDialog bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon KeyDialog::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin KeyDialog icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end KeyDialog icon retrieval
}

void KeyDialog::CheckChanged()
{
    m_pCtrlSave->Enable(m_pCtrlSshKey->GetValue() != m_sSshKey);
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_IMPORT
 */

void KeyDialog::OnButtonImportClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_IMPORT in KeyDialog.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_IMPORT in KeyDialog. 
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_DEFAULT
 */

void KeyDialog::OnDEFAULTClick( wxCommandEvent& event )
{
    wxString fn;
    wxConfigBase::Get()->Read(wxT("Config/SystemNxDir"), &fn);
    fn << wxFileName::GetPathSeparator() << wxT("share")
       << wxFileName::GetPathSeparator() << wxT("keys")
       << wxFileName::GetPathSeparator() << wxT("server.id_dsa.key");
    m_pCtrlSshKey->LoadFile(fn);
    CheckChanged();
    event.Skip();
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_SAVE
 */

void KeyDialog::OnSAVEClick( wxCommandEvent& event )
{
    TransferDataFromWindow();
    CheckChanged();
    event.Skip();
}


/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXTCTRL_SSHKEY
 */

void KeyDialog::OnTextctrlSshkeyUpdated( wxCommandEvent& event )
{
    //if (m_bCharTyped)
        CheckChanged();
    m_bCharTyped = false;
    event.Skip();
}

/*!
 * wxEVT_CHAR event handler for ID_TEXTCTRL_SSHKEY
 */

void KeyDialog::OnChar( wxKeyEvent& event )
{
    m_bCharTyped = true;
    event.Skip();
}


