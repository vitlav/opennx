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

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
#ifdef __WXMSW__
#define _WIN32_IE 0x0400
#include <shlobj.h>
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
#include <wx/tokenzr.h>
#include <wx/wfstream.h>
#include <wx/mimetype.h>
#include <wx/utils.h>
#include <wx/stdpaths.h>
#include <wx/apptrait.h>
#include <wx/socket.h>

#include "opennxApp.h"
#include "SessionAdmin.h"
#include "SessionProperties.h"
#include "LoginDialog.h"
#include "LibOpenSC.h"
#include "MyWizard.h"
#include "MyIPC.h"
#include "MyXmlConfig.h"
#include "PanicDialog.h"
#include "QuitDialog.h"
#include "ForeignFrame.h"
#include "Icon.h"
#include "LibUSB.h"
#include "LibOpenSC.h"
#include "osdep.h"
#include "xh_richtext.h"
#include "UsbIp.h"

#include "memres.h"

#include "trace.h"
ENABLE_TRACE;
DECLARE_TRACETAGS;

// Create a new application object: this macro will allow wxWindows to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also declares the accessor function
// wxGetApp() which will return the reference of the right type (i.e. opennxApp and
// not wxApp)

IMPLEMENT_APP(opennxApp);

    opennxApp::opennxApp()
    : wxApp()
    ,m_pCfg(NULL)
    ,m_pSessionCfg(NULL)
    ,m_nNxSshPID(-1)
    ,m_iReader(-1)
    ,m_bNxSmartCardSupport(false)
    ,m_bRunproc(false)
    ,m_bLibUSBAvailable(false)
    ,m_bRequireWatchReader(false)
      ,m_bRequireStartUsbIp(false)
{
    SetAppName(wxT("OpenNX"));
#ifdef __WXMSW__
    m_pCfg = new wxConfig(wxT("OpenNX"), wxT("InnoviData"));
#else
    m_pCfg = new wxConfig(wxT("OpenNX"), wxT("InnoviData"), wxT(".opennx"), wxT("opennx.conf"));
#endif
    wxConfigBase::Set(m_pCfg);

#ifdef __WXMSW__
    DWORD dummy;
    DWORD viSize;
    LPVOID vi;
    TCHAR mySelf[MAX_PATH];
    VS_FIXEDFILEINFO *vsFFI;
    UINT vsFFIlen;
    m_sVersion = wxT("?.?");

    if (GetModuleFileName(NULL, mySelf, sizeof(mySelf))) {
        viSize = GetFileVersionInfoSize(mySelf, &dummy);
        if (viSize) {
            vi = (LPVOID)malloc(viSize);
            if (vi) {
                if (GetFileVersionInfo(mySelf, dummy, viSize, vi)) {
                    if (VerQueryValue(vi, wxT("\\"), (LPVOID *)&vsFFI, &vsFFIlen)) {
                        m_sVersion = wxString::Format(wxT("%d.%d"), HIWORD(vsFFI->dwFileVersionMS),
                                LOWORD(vsFFI->dwFileVersionMS));
                        if (vsFFI->dwFileVersionLS)
                            m_sVersion += wxString::Format(wxT(".%d"), HIWORD(vsFFI->dwFileVersionLS));
                        if (LOWORD(vsFFI->dwFileVersionLS))
                            m_sVersion += wxString::Format(wxT(".%d"), LOWORD(vsFFI->dwFileVersionLS));
                    }

                }
                free(vi);
            }
        }
    }
#else
    m_sVersion = wxT(PACKAGE_VERSION);
    while (m_sVersion.Freq(wxT('.')) < 2)
        m_sVersion.Append(wxT(".0"));
    m_sVersion.Append(wxT(".")).Append(wxT(SVNREV));
    {
        wxLogNull dummy;
        // Try to get KDE language settings and override locale accordingly
        wxFileInputStream fis(::wxGetHomeDir() +
                wxFileName::GetPathSeparator() + wxT(".kde") + 
                wxFileName::GetPathSeparator() + wxT("share") + 
                wxFileName::GetPathSeparator() + wxT("config") + 
                wxFileName::GetPathSeparator() + wxT("kdeglobals"));
        if (fis.IsOk()) {
            wxFileConfig cfg(fis);
            wxString country = cfg.Read(wxT("Locale/Country"), wxEmptyString);
            wxString lang = cfg.Read(wxT("Locale/Language"), wxEmptyString);
            if ((!lang.IsEmpty()) && (!country.IsEmpty()))
                ::wxSetEnv(wxT("LANG"), lang + wxT("_") + country.Upper() + wxT(".UTF-8"));
        }
    }
#endif
}

opennxApp::~opennxApp()
{
    if (m_pCfg)
        delete m_pCfg;
    if (m_pSessionCfg)
        delete m_pSessionCfg;
}

    wxString
opennxApp::LoadFileFromResource(const wxString &loc, bool bUseLocale /* = true */)
{
    bool tryloop = true;
    wxString ret;
    wxString cloc = bUseLocale ? m_cLocale.GetCanonicalName() : wxT("");
    wxFileSystem fs;
    wxFSFile *f;

    do {
        wxString tryloc = loc;

        if (!cloc.IsEmpty()) {
            tryloc = wxFileName(loc).GetPath(wxPATH_GET_SEPARATOR|wxPATH_GET_VOLUME)
                + cloc + wxT("_") + wxFileName(loc).GetFullName();
            cloc = cloc.BeforeLast(_T('_'));
        } else
            tryloop = false;

        // try plain loc first
        f = fs.OpenFile(tryloc);

        if (!f)
            f = fs.OpenFile(GetResourcePrefix() + tryloc);

        if (f) {
            wxInputStream *is = f->GetStream();
            size_t size = is->GetSize();
            char *buf = new char[size+2];
            is->Read(buf, size);
            delete f;
            buf[size] = buf[size+1] = 0;
            ret = wxConvLocal.cMB2WX(buf);
            delete []buf;
        }
    } while (ret.IsEmpty() && tryloop);
    return ret;
}

