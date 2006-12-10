// $Id: opennxApp.cpp 360 2006-12-08 00:58:21Z felfert $
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

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "TextViewer.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/wfstream.h>
#include <wx/txtstrm.h>

#include "debugleak.h"

////@begin includes
////@end includes

#include "TextViewer.h"
#include "opennxApp.h"

////@begin XPM images
////@end XPM images

/*!
 * TextViewer type definition
 */

IMPLEMENT_DYNAMIC_CLASS( TextViewer, wxDialog )

    /*!
     * TextViewer event table definition
     */

BEGIN_EVENT_TABLE( TextViewer, wxDialog )

////@begin TextViewer event table entries
    EVT_BUTTON( wxID_CLOSE, TextViewer::OnCloseClick )

////@end TextViewer event table entries

END_EVENT_TABLE()

/*!
 * TextViewer constructors
 */

TextViewer::TextViewer( )
{
}

TextViewer::TextViewer( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Create(parent, id, caption, pos, size, style);
}

/*!
 * TextViewer creator
 */

bool TextViewer::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    ////@begin TextViewer member initialisation
    m_sFileName = wxT("");
    m_pTextCtrl = NULL;
    ////@end TextViewer member initialisation

    ////@begin TextViewer creation
    SetExtraStyle(GetExtraStyle()|wxWS_EX_BLOCK_EVENTS);
    SetParent(parent);
    CreateControls();
    SetIcon(GetIconResource(wxT("res/nx.png")));
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
    ////@end TextViewer creation
    wxUnusedVar(style);
    wxUnusedVar(size);
    wxUnusedVar(pos);
    wxUnusedVar(caption);
    wxUnusedVar(id);
    return true;
}

/*!
 * Control creation for TextViewer
 */

void TextViewer::CreateControls()
{    
    ////@begin TextViewer content construction
    if (!wxXmlResource::Get()->LoadDialog(this, GetParent(), _T("ID_TEXTVIEWER")))
        wxLogError(wxT("Missing wxXmlResource::Get()->Load() in OnInit()?"));
    m_pTextCtrl = XRCCTRL(*this, "ID_TEXTCTRL", wxTextCtrl);
    ////@end TextViewer content construction

    // Create custom windows not generated automatically here.

    ////@begin TextViewer content initialisation
    ////@end TextViewer content initialisation
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CLOSE
 */

void TextViewer::OnCloseClick( wxCommandEvent& event )
{
    Destroy();
    event.Skip();
}

/*!
 * Should we show tooltips?
 */

bool TextViewer::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap TextViewer::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
    ////@begin TextViewer bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
    ////@end TextViewer bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon TextViewer::GetIconResource( const wxString& name )
{
    // Icon retrieval
    ////@begin TextViewer icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
    ////@end TextViewer icon retrieval
}

    bool
TextViewer::LoadFile(const wxString &sFileName)
{
    bool ret = false;
    m_sFileName = sFileName;
    wxFileName fn(sFileName);
    bool isRtf = (fn.GetExt().CmpNoCase(wxT("rtf")) == 0);

#ifdef __UNIX__
    // On unix, wxTextCtrl doesn't support RTF, so we use .txt there
    if (isRtf)
        fn.SetExt(wxT("txt"));
    m_sFileName = fn.GetFullPath();
#endif
    {
        wxLogNull l;
        ret = m_pTextCtrl->LoadFile(m_sFileName);
    }
    if (ret) {
        SetTitle(fn.GetName());
    } else {
        wxFileSystem fs;
        wxFSFile *f = fs.OpenFile(m_sFileName);
        if (f) {
            size_t sz = f->GetStream()->GetSize();
            if (!isRtf) {
                char *buf = new char[sz + 1];
                f->GetStream()->Read(buf, sz);
                buf[sz] = '\0';
                m_pTextCtrl->SetValue(wxConvLocal.cMB2WX(buf));
                delete buf;
            } else {
                wxString s;
                f->GetStream()->Read(s.GetWriteBuf(sz), sz);
                s.UngetWriteBuf();
                m_pTextCtrl->SetValue(s);
            }
            m_pTextCtrl->DiscardEdits();
            ret = true;
            delete f;
        }
    }
    return ret;
}
