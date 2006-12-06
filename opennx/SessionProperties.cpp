/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// Author:      Fritz Elfert
// Created:     06/13/04 22:44:30
// Copyright:   Copyright 2004 Millenux GmbH
// Licence:     LGPL
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma implementation "SessionProperties.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
#ifdef __WXMSW__
#include <shlobj.h>
#endif

////@begin includes
////@end includes
#include <wx/config.h>
#include <wx/fontdlg.h>
#include <wx/imaglist.h>

#include "SessionProperties.h"
#include "UnixImageSettingsDialog.h"
#include "RdpImageSettingsDialog.h"
#include "VncImageSettingsDialog.h"
#include "X11PropertyDialog.h"
#include "RdpPropertyDialog.h"
#include "VncPropertyDialog.h"
#include "AboutDialog.h"
#include "SmbShareProperties.h"
#include "MxXmlConfig.h"
#include "MxValidator.h"
#include "Icon.h"

////@begin XPM images
////@end XPM images

/*!
 * SessionProperties type definition
 */

IMPLEMENT_CLASS( SessionProperties, wxDialog )

/*!
 * SessionProperties event table definition
 */

BEGIN_EVENT_TABLE( SessionProperties, wxDialog )
////@begin SessionProperties event table entries
    EVT_TEXT( XRCID("ID_TEXTCTRL_HOST"), SessionProperties::OnTextctrlHostUpdated )

    EVT_TEXT( XRCID("ID_TEXTCTRL_PORT"), SessionProperties::OnTextctrlPortUpdated )

    EVT_CHECKBOX( XRCID("ID_CHECKBOX_PWSAVE"), SessionProperties::OnCheckboxPwsaveClick )

    EVT_CHECKBOX( XRCID("ID_CHECKBOX_SMARTCARD"), SessionProperties::OnCheckboxSmartcardClick )

    EVT_COMBOBOX( XRCID("ID_COMBOBOX_DPROTO"), SessionProperties::OnComboboxDprotoSelected )

    EVT_COMBOBOX( XRCID("ID_COMBOBOX_DTYPE"), SessionProperties::OnComboboxDtypeSelected )

    EVT_BUTTON( XRCID("ID_BUTTON_DSETTINGS"), SessionProperties::OnButtonDsettingsClick )

    EVT_SLIDER( XRCID("ID_SLIDER_SPEED"), SessionProperties::OnSliderSpeedUpdated )

    EVT_COMBOBOX( XRCID("ID_COMBOBOX_DISPTYPE"), SessionProperties::OnComboboxDisptypeSelected )

    EVT_SPINCTRL( XRCID("ID_SPINCTRL_WIDTH"), SessionProperties::OnSpinctrlWidthUpdated )

    EVT_SPINCTRL( XRCID("ID_SPINCTRL_HEIGHT"), SessionProperties::OnSpinctrlHeightUpdated )

    EVT_RADIOBUTTON( XRCID("ID_RADIOBUTTON_IMG_DEFAULT"), SessionProperties::OnRadiobuttonImgDefaultSelected )

    EVT_RADIOBUTTON( XRCID("ID_RADIOBUTTON_IMG_CUSTOM"), SessionProperties::OnRadiobuttonImgCustomSelected )

    EVT_BUTTON( XRCID("ID_BUTTON_IMG_CUSTOM"), SessionProperties::OnButtonImgCustomClick )

    EVT_CHECKBOX( XRCID("ID_CHECKBOX_DISABLETCPNODEL"), SessionProperties::OnCheckboxDisabletcpnodelClick )

    EVT_CHECKBOX( XRCID("ID_CHECKBOX_DISABLEZCOMP"), SessionProperties::OnCheckboxDisablezcompClick )

    EVT_CHECKBOX( XRCID("ID_CHECKBOX_ENABLESSL"), SessionProperties::OnCheckboxEnablesslClick )

    EVT_COMBOBOX( XRCID("ID_COMBOBOX_CACHEMEM"), SessionProperties::OnComboboxCachememSelected )

    EVT_COMBOBOX( XRCID("ID_COMBOBOX_CACHEDISK"), SessionProperties::OnComboboxCachediskSelected )

    EVT_BUTTON( XRCID("ID_BUTTON_CACHECLEAN"), SessionProperties::OnButtonCachecleanClick )

    EVT_COMBOBOX( XRCID("ID_COMBOBOX_BACKINGSTORE"), SessionProperties::OnComboboxBackingstoreSelected )

    EVT_RADIOBUTTON( XRCID("ID_RADIOBUTTON_KBDKEEP"), SessionProperties::OnRadiobuttonKbdkeepSelected )

    EVT_RADIOBUTTON( XRCID("ID_RADIOBUTTON_KBDOTHER"), SessionProperties::OnRadiobuttonKbdotherSelected )

    EVT_COMBOBOX( XRCID("ID_COMBOBOX_KBDLAYOUT"), SessionProperties::OnComboboxKbdlayoutSelected )

    EVT_CHECKBOX( XRCID("ID_CHECKBOX_SMB"), SessionProperties::OnCheckboxSmbClick )

    EVT_LIST_ITEM_SELECTED( XRCID("ID_LISTCTRL_SMB_SHARES"), SessionProperties::OnListctrlSmbSharesSelected )
    EVT_LIST_ITEM_ACTIVATED( XRCID("ID_LISTCTRL_SMB_SHARES"), SessionProperties::OnListctrlSmbSharesItemActivated )

    EVT_BUTTON( XRCID("ID_BUTTON_SMB_ADD"), SessionProperties::OnButtonSmbAddClick )

    EVT_BUTTON( XRCID("ID_BUTTON_SMB_MODIFY"), SessionProperties::OnButtonSmbModifyClick )

    EVT_BUTTON( XRCID("ID_BUTTON_SMB_DELETE"), SessionProperties::OnButtonSmbDeleteClick )

    EVT_TEXT( XRCID("ID_TEXTCTRL_USERDIR"), SessionProperties::OnTextctrlUserdirUpdated )

    EVT_BUTTON( XRCID("ID_BUTTON_BROWSE_USERDIR"), SessionProperties::OnButtonBrowseUserdirClick )

    EVT_CHECKBOX( XRCID("ID_CHECKBOX_REMOVEOLDSF"), SessionProperties::OnCheckboxRemoveoldsfClick )

    EVT_TEXT( XRCID("ID_TEXTCTRL_SYSDIR"), SessionProperties::OnTextctrlSysdirUpdated )

    EVT_BUTTON( XRCID("ID_BUTTON_BROWSE_SYSDIR"), SessionProperties::OnButtonBrowseSysdirClick )

    EVT_CHECKBOX( XRCID("ID_CHECKBOX_FONTSRV"), SessionProperties::OnCheckboxFontsrvClick )

    EVT_TEXT( XRCID("ID_TEXTCTRL_FSHOST"), SessionProperties::OnTextctrlFshostUpdated )

    EVT_TEXT( XRCID("ID_TEXTCTRL_FSPORT"), SessionProperties::OnTextctrlFsportUpdated )

    EVT_BUTTON( XRCID("ID_BUTTON_FONT_DEFAULT"), SessionProperties::OnButtonFontDefaultClick )

    EVT_BUTTON( XRCID("ID_BUTTON_FONT_FIXED"), SessionProperties::OnButtonFontFixedClick )

    EVT_BUTTON( XRCID("ID_BUTTON_ABOUT"), SessionProperties::OnButtonAboutClick )

    EVT_BUTTON( wxID_CLEAR, SessionProperties::OnClearClick )

    EVT_BUTTON( wxID_APPLY, SessionProperties::OnApplyClick )