static const wxChar *desktopDirs[] = {
    wxT("Desktop"), wxT("KDesktop"), wxT(".gnome-desktop"), NULL
};

    bool
opennxApp::CreateDesktopEntry(MyXmlConfig *cfg)
{
    bool ret = false;

    wxString appDir;
    wxConfigBase::Get()->Read(wxT("Config/SystemNxDir"), &appDir);
#ifdef __WXMSW__
    TCHAR dtPath[MAX_PATH];
    if (SHGetSpecialFolderPath(NULL, dtPath, CSIDL_DESKTOPDIRECTORY, FALSE)) {
        wxString linkPath = wxString::Format(wxT("%s\\%s.lnk"), dtPath, cfg->sGetName().c_str());
        wxString targetPath = GetSelfPath();
        wxString desc = _("Launch NX Session");
        wxString args = wxString::Format(wxT("--session=\"%s\""), cfg->sGetFileName().c_str());
        HRESULT hres;
        IShellLink* psl;

        hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER,
                IID_IShellLink, (LPVOID *) &psl);
        if (SUCCEEDED(hres)) {
            IPersistFile* ppf;
            psl->SetPath(targetPath.c_str());
            psl->SetWorkingDirectory(appDir.c_str());
            psl->SetDescription(desc.c_str());
            psl->SetArguments(args.c_str());
            psl->SetIconLocation(targetPath, 1);
            hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf);
            if (SUCCEEDED(hres)) {
                hres = ppf->Save(wxConvLocal.cWX2WC(linkPath), TRUE);
                ppf->Release();
                ret = true;
            }
            psl->Release();
        }
    }
#endif
#ifdef __UNIX__
# ifdef __WXMAC__
    wxString path = wxGetHomeDir() + wxFileName::GetPathSeparator() + wxT("Desktop") +
        wxFileName::GetPathSeparator() + wxT(".") + cfg->sGetName() +
        wxFileName::GetPathSeparator() + wxT("Contents");
    wxFileName::Mkdir(path, 0755, wxPATH_MKDIR_FULL);
    wxFileName::Mkdir(path + wxFileName::GetPathSeparator() + wxT("Resources"), 0755, wxPATH_MKDIR_FULL);
    wxFileName::Mkdir(path + wxFileName::GetPathSeparator() + wxT("MacOS"), 0755, wxPATH_MKDIR_FULL);
    wxString plist = wxT("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    plist += wxT("<!DOCTYPE plist SYSTEM \"file://localhost/System/Library/DTDs/PropertyList.dtd\">\n");
    plist += wxT("<plist version=\"0.9\">\n");
    plist += wxT("<dict>\n");
    plist += wxT(" <key>CFBundleInfoDictionaryVersion</key>\n");
    plist += wxT(" <string>6.0</string>\n");
    plist += wxT(" <key>CFBundleIdentifier</key>\n");
    plist += wxString::Format(wxT(" <string>org.opennx.%s.%s</string>\n"),
            wxGetUserId().c_str(), cfg->sGetName().c_str());
    plist += wxT(" <key>CFBundleDevelopmentRegion</key>\n");
    plist += wxT(" <string>English</string>\n");
    plist += wxT(" <key>CFBundleExecutable</key>\n");
    plist += wxT(" <string>startnxsession</string>\n");
    plist += wxT(" <key>CFBundleIconFile</key>\n");
    plist += wxT(" <string>nxdesktop</string>\n");
    plist += wxT(" <key>CFBundleName</key>\n");
    plist += wxT(" <string>") + cfg->sGetName() + wxT("</string>\n");
    plist += wxT(" <key>CFBundlePackageType</key>\n");
    plist += wxT(" <string>APPL</string>\n");
    plist += wxT(" <key>CFBundleSignature</key>\n");
    plist += wxT(" <string>\?\?\?\?</string>\n");
    plist += wxT(" <key>CFBundleVersion</key>\n");
    plist += wxT(" <string>1.0</string>\n");
    plist += wxT(" <key>CFBundleShortVersionString</key>\n");
    plist += wxT(" <string>1.0</string>\n");
    plist += wxT(" <key>CFBundleGetInfoString</key>\n");
    plist += wxT(" <string>Starts an OpenNX session</string>\n");
    plist += wxT(" <key>LSRequiresCarbon</key>\n");
    plist += wxT(" <true/>\n");
    plist += wxT(" <key>CSResourcesFileMapped</key>\n");
    plist += wxT(" <true/>\n");
    plist += wxT("</dict>\n");
    plist += wxT("</plist>\n");
    if (::wxDirExists(path)) {

        wxFile f;
        wxFileSystem fs;
        wxString fn = path + wxFileName::GetPathSeparator() + wxT("Resources") +
            wxFileName::GetPathSeparator() + wxT("nxdesktop.icns");
        wxFSFile *ff = fs.OpenFile(::wxGetApp().GetResourcePrefix() + wxT("res/nx-desktop.icns"));

        // Write icon file
        ::wxLogTrace(MYTRACETAG, wxT("Creating '%s'"), fn.c_str());
        if (ff  && f.Create(fn, true, wxS_IRUSR|wxS_IWUSR|wxS_IRGRP|wxS_IROTH)) {
            wxInputStream *is = ff->GetStream();
            size_t len = is->GetSize();
            unsigned char *buf = new unsigned char[len];
            is->Read(buf, len);
            f.Write(buf, len);
            f.Close();
            delete buf;
        }

        // Write shell script
        fn = path + wxFileName::GetPathSeparator() + wxT("MacOS") +
            wxFileName::GetPathSeparator() + wxT("startnxsession");
        ::wxLogTrace(MYTRACETAG, wxT("Creating '%s'"), fn.c_str());
        if (f.Create(fn, true, wxS_IRUSR|wxS_IWUSR|wxS_IXUSR|wxS_IRGRP|wxS_IXGRP|wxS_IROTH)) {
            wxString cmd = wxT("#!/bin/sh\nexec \"") + GetSelfPath() + wxT("\" --session=\"") + cfg->sGetFileName() + wxT("\"\n");
            f.Write(cmd);
            f.Close();
        }

        fn = path + wxFileName::GetPathSeparator() + wxT("Info.plist");
        ::wxLogTrace(MYTRACETAG, wxT("Creating '%s'"), fn.c_str());
        if (f.Create(fn, true, wxS_IRUSR|wxS_IWUSR|wxS_IRGRP|wxS_IROTH)) {
            f.Write(plist);
            f.Close();
        }

        fn = path + wxFileName::GetPathSeparator() + wxT("PkgInfo");
        ::wxLogTrace(MYTRACETAG, wxT("Creating '%s'"), fn.c_str());
        if (f.Create(fn, true, wxS_IRUSR|wxS_IWUSR|wxS_IRGRP|wxS_IROTH)) {
            wxString pkginfo = wxT("APPL????");
            f.Write(pkginfo);
            f.Close();
        }

        wxFileName dir(path);
        fn = wxGetHomeDir() + wxFileName::GetPathSeparator() + wxT("Desktop") +
            wxFileName::GetPathSeparator() + cfg->sGetName() + wxT(".app");
        ::wxRenameFile(dir.GetPath(wxPATH_GET_VOLUME), fn, false);

    }
