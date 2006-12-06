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
#include "Icon.h"
#include "mxclientApp.h"

#include <wx/tokenzr.h>

////@begin XPM images
////@end XPM images

#ifdef MYTRACETAG
# undef MYTRACETAG
#endif
#define MYTRACETAG wxT("AboutDialog")

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

#ifdef __UNIX__
    static bool
which(wxString cmd)
{
    if (!(cmd.StartsWith(wxT("/")) || cmd.StartsWith(wxT("./")))) {
        wxString path;
        if (::wxGetEnv(wxT("PATH"), &path)) {
            wxStringTokenizer t(path, wxT(":"));
            while (t.HasMoreTokens()) {
                wxString tst = t.GetNextToken() + wxT("/") + cmd;
                if (::access(tst.fn_str(), R_OK|X_OK) == 0)
                    return true;
            }
        }
    } else
        return (::access(cmd.fn_str(), R_OK|X_OK) == 0);
    return false;
}
#endif

void extHtmlWindow::OnLinkClicked(const wxHtmlLinkInfo& link)
{
    wxString href = link.GetHref();
#ifdef __WXMSW__
    ShellExecute((HWND)GetHandle(), wxT("open"), href.c_str(), NULL, NULL, SW_SHOWNORMAL);
    return;
#endif
#ifdef __UNIX__
    wxString browser;
    if (::wxGetEnv(wxT("BROWSER"), &browser)) {
        ::wxExecute(browser + wxT(" \"") + href + wxT("\""));
        return;
    }
    if (which(wxT("htmlview"))) {
        ::wxExecute(wxT("htmlview \"") + href + wxT("\""));
        return;
    }
    if (which(wxT("kfmclient"))) {
        ::wxExecute(wxT("kfmklient openURL \"") + href + wxT("\""));
        return;
    }
    if (which(wxT("gnome-open"))) {
        ::wxExecute(wxT("gnome-open \"") + href + wxT("\""));
        return;
    }
    if (which(wxT("firefox"))) {
        ::wxExecute(wxT("firefox -url \"") + href + wxT("\""));
        return;
    }
    if (which(wxT("mozilla"))) {
        ::wxExecute(wxT("mozilla \"") + href + wxT("\""));
        return;
    }
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
    wxString sVersion = wxT("?.?");

    if (GetModuleFileName(NULL, mySelf, sizeof(mySelf))) {
        viSize = GetFileVersionInfoSize(mySelf, &dummy);
        if (viSize) {
            vi = (LPVOID)malloc(viSize);
            if (vi) {
                if (GetFileVersionInfo(mySelf, dummy, viSize, vi)) {
                    if (VerQueryValue(vi, wxT("\\"), (LPVOID *)&vsFFI, &vsFFIlen)) {
                        sVersion = wxString::Format(wxT("%d.%d"), HIWORD(vsFFI->dwFileVersionMS),
                                LOWORD(vsFFI->dwFileVersionMS));
                        if (vsFFI->dwFileVersionLS)
                            sVersion += wxString::Format(wxT(".%d"), HIWORD(vsFFI->dwFileVersionLS));
                        if (LOWORD(vsFFI->dwFileVersionLS))
                            sVersion += wxString::Format(wxT(".%d"), LOWORD(vsFFI->dwFileVersionLS));
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
    content = wxT("<HTML><BODY BGCOLOR=\"#FFFFFF\"><CENTER>");
    content += wxT("<IMG SRC=\"") + wxGetApp().getResourcePrefix() + wxT("res/mxclient.png\"></IMG><P>");
#ifdef __WXDEBUG__
    content += _("Version") + wxString(wxT(" <B>")) + sVersion + wxT("</B> (DEBUG)<P>");
#else
    content += _("Version") + wxString(wxT(" <B>")) + sVersion + wxT("</B> (RELEASE)<BR>");
#endif
    content += wxT("Copyright &copy; 2004 <A HREF=\"http://www.millenux.com\">Millenux GmbH</A><BR>");
    content += wxT("</CENTER></BODY></HTML>");
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

    wxXmlResource::Get()->LoadDialog(this, GetParent(), wxT("ID_DIALOG_ABOUT"));
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
