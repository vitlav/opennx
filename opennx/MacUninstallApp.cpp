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
//#include <wx/wfstream.h>
//#include <wx/txtstrm.h>
#include <Security/Authorization.h>
#include <Security/AuthorizationTags.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "macuninstallapp.h"

////@begin XPM images

////@end XPM images


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
    m_bBatchMode = false;
    m_sSelfPath = wxFileName(
            GetTraits()->GetStandardPaths().GetExecutablePath()).GetFullPath();
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
        if (0 != geteuid()) {
            ::wxMessageBox(_("Batch uninstall needs to be started as root."),
                    _("Uninstall OpenNX"), wxOK|wxICON_ERROR);
            return false;
        }
        DoUninstall(wxT("OpenNX"));
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
                    _("In order to uninstall OpenNX, administrative rights are required.\n"));
        }
    }
    return false;
}


/*
 * Cleanup for MacUninstallApp
 */

int MacUninstallApp::OnExit()
{    
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
    wxString rpath = wxT("/Library/Receipts/");
    rpath.Append(pkg).Append(wxT(".pkg"));
    wxString proot = GetInstalledPath(false,
            rpath + wxT("/Contents/Info.plist"));
    if (proot.IsEmpty())
        return;
    wxArrayString d;
    wxArrayString f;
    if (!FetchBOM(false, rpath + wxT("/Contents/Archive.bom"), d, f))
        return;
    size_t i;
    for (i = 0; i < f.GetCount(); i++) {
        wxFileName fn(proot + f[i]);
        if (fn.Normalize(wxPATH_NORM_DOTS|wxPATH_NORM_ABSOLUTE)) {
#if 0
            if (0 == unlink((const char *)fn.GetFullPath().mb_str()))
                f.RemoveAt(i);
#else
            fprintf(stderr, "rm %s\n",
                    (const char *)fn.GetFullPath().mb_str());
            fflush(stderr);
#endif
        }
    }
    size_t lcd;
    do {
        lcd = d.GetCount();
        for (i = 0; i < d.GetCount(); i++) {
            wxFileName fn(proot + d[i]);
            if (fn.Normalize(wxPATH_NORM_DOTS|wxPATH_NORM_ABSOLUTE)) {
#if 0
                if (0 == rmdir(fn.GetFullPath().mb_str()))
                    d.RemoveAt(i--);
#else
                fprintf(stderr, "rmdir %s\n",
                        (const char *)fn.GetFullPath().mb_str());
                fflush(stderr);
#endif
            }
        }
    } while (lcd != d.GetCount());
}

void MacUninstallApp::ElevatedUninstall(const wxString &pkg,
        const wxString &msg)
{
    if (geteuid() == 0) {
        DoUninstall(pkg);
        return;
    }
    char *prompt = strdup(msg.utf8_str());

    OSStatus myStatus;
    AuthorizationFlags myFlags = kAuthorizationFlagDefaults;
    AuthorizationRef myAuthorizationRef;
    AuthorizationItem promptItem = {
        kAuthorizationEnvironmentPrompt, strlen(prompt), prompt, 0
    };
    AuthorizationEnvironment aEnv = { 1, &promptItem };

    myStatus = AuthorizationCreate(NULL, kAuthorizationEmptyEnvironment,
            myFlags, &myAuthorizationRef);
    if (errAuthorizationSuccess != myStatus)
        return;
    AuthorizationItem myItems = { kAuthorizationRightExecute, 0, NULL, 0 };
    AuthorizationRights myRights = { 1, &myItems };
    myFlags = kAuthorizationFlagDefaults |
        kAuthorizationFlagInteractionAllowed |
        kAuthorizationFlagPreAuthorize |
        kAuthorizationFlagExtendRights;
    myStatus = AuthorizationCopyRights(myAuthorizationRef, &myRights, &aEnv, myFlags, NULL );
    if (errAuthorizationSuccess == myStatus) {
        char *myToolPath = strdup(m_sSelfPath.utf8_str());
        char *myArguments[] = { "--batch", NULL };
        FILE *myCommunicationsPipe = NULL;
        char myReadBuffer[128];

        myFlags = kAuthorizationFlagDefaults;
        myStatus = AuthorizationExecuteWithPrivileges(myAuthorizationRef,
                myToolPath, myFlags, myArguments, &myCommunicationsPipe);
        if (errAuthorizationSuccess == myStatus) {
            for(;;) {
                int bytesRead = read(fileno(myCommunicationsPipe),
                        myReadBuffer, sizeof(myReadBuffer));
                if (bytesRead < 1)
                    break;
            }
        }
    }
    AuthorizationFree(myAuthorizationRef, kAuthorizationFlagDefaults);
    if (myStatus)
        printf("Status: %ld\n", myStatus);
}