# else
    wxString dtEntry = wxT("[Desktop Entry]\n");
    dtEntry += wxT("Encoding=UTF-8\n");
    dtEntry += wxT("Type=Application\n");
    dtEntry += wxT("MimeType=\n");
    dtEntry += wxT("StartupNotify=true\n");
    dtEntry += wxT("Terminal=false\n");
    dtEntry += wxT("TerminalOptions=\n");
    dtEntry += wxT("Comment=Launch NX Session\n");
    dtEntry += wxT("Comment[de]=Starte NX Sitzung\n");
    dtEntry += wxT("Name=") + cfg->sGetName() + wxT("\n");
    dtEntry += wxT("GenericName=OpenNX Client\n");
    dtEntry += wxT("GenericName[de]=OpenNX Client\n");
    dtEntry += wxT("Exec=\"") + GetSelfPath() + wxT("\" --session=\"")
        + cfg->sGetFileName() + wxT("\"\n");
    dtEntry += wxT("Icon=") + appDir + wxT("/share/icons/nx-desktop.png\n");

    const wxChar **p = desktopDirs;
    while (*p) {
        wxString path = wxGetHomeDir() + wxT("/") + *p;
        if (::wxDirExists(path)) {
            wxFile f;
            wxString fn = path + wxT("/") + cfg->sGetName() + wxT(".desktop");
            ::wxLogTrace(MYTRACETAG, wxT("Creating '%s'"), fn.c_str());
            if (f.Create(fn, true, wxS_IRUSR|wxS_IWUSR|wxS_IRGRP|wxS_IROTH)) {
                f.Write(dtEntry);
                f.Close();
                ret = true;
            }
        }
        p++;
    }
# endif // !__WXMAC__
#endif // __UNIX__
    return ret;
}

    bool
opennxApp::RemoveDesktopEntry(MyXmlConfig *cfg)
{
#ifdef __WXMSW__
    TCHAR dtPath[MAX_PATH];
    if (SHGetSpecialFolderPath(NULL, dtPath, CSIDL_DESKTOPDIRECTORY, FALSE)) {
        wxString lpath = wxString::Format(_T("%s\\%s.lnk"), dtPath, wx_static_cast(const char *,cfg->sGetName().mb_str()));
        ::wxLogTrace(MYTRACETAG, wxT("Removing '%s'"), lpath.c_str());
        ::wxRemoveFile(lpath);
    }
#endif
#ifdef __UNIX__
    const wxChar **p = desktopDirs;

    while (*p) {
        ::wxRemoveFile(wxString::Format(_T("%s/%s/%s.desktop"),
                    ::wxGetHomeDir().c_str(), *p,cfg->sGetName().c_str()));
        p++;
    }
#endif
    ::wxLogTrace(MYTRACETAG, wxT("Removing '%s'"), cfg->sGetFileName().c_str());
    ::wxRemoveFile(cfg->sGetFileName());
    return true;
}

    void
