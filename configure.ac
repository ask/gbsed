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

AC_INIT([gbsed], [1.0], [Ask Solem ask@0x61736b.net], [gbsed])
AC_CONFIG_AUX_DIR(config)
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE([gbsed],[1.0])
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

# check for basename
AC_MSG_CHECKING([for libgen.h])
AC_CACHE_VAL(system_has_libgen_h,
[AC_TRY_COMPILE([
#include <libgen.h>],
[const char *mydir;
       char *bname;
 mydir = "hello/dir";
 bname = basename(bname);
],
system_has_libgen_h=yes,
system_has_libgen_h=no,
)])
if test $system_has_libgen_h = yes; then
    AC_DEFINE(HAVE_LIBGEN_H, 1, [
        Define if the system has the xopen libgen.h header.
    ])
fi
AC_MSG_RESULT($system_has_libgen_h)
AC_CACHE_SAVE

AC_SUBST(BUILD_TARGET)
AC_SUBST(CFLAGS)
AC_SUBST(CPPFLAGS)
AC_SUBST(LDFLAGS)
AC_SUBST(LIBTOOL)

AC_CACHE_SAVE
AC_OUTPUT([Makefile m4/Makefile src/Makefile src/libgbsed/Makefile
src/gbsed/Makefile man/Makefile])
    

