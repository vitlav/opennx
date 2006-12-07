/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// Author:      Fritz Elfert
// Created:     08/08/04 18:31:09
// Copyright:   Copyright 2004 Millenux GmbH
// Licence:     LGPL
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma implementation "AboutDialog.h"
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
#include "AboutDialog.h"
#include "TextViewer.h"
#include "Icon.h"
#include "opennxApp.h"

////@begin XPM images
////@end XPM images

#ifdef MYTRACETAG
# undef MYTRACETAG
#endif
#define MYTRACETAG wxT("AboutDialog")

/**
 * Custom HTML window.
 * This derivate of wxHtmlWindow interpretes links in a special
 * way:<br>
 * <ul>
 * <li>If "TV" is specified as target, a builting TextViewer is used.
 * <li>Other links are opened in an external browser.
 * </ul>
 */
class extHtmlWindow : public wxHtmlWindow
{
    DECLARE_DYNAMIC_CLASS( extHtmlWindow )

public:
    extHtmlWindow() : wxHtmlWindow() { }

    /**
     * Performs the actual action.
     * If target is TV, open url in internal TextViewer,
     * otherwise in external browser.
     */
    virtual void OnLinkClicked(const wxHtmlLinkInfo&);
};

IMPLEMENT_DYNAMIC_CLASS( extHtmlWindow, wxHtmlWindow )

void extHtmlWindow::OnLinkClicked(const wxHtmlLinkInfo& link)
{
    wxString href = link.GetHref();
    wxString target = link.GetTarget();
    if (target.StartsWith(wxT("TV:"))) {
        TextViewer *tw = new TextViewer(this);
        tw->LoadFile(href);
        target = target.AfterFirst(wxT(':'));
        if (!target.IsEmpty())
            tw->SetTitle(target);
        tw->Show(true);
        return;
    }
#ifdef __WXMSW__
    ShellExecute((HWND)GetHandle(), wxT("open"), href.c_str(), NULL, NULL, SW_SHOWNORMAL);
    return;
#endif
#ifdef __UNIX__
    wxString browser;
    if (wxGetEnv(wxT("BROWSER"), &browser)) {
        if (wxExecute(browser + wxT(" \"") + href + wxT("\"")))
            return;
    }
    if (wxExecute(wxT("htmlview \"") + href + wxT("\"")))
        return;
    if (wxExecute(wxT("kfmklient openURL \"") + href + wxT("\"")))
        return;
    if (wxExecute(wxT("gnome-open \"") + href + wxT("\"")))
        return;
    if (wxExecute(wxT("firefox -url \"") + href + wxT("\"")))
        return;
    if (wxExecute(wxT("mozilla \"") + href + wxT("\"")))
        return;
#endif
}

/*!
 * AboutDialog type definition
 */

IMPLEMENT_DYNAMIC_CLASS( AboutDialog, wxDialog )

    /*!
     * AboutDialog event table definition
     */

BEGIN_EVENT_TABLE( AboutDialog, wxDialog )

////@begin AboutDialog event table entries
////@end AboutDialog event table entries

END_EVENT_TABLE()

    /*!
     * AboutDialog constructors
     */

AboutDialog::AboutDialog( )
{
}

AboutDialog::AboutDialog( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Create(parent, id, caption, pos, size, style);
}

/*!
 * AboutDialog creator
 */

bool AboutDialog::Create( wxWindow* parent, wxWindowID WXUNUSED(id), const wxString& WXUNUSED(caption), const wxPoint& WXUNUSED(pos), const wxSize& WXUNUSED(size), long WXUNUSED(style) )
{
////@begin AboutDialog member initialisation
    m_pHtmlWindow = NULL;
////@end AboutDialog member initialisation

////@begin AboutDialog creation
    SetExtraStyle(GetExtraStyle()|wxWS_EX_BLOCK_EVENTS);
    SetParent(parent);
    CreateControls();
    SetIcon(GetIconResource(wxT("res/nx.png")));
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end AboutDialog creation
    return TRUE;
}

/*!
 * Control creation for AboutDialog
 */

void AboutDialog::CreateControls()
{    
////@begin AboutDialog content construction
    if (!wxXmlResource::Get()->LoadDialog(this, GetParent(), _T("ID_DIALOG_ABOUT")))
        wxLogError(wxT("Missing wxXmlResource::Get()->Load() in OnInit()?"));
    m_pHtmlWindow = XRCCTRL(*this, "ID_HTMLWINDOW_ABOUT", extHtmlWindow);
////@end AboutDialog content construction

    // Create custom windows not generated automatically here.

////@begin AboutDialog content initialisation
////@end AboutDialog content initialisation

    int fs[7];
    wxFont fv = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
    wxFont ff = wxSystemSettings::GetFont(wxSYS_ANSI_FIXED_FONT);
    for (int i = 0; i < 7; i++)
        fs[i] = fv.GetPointSize();
    m_pHtmlWindow->SetFonts(fv.GetFaceName(), ff.GetFaceName(), fs);
    m_pHtmlWindow->SetBorders(0);

    wxString version = _("Version") + wxString::Format(wxT(" <B>%s</B>"),
        ::wxGetApp().GetVersion().c_str());
#ifdef __WXDEBUG__
    version += wxT(" (DEBUG)");
#else
    version += wxT(" (RELEASE)");
#endif

    wxString content = ::wxGetApp().LoadFileFromResource(wxT("res/about.html"));
    content.Replace(wxT("<VERSION>"), version);
    content.Replace(wxT("<WXVERSION>"), wxVERSION_STRING);
    content.Replace(wxT("\"res:"), wxT("\"") + ::wxGetApp().GetResourcePrefix());

    m_pHtmlWindow->SetPage(content);
    m_pHtmlWindow->SetBackgroundColour(GetBackgroundColour());

    int width, height;
    m_pHtmlWindow->GetSize(&width, &height);
    m_pHtmlWindow->GetInternalRepresentation()->Layout(width);
    height = m_pHtmlWindow->GetInternalRepresentation()->GetHeight();
    width = m_pHtmlWindow->GetInternalRepresentation()->GetWidth();
    m_pHtmlWindow->SetSize(width, height);
    m_pHtmlWindow->SetSizeHints(width, height);
    Fit();
}

/*!
 * Should we show tooltips?
 */

bool AboutDialog::ShowToolTips()
{
    return TRUE;
}

/*!
 * Get bitmap resources
 */

wxBitmap AboutDialog::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
    return CreateBitmapFromFile(name);
}

/*!
 * Get icon resources
 */

wxIcon AboutDialog::GetIconResource( const wxString& name )
{
    // Icon retrieval
    return CreateIconFromFile(name);
}
