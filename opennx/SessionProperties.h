// $Id$
//
// Copyright (C) 2006 The OpenNX Team
// Author: Fritz Elfert
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU LibroxyCommary General Public License as
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

#ifndef _SESSIONPROPERTIES_H_
#define _SESSIONPROPERTIES_H_

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "SessionProperties.cpp"
#endif

/*!
 * Includes
 */

////@begin includes
#include "SessionProperties_symbols.h"
#include "wx/xrc/xmlres.h"
#include "wx/notebook.h"
#include "wx/spinctrl.h"
#include "wx/valgen.h"
#include "wx/listctrl.h"
#include "wx/html/htmlwin.h"
////@end includes
#include "wx/sizer.h"

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxNotebook;
class wxSpinCtrl;
class wxListCtrl;
class extHtmlWindow;
////@end forward declarations
class wxFont;

#include "MyValidator.h"
#include "MyXmlConfig.h"

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_DIALOG_PROPERTIES 10006
#define SYMBOL_SESSIONPROPERTIES_STYLE wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_SESSIONPROPERTIES_TITLE _("Session properties - OpenNX")
#define SYMBOL_SESSIONPROPERTIES_IDNAME ID_DIALOG_PROPERTIES
#define SYMBOL_SESSIONPROPERTIES_SIZE wxDefaultSize
#define SYMBOL_SESSIONPROPERTIES_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

class KbdLayout;
WX_DECLARE_OBJARRAY(KbdLayout, KbdLayoutTable);

/*!
 * SessionProperties class declaration
 */

class SessionProperties: public wxDialog, KeyTypeCallback
{    
    DECLARE_CLASS( SessionProperties )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    SessionProperties( );
    SessionProperties( wxWindow* parent, wxWindowID id = SYMBOL_SESSIONPROPERTIES_IDNAME, const wxString& caption = SYMBOL_SESSIONPROPERTIES_TITLE, const wxPoint& pos = SYMBOL_SESSIONPROPERTIES_POSITION, const wxSize& size = SYMBOL_SESSIONPROPERTIES_SIZE, long style = SYMBOL_SESSIONPROPERTIES_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_SESSIONPROPERTIES_IDNAME, const wxString& caption = SYMBOL_SESSIONPROPERTIES_TITLE, const wxPoint& pos = SYMBOL_SESSIONPROPERTIES_POSITION, const wxSize& size = SYMBOL_SESSIONPROPERTIES_SIZE, long style = SYMBOL_SESSIONPROPERTIES_STYLE );

    void UpdateDialogConstraints(bool);
    void SetConfig(MyXmlConfig *config);

private:
    /// Creates the controls and sizers
    void CreateControls();
    /**
     * Checks for any changes and Enables/Disables the Apply-Button
     */
    void CheckChanged();
    /**
     * Handler for OnChar events.
     */
    virtual void KeyTyped();
    /**
     * Installs event handler for OnChar event in all wxTextCtrl and wxSpinCtrl
     * childs.
     */
    void InstallOnCharHandlers(wxWindow *w = NULL); 

public:

    void OnFocus( wxFocusEvent& event );

////@begin SessionProperties event handler declarations

    /// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXTCTRL_HOST
    void OnTextctrlHostUpdated( wxCommandEvent& event );

    /// wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL_PORT
    void OnSpinctrlPortUpdated( wxSpinEvent& event );

