/////////////////////////////////////////////////////////////////////////////
// $Id: AboutDialog.cpp 883 2004-11-04 21:15:18Z felfert $
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
#include "Icon.h"

////@begin XPM images
////@end XPM images

// Like wxHtmlWindow, but all links are opened in
// an external browser.
class extHtmlWindow : public wxHtmlWindow
{
    DECLARE_DYNAMIC_CLASS( extHtmlWindow )

public:
    extHtmlWindow() : wxHtmlWindow() { }
    virtual void OnLinkClicked(const wxHtmlLinkInfo&);
};

IMPLEMENT_DYNAMIC_CLASS( extHtmlWindow, wxHtmlWindow )

void extHtmlWindow::OnLinkClicked(const wxHtmlLinkInfo& link)
{
    wxString href = link.GetHref();
#ifdef __WXMSW__
    ShellExecute((HWND)GetHandle(), _T("open"), href.c_str(), NULL, NULL, SW_SHOWNORMAL);
    return;
#endif
#ifdef __UNIX__
    wxString browser;
    if (wxGetEnv(_T("BROWSER"), &browser)) {
        if (wxExecute(browser + _T(" \"") + href + _T("\"")))
            return;
    }
    if (wxExecute(_T("htmlview \"") + href + _T("\"")))
        return;
    if (wxExecute(_T("kfmklient openURL \"") + href + _T("\"")))
        return;
    if (wxExecute(_T("gnome-open \"") + href + _T("\"")))
        return;
    if (wxExecute(_T("firefox -url \"") + href + _T("\"")))
        return;
    if (wxExecute(_T("mozilla \"") + href + _T("\"")))
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
    m_pAboutHtml = NULL;
////@end AboutDialog member initialisation

////@begin AboutDialog creation
    SetExtraStyle(GetExtraStyle()|wxWS_EX_BLOCK_EVENTS);
    SetParent(parent);
    CreateControls();
    SetIcon(GetIconResource(wxT("res/nx.png")));
    GetSizer()->Fit(this);
    GetSizer()->SetSizeHints(this);
    Centre();
////@end AboutDialog creation
#ifdef __WXMSW__
    DWORD dummy;
    DWORD viSize;
    LPVOID vi;
    TCHAR mySelf[MAX_PATH];
    VS_FIXEDFILEINFO *vsFFI;
    UINT vsFFIlen;
    wxString sVersion = _T("?.?");
    
    if (GetModuleFileName(NULL, mySelf, sizeof(mySelf))) {
        viSize = GetFileVersionInfoSize(mySelf, &dummy);
        if (viSize) {
            vi = (LPVOID)malloc(viSize);
            if (vi) {
                if (GetFileVersionInfo(mySelf, dummy, viSize, vi)) {
                    if (VerQueryValue(vi, _T("\\"), (LPVOID *)&vsFFI, &vsFFIlen)) {
                        sVersion = wxString::Format(_T("%d.%d"), HIWORD(vsFFI->dwFileVersionMS),
                            LOWORD(vsFFI->dwFileVersionMS));
                        if (vsFFI->dwFileVersionLS)
                            sVersion += wxString::Format(_T(".%d"), HIWORD(vsFFI->dwFileVersionLS));
                        if (LOWORD(vsFFI->dwFileVersionLS))
                            sVersion += wxString::Format(_T(".%d"), LOWORD(vsFFI->dwFileVersionLS));
                    }
                    
                }
                free(vi);
            }
        }
    }

#else
    wxString sVersion = wxT(PACKAGE_VERSION);
#endif

    int fs[7];
    wxString content;

    wxFont fv = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
    wxFont ff = wxSystemSettings::GetFont(wxSYS_ANSI_FIXED_FONT);
    for (int i = 0; i < 7; i++)
        fs[i] = fv.GetPointSize();
    m_pAboutHtml->SetFonts(fv.GetFaceName(), ff.GetFaceName(), fs);
    m_pAboutHtml->SetBorders(0);
    content = _T("<HTML><BODY BGCOLOR=\"#FFFFFF\"><CENTER>");
    content += _T("<IMG SRC=\"file:mxclient.rsc#zip:res/mxclient.png\"></IMG><P>");
#ifdef __WXDEBUG__
    content += _("Version") + wxString(_T(" <B>")) + sVersion + _T("</B> (DEBUG)<P>");
#else
    content += _("Version") + wxString(_T(" <B>")) + sVersion + _T("</B> (RELEASE)<BR>");
#endif
    content += _T("Copyright &copy; 2004 <A HREF=\"http://www.millenux.com\">Millenux GmbH</A><BR>");
    content += _T("</CENTER></BODY></HTML>");
    m_pAboutHtml->SetPage(content);
    int width, height;
    m_pAboutHtml->GetSize(&width, &height);
    m_pAboutHtml->GetInternalRepresentation()->Layout(width);
    height = m_pAboutHtml->GetInternalRepresentation()->GetHeight();
    m_pAboutHtml->SetSize(width, height);
    m_pAboutHtml->SetSizeHints(width, height);
    Fit();
    return TRUE;
}

/*!
 * Control creation for AboutDialog
 */

void AboutDialog::CreateControls()
{    
////@begin AboutDialog content construction

    wxXmlResource::Get()->LoadDialog(this, GetParent(), _T("ID_DIALOG_ABOUT"));
    m_pAboutHtml = XRCCTRL(*this, "ID_HTMLWINDOW_ABOUT", extHtmlWindow);
////@end AboutDialog content construction

    // Create custom windows not generated automatically here.

////@begin AboutDialog content initialisation

////@end AboutDialog content initialisation
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
