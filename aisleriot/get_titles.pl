#!/usr/bin/env perl

#  Aisleriot Game Name Internationalizator
#
#  Copyright (C) 2001 Free Software Foundation.
#
#  This script is free software; you can redistribute it and/or
#  modify it under the terms of the GNU General Public License as
#  published by the Free Software Foundation; either version 2 of the
#  License, or (at your option) any later version.
#
#  This script is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this library; if not, write to the Free Software
#  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
#  Authors:
#  2006 - Richard Hoelscher <rah@rahga.com>
#  2001 - Kenneth Christiansen <kenneth@gnu.org>

open OUTFILE, "| sort >translatable_game_names.h";

opendir(DIR, "rules/");

while ($_ = readdir(DIR)) {
  if (s/^(.)(.*).scm/\u$1$2/) { # Match scm filenames. Upcase first letter.
    s/_(.)/ \u$1/g;             # Replace underscores and following letter
				# with space and upcase letter.
    print OUTFILE "gchar *s = N_(\"$_\")\n";
  }
}
