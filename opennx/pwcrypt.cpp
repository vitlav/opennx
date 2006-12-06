/////////////////////////////////////////////////////////////////////////////
// Name:        pwcrypt.cpp
// Purpose:     Obfuscate passwords in config files
// Author:      Fritz Elfert
// Modified by: 
// Created:     06/13/04 22:44:20
// RCS-ID:      $Id$
// Copyright:   Copyright 2004 Millenux GmbH
// Licence:     LGPL
/////////////////////////////////////////////////////////////////////////////

#include <wx/datetime.h>
#include "pwcrypt.h"

static const wxString dummyString = _T("{{{{");
static const wxString validChars =
    _T("!#$%&()*+-.0123456789:;<>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[]_abcdefghijklmnopqrstuvwxyz{|}");

static wxChar
getRandomValidCharFromList()
{
  return validChars[(int)wxDateTime::Now().GetSecond()];
}

wxString
encodeString(const wxString &s)
{
    size_t i;
    wxString ret = _T("");

    if (s.Length()) {
        ret = _T(":");
        for (i = 0; i < s.Length(); i++)
            ret += wxString::Format(_T("%d:"), s[i] + i + 1);
    }
    return ret;
}

wxString
decodeString(const wxString &s)
{
    wxString val = s;
    wxString ret = _T("");

    if (val.Left(1) == _T(":") && val.Right(1) == _T(":") && val.Length() > 1) {
        int idx = 1;
        val.Remove(0, 1);
        while (val.Length()) {
            int p = val.Find(_T(":"));
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
    wxString sRet = _T("");
    
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
    wxString sRet = _T("");
    
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
    sRet = _T("");
    // Reverse string
    for (i = (str.Length() - 1); (int)i >= 0; i--)
        sRet += str[i];
    
    if (sRet.StartsWith(dummyString))
        sRet.Remove(0, dummyString.Length());

    return decodeString(sRet);
}
