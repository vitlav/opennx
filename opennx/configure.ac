#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT([opennx],[0.16],[felfert@users.sf.net])
AC_CONFIG_AUX_DIR([conf])
AC_CONFIG_SRCDIR([opennxApp.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE([foreign no-installinfo no-installman])
dnl Unfortunately, silent-rules don't work on MacOSX
dnl AM_INIT_AUTOMAKE([foreign silent-rules no-installinfo no-installman])
dnl AM_SILENT_RULES([yes])

case $build_os in
    mingw*)
        if test "$build_os" = "$target_os" ; then
            # On native MinGW, the default include/lib paths
            # are /mingw/include resp. /mingw/lib, however
            # several contributed stuff is under /usr/...
            CPPFLAGS="$CPPFLAGS -I/usr/include"
            LDFLAGS="$LDFLAGS -L/usr/lib"
            # On native MinGW, we also want a prefix of /,
            # because we want to install directly in our
            # setupdir's bin (see packaging/win32/Makefile.am)
            if test "x$prefix" = xNONE; then
                prefix=/
            fi
        fi
        ;;
esac
case $target_os in
    darwin8*)
        am_dependency_tracking=no
        enable_dependency_tracking=no
        if test "x$prefix" = xNONE; then
            prefix=/Library/OpenNX
        fi
        ;;
esac

# Checks for programs.
AC_DISABLE_SHARED
AC_PROG_LIBTOOL
AC_PROG_CXX
AC_PROG_CC
AC_PROG_LN_S
dnl AM_GNU_GETTEXT_VERSION(0.14.5)
AM_GNU_GETTEXT([external])
dnl We use objdump on the MinGW target for the
dnl findrequires.pl script
AC_CHECK_TARGET_TOOL([OBJDUMP],[objdump],[
    case $target_os in
        mingw*)
            AC_MSG_ERROR([Could not find required tool 'objdump'])
            ;;
    esac
])
AC_SUBST(OBJDUMP)
dnl For the same reason (and for building Xming), we require perl
AC_CHECK_TOOL([PERL],[perl],[
    case $target_os in
        mingw*)
            AC_MSG_ERROR([Could not find required tool 'perl'])
            ;;
    esac
])
AC_SUBST(PERL)

# Checks for libraries.
WS2LIB=
AC_CHECK_LIB(ws2_32,main,[WS2LIB=-lws2_32],[
    case $target_os in
        mingw*)
            AC_MSG_ERROR([Could not find required library ws2_32])
            ;;
    esac
])
AC_SUBST(WS2LIB)

RESLIB=
AC_CHECK_LIB(resolv,res_init,[RESLIB=-lresolv],[
    case $target_os in
        darwin8*)
            AC_MSG_ERROR([Could not find required library resolv])
            ;;
    esac
])
AC_SUBST(RESLIB)

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h limits.h netinet/in.h sys/socket.h syslog.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_TYPE_SIZE_T
AC_TYPE_OFF_T
AC_TYPE_PID_T
AC_CHECK_TYPES(socklen_t)

# Checks for library functions.
AC_CHECK_FUNCS([memchr memset putenv socket strchr strdup strrchr strstr])
AC_FUNC_LSTAT_FOLLOWS_SLASHED_SYMLINK

WXPARAMS=
AM_OPTIONS_WXCONFIG

ac_cv_enable_usbip=NO
AC_ARG_ENABLE([usbip],
	AS_HELP_STRING([--enable-usbip],
		[Enable usbip support (Default: NO, currently on Linux only)]),
	[ test "$enableval" = "yes" && ac_cv_enable_usbip=YES ]
)
if test "$ac_cv_enable_usbip" = "YES" ; then
    AC_DEFINE(SUPPORT_USBIP,1,[Set to 1 for enabling USBIP support])
fi

ac_cv_enable_debug=NO
AC_ARG_ENABLE([debug],
	AS_HELP_STRING([--enable-debug],
		[Enable debug build (Default: NO)]),
	[ test "$enableval" = "yes" && ac_cv_enable_debug=YES ]
)
if test "$ac_cv_enable_debug" = "YES" ; then
    AC_MSG_NOTICE([will build debug version])
    WXPARAMS="--debug=yes"
else
    AC_MSG_NOTICE([will build release version])
    WXPARAMS="--debug=no"
fi

ac_cv_enable_unicode=YES
AC_ARG_ENABLE([unicode],
	AS_HELP_STRING([--enable-unicode],
		[Enable unicode build (Default: YES)]),
	[ test "$enableval" = "no" && ac_cv_enable_unicode=NO ]
)
if test "$ac_cv_enable_unicode" = "YES" ; then
    AC_MSG_NOTICE([will enable unicode build])
    WXPARAMS="$WXPARAMS --unicode=yes"
