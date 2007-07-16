#!/bin/sh
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

echo "Making man page for libgbsed.c    -> man/libgbsed.3"
pod2man --section=3 ./src/libgbsed/libgbsed.c   > man/libgbsed.3
echo "Making man page for gbsed.c       -> man/gbsed.1"
pod2man --section=1 ./src/gbsed/gbsed.c         > man/gbsed.1

echo "Making pod for libgbsed.c         -> doc/libgbsed.pod"
perldoc -u ./src/libgbsed/libgbsed.c            > doc/libgbsed.doc
echo "Making pod for gbsed.c            -> doc/gbsed.pod"
perldoc -u ./src/gbsed/gbsed.c                  > doc/gbsed.doc