opennxApp::setUserDir()
{
    wxString tmp;
    if (!wxConfigBase::Get()->Read(wxT("Config/UserNxDir"), &tmp))
        tmp = ::wxGetHomeDir() + wxFileName::GetPathSeparator() + wxT(".nx");
    wxFileName::Mkdir(tmp, 0750, wxPATH_MKDIR_FULL);
    wxFileName fn(tmp);
    wxConfigBase::Get()->Write(wxT("Config/UserNxDir"), fn.GetShortPath());
    wxFileName::Mkdir(tmp +  wxFileName::GetPathSeparator() + wxT("config"), 0750,
            wxPATH_MKDIR_FULL);
}

    bool
opennxApp::setSelfPath()
{
    wxString tmp;
    wxFileName fn;
#define NotImplemented
#if defined(__WXMSW__) || defined(__WXMAC__) || defined(__LINUX__)
    // On these platforms, wxAppTraits::GetExecutablePath() returns
    // the *actual* path of the running executable, regardless of
    // where it is installed.
    fn.Assign(GetTraits()->GetStandardPaths().GetExecutablePath());
#undef NotImplemented
#endif
#if defined(__OPENBSD__) 
    // FIXME: How to get one's own exe path on OpenBSD?
    // for now, we resemble sh's actions
    tmp = this->argv[0];
    if (!::wxIsAbsolutePath(tmp)) {
        if (tmp.StartsWith(wxT("."))) {
            // a relative path
            fn.Assign(tmp);
            fn.MakeAbsolute();
            tmp = fn.GetFullPath();
        } else {
            bool found = false;
            ::wxGetEnv(wxT("PATH"), &tmp);
            if (tmp.IsEmpty()) {
                wxLogError(_("Could not get PATH environment"));
                return false;
            }
            wxStringTokenizer st(tmp, wxT(":"));
            while (st.HasMoreTokens()) {
                tmp = st.GetNextToken();
                fn.Assign(tmp, argv[0]);
                if (fn.IsFileExecutable()) {
                    tmp = fn.GetFullPath();
                    found = true;
                    break;
                }
            }
            if (!found) {
                tmp = argv[0];
                wxLogError(_("Could not find %s in PATH"), tmp.c_str());
                return false;
            }
        }
    }
    int ret;
    char ldst[PATH_MAX+1];
    while (true) {
        struct stat st;
        if (lstat(tmp.fn_str(), &st) != 0) {
            wxLogSysError(_("Could not stat %s"), tmp.c_str());
            return false;
        }
        if (S_ISLNK(st.st_mode)) {
            ret = readlink(tmp.fn_str(), ldst, PATH_MAX);
            if (ret == -1) {
                wxLogSysError(_("Could not read link %s"), tmp.c_str());
                return false;
            }
            ldst[ret] = '\0';
            if (ldst[0] == '/')
                tmp = wxConvLocal.cMB2WX(ldst);
            else {
                fn.Assign(tmp);
                tmp = fn.GetPathWithSep() + wxConvLocal.cMB2WX(ldst);
            }
        } else {
            fn.Assign(tmp);
            fn.MakeAbsolute();
            break;
        }
    }
# undef NotImplemented
#endif
#ifdef NotImplemented
# error Missing Implementation for this OS
#endif
    m_sSelfPath = fn.GetFullPath();
    return true;
}

    bool
opennxApp::preInit()
{
    initWxTraceTags();
    setUserDir();
    if (!setSelfPath())
        return false;

    wxString tmp;
    if (!wxConfigBase::Get()->Read(wxT("Config/SystemNxDir"), &tmp)) {
        wxFileName fn(GetSelfPath());
        if (fn.GetDirs().Last().IsSameAs(wxT("bin")))
            fn.RemoveLastDir();
        fn.SetFullName(wxEmptyString);
        tmp = fn.GetShortPath();
        wxString rest;
        wxString sep = wxFileName::GetPathSeparator();
        if (tmp.EndsWith(sep, &rest))
            tmp = rest;
        wxConfigBase::Get()->Write(wxT("Config/SystemNxDir"), tmp);
        wxConfigBase::Get()->Flush();
    }
#ifdef SUPPORT_USBIP
    if (!wxConfigBase::Get()->Read(wxT("Config/UsbipdSocket"), &tmp)) {
        tmp = wxT("/var/run/usbipd2.socket");
        wxConfigBase::Get()->Write(wxT("Config/UsbipdSocket"), tmp);
        wxConfigBase::Get()->Flush();
    }
    if (!wxConfigBase::Get()->Read(wxT("Config/UsbipPort"), &tmp)) {
        wxConfigBase::Get()->Write(wxT("Config/UsbipPort"), 3420);
        wxConfigBase::Get()->Flush();
    }
#endif

    wxConfigBase::Get()->Read(wxT("Config/SystemNxDir"), &tmp);
#ifdef __WXMSW__
    wxString ldpath;
    if (::wxGetEnv(wxT("PATH"), &ldpath))
        ldpath += wxT(";");
    ldpath = tmp + wxT("\\bin");
    if (!::wxSetEnv(wxT("PATH"), ldpath)) {
        ::wxLogSysError(wxT("Can not set PATH"));
        return false;
    }
#endif

#ifdef __UNIX__
    wxString ldpath;
    if (::wxGetEnv(wxT("LD_LIBRARY_PATH"), &ldpath))
        ldpath += wxT(":");
#if defined(__x86_64) || defined(__IA64__)
    ldpath += tmp + wxT("/lib64");
#else
    ldpath += tmp + wxT("/lib");
#endif
    if (!::wxSetEnv(wxT("LD_LIBRARY_PATH"), ldpath)) {
        ::wxLogSysError(wxT("Can not set LD_LIBRARY_PATH"));
        return false;
    }
#endif

    if (::wxGetEnv(wxT("WXTRACE"), &tmp)) {
        wxStringTokenizer t(tmp, wxT(",:"));
        while (t.HasMoreTokens()) {
            wxString tag = t.GetNextToken();
            if (allTraceTags.Index(tag) != wxNOT_FOUND) {
                ::wxLogDebug(wxT("Trace for '%s' enabled"), tag.c_str());
                wxLog::AddTraceMask(tag);
            }
        }
    }

    checkNxSmartCardSupport();
    checkLibUSB();
    return true;
}