else
    AC_MSG_NOTICE([will disable unicode build])
    WXPARAMS="$WXPARAMS --unicode=no"
fi

ac_cv_enable_staticwx=NO
AC_ARG_ENABLE([staticwx],
	AS_HELP_STRING([--enable-staticwx],
		[Link against static wxWidgets libs (Default: NO)]),
	[ test "$enableval" = "yes" && ac_cv_enable_staticwx=YES ]
)
if test "$ac_cv_enable_staticwx" = "YES" ; then
    AC_MSG_NOTICE([will link against static wxWidgets libs])
    WXPARAMS="$WXPARAMS --static=yes"
else
    AC_MSG_NOTICE([will link against shared wxWidgets libs])
    WXPARAMS="$WXPARAMS --static=no"
fi

case $target_os in
    mingw*)
        # On mingw, we don't use qa
        _wx_required=xrc,xml,richtext,html,adv,core,net
        ;;
    *)
        _wx_required=xrc,xml,richtext,qa,html,adv,core,net
        ;;
esac
AM_PATH_WXCONFIG(2.8.0,wxWidgets=1,wxWidgets=0,[$_wx_required], $WXPARAMS)
test ${wxWidgets} = 0 && AC_MSG_ERROR(Could not find wxWidgets library)

APP_LDADD=
OPENNX_RES=
TRACELOG=
TRACELOG_RES=
WATCHREADER=
WATCHREADER_RES=
WATCHUSBIP=
MACFIDREADER=
MACUNINSTALL=
BUNDLE=
OSRESDEPS=
UNIFLAGS=
case $target_os in
    mingw*)
        # On mingw, the default lib path is /mingw/lib
        # If wxWidgets was built with a different prefix, we therefore
        # must add that prefic to the linker flags.
        [_wx_libpath="`$WX_CONFIG_WITH_ARGS --prefix`/lib"]
        test "$_wx_libpath" != "/mingw/lib" && LDFLAGS="$LDFLAGS -L$_wx_libpath"
        APP_LDADD="-lole32 -luuid -lversion"
        OPENNX_RES='opennx.$(OBJEXT)'
        TRACELOG='tracelog$(EXEEXT)'
        TRACELOG_RES='tracelog.$(OBJEXT)'
        WATCHREADER='watchreader$(EXEEXT)'
        WATCHREADER_RES='watchreader.$(OBJEXT)'
        OSRESDEPS='$(top_srcdir)/extres/*.ico'
        tagnames="RC"
        _LT_AC_TAGCONFIG
        ;;
    darwin8*)
        MACUNINSTALL='macuninstall$(EXEEXT)'
        MACFINDREADER='macfindreader$(EXEEXT)'
        WATCHREADER='watchreader$(EXEEXT)'
        BUNDLE=bundle
        OSRESDEPS='$(top_srcdir)/extres/*.icns'
        APP_LDADD="-lX11 -lXmu"
        # Automatically build universal binary, if wxWidgets is a universal binary
        if test -n "`echo -- $WX_LIBS|grep -- '-arch ppc'`" ; then
            UNIFLAGS="-arch ppc"
            WX_CFLAGS="$WX_CFLAGS -arch ppc"
            WX_CXXFLAGS="$WX_CXXFLAGS -arch ppc"
        fi
        if test -n "`echo -- $WX_LIBS|grep -- '-arch i386'`" ; then
            UNIFLAGS="$UNIFLAGS -arch i386"
            WX_CFLAGS="$WX_CFLAGS -arch i386"
            WX_CXXFLAGS="$WX_CXXFLAGS -arch i386"
        fi
        ;;
    *)
        APP_LDADD="-lstdc++ -lX11 -lXmu"
        WATCHREADER='watchreader$(EXEEXT)'
        if test "$ac_cv_enable_usbip" = "YES" ; then
            WATCHUSBIP='watchusbip$(EXEEXT)'
        fi
        ;;
esac
AC_SUBST(APP_LDADD)
AC_SUBST(OPENNX_RES)
AC_SUBST(TRACELOG)
AC_SUBST(TRACELOG_RES)
AC_SUBST(WATCHREADER)
AC_SUBST(WATCHREADER_RES)
AC_SUBST(WATCHUSBIP)
AC_SUBST(BUNDLE)
AC_SUBST(OSRESDEPS)
AC_SUBST(MACFINDREADER)
AC_SUBST(MACUNINSTALL)
AC_SUBST(UNIFLAGS)

