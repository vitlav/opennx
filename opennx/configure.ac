#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT([opennx],[0.15],[felfert@users.sf.net])
AC_CONFIG_AUX_DIR([conf])
AC_CONFIG_SRCDIR([opennxApp.cpp])
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE([foreign no-installinfo no-installman])
AC_CONFIG_HEADERS([config.h])

case $build_os in
	mingw*)
        CPPFLAGS="$CPPFLAGS -I/usr/include"
        LDFLAGS="$LDFLAGS -L/usr/lib"
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

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_TYPE_SIZE_T

# Checks for library functions.
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

if test "$enable_static" = "yes" ; then
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
OPENNX_RESHDR=
TRACELOG=
WATCHREADER=
WATCHUSBIP=
case $target_os in
	mingw*)
        # On mingw, the default lib path is /mingw/lib
        # If wxWidgets was built with a different prefix, we therefore
        # must add that prefic to the linker flags.
        [_wx_libpath="`$WX_CONFIG_WITH_ARGS --prefix`/lib"]
        test "$_wx_libpath" != "/mingw/lib" && LDFLAGS="$LDFLAGS -L$_wx_libpath"
        APP_LDADD="-lstdc++ -lole32 -luuid -lversion"
		OPENNX_RES='opennx.$(OBJEXT)'
		OPENNX_RESHDR="res/opennx.rc"
        TRACELOG='tracelog$(EXEEXT)'
		TRACELOG_RES='tracelog.$(OBJEXT)'
		tagnames="RC"
		_LT_AC_TAGCONFIG
		;;
	darwin8*)
		APP_LDADD="-lX11 -lXmu"
		# Automatically build universal binary, if wxWidgets is a universal binary
		if test -n "`echo -- $WX_LIBS|grep -- '-arch ppc'`" ; then
			WX_CFLAGS="$WX_CFLAGS -arch ppc"
			WX_CXXFLAGS="$WX_CXXFLAGS -arch ppc"
		fi
		if test -n "`echo -- $WX_LIBS|grep -- '-arch i386'`" ; then
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
AC_SUBST(OPENNX_RESHDR)
AC_SUBST(TRACELOG)
AC_SUBST(TRACELOG_RES)
AC_SUBST(WATCHREADER)
AC_SUBST(WATCHUSBIP)

AC_ARG_ENABLE([warn],
	AS_HELP_STRING([--enable-warn],
		[Enable some g++ warnings (Default: NO)]),
	[
		AC_MSG_NOTICE([will enable additional g++ warnings])
		CPPFLAGS="$CPPFLAGS -Wall -Wchar-subscripts -Wcomment -Wformat -Wimplicit -Wmissing-braces -Wparentheses -Wreturn-type -Wswitch -Wuninitialized"
		CXXFLAGS="$CXXFLAGS -Wctor-dtor-privacy -Wnon-virtual-dtor -Woverloaded-virtual"
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
                           [Specify NX protocol version (Default: 2.1.0)]),
            [ac_cv_nxproto=${withval}])
AC_DEFINE_UNQUOTED(NX_PROTOCOL_VERSION,"${ac_cv_nxproto}",
                   [Set to the NX protocol version])

AC_PATH_XTRA

ac_cv_samba=
AC_ARG_WITH([samba],
            AS_HELP_STRING([--with-samba=DIR],
                           [Specify path to samba source]),
	[ac_cv_samba=$withval])
test -n "$ac_cv_samba" && CPPFLAGS="$CPPFLAGS -I$ac_cv_samba/source/include"

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

dnl AC_DEFINE(wxUSE_LOG_DEBUG,1,[Set to 1 for enabling trace logging])

AC_CONFIG_FILES([Makefile opennx.spec Info.plist res/opennx.rc po/Makefile.in])
AC_OUTPUT
