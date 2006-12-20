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

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma implementation "MyXmlConfig.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/filename.h>
#include <wx/xml/xml.h>
#include <wx/arrimpl.cpp>
#include <wx/file.h>
#include <wx/mstream.h>
#include <wx/wfstream.h>
#include <wx/regex.h>

class wxConfigBase;

#include "MyXmlConfig.h"
#include "WinShare.h"
#include "opennxApp.h"
#include "pwcrypt.h"
#include "osdep.h"

static wxString MYTRACETAG(wxFileName::FileName(wxT(__FILE__)).GetName());

#define STR_TRUE wxT("true")
#define STR_FALSE wxT("false");
#define STR_ONE wxT("1")
#define STR_ZERO wxT("0")

// We use something the user cannot type here!
#define DUMMY_MD5_PASSWORD wxT("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b")
#define DUMMY_CLR_PASSWORD wxT("\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r")

WX_DEFINE_OBJARRAY(ArrayOfShareGroups);

bool ShareGroup::operator ==(const ShareGroup &other)
{
    if (m_bMountNow != other.m_bMountNow) return false;
    if (m_iType != other.m_iType) return false;
    if (m_sGroupName != other.m_sGroupName) return false;
    if (m_sMountPoint != other.m_sMountPoint) return false;
    if (m_sName != other.m_sName) return false;
    if (m_sUsername != other.m_sUsername) return false;
    if (m_sPassword != other.m_sPassword) return false;
    return true;
}

bool ShareGroup::operator !=(const ShareGroup &other)
{
    return (!(*this == other));
}

MyXmlConfig::MyXmlConfig()
    : saved(NULL)
    , m_bDisableBackingstore(false)
    , m_bDisableComposite(false)
    , m_bDisableImageCompression(false)
    , m_bDisableJpeg(false)
    , m_bDisableRender(false)
    , m_bDisableShmem(false)
    , m_bDisableShpix(false)
    , m_bDisableStreaming(false)
    , m_bDisableTaint(false)
    , m_bDisableTcpNoDelay(false)
    , m_bDisableXagent(false)
    , m_bDisableZlibCompression(false)
    , m_bEnableMultimedia(false)
    , m_bEnableSmbSharing(false)
    , m_bEnableSSL(true)
    , m_bGuestMode(false)
    , m_bImageEncodingJpeg(true)
    , m_bImageEncodingPlainX(false)
    , m_bImageEncodingPNG(false)
    , m_bKbdLayoutOther(false)
    , m_bRdpRememberPassword(false)
    , m_bRdpRunApplication(false)
    , m_bRememberPassword(false)
    , m_bRemoveOldSessionFiles(true)
    , m_bRunConsole(false)
    , m_bRunXclients(false)
    , m_bUseCups(false)
    , m_bUseCustomImageEncoding(false)
    , m_bUseJpegQuality(false)
    , m_bUseProxy(false)
    , m_bUseSmartCard(false)
    , m_bUseTightJpeg(false)
    , m_bValid(false)
    , m_bVirtualDesktop(false)
    , m_bVncRememberPassword(false)
    , m_bVncUseNxAuth(false)

    , m_iCupsPort(631)
    , m_iDisplayHeight(480)
    , m_iDisplayWidth(640)
    , m_iJpegQuality(6)
    , m_iKbdLayoutLanguage(mapLocaleToKeyboard())
    , m_iProxyPort(3128)
    , m_iRdpAuthType(1)
    , m_iRdpImageCompression(1)
    , m_iServerPort(22)
    , m_iUsedShareGroups(0)
    , m_iVncDisplayNumber(0)
    , m_iVncImageEncoding(0)

    , m_eCacheDisk(CACHEDISK_32MB)
    , m_eCacheMemory(CACHEMEM_8MB)
    , m_eConnectionSpeed(SPEED_ADSL)
    , m_eDesktopType(DTYPE_KDE)
    , m_eDisplayType(DPTYPE_AVAILABLE)
    , m_eSessionType(STYPE_UNIX)

    , m_sCommandLine(wxT(""))
    , m_sCupsPath(wxT(CUPS_DEFAULT_PATH))
    , m_sFileName(wxT(""))
    , m_sGuestUser(wxT(""))
    , m_sGuestPassword(wxT(""))
    , m_sName(wxT(""))
    , m_sPassword(wxT(""))
    , m_sProxyHost(wxT(""))
    , m_sRdpApplication(wxT(""))
    , m_sRdpHostName(wxT(""))
    , m_sRdpPassword(wxT(""))
    , m_sRdpUsername(wxT(""))
    , m_sServerHost(wxT(""))
    , m_sSshKey(wxT(""))
    , m_sUsername(wxT(""))
    , m_sVncHostName(wxT(""))
    , m_sVncPassword(wxT(""))

    , m_pMd5Password(NULL)
    , m_pClrPassword(NULL)
{
}

    MyXmlConfig &
MyXmlConfig::operator =(const MyXmlConfig &other)
{
    saved = NULL;

    m_bDisableBackingstore = other.m_bDisableBackingstore;
    m_bDisableComposite = other.m_bDisableComposite;
    m_bDisableImageCompression = other.m_bDisableImageCompression;
    m_bDisableJpeg = other.m_bDisableJpeg;
    m_bDisableRender = other.m_bDisableRender;
    m_bDisableShmem = other.m_bDisableShmem;
    m_bDisableShpix = other.m_bDisableShpix;
    m_bDisableStreaming = other.m_bDisableStreaming;
    m_bDisableTaint = other.m_bDisableTaint;
    m_bDisableTcpNoDelay = other.m_bDisableTcpNoDelay;
    m_bDisableXagent = other.m_bDisableXagent;
    m_bDisableZlibCompression = other.m_bDisableZlibCompression;
    m_bEnableMultimedia = other.m_bEnableMultimedia;
    m_bEnableSmbSharing = other.m_bEnableSmbSharing;
    m_bEnableSSL = other.m_bEnableSSL;
    m_bGuestMode = other.m_bGuestMode;
    m_bImageEncodingJpeg = other.m_bImageEncodingJpeg;
    m_bImageEncodingPlainX = other.m_bImageEncodingPlainX;
    m_bImageEncodingPNG = other.m_bImageEncodingPNG;
    m_bKbdLayoutOther = other.m_bKbdLayoutOther;
    m_bRdpRememberPassword = other.m_bRdpRememberPassword;
    m_bRdpRunApplication = other.m_bRdpRunApplication;
    m_bRememberPassword = other.m_bRememberPassword;
    m_bRemoveOldSessionFiles = other.m_bRemoveOldSessionFiles;
    m_bRunConsole = other.m_bRunConsole;
    m_bRunXclients = other.m_bRunXclients;
    m_bUseCups = other.m_bUseCups;
    m_bUseCustomImageEncoding = other.m_bUseCustomImageEncoding;
    m_bUseJpegQuality = other.m_bUseJpegQuality;
    m_bUseProxy = other.m_bUseProxy;
    m_bUseSmartCard = other.m_bUseSmartCard;
    m_bUseTightJpeg = other.m_bUseTightJpeg;
    m_bValid = other.m_bValid;
    m_bVirtualDesktop = other.m_bVirtualDesktop;
    m_bVncRememberPassword = other.m_bVncRememberPassword;
    m_bVncUseNxAuth = other.m_bVncUseNxAuth;

    m_iCupsPort = other.m_iCupsPort;
    m_iDisplayHeight = other.m_iDisplayHeight;
    m_iDisplayWidth = other.m_iDisplayWidth;
    m_iJpegQuality = other.m_iJpegQuality;
    m_iKbdLayoutLanguage = other.m_iKbdLayoutLanguage;
    m_iProxyPort = other.m_iProxyPort;
    m_iRdpAuthType = other.m_iRdpAuthType;
    m_iRdpImageCompression = other.m_iRdpImageCompression;
    m_iServerPort = other.m_iServerPort;
    m_iUsedShareGroups = other.m_iUsedShareGroups;
    m_iVncDisplayNumber = other.m_iVncDisplayNumber;
    m_iVncImageEncoding = other.m_iVncImageEncoding;

    m_eCacheDisk = other.m_eCacheDisk;
    m_eCacheMemory = other.m_eCacheMemory;
    m_eConnectionSpeed = other.m_eConnectionSpeed;
    m_eDesktopType = other.m_eDesktopType;
    m_eDisplayType = other.m_eDisplayType;
    m_eSessionType = other.m_eSessionType;

    m_sCommandLine = other.m_sCommandLine;
    m_sCupsPath = other.m_sCupsPath;
    m_sGuestPassword = other.m_sGuestPassword;
    m_sGuestUser = other.m_sGuestUser;
    m_sName = other.m_sName;
    m_sPassword = other.m_sPassword;
    m_sProxyHost = other.m_sProxyHost;
    m_sRdpApplication = other.m_sRdpApplication;
    m_sRdpHostName = other.m_sRdpHostName;
    m_sRdpPassword = other.m_sRdpPassword;
    m_sRdpUsername = other.m_sRdpUsername;
    m_sServerHost = other.m_sServerHost;
    m_sSshKey = other.m_sSshKey;
    m_sUsername = other.m_sUsername;
    m_sVncHostName = other.m_sVncHostName;
    m_sVncPassword = other.m_sVncPassword;

    m_aShareGroups = other.m_aShareGroups;
    m_aUsedShareGroups = other.m_aUsedShareGroups;

    if (other.m_pMd5Password) {
        if (m_pMd5Password)
            delete m_pMd5Password;
        m_pMd5Password = new wxString(*other.m_pMd5Password);
    }
    if (other.m_pClrPassword) {
        if (m_pClrPassword)
            delete m_pClrPassword;
        m_pClrPassword = new wxString(*other.m_pClrPassword);
    }
    return *this;
}