AC_ARG_ENABLE([warn],
	AS_HELP_STRING([--enable-warn],
		[Enable additional warnings (Default: NO)]),
	[
		AC_MSG_NOTICE([will enable additional warnings])
        fe_CHECK_FLAG([C++],[CPPFLAGS],[-Wall])
        fe_CHECK_FLAG([C++],[CPPFLAGS],[-Wextra])
        fe_CHECK_FLAG([C++],[CPPFLAGS],[-Wchar-subscripts])
        fe_CHECK_FLAG([C++],[CPPFLAGS],[-Wcomment])
        fe_CHECK_FLAG([C++],[CPPFLAGS],[-Wformat])
        fe_CHECK_FLAG([C++],[CPPFLAGS],[-Wformat-security])
        fe_CHECK_FLAG([C++],[CPPFLAGS],[-Wimplicit])
        fe_CHECK_FLAG([C++],[CPPFLAGS],[-Wmissing-braces])
        fe_CHECK_FLAG([C++],[CPPFLAGS],[-Wparentheses])
        fe_CHECK_FLAG([C++],[CPPFLAGS],[-Wreturn-type])
        fe_CHECK_FLAG([C++],[CPPFLAGS],[-Wswitch])
        fe_CHECK_FLAG([C++],[CPPFLAGS],[-Wuninitialized])
        CPPFLAGS="$CPPFLAGS $ac_checked_CPPFLAGS"
        fe_CHECK_FLAG([C++],[CXXFLAGS],[-Wctor-dtor-privacy])
        fe_CHECK_FLAG([C++],[CXXFLAGS],[-Wnon-virtual-dtor])
        fe_CHECK_FLAG([C++],[CXXFLAGS],[-Woverloaded-virtual])
        CXXFLAGS="$CXXFLAGS $ac_checked_CXXFLAGS"
	]
)

dnl AC_CHECK_LIB(cups,cupsServer)

CUPS_DEFAULT_PATH=
AC_PATH_PROG([cupsd],[cupsd],[],[$PATH:/usr/sbin:/usr/local/sbin])
test -n "$cupsd" && CUPS_DEFAULT_PATH=$cupsd
AC_DEFINE_UNQUOTED(CUPS_DEFAULT_PATH,"$CUPS_DEFAULT_PATH",
                   [Set to the path of your CUPS daemon])

ac_cv_nxproto="3.2.0"
AC_ARG_WITH([nxproto],
            AS_HELP_STRING([--with-nxproto],
                           [Specify NX protocol version (Default: 3.2.0)]),
            [ac_cv_nxproto=${withval}])
AC_DEFINE_UNQUOTED(NX_PROTOCOL_VERSION,"${ac_cv_nxproto}",
                   [Set to the NX protocol version])

dnl Don't check for X11 on mingw, otherwise we
dnl might get incorrect C/CPP/LD FLAGS in a cross-compile
dnl environment
case $target_os in
    mingw*)
        ;;
    *)
        AC_PATH_XTRA
        ;;
esac

ac_cv_samba=
AC_ARG_WITH([samba],
            AS_HELP_STRING([--with-samba=DIR],
                           [Specify path to samba source]),
	[ac_cv_samba=$withval])
if test -n "$ac_cv_samba" ; then
    CPPFLAGS="$CPPFLAGS -I$ac_cv_samba/source/include"
else
    case $target_os in
        darwin8*)
            CPPFLAGS="$CPPFLAGS -I$srcdir/packaging/MacOSX"
            ;;
    esac
fi

ac_cv_opensc=
AC_ARG_WITH([opensc],
            AS_HELP_STRING([--with-opensc=DIR],
                           [Specify path to opensc installation]),
	[ac_cv_opensc=$withval])
if test -n "$ac_cv_opensc" ; then
    CPPFLAGS="$CPPFLAGS -I$ac_cv_opensc/include"
    LDFLAGS="$LDFLAGS -L$ac_cv_opensc/lib"
else
    case $target_os in
        darwin8*)
            CPPFLAGS="$CPPFLAGS -I/Library/OpenSC/include"
            LDFLAGS="$LDFLAGS -L/Library/OpenSC/lib"
            ;;
    esac
fi

ac_cv_dllpath=
AC_ARG_WITH([dllpath],
            AS_HELP_STRING([--with-dllpath=PATH],
                           [Specify path for DLLs (mingw32 crossbuild only)]),
	[ac_cv_dllpath=$withval])

FE_SVNREV([SVNREV])
AC_DEFINE_UNQUOTED(SVNREV,"$SVNREV",[SVN revision])
FE_RCVERSION

