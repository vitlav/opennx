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

#ifndef _USBLINUX_H_
#define _USBLINUX_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "LibUSB.h"
#endif

#include <wx/dynarray.h>
#include <wx/string.h>
#include <wx/object.h>

class USB;

class USBDevice : public wxObject {
    public:
        USBDevice(int, int, unsigned char);

        wxString toString();

    private:
        int m_iVendor;
        int m_iProduct;
        unsigned char m_iClass;
        wxString m_sVendor;
        wxString m_sProduct;
        wxString m_sSerial;

        friend class USB;
};

WX_DECLARE_OBJARRAY(USBDevice, ArrayOfUSBDevices);

class wxDynamicLibrary;
struct usb_device;

class USB {
    public:
        USB();
        ArrayOfUSBDevices GetDevices();

    private:
        void adddev(wxDynamicLibrary *, struct usb_device *, unsigned char);
        void usbscan(wxDynamicLibrary *);

        ArrayOfUSBDevices m_aDevices;
};

#endif
// _USBLINUX_H_
