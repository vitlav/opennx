#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(mxclient, 0.1, fritz.elfert@millenux.com)
AC_CONFIG_AUX_DIR([conf])
AC_CONFIG_SRCDIR([mxclientApp.cpp])
AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
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

ac_cv_enable_static=NO
AC_ARG_ENABLE([static],
	AS_HELP_STRING([--enable-static],
		[link against static wxWidgets libs (Default: NO)]),
	[ test "$enableval" = "yes" && ac_cv_enable_static=YES ]
)
if test "$ac_cv_enable_static" = "YES" ; then
	AC_MSG_NOTICE([will link against static wxWidgets libs])
	WXPARAMS="$WXPARAMS --static"
else
	AC_MSG_NOTICE([will link against shared wxWidgets libs])
fi

AM_PATH_WXCONFIG(2.5.3,wxWidgets=1,wxWidgets=0,[xrc,qa,html,adv,core,xml,net], $WXPARAMS)

test ${wxWidgets} = 0 && AC_MSG_ERROR(Could not find wxWidgets library)

LIBS="$LIBS $WX_LIBS"
CPPFLAGS="$CPPFLAGS $WX_CPPFLAGS"
#CXXFLAGS="$CXXFLAGS $WX_CXXFLAGS -fuse-cxa-atexit"
CXXFLAGS="$CXXFLAGS $WX_CXXFLAGS"
LDFLAGS="$LDFLAGS $WX_LDFLAGS"

AC_ARG_ENABLE([warn],
	AS_HELP_STRING([--enable-warn],
		[Enable some g++ warnings (Default: NO)]),
	[
		AC_MSG_NOTICE([will enable additional g++ warnings])
		CPPFLAGS="$CPPFLAGS -Wctor-dtor-privacy -Wnon-virtual-dtor -Weffc++ -Wold-style-cast -Woverloaded-virtual -Wchar-subscripts -Wcomment -Wformat -Wimplicit -Wmissing-braces -Wparentheses -Wreturn-type -Wswitch -Wuninitialized"
	]
)

AC_CONFIG_FILES([Makefile po/Makefile.in m4/Makefile])
AC_OUTPUT
