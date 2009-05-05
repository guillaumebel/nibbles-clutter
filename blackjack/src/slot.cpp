// -*- mode:C++; tab-width:8; c-basic-offset:8; indent-tabs-mode:nil -*-
/*
 * Blackjack - slot.cpp
 *
 * Copyright (C) 2003-2004 William Jon McCann <mccann@jhu.edu>
 * Copyright (C) 1998 Jonathan Blandford <jrb@mit.edu>
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
#include <libgames-support/games-preimage.h>
#include <libgames-support/games-runtime.h>

#include "blackjack.h"
#include "slot.h"
#include "card.h"
#include "chips.h"
#include "draw.h"

#define SLOT_FILENAME "white-outline.svg"

GList *slot_list = NULL;

GamesPreimage *slot_preimage = NULL;
GdkPixbuf *slot_scaled_pixbuf = NULL;
GdkPixmap *default_background_pixmap = NULL;

GdkPixmap *
get_background_pixmap () {
  
        return default_background_pixmap;
}

void
bj_slot_set_size (gint width,
                  gint height)
{
        const char *pixmapdir;
        gchar *name;

        pixmapdir = games_runtime_get_directory (GAMES_RUNTIME_GAME_PIXMAP_DIRECTORY);
        name = g_build_filename (pixmapdir, SLOT_FILENAME, NULL);

        if (!name)
                return;

        if (!slot_preimage)
                slot_preimage = games_preimage_new_from_file (name, NULL);

        if (slot_scaled_pixbuf)
                g_object_unref (slot_scaled_pixbuf);

        slot_scaled_pixbuf = games_preimage_render (slot_preimage,
                                                    width,
                                                    height);
        g_free (name);
}

GdkPixbuf *
bj_slot_get_scaled_pixbuf ()
{
        return slot_scaled_pixbuf;
}

void
bj_slot_load_pixmaps (void)
{
        const char *pixmapdir;
        gchar *buffer;

        pixmapdir = games_runtime_get_directory (GAMES_RUNTIME_GAME_PIXMAP_DIRECTORY);
        buffer = g_build_filename (pixmapdir, "baize.png", NULL);
        default_background_pixmap = get_pixmap (buffer);
        g_free (buffer);
}

void
bj_slot_free_pixmaps (void)
{
        if (slot_scaled_pixbuf)
                g_object_unref (slot_scaled_pixbuf);

        if (default_background_pixmap)
                g_object_unref (default_background_pixmap);
}

void 
bj_slot_add (gint id)
{
        hslot_type hslot = (hslot_type) g_malloc (sizeof (slot_type));

        hslot->id = id;
        hslot->cards = NULL;

        if (id == 0) {
                hslot->x = DEALER_SLOT_ORIGIN_X;
                hslot->y = DEALER_SLOT_ORIGIN_Y;
        } else {
                hslot->x = PLAYER_SLOT_ORIGIN_X;
                hslot->y = PLAYER_SLOT_ORIGIN_Y;
        }
        
        hslot->dx = hslot->dy = 0;
        hslot->pixelx = hslot->pixely = 0;
        hslot->pixeldx = hslot->pixeldy = 0;
        hslot->expansion_depth = 0;

        hslot->dx = x_expanded_offset;
    
        hslot->length = hslot->exposed = 0;
        bj_slot_update_length (hslot);

        if (slot_list)
                slot_list = g_list_append (slot_list, hslot);
        else {
                slot_list = g_list_alloc ();
                slot_list->data = hslot;
        }
}

hslot_type
bj_slot_add_before_slot (hslot_type next_slot, gint id)
{
        GList *list_ptr = NULL;

        hslot_type hslot = (hslot_type) g_malloc (sizeof (slot_type));

        hslot->id = id;
        hslot->cards = NULL;

        if (id == 0) {
                hslot->x = DEALER_SLOT_ORIGIN_X;
                hslot->y = DEALER_SLOT_ORIGIN_Y;
        } else {
                hslot->x = PLAYER_SLOT_ORIGIN_X;
                hslot->y = PLAYER_SLOT_ORIGIN_Y;
        }

        hslot->dx = hslot->dy = 0;
        hslot->pixelx = hslot->pixely = 0;
        hslot->pixeldx = hslot->pixeldy = 0;
        hslot->expansion_depth = 0;

        hslot->dx = x_expanded_offset;
    
        hslot->length = hslot->exposed = 0;
        bj_slot_update_length (hslot);

        if (slot_list) {
                list_ptr = g_list_find (slot_list, next_slot);
                slot_list = g_list_insert_before (slot_list, list_ptr, hslot);
        }
        else {
                slot_list = g_list_alloc ();
                slot_list->data = hslot;
        }
        return hslot;
}

void 
bj_slot_pressed (gint x, gint y, hslot_type *slot, gint *cardid) 
{
        GList *tempptr;

        gint num_cards;
        gboolean got_slot = FALSE;

        *slot = NULL;
        *cardid = -1;

        for (tempptr = g_list_last (slot_list); tempptr; tempptr = tempptr->prev) {
                hslot_type hslot = (hslot_type) tempptr->data;

                /* if point is within our rectangle */
                if (hslot->pixelx < x && x < hslot->pixelx + hslot->width 
                    && hslot->pixely < y && y < hslot->pixely + hslot->height) {
                        num_cards = hslot->length;

                        if ( got_slot == FALSE || num_cards > 0 ) {  
                                /* if we support exposing more than one card,
                                 * find the exact card  */

                                gint depth = 1;
            
                                if (hslot->pixeldx > 0)		
                                        depth += (x - hslot->pixelx) / hslot->pixeldx;
                                else if (hslot->pixeldy > 0)
                                        depth += (y - hslot->pixely) / hslot->pixeldy;
            
                                /* account for the last card getting much more display area
                                 * or no cards */
            
                                if (depth > hslot->exposed)
                                        depth = hslot->exposed;

                                *slot = hslot;
            
                                /* card = #cards in slot + card chosen 
                                   (indexed in # exposed cards) - # exposed cards */

                                *cardid = num_cards + depth - hslot->exposed;
            
                                /* this is the topmost slot with a card */
                                /* take it and run*/
                                if ( num_cards > 0 )
                                        break;
            
                                got_slot = TRUE;
                        }
                }
        }
}

