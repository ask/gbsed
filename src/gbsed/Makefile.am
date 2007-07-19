## Process this file with automake to produce Makefile.in
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

AM_CFLAGS    = -Wall -pedantic -ansi 

bin_PROGRAMS = gbsed
INCLUDES     = -I$(top_srcdir)/lib -I../libgbsed/
LDADD        = ../libgbsed/libgbsed.a
DEFS         = @DEFS@ -DVERSION=\"$(VERSION)\"

$(bin_PROGRAMS): ../libgbsed/libgbsed.a


gbsed_SOURCES: gbsed.c
