/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* 
 *   Gnome Nibbles: Gnome Worm Game
 *   Written by Sean MacIsaac <sjm@acm.org>, Ian Peters <itp@gnu.org>
 * 
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
#include <config.h>

#include <stdlib.h>

#include <glib/gi18n.h>
#include <libgames-support/games-runtime.h>

#include <clutter-gtk/clutter-gtk.h>

#include "main.h"
#include "gnibbles.h"
#include "properties.h"

#include "board.h"

void gnibbles_board_draw (GnibblesBoard *board);
void throw_error (char *msg);

extern GnibblesProperties *properties;

GnibblesBoard *
gnibbles_board_new (gint t_w, gint t_h) 
{
  ClutterColor stage_color = {0x00,0x00,0x00,0xff};

  GnibblesBoard *board = g_new (GnibblesBoard, 1);
  board->width = t_w;
  board->height = t_h;

  board->level = clutter_group_new ();
  board->surface = clutter_group_new ();
  board->clutter_widget = gtk_clutter_embed_new ();

  ClutterActor *stage;
  
  stage = gtk_clutter_embed_get_stage (GTK_CLUTTER_EMBED (board->clutter_widget));
  clutter_stage_set_color (CLUTTER_STAGE(stage), &stage_color);
  clutter_stage_set_user_resizable (CLUTTER_STAGE(stage), FALSE); //False for now, will implement true later
  clutter_actor_set_size (CLUTTER_ACTOR (stage), 653, 520);
  clutter_actor_show (stage);

  gnibbles_board_draw (board);

  return board;
}

void 
gnibbles_board_draw (GnibblesBoard *board) 
{
  int i,j;
  gchar *filename;
  const char *dirname;

  dirname = games_runtime_get_directory (GAMES_RUNTIME_GAME_PIXMAP_DIRECTORY);
  filename = g_build_filename (dirname, wall_images[0], NULL);

  ClutterActor *tmp = clutter_texture_new_from_file (filename, NULL);
  clutter_actor_set_size (CLUTTER_ACTOR (tmp), 
                          properties->tilesize, 
                          properties->tilesize);
  ClutterActor *clone;

  gint x_pos;
  gint y_pos;

  for (i = 0; i < BOARDHEIGHT; i++) {
    y_pos = i * properties->tilesize;
    for (j = 0; j < BOARDWIDTH; j++) {
      /*One actor at each tile */
      x_pos = j * properties->tilesize;
      clone = clutter_clone_new (CLUTTER_ACTOR (tmp));
      clutter_actor_set_position (CLUTTER_ACTOR (clone), 
                                  x_pos,
                                  y_pos);
      clutter_actor_show (clone);
      clutter_container_add_actor (CLUTTER_CONTAINER (board->surface), CLUTTER_ACTOR(clone));
      //TODO unref clone ?
    }
  }

  ClutterActor *stage = gnibbles_board_get_stage (board);
  clutter_container_add_actor (CLUTTER_CONTAINER (stage), board->surface);

}

ClutterActor *
gnibbles_board_get_stage (GnibblesBoard *board) 
{
  return gtk_clutter_embed_get_stage (GTK_CLUTTER_EMBED (board->clutter_widget));
}


void 
gnibbles_board_load_level (GnibblesBoard *board, GnibblesLevel *level) 
{
  gint i,j;
  gint x_pos, y_pos;
  //TODO: unref/free  board->level
  ClutterActor *tmp;
  
  gchar *filename;
  const char *dirname;

  gboolean wall = TRUE;

  dirname = games_runtime_get_directory (GAMES_RUNTIME_GAME_PIXMAP_DIRECTORY);

  FILE *fil = fopen ("lvlll.txt", "w");
  
  /* Load walls onto the surface*/
  for (i = 0; i < BOARDHEIGHT; i++) {
    y_pos = i * properties->tilesize;
    for (j = 0; j < BOARDWIDTH; j++) {
      wall = TRUE;
      fputc ((int)level->walls[i][j],  fil);
      switch (level->walls[i][j]) {
        case 'a': // empty space
          wall = FALSE;
          break; // break right away
        case 'b': // straight up
          filename = g_build_filename (dirname, wall_images[1], NULL);
          if (!filename) 
            throw_error (wall_images[1]);
          break;
        case 'c': // straight side
          filename = g_build_filename (dirname, wall_images[2], NULL);
          if (!filename) 
            throw_error (wall_images[2]);
          break;
        case 'd': // corner bottom left
          filename = g_build_filename (dirname, wall_images[3], NULL);
          if (!filename) 
            throw_error (wall_images[3]);
          break;
        case 'e': // corner bottom right
          filename = g_build_filename (dirname, wall_images[4], NULL);
          if (!filename) 
            throw_error (wall_images[4]);
          break;
        case 'f': // corner up left
          filename = g_build_filename (dirname, wall_images[5], NULL);
          if (!filename) 
            throw_error (wall_images[5]);
          break;
        case 'g': // corner up right
          filename = g_build_filename (dirname, wall_images[6], NULL);
          if (!filename) 
            throw_error (wall_images[6]);
          break;
        case 'h': // tee up
          filename = g_build_filename (dirname, wall_images[7], NULL);
          if (!filename) 
            throw_error (wall_images[7]);
          break;
        case 'i': // tee right
          filename = g_build_filename (dirname, wall_images[8], NULL);
          if (!filename) 
            throw_error (wall_images[8]);
          break;
        case 'j': // tee left
          filename = g_build_filename (dirname, wall_images[9], NULL);
          if (!filename) 
            throw_error (wall_images[9]);
          break;
        case 'k': // tee down
          filename = g_build_filename (dirname, wall_images[10], NULL);
          if (!filename) 
            throw_error (wall_images[10]);
          break;
        case 'l': // cross
          filename = g_build_filename (dirname, wall_images[11], NULL);
          if (!filename) 
            throw_error (wall_images[11]);
          break;
        default:
          wall = FALSE;
          break;
      }

      if (wall == TRUE) {
        x_pos = j * properties->tilesize;

        tmp = clutter_texture_new_from_file(filename, NULL);
        clutter_actor_set_size (CLUTTER_ACTOR(tmp),
                                 properties->tilesize,
                                 properties->tilesize);

        clutter_actor_set_position (CLUTTER_ACTOR (tmp), x_pos, y_pos);
        clutter_actor_show (CLUTTER_ACTOR (tmp));
        clutter_container_add_actor (CLUTTER_CONTAINER (board->level), tmp);
      }
    }
    fputc ((int) '\n', fil);
  }
  fclose (fil);
  ClutterActor *stage = gnibbles_board_get_stage (board);
  clutter_container_add_actor (CLUTTER_CONTAINER (stage), board->level);
  //raise the level above the surface
  clutter_actor_raise (board->level, board->surface);
}

void throw_error (char *msg) {
  char *message = 
        g_strdup_printf (_("Nibbles couldn't find pixmap file:\n%s\n\n"
			              "Please check your Nibbles installation"), msg);

  //gnibbles_error (window, message);
  g_free (message);
}
