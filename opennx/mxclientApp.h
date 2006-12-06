/////////////////////////////////////////////////////////////////////////////
// Name:        mxclientApp.h
// Purpose:     NoMachine Client reloaded
// Author:      Fritz Elfert
// Modified by: 
// Created:     
// RCS-ID:      $Id$
// Copyright:   Copyright 2004 Millenux GmbH
// Licence:     LGPL
/////////////////////////////////////////////////////////////////////////////

#ifndef mxclientApp_H
#define mxclientApp_H

#include <wx/intl.h>

class wxConfigBase;

// Define a new application type, each program should derive a class from wxApp
class mxclientApp : public wxApp
{
public:
    enum mode {
        MODE_CLIENT,
        MODE_ADMIN,
        MODE_WIZARD,
    };

    mxclientApp();
    virtual ~mxclientApp();
    wxLocale *getLocale() { return &m_cLocale; }
    const wxString &getResourcePrefix() { return m_sResourcePrefix; }

    // override base class virtuals
    // ----------------------------

    // this one is called on application startup and is a good place for the app
    // initialization (doing it here and not in the ctor allows to have an error
    // return: if OnInit() returns false, the application terminates)
    virtual bool OnInit();
    virtual void OnInitCmdLine(wxCmdLineParser& parser);
    virtual bool OnCmdLineParsed(wxCmdLineParser& parser);
    virtual int OnExit();

private:
    wxConfigBase *m_pCfg;
    wxLocale m_cLocale;
    wxString m_sSessionName;
    wxString m_sResourcePrefix;
    enum mode m_eMode;
    const unsigned char *m_szMemRes;
};

DECLARE_APP(mxclientApp)

#endif // mxclientApp_H


