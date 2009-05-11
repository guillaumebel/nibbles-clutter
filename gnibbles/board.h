/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* 
 *   Gnome Nibbles: Gnome Worm Game
 *   Written by Sean MacIsaac <sjm@acm.org>, Ian Peters <itp@gnu.org>
 *                 Guillaume Beland <guillaume.beland@gmail.com>
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

#ifndef BOARD_H_
#define BOARD_H_

#include <gtk/gtk.h>
#include <clutter/clutter.h>

#include "level.h"

typedef struct {
        gint width;
        gint height;
        GtkWidget *clutter_widget;
        ClutterActor *surface;
        ClutterActor *level;
} GnibblesBoard;

static gchar *wall_images[] = {
    "wall.png",
    "wall-straight-up.svg",
    "wall-straight-side.svg",
    "wall-corner-bottom-left.svg",
    "wall-corner-bottom-right.svg",
    "wall-corner-top-left.svg",
    "wall-corner-top-right.svg",
    "wall-tee-up.svg",
    "wall-tee-right.svg",
    "wall-tee-left.svg",
    "wall-tee-down.svg",
    "wall-cross.svg",
};

GnibblesBoard *gnibbles_board_new (gint t_w, gint t_h);

ClutterActor *gnibbles_board_get_stage (GnibblesBoard *board);
gpointer gnibbles_board_draw (void *arg);
void gnibbles_board_load_level (GnibblesBoard *board, GnibblesLevel *level);

#endif
