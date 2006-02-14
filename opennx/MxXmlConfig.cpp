// $Id: MxXmlConfig.cpp 888 2004-11-09 15:16:50Z felfert $
//
//////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma implementation "MxXmlConfig.h"
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

class wxConfigBase;

#include "MxXmlConfig.h"
#include "WinShare.h"
#include "mxclientApp.h"
#include "pwcrypt.h"

#define STR_TRUE _T("true")
#define STR_FALSE _T("false");
#define STR_ONE _T("1")
#define STR_ZERO _T("0")

// We use something the user cannot type here!
#define DUMMY_MD5_PASSWORD _T("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b")
#define DUMMY_CLR_PASSWORD _T("\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r")

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

MxXmlConfig::MxXmlConfig()
    : m_bValid(false)
    , saved(NULL)
    , m_bRememberPassword(false)
    , m_bUseCustomImageEncoding(false)
    , m_bUseSmartCard(false)
    , m_bDisableTcpNoDelay(false)
    , m_bDisableZlibCompression(false)
    , m_bEnableSSL(true)
    , m_bKbdLayoutOther(false)
    , m_bEnableSmbSharing(false)
    , m_bEnableMultimedia(false)
    , m_bRemoveOldSessionFiles(true)
    , m_bUseFontServer(false)
    , m_bRunConsole(false)
    , m_bRunXclients(false)
    , m_bVirtualDesktop(false)
    , m_bDisableTaint(false)
    , m_bRdpRememberPassword(false)
    , m_bRdpRunApplication(false)
    , m_bVncRememberPassword(false)
    , m_bVncUseMxAuth(false)
    , m_bUsePNG(false)
    , m_bUsePlainX(false)
    , m_bUseJpeg(true)
    , m_bUseJpegQuality(false)
    , m_bDisableRender(false)
    , m_bUseTightJpeg(false)
    , m_iDisplayHeight(480)
    , m_iDisplayWidth(640)
    , m_iServerPort(22)
    , m_iFontServerPort(7100)
    , m_iRdpAuthType(1)
    , m_iVncDisplayNumber(0)
    , m_iRdpImageCompression(1)
    , m_iVncImageEncoding(0)
    , m_iUsedShareGroups(0)
    , m_iJpegQuality(6)
    , m_eConnectionSpeed(SPEED_ADSL)
    , m_eDesktopType(DTYPE_KDE)
    , m_eDisplayType(DPTYPE_AVAILABLE)
    , m_eSessionType(STYPE_UNIX)
    , m_eCacheMemory(CACHEMEM_8MB)
    , m_eCacheDisk(CACHEDISK_32MB)
    , m_sName(_T(""))
    , m_sUsername(_T(""))
    , m_sPassword(_T(""))
    , m_sBackingstore(_T(""))
    , m_sServerHost(_T(""))
    , m_sCommandLine(_T(""))
    , m_sRdpHostName(_T(""))
    , m_sRdpUsername(_T(""))
    , m_sRdpPassword(_T(""))
    , m_sRdpApplication(_T(""))
    , m_sVncHostName(_T(""))
    , m_sVncPassword(_T(""))
    , m_sFontServerHost(_T(""))
    , m_sFileName(_T(""))
    , m_pMd5Password(NULL)
    , m_pClrPassword(NULL)
{
    m_iKbdLayoutLanguage = mapLocaleToKeyboard();
}

