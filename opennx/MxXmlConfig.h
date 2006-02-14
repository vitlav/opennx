// MxXmlConfig.h: interface for the MxXmlConfig class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _MXXMLCONFIG_H_
#define _MXXMLCONFIG_H_

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "MxXmlConfig.cpp"
#endif

#include <wx/dynarray.h>
#include <wx/string.h>
#include <wx/object.h>

class wxXmlNode;

class ShareGroup : public wxObject
{
public:
    ShareGroup() : wxObject() {}
    virtual ~ShareGroup() {}

    bool operator ==(const ShareGroup &);
    bool operator !=(const ShareGroup &);

    bool m_bMountNow;
    int m_iType;
    wxString m_sGroupName;
    wxString m_sMountPoint;
    wxString m_sName;
    wxString m_sUsername;
    wxString m_sPassword;
};

WX_DECLARE_OBJARRAY(ShareGroup, ArrayOfShareGroups);

class MxXmlConfig
{
public:
    typedef enum {
        SPEED_MODEM = 0,
        SPEED_ISDN,
        SPEED_ADSL,
        SPEED_WAN,
        SPEED_LAN,
    } ConnectionSpeed;

    typedef enum {
        STYPE_UNIX = 0,
        STYPE_WINDOWS,
        STYPE_VNC,
    } SessionType;

    typedef enum {
        DTYPE_RDP = -2,
        DTYPE_RFB = -1,
        DTYPE_KDE = 0,
        DTYPE_GNOME,
        DTYPE_CDE,
        DTYPE_XFCE,
        DTYPE_CUSTOM
    } DesktopType;

    typedef enum {
        DPTYPE_640x480 = 0,
        DPTYPE_800x600,
        DPTYPE_1024x768,
        DPTYPE_AVAILABLE,
        DPTYPE_FULLSCREEN,
        DPTYPE_CUSTOM
    } DisplayType;

    typedef enum {
        CACHEMEM_1MB = 0,
        CACHEMEM_2MB,
        CACHEMEM_4MB,
        CACHEMEM_8MB,
        CACHEMEM_16MB,
        CACHEMEM_32MB,
    } CacheMemory;

    typedef enum CacheDisk {
        CACHEDISK_0MB = 0,
        CACHEDISK_4MB,
        CACHEDISK_8MB,
        CACHEDISK_16MB,
        CACHEDISK_32MB,
        CACHEDISK_64MB,
        CACHEDISK_128MB,
    } CacheDisk;

    MxXmlConfig();
    MxXmlConfig(const wxString &);
    virtual ~MxXmlConfig();
    
    MxXmlConfig &operator =(const MxXmlConfig &);
    bool operator ==(const MxXmlConfig &);

    void saveState();
    bool checkChanged();
    bool SaveToFile();

    bool IsValid() { return m_bValid; }
    bool bGetRememberPassword() { return m_bRememberPassword; }
    bool bGetUseCustomImageEncoding() { return m_bUseCustomImageEncoding; }
    bool bGetUseSmartCard() { return m_bUseSmartCard; }
    bool bGetDisableTcpNoDelay() { return m_bDisableTcpNoDelay; }
    bool bGetDisableZlibCompression() { return m_bDisableZlibCompression; }
    bool bGetEnableSSL() { return m_bEnableSSL; }
    bool bGetKbdLayoutOther() { return m_bKbdLayoutOther; }
    bool bGetEnableSmbSharing() { return m_bEnableSmbSharing; }
    bool bGetEnableMultimedia() { return m_bEnableMultimedia; }
    bool bGetRemoveOldSessionFiles() { return m_bRemoveOldSessionFiles; }
    bool bGetUseFontServer() { return m_bUseFontServer; }
    bool bGetRunConsole() { return m_bRunConsole; }
    bool bGetRunXclients() { return m_bRunXclients; }
    bool bGetVirtualDesktop() { return m_bVirtualDesktop; }
    bool bGetDisableTaint() { return m_bDisableTaint; }
    bool bGetRdpRememberPassword() { return m_bRdpRememberPassword; }
    bool bGetRdpRunApplication() { return m_bRdpRunApplication; }
    bool bGetVncRememberPassword() { return m_bVncRememberPassword; }
    bool bGetVncUseMxAuth() { return m_bVncUseMxAuth; }
    bool bGetUsePNG() { return m_bUsePNG; }
    bool bGetUsePlainX() { return m_bUsePlainX; }
    bool bGetUseJpeg() { return m_bUseJpeg; }
    bool bGetUseJpegQuality() { return m_bUseJpegQuality; }
    bool bGetDisableRender() { return m_bDisableRender; }
    bool bGetUseTightJpeg() { return m_bUseTightJpeg; }
    