void 
bj_slot_update_length (hslot_type hslot) 
{
        gint delta = g_list_length (hslot->cards) - hslot->length;

        hslot->length += delta;
        hslot->exposed += delta;

        if (0 < hslot->expansion_depth && hslot->expansion_depth < hslot->exposed)
                hslot->exposed = hslot->expansion_depth;

        if ((hslot->pixeldx == 0 && hslot->pixeldy == 0 && hslot->exposed > 1) 
            || (hslot->length > 0 && hslot->exposed < 1))
                hslot->exposed = 1;

        delta = hslot->exposed ? hslot->exposed - 1 : 0;

        hslot->width = card_width + delta * hslot->pixeldx;
        hslot->height = card_height;
}

GList* 
bj_slot_get_list ()
{
        return slot_list;
}

hslot_type
bj_slot_get (gint slotid)
{
        GList* tempptr;
  
        for (tempptr = slot_list; tempptr; tempptr = tempptr->next)
                if (((hslot_type) tempptr->data)-> id == slotid)
                        return (hslot_type)tempptr->data;
        return NULL;
}

void
bj_slot_add_cards (GList* newcards, hslot_type hslot)
{

        hslot->cards = g_list_concat (hslot->cards, newcards);
        bj_slot_update_length (hslot);
}

void
bj_slot_delete (hslot_type hslot) 
{
        GList* temptr;
  
        for (temptr = hslot->cards; temptr; temptr = temptr->next)
                free (temptr->data);
        g_list_free (hslot->cards);
        free (hslot);
}

void
delete_surface ()
{
        GList* temptr;

        // delete card slots
        for (temptr = slot_list; temptr; temptr = temptr->next) {
                bj_slot_delete ((slot_type*)temptr->data);
                temptr->data = NULL;
        }

        g_list_free (slot_list);
        slot_list = NULL;

        // delete chip stacks
        bj_chip_stack_delete_all ();
}
