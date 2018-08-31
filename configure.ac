# Release version
m4_define([package_version_major],[1])
m4_define([package_version_minor],[1])
m4_define([package_version_micro],[0])
 m4_define([package_repository],[https://github.com/astronaos/BWM801])
m4_define([package_auth_email],[bwmulligan@astronaos.com])

# API version
m4_define([api_version],package_version_major)

AC_INIT(BWM801,package_version_major.package_version_minor.package_version_micro,package_auth_email,bwm801,package_repository)
AC_CONFIG_MACRO_DIR([m4])

LT_INIT

PKG_PROG_PKG_CONFIG
PKG_INSTALLDIR

MOD_PROG_ENVMOD
MOD_INSTALLDIR

AM_INIT_AUTOMAKE

#AC_PROG_RANLIB

AC_LANG(C++)

AC_PROG_CXX

AX_CXX_COMPILE_STDCXX(11,noext,mandatory) #use c++11 standard, required in a variety of places

AX_PTHREAD #needed for anything using EngineMk1

#m4_ifdef([AM_PROG_AR], [AM_PROG_AR]) #not sure why the ifdef is required; this is supposed to be a standard command in automake, but my implementation (v1.15) seems to not find it.

AC_SUBST([PACKAGE_VERSION_MAJOR],package_version_major)
AC_SUBST([PACKAGE_VERSION_MINOR],package_version_minor)
AC_SUBST([PACKAGE_VERSION_MICRO],package_version_micro)
AC_SUBST([PACKAGE_REPOSITORY],package_repository)
AC_SUBST([PACKAGE_AUTH_EMAIL],package_auth_email)
AC_SUBST([API_VERSION],api_version)

AC_SUBST([LIB_NAME],[bwm801])
AC_SUBST([LIB_DESCRIPTION],["BWM801 cross-OS multithreaded game engine"])
AC_SUBST([LIB_REQUIRES],["x11 freetype2 ftgl gl glu libpng"])


#AC_CHECK_HEADERS([cmath],[],[AC_MSG_ERROR[" Could not find cmath"]])
#AC_CHECK_HEADERS([vector],[],[AC_MSG_ERROR[" Could not find vector"]])
#AC_CHECK_HEADERS([deque],[],[AC_MSG_ERROR[" Could not find deque"]])
#AC_CHECK_HEADERS([map],[],[AC_MSG_ERROR[" Could not find map"]])
#AC_CHECK_HEADERS([iostream],[],[AC_MSG_ERROR[" Could not find iostream"]])
#AC_CHECK_HEADERS([cstdio],[],[AC_MSG_ERROR[" Could not find cstdio"]])
#AC_CHECK_HEADERS([cstdlib],[],[AC_MSG_ERROR[" Could not find cstdlib"]])
#AC_CHECK_HEADERS([cstring],[],[AC_MSG_ERROR[" Could not find cstring"]])
#AC_CHECK_HEADERS([sstream],[],[AC_MSG_ERROR[" Could not find sstream"]])
#AC_CHECK_HEADERS([ctime],[],[AC_MSG_ERROR[" Could not find ctime"]])
#AC_CHECK_HEADERS([string],[],[AC_MSG_ERROR[" Could not find string"]])

AC_CHECK_HEADERS([errno.h],[],[AC_MSG_ERROR[" Could not find errno.h"]])
AC_CHECK_HEADERS([assert.h],[],[AC_MSG_ERROR[" Could not find assert.h"]])

AC_CHECK_HEADERS([thread],[],[AC_MSG_ERROR[" Could not find thread"]])

#AC_CHECK_HEADERS([dirent.h],[],[AC_MSG_ERROR[" Could not find dirent.h"]])
AC_HEADER_DIRENT
#AC_CHECK_HEADERS([X11/Xlib.h],[],[AC_MSG_ERROR[" Could not find X11/Xlib.h.  libX11 (development version) is required for linux variant of BWM801"]])
AC_CHECK_HEADERS([unistd.h],[],[AC_MSG_ERROR[" Could not find unistd.h"]])

AC_CHECK_HEADERS([netdb.h],[],[AC_MSG_ERROR[" Could not find netdb.h"]])
AC_CHECK_HEADERS([sys/types.h],[],[AC_MSG_ERROR[" Could not find sys/types.h"]]) #TODO update this and engine to use one of (stdint.h inttypes.h sys/types.h) when available
AC_CHECK_HEADERS([sys/socket.h],[],[AC_MSG_ERROR[" Could not find sys/socket.h"]]) #TODO some systems may require stdlib.h and stdio.h for this to work 

PKG_CHECK_MODULES([x11],[x11],[],[AC_MSG_ERROR[" Could not find X11 development package"]])
PKG_CHECK_MODULES([gl],[gl],[],[AC_MSG_ERROR[" Could not find openGL development package"]])
PKG_CHECK_MODULES([glu],[glu],[],[AC_MSG_ERROR[" Could not find openGL GLU development package"]])
PKG_CHECK_MODULES([freetype2],[freetype2],[],[AC_MSG_ERROR[" Could not find freetype2. The freetype2 development library must be installed"]])
PKG_CHECK_MODULES([ftgl],[ftgl],[],[AC_MSG_ERROR[" Could not find ftgl."]])
PKG_CHECK_MODULES([libpng],[libpng],[],[AC_MSG_ERROR[" Could not find png."]])

AC_CONFIG_FILES([Makefile $PACKAGE_NAME-$API_VERSION.pc:lib.pc.in],[],[PACKAGE_NAME='$PACKAGE_NAME' API_VERSION='$API_VERSION'])

AS_IF( [test ${MODTYPE} = "LMOD"],
	[AC_CONFIG_FILES([$PACKAGE_VERSION.lua:modfile.lua.in])],
	[AS_IF( [test ${MODTYPE} = "ENVMOD"], [AC_CONFIG_FILES([$PACKAGE_VERSION:modfile.in])], [])])


AC_OUTPUT