int opennxApp::FilterEvent(wxEvent& event)
{
    if (event.IsCommandEvent() && m_bRunproc) {
        wxCommandEvent *ce = (wxCommandEvent *)&event;
        if (ce->GetEventType() == wxEVT_GENERIC) {
            MyIPC::tSessionEvents e = wx_static_cast(MyIPC::tSessionEvents, ce->GetInt());
            wxString msg(ce->GetString());
            switch (e) {
                case MyIPC::ActionTerminated:
                    m_bRunproc = false;
                    return true;
                case MyIPC::ActionStderr:
                    if (msg.IsSameAs(wxT("no support for smartcards.")))
                        m_bNxSmartCardSupport = false;
                    return true;
            }
            return false;
        }
    }
    return -1;
}

void opennxApp::checkLibUSB()
{
#ifdef SUPPORT_USBIP
    USB u;
    m_bLibUSBAvailable = u.IsAvailable();
#else
    m_bLibUSBAvailable = false;
#endif
}

void opennxApp::checkNxSmartCardSupport()
{
    LibOpenSC l;
    if (!l.HasOpenSC()) {
        m_bNxSmartCardSupport = false;
        wxConfigBase::Get()->Write(wxT("Config/NxSshSmartCardSupport"), m_bNxSmartCardSupport);
        wxConfigBase::Get()->Flush();
        return;
    }
    wxString sysdir;
    wxConfigBase::Get()->Read(wxT("Config/SystemNxDir"), &sysdir);
    wxFileName fn(sysdir, wxEmptyString);
    fn.AppendDir(wxT("bin"));
#ifdef __WXMSW__
    fn.SetName(wxT("nxssh.exe"));
#else
    fn.SetName(wxT("nxssh"));
#endif
    if (!fn.FileExists())
        return;
    time_t last_mtime;
    long last_size;
    time_t mtime = fn.GetModificationTime().GetTicks();
    long size = fn.GetSize().ToULong();
    wxConfigBase::Get()->Read(wxT("Config/NxSshStamp"), &last_mtime, 0);
    wxConfigBase::Get()->Read(wxT("Config/NxSshSize"), &last_size, 0);
    wxConfigBase::Get()->Read(wxT("Config/NxSshSmartCardSupport"), &m_bNxSmartCardSupport, false);

    if ((mtime != last_mtime) || (size != last_size)) {
        wxConfigBase::Get()->Write(wxT("Config/NxSshStamp"), mtime);
        wxConfigBase::Get()->Write(wxT("Config/NxSshSize"), size);
        wxString nxsshcmd = fn.GetShortPath();
        nxsshcmd << wxT(" -I 0 -V");
        MyIPC testproc;
        if (testproc.GenericProcess(nxsshcmd, wxEmptyString, this)) {
            m_bNxSmartCardSupport = true;
            m_bRunproc = true;
            while (m_bRunproc) {
                wxLog::FlushActive();
                Yield(true);
                wxThread::Sleep(500);
            }
            wxConfigBase::Get()->Write(wxT("Config/NxSshSmartCardSupport"), m_bNxSmartCardSupport);
        }
        wxConfigBase::Get()->Flush();
    }
}

void opennxApp::OnInitCmdLine(wxCmdLineParser& parser)
{
    // Init standard options (--help, --verbose);
    wxApp::OnInitCmdLine(parser);

    // tags will be appended to the last switch/option
    wxString tags;
    allTraceTags.Sort();
    for (int i = 0; i < allTraceTags.GetCount(); i++) {
        if (!tags.IsEmpty())
            tags += wxT(" ");
        tags += allTraceTags.Item(i);
    }
    tags.Prepend(_("\n\nSupported trace tags: "));

    parser.AddSwitch(wxEmptyString, wxT("admin"),
            _("Start the session administration tool."));
    parser.AddOption(wxEmptyString, wxT("caption"),
            _("Secify window title for dialog mode."));
    parser.AddOption(wxEmptyString, wxT("style"),
            _("Secify dialog style for dialog mode."));
    parser.AddOption(wxEmptyString, wxT("dialog"),
            _("Run in dialog mode."));
    parser.AddSwitch(wxEmptyString, wxT("local"),
            _("Dialog mode proxy."));
    parser.AddOption(wxEmptyString, wxT("message"),
            _("Specify message for dialog mode."));
    parser.AddOption(wxEmptyString, wxT("parent"),
            _("Specify parent PID for dialog mode."), wxCMD_LINE_VAL_NUMBER);
    parser.AddOption(wxEmptyString, wxT("session"),
            _("Run a session importing configuration settings from FILENAME."));
    parser.AddOption(wxEmptyString, wxT("window"),
            _("Specify window-ID for dialog mode."), wxCMD_LINE_VAL_NUMBER);
    parser.AddOption(wxEmptyString, wxT("trace"),
            _("Specify wxWidgets trace mask."));
    parser.AddSwitch(wxEmptyString, wxT("wizard"),
            _("Guide the user through the steps to configure a session.") + tags);
}

