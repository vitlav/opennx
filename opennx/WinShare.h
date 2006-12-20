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

#ifndef _WINSHARE_H_
#define _WINSHARE_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "WinShare.h"
#endif

#include <wx/dynarray.h>
#include <wx/string.h>
#include <wx/object.h>

class DllData;

class SharedResource : public wxObject
{
    public:
        typedef enum {
            SHARE_UNKNOWN,
            SHARE_SMB_DISK,
            SHARE_SMB_PRINTER,
            SHARE_CUPS_PRINTER,
        } ShareType;

        SharedResource() : wxObject() {}
        virtual ~SharedResource() {}

        wxString name;
        wxString description;
        ShareType sharetype;
};

WX_DECLARE_OBJARRAY(SharedResource, ArrayOfShares);

class CupsClient {
    public:
        CupsClient();
        ~CupsClient();

        ArrayOfShares GetShares();
        bool IsAvailable();

    private:
        DllData *dllPrivate;
        ArrayOfShares m_shares;
};

class SmbClient {
    public:
        SmbClient();
        ~SmbClient();

        ArrayOfShares GetShares();
        bool IsAvailable();

    private:
        DllData *dllPrivate;
        ArrayOfShares m_shares;
};

#endif
// _WINSHARE_H_
