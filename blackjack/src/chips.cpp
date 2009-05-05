// -*- mode:C++; tab-width:8; c-basic-offset:8; indent-tabs-mode:nil -*-
/* 
 * Blackjack - chips.cpp
 *
 * Copyright (C) 2003-2004 William Jon McCann <mccann@jhu.edu>
 *
 * This game is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307
 * USA
 */

#include <config.h>

#include <stdlib.h>
#include <math.h>
#include <libgames-support/games-preimage.h>
#include <libgames-support/games-runtime.h>

#include "blackjack.h"
#include "chips.h"
#include "draw.h"

#define CHIP_FILENAME_100 "chip-100.svg"
#define CHIP_FILENAME_25  "chip-25.svg"
#define CHIP_FILENAME_5   "chip-5.svg"
#define CHIP_FILENAME_1   "chip-1.svg"

GamesPreimage *chip_preimage[4] = { NULL, NULL, NULL, NULL };
GdkPixbuf *chip_scaled_pixbuf[4] = { NULL, NULL, NULL, NULL };

GList *chip_stack_list = NULL;

gint
bj_chip_get_id (gfloat value)
{
        gint id;

        if (value == 100)
                id = CHIP_100;
        else if (value == 25)
                id = CHIP_25;
        else if (value == 5)
                id = CHIP_5;
        else if (value == 1)
                id = CHIP_1;
        else
                id = -1;

        return id;
}

void
bj_chip_set_size (gint width,
                  gint height)
{
        static const gchar *names[4] = { CHIP_FILENAME_100,
                                         CHIP_FILENAME_25,
                                         CHIP_FILENAME_5,
                                         CHIP_FILENAME_1 };
        const char *pixmapdir;

        pixmapdir = games_runtime_get_directory (GAMES_RUNTIME_GAME_PIXMAP_DIRECTORY);

        for (guint i = 0; i < G_N_ELEMENTS (names); i++) {
                gchar *fullname;

                fullname = g_build_filename (pixmapdir, names[i], NULL);

                if (!fullname)
                        continue;

		if (!chip_preimage[i])
			chip_preimage[i] = games_preimage_new_from_file (fullname,
									 NULL);

                if (chip_scaled_pixbuf[i])
                        g_object_unref (chip_scaled_pixbuf[i]);

                chip_scaled_pixbuf[i] = games_preimage_render (chip_preimage[i],
                                                               width,
                                                               height);
                g_free (fullname);
        }
}

GdkPixbuf *
bj_chip_get_scaled_pixbuf (gint chip)
{
        if (chip < 0 || chip > 3)
                return NULL;
        return chip_scaled_pixbuf[chip];
}

gdouble
bj_chip_get_value (gint chip)
{
        gdouble value;

        switch (chip) {
        case CHIP_100: value = 100.0; break;
        case CHIP_25: value = 25.0; break;
        case CHIP_5: value = 5.0; break;
        case CHIP_1: value = 1.0; break;
        default: value = 0;
        }
        return value;
}

void
bj_chip_stack_decompose_value (gfloat value, gint *blacks, 
                               gint *greens, gint *reds, gint *whites)
{
        gfloat remainder = value;
        *blacks = (gint) remainder / 100;
        remainder -= (*blacks * 100);
        *greens = (gint) remainder / 25;
        remainder -= (*greens * 25);
        *reds = (gint) remainder / 5;
        remainder -= (*reds * 5);
        *whites = (gint) remainder / 1;
}

gfloat
bj_chip_stack_get_chips_value (GList *chips)
{
        gfloat value = 0;
        for (GList *tmptr=chips; tmptr; tmptr=tmptr->next)
                value += ((hchip_type)tmptr->data)->value;

        return value;
}

void 
bj_chip_stack_add (gint id, double x, double y)
{
        hstack_type hstack = (hstack_type) g_malloc (sizeof (chip_stack_type));

        hstack->id = id;
        hstack->chips = NULL;
        hstack->x = x;
        hstack->y = y;
        hstack->pixelx = 0;
        hstack->pixely = 0;

        hstack->dx = 0;
        hstack->dy = 0;
        hstack->pixeldx = 0;
        hstack->pixeldy = 0;
        hstack->expansion_depth = 0;

        hstack->length = hstack->exposed = 0;
        bj_chip_stack_update_length (hstack);

        chip_stack_list = g_list_append (chip_stack_list, hstack);
}

hchip_type 
bj_chip_new (float value)
{
        hchip_type temp_chip = (hchip_type) g_malloc (sizeof (chip_type));

        temp_chip->value = value;

        return temp_chip;
}

