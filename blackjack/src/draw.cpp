// -*- mode:C++; tab-width:8; c-basic-offset:8; indent-tabs-mode:nil -*-
/*
 * Blackjack - draw.cpp
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

using namespace std;

#include "draw.h"
#include "blackjack.h"
#include "slot.h"
#include "card.h"
#include "chips.h"

#include "game.h"
#include "hand.h"

/* The ratio of height to width for a card. */
#define CARD_HW_RATIO (0.65)
  	 
/* The proportion of a slot dedicated to the card (horiz or vert). */
#define CARD_SLOT_PROP (0.8)

/* The size of the drawing area. */
int window_width = 0;
int window_height = 0;

/* The size of a slot in pixels. */
static double xslotstep = 0;
static double yslotstep = 0;

/* The size of a slot in scaled units. */
static int slot_width = 1;
static int slot_height = 2;

/* The size of a card. */
int card_width;
int card_height;

int chip_width;

/* The offset of the cards within the slot. */
int xoffset, yoffset;

gchar *dealer_markup = NULL;
gchar *player_markup = NULL;

static void
bj_draw_hand_counts ()
{
        bj_game_show_hand_counts ();
}

void
bj_draw_set_dealer_text (gchar *markup)
{
        if (dealer_markup)
                g_free (dealer_markup);

        if (markup)
                dealer_markup = g_strdup (markup);
        else
                dealer_markup = NULL;
}

void
bj_draw_set_player_text (gchar *markup)
{
        if (player_markup)
                g_free (player_markup);

        if (markup)
                player_markup = g_strdup (markup);
        else
                player_markup = NULL;
}

static void
bj_draw_paint_text (void)
{
        PangoLayout *layout;
        gint x, y, width, height;

        if (playing_area) {
                layout = gtk_widget_create_pango_layout (playing_area, "");
                if (dealer_markup)
                        pango_layout_set_markup (layout, dealer_markup, -1);
                x = y = 10;
                gtk_paint_layout (playing_area->style, playing_area->window, 
                                  (GtkStateType) GTK_WIDGET_STATE (playing_area),
                                  FALSE, NULL, playing_area, NULL, x, y, layout);
                pango_layout_get_size (layout, &width, &height);
                g_object_unref (layout);

                layout = gtk_widget_create_pango_layout (playing_area, "");
                if (player_markup)
                        pango_layout_set_markup (layout, player_markup, -1);
                x = 10;
                y = y + PANGO_PIXELS (height);
                gtk_paint_layout (playing_area->style, playing_area->window, 
                                  (GtkStateType) GTK_WIDGET_STATE (playing_area),
                                  FALSE, NULL, playing_area, NULL, x, y, layout);
                g_object_unref (layout);
        }
}

void
bj_draw_playing_area_text (gchar *markup, gint x, gint y)
{
        PangoLayout *layout;
        if (playing_area) {
                layout = gtk_widget_create_pango_layout (playing_area, "");
                pango_layout_set_markup (layout, markup, -1);
                gtk_paint_layout (playing_area->style, playing_area->window, 
                                  (GtkStateType) GTK_WIDGET_STATE (playing_area),
                                  FALSE, NULL, playing_area, NULL, x, y, layout);
                g_object_unref (layout);
        }
}

static void
bj_draw_chips ()
{
        GList* chip_stack;

        for (chip_stack = bj_chip_stack_get_list ();
             chip_stack; 
             chip_stack = chip_stack->next) {
                hstack_type hstack = (hstack_type) chip_stack->data;

                if (hstack->chips) {
                        GList *chip_list = hstack->chips; //g_list_nth (hstack->chips, hstack->length - hstack->exposed);
                        
                        gint x = hstack->pixelx;
                        gint y = hstack->pixely;
          
                        for (; chip_list; chip_list = chip_list->next) {
                                chip_type *chip = (chip_type*)chip_list->data;
                                
                                GdkPixbuf *image = bj_chip_get_scaled_pixbuf (bj_chip_get_id (chip->value));
	
                                if (image)
                                        gdk_draw_pixbuf (surface,
                                                         NULL,
                                                         image,
                                                         0, 0, 
                                                         x, y, 
                                                         -1, -1, 
                                                         GDK_RGB_DITHER_MAX,
                                                         0, 0);
                                
                                x += hstack->pixeldx;
                                y -= hstack->pixeldy;
                        }
                }
        }
}

static void
calculate_card_location (hslot_type hslot)
{
        int xofs, yofs;

        xofs = xoffset;
        yofs = yoffset;

        /* If this is an extended slot, position the cards to one side. */
        if (hslot->dx > 0.0)
                xofs = yofs;
        if (hslot->dy > 0.0)
                yofs = xofs;

        hslot->pixelx = (int)(window_width * hslot->x + xofs);
        hslot->pixely = (int)(window_height * hslot->y + yofs);
        hslot->pixeldx = (int)(hslot->dx * card_width);
        hslot->pixeldy = (int)(hslot->dy * card_height);

        bj_slot_update_length (hslot);
}