// Retrieve parameters for proxy otion file
    wxString
MyXmlConfig::sGetProxyParams(const wxString &protocolVersion)
{
    wxUnusedVar(protocolVersion);
    wxString ret = wxT("");
    ret << wxT(",shmem=") << (m_bDisableShmem ? 0 : 1)
        << wxT(",shpix=") << (m_bDisableShpix ? 0 : 1)
        ;
    // FIXME: use real settings
    ret << wxT(",font=1");
    return ret;
}

// Retrieve parameters for listsession command
    wxString
MyXmlConfig::sGetListParams(const wxString &protocolVersion)
{
    wxUnusedVar(protocolVersion);
    wxString ret = wxT(" --user=\"");

    ret << m_sUsername << wxT("\" --status=\"suspended,running\"")
        << wxT(" --type=\"");
    switch (m_eSessionType) {
        case STYPE_UNIX:
            ret << wxT("unix-");
            switch (m_eDesktopType) {
                case DTYPE_KDE:
                    ret << wxT("kde\"");
                    break;
                case DTYPE_GNOME:
                    ret << wxT("gnome\"");
                    break;
                case DTYPE_CDE:
                    ret << wxT("cde\"");
                    break;
                case DTYPE_XFCE:
                    ret << wxT("xfce\"");
                    break;
                case DTYPE_CUSTOM:
                    ret << wxT("custom\"");
                    break;
            }
            break;
        case STYPE_WINDOWS:
            ret << wxT("rdp");
            break;
        case STYPE_VNC:
            ret << wxT("vnc");
            break;
    }
    int w, h;
    ::wxDisplaySize(&w, &h);
    ret << wxT(" --geometry=\"") << w << wxT("x") << h << wxT("x")
       << ::wxDisplayDepth() << (m_bDisableRender ? wxT("") : wxT("+render")) << wxT("\"");
    return ret;
}

// Retrieve parameters for startsession command
    wxString
MyXmlConfig::sGetSessionParams(const wxString &protocolVersion, bool bNew)
{
    wxUnusedVar(protocolVersion);
    wxString ret = wxT("");

    if (bNew) {
        ret += wxString::Format(wxT(" --session=\"%s\""), m_sName.c_str());
        ret += wxT(" --type=\"");
        switch (m_eSessionType) {
            case STYPE_UNIX:
                ret += wxT("unix-");
                switch (m_eDesktopType) {
                    case DTYPE_KDE:
                        ret += wxT("kde\" ");
                        break;
                    case DTYPE_GNOME:
                        ret += wxT("gnome\" ");
                        break;
                    case DTYPE_CDE:
                        ret += wxT("cde\" ");
                        break;
                    case DTYPE_XFCE:
                        ret += wxT("xfce\" ");
                        break;
                    case DTYPE_CUSTOM:
                        ret += wxT("custom\" ");
                        break;
                }
                break;
            case STYPE_WINDOWS:
                ret += wxT("rdp");
                break;
            case STYPE_VNC:
                ret += wxT("vnc");
                break;
        }
    }
    ret += wxT(" --cache=\"");
    switch (m_eCacheMemory) {
        case CACHEMEM_1MB:
            ret += wxT("1M\"");
            break;
        case CACHEMEM_2MB:
            ret += wxT("2M\"");
            break;
        case CACHEMEM_4MB:
            ret += wxT("4M\"");
            break;
        case CACHEMEM_8MB:
            ret += wxT("8M\"");
            break;
        case CACHEMEM_16MB:
            ret += wxT("16M\"");
            break;
        case CACHEMEM_32MB:
            ret += wxT("32M\"");
            break;
    }
    ret += wxT(" --images=\"");
    switch (m_eCacheDisk) {
        case CACHEDISK_0MB:
            ret += wxT("0\"");
            break;
        case CACHEDISK_4MB:
            ret += wxT("4M\"");
            break;
        case CACHEDISK_8MB:
            ret += wxT("8M\"");
            break;
        case CACHEDISK_16MB:
            ret += wxT("16M\"");
            break;
        case CACHEDISK_32MB:
            ret += wxT("32M\"");
            break;
        case CACHEDISK_64MB:
            ret += wxT("64M\"");
            break;
        case CACHEDISK_128MB:
            ret += wxT("128M\"");
            break;
    }
    ret += wxT(" --link=\"");
    switch (m_eConnectionSpeed) {
        case SPEED_MODEM:
            ret += wxT("modem\"");
            break;
        case SPEED_ISDN:
            ret += wxT("isdn\"");
            break;
        case SPEED_ADSL:
            ret += wxT("adsl\"");
            break;
        case SPEED_WAN:
            ret += wxT("wan\"");
            break;
        case SPEED_LAN:
            ret += wxT("lan\"");
            break;
    }
    ret += wxT(" --geometry=\"");
    switch (m_eDisplayType) {
        case DPTYPE_640x480:
            ret += wxT("640x480\"");
            break;
        case DPTYPE_800x600:
            ret += wxT("800x600\"");
            break;
        case DPTYPE_1024x768:
            ret += wxT("1024x768\"");
            break;
        case DPTYPE_AVAILABLE:
            ret += wxT("available\"");
            break;
        case DPTYPE_FULLSCREEN:
            ret += wxT("fullscreen\"");
            break;
        case DPTYPE_CUSTOM:
            ret += wxString::Format(wxT("%dx%d\""), m_iDisplayWidth, m_iDisplayHeight);
            break;
    }
    int w, h;
    ::wxDisplaySize(&w, &h);
    ret << wxT(" --screeninfo=\"") << w << wxT("x") << h << wxT("x")
        << ::wxDisplayDepth() << (m_bDisableRender ? wxT("") : wxT("+render")) << wxT("\"");

    wxString kbdLocal = wxConvLocal.cMB2WX(x11_keyboard_type);
    ret << wxT(" --keyboard=\"");
    if (m_bKbdLayoutOther)
        ret << kbdLocal.BeforeFirst(wxT('/')) << wxT("/") << isoKbd(m_iKbdLayoutLanguage);
    else
        ret << kbdLocal;
    ret << wxT("\"")
        << wxT(" --backingstore=\"")
        << (m_bDisableBackingstore ? 0 : 1)
        << wxT("\"")
        << wxT(" --encryption=\"")
        << (m_bEnableSSL ? 1 : 0)
        << wxT("\"")
        << wxT(" --composite=\"") << (m_bDisableComposite ? 0 : 1) << wxT("\"")
        << wxT(" --shmem=\"") << (m_bDisableShmem ? 0 : 1) << wxT("\"")
        << wxT(" --shpix=\"") << (m_bDisableShpix ? 0 : 1) << wxT("\"")
        << wxT(" --streaming=\"") << (m_bDisableStreaming ? 0 : 1) << wxT("\"")
        << wxT(" --cups=\"") << (m_bUseCups ? 1 : 0) << wxT("\"")
        << wxT(" --nodelay=\"") << (m_bDisableTcpNoDelay ? 0 : 1) << wxT("\"")
#ifdef __UNIX__
        << wxT(" --client=\"linux\"")
#else
        << wxT(" --client=\"windows\"")
#endif
        ;
        // FIXME: Add real settings
        //
        ret += wxT(" --media=\"0\"");
        ret += wxT(" --strict=\"0\"");
    return ret;
}

// Compare two arrays of ShareGroup where sequence of elements is irrelevant
// return true, if arrays are different.
    bool
MyXmlConfig::cmpShareGroups(ArrayOfShareGroups a1, ArrayOfShareGroups a2)
{
    size_t cnt = a1.GetCount();
    if (cnt != a2.GetCount())
        return true;
    ArrayOfShareGroups ca = a2;
    for (size_t i = 0; i < cnt; i++) {
        size_t cnt2 = ca.GetCount();
        int idx = wxNOT_FOUND;
        for (size_t j = 0; j < cnt2; j++) {
            if (a1[i] == ca[j]) {
                idx = j;
                break;
            }
        }
        if (idx == wxNOT_FOUND)
            return true;
        ca.RemoveAt(idx);
    }
    return (ca.GetCount() != 0);
}

// Compare two wxArrayString where sequence of elements is irrelevant
// return true, if arrays are different.
    bool
MyXmlConfig::cmpUsedShareGroups(wxArrayString a1, wxArrayString a2)
{
    size_t cnt = a1.GetCount();
    if (cnt != a2.GetCount())
        return true;
    wxArrayString ca = a2;
    for (size_t i = 0; i < cnt; i++) {
        int idx = ca.Index(a1.Item(i));
        if (idx == wxNOT_FOUND)
            return true;
        ca.RemoveAt(idx);
    }
    return (ca.GetCount() != 0);
}

    bool
