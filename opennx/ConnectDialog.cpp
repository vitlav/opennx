/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// Author:      Fritz Elfert
// Created:     09/28/04 12:57:18
// Copyright:   Copyright 2004 Millenux GmbH
// Licence:     LGPL
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma implementation "ConnectDialog.h"
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

#include "ConnectDialog.h"
#include "Icon.h"

////@begin XPM images
////@end XPM images

/*!
 * ConnectDialog type definition
 */

IMPLEMENT_DYNAMIC_CLASS( ConnectDialog, wxDialog )

/*!
 * ConnectDialog event table definition
 */

BEGIN_EVENT_TABLE( ConnectDialog, wxDialog )

////@begin ConnectDialog event table entries
    EVT_BUTTON( wxID_CANCEL, ConnectDialog::OnCancelClick )

////@end ConnectDialog event table entries

END_EVENT_TABLE()

/*!
 * ConnectDialog constructors
 */

ConnectDialog::ConnectDialog( )
{
}

ConnectDialog::ConnectDialog( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Create(parent, id, caption, pos, size, style);
}

/*!
 * ConnectDialog creator
 */

bool ConnectDialog::Create( wxWindow* parent, wxWindowID WXUNUSED(id), const wxString& WXUNUSED(caption), const wxPoint& WXUNUSED(pos), const wxSize& WXUNUSED(size), long WXUNUSED(style) )
{
////@begin ConnectDialog member initialisation
    m_bAbort = false;
    m_pCtrlStatus = NULL;
    m_pCtrlProgress = NULL;
////@end ConnectDialog member initialisation

////@begin ConnectDialog creation
    SetExtraStyle(GetExtraStyle()|wxWS_EX_BLOCK_EVENTS);
    SetParent(parent);
    CreateControls();
    SetIcon(GetIconResource(wxT("res/nx.png")));
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end ConnectDialog creation
    return TRUE;
}

/*!
 * Control creation for ConnectDialog
 */

void ConnectDialog::CreateControls()
{    
////@begin ConnectDialog content construction
    if (!wxXmlResource::Get()->LoadDialog(this, GetParent(), _T("ID_DIALOG_CONNECT")))
        wxLogError(wxT("Missing wxXmlResource::Get()->Load() in OnInit()?"));
    m_pCtrlStatus = XRCCTRL(*this, "ID_STATIC_STATUS", wxStaticText);
    m_pCtrlProgress = XRCCTRL(*this, "ID_GAUGE", wxGauge);
////@end ConnectDialog content construction

    // Create custom windows not generated automatically here.

////@begin ConnectDialog content initialisation
////@end ConnectDialog content initialisation
}

/*!
 * Should we show tooltips?
 */

bool ConnectDialog::ShowToolTips()
{
    return TRUE;
}

/*!
 * Get bitmap resources
 */

wxBitmap ConnectDialog::GetBitmapResource( const wxString& name)
{
    // Bitmap retrieval
    return CreateBitmapFromFile(name);
}

/*!
 * Get icon resources
 */

wxIcon ConnectDialog::GetIconResource( const wxString& name )
{
    // Icon retrieval
    return CreateIconFromFile(name);
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_ABORT
 */

void ConnectDialog::OnCancelClick( wxCommandEvent& WXUNUSED(event) )
{
    m_bAbort = true;
}


void ConnectDialog::SetStatusText(wxString txt)
{
    m_pCtrlStatus->SetLabel(txt);
}

void ConnectDialog::SetProgress(int i)
{
    if (i < 0)
        i = 0;
    if (i > 100)
        i = 100;
    m_pCtrlProgress->SetValue(i);
}

