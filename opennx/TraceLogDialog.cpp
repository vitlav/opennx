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
#pragma implementation "TraceLogDialog.h"
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

#include "TraceLogDialog.h"

////@begin XPM images
////@end XPM images


/*!
 * TraceLogDialog type definition
 */

IMPLEMENT_DYNAMIC_CLASS( TraceLogDialog, wxDialog )


/*!
 * TraceLogDialog event table definition
 */

BEGIN_EVENT_TABLE( TraceLogDialog, wxDialog )

////@begin TraceLogDialog event table entries
    EVT_BUTTON( wxID_CLOSE, TraceLogDialog::OnCLOSEClick )

////@end TraceLogDialog event table entries

END_EVENT_TABLE()


/*!
 * TraceLogDialog constructors
 */

TraceLogDialog::TraceLogDialog()
{
    Init();
}

TraceLogDialog::TraceLogDialog( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*!
 * TraceLogDialog creator
 */

bool TraceLogDialog::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin TraceLogDialog creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    SetParent(parent);
    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end TraceLogDialog creation
    return true;
}


/*!
 * TraceLogDialog destructor
 */

TraceLogDialog::~TraceLogDialog()
{
////@begin TraceLogDialog destruction
////@end TraceLogDialog destruction
}


/*!
 * Member initialisation
 */

void TraceLogDialog::Init()
{
////@begin TraceLogDialog member initialisation
////@end TraceLogDialog member initialisation
}


/*!
 * Control creation for TraceLogDialog
 */

void TraceLogDialog::CreateControls()
{    
////@begin TraceLogDialog content construction
    if (!wxXmlResource::Get()->LoadDialog(this, GetParent(), _T("ID_TRACELOGDIALOG")))
        wxLogError(wxT("Missing wxXmlResource::Get()->Load() in OnInit()?"));
////@end TraceLogDialog content construction

    // Create custom windows not generated automatically here.
////@begin TraceLogDialog content initialisation
////@end TraceLogDialog content initialisation

    m_pCtrlTraceLog->InsertColumn(0, _("Time"));
    m_pCtrlTraceLog->InsertColumn(1, _("PID"), wxLIST_FORMAT_RIGHT);
    m_pCtrlTraceLog->InsertColumn(2, _("Text"));
}

void TraceLogDialog::AddEntry(pid_t pid, wxDateTime stamp, wxString &txt)
{
    wxListItem i;
    i.SetColumn(0);
    i.SetText(stamp.Format(wxT("%c")));
    m_pCtrlTraceLog->InsertItem(i);
    i.SetColumn(1);
    i.SetText(wxString::Format(wxT("%d"), pid));
    m_pCtrlTraceLog->InsertItem(i);
    i.SetColumn(2);
    i.SetText(txt);
    m_pCtrlTraceLog->InsertItem(i);
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CLOSE
 */

void TraceLogDialog::OnCLOSEClick( wxCommandEvent& event )
{
    Show(false);
    event.Skip();
}


/*!
 * Should we show tooltips?
 */

bool TraceLogDialog::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap TraceLogDialog::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin TraceLogDialog bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end TraceLogDialog bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon TraceLogDialog::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin TraceLogDialog icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end TraceLogDialog icon retrieval
}
