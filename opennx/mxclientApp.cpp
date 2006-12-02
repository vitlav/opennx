/////////////////////////////////////////////////////////////////////////////
// $Id: mxclientApp.cpp 1090 2005-05-09 20:31:27Z felfert $
//
// Author:      Fritz Elfert
// Created:
// Copyright:   Copyright 2004 Millenux GmbH
// Licence:     LGPL
/////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
	#include "wx/wx.h"
#endif

#ifdef __UNIX__
#include <unistd.h>
#endif
#include <wx/cmdline.h>
#include <wx/xrc/xmlres.h>
#include <wx/image.h>
#include <wx/config.h>
#include <wx/fs_zip.h>
#include "wx/fs_mem.h"
#include <wx/sysopt.h>

#include "resource.h"
#include "mxclientApp.h"
#include "SessionAdmin.h"
#include "SessionProperties.h"
#include "LoginDialog.h"
#include "MxWizard.h"
#include "MxXmlConfig.h"

#include "memres.h"


// Create a new application object: this macro will allow wxWindows to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also declares the accessor function
// wxGetApp() which will return the reference of the right type (i.e. mxclientApp and
// not wxApp)
IMPLEMENT_APP(mxclientApp)

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------

mxclientApp::mxclientApp()
    : m_pCfg(NULL)
    , m_szMemRes(NULL)
{
    
#ifdef __WXDEBUG__
#ifdef __WXMSW__
    m_pCfg = new wxConfig(_T("MxClient-Debug"), _T("Millenux"));
#else
    m_pCfg = new wxConfig(_T("MxClient-Debug"), _T("Millenux"), _T(".mxclient-debug"));
#endif
#else
#ifdef __WXMSW__
    m_pCfg = new wxConfig(_T("MxClient"), _T("Millenux"));
#else
    m_pCfg = new wxConfig(_T("MxClient"), _T("Millenux"), _T(".mxclient"));
#endif
#endif
    wxConfigBase::Set(m_pCfg);

    wxString tmp;
    if (!wxConfigBase::Get()->Read(_T("Config/UserMxDir"), &tmp)) {
        tmp = ::wxGetHomeDir() + wxFileName::GetPathSeparator() + _T(".nx");
        if (!wxFileName(tmp).DirExists())
            wxFileName(tmp).Mkdir();
        wxConfigBase::Get()->Write(_T("Config/UserMxDir"), tmp);
    }
    if (!wxConfigBase::Get()->Read(_T("Config/SystemMxDir"), &tmp)) {
        tmp = _T("/usr/NX");
#ifdef __WXMSW__
        int ret = GetModuleFileName(NULL, tmp.GetWriteBuf(1024), 1024);
        tmp.UngetWriteBuf(ret);
        tmp = wxFileName(tmp).GetPath(wxPATH_GET_VOLUME);
#endif
#ifdef __LINUX__
        // Get executable path from /proc/self/exe
	char ldst[PATH_MAX];
        int ret = readlink("/proc/self/exe", ldst, PATH_MAX);
	tmp = wxConvLocal.cMB2WX(ldst);
        if (ret == -1)
            ret = 0;
        if (ret == 0)
            tmp = _T("/usr/NX");
        else
            tmp = wxFileName(tmp).GetPath();
#endif
        wxConfigBase::Get()->Write(_T("Config/SystemMxDir"), tmp);
    }
    wxConfigBase::Get()->Flush();
    wxConfigBase::Get()->Read(_T("Config/SystemMxDir"), &tmp);
    // Change to our system dir so we can read our resources
    wxSetWorkingDirectory(tmp);
}

mxclientApp::~mxclientApp()
{
    if (m_pCfg)
        delete m_pCfg;
}