void 
bj_chip_stack_pressed (gint x, gint y, hstack_type *stack, gint *chipid) 
{
        GList *tempptr;

        gint num_chips;
        gboolean got_stack = FALSE;

        *stack = NULL;
        *chipid = -1;

        gint chip_height = chip_width;
        for (tempptr = g_list_last (chip_stack_list); tempptr; tempptr = tempptr->prev) {
                
                hstack_type hstack = (hstack_type) tempptr->data;
                gint bottom_chip_edge = hstack->pixely + chip_height;
                gint top_chip_edge = bottom_chip_edge - hstack->height;
                /* if point is within our rectangle */
                if (hstack->pixelx < x && x < hstack->pixelx + hstack->width 
                    && bottom_chip_edge > y && y > top_chip_edge) {
                        num_chips = hstack->length;
        
                        if (got_stack == FALSE || num_chips > 0) {  
                                // loop through chips top down (cascade)
                                // check if click is within chip radius
                                gint x_center, y_center;
                                x_center = hstack->pixelx + chip_height / 2;
                                gfloat radius, x_diff, y_diff;
                                for (gint chip_i = num_chips - 1; chip_i >= 0; chip_i--) {
                                        y_center = hstack->pixely + chip_height / 2 - chip_i * hstack->pixeldy;
                                        y_diff = ABS ((y - y_center));
                                        x_diff = ABS ((x - x_center));
                                        radius = sqrt (x_diff * x_diff + y_diff * y_diff);
                                        if (radius < chip_height / 2) {
                                                *chipid = chip_i;
                                                break;
                                        }
                                }

                                *stack = hstack;
                                
                                /* this is the topmost stack with a chip */
                                /* take it and run*/
                                if (num_chips > 0)
                                        break;
                                
                                got_stack = TRUE;
                        }
                }
        }
}

void 
bj_chip_stack_update_length (hstack_type hstack)
{
        gint delta = g_list_length (hstack->chips) - hstack->length;

        hstack->length += delta;
        hstack->exposed += delta;

        if ((hstack->pixeldx == 0 && hstack->pixeldy == 0 && hstack->exposed > 1) 
            || (hstack->length > 0 && hstack->exposed < 1))
                hstack->exposed = 1;

        delta = hstack->exposed ? hstack->exposed - 1 : 0;
  
        hstack->width = chip_width + delta * hstack->pixeldx;
        hstack->height = chip_width + delta * hstack->pixeldy;
}

GList * 
bj_chip_stack_get_list ()
{
        return chip_stack_list;
}

void
bj_chip_stack_add_chips (GList* newchips, hstack_type hstack)
{
        hstack->chips = g_list_concat (hstack->chips, newchips);
        bj_chip_stack_update_length (hstack);
}

void
bj_chip_stack_delete (hstack_type hstack) 
{
        GList* temptr;
  
        for (temptr = hstack->chips; temptr; temptr = temptr->next)
                g_free (temptr->data);
        g_list_free (hstack->chips);
        free (hstack);
}

void
bj_chip_stack_delete_all (void)
{
        GList* temptr;
  
        for (temptr = chip_stack_list; temptr; temptr = temptr->next) {
                bj_chip_stack_delete ((hstack_type)temptr->data);
                temptr->data = NULL;
        }
        g_list_free (chip_stack_list);
        chip_stack_list = NULL;
}

void
bj_chip_stack_delete_all_wagers (void)
{
        bj_chip_stack_delete_all ();
        bj_chip_stack_create_sources ();
}

void
bj_chip_stack_create_sources (void)
{
        double x = CHIP_X_ORIGIN;
        double y = CHIP_Y_ORIGIN;
        double x_offset = 0.0;
        for (int i = 0; i < 4; i++) {
                bj_chip_stack_new_source_with_value (bj_chip_get_value (i),
                                                     x + x_offset, y);
                x_offset += CHIP_X_OFFSET;
        }
}

void
bj_chip_stack_new_with_id_value (gint id, gfloat value, double x, double y)
{
        bj_chip_stack_add (id, x, y);
        GList *chiplist = NULL;

        gint blacks, greens, reds, whites;
        bj_chip_stack_decompose_value (value, &blacks, 
                                       &greens, &reds, &whites);

        // Put higher value chips on bottom
        for (int i = 0; i < blacks; i++) {
                hchip_type newchip = bj_chip_new (100.0);
                chiplist = g_list_append (chiplist, newchip);
        }
        for (int i = 0; i < greens; i++) {
                hchip_type newchip = bj_chip_new (25.0);
                chiplist = g_list_append (chiplist, newchip);
        }
        for (int i = 0; i < reds; i++) {
                hchip_type newchip = bj_chip_new (5.0);
                chiplist = g_list_append (chiplist, newchip);
        }
        for (int i = 0; i < whites; i++) {
                hchip_type newchip = bj_chip_new (1.0);
                chiplist = g_list_append (chiplist, newchip);
        }
        bj_chip_stack_add_chips (chiplist, 
                                 (hstack_type)g_list_last (bj_chip_stack_get_list ())->data);
}

void
bj_chip_stack_new_with_value (gfloat value, double x, double y)
{
        // New wager chip stack
        bj_chip_stack_new_with_id_value (1, value, x, y);
}

void
bj_chip_stack_new_source_with_value (gfloat value, double x, double y)
{
        // New source chip stack
        bj_chip_stack_new_with_id_value (0, value, x, y);
}

gboolean
bj_chip_stack_is_source (hstack_type hstack)
{
        // return true if stack is a chip source stack
        if (! hstack)
                return FALSE;
        return (hstack->id == 0) ? TRUE : FALSE;
}
