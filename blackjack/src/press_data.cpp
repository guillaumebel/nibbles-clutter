// -*- mode:C++; tab-width:8; c-basic-offset:8; indent-tabs-mode:nil -*-
/*
 * Blackjack - press_data.cpp
 *
 * Copyright (C) 2003-2004  William Jon McCann <mccann@jhu.edu>
 * Copyright (C) 1998, 2003 Jonathan Blandford <jrb@mit.edu>
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
#include <gdk/gdk.h>
#include "press_data.h"
#include "card.h"
#include "draw.h"
#include "slot.h"
#include "chips.h"
#include "blackjack.h"

press_data_type *press_data;

void
bj_press_data_create (void)
{
        GdkWindowAttr attributes;

        attributes.wclass = GDK_INPUT_OUTPUT;
        attributes.window_type = GDK_WINDOW_CHILD;
        attributes.event_mask = 0;
        attributes.width = card_width;
        attributes.height = card_height;
        attributes.colormap = gdk_drawable_get_colormap (GDK_DRAWABLE (playing_area->window));
        attributes.visual = gdk_drawable_get_visual (GDK_DRAWABLE (playing_area->window));
  
        press_data = (press_data_type*) g_malloc0 (sizeof (press_data_type));
        press_data->moving_cards = gdk_window_new (playing_area->window, &attributes,
                                                   (GDK_WA_VISUAL | GDK_WA_COLORMAP));
        press_data->status = 0;
}

void
bj_press_data_generate (void)
{
        GList *tempptr;
        hslot_type hslot = press_data->hslot;
        gint delta, height, width, x, y;
        GdkGC *gc1, *gc2;
        GdkColor masked = {0, 0, 0, 0}, unmasked = {1, 65535, 65535, 65535};

        delta = hslot->exposed - (hslot->length - press_data->cardid) - 1;
        press_data->xoffset -= x = hslot->pixelx + delta * hslot->pixeldx;
        press_data->yoffset -= y = hslot->pixely + delta * hslot->pixeldy;

        press_data->cards = g_list_nth (hslot->cards, press_data->cardid - 1);
        width = card_width + (hslot->length - press_data->cardid) * hslot->pixeldx;
        height = card_height + (hslot->length - press_data->cardid) * hslot->pixeldy;
  
        gdk_window_resize (press_data->moving_cards, width, height);
        gdk_window_move (press_data->moving_cards, x, y);

        press_data->moving_pixmap = 
                gdk_pixmap_new (press_data->moving_cards, width, height,
                                gdk_drawable_get_visual (press_data->moving_cards)->depth);
        press_data->moving_mask = 
                gdk_pixmap_new (press_data->moving_cards, width, height, 1);

        gc1 = gdk_gc_new (press_data->moving_pixmap);
        gc2 = gdk_gc_new (press_data->moving_mask);

        gdk_gc_set_foreground (gc2, &masked);
        gdk_draw_rectangle (press_data->moving_mask, gc2, TRUE, 0, 0, width, height);
        gdk_gc_set_foreground (gc2, &unmasked);

        gdk_gc_set_clip_mask (gc1, bj_card_get_mask ()); 
        gdk_gc_set_clip_mask (gc2, bj_card_get_mask ()); 

        x = y = 0; width = card_width; height = card_height;

        for (tempptr = press_data->cards; tempptr; tempptr = tempptr->next) {
                hcard_type hcard = (hcard_type) tempptr->data; 
                GdkPixmap* cardpix;
      
                if (hcard->direction == UP)
                        cardpix = bj_card_get_picture (hcard->suit, hcard->value);
                else
                        cardpix = bj_card_get_back_pixmap ();

                gdk_gc_set_clip_origin (gc1, x, y);
                gdk_gc_set_clip_origin (gc2, x, y);
                if (cardpix != NULL)
                        gdk_draw_drawable (press_data->moving_pixmap, gc1, cardpix,
                                           0, 0, x, y, width, height);
                gdk_draw_rectangle (press_data->moving_mask, gc2, TRUE, 
                                    x, y, width, height);
      
                x += hslot->pixeldx; y += hslot->pixeldy;
        }
        g_object_unref (gc1);
        g_object_unref (gc2);
  
        gdk_window_set_back_pixmap (press_data->moving_cards, 
                                    press_data->moving_pixmap, 0);
        gdk_window_shape_combine_mask (press_data->moving_cards, 
                                       press_data->moving_mask, 0, 0);
        gdk_window_show (press_data->moving_cards);

        if (press_data->cards->prev) 
                press_data->cards->prev->next = NULL;
        else 
                hslot->cards = NULL;

        press_data->cards->prev = NULL;
        bj_slot_update_length (press_data->hslot);
}

chip_stack_press_data_type *chip_stack_press_data;

void
bj_chip_stack_press_data_create (void)
{
        GdkWindowAttr attributes;

        attributes.wclass = GDK_INPUT_OUTPUT;
        attributes.window_type = GDK_WINDOW_CHILD;
        attributes.event_mask = 0;
        attributes.width = chip_width;
        attributes.height = attributes.width;
        attributes.colormap = gdk_drawable_get_colormap (GDK_DRAWABLE (playing_area->window));
        attributes.visual = gdk_drawable_get_visual (GDK_DRAWABLE (playing_area->window));
  
        chip_stack_press_data = 
                (chip_stack_press_data_type*) g_malloc0 (sizeof (chip_stack_press_data_type));
        chip_stack_press_data->moving_chips = gdk_window_new (playing_area->window, 
                                                              &attributes,
                                                              (GDK_WA_VISUAL 
                                                               | GDK_WA_COLORMAP));
        chip_stack_press_data->status = 0;
}

void
bj_chip_stack_press_data_generate (void)
{
        GList *tempptr;
        hstack_type hstack = chip_stack_press_data->hstack;
        gint delta, height, width, x, y;
        GdkGC *gc1, *gc2;
        GdkColor masked = {0, 0, 0, 0}, unmasked = {1, 65535, 65535, 65535};

        delta = hstack->length - 1;
        chip_stack_press_data->xoffset -= x = hstack->pixelx + delta * hstack->pixeldx;
        chip_stack_press_data->yoffset -= y = hstack->pixely - delta * hstack->pixeldy;

        chip_stack_press_data->chips = g_list_nth (hstack->chips, 
                                                   chip_stack_press_data->chipid);
        width = chip_width
                + (hstack->length - chip_stack_press_data->chipid) * hstack->pixeldx;
        height = chip_width
                + (hstack->length - chip_stack_press_data->chipid) * hstack->pixeldy;

        gdk_window_resize (chip_stack_press_data->moving_chips, width, height);
        gdk_window_move (chip_stack_press_data->moving_chips, x, y);

        chip_stack_press_data->moving_pixmap = 
                gdk_pixmap_new (chip_stack_press_data->moving_chips, width, height,
                                gdk_drawable_get_visual 
                                (chip_stack_press_data->moving_chips)->depth);
        chip_stack_press_data->moving_mask = 
                gdk_pixmap_new (chip_stack_press_data->moving_chips, width, height, 1);

        gc1 = gdk_gc_new (chip_stack_press_data->moving_pixmap);
        gc2 = gdk_gc_new (chip_stack_press_data->moving_mask);

        gdk_gc_set_foreground (gc2, &masked);
        gdk_draw_rectangle (chip_stack_press_data->moving_mask, gc2, TRUE, 
                            0, 0, width, height);
        gdk_gc_set_foreground (gc2, &unmasked);

        GdkBitmap *mask;
        gdk_pixbuf_render_pixmap_and_mask (bj_chip_get_scaled_pixbuf (1), NULL, &mask, 127);
        gdk_gc_set_clip_mask (gc1, mask); 
        gdk_gc_set_clip_mask (gc2, mask); 

        x = 0;
        y = (hstack->length - chip_stack_press_data->chipid - 1) * hstack->pixeldy;
        width = height = chip_width; 

        for (tempptr=chip_stack_press_data->chips; tempptr; tempptr=tempptr->next) {
                hchip_type hchip = (hchip_type) tempptr->data; 
                GdkPixbuf *chippix;
      
                chippix = bj_chip_get_scaled_pixbuf (bj_chip_get_id (hchip->value));
                gdk_gc_set_clip_origin (gc1, x, y);
                gdk_gc_set_clip_origin (gc2, x, y);
                if (chippix != NULL)
                        gdk_draw_pixbuf (chip_stack_press_data->moving_pixmap, gc1, chippix,
                                         0, 0, x, y, width, height, GDK_RGB_DITHER_MAX, 0, 0);
                gdk_draw_rectangle (chip_stack_press_data->moving_mask, gc2, TRUE, 
                                    x, y, width, height);
      
                x += hstack->pixeldx; y -= hstack->pixeldy;
        }
        g_object_unref (gc1);
        g_object_unref (gc2);
  
        gdk_window_set_back_pixmap (chip_stack_press_data->moving_chips, 
                                    chip_stack_press_data->moving_pixmap, 0);
        gdk_window_shape_combine_mask (chip_stack_press_data->moving_chips, 
                                       chip_stack_press_data->moving_mask, 0, 0);
        gdk_window_show (chip_stack_press_data->moving_chips);

        if (chip_stack_press_data->chips->prev) 
                chip_stack_press_data->chips->prev->next = NULL;
        else 
                hstack->chips = NULL;

        chip_stack_press_data->chips->prev = NULL;
        bj_chip_stack_update_length (chip_stack_press_data->hstack);
}

/* This does slightly more than free data, it also hides the window for
 * instance, but it is certainly in the correct spirit. */