MyXmlConfig::operator ==(const MyXmlConfig &other)
{
    if (m_bDisableBackingstore != other.m_bDisableBackingstore) return false;
    if (m_bDisableComposite != other.m_bDisableComposite) return false;
    if (m_bDisableImageCompression != other.m_bDisableImageCompression) return false;
    if (m_bDisableJpeg != other.m_bDisableJpeg) return false;
    if (m_bDisableRender != other.m_bDisableRender) return false;
    if (m_bDisableShmem != other.m_bDisableShmem) return false;
    if (m_bDisableShpix != other.m_bDisableShpix) return false;
    if (m_bDisableStreaming != other.m_bDisableStreaming) return false;
    if (m_bDisableTaint != other.m_bDisableTaint) return false;
    if (m_bDisableTcpNoDelay != other.m_bDisableTcpNoDelay) return false;
    if (m_bDisableXagent != other.m_bDisableXagent) return false;
    if (m_bDisableZlibCompression != other.m_bDisableZlibCompression) return false;
    if (m_bEnableMultimedia != other.m_bEnableMultimedia) return false;
    if (m_bEnableSmbSharing != other.m_bEnableSmbSharing) return false;
    if (m_bEnableSSL != other.m_bEnableSSL) return false;
    if (m_bGuestMode != other.m_bGuestMode) return false;
    if (m_bImageEncodingJpeg != other.m_bImageEncodingJpeg) return false;
    if (m_bImageEncodingPlainX != other.m_bImageEncodingPlainX) return false;
    if (m_bImageEncodingPNG != other.m_bImageEncodingPNG) return false;
    if (m_bKbdLayoutOther != other.m_bKbdLayoutOther) return false;
    if (m_bRdpRememberPassword != other.m_bRdpRememberPassword) return false;
    if (m_bRdpRunApplication != other.m_bRdpRunApplication) return false;
    if (m_bRememberPassword != other.m_bRememberPassword) return false;
    if (m_bRemoveOldSessionFiles != other.m_bRemoveOldSessionFiles) return false;
    if (m_bRunConsole != other.m_bRunConsole) return false;
    if (m_bRunXclients != other.m_bRunXclients) return false;
    if (m_bUseCups != other.m_bUseCups) return false;
    if (m_bUseCustomImageEncoding != other.m_bUseCustomImageEncoding) return false;
    if (m_bUseJpegQuality != other.m_bUseJpegQuality) return false;
    if (m_bUseProxy != other.m_bUseProxy) return false;
    if (m_bUseSmartCard != other.m_bUseSmartCard) return false;
    if (m_bUseTightJpeg != other.m_bUseTightJpeg) return false;
    if (m_bValid != other.m_bValid) return false;
    if (m_bVirtualDesktop != other.m_bVirtualDesktop) return false;
    if (m_bVncRememberPassword != other.m_bVncRememberPassword) return false;
    if (m_bVncUseNxAuth != other.m_bVncUseNxAuth) return false;

    if (m_iCupsPort != other.m_iCupsPort) return false;
    if (m_iDisplayHeight != other.m_iDisplayHeight) return false;
    if (m_iDisplayWidth != other.m_iDisplayWidth) return false;
    if (m_iJpegQuality != other.m_iJpegQuality) return false;
    if (m_iKbdLayoutLanguage != other.m_iKbdLayoutLanguage) return false;
    if (m_iProxyPort != other.m_iProxyPort) return false;
    if (m_iRdpAuthType != other.m_iRdpAuthType) return false;
    if (m_iRdpImageCompression != other.m_iRdpImageCompression) return false;
    if (m_iServerPort != other.m_iServerPort) return false;
    if (m_iUsedShareGroups != other.m_iUsedShareGroups) return false;
    if (m_iVncDisplayNumber != other.m_iVncDisplayNumber) return false;
    if (m_iVncImageEncoding != other.m_iVncImageEncoding) return false;

    if (m_eCacheDisk != other.m_eCacheDisk) return false;
    if (m_eCacheMemory != other.m_eCacheMemory) return false;
    if (m_eConnectionSpeed != other.m_eConnectionSpeed) return false;
    if (m_eDesktopType != other.m_eDesktopType) return false;
    if (m_eDisplayType != other.m_eDisplayType) return false;
    if (m_eSessionType != other.m_eSessionType) return false;

    if (m_sCommandLine != other.m_sCommandLine) return false;
    if (m_sCupsPath != other.m_sCupsPath) return false;
    // if (m_sFileName != other.m_sFileName) return false;
    if (m_sName != other.m_sName) return false;
    if (m_sPassword != other.m_sPassword) return false;
    if (m_sProxyHost != other.m_sProxyHost) return false;
    if (m_sRdpApplication != other.m_sRdpApplication) return false;
    if (m_sRdpHostName != other.m_sRdpHostName) return false;
    if (m_sRdpPassword != other.m_sRdpPassword) return false;
    if (m_sRdpUsername != other.m_sRdpUsername) return false;
    if (m_sServerHost != other.m_sServerHost) return false;
    if (m_sSshKey != other.m_sSshKey) return false;
    if (m_sUsername != other.m_sUsername) return false;
    if (m_sVncHostName != other.m_sVncHostName) return false;
    if (m_sVncPassword != other.m_sVncPassword) return false;

    if (cmpUsedShareGroups(m_aUsedShareGroups, other.m_aUsedShareGroups)) return false;
    if (cmpShareGroups(m_aShareGroups, other.m_aShareGroups)) return false;
    return true;
}

