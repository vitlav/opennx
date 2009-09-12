/////////////////////////////////////////////////////////////////////////////
// Name:        macuninstallapp.cpp
// Purpose:     
// Author:      Fritz Elfert
// Modified by: 
// Created:     Fri 11 Sep 2009 01:17:42 PM CEST
// RCS-ID:      
// Copyright:   (C) 2009 by Fritz Elfert
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifdef HAVE_CONFIG_H
# include "config.h"
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
#include <wx/stdpaths.h>
#include <wx/apptrait.h>
#include <wx/filename.h>
#include <wx/cmdline.h>
#include <wx/xml/xml.h>
#include <wx/arrstr.h>
#include <wx/wfstream.h>
#include <wx/txtstrm.h>
#include <wx/dir.h>
#include <Security/Authorization.h>
#include <Security/AuthorizationTags.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "macuninstallapp.h"

////@begin XPM images

////@end XPM images

class RmRfTraverser : public wxDirTraverser
{
    public:
        RmRfTraverser() { }

        ~RmRfTraverser()
        {
            int n = m_aFiles.GetCount() - 1;
            wxString fn;
            wxTextOutputStream *s = wxGetApp().GetLog();
            while (n >= 0) {
                fn = m_aFiles[n--];
                if (::wxRemoveFile(fn)) {
                    if (s)
                        *s << wxT("rm ") << fn << endl;
                }
            }
            n = m_aDirs.GetCount() - 1;
            while (n >= 0) {
                fn = m_aDirs[n--];
                if (::wxRmdir(fn)) {
                    if (s)
                        *s << wxT("rmdir ") << fn << endl;
                }
            }
        }

        virtual wxDirTraverseResult OnFile(const wxString& filename)
        {
            m_aFiles.Add(filename);
            return wxDIR_CONTINUE;
        }

        virtual wxDirTraverseResult OnDir(const wxString& dirpath)
        {
            m_aDirs.Add(dirpath);
            return wxDIR_CONTINUE;
        }

    private:
        wxArrayString m_aDirs;
        wxArrayString m_aFiles;
};

/*
 * Application instance implementation
 */

////@begin implement app
IMPLEMENT_APP( MacUninstallApp )
////@end implement app


/*
 * MacUninstallApp type definition
 */

IMPLEMENT_CLASS( MacUninstallApp, wxApp )


/*
 * MacUninstallApp event table definition
 */

BEGIN_EVENT_TABLE( MacUninstallApp, wxApp )

////@begin MacUninstallApp event table entries
////@end MacUninstallApp event table entries

END_EVENT_TABLE()


/*
 * Constructor for MacUninstallApp
 */

MacUninstallApp::MacUninstallApp()
{
    Init();
}


/*
 * Member initialisation
 */

void MacUninstallApp::Init()
{
////@begin MacUninstallApp member initialisation
////@end MacUninstallApp member initialisation
    m_log = NULL;
    m_bBatchMode = false;
    m_sSelfPath = wxFileName(
            GetTraits()->GetStandardPaths().GetExecutablePath()).GetFullPath();
    m_nodelete.insert(wxT("."));
    m_nodelete.insert(wxT("./Applications"));
    m_nodelete.insert(wxT("./Library"));
}

void MacUninstallApp::OnInitCmdLine(wxCmdLineParser& parser)
{
    // Init standard options (--help, --verbose);
    wxApp::OnInitCmdLine(parser);

    parser.AddSwitch(wxEmptyString, wxT("batch"),
            _("Uninstall without asking the user (needs admin rights)."));
}

bool MacUninstallApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
    m_bBatchMode = parser.Found(wxT("batch"));
    return true;
}

/*
 * Initialisation for MacUninstallApp
 */

bool MacUninstallApp::OnInit()
{
    wxFileName fn(m_sSelfPath);
    fn.RemoveLastDir();
    fn.AppendDir(wxT("share"));
    fn.AppendDir(wxT("locale"));
    m_cLocale.AddCatalogLookupPathPrefix(fn.GetPath());
    m_cLocale.Init();
    m_cLocale.AddCatalog(wxT("opennx"));

    // Call to base class needed for initializing command line processing
    if (!wxApp::OnInit())
        return false;

#if wxUSE_XPM
    wxImage::AddHandler(new wxXPMHandler);
#endif
#if wxUSE_LIBPNG
    wxImage::AddHandler(new wxPNGHandler);
#endif
#if wxUSE_LIBJPEG
    wxImage::AddHandler(new wxJPEGHandler);
#endif
#if wxUSE_GIF
    wxImage::AddHandler(new wxGIFHandler);
#endif

    if (m_bBatchMode) {
        wxLogNull ignoreErrors;
        if (0 != geteuid()) {
            ::wxMessageBox(_("Batch uninstall needs to be started as root."),
                    _("Uninstall OpenNX"), wxOK|wxICON_ERROR);
            return false;
        }
        DoUninstall(wxT("OpenNX"));
        if (m_log) {
            *m_log << wxT("Uninstall finished at ")
                << wxDateTime::Now().Format() << endl;
            delete m_log;
            m_log = NULL;
        }
    } else {
        int r = ::wxMessageBox(
                _("This operation can not be undone!\nDo you really want to uninstall OpenNX?"),
                _("Uninstall OpenNX"), wxYES_NO|wxNO_DEFAULT|wxICON_EXCLAMATION);
        if (wxYES == r) {
            if (!TestReceipt(wxT("OpenNX"))) {
                while (Pending())
                    Dispatch();
                return false;
            }
            ElevatedUninstall(wxT("OpenNX"),
                    _("In order to uninstall OpenNX, administrative rights are required.\n\n"));
        }
    }
    return false;
}


