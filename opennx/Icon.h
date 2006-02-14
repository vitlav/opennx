/////////////////////////////////////////////////////////////////////////////
// Name:        Icon.h
// Purpose:     Generic Icon loader
// Author:      Fritz Elfert
// Modified by: 
// Created:     06/13/04 22:47:28
// RCS-ID:      $Id: Icon.h 795 2004-08-03 18:30:08Z felfert $
// Copyright:   Copyright 2004 Millenux GmbH
// Licence:     LGPL
/////////////////////////////////////////////////////////////////////////////

#ifndef _ICON_H_
#define _ICON_H_

extern wxIcon CreateIconFromFile(const wxString &, int width = -1, int height = -1);
extern wxBitmap CreateBitmapFromFile(const wxString &, int width = -1, int height = -1);
#endif