MyXmlConfig::MyXmlConfig(const wxString &filename)
    : saved(NULL)

    , m_bDisableBackingstore(false)
    , m_bDisableComposite(false)
    , m_bDisableImageCompression(false)
    , m_bDisableJpeg(false)
    , m_bDisableRender(false)
    , m_bDisableShmem(false)
    , m_bDisableShpix(false)
    , m_bDisableStreaming(false)
    , m_bDisableTaint(false)
    , m_bDisableTcpNoDelay(false)
    , m_bDisableXagent(false)
    , m_bDisableZlibCompression(false)
    , m_bEnableMultimedia(false)
    , m_bEnableSmbSharing(false)
    , m_bEnableSSL(true)
    , m_bGuestMode(false)
    , m_bImageEncodingJpeg(true)
    , m_bImageEncodingPlainX(false)
    , m_bImageEncodingPNG(false)
    , m_bKbdLayoutOther(false)
    , m_bRdpRememberPassword(false)
    , m_bRdpRunApplication(false)
    , m_bRememberPassword(false)
    , m_bRemoveOldSessionFiles(true)
    , m_bRunConsole(false)
    , m_bRunXclients(false)
    , m_bUseCups(false)
    , m_bUseCustomImageEncoding(false)
    , m_bUseJpegQuality(false)
    , m_bUseProxy(false)
    , m_bUseSmartCard(false)
    , m_bUseTightJpeg(false)
    , m_bValid(false)
    , m_bVirtualDesktop(false)
    , m_bVncRememberPassword(false)
    , m_bVncUseNxAuth(false)

    , m_iCupsPort(631)
    , m_iDisplayHeight(480)
    , m_iDisplayWidth(640)
    , m_iJpegQuality(6)
    , m_iKbdLayoutLanguage(mapLocaleToKeyboard())
    , m_iProxyPort(3128)
    , m_iRdpAuthType(1)
    , m_iRdpImageCompression(1)
    , m_iServerPort(22)
    , m_iUsedShareGroups(0)
    , m_iVncDisplayNumber(0)
    , m_iVncImageEncoding(0)

    , m_eConnectionSpeed(SPEED_ADSL)
    , m_eDesktopType(DTYPE_KDE)
    , m_eDisplayType(DPTYPE_AVAILABLE)
    , m_eSessionType(STYPE_UNIX)
    , m_eCacheMemory(CACHEMEM_8MB)
    , m_eCacheDisk(CACHEDISK_32MB)

    , m_sCommandLine(wxT(""))
    , m_sCupsPath(wxT(CUPS_DEFAULT_PATH))
    , m_sFileName(wxT(""))
    , m_sGuestPassword(wxT(""))
    , m_sGuestUser(wxT(""))
    , m_sName(wxT(""))
    , m_sPassword(wxT(""))
    , m_sProxyHost(wxT(""))
    , m_sRdpApplication(wxT(""))
    , m_sRdpHostName(wxT(""))
    , m_sRdpPassword(wxT(""))
    , m_sRdpUsername(wxT(""))
    , m_sServerHost(wxT(""))
    , m_sSshKey(wxT(""))
    , m_sUsername(wxT(""))
    , m_sVncHostName(wxT(""))
    , m_sVncPassword(wxT(""))

    , m_pMd5Password(NULL)
    , m_pClrPassword(NULL)
{
    wxString tmp;
    int itmp;

    {
        wxLogNull dummy; 
        wxFile *f = new wxFile(filename);
        if ((!f->IsOpened()) || f->Eof()) {
            delete f;
            return;
        }
        delete f;
    }
    ::wxLogTrace(MYTRACETAG, wxT("Reading %s"), filename.c_str());
    wxXmlDocument cfg(filename);
    wxXmlNode *cfgnode = cfg.GetRoot();
    if (cfgnode && (cfgnode->GetName() == wxT("NXClientSettings"))) {
        if (!(cfgnode->GetPropVal(wxT("application"), wxT("")) == wxT("nxclient")))
            return;
        if (!(cfgnode->GetPropVal(wxT("version"), wxT("")) == wxT("1.3")))
            return;
        cfgnode = cfgnode->GetChildren();
        while (cfgnode) {
            if (cfgnode->GetName() == wxT("group")) {

                // Tab "Advanced"
                if (cfgnode->GetPropVal(wxT("name"), wxT("")) == wxT("Advanced")) {
                    wxXmlNode *opt = cfgnode->GetChildren();
                    while (opt) {

                        itmp = getLong(opt, wxT("Cache size"), -1);
                        switch (itmp) {
                            case 0:
                                m_eCacheMemory = CACHEMEM_0MB;
                                break;
                            case 1:
                                m_eCacheMemory = CACHEMEM_1MB;
                                break;
                            case 2:
                                m_eCacheMemory = CACHEMEM_2MB;
                                break;
                            case 4:
                                m_eCacheMemory = CACHEMEM_4MB;
                                break;
                            case 8:
                                m_eCacheMemory = CACHEMEM_8MB;
                                break;
                            case 16:
                                m_eCacheMemory = CACHEMEM_16MB;
                                break;
                            case 32:
                                m_eCacheMemory = CACHEMEM_32MB;
                                break;
                        }
                        itmp = getLong(opt, wxT("Cache size on disk"), -1);
                        switch (itmp) {
                            case 0:
                                m_eCacheDisk = CACHEDISK_0MB;
                                break;
                            case 4:
                                m_eCacheDisk = CACHEDISK_4MB;
                                break;
                            case 8:
                                m_eCacheDisk = CACHEDISK_8MB;
                                break;
                            case 16:
                                m_eCacheDisk = CACHEDISK_16MB;
                                break;
                            case 32:
                                m_eCacheDisk = CACHEDISK_32MB;
                                break;
                            case 64:
                                m_eCacheDisk = CACHEDISK_64MB;
                                break;
                            case 128:
                                m_eCacheDisk = CACHEDISK_128MB;
                                break;
                        }
#if 0
                        if (wxGetEnv(wxT("THINCLIENT"), NULL))
                            m_eCacheDisk = CACHEDISK_0MB;
#endif
                        m_bKbdLayoutOther = !getBool(opt, wxT("Current keyboard"),
                                !m_bKbdLayoutOther);
                        itmp = getLong(opt, wxT("Custom keyboard layout"), -1);
                        if (itmp != -1)
                            m_iKbdLayoutLanguage = mapNxLanguage(itmp);
                        m_bDisableTcpNoDelay = getBool(opt, wxT("Disable TCP no-delay"),
                                m_bDisableTcpNoDelay);
                        m_bDisableZlibCompression = getBool(opt,
                                wxT("Disable ZLIB stream compression"),
                                m_bDisableZlibCompression);
                        m_bUseProxy = !getBool(opt, wxT("Enable HTTP proxy"), !m_bUseProxy);
                        m_bEnableSSL = getBool(opt, wxT("Enable SSL encryption"), m_bEnableSSL);
// Enable response time optimizations false
                        m_sProxyHost = getString(opt, wxT("HTTP Proxy host"), m_sProxyHost);
                        m_iProxyPort = getLong(opt, wxT("HTTP Proxy port"), m_iProxyPort);
// Restore cache true
// StreamCompression ""

                        opt = opt->GetNext();
                    }
                    cfgnode = cfgnode->GetNext();
                    continue;
                }

                // Tab "Environment"
                if (cfgnode->GetPropVal(wxT("name"), wxT("")) == wxT("Environment")) {
                    wxXmlNode *opt = cfgnode->GetChildren();
                    while (opt) {
                        m_sCupsPath = getString(opt, wxT("CUPSD path"), m_sCupsPath);
                        opt = opt->GetNext();
                    }
                    cfgnode = cfgnode->GetNext();
                    continue;
                }

                // Tab "General"
                if (cfgnode->GetPropVal(wxT("name"), wxT("")) == wxT("General")) {
                    wxXmlNode *opt = cfgnode->GetChildren();
                    while (opt) {

// Automatic reconnect true
                        m_sCommandLine = getString(opt, wxT("Command line"), m_sCommandLine);
// Custom Unix Desktop "application"
                        tmp = getString(opt, wxT("Desktop"));
                        if (tmp.CmpNoCase(wxT("RDP")) == 0)
                            m_eDesktopType = DTYPE_RDP;
                        if (tmp.CmpNoCase(wxT("RFB")) == 0)
                            m_eDesktopType = DTYPE_RFB;
                        if (tmp.CmpNoCase(wxT("KDE")) == 0)
                            m_eDesktopType = DTYPE_KDE;
                        if (tmp.CmpNoCase(wxT("Gnome")) == 0)
                            m_eDesktopType = DTYPE_GNOME;
                        if (tmp.CmpNoCase(wxT("CDE")) == 0)
                            m_eDesktopType = DTYPE_CDE;
                        if (tmp.CmpNoCase(wxT("XFCE")) == 0)
                            m_eDesktopType = DTYPE_XFCE;
                        if (tmp.CmpNoCase(wxT("Console")) == 0)
                            m_eDesktopType = DTYPE_CUSTOM;
                        m_bDisableShmem = getBool(opt, wxT("Disable SHM"), m_bDisableShmem);
                        m_bDisableShpix = getBool(opt, wxT("Disable emulate shared pixmaps"), m_bDisableShpix);
                        tmp = getString(opt, wxT("Link speed"));
                        if (tmp == wxT("modem"))
                            m_eConnectionSpeed = SPEED_MODEM;
                        if (tmp == wxT("isdn"))
                            m_eConnectionSpeed = SPEED_ISDN;
                        if (tmp == wxT("adsl"))
                            m_eConnectionSpeed = SPEED_ADSL;
                        if (tmp == wxT("wan"))
                            m_eConnectionSpeed = SPEED_WAN;
                        if (tmp == wxT("lan"))
                            m_eConnectionSpeed = SPEED_LAN;

                        m_bRememberPassword = getBool(opt, wxT("Remember password"),
                                m_bRememberPassword);

                        tmp = getString(opt, wxT("Resolution"));
                        if (!tmp.IsEmpty())
                            m_eDisplayType = DPTYPE_CUSTOM;
                        if (tmp == wxT("640x480"))
                            m_eDisplayType = DPTYPE_640x480;
                        if (tmp == wxT("800x600"))
                            m_eDisplayType = DPTYPE_800x600;
                        if (tmp == wxT("1024x768"))
                            m_eDisplayType = DPTYPE_1024x768;
                        if (tmp == wxT("available"))
                            m_eDisplayType = DPTYPE_AVAILABLE;
                        if (tmp == wxT("fullscreen"))
                            m_eDisplayType = DPTYPE_FULLSCREEN;

                        m_iDisplayHeight = getLong(opt, wxT("Resolution height"),
                                m_iDisplayHeight);
                        m_iDisplayWidth = getLong(opt, wxT("Resolution width"),
                                m_iDisplayWidth);
                        m_sServerHost = getString(opt, wxT("Server host"), m_sServerHost);
                        m_iServerPort = getLong(opt, wxT("Server port"), m_iServerPort);

                        tmp = getString(opt, wxT("Session"));
                        if (tmp.CmpNoCase(wxT("unix")) == 0) {
                            m_eSessionType = STYPE_UNIX;
                            m_bValid = true;
                        }
                        if (tmp.CmpNoCase(wxT("windows")) == 0) {
                            m_eSessionType = STYPE_WINDOWS;
                            m_bValid = true;
                        }
                        if (tmp.CmpNoCase(wxT("vnc")) == 0) {
                            m_eSessionType = STYPE_VNC;
                            m_bValid = true;
                        }
                        // this is 0/1 in NX but getBool can handle that
                        m_bUseCustomImageEncoding = getBool(opt,
                                wxT("Use default image encoding"), m_bUseCustomImageEncoding);
                        m_bDisableRender = !getBool(opt, wxT("Use render"), !m_bDisableRender);
                        m_bUseSmartCard = getBool(opt, wxT("Use smartcard"), m_bUseSmartCard);
                        m_bDisableTaint = !getBool(opt, wxT("Use taint"), !m_bDisableTaint);
                        m_bVirtualDesktop = getBool(opt, wxT("Virtual desktop"),
                                m_bVirtualDesktop);
                        m_bDisableXagent = !getBool(opt, wxT("XAgent encoding"),
                                !m_bDisableXagent);
// displaySaveOnExit  = true
// xdm broadcast port = 177
// xdm mode = "server_decide"
// xdm query host = "localhost"
// xdm query port = 177
                        m_bRunConsole = getBool(opt, wxT("Only console"), m_bRunConsole);
                        m_bRunXclients = getBool(opt, wxT("Run default script"), m_bRunXclients);

                        opt = opt->GetNext();
                    }
                    cfgnode = cfgnode->GetNext();
                    continue;
                }

                // Sub-Dialog Custom image compresion
                if (cfgnode->GetPropVal(wxT("name"), wxT("")) == wxT("Images")) {
                    wxXmlNode *opt = cfgnode->GetChildren();
                    while (opt) {
                        m_bDisableJpeg = getLongBool(opt, wxT("Disable JPEG Compression"),
                                m_bDisableJpeg);
                        m_bDisableImageCompression = getBool(opt,
                                wxT("Disable all image optimisations"),
                                m_bDisableImageCompression);
                        m_bDisableBackingstore = getBool(opt, wxT("Disable backingstore"),
                                m_bDisableBackingstore);
                        m_bDisableComposite = getBool(opt, wxT("Disable coposite"),
                                m_bDisableComposite);
                        m_bDisableStreaming = getBool(opt, wxT("Disable image streaming"),
                                m_bDisableStreaming);

                        itmp = getLong(opt, wxT("Image Compression Type"), -1);
                        switch (itmp) {
                            case 0:
                                m_bImageEncodingJpeg = true;
                                m_bUseJpegQuality = false;
                                m_bImageEncodingPlainX = false;
                                break;
                            case 1:
                                m_bImageEncodingJpeg = true;
                                m_bUseJpegQuality = true;
                                m_bImageEncodingPlainX = false;
                                break;
                            case 2:
                                m_bImageEncodingJpeg = false;
                                m_bUseJpegQuality = false;
                        }
                        m_bUseTightJpeg = getBool(opt, wxT("Image JPEG Encoding"),
                                m_bUseTightJpeg);
                        m_iJpegQuality = getLong(opt, wxT("JPEG Quality"), m_iJpegQuality);
// RDP optimization for low-bandwidth link = false
// Reduce colors to = ""
                        m_bImageEncodingPNG = getBool(opt, wxT("Use PNG Compression"),
                                m_bImageEncodingPNG);
                        m_iVncImageEncoding = getLong(opt, wxT("VNC images compression"),
                                m_iVncImageEncoding);
                        m_iRdpImageCompression = getLong(opt, wxT("Windows Image compression"),
                                m_iRdpImageCompression);

                        opt = opt->GetNext();
                        if (!opt)
                            m_bImageEncodingPlainX = !m_bImageEncodingPNG;
                    }
                    cfgnode = cfgnode->GetNext();
                    continue;
                }

                // Main login params
                if (cfgnode->GetPropVal(wxT("name"), wxT("")) == wxT("Login")) {
                    wxXmlNode *opt = cfgnode->GetChildren();
                    while (opt) {
                        m_pMd5Password = getStringNew(opt, wxT("Auth"), m_pMd5Password);
                        m_bGuestMode = getBool(opt, wxT("Guest Mode"), m_bGuestMode);
                        m_sGuestPassword = getString(opt, wxT("Guest password"),
                                m_sGuestPassword);
                        m_sGuestUser = getString(opt, wxT("Guest username"), m_sGuestUser);
// Login method = "nx"
                        tmp = m_sSshKey;
                        m_sSshKey = getString(opt, wxT("Public Key"), m_sSshKey);
                        if (tmp != m_sSshKey) {
                            if (!m_sSshKey.IsEmpty()) {
                                wxRegEx r(wxT("^(-----BEGIN .SA PRIVATE KEY-----)\\s+(.*)(-----END .SA PRIVATE KEY-----)\\s+$"), wxRE_ADVANCED);
                                if (r.Matches(m_sSshKey)) {
                                    tmp = r.GetMatch(m_sSshKey, 2);
                                    tmp.Replace(wxT(" "), wxT("\n"));
                                    m_sSshKey = r.GetMatch(m_sSshKey, 1) + wxT("\n")
                                        + tmp + r.GetMatch(m_sSshKey, 3) + wxT("\n");
                                }
                            }
                        }
                        m_sUsername = getString(opt, wxT("User"), m_sUsername);

                        m_pClrPassword = getStringNew(opt, wxT("Password"), m_pClrPassword);
                        opt = opt->GetNext();
                    }
                    cfgnode = cfgnode->GetNext();
                    continue;
                }

                // Tab "Services"
                if (cfgnode->GetPropVal(wxT("name"), wxT("")) == wxT("Services")) {
                    wxXmlNode *opt = cfgnode->GetChildren();
                    while (opt) {
                        m_bEnableMultimedia = getBool(opt, wxT("Audio"), m_bEnableMultimedia);
                        m_iCupsPort = getLong(opt, wxT("IPPPort"), m_iCupsPort);
                        m_bUseCups = getBool(opt, wxT("IPPPrinting"), m_bUseCups);
                        m_bEnableSmbSharing = getBool(opt, wxT("Shares"), m_bEnableSmbSharing);
                        opt = opt->GetNext();
                    }
                    cfgnode = cfgnode->GetNext();
                    continue;
                }

                if (cfgnode->GetPropVal(wxT("name"), wxT("")) == wxT("VNC Session")) {
                    wxXmlNode *opt = cfgnode->GetChildren();
                    while (opt) {
                        tmp = getString(opt, wxT("Display"));
                        if (tmp.Left(1) == wxT(":")) {
                            tmp = tmp.Mid(1);
                            if (tmp.IsNumber()) {
                                long l;
                                tmp.ToLong(&l, 0);
                                m_iVncDisplayNumber = l;
                            }
                        }
                        m_sVncPassword = getPassword(opt, wxT("Password"), m_sVncPassword);
                        m_bVncRememberPassword = getBool(opt, wxT("Remember"),
                                m_bVncRememberPassword);
                        m_sVncHostName = getString(opt, wxT("Server"), m_sVncHostName);
                        opt = opt->GetNext();
                    }
                    cfgnode = cfgnode->GetNext();
                    continue;
                }

                if (cfgnode->GetPropVal(wxT("name"), wxT("")) == wxT("Windows Session")) {
                    wxXmlNode *opt = cfgnode->GetChildren();
                    while (opt) {
                        m_sRdpApplication = getString(opt, wxT("Application"),
                                m_sRdpApplication);
                        m_iRdpAuthType = getLong(opt, wxT("Authentication"), m_iRdpAuthType);
// Color Depth = 8
// Domain = ""
// Image Cache = true
                        m_sRdpPassword = getPassword(opt, wxT("Password"), m_sRdpPassword);
                        m_bRdpRememberPassword = getBool(opt, wxT("Remember"),
                                m_bRdpRememberPassword);
                        m_bRdpRunApplication = getBool(opt, wxT("Run application"),
                                m_bRdpRunApplication);
                        m_sRdpHostName = getString(opt, wxT("Server"), m_sRdpHostName);
                        m_sRdpUsername = getString(opt, wxT("User"), m_sRdpUsername);
                        opt = opt->GetNext();
                    }
                    cfgnode = cfgnode->GetNext();
                    continue;
                }

                if (cfgnode->GetPropVal(wxT("name"), wxT("")) == wxT("share chosen")) {
                    wxXmlNode *opt = cfgnode->GetChildren();
                    while (opt) {
                        wxString key = opt->GetPropVal(wxT("key"), wxT(""));
                        if (key == wxT("Share number")) {
                            m_iUsedShareGroups = getLong(opt, wxT("Share number"),
                                    m_iUsedShareGroups);
                        } else if (key == wxT("default printer")) {
                            /// ?
                        } else
                            m_aUsedShareGroups.Add(opt->GetPropVal(wxT("value"), wxT("")));
                        opt = opt->GetNext();
                    }
                    cfgnode = cfgnode->GetNext();
                    continue;
                }

                // When we reach here, we got an "unknown" group name. This is usually
                // a mounted share description.
                {
                    int shareOptions = 0;
                    ShareGroup s;
                    s.m_sGroupName = cfgnode->GetPropVal(wxT("name"), wxT(""));
                    s.m_iType = SharedResource::SHARE_UNKNOWN;
                    wxXmlNode *opt = cfgnode->GetChildren();
                    while (opt) {
                        wxString key = opt->GetPropVal(wxT("key"), wxT(""));
// Public = false
// Default = true
                        if (key == wxT("Alias")) {
                            shareOptions++;
                            s.m_sMountPoint = getString(opt, wxT("Alias"), wxT(""));
                        }
                        if (key == wxT("Sharename")) {
                            shareOptions++;
                            s.m_sName = getString(opt, wxT("Sharename"), wxT(""));
                        }
                        if (key == wxT("Username")) {
                            shareOptions++;
                            s.m_sUsername = getString(opt, wxT("Username"), wxT(""));
                        }
                        if (key == wxT("Password")) {
                            shareOptions++;
                            s.m_sPassword = getPassword(opt, wxT("Password"), wxT(""));
                        }
                        if (key == wxT("Type")) {
                            shareOptions++;
                            wxString tmp = getString(opt, wxT("Type"), wxT(""));
                            if (tmp == wxT("cupsprinter"))
                                s.m_iType = SharedResource::SHARE_CUPS_PRINTER;
                            if (tmp == wxT("disk"))
                                s.m_iType = SharedResource::SHARE_SMB_DISK;
                            if (tmp == wxT("drinter"))
                                s.m_iType = SharedResource::SHARE_SMB_PRINTER;
                        }
                        if (key == wxT("Mounted now")) {
                            shareOptions++;
                            s.m_bMountNow = getBool(opt, wxT("Mounted now"), false);
                        }
                        opt = opt->GetNext();
                    }
                    if (shareOptions >= 5)
                        m_aShareGroups.Add(s);
                }

            }
            cfgnode = cfgnode->GetNext();
        }
    }
    if (m_bValid) {
        m_sName = wxFileName(filename).GetName();
        m_sFileName = wxFileName(filename).GetFullPath();
        if (m_pMd5Password) {
            m_sPassword = DUMMY_MD5_PASSWORD;
        } else if (m_pClrPassword) {
            m_sPassword = DUMMY_CLR_PASSWORD;
        }
    } else {
        m_sUsername = wxT("");
        m_sPassword = wxT("");
    }
}