    /// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_SPINCTRL_PORT
    void OnTextctrlPortUpdated( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_PWSAVE
    void OnCheckboxPwsaveClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_KEYMANAGE
    void OnButtonKeymanageClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_SMARTCARD
    void OnCheckboxSmartcardClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_COMBOBOX_DPROTO
    void OnComboboxDprotoSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_COMBOBOX_DTYPE
    void OnComboboxDtypeSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_DSETTINGS
    void OnButtonDsettingsClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_SLIDER_UPDATED event handler for ID_SLIDER_SPEED
    void OnSliderSpeedUpdated( wxCommandEvent& event );

    /// wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_COMBOBOX_DISPTYPE
    void OnComboboxDisptypeSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL_WIDTH
    void OnSpinctrlWidthUpdated( wxSpinEvent& event );

    /// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_SPINCTRL_WIDTH
    void OnSpinctrlWidthTextUpdated( wxCommandEvent& event );

    /// wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL_HEIGHT
    void OnSpinctrlHeightUpdated( wxSpinEvent& event );

    /// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_SPINCTRL_HEIGHT
    void OnSpinctrlHeightTextUpdated( wxCommandEvent& event );

    /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_IMG_DEFAULT
    void OnRadiobuttonImgDefaultSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_IMG_CUSTOM
    void OnRadiobuttonImgCustomSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_IMG_CUSTOM
    void OnButtonImgCustomClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_DISABLETCPNODEL
    void OnCheckboxDisabletcpnodelClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_DISABLEZCOMP
    void OnCheckboxDisablezcompClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_ENABLESSL
    void OnCheckboxEnablesslClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_NOPROXY
    void OnRadiobuttonNoproxySelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_HTTPPROXY
    void OnRadiobuttonHttpproxySelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXTCTRL_PROXYHOST
    void OnTextctrlProxyhostUpdated( wxCommandEvent& event );

    /// wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL_PROXYPORT
    void OnSpinctrlProxyportUpdated( wxSpinEvent& event );

    /// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_SPINCTRL_PROXYPORT
    void OnSpinctrlProxyportTextUpdated( wxCommandEvent& event );

    /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_EXTERNALPROXY
    void OnRadiobuttonExternalproxySelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXTCTRL_PROXYCOMMAND
    void OnTextctrlProxycommandTextUpdated( wxCommandEvent& event );

    /// wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_COMBOBOX_CACHEMEM
    void OnComboboxCachememSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_COMBOBOX_CACHEDISK
    void OnComboboxCachediskSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_CACHECLEAN
    void OnButtonCachecleanClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_KBDKEEP
    void OnRadiobuttonKbdkeepSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_KBDOTHER
    void OnRadiobuttonKbdotherSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_COMBOBOX_KBDLAYOUT
    void OnComboboxKbdlayoutSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_SMB
    void OnCheckboxSmbClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_CUPSENABLE
    void OnCheckboxCupsenableClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL_CUPSPORT
    void OnSpinctrlCupsportUpdated( wxSpinEvent& event );

    /// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_SPINCTRL_CUPSPORT
    void OnSpinctrlCupsportTextUpdated( wxCommandEvent& event );

    /// wxEVT_COMMAND_LIST_ITEM_SELECTED event handler for ID_LISTCTRL_SMB_SHARES
    void OnListctrlSmbSharesSelected( wxListEvent& event );

    /// wxEVT_COMMAND_LIST_ITEM_ACTIVATED event handler for ID_LISTCTRL_SMB_SHARES
    void OnListctrlSmbSharesItemActivated( wxListEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_SMB_ADD
    void OnButtonSmbAddClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_SMB_MODIFY
    void OnButtonSmbModifyClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_SMB_DELETE
    void OnButtonSmbDeleteClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_MMEDIA
    void OnCheckboxMmediaClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_USBENABLE
    void OnCHECKBOXUSBENABLEClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_LIST_ITEM_SELECTED event handler for ID_LISTCTRL_USBFILTER
    void OnListctrlUsbfilterSelected( wxListEvent& event );

    /// wxEVT_COMMAND_LIST_ITEM_ACTIVATED event handler for ID_LISTCTRL_USBFILTER
    void OnListctrlUsbfilterItemActivated( wxListEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_USBADD
    void OnButtonUsbaddClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_USBMODIFY
    void OnButtonUsbmodifyClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_USBDELETE
    void OnButtonUsbdeleteClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXTCTRL_USERDIR
    void OnTextctrlUserdirUpdated( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_BROWSE_USERDIR
    void OnButtonBrowseUserdirClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_REMOVEOLDSF
    void OnCheckboxRemoveoldsfClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXTCTRL_SYSDIR
    void OnTextctrlSysdirUpdated( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_BROWSE_SYSDIR
    void OnButtonBrowseSysdirClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXTCTRL_CUPSPATH
    void OnTextctrlCupspathUpdated( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_BROWSE_CUPSPATH
    void OnButtonBrowseCupspathClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXTCTRL_USBIPD_SOCKET
    void OnTextctrlUsbipdSocketTextUpdated( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_BROWSE_USBIPD_SOCKET
    void OnButtonBrowseUsbipdSocketClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL_USB_LOCALPORT
    void OnSpinctrlUsbLocalportUpdated( wxSpinEvent& event );

    /// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_SPINCTRL_USB_LOCALPORT
    void OnSpinctrlUsbLocalportTextUpdated( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_FONT_DEFAULT
    void OnButtonFontDefaultClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_FONT_FIXED
    void OnButtonFontFixedClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_DELETE
    void OnDeleteClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_APPLY
    void OnApplyClick( wxCommandEvent& event );

////@end SessionProperties event handler declarations

////@begin SessionProperties member function declarations

    wxString GetsUserNxDir() const { return m_sUserNxDir ; }
    void SetsUserNxDir(wxString value) { m_sUserNxDir = value ; }

    wxString GetsSystemNxDir() const { return m_sSystemNxDir ; }
    void SetsSystemNxDir(wxString value) { m_sSystemNxDir = value ; }

    int GetUsbLocalPort() const { return m_iUsbLocalPort ; }
    void SetUsbLocalPort(int value) { m_iUsbLocalPort = value ; }

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end SessionProperties member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

private:
////@begin SessionProperties member variables
    wxNotebook* m_pNoteBook;
    wxTextCtrl* m_pCtrlHostname;
    wxSpinCtrl* m_pCtrlPort;
    wxCheckBox* m_pCtrlUseSmartCard;
    wxComboBox* m_pCtrlSessionType;
    wxComboBox* m_pCtrlDesktopType;
    wxButton* m_pCtrlDesktopSettings;
    wxComboBox* m_pCtrlDisplayType;
    wxSpinCtrl* m_pCtrlDisplayWidth;
    wxSpinCtrl* m_pCtrlDisplayHeight;
    wxRadioButton* m_pCtrlImageEncDefault;
    wxRadioButton* m_pCtrlImageEncCustom;
    wxButton* m_pCtrlImageSettings;
    wxCheckBox* m_pCtrlEnableSSL;
    wxTextCtrl* m_pCtrlProxyHost;
    wxSpinCtrl* m_pCtrlProxyPort;
    wxTextCtrl* m_pCtrlProxyCommand;
    wxRadioButton* m_pCtrlKeyboardCurrent;
    wxRadioButton* m_pCtrlKeyboardOther;
    wxComboBox* m_pCtrlKeyboardLayout;
    wxCheckBox* m_pCtrlSmbEnable;
    wxCheckBox* m_pCtrlCupsEnable;
    wxSpinCtrl* m_pCtrlCupsPort;
    wxListCtrl* m_pCtrlSmbShares;
    wxButton* m_pCtrlShareAdd;
    wxButton* m_pCtrlShareModify;
    wxButton* m_pCtrlShareDelete;
    wxCheckBox* m_pCtrlUsbEnable;
    wxListCtrl* m_pCtrlUsbFilter;
    wxButton* m_pCtrlUsbAdd;
    wxButton* m_pCtrlUsbModify;
    wxButton* m_pCtrlUsbDelete;
    wxTextCtrl* m_pCtrlUserNxDir;
    wxTextCtrl* m_pCtrlSystemNxDir;
    wxTextCtrl* m_pCtrlCupsPath;
    wxButton* m_pCtrlCupsBrowse;
    wxPanel* m_pCtrlUsbipdDaemon;
    wxTextCtrl* m_pCtrlUsbIpdSocket;
    wxButton* m_pCtrlUsbipdSocketBrowse;
    wxSpinCtrl* m_pCtrlUsbLocalPort;
    wxButton* m_pCtrlFontDefault;
    wxButton* m_pCtrlFontFixed;
    wxPanel* m_pCtrlPanelAbout;
    extHtmlWindow* m_pHtmlWindow;
    wxButton* m_pCtrlApplyButton;
private:
    int m_iPort;
    wxString m_sHostName;
    int m_iDisplayWidth;
    int m_iDisplayHeight;
    bool m_bRememberPassword;
    bool m_bUseSmartCard;
    int m_iSessionType;
    int m_iConnectionSpeed;
    int m_iDesktopType;
    int m_iDisplayType;
    bool m_bDisableTcpNoDelay;
    bool m_bDisableZlibCompression;
    bool m_bEnableSSL;
    int m_iCacheMem;
    int m_iCacheDisk;
    bool m_bKbdLayoutOther;
    wxString m_sKbdLayoutLanguage;
    bool m_bEnableSmbSharing;
    bool m_bEnableMultimedia;
    wxString m_sUserNxDir;
    wxString m_sSystemNxDir;
    bool m_bRemoveOldSessionFiles;
    wxString m_sCupsPath;
    int m_iCupsPort;
    bool m_bUseDefaultImageEncoding;
    bool m_bUseCustomImageEncoding;
    int m_iDesktopTypeDialog;
    int m_iProxyPort;
    wxString m_sProxyHost;
    bool m_bUseProxy;
    bool m_bUseCups;
    bool m_bEnableUSBIP;
    wxString m_sProxyCommand;
    bool m_bExternalProxy;
    int m_iUsbLocalPort;
    wxString m_sUsbipdSocket;
    int m_iSavedUsbLocalPort;
////@end SessionProperties member variables

    bool readKbdLayouts();
    void setFontLabel(wxButton *, const wxFont &);
    int findSelectedShare();
#ifdef SUPPORT_USBIP
    int findSelectedUsbDevice();
    void appendUsbDevice(SharedUsbDevice &, int);
#endif
    void updateListCtrlColumnWidth(wxListCtrl *);
    void removePage(const wxString &);

    wxFont m_cFontDefault;
    wxFont m_cFontFixed;
    wxString m_sSavedUserNxDir;
    wxString m_sSavedSystemNxDir;
    wxString m_sSavedUsbipdSocket;
    bool m_bKeyTyped;
    bool m_bStorePasswords;
    int m_iUnixDesktopType;

    MyXmlConfig *m_pCfg;
    KbdLayoutTable m_aKbdLayoutTable;
    ArrayOfUsbForwards m_aUsbForwards;
};

#endif
    // _SESSIONPROPERTIES_H_
