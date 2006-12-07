/////////////////////////////////////////////////////////////////////////////
// Name:        TextViewer.cpp
// Author:      Fritz Elfert
// Created:     29/03/2006 15:51:47
// RCS-ID:      $Id: TextViewer.cpp,v 1.7 2006/06/09 11:45:22 felfert Exp $
// Copyright:   (C) 2006 InnoviData GmbH
/////////////////////////////////////////////////////////////////////////////

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
    wxFileName fn(sFileName);

    m_sFileName = fn.GetFullPath();
    {
        wxLogNull l;
        ret = m_pTextCtrl->LoadFile(sFileName);
    }
    if (ret) {
        SetTitle(fn.GetName());
    } else {
        wxFileSystem fs;
        wxFSFile *f = fs.OpenFile(sFileName);
        if (f) {
            wxInputStream *is = f->GetStream();
            size_t sz = is->GetSize();
            wxString s;
            is->Read(s.GetWriteBuf(sz), sz);
            s.UngetWriteBuf();
            m_pTextCtrl->SetValue(s);
            m_pTextCtrl->DiscardEdits();
            ret = true;
            delete f;
        }
    }
    return ret;
}