MyXmlConfig::~MyXmlConfig()
{
    if (saved)
        delete saved;
    if (m_pMd5Password)
        delete m_pMd5Password;
    if (m_pClrPassword)
        delete m_pClrPassword;
}

    wxString
MyXmlConfig::sGetDecryptedPassword()
{
    wxString ret;
    if ((m_sPassword == DUMMY_MD5_PASSWORD) && m_pMd5Password)
        ret = decryptString(*m_pMd5Password);
    else
        ret = m_sPassword;
    return ret;
}
    void 
MyXmlConfig::bAddOption(wxXmlNode *group, const wxString &name, const bool val)
{
    wxXmlNode *n = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("option"));
    n->AddProperty(new wxXmlProperty(wxT("key"), name, NULL));
    n->AddProperty(new wxXmlProperty(wxT("value"), val ? wxT("true") : wxT("false"), NULL));
    group->AddChild(n);
}

    void
MyXmlConfig::iAddOptionBool(wxXmlNode *group, const wxString &name, const bool val)
{
    wxXmlNode *n = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("option"));
    n->AddProperty(new wxXmlProperty(wxT("key"), name, NULL));
    n->AddProperty(new wxXmlProperty(wxT("value"), wxString::Format(wxT("%d"), val ? 1 : 0), NULL));
    group->AddChild(n);
}

    void
