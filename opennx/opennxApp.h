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
class wxTaskBarIcon;
class MyXmlConfig;

// Define a new application type, each program should derive a class from wxApp
class opennxApp : public wxApp
{
    public:
        opennxApp();
        virtual ~opennxApp();

        wxLocale *GetLocale() { return &m_cLocale; }
        const wxString &GetResourcePrefix() { return m_sResourcePrefix; }
        const wxString &GetVersion() { return m_sVersion; }
        const wxString &GetSelfPath() { return m_sSelfPath; }
        wxString LoadFileFromResource(const wxString &loc, bool bUseLocale = true);
#ifdef __WXMAC__
        // Respond to Apple Event for opening a document
        virtual void MacOpenFile(const wxString& filename);
#endif
        bool CreateDesktopEntry(MyXmlConfig *);
        bool RemoveDesktopEntry(MyXmlConfig *);
        /**
         * Return status of SmartCard support
         * in nxssh. If nxssh has SmartCard support
         * compiled in, return true, otherwise return false.
         */
        bool NxSmartCardSupport() { return m_bNxSmartCardSupport; }
        bool LibUSBAvailable() { return m_bLibUSBAvailable; }

        void SetRequireWatchReader(bool b) { m_bRequireWatchReader = b; }
        void SetRequireStartUsbIp(bool b) { m_bRequireStartUsbIp = b; }
        void SetNxSshPID(long l) { m_nNxSshPID = l; }
        void SetReader(int i) { m_iReader = i; }
        void SetSessionCfg(MyXmlConfig &);
        void SetSessionID(const wxString &s) { m_sSessionID = s; };

        virtual bool OnInit();
        virtual void OnInitCmdLine(wxCmdLineParser& parser);
        virtual bool OnCmdLineParsed(wxCmdLineParser& parser);
        virtual int OnExit();
        int FilterEvent(wxEvent& event);

    private:
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

        wxConfigBase *m_pCfg;
        MyXmlConfig *m_pSessionCfg;
        wxLocale m_cLocale;
        wxString m_sSessionName;
        wxString m_sSessionID;
        wxString m_sResourcePrefix;
        wxString m_sVersion;
        wxString m_sSelfPath;
        wxString m_sDialogCaption;
        wxString m_sDialogMessage;
        long m_nNxSshPID;
        long m_nOtherPID;
        long m_nWindowID;
        int m_iDialogStyle;
        int m_iReader;
        enum mode m_eMode;
        bool m_bNxSmartCardSupport;
        bool m_bRunproc;
        bool m_bLibUSBAvailable;
        bool m_bRequireWatchReader;
        bool m_bRequireStartUsbIp;

        bool preInit();
        bool realInit();
        bool setSelfPath();
        void setUserDir();
        void checkNxSmartCardSupport();
        void checkLibUSB();
        wxString findExecutable(wxString name);
};

DECLARE_APP(opennxApp)

#endif // opennxApp_H