static void
calculate_stack_location (hstack_type hstack)
{
        int xofs, yofs;

        xofs = xoffset;
        yofs = yoffset;

        /* If this is an extended stack, position the cards to one side. */
        if (hstack->dx > 0.0)
                xofs = yofs;
        if (hstack->dy > 0.0)
                yofs = xofs;

        hstack->pixelx = (int)(window_width * hstack->x + xofs);
        hstack->pixely = (int)(window_height * hstack->y + yofs);
        //hstack->pixeldx = (int)(hstack->dx * chip_width);
        //hstack->pixeldy = (int)(hstack->dy * chip_width);
        hstack->pixeldy = chip_width / 5;

        bj_chip_stack_update_length (hstack);
}

/* Work out new sizes and spacings for the cards. */
void
bj_draw_set_geometry (gint new_width, gint new_height)
{
        double twidth, theight;

        slot_width = new_width;
        slot_height = new_height;

        /* We are called for two reasons: if the logical size of the board
         * has changed and if the physical size of the board has changed.
         * This catches the case where the logical size is set before the
         * physical size. In that case we ignore anything that needs
         * knowledge of the physical size. Yes, this probably is a sign that
         * the code needs reorganising. */
        if ((window_height == 0) || (window_width == 0))
                return;

        xslotstep = window_width / slot_width;
        yslotstep = window_height / slot_height;

        twidth = CARD_SLOT_PROP * xslotstep;
        theight = CARD_SLOT_PROP * yslotstep;
        if (twidth / theight < CARD_HW_RATIO) {
                card_height = (int) (twidth / CARD_HW_RATIO);
                card_width = (int)twidth;
        } else {
                card_width = (int) (CARD_HW_RATIO * theight);
                card_height = (int)theight;
        }
        xoffset = (int)(xslotstep - twidth) / 2;
        yoffset = (int)(yslotstep - theight) / 2;

        bj_card_set_size (card_width, card_height);

        chip_width = card_width / 2;
        
        /* Recalculate the slot locations. */
        g_list_foreach (bj_slot_get_list (),
                        (GFunc) calculate_card_location, NULL);

        g_list_foreach (bj_chip_stack_get_list (),
                        (GFunc) calculate_stack_location, NULL);
}

void
bj_draw_rescale_cards (void)
{
  bj_draw_set_geometry (slot_width, slot_height);
}

static void
bj_draw_cards ()
{
        GList *slot;

        for (slot = bj_slot_get_list (); slot; slot = slot->next) {
                hslot_type hslot = (hslot_type) slot->data;
      
                if (hslot->cards) {
                        gint x = hslot->pixelx;
                        gint y = hslot->pixely;
                        GList *card_list = g_list_nth (hslot->cards, hslot->length - hslot->exposed);

                        for (; card_list; card_list = card_list->next) {
                                GdkPixmap *image;
                                card_type *card = (card_type*)card_list->data;
                                
                                if (card->direction == DOWN) 
                                        image = bj_card_get_back_pixmap ();
                                else 
                                        image = bj_card_get_picture (card->suit,
                                                                     card->value);
                                
                                gdk_gc_set_clip_origin (draw_gc, x, y);
                                if (image)
                                        gdk_draw_drawable (surface, draw_gc, image,
                                                           0, 0,
                                                           x, y,
                                                           -1, -1);

                                x += hslot->pixeldx;
                                y += hslot->pixeldy;
                        }
                }
        }
}

void
bj_draw_take_snapshot ()
{
        GList* slot;

        gdk_draw_rectangle (surface, bg_gc, TRUE, 0, 0, -1, -1);

        for (slot = bj_slot_get_list (); slot; slot = slot->next) {
                GdkPixbuf *slot_pixbuf = bj_slot_get_scaled_pixbuf ();

                if (slot_pixbuf) {
                        gint x = ((hslot_type)slot->data)->pixelx;
                        gint y = ((hslot_type)slot->data)->pixely;
                        gdk_draw_pixbuf (surface,
                                         slot_gc,
                                         slot_pixbuf,
                                         0, 0,
                                         x, y,
                                         -1, -1,
                                         GDK_RGB_DITHER_MAX,
                                         0, 0);
                }
        }
        bj_draw_cards ();
        bj_draw_chips ();
        if (bj_game_is_active () && bj_get_show_probabilities ())
                bj_draw_paint_text ();
        gdk_window_set_back_pixmap (playing_area->window, surface, 0);
}

void
bj_draw_refresh_screen ()
{
        bj_draw_take_snapshot();
        gdk_window_clear (playing_area->window);
        if (bj_game_is_active () && bj_get_show_probabilities ())
                bj_draw_paint_text ();
        bj_draw_hand_counts ();
}
