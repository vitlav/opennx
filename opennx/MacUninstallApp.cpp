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
            ::wxMessageBox(_("Batch uninstall nedds to be started as root."),
                    _("Uninstall OpenNX"), wxOK|wxICON_ERROR);
            return false;
        }
        doUninstall();
    } else {
        int r = ::wxMessageBox(
                _("This operation can not be undone!\nDo you really want to uninstall OpenNX?"),
                _("Uninstall OpenNX"), wxYES_NO|wxICON_EXCLAMATION);
        if (wxYES == r)
            ElevatedUninstall(
                    _("In order to uninstall OpenNX, administrative rights are required.\n"));
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

void MacUninstallApp::doUninstall()
{
    ::wxMessageBox(_("Uninstalling as root"), _("Uninstall OpenNX"));
}

void MacUninstallApp::ElevatedUninstall(const wxString &msg)
{
    if (geteuid() == 0) {
        doUninstall();
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
