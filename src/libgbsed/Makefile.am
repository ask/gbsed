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

AM_CFLAGS            = -Wall -Werror -pedantic -ansi -fno-common  \
                       -no-cpp-precomp -fno-strict-aliasing -fPIC -O3   
#                       -arch ppc -arch i386 #-bundle -undefined dynamic_lookup

lib_LIBRARIES        = libgbsed.a

INCLUDES             = -I$(top_srcdir)/lib
DEFS                 = @DEFS@ -DVERSION=\"$(VERSION)\" -DLIBGBSED_PRIVATE=1



include_HEADERS      = libgbsed.h

BUILT_SOURCES        = libgbsed.c
libgbsed_a_SOURCES   = libgbsed.c 
MAINTAINERCLEANFILES = $(BUILD_SOURCES)
