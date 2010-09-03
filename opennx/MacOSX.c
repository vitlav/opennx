/* $Id$
 *
 * Copyright (C) 2006 The OpenNX Team
 * Author: Fritz Elfert
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Library General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc.,
 * 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

/*
 * Defines canonicalized platform names (e.g. __LINUX__)
 */
#include <wx/platform.h>

#ifdef __WXMAC__
/*
 * Allow app to be started from the commandline
 */
#include <ApplicationServices/ApplicationServices.h>
static void __attribute__ ((constructor))
    makeForegroundApp()
{
    ProcessSerialNumber PSN;
    if (noErr == GetCurrentProcess(&PSN)) {
# ifdef APP_WATCHREADER
        TransformProcessType(&PSN, 0);
# else
        TransformProcessType(&PSN, kProcessTransformToForegroundApplication);
# endif
    }
}

#include <Carbon/Carbon.h>
void getMacKeyboard() {
	KeyboardLayoutRef klr;
	if (noErr == KLGetCurrentKeyboardLayout(&klr)) {
		uint32 pt = kKLName;
		const void *oValue;
		if (noErr == KLGetKeyboardLayoutProperty(klr, pt, &oValue)) {
			char buf[1024];
			if (CFStringGetCString((CFStringRef)oValue, buf, sizeof(buf), kCFStringEncodingISOLatin1)) {
				fprintf(stderr, "kbdlayout='%s'\n", buf); fflush(stderr);
			}
		}
	}
}
#endif