////@end SessionProperties event table entries

END_EVENT_TABLE()

/*!
 * SessionProperties constructors
 */

SessionProperties::SessionProperties( )
    : m_pCfg(NULL)
    , m_bKeyTyped(false)
{
    m_cFontDefault = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
    m_cFontFixed = wxSystemSettings::GetFont(wxSYS_ANSI_FIXED_FONT);
}

SessionProperties::SessionProperties( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
    : m_pCfg(NULL)
    , m_bKeyTyped(false)
{
    m_cFontDefault = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
    m_cFontFixed = wxSystemSettings::GetFont(wxSYS_ANSI_FIXED_FONT);
    Create(parent, id, caption, pos, size, style);
}

void SessionProperties::SetConfig(MxXmlConfig *cfg)
{
    m_pCfg = cfg;
}

void SessionProperties::CheckChanged()
{
    wxASSERT_MSG(m_pCfg, _T("SessionProperties::CheckChanged: No configuration"));
    if (m_pCfg) {
        m_bKeyTyped = false;
        TransferDataFromWindow();

        // variables on 'General' Tab
        m_pCfg->bSetRememberPassword(m_bRememberPassword);
        m_pCfg->bSetUseSmartCard(m_bUseSmartCard);
        m_pCfg->bSetUseCustomImageEncoding(m_bUseCustomImageEncoding);
        m_pCfg->iSetServerPort(m_iPort);
        m_pCfg->eSetSessionType((MxXmlConfig::SessionType)m_iSessionType);
        switch (m_iSessionType) {
            case MxXmlConfig::STYPE_UNIX:
                m_iDesktopType = m_iDesktopTypeDialog;
                break;
            case MxXmlConfig::STYPE_WINDOWS:
                m_iDesktopType = MxXmlConfig::DTYPE_RDP;
                break;
            case MxXmlConfig::STYPE_VNC:
                m_iDesktopType = MxXmlConfig::DTYPE_RFB;
                break;
        }
        m_pCfg->eSetDesktopType((MxXmlConfig::DesktopType)m_iDesktopType);
        m_pCfg->eSetConnectionSpeed((MxXmlConfig::ConnectionSpeed)m_iConnectionSpeed);
        m_pCfg->eSetDisplayType((MxXmlConfig::DisplayType)m_iDisplayType);
        m_pCfg->iSetDisplayWidth(m_iDisplayWidth);
        m_pCfg->iSetDisplayHeight(m_iDisplayHeight);
        m_pCfg->sSetServerHost(m_sHostName);

        // variables on 'Advanced' tab
        m_pCfg->bSetDisableTcpNoDelay(m_bDisableTcpNoDelay);
        m_pCfg->bSetDisableZlibCompression(m_bDisableZlibCompression);
        m_pCfg->bSetEnableSSL(m_bEnableSSL);
        m_pCfg->eSetCacheMemory((MxXmlConfig::CacheMemory)m_iCacheMem);
        m_pCfg->eSetCacheDisk((MxXmlConfig::CacheDisk)m_iCacheDisk);
        m_pCfg->bSetKbdLayoutOther(m_bKbdLayoutOther);
        m_pCfg->iSetKbdLayoutLanguage(m_iKbdLayoutLanguage);

        // variables on 'Services' tab
        m_pCfg->bSetEnableSmbSharing(m_bEnableSmbSharing);
        m_pCfg->bSetEnableMultimedia(m_bEnableMultimedia);

        // variabless on 'Environment' tab
        m_pCfg->bSetRemoveOldSessionFiles(m_bRemoveOldSessionFiles);
        m_pCfg->bSetUseFontServer(m_bUseFontServer);
        m_pCfg->iSetFontServerPort(m_iFontServerPort);
        m_pCfg->sSetFontServerHost(m_sFontServer);

        bool changed = m_pCfg->checkChanged();
        changed |= (m_sSavedUserMxDir != m_sUserMxDir);
        changed |= (m_sSavedSystemMxDir != m_sSystemMxDir);
        m_pCtrlApplyButton->Enable(changed);
    }
}

void SessionProperties::KeyTyped() {
    m_bKeyTyped = true;
}

/*!
 * SessionProperties creator
 */

bool SessionProperties::Create( wxWindow* parent, wxWindowID WXUNUSED(id), const wxString& WXUNUSED(caption), const wxPoint& WXUNUSED(pos), const wxSize& WXUNUSED(size), long WXUNUSED(style) )
{
////@begin SessionProperties member initialisation
    m_pNoteBook = NULL;
    m_pCtrlHostname = NULL;
    m_pCtrlPort = NULL;
    m_pCtrlSessionType = NULL;
    m_pCtrlDesktopType = NULL;
    m_pCtrlDesktopSettings = NULL;
    m_pCtrlDisplayType = NULL;
    m_pCtrlDisplayWidth = NULL;
    m_pCtrlDisplayHeight = NULL;
    m_pCtrlImageEncDefault = NULL;
    m_pCtrlImageEncCustom = NULL;
    m_pCtrlImageSettings = NULL;
    m_pCtrlEnableSSL = NULL;
    m_pCtrlBackingStore = NULL;
    m_pCtrlKeyboardCurrent = NULL;
    m_pCtrlKeyboardOther = NULL;
    m_pCtrlKeyboardLayout = NULL;
    m_pCtrlSmbEnable = NULL;
    m_pCtrlSmbShares = NULL;
    m_pCtrlShareAdd = NULL;
    m_pCtrlShareModify = NULL;
    m_pCtrlShareDelete = NULL;
    m_pCtrlUserMxDir = NULL;
    m_pCtrlSystemMxDir = NULL;
    m_pCtrlUseFontserver = NULL;
    m_pCtrlFontserverHost = NULL;
    m_pCtrlFontserverPort = NULL;
    m_pCtrlFontDefault = NULL;
    m_pCtrlFontFixed = NULL;
    m_pCtrlApplyButton = NULL;
////@end SessionProperties member initialisation

    wxASSERT_MSG(m_pCfg, _T("SessionProperties::Create: No configuration"));
    if (m_pCfg) {
        // variables on 'General' Tab
        m_bRememberPassword = m_pCfg->bGetRememberPassword();
        m_bUseSmartCard = m_pCfg->bGetUseSmartCard();
        m_bUseCustomImageEncoding = m_pCfg->bGetUseCustomImageEncoding();
        m_iPort = m_pCfg->iGetServerPort();
        m_iSessionType = m_pCfg->eGetSessionType();
        m_iDesktopType = m_pCfg->eGetDesktopType();
        m_iDesktopTypeDialog = (m_iDesktopType >= 0) ? m_iDesktopType : 0;
        m_iUnixDesktopType = m_iDesktopType;
        m_iConnectionSpeed = m_pCfg->eGetConnectionSpeed();
        m_iDisplayType = m_pCfg->eGetDisplayType();
        m_iDisplayWidth = m_pCfg->iGetDisplayWidth();
        m_iDisplayHeight = m_pCfg->iGetDisplayHeight();
        m_sHostName = m_pCfg->sGetServerHost();

        // variables on 'Advanced' tab
        m_bDisableTcpNoDelay = m_pCfg->bGetDisableTcpNoDelay();
        m_bDisableZlibCompression = m_pCfg->bGetDisableZlibCompression();
        m_bEnableSSL = m_pCfg->bGetEnableSSL();
        m_iCacheMem = m_pCfg->eGetCacheMemory();
        m_iCacheDisk = m_pCfg->eGetCacheDisk();
        m_bKbdLayoutOther = m_pCfg->bGetKbdLayoutOther();
        m_iKbdLayoutLanguage = m_pCfg->iGetKbdLayoutLanguage();

        // variables on 'Services' tab
        m_bEnableSmbSharing = m_pCfg->bGetEnableSmbSharing();
        m_bEnableMultimedia = m_pCfg->bGetEnableMultimedia();

        // variabless on 'Environment' tab
        m_bRemoveOldSessionFiles = m_pCfg->bGetRemoveOldSessionFiles();
        m_bUseFontServer = m_pCfg->bGetUseFontServer();
        m_iFontServerPort = m_pCfg->iGetFontServerPort();
        m_sFontServer = m_pCfg->sGetFontServerHost();
    }
    wxConfigBase::Get()->Read(_T("Config/UserMxDir"), &m_sUserMxDir);
    wxConfigBase::Get()->Read(_T("Config/SystemMxDir"), &m_sSystemMxDir);

////@begin SessionProperties creation
    SetExtraStyle(GetExtraStyle()|wxWS_EX_VALIDATE_RECURSIVELY|wxWS_EX_BLOCK_EVENTS);
    SetParent(parent);
    CreateControls();
    SetIcon(GetIconResource(wxT("res/nx.png")));
    GetSizer()->Fit(this);
    GetSizer()->SetSizeHints(this);
    Centre();
////@end SessionProperties creation

    SetFontLabel(m_pCtrlFontDefault, m_cFontDefault);
    SetFontLabel(m_pCtrlFontFixed, m_cFontFixed);
#define SHI_SIZE 16
    wxImageList *il = new wxImageList(SHI_SIZE, SHI_SIZE);
    il->Add(CreateBitmapFromFile(_T("res/shbroken.png"), SHI_SIZE, SHI_SIZE));
    il->Add(CreateBitmapFromFile(_T("res/shfolder.png"), SHI_SIZE, SHI_SIZE));
    il->Add(CreateBitmapFromFile(_T("res/shprinter.png"), SHI_SIZE, SHI_SIZE));
#undef SHI_SIZE
    m_pCtrlSmbShares->AssignImageList(il, wxIMAGE_LIST_SMALL);
    m_pCtrlSmbShares->InsertColumn(0, _("Share"));
    m_pCtrlSmbShares->InsertColumn(1, _("Mount on"));
    m_pCtrlSmbShares->InsertColumn(2, _("Comment"));
    if (m_pCfg && (m_pCfg->iGetUsedShareGroups() > 0)) {
        size_t i;
        WinShare s;
        ArrayOfShareGroups sg = m_pCfg->aGetShareGroups();
        ArrayOfShares sa = s.GetShares();
        wxArrayString usg = m_pCfg->aGetUsedShareGroups();
        for (i = 0; i < sg.GetCount(); i++) {
            if (usg.Index(sg[i].m_sGroupName) != wxNOT_FOUND) {
                size_t si;
                int iconidx = 0;
                wxString comment = _("Currently not available");
                for (si = 0; si < sa.GetCount(); si++) {
                    if (sa[si].name == sg[i].m_sName) {
                        comment = sa[si].description;
                        iconidx = (sa[si].sharetype == SharedResource::SHARE_DISK) ? 1 : 2;
                        break;
                    }
                }
                int lidx = m_pCtrlSmbShares->InsertItem(0, sg[i].m_sName, iconidx);
                m_pCtrlSmbShares->SetItem(lidx, 1, sg[i].m_sMountPoint);
                m_pCtrlSmbShares->SetItem(lidx, 2, comment);
                m_pCtrlSmbShares->SetItemData(lidx, i);
            }
        }
        if (m_pCtrlSmbShares->GetItemCount() > 0) {
            m_pCtrlSmbShares->SetColumnWidth(0, wxLIST_AUTOSIZE);
            m_pCtrlSmbShares->SetColumnWidth(1, wxLIST_AUTOSIZE);
            m_pCtrlSmbShares->SetColumnWidth(2, wxLIST_AUTOSIZE);
        }
    }
    int lwidth = (m_pCtrlSmbShares->GetItemCount() > 0) ? wxLIST_AUTOSIZE : wxLIST_AUTOSIZE_USEHEADER;
    m_pCtrlSmbShares->SetColumnWidth(0, lwidth);
    m_pCtrlSmbShares->SetColumnWidth(1, lwidth);
    m_pCtrlSmbShares->SetColumnWidth(2, lwidth);
    UpdateDialogConstraints(false);

    // Fix broken fonts in Spin-Controls
    m_pCtrlDisplayWidth->SetFont(wxSystemSettings::GetFont(wxSYS_ANSI_VAR_FONT));
    m_pCtrlDisplayHeight->SetFont(wxSystemSettings::GetFont(wxSYS_ANSI_VAR_FONT));

    m_pCtrlApplyButton->Enable(false);
    m_pCfg->saveState();
    m_sSavedUserMxDir = m_sUserMxDir;
    m_sSavedSystemMxDir = m_sSystemMxDir;

    ((MxValidator *)(m_pCtrlHostname->GetValidator()))->SetKeyTyped(this);
    ((MxValidator *)(m_pCtrlPort->GetValidator()))->SetKeyTyped(this);
    ((MxValidator *)(m_pCtrlUserMxDir->GetValidator()))->SetKeyTyped(this);
    ((MxValidator *)(m_pCtrlSystemMxDir->GetValidator()))->SetKeyTyped(this);
    ((MxValidator *)(m_pCtrlFontserverHost->GetValidator()))->SetKeyTyped(this);
    ((MxValidator *)(m_pCtrlFontserverPort->GetValidator()))->SetKeyTyped(this);

    return TRUE;
}

void SessionProperties::UpdateDialogConstraints(bool getValues)
{
    if (getValues)
        TransferDataFromWindow();

    // 'General' tab
    switch (m_iSessionType) {
        case MxXmlConfig::STYPE_UNIX:
            m_pCtrlDesktopType->SetString(0, _("KDE"));
            m_pCtrlDesktopType->SetSelection(m_iDesktopTypeDialog);
            m_pCtrlDesktopType->Enable(true);
            m_pCtrlDesktopSettings->Enable(m_iDesktopTypeDialog == MxXmlConfig::DTYPE_CUSTOM);
            break;
        case MxXmlConfig::STYPE_WINDOWS:
            m_pCtrlDesktopType->SetString(0, _("RDP"));
            m_pCtrlDesktopType->SetSelection(0);
            m_iDesktopType = MxXmlConfig::DTYPE_RDP;
            m_iDesktopTypeDialog = 0;
            m_pCtrlDesktopType->Enable(false);
            m_pCtrlDesktopSettings->Enable(true);
            break;
        case MxXmlConfig::STYPE_VNC:
            m_pCtrlDesktopType->SetString(0, _("RFB"));
            m_pCtrlDesktopType->SetSelection(0);
            m_iDesktopType = MxXmlConfig::DTYPE_RFB;
            m_iDesktopTypeDialog = 0;
            m_pCtrlDesktopType->Enable(false);
            m_pCtrlDesktopSettings->Enable(true);
            break;
    }
    switch (m_iDisplayType) {
        case MxXmlConfig::DPTYPE_CUSTOM:
            m_pCtrlDisplayWidth->Enable(true);
            m_pCtrlDisplayHeight->Enable(true);
            break;
        default:
            m_pCtrlDisplayWidth->Enable(false);
            m_pCtrlDisplayHeight->Enable(false);
            break;
    }
    if (m_bUseSmartCard) {
        m_pCtrlEnableSSL->SetValue(true);
        m_pCtrlEnableSSL->Enable(false);
        m_bEnableSSL = true;
    } else
        m_pCtrlEnableSSL->Enable(true);

    m_pCtrlImageSettings->Enable(m_bUseCustomImageEncoding);

    // 'Services' tab
    m_pCtrlSmbShares->Enable(m_bEnableSmbSharing);
    m_pCtrlShareAdd->Enable(m_bEnableSmbSharing);
    m_pCtrlShareDelete->Enable(m_bEnableSmbSharing && (m_pCtrlSmbShares->GetSelectedItemCount() > 0));
    m_pCtrlShareModify->Enable(m_bEnableSmbSharing && (m_pCtrlSmbShares->GetSelectedItemCount() > 0));

    // 'Advanced' tab
    m_pCtrlKeyboardLayout->Enable(m_bKbdLayoutOther);

    // 'Environment' tab
    m_pCtrlFontserverHost->Enable(m_bUseFontServer);
    m_pCtrlFontserverPort->Enable(m_bUseFontServer);
}

/*!
 * Control creation for SessionProperties
 */

void SessionProperties::CreateControls()
{
////@begin SessionProperties content construction

    wxXmlResource::Get()->LoadDialog(this, GetParent(), _T("ID_DIALOG_PROPERTIES"));
    m_pNoteBook = XRCCTRL(*this, "ID_NOTEBOOK", wxNotebook);
    m_pCtrlHostname = XRCCTRL(*this, "ID_TEXTCTRL_HOST", wxTextCtrl);
    m_pCtrlPort = XRCCTRL(*this, "ID_TEXTCTRL_PORT", wxTextCtrl);
    m_pCtrlSessionType = XRCCTRL(*this, "ID_COMBOBOX_DPROTO", wxComboBox);
    m_pCtrlDesktopType = XRCCTRL(*this, "ID_COMBOBOX_DTYPE", wxComboBox);
    m_pCtrlDesktopSettings = XRCCTRL(*this, "ID_BUTTON_DSETTINGS", wxButton);
    m_pCtrlDisplayType = XRCCTRL(*this, "ID_COMBOBOX_DISPTYPE", wxComboBox);
    m_pCtrlDisplayWidth = XRCCTRL(*this, "ID_SPINCTRL_WIDTH", wxSpinCtrl);
    m_pCtrlDisplayHeight = XRCCTRL(*this, "ID_SPINCTRL_HEIGHT", wxSpinCtrl);
    m_pCtrlImageEncDefault = XRCCTRL(*this, "ID_RADIOBUTTON_IMG_DEFAULT", wxRadioButton);
    m_pCtrlImageEncCustom = XRCCTRL(*this, "ID_RADIOBUTTON_IMG_CUSTOM", wxRadioButton);
    m_pCtrlImageSettings = XRCCTRL(*this, "ID_BUTTON_IMG_CUSTOM", wxButton);
    m_pCtrlEnableSSL = XRCCTRL(*this, "ID_CHECKBOX_ENABLESSL", wxCheckBox);
    m_pCtrlBackingStore = XRCCTRL(*this, "ID_COMBOBOX_BACKINGSTORE", wxComboBox);
    m_pCtrlKeyboardCurrent = XRCCTRL(*this, "ID_RADIOBUTTON_KBDKEEP", wxRadioButton);
    m_pCtrlKeyboardOther = XRCCTRL(*this, "ID_RADIOBUTTON_KBDOTHER", wxRadioButton);
    m_pCtrlKeyboardLayout = XRCCTRL(*this, "ID_COMBOBOX_KBDLAYOUT", wxComboBox);
    m_pCtrlSmbEnable = XRCCTRL(*this, "ID_CHECKBOX_SMB", wxCheckBox);
    m_pCtrlSmbShares = XRCCTRL(*this, "ID_LISTCTRL_SMB_SHARES", wxListCtrl);
    m_pCtrlShareAdd = XRCCTRL(*this, "ID_BUTTON_SMB_ADD", wxButton);
    m_pCtrlShareModify = XRCCTRL(*this, "ID_BUTTON_SMB_MODIFY", wxButton);
    m_pCtrlShareDelete = XRCCTRL(*this, "ID_BUTTON_SMB_DELETE", wxButton);
    m_pCtrlUserMxDir = XRCCTRL(*this, "ID_TEXTCTRL_USERDIR", wxTextCtrl);
    m_pCtrlSystemMxDir = XRCCTRL(*this, "ID_TEXTCTRL_SYSDIR", wxTextCtrl);
    m_pCtrlUseFontserver = XRCCTRL(*this, "ID_CHECKBOX_FONTSRV", wxCheckBox);
    m_pCtrlFontserverHost = XRCCTRL(*this, "ID_TEXTCTRL_FSHOST", wxTextCtrl);
    m_pCtrlFontserverPort = XRCCTRL(*this, "ID_TEXTCTRL_FSPORT", wxTextCtrl);
    m_pCtrlFontDefault = XRCCTRL(*this, "ID_BUTTON_FONT_DEFAULT", wxButton);
    m_pCtrlFontFixed = XRCCTRL(*this, "ID_BUTTON_FONT_FIXED", wxButton);
    m_pCtrlApplyButton = XRCCTRL(*this, "wxID_APPLY", wxButton);
    // Set validators
    if (FindWindow(XRCID("ID_TEXTCTRL_HOST")))
        FindWindow(XRCID("ID_TEXTCTRL_HOST"))->SetValidator( MxValidator(MxValidator::MXVAL_HOST, & m_sHostName) );
    if (FindWindow(XRCID("ID_TEXTCTRL_PORT")))
        FindWindow(XRCID("ID_TEXTCTRL_PORT"))->SetValidator( MxValidator(MxValidator::MXVAL_NUMERIC, & m_iPort) );
    if (FindWindow(XRCID("ID_CHECKBOX_PWSAVE")))
        FindWindow(XRCID("ID_CHECKBOX_PWSAVE"))->SetValidator( wxGenericValidator(& m_bRememberPassword) );
    if (FindWindow(XRCID("ID_CHECKBOX_SMARTCARD")))
        FindWindow(XRCID("ID_CHECKBOX_SMARTCARD"))->SetValidator( wxGenericValidator(& m_bUseSmartCard) );
    if (FindWindow(XRCID("ID_COMBOBOX_DPROTO")))
        FindWindow(XRCID("ID_COMBOBOX_DPROTO"))->SetValidator( wxGenericValidator(& m_iSessionType) );
    if (FindWindow(XRCID("ID_COMBOBOX_DTYPE")))
        FindWindow(XRCID("ID_COMBOBOX_DTYPE"))->SetValidator( wxGenericValidator(& m_iDesktopTypeDialog) );
    if (FindWindow(XRCID("ID_SLIDER_SPEED")))
        FindWindow(XRCID("ID_SLIDER_SPEED"))->SetValidator( wxGenericValidator(& m_iConnectionSpeed) );
    if (FindWindow(XRCID("ID_COMBOBOX_DISPTYPE")))
        FindWindow(XRCID("ID_COMBOBOX_DISPTYPE"))->SetValidator( wxGenericValidator(& m_iDisplayType) );
    if (FindWindow(XRCID("ID_SPINCTRL_WIDTH")))
        FindWindow(XRCID("ID_SPINCTRL_WIDTH"))->SetValidator( wxGenericValidator(& m_iDisplayWidth) );
    if (FindWindow(XRCID("ID_SPINCTRL_HEIGHT")))
        FindWindow(XRCID("ID_SPINCTRL_HEIGHT"))->SetValidator( wxGenericValidator(& m_iDisplayHeight) );
    if (FindWindow(XRCID("ID_RADIOBUTTON_IMG_CUSTOM")))
        FindWindow(XRCID("ID_RADIOBUTTON_IMG_CUSTOM"))->SetValidator( wxGenericValidator(& m_bUseCustomImageEncoding) );
    if (FindWindow(XRCID("ID_CHECKBOX_DISABLETCPNODEL")))
        FindWindow(XRCID("ID_CHECKBOX_DISABLETCPNODEL"))->SetValidator( wxGenericValidator(& m_bDisableTcpNoDelay) );
    if (FindWindow(XRCID("ID_CHECKBOX_DISABLEZCOMP")))
        FindWindow(XRCID("ID_CHECKBOX_DISABLEZCOMP"))->SetValidator( wxGenericValidator(& m_bDisableZlibCompression) );
    if (FindWindow(XRCID("ID_CHECKBOX_ENABLESSL")))
        FindWindow(XRCID("ID_CHECKBOX_ENABLESSL"))->SetValidator( wxGenericValidator(& m_bEnableSSL) );
    if (FindWindow(XRCID("ID_COMBOBOX_CACHEMEM")))
        FindWindow(XRCID("ID_COMBOBOX_CACHEMEM"))->SetValidator( wxGenericValidator(& m_iCacheMem) );
    if (FindWindow(XRCID("ID_COMBOBOX_CACHEDISK")))
        FindWindow(XRCID("ID_COMBOBOX_CACHEDISK"))->SetValidator( wxGenericValidator(& m_iCacheDisk) );
    if (FindWindow(XRCID("ID_RADIOBUTTON_KBDOTHER")))
        FindWindow(XRCID("ID_RADIOBUTTON_KBDOTHER"))->SetValidator( wxGenericValidator(& m_bKbdLayoutOther) );
    if (FindWindow(XRCID("ID_COMBOBOX_KBDLAYOUT")))
        FindWindow(XRCID("ID_COMBOBOX_KBDLAYOUT"))->SetValidator( wxGenericValidator(& m_iKbdLayoutLanguage) );
    if (FindWindow(XRCID("ID_CHECKBOX_SMB")))
        FindWindow(XRCID("ID_CHECKBOX_SMB"))->SetValidator( wxGenericValidator(& m_bEnableSmbSharing) );
    if (FindWindow(XRCID("ID_CHECKBOX_MMEDIA")))
        FindWindow(XRCID("ID_CHECKBOX_MMEDIA"))->SetValidator( wxGenericValidator(& m_bEnableMultimedia) );
    if (FindWindow(XRCID("ID_TEXTCTRL_USERDIR")))
        FindWindow(XRCID("ID_TEXTCTRL_USERDIR"))->SetValidator( MxValidator(& m_sUserMxDir) );
    if (FindWindow(XRCID("ID_CHECKBOX_REMOVEOLDSF")))
        FindWindow(XRCID("ID_CHECKBOX_REMOVEOLDSF"))->SetValidator( wxGenericValidator(& m_bRemoveOldSessionFiles) );
    if (FindWindow(XRCID("ID_TEXTCTRL_SYSDIR")))
        FindWindow(XRCID("ID_TEXTCTRL_SYSDIR"))->SetValidator( MxValidator(& m_sSystemMxDir) );
    if (FindWindow(XRCID("ID_CHECKBOX_FONTSRV")))
        FindWindow(XRCID("ID_CHECKBOX_FONTSRV"))->SetValidator( wxGenericValidator(& m_bUseFontServer) );
    if (FindWindow(XRCID("ID_TEXTCTRL_FSHOST")))
        FindWindow(XRCID("ID_TEXTCTRL_FSHOST"))->SetValidator( MxValidator(MxValidator::MXVAL_HOST, & m_sFontServer) );
    if (FindWindow(XRCID("ID_TEXTCTRL_FSPORT")))
        FindWindow(XRCID("ID_TEXTCTRL_FSPORT"))->SetValidator( MxValidator(MxValidator::MXVAL_NUMERIC, & m_iFontServerPort) );
////@end SessionProperties content construction

    // Create custom windows not generated automatically here.

////@begin SessionProperties content initialisation

////@end SessionProperties content initialisation
}

/*!
 * Should we show tooltips?
 */

bool SessionProperties::ShowToolTips()
{
    return TRUE;
}

/*!
 * Get bitmap resources
 */

wxBitmap SessionProperties::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
    return CreateBitmapFromFile(name);
}