    MxXmlConfig::ConnectionSpeed eGetConnectionSpeed() { return m_eConnectionSpeed; }
    MxXmlConfig::DesktopType eGetDesktopType() { return m_eDesktopType; }
    MxXmlConfig::DisplayType eGetDisplayType() { return m_eDisplayType; }
    MxXmlConfig::SessionType eGetSessionType() { return m_eSessionType; }
    MxXmlConfig::CacheMemory eGetCacheMemory() { return m_eCacheMemory; }
    MxXmlConfig::CacheDisk eGetCacheDisk() { return m_eCacheDisk; }

    int iGetDisplayWidth() { return m_iDisplayWidth; }
    int iGetDisplayHeight() { return m_iDisplayHeight; }
    int iGetServerPort() { return m_iServerPort; }
    int iGetFontServerPort() { return m_iFontServerPort; }
    int iGetRdpAuthType() { return m_iRdpAuthType; }
    int iGetVncDisplayNumber() { return m_iVncDisplayNumber; }
    int iGetKbdLayoutLanguage() { return m_iKbdLayoutLanguage; }
    int iGetRdpImageCompression() { return m_iRdpImageCompression; }
    int iGetVncImageEncoding() { return m_iVncImageEncoding; }
    int iGetUsedShareGroups() { return m_iUsedShareGroups; }
    int iGetJpegQuality() { return m_iJpegQuality; }

    wxString sGetName() { return m_sName; }
    wxString sGetUsername() { return m_sUsername; }
    wxString sGetPassword() { return m_sPassword; }
    wxString sGetBackingstore() { return m_sBackingstore; }
    wxString sGetServerHost() { return m_sServerHost; }
    wxString sGetCommandLine() { return m_sCommandLine; }
    wxString sGetRdpHostName() { return m_sRdpHostName; }
    wxString sGetRdpUsername() { return m_sRdpUsername; }
    wxString sGetRdpPassword() { return m_sRdpPassword; }
    wxString sGetRdpApplication() { return m_sRdpApplication; }
    wxString sGetVncHostName() { return m_sVncHostName; }
    wxString sGetVncPassword() { return m_sVncPassword; }
    wxString sGetFontServerHost() { return m_sFontServerHost; }
    wxString sGetFileName() { return m_sFileName; }

    wxArrayString aGetUsedShareGroups() { return m_aUsedShareGroups; }
    ArrayOfShareGroups aGetShareGroups() { return m_aShareGroups; }

