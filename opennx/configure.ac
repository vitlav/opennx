#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT([opennx],[0.14],[felfert@users.sf.net])
AC_CONFIG_AUX_DIR([conf])
AC_CONFIG_SRCDIR([opennxApp.cpp])
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE([foreign no-installinfo no-installman])
AC_CONFIG_HEADERS([config.h])


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

ac_cv_enable_debug=NO
AC_ARG_ENABLE([debug],
	AS_HELP_STRING([--enable-debug],
		[Enable debug build (Default: NO)]),
	[ test "$enableval" = "yes" && ac_cv_enable_debug=YES ]
)
if test "$ac_cv_enable_debug" = "YES" ; then
	AC_MSG_NOTICE([will build debug version])
	WXPARAMS="--debug"
else
	AC_MSG_NOTICE([will build release version])
fi

ac_cv_enable_unicode=YES
AC_ARG_ENABLE([unicode],
	AS_HELP_STRING([--enable-unicode],
		[Enable unicode build (Default: YES)]),
	[ test "$enableval" = "no" && ac_cv_enable_unicode=NO ]
)
if test "$ac_cv_enable_unicode" = "YES" ; then
	AC_MSG_NOTICE([will enable unicode build])
	WXPARAMS="$WXPARAMS --unicode"
else
	AC_MSG_NOTICE([will disable unicode build])
fi

if test "$enable_static" = "yes" ; then
	AC_MSG_NOTICE([will link against static wxWidgets libs])
	WXPARAMS="$WXPARAMS --static"
else
	AC_MSG_NOTICE([will link against shared wxWidgets libs])
fi

AM_PATH_WXCONFIG(2.8.0,wxWidgets=1,wxWidgets=0,[xrc,xml,qa,html,adv,core,net], $WXPARAMS)

# Fix incorrect references to builtin support libs of WX_
if test "$enable_static" = "yes" ; then
	bdir=
	for part in $WX_LIBS ; do
		case $part in
			*/libwx_*)
			bdir=`dirname $part`
			;;
		esac
	done
	if test -z "$bdir" ; then
		AC_MSG_ERROR([Could not determine wx library directory])
	fi
	for lib in regex expat tiff jpeg png zlib ; do
		[WX_LIBS="`echo $WX_LIBS|sed -e \"s@-lwx\($lib[^ ]*\)@$bdir/libwx\1.a@g\"`"]
	done
fi


test ${wxWidgets} = 0 && AC_MSG_ERROR(Could not find wxWidgets library)

APP_LDADD=
WIN_RES=
WIN_RESHDR=
case $target_os in
	mingw*)
		APP_LDADD="-lstdc++ -lversion"
		WIN_RES='opennx.$(OBJEXT)'
		WIN_RESHDR="resource.h res/opennx.rc"
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
		;;
esac
AC_SUBST(APP_LDADD)
AC_SUBST(WIN_RES)
AC_SUBST(WIN_RESHDR)

AC_ARG_ENABLE([warn],
	AS_HELP_STRING([--enable-warn],
		[Enable some g++ warnings (Default: NO)]),
	[
		AC_MSG_NOTICE([will enable additional g++ warnings])
		CPPFLAGS="$CPPFLAGS -Wall -Wchar-subscripts -Wcomment -Wformat -Wimplicit -Wmissing-braces -Wparentheses -Wreturn-type -Wswitch -Wuninitialized"
		CXXFLAGS="$CXXFLAGS -Wctor-dtor-privacy -Wnon-virtual-dtor -Woverloaded-virtual"
	]
)

ac_cv_enable_smartcard=NO
AC_ARG_ENABLE([smartcard],
	AS_HELP_STRING([--enable-smartcard],
		[Enable SmartCard support (Default: NO)]),
	[ test "$enableval" = "yes" && ac_cv_enable_smartcard=YES ]
)
if test "$ac_cv_enable_smartcard" = "YES" ; then
	AC_MSG_NOTICE([SmartCard feature enabled])
    AC_DEFINE(ENABLE_SMARTCARD,1,[Define, if SmartCard support should be enabled])
else
	AC_MSG_NOTICE([SmartCard feature disabled])
fi

dnl AC_CHECK_LIB(cups,cupsServer)

CUPS_DEFAULT_PATH=
AC_PATH_PROG([cupsd],[cupsd],[],[$PATH:/usr/sbin:/usr/local/sbin])
test -n "$cupsd" && CUPS_DEFAULT_PATH=$cupsd
AC_DEFINE_UNQUOTED(CUPS_DEFAULT_PATH,"$CUPS_DEFAULT_PATH",
                   [Set to the path of your CUPS daemon])

ac_cv_nxproto="2.1.0"
AC_ARG_WITH([nxproto],
            AS_HELP_STRING([--with-nxproto],
                           [Specify NX protocol version (Default: 2.1.0)]),
            [ac_cv_nxproto=${withval}])
AC_DEFINE_UNQUOTED(NX_PROTOCOL_VERSION,"${ac_cv_nxproto}",
                   [Set to the NX protocol version])

AC_PATH_XTRA
test -n "$x_includes" && CFLAGS="$CFLAGS -I$x_includes"
test -n "$x_libraries" && LDFLAGS="$LDFLAGS -L$x_libraries"

ac_cv_samba=
AC_ARG_WITH([samba],
            AS_HELP_STRING([--with-samba=DIR],
                           [Specify path to samba source]),
	[ac_cv_samba=$withval])
test -n "$ac_cv_samba" && CPPFLAGS="$CPPFLAGS -I$ac_cv_samba/source/include"

VERSION_COMMASEPARATED=`echo $VERSION|sed -e 's/\./,/g'`
while test -z "`echo -n $VERSION_COMMASEPARATED|cut -d, -f4`" ; do
	VERSION_COMMASEPARATED="${VERSION_COMMASEPARATED},0"
done
AC_SUBST(VERSION_COMMASEPARATED)

AC_CONFIG_FILES([Makefile opennx.spec Info.plist res/opennx.rc po/Makefile.in])
AC_OUTPUT
