/////////////////////////////////////////////////////////////////////////////
// Name:        tracelogApp.cpp
// Purpose:     
// Author:      Fritz Elfert
// Modified by: 
// Created:     19/10/2008 02:49:40
// RCS-ID:      
// Copyright:   (C) 2008 by Fritz Elfert
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
#include "wx/fs_zip.h"
////@end includes

#include <wx/sysopt.h>
#include "wx/fs_mem.h"
#include <wx/taskbar.h>
#include <wx/dynarray.h>
#include <wx/mimetype.h>

#include "tracelogApp.h"
#include "Icon.h"
#include "TraceLogFrame.h"
#include "DebugStringGrabber.h"

#include "memres.h"

////@begin XPM images
////@end XPM images

static int TB_SHOWTRACE = ::wxNewId();

class DebugEntry {
    public:
        pid_t pid;
        wxDateTime stamp;
        wxString msg;
};

WX_DECLARE_OBJARRAY(DebugEntry, DebugEntryArray);

#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY(DebugEntryArray);

class MyTaskBarIcon : public wxTaskBarIcon {
    public:
        virtual wxMenu *CreatePopupMenu() {
            wxMenu *menu = new wxMenu;
            menu->Append(TB_SHOWTRACE, _("Show Trace Log"));
            menu->Append(wxID_EXIT, _("E&xit"));
            return menu;
        }

        void OnMenuShowTrace(wxCommandEvent &evt) {
            ::wxGetApp().GetTopWindow()->Show(true);
            evt.Skip();
        }

        void OnShowTrace(wxTaskBarIconEvent &evt) {
            ::wxGetApp().GetTopWindow()->Show(true);
            evt.Skip();
        }

        void OnMenuExit(wxCommandEvent &evt) {
            ::wxGetApp().Terminate();
            evt.Skip();
        }

        DECLARE_EVENT_TABLE();

    private:
        // wxDateTime startStamp;
        // DebugEntryArray m_aDebugEntries;
        // DebugStringGrabber *m_pGrabber;

};

BEGIN_EVENT_TABLE(MyTaskBarIcon, wxTaskBarIcon)
    EVT_MENU(TB_SHOWTRACE, MyTaskBarIcon::OnMenuShowTrace)
    EVT_MENU(wxID_EXIT, MyTaskBarIcon::OnMenuExit)
    EVT_TASKBAR_LEFT_DCLICK(MyTaskBarIcon::OnShowTrace)
END_EVENT_TABLE();

/*!
 * Application instance implementation
 */

////@begin implement app
IMPLEMENT_APP( tracelogApp )
////@end implement app


/*!
 * tracelogApp type definition
 */

IMPLEMENT_CLASS( tracelogApp, wxApp )


/*!
 * tracelogApp event table definition
 */

BEGIN_EVENT_TABLE(tracelogApp, wxApp)

////@begin tracelogApp event table entries
////@end tracelogApp event table entries

END_EVENT_TABLE()


/*!
 * Constructor for tracelogApp
 */

tracelogApp::tracelogApp()
{
    Init();
}


/*!
 * Member initialisation
 */

void tracelogApp::Init()
{
    m_pTaskBarIcon = NULL;
    m_pGrabber = NULL;
////@begin tracelogApp member initialisation
////@end tracelogApp member initialisation
}


void tracelogApp::OnDebugString(wxCommandEvent &event)
{
    m_pLogFrame->AddEntry(wxDateTime::UNow(), event.GetInt(), event.GetString());
    event.Skip();
}

void tracelogApp::Terminate()
{
    if (m_pTaskBarIcon)
        m_pTaskBarIcon->RemoveIcon();
    if (m_pGrabber)
        delete m_pGrabber;
    m_pGrabber = NULL;
    if (m_pLogFrame) {
        m_pLogFrame->Close(true);
        delete m_pLogFrame;
    }
    m_pLogFrame = NULL;
    ExitMainLoop();
}

/*!
 * Initialisation for tracelogApp
 */
bool tracelogApp::OnInit()
{    
    // Win: Don't remap bitmaps to system colors
    wxSystemOptions::SetOption(wxT("msw.remap"), 0);
    // WinXP: Don't draw themed gradients on notebook pages
    wxSystemOptions::SetOption(wxT("msw.notebook.themed-background"), 0);

    // Call to base class needed for initializing command line processing
    if (!wxApp::OnInit())
        return false;

    wxFileSystem::AddHandler(new wxZipFSHandler);
    wxFileSystem::AddHandler(new wxMemoryFSHandler);
    wxInitAllImageHandlers();
    wxBitmap::InitStandardHandlers();
    wxXmlResource::Get()->InitAllHandlers();

    const unsigned char *resptr = get_mem_res();
    wxMemoryFSHandler::AddFileWithMimeType(wxT("memrsc"), resptr, cnt_mem_res, wxT("application/zip"));
    {
        // The following code eliminates a stupid error dialog which shows up
        // if some .desktop entires (in KDE or GNOME applink dirs) are dangling symlinks.
        wxLogNull lognull;
        wxTheMimeTypesManager->GetFileTypeFromExtension(wxT("zip"));
    }

    free_mem_res(resptr);
    m_sResourcePrefix = wxT("memory:memrsc#zip:");
    if (!wxXmlResource::Get()->Load(m_sResourcePrefix + wxT("res/tracelog.xrc")))
        return false;

    m_pGrabber = new DebugStringGrabber();
    if (!m_pGrabber->IsOk())
        return false;
	m_pLogFrame = new TraceLogFrame(NULL);
    SetTopWindow(m_pLogFrame);
    Connect(wxEVT_DEBUGSTRING, wxCommandEventHandler(tracelogApp::OnDebugString));
    m_pGrabber->SetHandler(this);
    m_pTaskBarIcon = new MyTaskBarIcon();
    m_pTaskBarIcon->SetIcon(CreateIconFromFile(wxT("res/nx.png")), _("OpenNX"));
    SetExitOnFrameDelete(true);
    return true;
}


/*!
 * Cleanup for tracelogApp
 */

int tracelogApp::OnExit()
{
    if (m_pTaskBarIcon)
        m_pTaskBarIcon->RemoveIcon();
    if (m_pGrabber)
        delete m_pGrabber;
    if (m_pLogFrame)
        delete m_pLogFrame;
////@begin tracelogApp cleanup
	return wxApp::OnExit();
////@end tracelogApp cleanup
}

