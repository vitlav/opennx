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

#include <wx/datetime.h>
#include "pwcrypt.h"

static const wxString dummyString = wxT("{{{{");
static const wxString validChars =
    wxT("!#$%&()*+-.0123456789:;<>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[]_abcdefghijklmnopqrstuvwxyz{|}");

static wxChar
getRandomValidCharFromList()
{
  return validChars[(int)wxDateTime::Now().GetSecond()];
}

wxString
encodeString(const wxString &s)
{
    size_t i;
    wxString ret = wxT("");

    if (s.Length()) {
        ret = wxT(":");
        for (i = 0; i < s.Length(); i++)
            ret += wxString::Format(wxT("%d:"), s[i] + i + 1);
    }
    return ret;
}

wxString
decodeString(const wxString &s)
{
    wxString val = s;
    wxString ret = wxT("");

    if (val.Left(1) == wxT(":") && val.Right(1) == wxT(":") && val.Length() > 1) {
        int idx = 1;
        val.Remove(0, 1);
        while (val.Length()) {
            int p = val.Find(wxT(":"));
            if (p != -1) {
                long l;
                val.Left(p).ToLong(&l);
                ret += wxChar(l - idx++);
            } else
                break;
            val.Remove(0, p + 1);
        }
    }
    return ret;
}

wxString
cryptString(const wxString &s)
{
    size_t i;
    wxString sRet = wxT("");
    
    if (s.IsEmpty())
        return s;
    
    wxString str = encodeString(s);
    
    if (str.Length() < 32)
        str += dummyString;
    
    // Reverse string
    for (i = (str.Length() - 1); (int)i >= 0; i--)
        sRet += str[i];
    
    if (sRet.Length() < 32)
        sRet += dummyString;
    
    wxChar k = getRandomValidCharFromList();
    int l = (k + sRet.Length()) - 2;
    sRet.Prepend(k);

    for (i = 1; i < sRet.Length(); i++) {
        int j = validChars.Find(sRet[i]);
        if (j == -1)
            return s;
        sRet.SetChar(i, validChars[(j + l * (i + 1)) % validChars.Length()]);
    }

    wxChar c = getRandomValidCharFromList() + 2;
    sRet.Append(c);
    
    return sRet;
}

wxString
decryptString(const wxString &s)
{
    size_t i;
    wxString sRet = wxT("");
    
    if (s.IsEmpty() || s.Length() < 5)
        return s;
    
    sRet = s;
    sRet.Truncate(s.Length() - 1);
    
    int n = (sRet.c_str()[0] + sRet.Length()) - 3;
    
    for (i = 1; i < sRet.Length(); i++) {
        int j = validChars.Find(sRet[i]);
        if( j == -1)
            return s;
        
        int k = j - (n * (i + 1)) % validChars.Length();
        
        if (k < 0)
            k = validChars.Length() + k;
        sRet.SetChar(i, validChars[k]);
    }
    
    sRet.Remove(0, 1);
    
    if (sRet.StartsWith(dummyString))
        sRet.Remove(0, dummyString.Length());
    
    wxString str = sRet;
    sRet = wxT("");
    // Reverse string
    for (i = (str.Length() - 1); (int)i >= 0; i--)
        sRet += str[i];
    
    if (sRet.StartsWith(dummyString))
        sRet.Remove(0, dummyString.Length());

    return decodeString(sRet);
}