MxXmlConfig &
MxXmlConfig::operator =(const MxXmlConfig &other)
{
    m_bValid = other.m_bValid;
    saved = NULL;
    m_bRememberPassword = other.m_bRememberPassword;
    m_bUseCustomImageEncoding = other.m_bUseCustomImageEncoding;
    m_bUseSmartCard = other.m_bUseSmartCard;
    m_bDisableTcpNoDelay = other.m_bDisableTcpNoDelay;
    m_bDisableZlibCompression = other.m_bDisableZlibCompression;
    m_bEnableSSL = other.m_bEnableSSL;
    m_bKbdLayoutOther = other.m_bKbdLayoutOther;
    m_bEnableSmbSharing = other.m_bEnableSmbSharing;
    m_bEnableMultimedia = other.m_bEnableMultimedia;
    m_bRemoveOldSessionFiles = other.m_bRemoveOldSessionFiles;
    m_bUseFontServer = other.m_bUseFontServer;
    m_bRunConsole = other.m_bRunConsole;
    m_bRunXclients = other.m_bRunXclients;
    m_bVirtualDesktop = other.m_bVirtualDesktop;
    m_bDisableTaint = other.m_bDisableTaint;
    m_bRdpRememberPassword = other.m_bRdpRememberPassword;
    m_bRdpRunApplication = other.m_bRdpRunApplication;
    m_bVncRememberPassword = other.m_bVncRememberPassword;
    m_bVncUseMxAuth = other.m_bVncUseMxAuth;
    m_bUsePNG = other.m_bUsePNG;
    m_bUsePlainX = other.m_bUsePlainX;
    m_bUseJpeg = other.m_bUseJpeg;
    m_bUseJpegQuality = other.m_bUseJpegQuality;
    m_bDisableRender = other.m_bDisableRender;
    m_bUseTightJpeg = other.m_bUseTightJpeg;
    m_iDisplayHeight = other.m_iDisplayHeight;
    m_iDisplayWidth = other.m_iDisplayWidth;
    m_iServerPort = other.m_iServerPort;
    m_iFontServerPort = other.m_iFontServerPort;
    m_iRdpAuthType = other.m_iRdpAuthType;
    m_iVncDisplayNumber = other.m_iVncDisplayNumber;
    m_iRdpImageCompression = other.m_iRdpImageCompression;
    m_iVncImageEncoding = other.m_iVncImageEncoding;
    m_iUsedShareGroups = other.m_iUsedShareGroups;
    m_iKbdLayoutLanguage = other.m_iKbdLayoutLanguage;
    m_iJpegQuality = other.m_iJpegQuality;
    m_eConnectionSpeed = other.m_eConnectionSpeed;
    m_eDesktopType = other.m_eDesktopType;
    m_eDisplayType = other.m_eDisplayType;
    m_eSessionType = other.m_eSessionType;
    m_eCacheMemory = other.m_eCacheMemory;
    m_eCacheDisk = other.m_eCacheDisk;
    m_sName = other.m_sName;
    m_sUsername = other.m_sUsername;
    m_sPassword = other.m_sPassword;
    m_sBackingstore = other.m_sBackingstore;
    m_sServerHost = other.m_sServerHost;
    m_sCommandLine = other.m_sCommandLine;
    m_sRdpHostName = other.m_sRdpHostName;
    m_sRdpUsername = other.m_sRdpUsername;
    m_sRdpPassword = other.m_sRdpPassword;
    m_sRdpApplication = other.m_sRdpApplication;
    m_sVncHostName = other.m_sVncHostName;
    m_sVncPassword = other.m_sVncPassword;
    m_sFontServerHost = other.m_sFontServerHost;
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

// Compare two arrays of ShareGroup where sequence of elements is irrelevant
// return true, if arrays are different.
bool
MxXmlConfig::cmpShareGroups(ArrayOfShareGroups a1, ArrayOfShareGroups a2)
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
MxXmlConfig::cmpUsedShareGroups(wxArrayString a1, wxArrayString a2)
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
MxXmlConfig::operator ==(const MxXmlConfig &other)
{
    if (m_bValid != other.m_bValid) return false;
    if (m_bRememberPassword != other.m_bRememberPassword) return false;
    if (m_bUseCustomImageEncoding != other.m_bUseCustomImageEncoding) return false;
    if (m_bUseSmartCard != other.m_bUseSmartCard) return false;
    if (m_bDisableTcpNoDelay != other.m_bDisableTcpNoDelay) return false;
    if (m_bDisableZlibCompression != other.m_bDisableZlibCompression) return false;
    if (m_bEnableSSL != other.m_bEnableSSL) return false;
    if (m_bKbdLayoutOther != other.m_bKbdLayoutOther) return false;
    if (m_bEnableSmbSharing != other.m_bEnableSmbSharing) return false;
    if (m_bEnableMultimedia != other.m_bEnableMultimedia) return false;
    if (m_bRemoveOldSessionFiles != other.m_bRemoveOldSessionFiles) return false;
    if (m_bUseFontServer != other.m_bUseFontServer) return false;
    if (m_bRunConsole != other.m_bRunConsole) return false;
    if (m_bRunXclients != other.m_bRunXclients) return false;
    if (m_bVirtualDesktop != other.m_bVirtualDesktop) return false;
    if (m_bDisableTaint != other.m_bDisableTaint) return false;
    if (m_bRdpRememberPassword != other.m_bRdpRememberPassword) return false;
    if (m_bRdpRunApplication != other.m_bRdpRunApplication) return false;
    if (m_bVncRememberPassword != other.m_bVncRememberPassword) return false;
    if (m_bVncUseMxAuth != other.m_bVncUseMxAuth) return false;
    if (m_bUsePNG != other.m_bUsePNG) return false;
    if (m_bUsePlainX != other.m_bUsePlainX) return false;
    if (m_bUseJpeg != other.m_bUseJpeg) return false;
    if (m_bUseJpegQuality != other.m_bUseJpegQuality) return false;
    if (m_bDisableRender != other.m_bDisableRender) return false;
    if (m_bUseTightJpeg != other.m_bUseTightJpeg) return false;
    if (m_iDisplayHeight != other.m_iDisplayHeight) return false;
    if (m_iDisplayWidth != other.m_iDisplayWidth) return false;
    if (m_iServerPort != other.m_iServerPort) return false;
    if (m_iFontServerPort != other.m_iFontServerPort) return false;
    if (m_iRdpAuthType != other.m_iRdpAuthType) return false;
    if (m_iVncDisplayNumber != other.m_iVncDisplayNumber) return false;
    if (m_iRdpImageCompression != other.m_iRdpImageCompression) return false;
    if (m_iVncImageEncoding != other.m_iVncImageEncoding) return false;
    if (m_iUsedShareGroups != other.m_iUsedShareGroups) return false;
    if (m_iKbdLayoutLanguage != other.m_iKbdLayoutLanguage) return false;
    if (m_iJpegQuality != other.m_iJpegQuality) return false;
    if (m_eConnectionSpeed != other.m_eConnectionSpeed) return false;
    if (m_eDesktopType != other.m_eDesktopType) return false;
    if (m_eDisplayType != other.m_eDisplayType) return false;
    if (m_eSessionType != other.m_eSessionType) return false;
    if (m_eCacheMemory != other.m_eCacheMemory) return false;
    if (m_eCacheDisk != other.m_eCacheDisk) return false;
    if (m_sName != other.m_sName) return false;
    if (m_sUsername != other.m_sUsername) return false;
    if (m_sPassword != other.m_sPassword) return false;
    if (m_sBackingstore != other.m_sBackingstore) return false;
    if (m_sServerHost != other.m_sServerHost) return false;
    if (m_sCommandLine != other.m_sCommandLine) return false;
    if (m_sRdpHostName != other.m_sRdpHostName) return false;
    if (m_sRdpUsername != other.m_sRdpUsername) return false;
    if (m_sRdpPassword != other.m_sRdpPassword) return false;
    if (m_sRdpApplication != other.m_sRdpApplication) return false;
    if (m_sVncHostName != other.m_sVncHostName) return false;
    if (m_sVncPassword != other.m_sVncPassword) return false;
    if (m_sFontServerHost != other.m_sFontServerHost) return false;
    if (cmpUsedShareGroups(m_aUsedShareGroups, other.m_aUsedShareGroups)) return false;
    if (cmpShareGroups(m_aShareGroups, other.m_aShareGroups)) return false;
    return true;
}

MxXmlConfig::MxXmlConfig(const wxString &filename)
    : m_bValid(false)
    , saved(NULL)
    , m_bRememberPassword(false)
    , m_bUseCustomImageEncoding(false)
    , m_bUseSmartCard(false)
    , m_bDisableTcpNoDelay(false)
    , m_bDisableZlibCompression(false)
    , m_bEnableSSL(true)
    , m_bKbdLayoutOther(false)
    , m_bEnableSmbSharing(false)
    , m_bEnableMultimedia(false)
    , m_bRemoveOldSessionFiles(true)
    , m_bUseFontServer(false)
    , m_bRunConsole(false)
    , m_bRunXclients(false)
    , m_bVirtualDesktop(false)
    , m_bDisableTaint(false)
    , m_bRdpRememberPassword(false)
    , m_bRdpRunApplication(false)
    , m_bVncRememberPassword(false)
    , m_bVncUseMxAuth(false)
    , m_bUsePNG(false)
    , m_bUsePlainX(false)
    , m_bUseJpeg(true)
    , m_bUseJpegQuality(false)
    , m_bDisableRender(false)
    , m_bUseTightJpeg(false)
    , m_iDisplayHeight(480)
    , m_iDisplayWidth(640)
    , m_iServerPort(22)
    , m_iFontServerPort(7100)
    , m_iRdpAuthType(1)
    , m_iVncDisplayNumber(0)
    , m_iRdpImageCompression(1)
    , m_iVncImageEncoding(0)
    , m_iUsedShareGroups(0)
    , m_iJpegQuality(6)
    , m_eConnectionSpeed(SPEED_ADSL)
    , m_eDesktopType(DTYPE_KDE)
    , m_eDisplayType(DPTYPE_AVAILABLE)
    , m_eSessionType(STYPE_UNIX)
    , m_eCacheMemory(CACHEMEM_8MB)
    , m_eCacheDisk(CACHEDISK_32MB)
    , m_sName(_T(""))
    , m_sUsername(_T(""))
    , m_sPassword(_T(""))
    , m_sBackingstore(_T(""))
    , m_sServerHost(_T(""))
    , m_sCommandLine(_T(""))
    , m_sRdpHostName(_T(""))
    , m_sRdpUsername(_T(""))
    , m_sRdpPassword(_T(""))
    , m_sRdpApplication(_T(""))
    , m_sVncHostName(_T(""))
    , m_sVncPassword(_T(""))
    , m_sFontServerHost(_T(""))
    , m_sFileName(_T(""))
    , m_pMd5Password(NULL)
    , m_pClrPassword(NULL)
{
    wxString tmp;
    int itmp;
    
    m_iKbdLayoutLanguage = mapLocaleToKeyboard();
    
    wxFile *f = new wxFile(filename);
    if ((!f->IsOpened()) || f->Eof()) {
        delete f;
        return;
    }
    delete f;
    ::wxLogDebug(_T("MxXmlConfig: Reading %s"), filename.c_str());
    wxXmlDocument cfg(filename);
    wxXmlNode *cfgnode = cfg.GetRoot();
    if (cfgnode && (cfgnode->GetName() == _T("NXClientSettings"))) {
        if (!(cfgnode->GetPropVal(_T("application"), _T("")) == _T("nxclient")))
            return;
        if (!(cfgnode->GetPropVal(_T("version"), _T("")) == _T("1.3")))
            return;
        cfgnode = cfgnode->GetChildren();
        while (cfgnode) {
            if (cfgnode->GetName() == _T("group")) {
                if (cfgnode->GetPropVal(_T("name"), _T("")) == _T("General")) {
                    wxXmlNode *opt = cfgnode->GetChildren();
                    while (opt) {
                        tmp = getString(opt, _T("Session"));
                        if (tmp == _T("Unix")) {
                            m_eSessionType = STYPE_UNIX;
                            m_bValid = true;
                        }
                        if (tmp == _T("Windows")) {
                            m_eSessionType = STYPE_WINDOWS;
                            m_bValid = true;
                        }
                        if (tmp == _T("VNC")) {
                            m_eSessionType = STYPE_VNC;
                            m_bValid = true;
                        }
                        
                        tmp = getString(opt, _T("Link speed"));
                        if (tmp == _T("modem"))
                            m_eConnectionSpeed = SPEED_MODEM;
                        if (tmp == _T("isdn"))
                            m_eConnectionSpeed = SPEED_ISDN;
                        if (tmp == _T("adsl"))
                            m_eConnectionSpeed = SPEED_ADSL;
                        if (tmp == _T("wan"))
                            m_eConnectionSpeed = SPEED_WAN;
                        if (tmp == _T("lan"))
                            m_eConnectionSpeed = SPEED_LAN;
                        
                        tmp = getString(opt, _T("Desktop"));
                        if (tmp == _T("RDP"))
                            m_eDesktopType = DTYPE_RDP;
                        if (tmp == _T("RFB"))
                            m_eDesktopType = DTYPE_RFB;
                        if (tmp == _T("KDE"))
                            m_eDesktopType = DTYPE_KDE;
                        if (tmp == _T("Gnome"))
                            m_eDesktopType = DTYPE_GNOME;
                        if (tmp == _T("CDE"))
                            m_eDesktopType = DTYPE_CDE;
                        if (tmp == _T("XFCE"))
                            m_eDesktopType = DTYPE_XFCE;
                        if (tmp == _T("Console"))
                            m_eDesktopType = DTYPE_CUSTOM;
                        
                        tmp = getString(opt, _T("Resolution"));
                        if (!tmp.IsEmpty())
                            m_eDisplayType = DPTYPE_CUSTOM;
                        if (tmp == _T("640x480"))
                            m_eDisplayType = DPTYPE_640x480;
                        if (tmp == _T("800x600"))
                            m_eDisplayType = DPTYPE_800x600;
                        if (tmp == _T("1024x768"))
                            m_eDisplayType = DPTYPE_1024x768;
                        if (tmp == _T("available"))
                            m_eDisplayType = DPTYPE_AVAILABLE;
                        if (tmp == _T("fullscreen"))
                            m_eDisplayType = DPTYPE_FULLSCREEN;
                        
                        itmp = getLong(opt, _T("Use default image encoding"), -1);
                        if (itmp != -1)
                            m_bUseCustomImageEncoding = (itmp == 0) ? false : true;
                        
                        m_sBackingstore = getString(opt, _T("Backingstore"), m_sBackingstore);
                        
                        // X11 props
                        m_sCommandLine = getString(opt, _T("Command line"), m_sCommandLine);
                        m_bRunConsole = getBool(opt, _T("Only console"), m_bRunConsole);
                        m_bRunXclients = getBool(opt, _T("Run default script"), m_bRunXclients);
                        m_bVirtualDesktop = getBool(opt, _T("Virtual desktop"), m_bVirtualDesktop);
                        m_bDisableTaint = !getBool(opt, _T("Use taint"), !m_bDisableTaint);
                        m_bDisableRender = !getBool(opt, _T("Use render"), !m_bDisableRender);
                        m_bUseSmartCard = getBool(opt, _T("Use smartcard"), m_bUseSmartCard);
                        
                        m_bRememberPassword = getBool(opt, _T("Remember password"), m_bRememberPassword);
                        m_iDisplayHeight = getLong(opt, _T("Resolution height"), m_iDisplayHeight);
                        m_iDisplayWidth = getLong(opt, _T("Resolution width"), m_iDisplayWidth);
                        m_sServerHost = getString(opt, _T("Server host"), m_sServerHost);
                        m_iServerPort = getLong(opt, _T("Server port"), m_iServerPort);
                        opt = opt->GetNext();
                    }
                    cfgnode = cfgnode->GetNext();
                    continue;
                }
                if (cfgnode->GetPropVal(_T("name"), _T("")) == _T("Login")) {
                    wxXmlNode *opt = cfgnode->GetChildren();
                    while (opt) {
                        m_sUsername = getString(opt, _T("User"), m_sUsername);
                        m_pMd5Password = getStringNew(opt, _T("Auth"), m_pMd5Password);
                        m_pClrPassword = getStringNew(opt, _T("Password"), m_pClrPassword);
                        // TODO: decode
                        opt = opt->GetNext();
                    }
                    cfgnode = cfgnode->GetNext();
                    continue;
                }
                if (cfgnode->GetPropVal(_T("name"), _T("")) == _T("Environment")) {
                    wxXmlNode *opt = cfgnode->GetChildren();
                    while (opt) {
                        m_bUseFontServer = getBool(opt, _T("Use font server"), m_bUseFontServer);
                        m_iFontServerPort = getLong(opt, _T("Font server port"), m_iFontServerPort);
                        m_sFontServerHost = getString(opt, _T("Font server host"), m_sFontServerHost);
                        opt = opt->GetNext();
                    }
                    cfgnode = cfgnode->GetNext();
                    continue;
                }
                if (cfgnode->GetPropVal(_T("name"), _T("")) == _T("Images")) {
                    wxXmlNode *opt = cfgnode->GetChildren();
                    while (opt) {
                        m_iRdpImageCompression = getLong(opt, _T("Windows images compression"), m_iRdpImageCompression);
                        m_iVncImageEncoding = getLong(opt, _T("Image Encoding Type"), m_iVncImageEncoding);
                        m_bUseTightJpeg = getBool(opt, _T("Image JPEG Encoding"), m_bUseTightJpeg);
                        m_iJpegQuality = getLong(opt, _T("JPEG Quality"), m_iJpegQuality);
                        m_bUsePNG = getBool(opt, _T("Use PNG Compression"), m_bUsePNG);
                        itmp = getLong(opt, _T("Image Compression Type"), -1);
                        switch (itmp) {
                            case 0:
                                m_bUseJpeg = true;
                                m_bUseJpegQuality = false;
                                m_bUsePlainX = false;
                                break;
                            case 1:
                                m_bUseJpeg = true;
                                m_bUseJpegQuality = true;
                                m_bUsePlainX = false;
                                break;
                            case 2:
                                m_bUseJpeg = false;
                                m_bUseJpegQuality = false;
                        }
                        opt = opt->GetNext();
                        if (!opt)
                            m_bUsePlainX = !m_bUsePNG;
                    }
                    cfgnode = cfgnode->GetNext();
                    continue;
                }
                if (cfgnode->GetPropVal(_T("name"), _T("")) == _T("Services")) {
                    wxXmlNode *opt = cfgnode->GetChildren();
                    while (opt) {
                        m_bEnableMultimedia = getBool(opt, _T("Audio"), m_bEnableMultimedia);
                        m_bEnableSmbSharing = getBool(opt, _T("Shares"), m_bEnableSmbSharing);
                        opt = opt->GetNext();
                    }
                    cfgnode = cfgnode->GetNext();
                    continue;
                }
                if (cfgnode->GetPropVal(_T("name"), _T("")) == _T("share chosen")) {
                    wxXmlNode *opt = cfgnode->GetChildren();
                    while (opt) {
                        if (opt->GetPropVal(_T("key"), _T("")) == _T("Share number"))
                            m_iUsedShareGroups = getLong(opt, _T("Share number"), m_iUsedShareGroups);
                        else
                            m_aUsedShareGroups.Add(opt->GetPropVal(_T("value"), _T("")));
                        opt = opt->GetNext();
                    }
                    cfgnode = cfgnode->GetNext();
                    continue;
                }
                if (cfgnode->GetPropVal(_T("name"), _T("")) == _T("Advanced")) {
                    wxXmlNode *opt = cfgnode->GetChildren();
                    while (opt) {
                        m_bKbdLayoutOther = !getBool(opt, _T("Current keyboard"), !m_bKbdLayoutOther);
                        m_bDisableTcpNoDelay = getBool(opt, _T("Disable TCP no-delay"), m_bDisableTcpNoDelay);
                        m_bDisableZlibCompression = getBool(opt, _T("Disable ZLIB stream compression"),
                            m_bDisableZlibCompression);
                        m_bEnableSSL = getBool(opt, _T("Enable SSL encryption"), m_bEnableSSL);
                        itmp = getLong(opt, _T("Cache size"), -1);
                        switch (itmp) {
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
                        if (wxGetEnv(_T("THINCLIENT"), NULL))
                            m_eCacheDisk = CACHEDISK_0MB;
                        else {
                            itmp = getLong(opt, _T("Cache size on disk"), -1);
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
                        }
                        itmp = getLong(opt, _T("Other keyboard"), -1);
                        if (itmp != -1)
                            m_iKbdLayoutLanguage = mapMxLanguage(itmp);
                        opt = opt->GetNext();
                    }
                    cfgnode = cfgnode->GetNext();
                    continue;
                }
                if (cfgnode->GetPropVal(_T("name"), _T("")) == _T("Windows Session")) {
                    wxXmlNode *opt = cfgnode->GetChildren();
                    while (opt) {
                        m_bRdpRememberPassword = getBool(opt, _T("Remember"), m_bRdpRememberPassword);
                        m_bRdpRunApplication = getBool(opt, _T("Run application"), m_bRdpRunApplication);
                        m_iRdpAuthType = getLong(opt, _T("Authentication"), m_iRdpAuthType);
                        m_sRdpHostName = getString(opt, _T("Server"), m_sRdpHostName);
                        m_sRdpUsername = getString(opt, _T("User"), m_sRdpUsername);
                        m_sRdpPassword = getPassword(opt, _T("Password"), m_sRdpPassword);
                        m_sRdpApplication = getString(opt, _T("Application"), m_sRdpApplication);
                        opt = opt->GetNext();
                    }
                    cfgnode = cfgnode->GetNext();
                    continue;
                }
                if (cfgnode->GetPropVal(_T("name"), _T("")) == _T("VNC Session")) {
                    wxXmlNode *opt = cfgnode->GetChildren();
                    while (opt) {
                        m_bVncRememberPassword = getBool(opt, _T("Remember"), m_bVncRememberPassword);
                        m_sVncHostName = getString(opt, _T("Server"), m_sVncHostName);
                        m_sVncPassword = getPassword(opt, _T("Password"), m_sVncPassword);
                        tmp = getString(opt, _T("Display"));
                        if (tmp.Left(1) == _T(":")) {
                            tmp = tmp.Mid(1);
                            if (tmp.IsNumber()) {
                                long l;
                                tmp.ToLong(&l, 0);
                                m_iVncDisplayNumber = l;
                            }
                        }
                        opt = opt->GetNext();
                    }
                    cfgnode = cfgnode->GetNext();
                    continue;
                }
                // When we reach here, we gon an "unknown" group name. This is usually
                // a mounted share description.
                {
                    int shareOptions = 0;
                    ShareGroup s;
                    s.m_sGroupName = cfgnode->GetPropVal(_T("name"), _T(""));
                    s.m_iType = SharedResource::SHARE_UNKNOWN;
                    wxXmlNode *opt = cfgnode->GetChildren();
                    while (opt) {
                        if (opt->GetPropVal(_T("key"), _T("")) == _T("Alias")) {
                            shareOptions++;
                            s.m_sMountPoint = getString(opt, _T("Alias"), _T(""));
                        }
                        if (opt->GetPropVal(_T("key"), _T("")) == _T("Sharename")) {
                            shareOptions++;
                            s.m_sName = getString(opt, _T("Sharename"), _T(""));
                        }
                        if (opt->GetPropVal(_T("key"), _T("")) == _T("Username")) {
                            shareOptions++;
                            s.m_sUsername = getString(opt, _T("Username"), _T(""));
                        }
                        if (opt->GetPropVal(_T("key"), _T("")) == _T("Password")) {
                            shareOptions++;
                            s.m_sPassword = getPassword(opt, _T("Password"), _T(""));
                        }
                        if (opt->GetPropVal(_T("key"), _T("")) == _T("Type")) {
                            shareOptions++;
                            wxString tmp = getString(opt, _T("Type"), _T(""));
                            if (tmp == _T("Disk"))
                                s.m_iType = SharedResource::SHARE_DISK;
                            if (tmp == _T("Printer"))
                                s.m_iType = SharedResource::SHARE_PRINTER;
                        }
                        if (opt->GetPropVal(_T("key"), _T("")) == _T("Mounted now")) {
                            shareOptions++;
                            s.m_bMountNow = getBool(opt, _T("Mounted now"), false);
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
        m_sUsername = _T("");
        m_sPassword = _T("");
    }
}

MxXmlConfig::~MxXmlConfig()
{
    if (saved)
	delete saved;
    if (m_pMd5Password)
        delete m_pMd5Password;
    if (m_pClrPassword)
        delete m_pClrPassword;
}

void 
MxXmlConfig::bAddOption(wxXmlNode *group, const wxString &name, bool val)
{
    wxXmlNode *n = new wxXmlNode(wxXML_ELEMENT_NODE, _T("option"));
    n->AddProperty(new wxXmlProperty(_T("key"), name, NULL));
    n->AddProperty(new wxXmlProperty(_T("value"), val ? _T("true") : _T("false"), NULL));
    group->AddChild(n);
}

void
MxXmlConfig::iAddOption(wxXmlNode *group, const wxString &name, long val)
{
    wxXmlNode *n = new wxXmlNode(wxXML_ELEMENT_NODE, _T("option"));
    n->AddProperty(new wxXmlProperty(_T("key"), name, NULL));
    n->AddProperty(new wxXmlProperty(_T("value"), wxString::Format(_T("%d"), val), NULL));
    group->AddChild(n);
}

void
MxXmlConfig::sAddOption(wxXmlNode *group, const wxString &name, wxString &val)
{
    wxXmlNode *n = new wxXmlNode(wxXML_ELEMENT_NODE, _T("option"));
    n->AddProperty(new wxXmlProperty(_T("key"), name, NULL));
    n->AddProperty(new wxXmlProperty(_T("value"), val, NULL));
    group->AddChild(n);
}

wxXmlNode *
MxXmlConfig::AddGroup(wxXmlNode *parent, const wxString &name)
{
    wxXmlNode *n = new wxXmlNode(wxXML_ELEMENT_NODE, _T("group"));
    n->AddProperty(new wxXmlProperty(_T("name"), name, NULL));
    parent->AddChild(n);
    return n;
}

bool
MxXmlConfig::SaveToFile()
{
    wxXmlDocument cfg;
    wxXmlNode *r;
    wxXmlNode *g;
    wxString optval;
    size_t i;

    r = new wxXmlNode(wxXML_ELEMENT_NODE, _T("NXClientSettings"));
    r->AddProperty(new wxXmlProperty(_T("application"), _T("nxclient"), NULL));
    r->AddProperty(new wxXmlProperty(_T("version"), _T("1.3"), NULL));
    cfg.SetRoot(r);

    g = AddGroup(r, _T("General"));
    switch (m_eSessionType) {
        case STYPE_UNIX:
            optval = _T("Unix");
            break;
        case STYPE_WINDOWS:
            optval = _T("Windows");
            break;
        case STYPE_VNC:
            optval = _T("VNC");
            break;
    }
    sAddOption(g, _T("Session"), optval);
    switch (m_eConnectionSpeed) {
        case SPEED_MODEM:
            optval = _T("modem");
            break;
        case SPEED_ISDN:
            optval = _T("isdn");
            break;
        case SPEED_ADSL:
            optval = _T("adsl");
            break;
        case SPEED_WAN:
            optval = _T("wan");
            break;
        case SPEED_LAN:
            optval = _T("lan");
            break;
    }
    sAddOption(g, _T("Link speed"), optval);
    switch (m_eDesktopType) {
        case DTYPE_RDP:
            optval = _T("RDP");
            break;
        case DTYPE_RFB:
            optval = _T("RFB");
            break;
        case DTYPE_KDE:
            optval = _T("KDE");
            break;
        case DTYPE_GNOME:
            optval = _T("Gnome");
            break;
        case DTYPE_CDE:
            optval = _T("CDE");
            break;
        case DTYPE_XFCE:
            optval = _T("XFCE");
            break;
        case DTYPE_CUSTOM:
            optval = _T("Console");
            break;
    }
    sAddOption(g, _T("Desktop"), optval);
    switch (m_eDisplayType) {
        case DPTYPE_CUSTOM:
            optval = wxString::Format(_T("%dx%d"),
                m_iDisplayWidth, m_iDisplayHeight);
            break;
        case DPTYPE_640x480:
            optval = _T("640x480");
            break;
        case DPTYPE_800x600:
            optval = _T("800x600");
            break;
        case DPTYPE_1024x768:
            optval = _T("1024x768");
            break;
        case DPTYPE_AVAILABLE:
            optval = _T("available");
            break;
        case DPTYPE_FULLSCREEN:
            optval = _T("fullscreen");
            break;
    }
    sAddOption(g, _T("Resolution"), optval);
    iAddOption(g, _T("Use default image encoding"), m_bUseCustomImageEncoding ? 1 : 0);
    sAddOption(g, _T("Backingstore"), m_sBackingstore);
    sAddOption(g, _T("Command line"), m_sCommandLine);
    bAddOption(g, _T("Only console"), m_bRunConsole);
    bAddOption(g, _T("Run default script"), m_bRunXclients);
    bAddOption(g, _T("Use taint"), !m_bDisableTaint);
    bAddOption(g, _T("Virtual desktop"), m_bVirtualDesktop);
    bAddOption(g, _T("Remember password"), m_bRememberPassword);
    bAddOption(g, _T("Use render"), !m_bDisableRender);
    bAddOption(g, _T("Use smartcard"), m_bUseSmartCard);
    iAddOption(g, _T("Resolution height"), m_iDisplayHeight);
    iAddOption(g, _T("Resolution width"), m_iDisplayWidth);
    sAddOption(g, _T("Server host"), m_sServerHost);
    iAddOption(g, _T("Server port"), m_iServerPort);

    g = AddGroup(r, _T("Login"));
    sAddOption(g, _T("User"), m_sUsername);
    if (m_bRememberPassword) {
        if ((m_sPassword != DUMMY_MD5_PASSWORD) &&
            (m_sPassword != DUMMY_CLR_PASSWORD)   ) {
            optval = cryptString(m_sPassword);
            sAddOption(g, _T("Auth"), optval);
        } else {
            if (m_pMd5Password)
                sAddOption(g, _T("Auth"), *m_pMd5Password);
            if (m_pClrPassword)
                sAddOption(g, _T("Password"), *m_pClrPassword);
        }
    } else {
        optval = _T("");
        sAddOption(g, _T("Auth"), optval);
        sAddOption(g, _T("Password"), optval);
    }

    g = AddGroup(r, _T("Environment"));
    bAddOption(g, _T("Use font server"), m_bUseFontServer);
    sAddOption(g, _T("Font server host"), m_sFontServerHost);
    iAddOption(g, _T("Font server port"), m_iFontServerPort);

    g = AddGroup(r, _T("Images"));
    iAddOption(g, _T("Windows images compression"), m_iRdpImageCompression);
    iAddOption(g, _T("Image Encoding Type"), m_iVncImageEncoding);
    iAddOption(g, _T("JPEG Quality"), m_iJpegQuality);
    bAddOption(g, _T("Use PNG Compression"), m_bUsePNG);
    bAddOption(g, _T("Image JPEG Encoding"), m_bUseTightJpeg);
    if ((m_bUsePlainX || m_bUsePNG) && (!m_bUseJpeg))
        optval = _T("2");
    else if (m_bUseJpeg && m_bUseJpegQuality)
        optval = _T("1");
    else
        optval = _T("0");
    sAddOption(g, _T("Image Compression Type"), optval);

    g = AddGroup(r, _T("Services"));
    bAddOption(g, _T("Audio"), m_bEnableMultimedia);
    bAddOption(g, _T("Shares"), m_bEnableSmbSharing);

    if (m_aUsedShareGroups.GetCount()) {
        g = AddGroup(r, _T("share chosen"));
        iAddOption(g, _T("Share number"), m_iUsedShareGroups);
        for (i = 0; i < m_aUsedShareGroups.GetCount(); i++) {
            optval = wxString::Format(_T("Share%d"), i);
            sAddOption(g, optval, m_aUsedShareGroups[i]); 
        }
    }

    g = AddGroup(r, _T("Advanced"));
    bAddOption(g, _T("Current keyboard"), !m_bKbdLayoutOther);
    bAddOption(g, _T("Disable TCP no-delay"), m_bDisableTcpNoDelay);
    bAddOption(g, _T("Disable ZLIB stream compression"), m_bDisableZlibCompression);
    bAddOption(g, _T("Enable SSL encryption"), m_bEnableSSL);
    switch (m_eCacheMemory) {
        case CACHEMEM_1MB:
            optval = _T("1");
            break;
        case CACHEMEM_2MB:
            optval = _T("2");
            break;
        case CACHEMEM_4MB:
            optval = _T("4");
            break;
        case CACHEMEM_8MB:
            optval = _T("8");
            break;
        case CACHEMEM_16MB:
            optval = _T("16");
            break;
        case CACHEMEM_32MB:
            optval = _T("32");
            break;
    }
    sAddOption(g, _T("Cache size"), optval);
    switch (m_eCacheDisk) {
        case CACHEDISK_0MB:
            optval = _T("0");
            break;
        case CACHEDISK_4MB:
            optval = _T("4");
            break;
        case CACHEDISK_8MB:
            optval = _T("8");
            break;
        case CACHEDISK_16MB:
            optval = _T("16");
            break;
        case CACHEDISK_32MB:
            optval = _T("32");
            break;
        case CACHEDISK_64MB:
            optval = _T("64");
            break;
        case CACHEDISK_128MB:
            optval = _T("128");
            break;
    }
    sAddOption(g, _T("Cache size on disk"), optval);
    iAddOption(g, _T("Other keyboard"), mapNxLanguage(m_iKbdLayoutLanguage));

    g = AddGroup(r, _T("Windows Session"));
    bAddOption(g, _T("Remember"), m_bRdpRememberPassword);
    bAddOption(g, _T("Run application"), m_bRdpRunApplication);
    iAddOption(g, _T("Authentication"), m_iRdpAuthType);
    sAddOption(g, _T("Server"), m_sRdpHostName);
    sAddOption(g, _T("User"), m_sRdpUsername);
    optval = m_bRdpRememberPassword ? encodeString(m_sRdpPassword) : _T("");
    sAddOption(g, _T("Password"), optval);
    sAddOption(g, _T("Application"), m_sRdpApplication);

    g = AddGroup(r, _T("VNC Session"));
    bAddOption(g, _T("Remember"), m_bVncRememberPassword);
    sAddOption(g, _T("Server"), m_sVncHostName);
    optval = m_bVncRememberPassword ? encodeString(m_sVncPassword) : _T("");
    sAddOption(g, _T("Password"), optval);
    optval = wxString::Format(_T(":%d"), m_iVncDisplayNumber);
    sAddOption(g, _T("Display"), optval);

    for (i = 0; i < m_aShareGroups.GetCount(); i++) {
        g = AddGroup(r, m_aShareGroups[i].m_sGroupName);
        sAddOption(g, _T("Alias"), m_aShareGroups[i].m_sMountPoint);
        sAddOption(g, _T("Sharename"), m_aShareGroups[i].m_sName);
        sAddOption(g, _T("Username"), m_aShareGroups[i].m_sUsername);
        optval = encodeString(m_aShareGroups[i].m_sPassword);
        sAddOption(g, _T("Password"), optval);
        switch (m_aShareGroups[i].m_iType) {
            case SharedResource::SHARE_DISK:
                optval = _T("Disk");
                sAddOption(g, _T("Type"), optval);
                break;
            case SharedResource::SHARE_PRINTER:
                optval = _T("Printer");
                sAddOption(g, _T("Type"), optval);
                break;
        }
        bAddOption(g, _T("Mounted now"), m_aShareGroups[i].m_bMountNow);
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
            ::wxLogDebug(_T("writing cfg '%s'"), m_sFileName.c_str());
            wxFileOutputStream fos(m_sFileName);
            if (!fos.Ok()) {
                delete data;
                return false;
            }
	    // NO _T() here!!
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

void MxXmlConfig::saveState()
{
    if (saved)
	delete saved;
    saved = new MxXmlConfig();
    *saved = *this;
}

bool MxXmlConfig::checkChanged()
{
    if (!saved)
	return true;
    return (!(*this == *saved));
}

bool MxXmlConfig::getBool(wxXmlNode *opt, const wxString &key, bool defval)
{
    bool val = defval;

    if (opt->GetName() == _T("option")) {
        if (opt->GetPropVal(_T("key"), _T("")) == key) {
            wxString tmp = opt->GetPropVal(_T("value"), _T(""));
            if (!tmp.IsEmpty())
                val = ((tmp == STR_TRUE) || (tmp == STR_ONE));
        }
    }
    return val;
}

long MxXmlConfig::getLong(wxXmlNode *opt, const wxString &key, long defval)
{
    long val = defval;

    if (opt->GetName() == _T("option")) {
        if (opt->GetPropVal(_T("key"), _T("")) == key) {
            wxString tmp = opt->GetPropVal(_T("value"), _T(""));
            if (tmp.IsNumber())
                tmp.ToLong(&val, 0);
        }
    }
    return val;
}

wxString MxXmlConfig::getString(wxXmlNode *opt, const wxString &key, const wxString &defval)
{
    wxString tmp = defval;

    if (opt->GetName() == _T("option")) {
        if (opt->GetPropVal(_T("key"), _T("")) == key)
            tmp = opt->GetPropVal(_T("value"), defval);
    }
    return tmp;
}

wxString *MxXmlConfig::getStringNew(wxXmlNode *opt, const wxString &key, wxString *defval)
{
    wxString *val = defval;

    if (opt->GetName() == _T("option")) {
        if (opt->GetPropVal(_T("key"), _T("")) == key) {
            if (opt->HasProp(_T("value"))) {
                val = new wxString(opt->GetPropVal(_T("value"), defval ? *defval : _T("")));
                if (defval)
                    delete defval;
            }
        }
    }
    return val;
}

wxString MxXmlConfig::getPassword(wxXmlNode *opt, const wxString &key, const wxString &defval)
{
    wxString val = defval;
    wxString *enc = getStringNew(opt, key, NULL);
    if (enc && enc->Left(1) == _T(":") && enc->Right(1) == _T(":") && enc->Length() > 1) {
        int idx = 1;
        wxString newpw = _T("");
        enc->Remove(0, 1);
        while (enc->Length()) {
            int p = enc->Find(_T(":"));
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

int MxXmlConfig::mapLocaleToKeyboard()
{
    int ret = 0;

    switch (wxGetApp().getLocale()->GetLanguage()) {
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

// Map keyboard language code from NXclient to MXclient
int
MxXmlConfig::mapMxLanguage(int origNXlang)
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

// Map keyboard language code from MXclient to NXclient
int 
MxXmlConfig::mapNxLanguage(int origMXlang)
{
    switch (origMXlang) {
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
            if (origMXlang < 12)
                return origMXlang + 2;
            if ((origMXlang >= 13) && (origMXlang <= 18))
                return origMXlang + 1;
            if ((origMXlang >= 20) && (origMXlang <= 24))
                return origMXlang;
            if ((origMXlang > 25) && (origMXlang <= 40))
                return origMXlang - 1;
            break;
    }
    return 0;
}
