// $Id$
//
// Copyright (C) 2009 The OpenNX Team
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

#ifndef _USBIP_H_
#define _USBIP_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "UsbIp.h"
#endif

#ifdef SUPPORT_USBIP

# include <wx/event.h>
# include <wx/dynarray.h>

class wxSocketClient;
class wxSocketEvent;

class UsbIpDevice : public wxObject {
    public:
        UsbIpDevice() : wxObject() { }
        virtual ~UsbIpDevice() {}

        wxString toString() const;
        wxString GetUsbBusID() const {
            return wxString::Format(wxT("%d-%d"), m_iUsbBusnum, m_iUsbDevnum);
        }
        const wxString& GetUsbIpBusID() const { return m_sUsbIpBusId; }

    private:
        friend class UsbIp;

        wxString m_sUsbIpBusId;
        wxString m_sDriver;
        wxString m_sConfig;
        int m_iUsbBusnum;
        int m_iUsbDevnum;
        int m_iVendorID;
        int m_iProductID;
};

WX_DECLARE_OBJARRAY(UsbIpDevice, ArrayOfUsbIpDevices);

class UsbIp : public wxEvtHandler {
    DECLARE_CLASS(UsbIp);
    DECLARE_EVENT_TABLE();

    public:
        UsbIp();
        virtual ~UsbIp();

        bool Connect(const wxString &);
        void SetSession(const wxString &);
        bool ExportDevice(const wxString &);
        bool UnexportDevice(const wxString &);
        ArrayOfUsbIpDevices GetDevices();

        bool IsConnected() { return m_bConnected; }
        bool HasError();
        bool Wait(long, long);

    private:
        typedef enum {
            None,
            Terminating,
            Initializing,
            Idle,
            Exporting,
            UnExporting,
            ListSessions,
            ListDevices,
            Exported,
            UnExported,
            GotSessions,
            GotDevices,
        } tStates;

        virtual void OnSocketEvent(wxSocketEvent &);
        void parse(const wxString &);
        void parsesession(const wxString &);
        void parsedevice(const wxString &);
        bool findsession(const wxString &);
        bool waitforstate(tStates, long state = 5000);
        bool send(const wxChar *fmt, ...) ATTRIBUTE_PRINTF_1;

        wxSocketClient *m_pSocketClient;
        wxString m_sSid;
        wxString m_sLineBuffer;
        bool m_bConnected;
        bool m_bError;
        tStates m_eState;
        wxArrayString m_aSessions;
        ArrayOfUsbIpDevices m_aDevices;
};

#endif
// SUPPORT_USBIP

#endif
// _USBIP_H_
