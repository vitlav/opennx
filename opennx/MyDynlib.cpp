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
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "MyDynlib.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/defs.h"
#endif

#include "MyDynlib.h"

#include <wx/dynlib.h>
#include <wx/log.h>
#include <wx/utils.h>
#include <wx/tokenzr.h>

#include "trace.h"
ENABLE_TRACE;

bool MyDynamicLibrary::Load(const wxString& name, int flags /* = wxDL_DEFAULT */)
{
    ::wxLogTrace(MYTRACETAG, wxT("Load(%s, 0x%0x)"), name.c_str(), flags);
#ifdef __WXMSW__
    return wxDynamicLibrary::Load(name, flags);
#else
    wxString ldpath;
    if (::wxGetEnv(wxT("LD_LIBRARY_PATH"), &ldpath)) {
        wxStringTokenizer t(ldpath, wxT(":"));
        while (t.HasMoreTokens()) {
            wxString abslib = t.GetNextToken() + wxFileName::GetPathSeparator() + name;
            ::wxLogTrace(MYTRACETAG, wxT("Trying to load %s"), abslib.c_str());
            if (wxDynamicLibrary::Load(abslib))
                return true;
        }
    }
    ::wxLogTrace(MYTRACETAG, wxT("Falling back to default load"));
    return wxDynamicLibrary::Load(name, flags);
#endif
}