/*!
 * Get icon resources
 */

wxIcon SessionProperties::GetIconResource( const wxString& name )
{
    // Icon retrieval
    return CreateIconFromFile(name);
}

void SessionProperties::SetFontLabel(wxButton *b, const wxFont &f)
{
    wxString lbl = f.GetFaceName();
    lbl = wxString::Format(wxT("%s %dpt"), lbl.IsEmpty() ? _("Font") : lbl.c_str(), f.GetPointSize());
    b->SetFont(f);
    b->SetLabel(lbl);
    b->SetSize(b->GetBestSize());
    wxSizer *s = b->GetContainingSizer();
    if (s) {
        s->SetItemMinSize(b, b->GetBestSize().GetWidth(), b->GetBestSize().GetHeight());
        s->Layout();
    }
}

int SessionProperties::FindSelectedShare()
{
    for (int i = 0; i < m_pCtrlSmbShares->GetItemCount(); i++) {
        int state = m_pCtrlSmbShares->GetItemState(i, wxLIST_STATE_SELECTED);
        if (state)
            return i;
    }
    return -1;
}

// ====================== Event handlers ===============================

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_DSETTINGS
 */

void SessionProperties::OnButtonDsettingsClick( wxCommandEvent& event )
{
    switch (m_iSessionType) {
        case MxXmlConfig::STYPE_UNIX:
            {
                X11PropertyDialog d;
                d.SetConfig(m_pCfg);
                d.Create(this);
                d.ShowModal();
                CheckChanged();
            }
            break;
        case MxXmlConfig::STYPE_WINDOWS:
            {
                RdpPropertyDialog d;
                d.SetConfig(m_pCfg);
                d.Create(this);
                d.ShowModal();
                CheckChanged();
            }
            break;
        case MxXmlConfig::STYPE_VNC:
            {
                VncPropertyDialog d;
                d.SetConfig(m_pCfg);
                d.Create(this);
                d.ShowModal();
                CheckChanged();
            }
            break;
    }
    event.Skip();
}