/*
 * Cleanup for MacUninstallApp
 */

int MacUninstallApp::OnExit()
{
    wxLogNull ignoreErrors;
    ////@begin MacUninstallApp cleanup
    return wxApp::OnExit();
    ////@end MacUninstallApp cleanup
}

wxString MacUninstallApp::GetInstalledPath(bool reportErrors,
        const wxString &rcptName)
{
    wxXmlDocument rcpt(wxT("/Library/Receipts/OpenNX.pkg/Contents/Info.plist"));
    if (rcpt.IsOk()) {
        if (rcpt.GetRoot()->GetName() != wxT("plist")) {
            if (reportErrors)
                ::wxLogError(_("Invalid plist format"));
            return wxEmptyString;
        }
        wxXmlNode *child = rcpt.GetRoot()->GetChildren();
        if (child->GetName() != wxT("dict")) {
            if (reportErrors)
                ::wxLogError(_("Invalid plist format"));
            return wxEmptyString;
        }
        child = child->GetChildren();
        bool needkey = true;
        bool found = false;
        while (child) {
            if (needkey) {
                if (child->GetName() != wxT("key")) {
                    if (reportErrors)
                        ::wxLogError(_("Invalid plist format"));
                    return wxEmptyString;
                }
                if (child->GetNodeContent() == wxT("IFPkgRelocatedPath"))
                    found = true;
            } else {
                if (found) {
                    if (child->GetName() != wxT("string")) {
                        if (reportErrors)
                            ::wxLogError(_("Invalid plist format"));
                        return wxEmptyString;
                    }
                    return child->GetNodeContent();
                }
            }
            needkey = (!needkey);
            child = child->GetNext();
        }
    } else
        if (reportErrors)
            ::wxLogError(_("Could not read package receipt"));
    return wxEmptyString;
}

bool MacUninstallApp::FetchBOM(bool reportErrors, const wxString &bom,
        wxArrayString &dirs, wxArrayString &files)
{
    if (!wxFileName::FileExists(bom)) {
        if (reportErrors)
            ::wxLogError(
                    _("Missing BOM (Bill Of Materials). Already unistalled?"));
        return false;
    }
    wxString cmd(wxT("lsbom -fbcl -p f "));
    cmd << bom;
    wxArrayString err;
    if (0 != ::wxExecute(cmd, files, err)) {
        if (reportErrors)
            ::wxLogError(
                    _("Could not list BOM (Bill Of Materials). Already unistalled?"));
        return false;
    }
    if (0 != err.GetCount() != 0) {
        if (reportErrors)
            ::wxLogError(
                    _("Invalid BOM (Bill Of Materials). Already unistalled?"));
        return false;
    }
    cmd = wxT("lsbom -d -p f ");
    cmd << bom;
    err.Empty();
    if (0 != ::wxExecute(cmd, dirs, err)) {
        if (reportErrors)
            ::wxLogError(
                    _("Could not list BOM (Bill Of Materials). Already unistalled?"));
        return false;
    }
    if (0 != err.GetCount() != 0) {
        if (reportErrors)
            ::wxLogError(
                    _("Invalid BOM (Bill Of Materials). Already unistalled?"));
        return false;
    }
    return true;
}

bool MacUninstallApp::TestReceipt(const wxString &pkg)
{
    wxString rpath = wxT("/Library/Receipts/");
    rpath.Append(pkg).Append(wxT(".pkg"));
    if (!wxFileName::DirExists(rpath)) {
        ::wxLogWarning(
                _("The package receipt does not exist. Already unistalled?"));
        return false;
    }
    wxString proot = GetInstalledPath(true,
            rpath + wxT("/Contents/Info.plist"));
    if (proot.IsEmpty())
        return false;
    if (!wxFileName::DirExists(proot)) {
        ::wxLogWarning(
                _("The package install path does not exist. Already unistalled?"));
        return false;
    }
    wxArrayString d;
    wxArrayString f;
    if (!FetchBOM(true, rpath + wxT("/Contents/Archive.bom"), d, f))
        return false;
    return true;
}

