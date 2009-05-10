/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* 
 *   Gnome Nibbles: Gnome Worm Game
 *   Written by Sean MacIsaac <sjm@acm.org>, Ian Peters <itp@gnu.org>
                Guillaume Beland <guillaume.beland@gmail.com>
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <stdlib.h>
#include <glib/gi18n.h>
#include "level.h"


GnibblesLevel *
gnibbles_level_new (gint level)
{
  GnibblesLevel *lvl = g_new (GnibblesLevel, 1);
  lvl->level = level;
  gchar *tmp = NULL;
  const char *dirname;
  gchar *filename;
  FILE *in;
  gchar tmpboard [BOARDWIDTH +2];
  gint i,j;
  
  tmp = g_strdup_printf("level%03d.gnl", level);
  
  dirname = games_runtime_get_directory (GAMES_RUNTIME_GAME_GAMES_DIRECTORY);
  filename = g_build_filename (dirname, tmp, NULL);

  g_free (tmp);

  if ((in = fopen (filename, "r")) == NULL) {
    char *message =
      g_strdup_printf (_
                       ("Nibbles couldn't load level file:\n%s\n\n"
                        "Please check your Nibbles installation"), filename);
    //gnibbles_error (window, message);
    g_free (message);
  }

  g_free (filename);

  for (i = 0; i < BOARDHEIGHT; i++) {
    if (!fgets (tmpboard, sizeof (tmpboard), in)) {
      char *message =
        g_strdup_printf (_
                         ("Level file appears to be damaged:\n%s\n\n"
                         "Please check your Nibbles installation"), filename);
      //gnibbles_error (window, message);
      g_free (message);
      break;
    }
    for (j = 0; j < BOARDWIDTH; j++) {
      lvl->walls[i][j] = tmpboard[j];
      //TODO: set worm start point, boni, etc
    }
  }

  fclose (in);

  return lvl;
}