static const wxChar *_dlgTypes[] = {
    wxT("yesno"), wxT("ok"), wxT("error"), wxT("panic"),
    wxT("quit"), wxT("pulldown"), wxT("yesnosuspend")
};

static wxArrayString aDlgTypes(sizeof(_dlgTypes)/sizeof(wxChar *), _dlgTypes);

static const wxChar *_dlgClasses[] = {
    wxT("info"), wxT("warning"), wxT("error")
};

static wxArrayString aDlgClasses(sizeof(_dlgClasses)/sizeof(wxChar *), _dlgClasses);

bool opennxApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
    if (!wxApp::OnCmdLineParsed(parser))
        return false;
    wxString sDlgType;

    m_eMode = MODE_CLIENT;
    if (parser.Found(wxT("dialog"), &sDlgType)) {
        wxString tmp;
        m_iDialogStyle = wxICON_WARNING;
        (void)parser.Found(wxT("caption"), &m_sDialogCaption);
        (void)parser.Found(wxT("message"), &m_sDialogMessage);
        (void)parser.Found(wxT("style"), &tmp);
        m_sDialogMessage.Replace(wxT("\\r\\n"), wxT("\n"));
        m_sDialogMessage.Replace(wxT("\\r"), wxT("\n"));
        m_sDialogMessage.Replace(wxT("\\n"), wxT("\n"));
        m_sDialogMessage.Replace(wxT("\\t"), wxT("\t"));
        if (!parser.Found(wxT("parent"), &m_nOtherPID))
            m_nOtherPID = (long)getppid();
        switch (aDlgClasses.Index(tmp)) {
            case 0:
                m_iDialogStyle = wxICON_INFORMATION;
                break;
            case 2:
                m_iDialogStyle = wxICON_ERROR;
                break;
            default:
                m_iDialogStyle = wxICON_WARNING;
                break;
        }
        switch (aDlgTypes.Index(sDlgType)) {
            case 0:
                // yesno
                m_iDialogStyle |= wxYES_NO;
                m_eMode = MODE_DIALOG_YESNO;
                return true;
            case 1:
                // ok
                m_iDialogStyle |= wxOK;
                m_eMode = MODE_DIALOG_OK;
                return true;
            case 2:
                // error
                m_iDialogStyle |= wxOK;
                m_eMode = MODE_DIALOG_ERROR;
                return true;
            case 3:
                // panic (Buttons: Terminate and Cancel, Terminate sends SIGTERM)
                m_eMode = MODE_DIALOG_PANIC;
                return true;
            case 4:
                // quit (Button: Quit, no signals)
                m_eMode = MODE_DIALOG_QUIT;
                return true;
            case 5:
                // pulldown (a toolbar, docked to top-center of wID),
                // timing out after ~ 6sec. The toolbar has 3 buttons:
                // suspend, terminate and close.
                // suspend sends SIGHUP to real ppid, terminate sends SIGTERM to pPID
                // and close sends WM_CLOSE event to wID
                if (!parser.Found(wxT("window"), &m_nWindowID)) {
                    OnCmdLineError(parser);
                    return false;
                }
                m_eMode = MODE_FOREIGN_TOOLBAR;
                return true;
            case 6:
                // yesnosuspend (Buttons: Suspend, Terminate and Cancel,
                // Terminate sends SIGTERM to pPID, Suspend sends SIGHUP to real ppid)
                break;
            default:
                OnCmdLineError(parser);
                return false;
        }
        return false;
    }
    if (parser.Found(wxT("admin")))
        m_eMode = MODE_ADMIN;
    if (parser.Found(wxT("wizard")))
        m_eMode = MODE_WIZARD;
    (void)parser.Found(wxT("session"), &m_sSessionName);
    wxString traceTags;
    if (parser.Found(wxT("trace"), &traceTags)) {
        wxStringTokenizer t(traceTags, wxT(","));
        while (t.HasMoreTokens()) {
            wxString tag = t.GetNextToken();
            if (allTraceTags.Index(tag) == wxNOT_FOUND) {
                OnCmdLineError(parser);
                return false;
            }
            ::wxLogDebug(wxT("Trace for '%s' enabled"), tag.c_str());
            wxLog::AddTraceMask(tag);
        }
    }
    return true;
}