    void bSetRememberPassword(bool b) { m_bRememberPassword = b; }
    void bSetUseCustomImageEncoding(bool b) { m_bUseCustomImageEncoding = b; }
    void bSetUseSmartCard(bool b) { m_bUseSmartCard = b; }
    void bSetDisableTcpNoDelay(bool b) { m_bDisableTcpNoDelay = b; }
    void bSetDisableZlibCompression(bool b) { m_bDisableZlibCompression = b; }
    void bSetEnableSSL(bool b) { m_bEnableSSL = b; }
    void bSetKbdLayoutOther(bool b) { m_bKbdLayoutOther = b; }
    void bSetEnableSmbSharing(bool b) { m_bEnableSmbSharing = b; }
    void bSetEnableMultimedia(bool b) { m_bEnableMultimedia = b; }
    void bSetRemoveOldSessionFiles(bool b) { m_bRemoveOldSessionFiles = b; }
    void bSetUseFontServer(bool b) { m_bUseFontServer = b; }
    void bSetRunConsole(bool b) { m_bRunConsole = b; }
    void bSetRunXclients(bool b) { m_bRunXclients = b; }
    void bSetVirtualDesktop(bool b) { m_bVirtualDesktop = b; }
    void bSetDisableTaint(bool b) { m_bDisableTaint = b; }
    void bSetRdpRememberPassword(bool b) { m_bRdpRememberPassword = b; }
    void bSetRdpRunApplication(bool b) { m_bRdpRunApplication = b; }
    void bSetVncRememberPassword(bool b) { m_bVncRememberPassword = b; }
    void bSetVncUseMxAuth(bool b) { m_bVncUseMxAuth = b; }
    void bSetUsePNG(bool b) { m_bUsePNG = b; }
    void bSetUsePlainX(bool b) { m_bUsePlainX = b; }
    void bSetUseJpeg(bool b) { m_bUseJpeg = b; }
    void bSetUseJpegQuality(bool b) { m_bUseJpegQuality = b; }
    void bSetDisableRender(bool b) { m_bDisableRender = b; }
    void bSetUseTightJpeg(bool b) { m_bUseTightJpeg = b; }
    
    void eSetConnectionSpeed(MxXmlConfig::ConnectionSpeed e) { m_eConnectionSpeed = e; }
    void eSetDesktopType(MxXmlConfig::DesktopType e) { m_eDesktopType = e; }
    void eSetDisplayType(MxXmlConfig::DisplayType e) { m_eDisplayType = e; }
    void eSetSessionType(MxXmlConfig::SessionType e) { m_eSessionType = e; }
    void eSetCacheMemory(MxXmlConfig::CacheMemory e) { m_eCacheMemory = e; }
    void eSetCacheDisk(MxXmlConfig::CacheDisk e) { m_eCacheDisk = e; }

    void iSetDisplayWidth(int i) { m_iDisplayWidth = i; }
    void iSetDisplayHeight(int i) { m_iDisplayHeight = i; }
    void iSetServerPort(int i) { m_iServerPort = i; }
    void iSetFontServerPort(int i) { m_iFontServerPort = i; }
    void iSetRdpAuthType(int i) { m_iRdpAuthType = i; }
    void iSetVncDisplayNumber(int i) { m_iVncDisplayNumber = i; }
    void iSetKbdLayoutLanguage(int i) { m_iKbdLayoutLanguage = i; }
    void iSetRdpImageCompression(int i) { m_iRdpImageCompression = i; }
    void iSetVncImageEncoding(int i) { m_iVncImageEncoding = i; }
    void iSetUsedShareGroups(int i) { m_iUsedShareGroups = i; }
    void iSetJpegQuality(int i) { m_iJpegQuality = i; }

    void sSetName(const wxString &s) { m_sName = s; }
    void sSetUsername(const wxString &s) { m_sUsername = s; }
    void sSetPassword(const wxString &s) { m_sPassword = s; }
    void sSetBackingstore(const wxString &s) { m_sBackingstore = s; }
    void sSetServerHost(const wxString &s) { m_sServerHost = s; }
    void sSetCommandLine(const wxString &s) { m_sCommandLine = s; }
    void sSetRdpHostName(const wxString &s) { m_sRdpHostName = s; }
    void sSetRdpUsername(const wxString &s) { m_sRdpUsername = s; }
    void sSetRdpPassword(const wxString &s) { m_sRdpPassword = s; }
    void sSetRdpApplication(const wxString &s) { m_sRdpApplication = s; }
    void sSetVncHostName(const wxString &s) { m_sVncHostName = s; }
    void sSetVncPassword(const wxString &s) { m_sVncPassword = s; }
    void sSetFontServerHost(const wxString &s) { m_sFontServerHost = s; }
    void sSetFileName(const wxString &s) { m_sFileName = s; }

