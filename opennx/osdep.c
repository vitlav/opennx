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

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif
/*
 * Defines canonicalized platform names (e.g. __LINUX__)
 */
#include <wx/platform.h>

int inKdeSession = 0;

#ifdef __WXMSW__
/* dummies for now */
char *x11_socket_path = "";
char *x11_keyboard_type = "";

//#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tlhelp32.h>
#include <stdlib.h>

static char _spath[_MAX_PATH+1];
static char _kbd[_MAX_PATH+1];

long getppid()
{
    OSVERSIONINFO osver;
    HINSTANCE hKernel32;
    HANDLE hSnapShot;
    PROCESSENTRY32 procentry;
    long mypid;
    long ppid = 0;

	/* ToolHelp Function Pointers.*/
    HANDLE (WINAPI *lpfCreateToolhelp32Snapshot)(DWORD,DWORD);
    BOOL (WINAPI *lpfProcess32First)(HANDLE,LPPROCESSENTRY32);
    BOOL (WINAPI *lpfProcess32Next)(HANDLE,LPPROCESSENTRY32);
	
    osver.dwOSVersionInfoSize = sizeof(osver);
    if (!GetVersionEx(&osver))
        return 0;
	if (osver.dwPlatformId != VER_PLATFORM_WIN32_NT)
        return 0;
    if ((hKernel32 = LoadLibraryA("kernel32.dll")) == NULL)
        return 0;
	lpfCreateToolhelp32Snapshot= (HANDLE(WINAPI *)(DWORD,DWORD))
        GetProcAddress(hKernel32, "CreateToolhelp32Snapshot");
	lpfProcess32First= (BOOL(WINAPI *)(HANDLE,LPPROCESSENTRY32))
        GetProcAddress(hKernel32, "Process32First");
    lpfProcess32Next= (BOOL(WINAPI *)(HANDLE,LPPROCESSENTRY32))
        GetProcAddress(hKernel32, "Process32Next");
    if (lpfProcess32Next == NULL || lpfProcess32First == NULL || lpfCreateToolhelp32Snapshot == NULL) {
        FreeLibrary(hKernel32);
        return 0;
    }
    hSnapShot = lpfCreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapShot == INVALID_HANDLE_VALUE) {
        FreeLibrary(hKernel32);
        return 0;
    }
    memset((LPVOID)&procentry,0,sizeof(PROCESSENTRY32));
    procentry.dwSize = sizeof(PROCESSENTRY32);
    mypid = GetCurrentProcessId();
    if (lpfProcess32First(hSnapShot, &procentry)) {
        do {
            if (mypid == (long)procentry.th32ProcessID) {
                ppid =  procentry.th32ParentProcessID;
                break;
            }
            procentry.dwSize = sizeof(PROCESSENTRY32);
        } while (lpfProcess32Next(hSnapShot, &procentry));
    }	
	FreeLibrary(hKernel32);
    return ppid;
}

/**
 * Xming can handle multiple monitors only, if
 * *all* monitors have the same geometry and color depth.
 * This function checks for that constraint.
 *
 * Returns number of monitors or 0, if constraits not met.
 */
int checkMultiMonitors() {
    DWORD didx;
    int mnum = 0;
    DISPLAY_DEVICE dd;
    int w, h, bpp;

	while (EnumDisplayDevices(0, didx, &dd, 0)) {
        if (dd.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP) {
            if (!(dd.StateFlags & DISPLAY_DEVICE_MIRRORING_DRIVER)) {
                /* get information about the monitor attached to this display adapter.
                 * Dualhead cards and laptop video cards can have multiple monitors attached.
                 */
                DISPLAY_DEVICE ddMon;
                memset(&ddMon, 0, sizeof(ddMon));
                ddMon.cb = sizeof(ddMon);
                DWORD midx = 0;

                /* This enumeration may not return the correct monitor if multiple monitors are attached.
                 * This is because not all display drivers return the ACTIVE flag for the monitor
                 * that is actually active.
                 */
                while (EnumDisplayDevices(dd.DeviceName, midx, &ddMon, 0)) {
                    DEVMODE dm;
                    memset(&dm, 0, sizeof(dm));
                    dm.dmSize = sizeof(dm);
                    if (EnumDisplaySettingsEx(ddMon.DeviceName, ENUM_CURRENT_SETTINGS, &dm, 0)) {
                        if (0 == mnum) {
                            w = dm.dmPelsWidth;
                            h = dm.dmPelsHeight;
                            bpp = dm.dmBitsPerPel;
                        } else {
                            if (w != dm.dmPelsWidth)
                                return 0;
                            if (h != dm.dmPelsHeight)
                                return 0;
                            if (bpp != dm.dmBitsPerPel)
                                return 0;
                        }
                        mnum++;
                    }
                    midx++;
                }
            }
        }
        didx++;
    }
    return mnum;
}

