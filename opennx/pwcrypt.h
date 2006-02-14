/////////////////////////////////////////////////////////////////////////////
// Name:        pwcrypt.h
// Purpose:     Obfuscate passwords in config files
// Author:      Fritz Elfert
// Modified by: 
// Created:     06/13/04 22:44:20
// RCS-ID:      $Id: pwcrypt.h 813 2004-08-06 00:41:57Z felfert $
// Copyright:   Copyright 2004 Millenux GmbH
// Licence:     LGPL
/////////////////////////////////////////////////////////////////////////////

#ifndef _PWCRYPT_H_
#define _PWCRYPT_H_

#include <wx/string.h>

extern wxString encodeString(const wxString &);
extern wxString decodeString(const wxString &);
extern wxString cryptString(const wxString &);
extern wxString decryptString(const wxString &);

#endif
// _PWCRYPT_H_
