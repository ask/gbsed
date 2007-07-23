## Process this file with autoconf to produce configure.in
#
# Copyright (C) 2007 Ask Solem <ask@0x61736b.net>
#
# This file is part of gbsed
#
#    gbsed is free software; you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation; either version 3 of the License, or
#    (at your option) any later version.
#
#    gbsed is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.

AC_INIT([gbsed], [0.66], [Ask Solem ask@0x61736b.net], [gbsed])
AC_REVISION($Revision$)
AC_CONFIG_AUX_DIR(config)
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE([gbsed],[0.66])
AC_CONFIG_SRCDIR(src/libgbsed/libgbsed.c)
AC_CONFIG_MACRO_DIR(m4)
AC_CANONICAL_HOST
AC_CACHE_LOAD

changequote(,)dnl
case $VERSION in
 [0-9]*.[0-9]*[a-z]) DIST_ALPHA="README-alpha";;
 [0-9]*.[0-9]*.[0-9]*) DIST_ALPHA="README-alpha";;
 *) DIST_ALPHA=;;
esac
changequote([, ])dnl
AC_SUBST(DIST_ALPHA)

AC_PROG_CC
AC_PROG_GCC_TRADITIONAL
AC_PROG_RANLIB
AC_PROG_INSTALL
AC_CACHE_SAVE

AC_HEADER_STDC
AC_CHECK_HEADERS(libgen.h stdlib.h stdbool.h unistd.h sys/stat.h sys/types.h)

# OS Specific

# # ## MAC OS X
can_it_be_osx='test -x /usr/bin/sw_vers'
get_osx_revision='sw_vers -productVersion
                | awk "{split(\$1,v,\".\");print v[2]}"
';
AC_MSG_CHECKING([for Mac OS X])
case "$host" in
    i*-apple-darwin*)
        ret=`$can_it_be_osx`
        if test $? -eq 0; then
            this_is_mac_os_x=yes
            can_universal_binaries=yes
            osx_revision=`$get_osx_revision`;
         fi
    ;;
    powerpc-apple-darwin*)
        ret=`$can_it_be_osx`
        if test $? -eq 0; then
            this_is_mac_os_x=yes
            osx_revision=`$get_osx_revision`;
            if test $osx_revision -ge 4; then
                can_universal_binaries=yes
            fi
        fi
    ;;
    *)
        this_is_mac_os_x=no
    ;;
esac
AC_MSG_RESULT($this_is_mac_os_x)

AC_MSG_CHECKING(for Mac OS X version)
if test "$this_is_mac_os_x" == "yes"; then
    osx_version=`sw_vers -productVersion`
    AC_MSG_RESULT($osx_version)
else
    AC_MSG_RESULT("no")
fi

AC_MSG_CHECKING([for Mac OS X universal binaries support])
AC_MSG_RESULT($can_universal_binaries)

AC_MSG_CHECKING([for Mac OS X deployment target])
osx_deployment_target=no
if test "$this_is_mac_os_x" == "yes"; then

    osxdt=$MACOSX_DEPLOYMENT_TARGET

    if test ! -z "$osxdt"; then
        osx_deployment_target=$osxdt
        osx_dt_version=`echo "$osxdt" | cut -d. -f2`;

        # universal binaries was first supported in 10.4
        if test $osx_dt_version -lt 4; then
            osx_deployment_target="$osxdt (will not build universal binaries)."
            can_universal_binaries=no
        fi
    fi
fi
AC_MSG_RESULT($osx_deployment_target)

if test "$this_is_mac_os_x" == "yes"; then
    if test "$can_universal_binaries" == "yes"; then
        CFLAGS="$CFLAGS -arch ppc -arch i386"
    fi
        dnl LDEXT="dylib"
        dnl LDFLAGS="$LDFLAGS -dynamiclib"
        dnl AM_CONDITIONAL(LDYLIB, true)
fi

AC_MSG_CHECKING(for hdiutil)
have_hdiutil=no
if test "$this_is_mac_os_x" == "yes"; then
    have_hdiutil=`which hdiutil`
fi
AC_MSG_RESULT($have_hdiutil)

AC_MSG_CHECKING(for packagemaker)
have_packagemaker=no
if test "$this_is_mac_os_x" == "yes"; then
   have_packagemaker=`which packagemaker`
fi
AC_MSG_RESULT($have_packagemaker) 

# #### /* MAC OS X */

AC_ARG_WITH(universal-binary,
[  --with-universal-binary Build (MacOSX|Darwin) universal binaries],,)

AC_ARG_WITH(dyld,
[  --with-dyld             Use (MacOSX|Darwin|OpenStep|Rhapsody) dynamic linker. ],,)

AH_TEMPLATE([PERL_MALLOC],
    [Define if you are using with perl/XS and want to use the perl memory
     allocation functions. ])

dnl AC_SUBST(BUILD_TARGET)
dnl AC_SUBST(CFLAGS)
dnl AC_SUBST(CPPFLAGS)
dnl AC_SUBST(LDFLAGS)
dnl AC_SUBST(LIBTOOL)
AC_SUBST(LIBRARY)
AC_MSG_CHECKING(LIBRARY)
if test -z "$LIBRARY"
then
    LIBRARY='libbsed$(VERSION).a'
fi
AC_MSG_RESULT($LIBRARY)

dnl Check some types.
AC_TYPE_MODE_T
AC_TYPE_SIZE_T

AC_MSG_CHECKING(for LIBGBSED_AUTHOR)
has_this=no
if test "$LIBGBSED_AUTHOR" = 1; then
    CFLAGS="$CFLAGS -Werror"
    has_this=yes
fi
AC_MSG_RESULT($has_this)

dnl Option --with-dyld
AC_MSG_CHECKING(for --with-dyld)
if test "$with_dyld"; then
    AC_MSG_RESULT(yes)
    dnl AC_DEFINE(WITH_DYLD)
    ns_dyld='set'
else
    AC_MSG_RESULT(no)
fi

dnl Option --with-universal-binary
AC_MSG_CHECKING(for --with-universal-binary)
if test "$with_universal_binary"; then
    AC_MSG_RESULT(yes)
    dnl AC_DEFINE(WITH_UB)
    ns_ub='set'
else
    AC_MSG_RESULT(no)
fi
 





AC_CACHE_SAVE
AC_OUTPUT([Makefile m4/Makefile src/Makefile src/libgbsed/Makefile
src/gbsed/Makefile man/Makefile])