// 'Main program' equivalent: the program execution "starts" here
bool opennxApp::realInit()
{
    if (!preInit())
        return false;

    wxString tmp;
    wxConfigBase::Get()->Read(wxT("Config/SystemNxDir"), &tmp);
    m_cLocale.AddCatalogLookupPathPrefix(tmp + wxFileName::GetPathSeparator()
            + wxT("share") + wxFileName::GetPathSeparator() + wxT("locale"));
    m_cLocale.AddCatalogLookupPathPrefix(wxT("locale"));
    m_cLocale.Init();
    m_cLocale.AddCatalog(wxT("opennx"));

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
    wxXmlResource::Get()->AddHandler(new wxRichTextCtrlXmlHandler());

    // This enable socket-I/O from other threads.
    wxSocketBase::Initialize();

    bool resok = false;
    wxString optionalRsc = tmp + wxFileName::GetPathSeparator() + wxT("share")
        + wxFileName::GetPathSeparator() + wxT("opennx.rsc");
    if (wxFileName::FileExists(optionalRsc)) {
        wxFile rf(optionalRsc);
        if (rf.IsOpened()) {
            unsigned char *resptr = (unsigned char *)malloc(rf.Length());
            if (resptr) {
                if (rf.Read(resptr, rf.Length()) == rf.Length()) {
                    wxMemoryFSHandler::AddFileWithMimeType(wxT("memrsc"), resptr, rf.Length(), wxT("application/zip"));
                    {
                        // The following code eliminates a stupid error dialog which shows up
                        // if some .desktop entires (in KDE or GNOME applink dirs) are dangling symlinks.
                        wxLogNull lognull;
                        wxTheMimeTypesManager->GetFileTypeFromExtension(wxT("zip"));
                    }
                    resok = true;
                }
                free(resptr);
            }
        }
    }
    if (!resok) {
        const unsigned char *resptr = get_mem_res();
        if (resptr) {
            wxMemoryFSHandler::AddFileWithMimeType(wxT("memrsc"), resptr, cnt_mem_res, wxT("application/zip"));
            {
                // The following code eliminates a stupid error dialog which shows up
                // if some .desktop entires (in KDE or GNOME applink dirs) are dangling symlinks.
                wxLogNull lognull;
                wxTheMimeTypesManager->GetFileTypeFromExtension(wxT("zip"));
            }
            free_mem_res(resptr);
        }
        resok = true;
    }
    if (!resok) {
        wxLogFatalError(wxT("Could not load application resource."));
        return false;
    }

    m_sResourcePrefix = wxT("memory:memrsc#zip:");
    if (!wxXmlResource::Get()->Load(m_sResourcePrefix + wxT("res/opennx.xrc")))
        return false;

    switch (m_eMode) {
        case MODE_CLIENT:
        case MODE_WIZARD:
            break;
        case MODE_DIALOG_YESNO:
            {
                wxMessageDialog d(NULL, m_sDialogMessage, m_sDialogCaption, m_iDialogStyle);
                d.SetIcon(CreateIconFromFile(wxT("res/nx.png")));
                if (d.ShowModal() == wxYES)
                    ::wxKill(m_nOtherPID, wxSIGTERM);
                return false;
            }
            break;
        case MODE_DIALOG_OK:
            {
                wxMessageDialog d(NULL, m_sDialogMessage, m_sDialogCaption, m_iDialogStyle);
                d.SetIcon(CreateIconFromFile(wxT("res/nx.png")));
                d.ShowModal();
                return false;
            }
            break;
        case MODE_DIALOG_ERROR:
            {
                wxMessageDialog d(NULL, m_sDialogMessage, m_sDialogCaption, m_iDialogStyle);
                d.SetIcon(CreateIconFromFile(wxT("res/nx.png")));
                d.ShowModal();
                ::wxKill(m_nOtherPID, wxSIGTERM);
                return false;
            }
            break;
        case MODE_DIALOG_PANIC:
            {
                PanicDialog d;
                d.SetMessage(m_sDialogMessage);
                d.SetDialogClass(m_iDialogStyle);
                d.Create(NULL, wxID_ANY, m_sDialogCaption);
                if (d.ShowModal() == wxID_OK)
                    ::wxKill(m_nOtherPID, wxSIGTERM);
                return false;
            }
            break;
        case MODE_DIALOG_QUIT:
            {
                QuitDialog d;
                d.SetMessage(m_sDialogMessage);
                d.SetDialogClass(m_iDialogStyle);
                d.Create(NULL, wxID_ANY, m_sDialogCaption);
                d.ShowModal();
                return false;
            }
            break;
        case MODE_FOREIGN_TOOLBAR:
            {
                ForeignFrame *ff = new ForeignFrame(NULL);
                // If we return true, the global config will
                // be deleted by the framework, so we set it to NULL here
                // to prevent double free.
                m_pCfg = NULL;
                ff->SetOtherPID(m_nOtherPID);
                ff->SetForeignWindowID(m_nWindowID);
                ff->Show();
                SetTopWindow(ff);
                return true;
            }
            break;
        case MODE_ADMIN:
            {
                SessionAdmin *sa = new SessionAdmin(NULL);
                // If we return true, the global config will
                // be deleted by the framework, so we set it to NULL here
                // to prevent double free.
                m_pCfg = NULL;
                sa->Show();
                SetTopWindow(sa);
                return true;
            }
            break;
    }

    if (!m_sSessionName.IsEmpty()) {
        wxFileName fn(m_sSessionName);
        if (fn.Normalize() && fn.FileExists())
            m_sSessionName = fn.GetFullPath();
    }
    if ((m_eMode == MODE_CLIENT) && m_sSessionName.IsEmpty()) {
        if (!wxConfigBase::Get()->Read(wxT("Config/LastSession"), &m_sSessionName))
            m_eMode = MODE_WIZARD;
    } else {
        if (!m_sSessionName.IsEmpty()) {
            MyXmlConfig cfg(m_sSessionName);
            if (cfg.IsValid())
                m_sSessionName = cfg.sGetFileName();
        }
    }

    if (m_eMode == MODE_WIZARD) {
        MyWizard wz(NULL);
        if (!wz.Run())
            return false;
        m_sSessionName = wz.sGetConfigName();
    }

    LoginDialog d;
    d.SetLastSessionFilename(m_sSessionName);
    d.Create(NULL);
    if (d.ShowModal() == wxID_OK) {
        m_sSessionName = d.GetLastSessionFilename();
        if (!m_sSessionName.IsEmpty())
            wxConfigBase::Get()->Write(wxT("Config/LastSession"), m_sSessionName);
    }

    // success: wxApp::OnRun() will be called which will enter the main message
    // loop and the application will run. We returne FALSE here, so that the
    // application exits if the dialog is destroyed.
    return false;
}