/*!
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_IMG_DEFAULT
 */

void SessionProperties::OnRadiobuttonImgDefaultSelected( wxCommandEvent& event )
{
    UpdateDialogConstraints(true);
    CheckChanged();
    event.Skip();
}

/*!
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_IMG_CUSTOM
 */

void SessionProperties::OnRadiobuttonImgCustomSelected( wxCommandEvent& event )
{
    UpdateDialogConstraints(true);
    CheckChanged();
    event.Skip();
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_IMG_CUSTOM
 */

void SessionProperties::OnButtonImgCustomClick( wxCommandEvent& event )
{
    switch (m_iSessionType) {
        case MxXmlConfig::STYPE_UNIX:
            {
                UnixImageSettingsDialog d;
                d.SetConfig(m_pCfg);
                d.Create(this);
                d.ShowModal();
                CheckChanged();
            }
            break;
        case MxXmlConfig::STYPE_WINDOWS:
            {
                RdpImageSettingsDialog d;
                d.SetConfig(m_pCfg);
                d.Create(this);
                d.ShowModal();
                CheckChanged();
            }
            break;
        case MxXmlConfig::STYPE_VNC:
            {
                VncImageSettingsDialog d;
                d.SetConfig(m_pCfg);
                d.Create(this);
                d.ShowModal();
                CheckChanged();
            }
            break;
    }

    // Insert custom code here
    event.Skip();
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_CACHECLEAN
 */

void SessionProperties::OnButtonCachecleanClick( wxCommandEvent& event )
{
    // Insert custom code here
    event.Skip();
}

/*!
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_KBDKEEP
 */

void SessionProperties::OnRadiobuttonKbdkeepSelected( wxCommandEvent& event )
{
    UpdateDialogConstraints(true);
    CheckChanged();
    event.Skip();
}

/*!
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_KBDOTHER
 */

void SessionProperties::OnRadiobuttonKbdotherSelected( wxCommandEvent& event )
{
    UpdateDialogConstraints(true);
    CheckChanged();
    event.Skip();
}

/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_SMB
 */

void SessionProperties::OnCheckboxSmbClick( wxCommandEvent& event )
{
    UpdateDialogConstraints(true);
    CheckChanged();
    event.Skip();
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_SMB_ADD
 */

void SessionProperties::OnButtonSmbAddClick( wxCommandEvent& event )
{
    SmbShareProperties d;
    d.SetConfig(m_pCfg);
    d.Create(this);
    if (d.ShowModal() == wxID_OK) {
        WinShare s;
        size_t sgidx = m_pCfg->aGetShareGroups().GetCount() - 1;
        ArrayOfShares sa = s.GetShares();
        ShareGroup sg = m_pCfg->aGetShareGroups().Item(sgidx);
        int iconidx = (sg.m_iType == SharedResource::SHARE_DISK) ? 1 : 2;
        size_t si;
        wxString comment = _("Currently not available");
        for (si = 0; si < sa.GetCount(); si++) {
            if (sa[si].name == sg.m_sName) {
                comment = sa[si].description;
                break;
            }
        }
        int lidx = m_pCtrlSmbShares->InsertItem(0, sg.m_sName, iconidx);
        m_pCtrlSmbShares->SetItem(lidx, 1, sg.m_sMountPoint);
        m_pCtrlSmbShares->SetItem(lidx, 2, comment);
        m_pCtrlSmbShares->SetItemData(lidx, m_pCfg->aGetShareGroups().GetCount() - 1);
        m_pCtrlSmbShares->SetColumnWidth(0, wxLIST_AUTOSIZE);
        m_pCtrlSmbShares->SetColumnWidth(1, wxLIST_AUTOSIZE);
        m_pCtrlSmbShares->SetColumnWidth(2, wxLIST_AUTOSIZE);
    }
    CheckChanged();
    event.Skip();
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_SMB_MODIFY
 */

void SessionProperties::OnButtonSmbModifyClick( wxCommandEvent& event )
{
    int idx = FindSelectedShare();
    if (idx != -1) {
        SmbShareProperties d;
        d.SetConfig(m_pCfg);
        d.SetCurrentShare(m_pCtrlSmbShares->GetItemData(idx));
        d.Create(this);
        if (d.ShowModal() == wxID_OK) {
            ShareGroup sg = m_pCfg->aGetShareGroups().Item(m_pCtrlSmbShares->GetItemData(idx));
            m_pCtrlSmbShares->SetItem(idx, 1, sg.m_sMountPoint);
            m_pCtrlSmbShares->SetColumnWidth(1, wxLIST_AUTOSIZE);
        }
        CheckChanged();
    }
    event.Skip();
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_SMB_DELETE
 */

void SessionProperties::OnButtonSmbDeleteClick( wxCommandEvent& event )
{
    int idx = FindSelectedShare();
    if (idx != -1) {
        int shidx = m_pCtrlSmbShares->GetItemData(idx);
        wxArrayString as = m_pCfg->aGetUsedShareGroups();
        ArrayOfShareGroups asg = m_pCfg->aGetShareGroups();
        as.Remove(asg[shidx].m_sGroupName);
        asg.RemoveAt(shidx);
        m_pCfg->aSetShareGroups(asg);
        m_pCfg->aSetUsedShareGroups(as);
        m_pCfg->iSetUsedShareGroups(m_pCfg->iGetUsedShareGroups() - 1);
        m_pCtrlSmbShares->DeleteItem(idx);
        for (int i = 0; i < m_pCtrlSmbShares->GetItemCount(); i++) {
            int id = m_pCtrlSmbShares->GetItemData(i);
            if (id > shidx)
                m_pCtrlSmbShares->SetItemData(i, id - 1);
        }
        CheckChanged();
        UpdateDialogConstraints(false);
    }
    event.Skip();
}



/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_BROWSE_USERDIR
 */

void SessionProperties::OnButtonBrowseUserdirClick( wxCommandEvent& event )
{
    const wxString& dir = wxDirSelector(_("Select User MX directory"),
        m_sUserMxDir, 0, wxDefaultPosition, this);
    if (!dir.IsEmpty()) {
        m_pCtrlUserMxDir->SetValue(dir);
        CheckChanged();
    }
    event.Skip();
}



/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_BROWSE_SYSDIR
 */

void SessionProperties::OnButtonBrowseSysdirClick( wxCommandEvent& event )
{
    const wxString& dir = wxDirSelector(_("Select System MX directory"),
        m_sSystemMxDir, 0, wxDefaultPosition, this);
    if (!dir.IsEmpty()) {
        m_pCtrlSystemMxDir->SetValue(dir);
        CheckChanged();
    }
    event.Skip();
}



/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_FONT_DEFAULT
 */

void SessionProperties::OnButtonFontDefaultClick( wxCommandEvent& event )
{
    // Insert custom code here
    wxFont tmp = wxGetFontFromUser(this, m_cFontDefault);
    if (tmp.Ok()) {
        SetFontLabel(m_pCtrlFontDefault, tmp);
        m_cFontDefault = tmp;
        CheckChanged();
    }
    event.Skip();
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_FONT_FIXED
 */

void SessionProperties::OnButtonFontFixedClick( wxCommandEvent& event )
{
    // Insert custom code here
    wxFont tmp = wxGetFontFromUser(this, m_cFontFixed);
    if (tmp.Ok()) {
        SetFontLabel(m_pCtrlFontFixed, tmp);
        m_cFontFixed = tmp;
        CheckChanged();
    }
    event.Skip();
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_DELETE
 */

void SessionProperties::OnClearClick( wxCommandEvent& event )
{


    if (wxMessageBox(wxString::Format(_("Really delete Session '%s' ?"),
				      m_pCfg->sGetName().c_str()),
		     _("Delete Session"),
                     wxICON_QUESTION|wxYES_NO|wxNO_DEFAULT) == wxYES) {
#ifdef __WXMSW__
        TCHAR dtPath[MAX_PATH];
        if (SHGetSpecialFolderPath(NULL, dtPath, CSIDL_DESKTOPDIRECTORY, FALSE))
            wxRemoveFile(wxString::Format(_T("%s\\%s.lnk"), dtPath, m_pCfg->sGetName().mb_str()));
#endif
#ifdef __UNIX__
        const char* dtPaths[] = { "Desktop", "KDesktop", ".gnome-desktop", NULL };
        const char **p = dtPaths;
        
        while (*p) {
            wxRemoveFile(wxString::Format(_T("%s/%s/%s.desktop"),
                ::wxGetHomeDir().c_str(), *p, m_pCfg->sGetName().c_str()));
            p++;
        }
#endif

        EndModal(wxID_CLEAR);
    }
    event.Skip();
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_SAVE
 */

void SessionProperties::OnApplyClick( wxCommandEvent& event )
{
    m_pCfg->saveState();
    m_sSavedUserMxDir = m_sUserMxDir;
    m_sSavedSystemMxDir = m_sSystemMxDir;
    m_pCtrlApplyButton->Enable(false);
    event.Skip();
}

/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_FONTSRV
 */

void SessionProperties::OnCheckboxFontsrvClick( wxCommandEvent& event )
{
    UpdateDialogConstraints(true);
    CheckChanged();
    event.Skip();
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_ABOUT
 */

void SessionProperties::OnButtonAboutClick( wxCommandEvent& event )
{
    AboutDialog d(this);
    d.ShowModal();
    event.Skip();
}


/*!
 * wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_COMBOBOX_DPROTO
 */

void SessionProperties::OnComboboxDprotoSelected( wxCommandEvent& event )
{
    if (event.GetInt() == 0) {
        m_iDesktopTypeDialog = m_iUnixDesktopType;
        m_pCtrlDesktopType->SetSelection(m_iDesktopTypeDialog);
    }
    UpdateDialogConstraints(true);
    CheckChanged();
    event.Skip();
}

/*!
 * wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_COMBOBOX_DTYPE
 */

void SessionProperties::OnComboboxDtypeSelected( wxCommandEvent& event )
{
    UpdateDialogConstraints(true);
    m_iUnixDesktopType = m_iDesktopTypeDialog;
    CheckChanged();
    event.Skip();
}

/*!
 * wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_COMBOBOX_DISPTYPE
 */

void SessionProperties::OnComboboxDisptypeSelected( wxCommandEvent& event )
{
    UpdateDialogConstraints(true);
    CheckChanged();
    event.Skip();
}

/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_PWSAVE
 */

void SessionProperties::OnCheckboxPwsaveClick( wxCommandEvent& event )
{
    CheckChanged();
    event.Skip();
}

/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_SMARTCARD
 */

void SessionProperties::OnCheckboxSmartcardClick( wxCommandEvent& event )
{
    UpdateDialogConstraints(true);
    CheckChanged();
    event.Skip();
}

/*!
 * wxEVT_COMMAND_SLIDER_UPDATED event handler for ID_SLIDER_SPEED
 */

void SessionProperties::OnSliderSpeedUpdated( wxCommandEvent& event )
{
    CheckChanged();
    event.Skip();
}


/*!
 * wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL_WIDTH
 */

void SessionProperties::OnSpinctrlWidthUpdated( wxSpinEvent& event )
{
    CheckChanged();
    event.Skip();
}

/*!
 * wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL_HEIGHT
 */

void SessionProperties::OnSpinctrlHeightUpdated( wxSpinEvent& event )
{
    CheckChanged();
    event.Skip();
}

/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_DISABLETCPNODEL
 */

void SessionProperties::OnCheckboxDisabletcpnodelClick( wxCommandEvent& event )
{
    CheckChanged();
    event.Skip();
}

/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_DISABLEZCOMP
 */

void SessionProperties::OnCheckboxDisablezcompClick( wxCommandEvent& event )
{
    CheckChanged();
    event.Skip();
}

/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_ENABLESSL
 */

void SessionProperties::OnCheckboxEnablesslClick( wxCommandEvent& event )
{
    CheckChanged();
    event.Skip();
}

/*!
 * wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_COMBOBOX_CACHEMEM
 */

void SessionProperties::OnComboboxCachememSelected( wxCommandEvent& event )
{
    CheckChanged();
    event.Skip();
}

/*!
 * wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_COMBOBOX_CACHEDISK
 */

void SessionProperties::OnComboboxCachediskSelected( wxCommandEvent& event )
{
    CheckChanged();
    event.Skip();
}

/*!
 * wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_COMBOBOX_BACKINGSTORE
 */

void SessionProperties::OnComboboxBackingstoreSelected( wxCommandEvent& event )
{
    CheckChanged();
    event.Skip();
}

/*!
 * wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_COMBOBOX_KBDLAYOUT
 */

void SessionProperties::OnComboboxKbdlayoutSelected( wxCommandEvent& event )
{
    CheckChanged();
    event.Skip();
}

/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_REMOVEOLDSF
 */

void SessionProperties::OnCheckboxRemoveoldsfClick( wxCommandEvent& event )
{
    CheckChanged();
    event.Skip();
}

/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXTCTRL_HOST
 */

void SessionProperties::OnTextctrlHostUpdated( wxCommandEvent& event )
{
    if (m_bKeyTyped && (wxWindow::FindFocus() == (wxWindow *)m_pCtrlHostname))
        CheckChanged();
    // Insert custom code here
    event.Skip();
}

/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXTCTRL_PORT
 */

void SessionProperties::OnTextctrlPortUpdated( wxCommandEvent& event )
{
    if (m_bKeyTyped && (wxWindow::FindFocus() == (wxWindow *)m_pCtrlPort))
        CheckChanged();
    event.Skip();
}

/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXTCTRL_USERDIR
 */

void SessionProperties::OnTextctrlUserdirUpdated( wxCommandEvent& event )
{
    if (m_bKeyTyped && (wxWindow::FindFocus() == (wxWindow *)m_pCtrlUserMxDir))
        CheckChanged();
    event.Skip();
}

/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXTCTRL_SYSDIR
 */

void SessionProperties::OnTextctrlSysdirUpdated( wxCommandEvent& event )
{
    if (m_bKeyTyped && (wxWindow::FindFocus() == (wxWindow *)m_pCtrlSystemMxDir))
        CheckChanged();
    event.Skip();
}

/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXTCTRL_FSHOST
 */

void SessionProperties::OnTextctrlFshostUpdated( wxCommandEvent& event )
{
    if (m_bKeyTyped && (wxWindow::FindFocus() == (wxWindow *)m_pCtrlFontserverHost))
        CheckChanged();
    event.Skip();
}

/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXTCTRL_FSPORT
 */

void SessionProperties::OnTextctrlFsportUpdated( wxCommandEvent& event )
{
    if (m_bKeyTyped && (wxWindow::FindFocus() == (wxWindow *)m_pCtrlFontserverPort))
        CheckChanged();
    event.Skip();
}

/*!
 * wxEVT_COMMAND_LIST_ITEM_SELECTED event handler for ID_LISTCTRL_SMB_SHARES
 */

void SessionProperties::OnListctrlSmbSharesSelected( wxListEvent& event )
{
    m_pCtrlShareDelete->Enable(true);
    m_pCtrlShareModify->Enable(true);
    event.Skip();
}

/*!
 * wxEVT_COMMAND_LIST_ITEM_ACTIVATED event handler for ID_LISTCTRL_SMB_SHARES
 */

void SessionProperties::OnListctrlSmbSharesItemActivated( wxListEvent& event )
{
    wxCommandEvent e;
    OnButtonSmbModifyClick(e);
    event.Skip();
}


