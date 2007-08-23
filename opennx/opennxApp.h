// $Id$
//
// Copyright (C) 2006 The OpenNX Team
// Author: Fritz Elfert
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Library General Public License as
// published by the Free Software Foundation; either version 2 of the
// License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this program; if not, write to the
// Free Software Foundation, Inc.,
// 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//

#ifndef opennxApp_H
#define opennxApp_H

#include <wx/intl.h>

class wxConfigBase;
class MyXmlConfig;

// Define a new application type, each program should derive a class from wxApp
class opennxApp : public wxApp
{
public:
    enum mode {
        MODE_CLIENT,
        MODE_ADMIN,
        MODE_WIZARD,
        MODE_DIALOG_OK,
        MODE_DIALOG_YESNO,
        MODE_DIALOG_ERROR,
        MODE_DIALOG_PANIC,
        MODE_DIALOG_QUIT,
        MODE_FOREIGN_TOOLBAR,
    };

    opennxApp();
    virtual ~opennxApp();
    wxLocale *GetLocale() { return &m_cLocale; }
    const wxString &GetResourcePrefix() { return m_sResourcePrefix; }
    const wxString &GetVersion() { return m_sVersion; }
    const wxString &GetSelfPath() { return m_sSelfPath; }
    wxString LoadFileFromResource(const wxString &loc, bool bUseLocale = true);
    bool CreateDesktopEntry(MyXmlConfig *);
    bool RemoveDesktopEntry(MyXmlConfig *);

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
    wxString m_sVersion;
    wxString m_sSelfPath;
    wxString m_sDialogCaption;
    wxString m_sDialogMessage;
    long m_nOtherPID;
    long m_nWindowID;
    int m_iDialogStyle;
    enum mode m_eMode;

    bool preInit();
};

DECLARE_APP(opennxApp)

#endif // opennxApp_H