// 'Main program' equivalent: the program execution "starts" here
bool opennxApp::OnInit()
{
    bool ret = realInit();
#ifdef SUPPORT_USBIP
    if (m_bRequireStartUsbIp) {
        wxString usock = wxConfigBase::Get()->Read(wxT("Config/UsbipdSocket"),
                wxT("/var/run/usbipd2.socket"));
        UsbIp usbip;
        if (usbip.Connect(usock)) {
            int i, j, k;
            wxLogTrace(MYTRACETAG, wxT("connected to usbipd2"));
            usbip.SetSession(m_sSessionID);
            ArrayOfUsbForwards af = m_pSessionCfg->aGetUsbForwards();
            ArrayOfUsbIpDevices aid = usbip.GetDevices();
            ArrayOfUSBDevices ad;
            if (LibUSBAvailable()) {
                USB u;
                ad = u.GetDevices();
            }
            for (i = 0; i < af.GetCount(); i++)
                if (SharedUsbDevice::MODE_REMOTE == af[i].m_eMode) {
                    if (!LibUSBAvailable()) {
                        ::wxLogError(_("libusb is not available. No USB devices will be exported"));
                        break;
                    }
                    ::wxLogTrace(MYTRACETAG, wxT("possibly exported USB device: %04x/%04x %s"),
                            af[i].m_iVendorID, af[i].m_iProductID, af[i].toShortString().c_str());
                    for (j = 0; j < ad.GetCount(); j++)
                        if (af[i].MatchHotplug(ad[j])) {
                            ::wxLogTrace(MYTRACETAG, wxT("Match on USB dev %s"), ad[j].toString().c_str());
                            for (k = 0; k < aid.GetCount(); k++) {
                                if (aid[k].GetUsbBusID().IsSameAs(ad[j].GetBusID())) {
                                    wxString exBusID = aid[k].GetUsbIpBusID();
                                    ::wxLogTrace(MYTRACETAG, wxT("Exporting busid %s"), exBusID.c_str());
                                    if (!usbip.WaitForSession(20))
                                        ::wxLogError(_("Unable to export USB device %s"), af[i].toShortString().c_str());
                                    if (!usbip.ExportDevice(exBusID))
                                        ::wxLogError(_("Unable to export USB device %s"), af[i].toShortString().c_str());
                                }
                            }
                        }
                }
        } else
            ::wxLogError(_("Could not connect to usbipd2. No USB devices will be exported"));
    }

    if (m_bRequireStartUsbIp) {
        wxString appDir;
        wxConfigBase::Get()->Read(wxT("Config/SystemNxDir"), &appDir);
        wxFileName fn(appDir, wxEmptyString);
        fn.AppendDir(wxT("bin"));
#ifdef __WXMSW__
        fn.SetName(wxT("watchusbip.exe"));
#else
        fn.SetName(wxT("watchusbip"));
#endif
        wxString watchcmd = fn.GetShortPath();
        ::wxLogTrace(MYTRACETAG, wxT("cfgfile='%s'"), m_pSessionCfg->sGetFileName().c_str());
        watchcmd << wxT(" -s ") << m_sSessionID << wxT(" -p ")
            << m_nNxSshPID << wxT(" -c \"") << m_pSessionCfg->sGetFileName() << wxT("\"");
#ifdef __WXDEBUG__
        watchcmd << wxT(" --trace=UsbIp,watchUsbIpApp");
#endif
        ::wxLogTrace(MYTRACETAG, wxT("starting %s"), watchcmd.c_str());
        {
            wxLogNull noerrors;
            ::wxExecute(watchcmd);
        }
        while (Pending())
            Dispatch();
    }
#endif
    if (m_bRequireWatchReader) {
        ::wxLogTrace(MYTRACETAG, wxT("require Watchreader: m_iReader = %d, m_nNxSshPID = %ld"), m_iReader, m_nNxSshPID);
        if (-1 != m_iReader) {
            wxLogNull noerrors;
            wxString appDir;
            wxConfigBase::Get()->Read(wxT("Config/SystemNxDir"), &appDir);
            wxFileName fn(appDir, wxEmptyString);
            fn.AppendDir(wxT("bin"));
#ifdef __WXMSW__
            fn.SetName(wxT("watchreader.exe"));
#else
            fn.SetName(wxT("watchreader"));
#endif
            wxString watchcmd = fn.GetShortPath();
            watchcmd << wxT(" -r ") << m_iReader << wxT(" -p ") << m_nNxSshPID;
            ::wxExecute(watchcmd);
        }
    }
    while (::wxGetApp().Pending())
        ::wxGetApp().Dispatch();
    if (!ret) {
        wxLogNull lognull;
        wxMemoryFSHandler::RemoveFile(wxT("memrsc"));
    }
    return ret;
}

/*!
 * Cleanup for opennxApp
 */
int opennxApp::OnExit()
{
    {
        wxLogNull lognull;
        wxMemoryFSHandler::RemoveFile(wxT("memrsc"));
    }
    return wxApp::OnExit();
}

void opennxApp::SetSessionCfg(MyXmlConfig &cfg)
{
    m_pSessionCfg = new MyXmlConfig();
    *m_pSessionCfg = cfg;
    m_pSessionCfg->sSetFileName(cfg.sGetFileName());
}
