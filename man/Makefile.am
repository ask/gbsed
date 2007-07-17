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

MAN_SECTION_1 = gbsed.1
MAN_SECTION_3 = libgbsed.3
man_MANS = $(MAN_SECTION_3) $(MAN_SECTION_1)
man_INSTALL = $(man_MANS)
man_aux  = $(MAN_SECTION_1:.1=.x) $(MAN_SECTION_3:.3=.x)
EXTRA_DIST = $(man_MANS)
MAINTAINERCLEANFILES = $(man_MANS)