MyXmlConfig::iAddOption(wxXmlNode *group, const wxString &name, const long val)
{
    wxXmlNode *n = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("option"));
    n->AddProperty(new wxXmlProperty(wxT("key"), name, NULL));
    n->AddProperty(new wxXmlProperty(wxT("value"), wxString::Format(wxT("%d"), val), NULL));
    group->AddChild(n);
}

    void
MyXmlConfig::sAddOption(wxXmlNode *group, const wxString &name, const wxString &val)
{
    wxXmlNode *n = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("option"));
    n->AddProperty(new wxXmlProperty(wxT("key"), name, NULL));
    n->AddProperty(new wxXmlProperty(wxT("value"), val, NULL));
    group->AddChild(n);
}

    wxXmlNode *
MyXmlConfig::AddGroup(wxXmlNode *parent, const wxString &name)
{
    wxXmlNode *n = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("group"));
    n->AddProperty(new wxXmlProperty(wxT("name"), name, NULL));
    parent->AddChild(n);
    return n;
}

    bool
MyXmlConfig::SaveToFile()
{
    wxXmlDocument cfg;
    wxXmlNode *r;
    wxXmlNode *g;
    wxString optval;
    size_t i;

    r = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("NXClientSettings"));
    r->AddProperty(new wxXmlProperty(wxT("application"), wxT("nxclient"), NULL));
    r->AddProperty(new wxXmlProperty(wxT("version"), wxT("1.3"), NULL));
    cfg.SetRoot(r);

    g = AddGroup(r, wxT("General")); // SessionProperties, Tab "General"
    bAddOption(g, wxT("Automatic reconnect"), true); // Not (yet?) configurable
    sAddOption(g, wxT("Command line"), m_sCommandLine);
    optval = wxT("application");
    if (m_bRunConsole)
        optval = wxT("console");
    if (m_bRunXclients)
        optval = wxT("default");
    sAddOption(g, wxT("Custom Unix Desktop"), optval);
    switch (m_eDesktopType) {
        case DTYPE_RDP:
            optval = wxT("rdp");
            break;
        case DTYPE_RFB:
            optval = wxT("rfb");
            break;
        case DTYPE_KDE:
            optval = wxT("kde");
            break;
        case DTYPE_GNOME:
            optval = wxT("gnome");
            break;
        case DTYPE_CDE:
            optval = wxT("cde");
            break;
        case DTYPE_XFCE:
            optval = wxT("xfce");
            break;
        case DTYPE_CUSTOM:
            optval = wxT("console");
            break;
    }
    sAddOption(g, wxT("Desktop"), optval);
    bAddOption(g, wxT("Disable SHM"), m_bDisableShmem); // Not in original GUI but in config ?!
    bAddOption(g, wxT("Disable emulate shared pixmaps"), m_bDisableShpix); // Not in original GUI but in config ?!
    switch (m_eConnectionSpeed) {
        case SPEED_MODEM:
            optval = wxT("modem");
            break;
        case SPEED_ISDN:
            optval = wxT("isdn");
            break;
        case SPEED_ADSL:
            optval = wxT("adsl");
            break;
        case SPEED_WAN:
            optval = wxT("wan");
            break;
        case SPEED_LAN:
            optval = wxT("lan");
            break;
    }
    sAddOption(g, wxT("Link speed"), optval);
    bAddOption(g, wxT("Remember password"), m_bRememberPassword);
    switch (m_eDisplayType) {
        case DPTYPE_CUSTOM:
            optval = wxString::Format(wxT("%dx%d"),
                    m_iDisplayWidth, m_iDisplayHeight);
            break;
        case DPTYPE_640x480:
            optval = wxT("640x480");
            break;
        case DPTYPE_800x600:
            optval = wxT("800x600");
            break;
        case DPTYPE_1024x768:
            optval = wxT("1024x768");
            break;
        case DPTYPE_AVAILABLE:
            optval = wxT("available");
            break;
        case DPTYPE_FULLSCREEN:
            optval = wxT("fullscreen");
            break;
    }
    sAddOption(g, wxT("Resolution"), optval);
    iAddOption(g, wxT("Resolution height"), m_iDisplayHeight);
    iAddOption(g, wxT("Resolution width"), m_iDisplayWidth);
    sAddOption(g, wxT("Server host"), m_sServerHost);
    iAddOption(g, wxT("Server port"), m_iServerPort);
    switch (m_eSessionType) {
        case STYPE_UNIX:
            optval = wxT("unix");
            break;
        case STYPE_WINDOWS:
            optval = wxT("windows");
            break;
        case STYPE_VNC:
            optval = wxT("vnc");
            break;
    }
    sAddOption(g, wxT("Session"), optval);
    iAddOption(g, wxT("Use default image encoding"), m_bUseCustomImageEncoding);
    bAddOption(g, wxT("Use render"), !m_bDisableRender);
    bAddOption(g, wxT("Use taint"), !m_bDisableTaint);
    bAddOption(g, wxT("Virtual desktop"), m_bVirtualDesktop);
    bAddOption(g, wxT("XAgent encoding"), !m_bDisableXagent);
    bAddOption(g, wxT("displaySaveOnExit"), true); // Not in original GUI but in config ?!
    iAddOption(g, wxT("xdm broadcast port"), 177); // Not in original GUI but in config ?!
    sAddOption(g, wxT("xdm list host"), wxT("localhost")); // Not in original GUI but in config ?!
    iAddOption(g, wxT("xdm list port"), 177); // Not in original GUI but in config ?!
    sAddOption(g, wxT("xdm mode"), wxT("server_decide")); // Not in original GUI but in config ?!
    sAddOption(g, wxT("xdm query host"), wxT("localhost")); // Not in original GUI but in config ?!
    iAddOption(g, wxT("xdm query port"), 177); // Not in original GUI but in config ?!
    bAddOption(g, wxT("Use smartcard"), m_bUseSmartCard); // Not in original

    // == UnixImageSettingsDialog, RdpImageSettingsDialog, VncImageSettingsDialog
    g = AddGroup(r, wxT("Images"));
    iAddOption(g, wxT("Disable JPEG Compression"), m_bDisableJpeg ? 1 : 0);
    bAddOption(g, wxT("Disable all image optimisations"), m_bDisableImageCompression);
    bAddOption(g, wxT("Disable backingstore"), m_bDisableBackingstore);
    bAddOption(g, wxT("Disable coposite"), m_bDisableComposite);
    bAddOption(g, wxT("Disable image streaming"), m_bDisableStreaming);
    if ((m_bImageEncodingPlainX || m_bImageEncodingPNG) && (!m_bImageEncodingJpeg))
        optval = wxT("2");
    else if (m_bImageEncodingJpeg && m_bUseJpegQuality)
        optval = wxT("1");
    else
        optval = wxT("0");
    sAddOption(g, wxT("Image Compression Type"), optval);
    iAddOption(g, wxT("Image Encoding Type"), m_iVncImageEncoding);
    bAddOption(g, wxT("Image JPEG Encoding"), m_bUseTightJpeg);
    iAddOption(g, wxT("JPEG Quality"), m_iJpegQuality);
    bAddOption(g, wxT("RDP optimization for low-bandwidth link"), false); // ??
    sAddOption(g, wxT("Reduce colors to"), wxT("")); // ??
    bAddOption(g, wxT("Use PNG Compression"), m_bImageEncodingPNG);
    iAddOption(g, wxT("VNC images compression"), 0); // Not in original GUI but in config ?!
    iAddOption(g, wxT("Windows Image Compression"), m_iRdpImageCompression);

    g = AddGroup(r, wxT("Login"));
    if (m_bRememberPassword) {
        if ((m_sPassword != DUMMY_MD5_PASSWORD) &&
                (m_sPassword != DUMMY_CLR_PASSWORD)   ) {
            optval = cryptString(m_sPassword);
            sAddOption(g, wxT("Auth"), optval);
        } else {
            if (m_pMd5Password)
                sAddOption(g, wxT("Auth"), *m_pMd5Password);
            if (m_pClrPassword)
                sAddOption(g, wxT("Password"), *m_pClrPassword);
        }
    } else {
        optval = wxT("");
        sAddOption(g, wxT("Auth"), optval);
        sAddOption(g, wxT("Password"), optval);
    }
    bAddOption(g, wxT("Guest Mode"), m_bGuestMode);
    sAddOption(g, wxT("Guest password"), m_sGuestPassword);
    sAddOption(g, wxT("Guest username"), m_sGuestUser);
    sAddOption(g, wxT("Login Method"), wxT("nx"));
    sAddOption(g, wxT("Public Key"), m_sSshKey);
    sAddOption(g, wxT("User"), m_sUsername);

    g = AddGroup(r, wxT("Environment"));
    sAddOption(g, wxT("CUPSD path"), m_sCupsPath);

    g = AddGroup(r, wxT("Services"));
    bAddOption(g, wxT("Audio"), m_bEnableMultimedia);
    bAddOption(g, wxT("Shares"), m_bEnableSmbSharing);
    bAddOption(g, wxT("IPPPrinting"), m_bUseCups);
    iAddOption(g, wxT("IPPPort"), m_iCupsPort);

    if (m_aUsedShareGroups.GetCount()) {
        g = AddGroup(r, wxT("share chosen"));
        iAddOption(g, wxT("Share number"), m_iUsedShareGroups);
        for (i = 0; i < m_aUsedShareGroups.GetCount(); i++) {
            optval = wxString::Format(wxT("Share%d"), i);
            sAddOption(g, optval, m_aUsedShareGroups[i]); 
        }
    }

    g = AddGroup(r, wxT("Advanced"));
    bAddOption(g, wxT("Current keyboard"), !m_bKbdLayoutOther);
    bAddOption(g, wxT("Disable TCP no-delay"), m_bDisableTcpNoDelay);
    bAddOption(g, wxT("Disable ZLIB stream compression"), m_bDisableZlibCompression);
    bAddOption(g, wxT("Enable SSL encryption"), m_bEnableSSL);
    switch (m_eCacheMemory) {
        case CACHEMEM_1MB:
            optval = wxT("1");
            break;
        case CACHEMEM_2MB:
            optval = wxT("2");
            break;
        case CACHEMEM_4MB:
            optval = wxT("4");
            break;
        case CACHEMEM_8MB:
            optval = wxT("8");
            break;
        case CACHEMEM_16MB:
            optval = wxT("16");
            break;
        case CACHEMEM_32MB:
            optval = wxT("32");
            break;
    }
    sAddOption(g, wxT("Cache size"), optval);
    switch (m_eCacheDisk) {
        case CACHEDISK_0MB:
            optval = wxT("0");
            break;
        case CACHEDISK_4MB:
            optval = wxT("4");
            break;
        case CACHEDISK_8MB:
            optval = wxT("8");
            break;
        case CACHEDISK_16MB:
            optval = wxT("16");
            break;
        case CACHEDISK_32MB:
            optval = wxT("32");
            break;
        case CACHEDISK_64MB:
            optval = wxT("64");
            break;
        case CACHEDISK_128MB:
            optval = wxT("128");
            break;
    }
    sAddOption(g, wxT("Cache size on disk"), optval);
    iAddOption(g, wxT("Other keyboard"), mapNxLanguage(m_iKbdLayoutLanguage));

    g = AddGroup(r, wxT("Windows Session"));
    bAddOption(g, wxT("Remember"), m_bRdpRememberPassword);
    bAddOption(g, wxT("Run application"), m_bRdpRunApplication);
    iAddOption(g, wxT("Authentication"), m_iRdpAuthType);
    sAddOption(g, wxT("Server"), m_sRdpHostName);
    sAddOption(g, wxT("User"), m_sRdpUsername);
    optval = m_bRdpRememberPassword ? encodeString(m_sRdpPassword) : wxT("");
    sAddOption(g, wxT("Password"), optval);
    sAddOption(g, wxT("Application"), m_sRdpApplication);

    g = AddGroup(r, wxT("VNC Session"));
    bAddOption(g, wxT("Remember"), m_bVncRememberPassword);
    sAddOption(g, wxT("Server"), m_sVncHostName);
    optval = m_bVncRememberPassword ? encodeString(m_sVncPassword) : wxT("");
    sAddOption(g, wxT("Password"), optval);
    optval = wxString::Format(wxT(":%d"), m_iVncDisplayNumber);
    sAddOption(g, wxT("Display"), optval);

    for (i = 0; i < m_aShareGroups.GetCount(); i++) {
        g = AddGroup(r, m_aShareGroups[i].m_sGroupName);
        sAddOption(g, wxT("Alias"), m_aShareGroups[i].m_sMountPoint);
        sAddOption(g, wxT("Sharename"), m_aShareGroups[i].m_sName);
        sAddOption(g, wxT("Username"), m_aShareGroups[i].m_sUsername);
        optval = encodeString(m_aShareGroups[i].m_sPassword);
        sAddOption(g, wxT("Password"), optval);
        switch (m_aShareGroups[i].m_iType) {
            case SharedResource::SHARE_SMB_DISK:
                optval = wxT("Disk");
                sAddOption(g, wxT("Type"), optval);
                break;
            case SharedResource::SHARE_SMB_PRINTER:
                optval = wxT("Printer");
                sAddOption(g, wxT("Type"), optval);
                break;
        }
        bAddOption(g, wxT("Mounted now"), m_aShareGroups[i].m_bMountNow);
    }

    // Must write to memory first, to get rid of the standard XML
    // doctype header
    wxMemoryOutputStream mos;
    cfg.Save(mos);
    off_t len = mos.TellO();
    if (len > 0) {
        char *data = new char[len];
        mos.CopyTo(data, len);
        char *secondline = (char *)memchr(data, '\n', len);
        if (secondline++) {
            // Replace 1st line with non-standard NXclient doctype
            len -= (secondline - data);
            ::wxLogTrace(MYTRACETAG, wxT("Writing '%s'"), m_sFileName.c_str());
            wxFileOutputStream fos(m_sFileName);
            if (!fos.Ok()) {
                delete data;
                return false;
            }
            // NO wxT() here!!
            fos.Write("<!DOCTYPE NXClientSettings>\n", 28);
            if (fos.LastWrite() != 28) {
                delete data;
                return false;
            }
            // Write out rest of file
            fos.Write(secondline, len);
            if (fos.LastWrite() != (size_t)len) {
                delete data;
                return false;
            }
        } else {
            delete data;
            return false;
        }
        delete data;
    }
    return true;
}