AC_CHECK_HEADERS([cups/cups.h cups.h libsmbclient.h opensc/opensc.h usb.h])
case $target_os in
    mingw*)
        ;;
    *)
        if test "$ac_cv_header_libsmbclient_h" != "yes" ; then
            AC_MSG_ERROR([Need libsmbclient.h for compiling on Unix])
        fi
        if test "$ac_cv_header_cups_cups_h" != "yes" -a "$ac_cv_header_cups_h" != "yes" ; then
            AC_MSG_ERROR([Need cups.h for compiling on Unix])
        fi
        ;;
esac

if test "$ac_cv_header_opensc_opensc_h" != "yes"; then
    AC_MSG_ERROR([Need opensc/opensc.h])
fi
if test "$ac_cv_enable_usbip" = "YES" -a "$ac_cv_header_usb_h" != "yes"; then
    AC_MSG_ERROR([Need usb.h when compiling with usbip support])
fi

# ISCC env variable
AC_ARG_VAR([ISCC],[Full path of InnoSetup compiler iscc.exe.])dnl

SETUP=
DLLPATH=
PLATFORMDIST=unknown
PULSE_BTYPE=unknown
XMING_BTYPE=unknown
NX_BTYPE=unknown
XMING_PLATFORM_PATCHES=
case $target_os in
    linux*)
        AC_CHECK_TOOL([RPMBUILD], [rpmbuild], [no])
        if test "$RPMBUILD" != "no" ; then
            PLATFORMDIST=rpm
        else
            AC_CHECK_TOOL([DPKG_BUILDPACKAGE], [dpkg-buildpackage], [no])
            if test "$DPKG_BUILDPACKAGE" != "no" ; then
                PLATFORMDIST=deb
            fi
        fi
        ;;
    darwin8*)
        PLATFORMDIST=dmg
        ;;
    mingw*)
        PLATFORMDIST=setup
        AC_DEFINE_UNQUOTED(_WIN32_WINNT,0x0500,
                   [Minimum required windows version])
        if test "$build_os" = "$target_os" ; then
            DLLPATH="$PATH"
            PULSE_BTYPE=native
            XMING_BTYPE=native
            NX_BTYPE=native
            XMING_PLATFORM_PATCHES=xming-build-native.patch
            AC_ARG_WITH([iscc],
                        AS_HELP_STRING([--with-iscc=PATH],[Override ISCC path. (native mingw only)]),
                        [ISCC="$withval"])
            AC_PATH_PROG([ISCC],[iscc],[""],[$PATH])
            if test -z "$ISCC" ; then
                AC_MSG_WARN([InnoSetup compiler is missing, therefore no Setup will be built!])
                SETUP=
            else
                SETUP=nativesetup
            fi
        else
            if test -z "$ac_cv_dllpath" ; then
                AC_MSG_ERROR([Need --with-dllpath option on mingw32 crossbuild])
            fi
            DLLPATH="$ac_cv_dllpath"
            AC_ARG_WITH([wine-iscc],
                        AS_HELP_STRING([--with-wine-iscc=PATH],[Path of ISCC to be run under wine. (mingw32 crossbuild only)]),
                        [ISCC="$withval"])
            if test -z "$ISCC" ; then
                AC_MSG_WARN([InnoSetup compiler is missing, therefore no Setup will be built!])
                SETUP=
            else
                AC_PATH_PROG([WINE],[wine],[""],[$PATH])
                if test -z "$WINE" ; then
                    AC_MSG_WARN([wine is not available, therefore no Setup will be built!])
                    SETUP=
                else
                    SETUP=crosssetup
                fi
            fi
            PULSE_BTYPE=cross
            XMING_BTYPE=cross
            NX_BTYPE=cross
            XMING_PLATFORM_PATCHES=xming-build-cross.patch
        fi
        ;;
esac
AC_SUBST(SETUP)
AC_SUBST(DLLPATH)
AC_SUBST(PLATFORMDIST)
AC_SUBST(PULSE_BTYPE)
AC_SUBST(NX_BTYPE)
AC_SUBST(XMING_BTYPE)
AC_SUBST(XMING_PLATFORM_PATCHES)

AC_CONFIG_FILES([Makefile opennx.spec res/opennx.rc
    packaging/win32/Makefile packaging/win32/findrequires.pl
    packaging/win32/xming/Makefile packaging/win32/nx/Makefile
    packaging/win32/pulse/Makefile
    packaging/MacOSX/Makefile packaging/MacOSX/opennx.plist
    packaging/MacOSX/opennx-main.plist packaging/MacOSX/pkg.plist
    po/Makefile.in])
AC_OUTPUT