void mxclientApp::OnInitCmdLine(wxCmdLineParser& parser)
{
    // Init standard options (--help, --verbose);
    wxApp::OnInitCmdLine(parser);

    parser.AddSwitch(_T(""), wxT("admin"),
        _("Start the session administration tool."));
    parser.AddOption(_T(""), wxT("session"),
        _("Run a session importing configuration settings from FILENAME."));
    parser.AddSwitch(_T(""), wxT("wizard"),
        _("Guide the user through the steps to configure a session."));
}

bool mxclientApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
    if (!wxApp::OnCmdLineParsed(parser))
        return false;

    m_eMode = MODE_CLIENT;
    if (parser.Found(_T("admin")))
        m_eMode = MODE_ADMIN;
    if (parser.Found(_T("wizard")))
        m_eMode = MODE_WIZARD;
    (void)parser.Found(_T("session"), &m_sSessionName);

    return true;
}

// 'Main program' equivalent: the program execution "starts" here
bool mxclientApp::OnInit()
{
    m_cLocale.AddCatalogLookupPathPrefix(wxT("locale"));
    m_cLocale.Init();
    m_cLocale.AddCatalog(wxT("mxclient"));

    // Don't remap bitmaps to system colors
    wxSystemOptions::SetOption(_T("msw.remap"), 0);

    // Call to base class needed for initializing command line processing
    if (!wxApp::OnInit())
        return false;

    wxFileSystem::AddHandler(new wxZipFSHandler);
    wxFileSystem::AddHandler(new wxMemoryFSHandler);
    wxInitAllImageHandlers();
    wxBitmap::InitStandardHandlers();
    wxXmlResource::Get()->InitAllHandlers();

    m_szMemRes = get_mem_res();
    wxMemoryFSHandler::AddFile(wxT("memrsc"), m_szMemRes, cnt_mem_res);
    m_sResourcePrefix = wxT("memory:memrsc#zip:");
    if (!wxXmlResource::Get()->Load(m_sResourcePrefix + wxT("res/mxclient.xrc"))) {
        if (m_szMemRes) {
            free_mem_res(m_szMemRes);
            m_szMemRes = NULL;
        }
        return false;
    }

    if (m_eMode == MODE_ADMIN) {
        SessionAdmin *sa = new SessionAdmin(NULL);
        // If we return true, the global config will
        // be deleted by the framework, so we set it to NULL here
        // to prevent double free.
        m_pCfg = NULL;
        sa->Show();
        SetTopWindow(sa);
        return true;
    }

    if ((m_eMode == MODE_CLIENT) && m_sSessionName.IsEmpty()) {
        if (!wxConfigBase::Get()->Read(_T("Config/LastSession"), &m_sSessionName))
            m_eMode = MODE_WIZARD;
    } else {
        if (!m_sSessionName.IsEmpty()) {
            MxXmlConfig cfg(m_sSessionName);
            if (cfg.IsValid())
                m_sSessionName = cfg.sGetName();
        }
    }

    if (m_eMode == MODE_WIZARD) {
        MxWizard wz(NULL);
        if (!wz.Run()) {
            if (m_szMemRes) {
                free_mem_res(m_szMemRes);
                m_szMemRes = NULL;
            }
            return false;
        }
        m_sSessionName = wz.sGetConfigName();
    }

    LoginDialog d;
    d.SetSSessionName(m_sSessionName);
    d.Create(NULL);
    if (d.ShowModal() == wxID_OK) {
        m_sSessionName = d.GetSSessionName();
        if (!m_sSessionName.IsEmpty())
            wxConfigBase::Get()->Write(_T("Config/LastSession"), m_sSessionName);
    }

    // success: wxApp::OnRun() will be called which will enter the main message
    // loop and the application will run. We returne FALSE here, so that the
    // application exits if the dialog is destroyed.
    return false;
}

/*!
 * Cleanup for GastroCardApp
 */
int mxclientApp::OnExit()
{
    if (m_szMemRes)
        free_mem_res(m_szMemRes);
    m_szMemRes = NULL;
    return wxApp::OnExit();
}