void MacUninstallApp::DoUninstall(const wxString &pkg)
{
    wxString logname = wxT("/tmp/uninstall-") + pkg;
    logname << wxT(".log");
    wxFileOutputStream *fo = new wxFileOutputStream(logname);
    m_log = new wxTextOutputStream(*fo);
    *m_log << wxT("Uninstall started at ")
        << wxDateTime::Now().Format() << endl;
    wxString rpath = wxT("/Library/Receipts/");
    rpath.Append(pkg).Append(wxT(".pkg"));
    wxString proot = GetInstalledPath(false,
            rpath + wxT("/Contents/Info.plist"));
    if (proot.IsEmpty()) {
        *m_log << wxT("No packageroot found") << endl;
        return;
    }
    wxArrayString d;
    wxArrayString f;
    if (!FetchBOM(false, rpath + wxT("/Contents/Archive.bom"), d, f)) {
        *m_log << wxT("Could not read BOM") << endl;
        return;
    }
    size_t i;
    *m_log << wxT("Deleting files:") << endl;
    for (i = 0; i < f.GetCount(); i++) {
        if (m_nodelete.find(f[i]) != m_nodelete.end()) {
            f.RemoveAt(i--);
            continue;
        }
        wxFileName fn(proot + f[i]);
        if (fn.Normalize(wxPATH_NORM_DOTS|wxPATH_NORM_ABSOLUTE)) {
            if (::wxRemoveFile(fn.GetFullPath())) {
                f.RemoveAt(i--);
                *m_log << wxT("rm ") << fn.GetFullPath() << endl;
            }
        }
    }
    size_t lcd;
    *m_log << wxT("Deleting directories:") << endl;
    do {
        lcd = d.GetCount();
        for (i = 0; i < d.GetCount(); i++) {
            if (m_nodelete.find(d[i]) != m_nodelete.end()) {
                d.RemoveAt(i--);
                continue;
            }
            wxFileName fn(proot + d[i]);
            if (fn.Normalize(wxPATH_NORM_DOTS|wxPATH_NORM_ABSOLUTE)) {
                if (::wxRmdir(fn.GetFullPath())) {
                    d.RemoveAt(i--);
                    *m_log << wxT("rmdir ") << fn.GetFullPath() << endl;
                }
            }
        }
    } while (lcd != d.GetCount());
    if (0 < f.GetCount()) {
        *m_log << wxT("=========================") << endl;
        *m_log << wxT("Files that could not be deleted:") << endl;
        for (i = 0; i < f.GetCount(); i++)
            *m_log << wxT(" ") << f[i] << endl;
    }
    if (0 < d.GetCount()) {
        *m_log << wxT("=========================") << endl;
        *m_log << wxT("Directories that could not be deleted:") << endl;
        for (i = 0; i < d.GetCount(); i++)
            *m_log << wxT(" ") << d[i] << endl;
    }
    if (0 == (d.GetCount() + f.GetCount())) {
        // Finally delete the receipe itself
        *m_log << wxT("Deleting receipt:") << endl;
        {
            wxDir d(rpath);
            RmRfTraverser t;
            d.Traverse(t);
        }
        if (::wxRmdir(rpath))
            *m_log << wxT("rmdir ") << rpath << endl;
    }
}

void MacUninstallApp::ElevatedUninstall(const wxString &pkg,
        const wxString &msg)
{
    wxLogNull ignoreErrors;
    if (geteuid() == 0) {
        DoUninstall(pkg);
        return;
    }
    char *prompt = strdup(msg.utf8_str());

    OSStatus st;
    AuthorizationFlags aFlags = kAuthorizationFlagDefaults;
    AuthorizationRef aRef;
    AuthorizationItem promptItem = {
        kAuthorizationEnvironmentPrompt, strlen(prompt), prompt, 0
    };
    AuthorizationEnvironment aEnv = { 1, &promptItem };

    st = AuthorizationCreate(NULL, kAuthorizationEmptyEnvironment,
            kAuthorizationFlagDefaults, &aRef);
    if (errAuthorizationSuccess != st)
        return;
    AuthorizationItem aItems = { kAuthorizationRightExecute, 0, NULL, 0 };
    AuthorizationRights aRights = { 1, &aItems };
    aFlags = kAuthorizationFlagDefaults |
        kAuthorizationFlagInteractionAllowed |
        kAuthorizationFlagPreAuthorize |
        kAuthorizationFlagExtendRights;
    st = AuthorizationCopyRights(aRef, &aRights, &aEnv, aFlags, NULL );
    if (errAuthorizationSuccess == st) {
        char *executable = strdup(m_sSelfPath.utf8_str());
        char *args[] = { "--batch", NULL };
        st = AuthorizationExecuteWithPrivileges(aRef,
                executable, kAuthorizationFlagDefaults, args, NULL);
    }
    AuthorizationFree(aRef, kAuthorizationFlagDefaults);
}