    void aSetUsedShareGroups(const wxArrayString &a) { m_aUsedShareGroups = a; }
    void aSetShareGroups(const ArrayOfShareGroups &a) { m_aShareGroups = a; }

private:

    bool getBool(wxXmlNode *, const wxString &, bool defval = false);
    long getLong(wxXmlNode *, const wxString &, long defval = 0);
    wxString getString(wxXmlNode *, const wxString &, const wxString &defval = _T(""));
    wxString getPassword(wxXmlNode *, const wxString &, const wxString &defval = _T(""));
    wxString *getStringNew(wxXmlNode *, const wxString &, wxString *defval = NULL);
    int mapMxLanguage(int);
    int mapNxLanguage(int);
    int mapLocaleToKeyboard();
    bool cmpShareGroups(ArrayOfShareGroups, ArrayOfShareGroups);
    bool cmpUsedShareGroups(wxArrayString, wxArrayString);
    void bAddOption(wxXmlNode *, const wxString &, bool);
    void iAddOption(wxXmlNode *, const wxString &, long);
    void sAddOption(wxXmlNode *, const wxString &, wxString &);
    wxXmlNode *AddGroup(wxXmlNode *, const wxString &);

    bool m_bValid;
    bool m_bRememberPassword;
    bool m_bUseCustomImageEncoding;
    bool m_bUseSmartCard;
    bool m_bDisableTcpNoDelay;
    bool m_bDisableZlibCompression;
    bool m_bEnableSSL;
    bool m_bKbdLayoutOther;
    bool m_bEnableSmbSharing;
    bool m_bEnableMultimedia;
    bool m_bRemoveOldSessionFiles;
    bool m_bUseFontServer;
    bool m_bRunConsole;
    bool m_bRunXclients;
    bool m_bVirtualDesktop;
    bool m_bDisableTaint;
    bool m_bVncUseMxAuth;
    bool m_bRdpRememberPassword;
    bool m_bRdpRunApplication;
    bool m_bVncRememberPassword;
    bool m_bUsePNG;
    bool m_bUsePlainX;
    bool m_bUseJpeg;
    bool m_bUseJpegQuality;
    bool m_bDisableRender;
    bool m_bUseTightJpeg;
    int m_iDisplayWidth;
    int m_iDisplayHeight;
    int m_iServerPort;
    int m_iFontServerPort;
    int m_iRdpAuthType;
    int m_iVncDisplayNumber;
    int m_iRdpImageCompression;
    int m_iVncImageEncoding;
    int m_iKbdLayoutLanguage;
    int m_iUsedShareGroups;
    int m_iJpegQuality;
    ConnectionSpeed m_eConnectionSpeed;
    DesktopType m_eDesktopType;
    DisplayType m_eDisplayType;
    SessionType m_eSessionType;
    CacheMemory m_eCacheMemory;
    CacheDisk m_eCacheDisk;
    wxString m_sName;
    wxString m_sUsername;
    wxString m_sPassword;
    wxString m_sBackingstore;
    wxString m_sServerHost;
    wxString m_sCommandLine;
    wxString m_sRdpHostName;
    wxString m_sRdpUsername;
    wxString m_sRdpPassword;
    wxString m_sRdpApplication;
    wxString m_sVncHostName;
    wxString m_sVncPassword;
    wxString m_sFontServerHost;
    wxString m_sFileName;

    ArrayOfShareGroups m_aShareGroups;
    wxArrayString m_aUsedShareGroups;

    MxXmlConfig *saved;
    wxString *m_pMd5Password;
    wxString *m_pClrPassword;
};

#endif
// _MXXMLCONFIG_H_
