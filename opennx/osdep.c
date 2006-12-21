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

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <dlfcn.h>

/*
 * Defines canonicalized platform names (e.g. __LINUX__)
 */
#include <wx/platform.h>

typedef int (*PFconnect)(int s, const struct sockaddr *name, socklen_t namelen);
static PFconnect real_connect = NULL;
static void *libc = NULL;
static int do_save = 1;
static char _spath[PATH_MAX+1];
static char _kbd[PATH_MAX+1];

char *x11_socket_path = _spath;
char *x11_keyboard_type = _kbd;

int connect(int s, const struct sockaddr *name, socklen_t namelen)
{
    if (do_save) {
        struct sockaddr_un *a = (struct sockaddr_un *)name;
        if (a->sun_family == AF_UNIX) {
            strncpy(_spath, a->sun_path, sizeof(_spath));
            do_save = 0;
        }
    }
    return real_connect(s, name, namelen);
}

static void __attribute__ ((constructor))
getx11socket()
{
    memset(&_spath, 0, sizeof(_spath));
    memset(&_kbd, 0, sizeof(_kbd));
#define NotImplemented
#ifdef __LINUX__
    libc = dlopen("libc.so.6", RTLD_NOW);
# undef NotImplemented
#endif
#ifdef __OPENBSD__
    libc = dlopen("libc.so", RTLD_NOW);
# undef NotImplemented
#endif
#ifdef NotImplemented
# error Specify libc name
#endif
    if (!libc) {
        fprintf(stderr, "Can't load libc: %s\n", dlerror());
        exit(1);
    }
    real_connect = dlsym(libc, "connect");
    if (!real_connect) {
        fprintf(stderr, "Can't find symbol connect in libc: %s\n", dlerror());
        exit(1);
    }
    Display *dpy = XOpenDisplay(NULL);
    if (dpy) {
        Atom a = XInternAtom(dpy, "_XKB_RULES_NAMES", True);
        if (a != None) {
            Atom type;
            int fmt;
            unsigned long n;
            unsigned long ba;
            char *prop;
            int res = XGetWindowProperty(dpy,
                    RootWindowOfScreen(DefaultScreenOfDisplay(dpy)),
                    a, 0, PATH_MAX, False, AnyPropertyType,
                    &type, &fmt, &n, &ba, (unsigned char **)&prop);
            if ((res == Success) && (fmt == 8) && prop) {
                unsigned long i = 0;
                int idx = 0;
                while (i < n) {
                    switch (idx++) {
                        case 1:
                            strncat(_kbd, prop, PATH_MAX);
                            strncat(_kbd, "/", PATH_MAX);
                            break;
                        case 2:
                            strncat(_kbd, prop, PATH_MAX);
                            break;
                    }
                    i += strlen(prop) + 1;
                    prop += strlen(prop) + 1;
                }
            }
        }
        XCloseDisplay(dpy);
    }
}

static void __attribute__ ((destructor))
free_libc()
{
    if (libc)
        dlclose(libc);
}

void close_foreign(long parentID)
{
#if defined(__LINUX__) || defined(__OPENBSD__)
    Display *dpy = XOpenDisplay(NULL);
    if (dpy) {
        XClientMessageEvent ev;
        ev.type = ClientMessage;
        ev.window = parentID;
        ev.message_type = XInternAtom(dpy, "WM_PROTOCOLS", True);
        ev.format = 32;
        ev.data.l[0] = XInternAtom(dpy, "WM_DELETE_WINDOW", True);
        ev.data.l[1] = CurrentTime;
        XSendEvent(dpy, parentID, False, 0, (XEvent *)&ev);
        XCloseDisplay(dpy);
    }
#else
#error Implement reparent_pulldown
#endif
}

void reparent_pulldown(long parentID)
{
#if defined(__LINUX__) || defined(__OPENBSD__)
    Display *dpy = XOpenDisplay(NULL);
    if (dpy) {
        Atom a = XInternAtom(dpy, "_NET_WM_PID", True);
        if (a != None) {
            int s;
            for (s = 0; s < ScreenCount(dpy); s++) {
                Window root = RootWindow(dpy, s);
                Window dummy;
                Window *childs = NULL;
                int nchilds = 0;
                if (XQueryTree(dpy, root, &dummy, &dummy, &childs, &nchilds)) {
                    int c;
                    for (c = 0; c < nchilds; c++) {
                        Window w = childs[c];
                        if (w != None) {
                            Atom type;
                            int fmt;
                            unsigned long n;
                            unsigned long ba;
                            unsigned long pid;
                            unsigned long *prop;
                            int res = XGetWindowProperty(dpy, w, a, 0, 32, False,
                                    XA_CARDINAL, &type, &fmt, &n, &ba, (unsigned char **)&prop);
                            if ((res == Success) && (fmt = 32) && (n == 1) && prop) {
                                if (*prop == getpid()) {
                                    int pw, cw, dummy;
                                    Window wdummy;

                                    XGetGeometry(dpy, w, &wdummy, &dummy, &dummy,
                                            &cw, &dummy, &dummy, &dummy);
                                    XGetGeometry(dpy, parentID, &wdummy, &dummy, &dummy,
                                            &pw, &dummy, &dummy, &dummy);
                                    XReparentWindow(dpy, w, parentID, pw / 2 - cw / 2, 0);
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
        XCloseDisplay(dpy);
    }
#else
#error Implement reparent_pulldown
#endif
}
