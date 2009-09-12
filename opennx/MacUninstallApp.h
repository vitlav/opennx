/////////////////////////////////////////////////////////////////////////////
// Name:        macuninstallapp.h
// Purpose:     
// Author:      Fritz Elfert
// Modified by: 
// Created:     Fri 11 Sep 2009 01:17:42 PM CEST
// RCS-ID:      
// Copyright:   (C) 2009 by Fritz Elfert
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _MACUNINSTALLAPP_H_
#define _MACUNINSTALLAPP_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/image.h"
////@end includes
#include <wx/intl.h>

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
////@end control identifiers

/*!
 * MacUninstallApp class declaration
 */

class wxArrayString;

class MacUninstallApp: public wxApp
{    
    DECLARE_CLASS( MacUninstallApp )
        DECLARE_EVENT_TABLE()

    public:
        /// Constructor
        MacUninstallApp();

        void Init();

        virtual void OnInitCmdLine(wxCmdLineParser& parser);
        virtual bool OnCmdLineParsed(wxCmdLineParser& parser);

        /// Initialises the application
        virtual bool OnInit();

        /// Called on exit
        virtual int OnExit();

        ////@begin MacUninstallApp event handler declarations
        ////@end MacUninstallApp event handler declarations

        ////@begin MacUninstallApp member function declarations
        ////@end MacUninstallApp member function declarations

    private:
        void ElevatedUninstall(const wxString &, const wxString &);
        void DoUninstall(const wxString &);
        bool TestReceipt(const wxString &);
        bool FetchBOM(bool, const wxString &, wxArrayString &, wxArrayString &);
        wxString GetInstalledPath(bool, const wxString &);

        ////@begin MacUninstallApp member variables
        ////@end MacUninstallApp member variables
        bool m_bBatchMode;
        wxLocale m_cLocale;
        wxString m_sSelfPath;
};

/*!
 * Application instance declaration 
 */

////@begin declare app
DECLARE_APP(MacUninstallApp)
    ////@end declare app

#endif
    // _MACUNINSTALLAPP_H_