void MyXmlConfig::saveState()
{
    if (saved)
        delete saved;
    saved = new MyXmlConfig();
    *saved = *this;
}

bool MyXmlConfig::checkChanged()
{
    if (!saved)
        return true;
    return (!(*this == *saved));
}

bool MyXmlConfig::getBool(wxXmlNode *opt, const wxString &key, bool defval)
{
    bool val = defval;

    if (opt->GetName() == wxT("option")) {
        if (opt->GetPropVal(wxT("key"), wxT("")) == key) {
            wxString tmp = opt->GetPropVal(wxT("value"), wxT(""));
            if (!tmp.IsEmpty())
                val = ((tmp == STR_TRUE) || (tmp == STR_ONE));
        }
    }
    return val;
}

long MyXmlConfig::getLong(wxXmlNode *opt, const wxString &key, long defval)
{
    long val = defval;

    if (opt->GetName() == wxT("option")) {
        if (opt->GetPropVal(wxT("key"), wxT("")) == key) {
            wxString tmp = opt->GetPropVal(wxT("value"), wxT(""));
            if (tmp.IsNumber())
                tmp.ToLong(&val, 0);
        }
    }
    return val;
}

bool MyXmlConfig::getLongBool(wxXmlNode *opt, const wxString &key, bool defval)
{
    long val = defval ? 1 : 0;

    if (opt->GetName() == wxT("option")) {
        if (opt->GetPropVal(wxT("key"), wxT("")) == key) {
            wxString tmp = opt->GetPropVal(wxT("value"), wxT(""));
            if (tmp.IsNumber())
                tmp.ToLong(&val, 0);
        }
    }
    return (val != 0);
}

wxString MyXmlConfig::getString(wxXmlNode *opt, const wxString &key, const wxString &defval)
{
    wxString tmp = defval;

    if (opt->GetName() == wxT("option")) {
        if (opt->GetPropVal(wxT("key"), wxT("")) == key)
            tmp = opt->GetPropVal(wxT("value"), defval);
    }
    return tmp;
}

wxString *MyXmlConfig::getStringNew(wxXmlNode *opt, const wxString &key, wxString *defval)
{
    wxString *val = defval;

    if (opt->GetName() == wxT("option")) {
        if (opt->GetPropVal(wxT("key"), wxT("")) == key) {
            if (opt->HasProp(wxT("value"))) {
                val = new wxString(opt->GetPropVal(wxT("value"), defval ? *defval : wxT("")));
                if (val && val->IsEmpty() && (!defval)) {
                    delete val;
                    val = NULL;
                }
                if (defval)
                    delete defval;
            }
        }
    }
    return val;
}

wxString MyXmlConfig::getPassword(wxXmlNode *opt, const wxString &key, const wxString &defval)
{
    wxString val = defval;
    wxString *enc = getStringNew(opt, key, NULL);
    if (enc && enc->Left(1) == wxT(":") && enc->Right(1) == wxT(":") && enc->Length() > 1) {
        int idx = 1;
        wxString newpw = wxT("");
        enc->Remove(0, 1);
        while (enc->Length()) {
            int p = enc->Find(wxT(":"));
            if (p != -1) {
                long l;
                enc->Left(p).ToLong(&l);
                newpw += wxChar(l - idx++);
            } else
                break;
            enc->Remove(0, p+1);
        }
        val = newpw;
    }
    if (enc)
        delete enc;
    return val;
}