void
bj_press_data_free (void)
{
        if (press_data == NULL)
                return;

        if (press_data->moving_cards)
                gdk_window_hide (press_data->moving_cards);

        if (press_data->moving_pixmap) {
                g_object_unref (press_data->moving_pixmap);
                press_data->moving_pixmap = NULL;
        }
        if (press_data->moving_mask) {
                g_object_unref (press_data->moving_mask);
                press_data->moving_mask = NULL;
        }
        press_data->status = STATUS_NONE;
        /* FIXME: I think there is a memory leak here, but I haven't managed
         * to figure out where the list is extracted from yet. This is what
         * most of the rest of the code does. */
        press_data->cards = NULL;
}

void
bj_chip_stack_press_data_free (void)
{
        if (chip_stack_press_data == NULL)
                return;

        if (chip_stack_press_data->moving_chips)
                gdk_window_hide (chip_stack_press_data->moving_chips);

        if (chip_stack_press_data->moving_pixmap) {
                g_object_unref (chip_stack_press_data->moving_pixmap);
                chip_stack_press_data->moving_pixmap = NULL;
        }
        if (chip_stack_press_data->moving_mask) {
                g_object_unref (chip_stack_press_data->moving_mask);
                chip_stack_press_data->moving_mask = NULL;
        }
        chip_stack_press_data->status = STATUS_NONE;
        /* FIXME: I think there is a memory leak here, but I haven't managed
         * to figure out where the list is extracted from yet. This is what
         * most of the rest of the code does. */
        chip_stack_press_data->chips = NULL;
}