#else /* ! __WXMSW__ */

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xmu/WinUtil.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <limits.h>
#include <string.h>

static char _spath[PATH_MAX+1];
static char _kbd[PATH_MAX+1];

char *x11_socket_path = _spath;
char *x11_keyboard_type = _kbd;

static void fatal(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
#ifdef __WXMAC__
    /* On MacOS, we use osascript (AppleScript) for displaying a simple
     * error message. (wxWidgets is not initialized yet).
     */
    FILE *p = popen("osascript", "w");
    fprintf(p, "display dialog \"");
    vfprintf(p, fmt, ap);
    fprintf(p, "\" buttons {\"OK\"} default button \"OK\" with icon stop with title \"OpenNX Error\"\n");
    pclose(p);
#endif
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    exit(1);
}

# ifdef __WXMAC__

/* get first free TCP port */
unsigned short
macFirstFreePort(unsigned short startPort) {
    int on = 1;
    unsigned short port = startPort;
    struct sockaddr_in sa;
    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    inet_aton("127.0.0.1", &sa.sin_addr);
    while (port < 65535) {
        int sock = socket(AF_INET, SOCK_STREAM, 6);
        if (sock >= 0) {
            if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == 0) {
                sa.sin_port = htons(port);
                if (bind(sock, (struct sockaddr *)&sa, sizeof(sa)) == 0) {
                    close(sock);
                    return port;
                }
            }
            close(sock);
        }
        port++;
    }
    return 0;
}

/* Startup XDarwin and try connecting to :0 */
static Display *launchX11() {
    Display *ret = NULL;
    time_t ts = time(NULL) + 15; /* Wait 15sec for X startup */
    system("/usr/X11R6/bin/X :0 -nolisten tcp &");
    while (!ret) {
        ret = XOpenDisplay(":0");
        if (!ret) {
            sleep(1);
            if (time(NULL) > ts)
                fatal("Timeout while waiting for X server.");
        }
    }
    putenv("DISPLAY=:0");
    /* If the following fails, there is usually a WM already running
     * so we don't care ... */
    if (0 == access("/usr/X11R6/bin/quartz-wm", X_OK)) {
        system("/usr/X11R6/bin/quartz-wm &");
        return ret;
    }
    if (0 == access("/usr/bin/quartz-wm", X_OK)) {
        system("/usr/bin/quartz-wm &");
        return ret;
    }
    system("quartz-wm &");
    return ret;
}
# endif /* __WXMAC__ */

