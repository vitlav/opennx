/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// Author:      Fritz Elfert
// Created:     06/17/04 03:34:48
// Copyright:   Copyright 2004 Millenux GmbH
// Licence:     LGPL
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma implementation "LogDialog.h"
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
#include <wx/textfile.h>

#include "LogDialog.h"
#include "Icon.h"

////@begin XPM images
////@end XPM images

/*!
 * LogDialog type definition
 */

IMPLEMENT_CLASS( LogDialog, wxDialog )

/*!
 * LogDialog event table definition
 */

BEGIN_EVENT_TABLE( LogDialog, wxDialog )

////@begin LogDialog event table entries
////@end LogDialog event table entries

END_EVENT_TABLE()

/*!
 * LogDialog constructors
 */

LogDialog::LogDialog( )
{
}

LogDialog::LogDialog( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Create(parent, id, caption, pos, size, style);
}

/*!
 * LogDialog creator
 */

bool LogDialog::Create( wxWindow* parent, wxWindowID WXUNUSED(id), const wxString& WXUNUSED(caption), const wxPoint& WXUNUSED(pos), const wxSize& WXUNUSED(size), long WXUNUSED(style) )
{
////@begin LogDialog member initialisation
    m_TextCtrl = NULL;
////@end LogDialog member initialisation

////@begin LogDialog creation
    SetExtraStyle(GetExtraStyle()|wxWS_EX_BLOCK_EVENTS);
    SetParent(parent);
    CreateControls();
    SetIcon(GetIconResource(wxT("res/nx.png")));
    GetSizer()->Fit(this);
    GetSizer()->SetSizeHints(this);
    Centre();
////@end LogDialog creation
    if (!m_sFileName.IsEmpty())
        ReadLogFile(m_sFileName);
    return TRUE;
}

/*!
 * Control creation for LogDialog
 */

void LogDialog::CreateControls()
{    
////@begin LogDialog content construction

    wxXmlResource::Get()->LoadDialog(this, GetParent(), _T("ID_DIALOG_LOG"));
    m_TextCtrl = XRCCTRL(*this, "ID_TEXTCTRL", wxTextCtrl);
////@end LogDialog content construction

    // Create custom windows not generated automatically here.

////@begin LogDialog content initialisation

////@end LogDialog content initialisation
}

void LogDialog::SetFileName(wxString fn)
{
    m_sFileName = fn;
}

void LogDialog::ReadLogFile(wxString name)
{
    if (!name.IsEmpty())
        m_sFileName = name;

    if (!m_sFileName.IsEmpty()) {
        wxTextFile f(m_sFileName);
        if (f.Open()) {
            wxString line;
            for(line = f.GetLastLine(); f.GetCurrentLine() > 0; line = f.GetPrevLine())
                m_TextCtrl->AppendText(line + _T("\n"));
        }
        f.Close();
    }
}

/*!
 * Should we show tooltips?
 */

bool LogDialog::ShowToolTips()
{
    return TRUE;
}

/*!
 * Get bitmap resources
 */

wxBitmap LogDialog::GetBitmapResource( const wxString& WXUNUSED(name) )
{
    // Bitmap retrieval
////@begin LogDialog bitmap retrieval
    return wxNullBitmap;
////@end LogDialog bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon LogDialog::GetIconResource( const wxString& name )
{
    // Icon retrieval
    return CreateIconFromFile(name);
}