int MyXmlConfig::mapLocaleToKeyboard()
{
    int ret = 0;

    switch (wxGetApp().GetLocale()->GetLanguage()) {
        case wxLANGUAGE_AFRIKAANS:
            ret = 0;
            break;
        case wxLANGUAGE_ALBANIAN:
            ret = 1;
            break;
        case wxLANGUAGE_ARABIC:
        case wxLANGUAGE_ARABIC_ALGERIA:
        case wxLANGUAGE_ARABIC_BAHRAIN:
        case wxLANGUAGE_ARABIC_EGYPT:
        case wxLANGUAGE_ARABIC_IRAQ:
        case wxLANGUAGE_ARABIC_JORDAN:
        case wxLANGUAGE_ARABIC_KUWAIT:
        case wxLANGUAGE_ARABIC_LEBANON:
        case wxLANGUAGE_ARABIC_LIBYA:
        case wxLANGUAGE_ARABIC_MOROCCO:
        case wxLANGUAGE_ARABIC_OMAN:
        case wxLANGUAGE_ARABIC_QATAR:
        case wxLANGUAGE_ARABIC_SAUDI_ARABIA:
        case wxLANGUAGE_ARABIC_SUDAN:
        case wxLANGUAGE_ARABIC_SYRIA:
        case wxLANGUAGE_ARABIC_TUNISIA:
        case wxLANGUAGE_ARABIC_UAE:
        case wxLANGUAGE_ARABIC_YEMEN:
            ret = 2;
            break;
        case wxLANGUAGE_BASQUE:
            ret = 3;
            break;
        case wxLANGUAGE_BELARUSIAN:
            ret = 4;
            break;
        case wxLANGUAGE_BULGARIAN:
            ret = 5;
            break;
        case wxLANGUAGE_CATALAN:
            ret = 6;
            break;
        case wxLANGUAGE_CHINESE:
        case wxLANGUAGE_CHINESE_SIMPLIFIED:
        case wxLANGUAGE_CHINESE_TRADITIONAL:
        case wxLANGUAGE_CHINESE_HONGKONG:
        case wxLANGUAGE_CHINESE_MACAU:
        case wxLANGUAGE_CHINESE_SINGAPORE:
        case wxLANGUAGE_CHINESE_TAIWAN:
            ret = 7;
            break;
        case wxLANGUAGE_CROATIAN:
            ret = 8;
            break;
        case wxLANGUAGE_CZECH:
            ret = 9;
            break;
        case wxLANGUAGE_DANISH:
            ret = 10;
            break;
        case wxLANGUAGE_DUTCH:
        case wxLANGUAGE_DUTCH_BELGIAN:
            ret = 11;
            break;
        case wxLANGUAGE_ENGLISH_UK:
        case wxLANGUAGE_ENGLISH_AUSTRALIA:
        case wxLANGUAGE_ENGLISH_BELIZE:
        case wxLANGUAGE_ENGLISH_BOTSWANA:
        case wxLANGUAGE_ENGLISH_DENMARK:
        case wxLANGUAGE_ENGLISH_EIRE:
        case wxLANGUAGE_ENGLISH_NEW_ZEALAND:
        case wxLANGUAGE_ENGLISH_PHILIPPINES:
        case wxLANGUAGE_ENGLISH_SOUTH_AFRICA:
        case wxLANGUAGE_ENGLISH_ZIMBABWE:
            ret = 12;
            break;
        default:
        case wxLANGUAGE_ENGLISH:
        case wxLANGUAGE_ENGLISH_TRINIDAD:
        case wxLANGUAGE_ENGLISH_US:
        case wxLANGUAGE_ENGLISH_CANADA:
        case wxLANGUAGE_ENGLISH_CARIBBEAN:
        case wxLANGUAGE_ENGLISH_JAMAICA:
            ret = 13;
            break;
        case wxLANGUAGE_ESTONIAN:
            ret = 14;
            break;
        case wxLANGUAGE_FAEROESE:
            ret = 15;
            break;
        case wxLANGUAGE_FARSI:
            ret = 16;
            break;
        case wxLANGUAGE_FINNISH:
            ret = 17;
            break;
        case wxLANGUAGE_FRENCH:
        case wxLANGUAGE_FRENCH_BELGIAN:
        case wxLANGUAGE_FRENCH_CANADIAN:
        case wxLANGUAGE_FRENCH_LUXEMBOURG:
        case wxLANGUAGE_FRENCH_MONACO:
        case wxLANGUAGE_FRENCH_SWISS:
            ret = 18;
            break;
        case wxLANGUAGE_GERMAN:
        case wxLANGUAGE_GERMAN_AUSTRIAN:
        case wxLANGUAGE_GERMAN_BELGIUM:
        case wxLANGUAGE_GERMAN_LIECHTENSTEIN:
        case wxLANGUAGE_GERMAN_LUXEMBOURG:
        case wxLANGUAGE_GERMAN_SWISS:
            ret = 19;
            break;
        case wxLANGUAGE_GREEK:
            ret = 20;
            break;
        case wxLANGUAGE_HEBREW:
            ret = 21;
            break;
        case wxLANGUAGE_HUNGARIAN:
            ret = 22;
            break;
        case wxLANGUAGE_ICELANDIC:
            ret = 23;
            break;
        case wxLANGUAGE_INDONESIAN:
            ret = 24;
            break;
        case wxLANGUAGE_ITALIAN:
        case wxLANGUAGE_ITALIAN_SWISS:
            ret = 25;
            break;
        case wxLANGUAGE_JAPANESE:
            ret = 26;
            break;
        case wxLANGUAGE_KOREAN:
            ret = 27;
            break;
        case wxLANGUAGE_LATVIAN:
            ret = 28;
            break;
        case wxLANGUAGE_LITHUANIAN:
            ret = 29;
            break;
        case wxLANGUAGE_NORWEGIAN_BOKMAL:
        case wxLANGUAGE_NORWEGIAN_NYNORSK:
            ret = 30;
            break;
        case wxLANGUAGE_POLISH:
            ret = 31;
            break;
        case wxLANGUAGE_PORTUGUESE:
        case wxLANGUAGE_PORTUGUESE_BRAZILIAN:
            ret = 32;
            break;
        case wxLANGUAGE_ROMANIAN:
            ret = 33;
            break;
        case wxLANGUAGE_RUSSIAN:
        case wxLANGUAGE_RUSSIAN_UKRAINE:
            ret = 34;
            break;
        case wxLANGUAGE_SLOVAK:
            ret = 35;
            break;
        case wxLANGUAGE_SLOVENIAN:
            ret = 36;
        case wxLANGUAGE_SPANISH:
        case wxLANGUAGE_SPANISH_ARGENTINA:
        case wxLANGUAGE_SPANISH_BOLIVIA:
        case wxLANGUAGE_SPANISH_CHILE:
        case wxLANGUAGE_SPANISH_COLOMBIA:
        case wxLANGUAGE_SPANISH_COSTA_RICA:
        case wxLANGUAGE_SPANISH_DOMINICAN_REPUBLIC:
        case wxLANGUAGE_SPANISH_ECUADOR:
        case wxLANGUAGE_SPANISH_EL_SALVADOR:
        case wxLANGUAGE_SPANISH_GUATEMALA:
        case wxLANGUAGE_SPANISH_HONDURAS:
        case wxLANGUAGE_SPANISH_MEXICAN:
        case wxLANGUAGE_SPANISH_MODERN:
        case wxLANGUAGE_SPANISH_NICARAGUA:
        case wxLANGUAGE_SPANISH_PANAMA:
        case wxLANGUAGE_SPANISH_PARAGUAY:
        case wxLANGUAGE_SPANISH_PERU:
        case wxLANGUAGE_SPANISH_PUERTO_RICO:
        case wxLANGUAGE_SPANISH_URUGUAY:
        case wxLANGUAGE_SPANISH_US:
        case wxLANGUAGE_SPANISH_VENEZUELA:
            ret = 37;
            break;
        case wxLANGUAGE_SWEDISH:
        case wxLANGUAGE_SWEDISH_FINLAND:
            ret = 38;
            break;
        case wxLANGUAGE_THAI:
            ret = 39;
            break;
        case wxLANGUAGE_TURKISH:
            ret = 40;
            break;
        case wxLANGUAGE_VIETNAMESE:
            ret = 41;
            break;

    }
    return ret;
}

// Map keyboard language code from NXclient to OpenNX
    int
MyXmlConfig::mapMyLanguage(int origNXlang)
{
    switch (origNXlang) {
        case 0:
            return 25;
            break;
        case 1:
            return 19;
            break;
        case 41:
            return 12;
            break;
        default:
            if ((origNXlang >= 2) && (origNXlang <= 13))
                return origNXlang - 2;
            if ((origNXlang >= 14) && (origNXlang <= 19))
                return origNXlang - 1;
            if ((origNXlang >= 20) && (origNXlang <= 24))
                return origNXlang;
            if ((origNXlang >= 25) && (origNXlang <= 40))
                return origNXlang + 1;
            break;
    }
    return 0;
}

// Map keyboard language iso code
    wxString 
MyXmlConfig::isoKbd(int lang)
{
    const wxChar *iso_codes[] =
    {
        wxT("af"), wxT("sq"), wxT("ar"), wxT("eu"), wxT(""), wxT("bg"), wxT("ca"), wxT("zh"),
        wxT("hr"), wxT("cs"), wxT("da"), wxT("nl"), wxT("en"), wxT("en_US"), wxT("et"), wxT("fo"),
        wxT("fa"), wxT("fi"), wxT("fr"), wxT("de"), wxT("el"), wxT("he"), wxT("hu"), wxT("is"),
        wxT("id"), wxT("it"), wxT("ja"), wxT("ko"), wxT("lv"), wxT("lt"), wxT("no"), wxT("pl"),
        wxT("pt"), wxT("ro"), wxT("ru"), wxT("sk"), wxT("sl"), wxT("es"), wxT("sv"), wxT("th"),
        wxT("tr"), wxT("vi")
    };
    wxString ret;
    if ((lang >= 0) && (lang <= 41))
        ret = iso_codes[lang];
            return ret;
}

// Map keyboard language code from OpenNX to NXclient
    int 
MyXmlConfig::mapNxLanguage(int origNXlang)
{
    switch (origNXlang) {
        case 25:
            return 0;
            break;
        case 19:
            return 1;
            break;
        case 12:
            return 41;
            break;
        default:
            if (origNXlang < 12)
                return origNXlang + 2;
            if ((origNXlang >= 13) && (origNXlang <= 18))
                return origNXlang + 1;
            if ((origNXlang >= 20) && (origNXlang <= 24))
                return origNXlang;
            if ((origNXlang > 25) && (origNXlang <= 40))
                return origNXlang - 1;
            break;
    }
    return 0;
}