static void __attribute__ ((constructor))
getx11socket()
{
    memset(&_spath, 0, sizeof(_spath));
    memset(&_kbd, 0, sizeof(_kbd));
    Display *dpy = XOpenDisplay(NULL);
#ifdef __WXMAC__
    /* Start X server, if necessary */
    if (!dpy)
        dpy = launchX11();
#endif
    if (dpy) {
        /* Find out X11 socket path */
        struct sockaddr_un una;
        socklen_t sl = sizeof(una);
        if (getpeername(ConnectionNumber(dpy), (struct sockaddr *)&una, &sl)) {
            perror("getpeername");
            exit(-1);
        } else {
            if (una.sun_family == AF_UNIX) {
                sl -= ((char *)&una.sun_path - (char *)&una);
                sl = (sl > sizeof(_spath)) ? sizeof(_spath) : sl;
                strncpy(_spath, una.sun_path, sl);
            }
        }

        Atom a = XInternAtom(dpy, "_KDE_RUNNING", True);
        /* Check, if we are running inside a KDE session */
        inKdeSession = (a != None);

        a = XInternAtom(dpy, "_XKB_RULES_NAMES", True);
        /* Get XKB rules (current keyboard layout language) */
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
#endif /* !__WXMSW__ */

/*
 * Close a foreign X11 window (just like a window-manager would do.
 */
void close_foreign(long parentID)
{
#if defined(__LINUX__) || defined(__OPENBSD__) || defined(__WXMAC__)
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
    (void)parentID;
#pragma message(__FILE__ " : warning: Implement close_foreign")
#endif
}

/*
 * Reparent our custom toolbar to the top center of a given foreign
 * X11 window. Since there is no way to retrieve our toolbar's X11-WindowID,
 * we use the _NET_WM_PID property (which automatically get's set by gdk), to find
 * our window. (At the time of the call it obviously should be the only one).
 * After that, proceed with a standard X11 reparenting ...
 */
void reparent_pulldown(long parentID)
{
#if defined(__LINUX__) || defined(__OPENBSD__) || defined(__WXMAC__)
    Display *dpy = XOpenDisplay(NULL);
    if (dpy) {
        Atom a = XInternAtom(dpy, "_NET_WM_PID", True);
        if (a != None) {
            int s;
            for (s = 0; s < ScreenCount(dpy); s++) {
                Window root = RootWindow(dpy, s);
                Window dummy;
                Window *childs = NULL;
                unsigned int nchilds = 0;
                if (XQueryTree(dpy, root, &dummy, &dummy, &childs, &nchilds)) {
                    unsigned int c;
                    for (c = 0; c < nchilds; c++) {
                        Window w = childs[c];
                        if (w != None) {
                            Atom type;
                            int fmt;
                            unsigned long n;
                            unsigned long ba;
                            unsigned long *prop;
                            int res = XGetWindowProperty(dpy, w, a, 0, 32, False,
                                    XA_CARDINAL, &type, &fmt, &n, &ba, (unsigned char **)&prop);
                            if ((res == Success) && (fmt = 32) && (n == 1) && prop) {
                                if (*prop == (unsigned long)getpid()) {
                                    int dummy;
                                    unsigned int pw, cw, udummy;
                                    Window wdummy;

                                    XGetGeometry(dpy, w, &wdummy, &dummy, &dummy,
                                            &cw, &udummy, &udummy, &udummy);
                                    XGetGeometry(dpy, parentID, &wdummy, &dummy, &dummy,
                                            &pw, &udummy, &udummy, &udummy);
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
    (void)parentID;
#pragma message(__FILE__ " : warning: Implement reparent_pulldown")
#endif
}

void close_sid(const char *sid)
{
#if defined(__LINUX__) || defined(__OPENBSD__) || defined(__WXMAC__)
    Display *dpy = XOpenDisplay(NULL);
    int closed = 0;
    if (dpy) {
        int s;
        for (s = 0; s < ScreenCount(dpy); s++) {
            Window root = RootWindow(dpy, s);
            Window dummy;
            Window *childs = NULL;
            unsigned int nchilds = 0;
            if (XQueryTree(dpy, root, &dummy, &dummy, &childs, &nchilds)) {
                unsigned int c;
                for (c = 0; c < nchilds; c++) {
                    Window w = XmuClientWindow(dpy, childs[c]);
                    if (w != None) {
                        char **cargv = NULL;
                        int cargc = 0;
                        if (XGetCommand(dpy, w, &cargv, &cargc)) { 
                            if ((cargc > 3) &&
                                    (strcmp(cargv[0], "/usr/NX/bin/nxagent") == 0) &&
                                    (strcmp(cargv[1], "-D") == 0) &&
                                    (strcmp(cargv[2], "-options") == 0) &&
                                    (strstr(cargv[3], sid) != NULL)) {
                                close_foreign(w);
                                closed = 1;
                                XFreeStringList(cargv);
                                break;
                            }
                        }
                        XFreeStringList(cargv);
                    }
                }
            }
            if (closed)
                break;
        }
        XCloseDisplay(dpy);
    }
#else
    (void)sid;
#pragma message(__FILE__ " : warning: Implement reparent_pulldown")
#endif
}

