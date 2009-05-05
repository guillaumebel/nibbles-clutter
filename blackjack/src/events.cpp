// -*- mode:C++; tab-width:8; c-basic-offset:8; indent-tabs-mode:nil -*-
/*
 * Blackjack - events.cpp
 *
 * Copyright (C) 2003-2005 William Jon McCann <mccann@jhu.edu>
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

#include <math.h>
#include <glib/gi18n.h>
#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

using namespace std;

#include "events.h"
#include "blackjack.h"
#include "draw.h"
#include "dialog.h"
#include "slot.h"
#include "card.h"
#include "menu.h"
#include "chips.h"
#include "press_data.h"

#include "hand.h"
#include "game.h"

/* These have been lifted straight from eog. */
  	 
#define hand_closed_data_width 20
#define hand_closed_data_height 20
static char hand_closed_data_bits[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x80, 0x3f, 0x00,
        0x80, 0xff, 0x00, 0x80, 0xff, 0x00, 0xb0, 0xff, 0x00, 0xf0, 0xff, 0x00,
        0xe0, 0xff, 0x00, 0xe0, 0x7f, 0x00, 0xc0, 0x7f, 0x00, 0x80, 0x3f, 0x00,
        0x00, 0x3f, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  	 
#define hand_closed_mask_width 20
#define hand_closed_mask_height 20
static char hand_closed_mask_bits[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x80, 0x3f, 0x00, 0xc0, 0xff, 0x00,
        0xc0, 0xff, 0x01, 0xf0, 0xff, 0x01, 0xf8, 0xff, 0x01, 0xf8, 0xff, 0x01,
        0xf0, 0xff, 0x01, 0xf0, 0xff, 0x00, 0xe0, 0xff, 0x00, 0xc0, 0x7f, 0x00,
        0x80, 0x7f, 0x00, 0x80, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  	 
#define hand_open_data_width 20
#define hand_open_data_height 20
static char hand_open_data_bits[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00,
        0x60, 0x36, 0x00, 0x60, 0x36, 0x00, 0xc0, 0x36, 0x01, 0xc0, 0xb6, 0x01,
        0x80, 0xbf, 0x01, 0x98, 0xff, 0x01, 0xb8, 0xff, 0x00, 0xf0, 0xff, 0x00,
        0xe0, 0xff, 0x00, 0xe0, 0x7f, 0x00, 0xc0, 0x7f, 0x00, 0x80, 0x3f, 0x00,
        0x00, 0x3f, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  	 
#define hand_open_mask_width 20
#define hand_open_mask_height 20
static char hand_open_mask_bits[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x60, 0x3f, 0x00,
        0xf0, 0x7f, 0x00, 0xf0, 0x7f, 0x01, 0xe0, 0xff, 0x03, 0xe0, 0xff, 0x03,
        0xd8, 0xff, 0x03, 0xfc, 0xff, 0x03, 0xfc, 0xff, 0x01, 0xf8, 0xff, 0x01,
        0xf0, 0xff, 0x01, 0xf0, 0xff, 0x00, 0xe0, 0xff, 0x00, 0xc0, 0x7f, 0x00,
        0x80, 0x7f, 0x00, 0x80, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

hslot_type last_hslot = NULL;
gint last_cardid = -1;

hstack_type last_hstack = NULL;
gint last_chipid = -1;

GTimer *click_timer = NULL;
gdouble dbl_click_time;

GdkCursor * hand_closed_cursor = NULL;
GdkCursor * hand_open_cursor = NULL;
GdkCursor * default_cursor = NULL;
  	 
enum {
        CURSOR_NONE = 0,
        CURSOR_OPEN,
        CURSOR_CLOSED
};
  	 
static void
set_cursor (int cursor)
{
        if (cursor == CURSOR_NONE)
                gdk_window_set_cursor (playing_area->window, default_cursor);
        if (cursor == CURSOR_OPEN)
                gdk_window_set_cursor (playing_area->window, hand_open_cursor);
        if (cursor == CURSOR_CLOSED)
                gdk_window_set_cursor (playing_area->window, hand_closed_cursor);
}
  	 
/* If we are over a slot, set the cursor to the given cursor,
 * otherwise use the default cursor. */
static void
set_cursor_by_location (int cursor, int x, int y)
{
        hslot_type hslot;
        gint cardid;
        hstack_type hstack;
        gint chipid; 

        bj_slot_pressed (x, y, &hslot, &cardid);
        bj_chip_stack_pressed (x, y, &hstack, &chipid);
        if (hslot || hstack)
                set_cursor (cursor);
        else
                set_cursor (CURSOR_NONE);
}

gboolean
bj_event_key_press (GtkWidget *widget, GdkEventKey *event, void *d)
{
        guint    key;
        gboolean events_pending = bj_hand_events_pending ();

        // ignore clicks during actions
        if (events_pending)
                return TRUE;

        key = event->keyval;

        // Carry out player option.
        switch (key) {
        case KEY_ENTER :
                bj_hand_new_deal ();
                return TRUE;
                break;
        default:
                break;
        }

        return FALSE;
}

int
waiting_for_mouse_up (void)
{
        if (!press_data)
                return 0;
        if (press_data->status == STATUS_IS_DRAG)
                return 1;
        if (press_data->status == STATUS_SHOW)
                return 1;
        return 0;
}

/* Button press statuses */

static void
drop_moving_cards (gint x, gint y)
{
        hslot_type hslot;
        gint cardid; 
        gboolean moved = false;
        gint width, height;
        gboolean do_split = false;

        bj_slot_pressed (x + card_width / 2 - press_data->xoffset, 
                         y + card_height / 2 - press_data->yoffset, 
                         &hslot, &cardid);

        if (!hslot) {
                if (press_data->hslot == bj_hand_get_slot ()) {
                        do_split = true;
                }
        }

        if (!moved) {
                hslot = press_data->hslot;
                bj_slot_add_cards (press_data->cards, hslot);
        }

        bj_slot_update_length (hslot);
        press_data->cards = NULL;

        gdk_drawable_get_size (press_data->moving_cards, &width, &height);
        gdk_window_move (press_data->moving_cards, 
                         hslot->pixelx + hslot->width - width, 
                         hslot->pixely + hslot->height - height);

        bj_press_data_free ();

        bj_draw_refresh_screen ();

        if (do_split)
                bj_hand_split ();

}

static void
drop_moving_chips (gint x, gint y)
{
        hstack_type hstack;
        gint chipid; 
        gboolean moved = false;

        bj_chip_stack_pressed (x, y, &hstack, &chipid);
        if (hstack) {
                if (hstack != chip_stack_press_data->hstack) {
                        if (bj_chip_stack_is_source (chip_stack_press_data->hstack)
                            && ! bj_chip_stack_is_source (hstack)) {
                                // Put chips back on source stack
                                bj_chip_stack_add_chips (chip_stack_press_data->chips, 
                                                         chip_stack_press_data->hstack);
                                bj_chip_stack_update_length (chip_stack_press_data->hstack);
                                
                                // Add chips value to wager
                                gfloat value = bj_chip_stack_get_chips_value 
                                        (chip_stack_press_data->chips);
                                bj_adjust_wager (value);
                                bj_clear_table ();
                                moved = true;
                        }
                        else if (! bj_chip_stack_is_source (chip_stack_press_data->hstack)
                                 && bj_chip_stack_is_source (hstack)) {
                                // Remove value from wager
                                // Make sure that minimum wager is maintained
                                gfloat value = bj_chip_stack_get_chips_value 
                                        (chip_stack_press_data->chips);
                                bj_adjust_wager (-1 * value);
                                bj_clear_table ();
                                moved = true;
                        }
                }
        }

        if (! moved) {
                bj_chip_stack_add_chips (chip_stack_press_data->chips,
                                         chip_stack_press_data->hstack);
                bj_chip_stack_update_length (chip_stack_press_data->hstack);
        }

        bj_chip_stack_press_data_free ();
        bj_draw_refresh_screen ();
}

static gint
handle_slot_pressed (GdkEventButton *event, hslot_type hslot, gint cardid)
{
        gboolean double_click;

        /* We can't let Gdk do the double-click detection since the entire playing
         * area is one big widget it can't distinguish between single-clicks on two
         * cards and a double-click on one. */
        double_click = (g_timer_elapsed (click_timer,NULL) < dbl_click_time)
                && (last_cardid == cardid)
                && (last_hslot->id == hslot->id);
        g_timer_start (click_timer);
        bj_slot_pressed ((gint)event->x, (gint)event->y, &last_hslot, &last_cardid);
  
        press_data->xoffset = (gint)event->x;
        press_data->yoffset = (gint)event->y;

        press_data->hslot = hslot;
        press_data->cardid = cardid;

        if (!(press_data->status == STATUS_CLICK && double_click))
                press_data->status = cardid > 0 ? STATUS_MAYBE_DRAG : STATUS_NOT_DRAG;

        if (cardid > 0 && press_data->status == STATUS_NONE) {
                hcard_type card = 
                        (hcard_type) g_list_nth (hslot->cards, cardid - 1)->data;
                
                if (card->direction == UP) {
                        guint delta = hslot->exposed - (hslot->length - cardid) - 1;
                        int x = hslot->pixelx + delta * hslot->pixeldx;
                        int y = hslot->pixely + delta * hslot->pixeldy;
          
                        press_data->status = STATUS_SHOW;
                        press_data->moving_pixmap = bj_card_get_picture (card->suit, card->value);
                        press_data->moving_mask = bj_card_get_mask ();
                        
                        if (press_data->moving_pixmap != NULL)
                                gdk_window_set_back_pixmap (press_data->moving_cards, 
                                                            press_data->moving_pixmap, 0);
                        gdk_window_shape_combine_mask (press_data->moving_cards, 
                                                       press_data->moving_mask, 0, 0);
                        gdk_window_move (press_data->moving_cards, x, y); 
                        gdk_window_show (press_data->moving_cards);
                }
        } 
        else if (double_click) {
                press_data->status = STATUS_NONE;
                //bj_draw_refresh_screen ();
                return TRUE;
        }
        return TRUE;
}

static gint
handle_chip_stack_pressed (GdkEventButton *event, 
                           hstack_type hstack,
                           gint chipid)
{
        gboolean double_click;

        if (bj_game_is_active () && bj_chip_stack_is_source (hstack)) {
                bj_hand_double ();
                return TRUE;
        }

        /* We can't let Gdk do the double-click detection since the entire playing
         * area is one big widget it can't distinguish between single-clicks on two
         * cards and a double-click on one. */
        double_click = (g_timer_elapsed (click_timer,NULL) < dbl_click_time)
                && (last_chipid == chipid)
                && (last_hstack == hstack);
        g_timer_start (click_timer);

        bj_chip_stack_pressed ((gint)event->x, (gint)event->y, 
                               &last_hstack, &last_chipid);
  
        chip_stack_press_data->xoffset = (gint)event->x;
        chip_stack_press_data->yoffset = (gint)event->y;

        chip_stack_press_data->hstack = hstack;
        chip_stack_press_data->chipid = chipid;

        if (!(chip_stack_press_data->status == STATUS_CLICK && double_click)) {
                chip_stack_press_data->status = 
                        chipid >= 0 ? STATUS_MAYBE_DRAG : STATUS_NOT_DRAG;
        }
        if (chipid >= 0 && chip_stack_press_data->status == STATUS_NONE) {
                guint delta = hstack->exposed - (hstack->length - chipid) - 1;
                int x = hstack->pixelx + delta * hstack->pixeldx;
                int y = hstack->pixely + delta * hstack->pixeldy;
                
                chip_stack_press_data->status = STATUS_SHOW;
                gfloat chip_value = 
                        ((hchip_type)g_list_nth_data (hstack->chips, chipid))->value;
                
                GdkPixbuf *pixbuf = bj_chip_get_scaled_pixbuf (bj_chip_get_id (chip_value));
                GdkPixmap *pixmap;
                GdkBitmap *mask;
                gdk_pixbuf_render_pixmap_and_mask (pixbuf, &pixmap, &mask, 127);
                chip_stack_press_data->moving_pixmap = pixmap;
                chip_stack_press_data->moving_mask = mask;
                
                if (chip_stack_press_data->moving_pixmap != NULL)
                        gdk_window_set_back_pixmap (chip_stack_press_data->moving_chips, 
                                                    chip_stack_press_data->moving_pixmap, 0);
                gdk_window_shape_combine_mask (chip_stack_press_data->moving_chips, 
                                               chip_stack_press_data->moving_mask, 0, 0);
                gdk_window_move (chip_stack_press_data->moving_chips, x, y); 
                gdk_window_show (chip_stack_press_data->moving_chips);
        } 
        else if (double_click) {
                chip_stack_press_data->status = STATUS_NONE;
                gfloat chip_value = 
                        ((hchip_type)g_list_last (hstack->chips)->data)->value;
                if (bj_chip_stack_is_source (hstack))
                        bj_adjust_wager (chip_value);
                else
                        bj_adjust_wager (-1 * chip_value);
                bj_clear_table ();
                bj_draw_refresh_screen ();
                return TRUE;
        }
        return TRUE;
}

gint
bj_event_button_press (GtkWidget *widget, GdkEventButton *event, void *d)
{
        gboolean events_pending = bj_hand_events_pending ();

        // ignore clicks during actions
        if (events_pending)
                return TRUE;

        if (event->button != 1)
                return TRUE;

        /* ignore the gdk synthetic click events */
        if (event->type != GDK_BUTTON_PRESS)
                return TRUE;
  
        if (press_data->status == STATUS_IS_DRAG
            || (press_data->status == STATUS_SHOW 
                || press_data->status == STATUS_IS_DRAG))
                return TRUE;

        if (chip_stack_press_data->status == STATUS_IS_DRAG
            || (chip_stack_press_data->status == STATUS_SHOW 
                || chip_stack_press_data->status == STATUS_IS_DRAG))
                return TRUE;

        gint chipid;
        hstack_type hstack;
        bj_chip_stack_pressed ((gint)event->x, (gint)event->y, &hstack, &chipid);
        if (hstack)
                return handle_chip_stack_pressed (event, hstack, chipid);

        gint cardid;
        hslot_type hslot;
        bj_slot_pressed ((gint)event->x, (gint)event->y, &hslot, &cardid);
        if (hslot)
                return handle_slot_pressed (event, hslot, cardid);

        return TRUE;
}

void
bj_slot_clicked ()
{
        if (! bj_game_is_active ())
                bj_hand_new_deal ();
        else {
                if (press_data->hslot == bj_hand_get_slot ())
                        bj_hand_hit_with_delay ();
                else
                        bj_hand_stand ();
        }
}

gint
bj_event_button_release (GtkWidget *widget, GdkEventButton *event, void *d)
{
        gboolean events_pending = bj_hand_events_pending ();

        // ignore clicks during actions
        if (events_pending)
                return TRUE;

        switch (press_data->status) {
        case STATUS_IS_DRAG:
                press_data->status = STATUS_NONE;
                drop_moving_cards ((gint)event->x, (gint)event->y);
                break;
        case STATUS_SHOW:
                bj_press_data_free ();
                break;
    
        case STATUS_MAYBE_DRAG:
        case STATUS_NOT_DRAG:
                press_data->status = STATUS_CLICK;
                bj_slot_clicked ();
                break;
      
        case STATUS_CLICK:
        case STATUS_NONE:
                break;
        }

        switch (chip_stack_press_data->status) {
        case STATUS_IS_DRAG:
                chip_stack_press_data->status = STATUS_NONE;
                drop_moving_chips ((gint)event->x, (gint)event->y);
                break;
        case STATUS_SHOW:
                bj_chip_stack_press_data_free ();
                break;
    
        case STATUS_MAYBE_DRAG:
        case STATUS_NOT_DRAG:
                chip_stack_press_data->status = STATUS_CLICK;
                //bj_slot_clicked ();
                break;
      
        case STATUS_CLICK:
        case STATUS_NONE:
                break;
        }

        set_cursor_by_location (CURSOR_OPEN, (int)event->x, (int)event->y);

        return TRUE;
}

static gint
handle_other_motion_event (GtkWidget *widget, GdkEventMotion *event)
{
        // This is primarily to display help information
        hstack_type hstack;
        gint chipid;
        guint context_id;

        context_id = gtk_statusbar_get_context_id (GTK_STATUSBAR (status_bar), "help");

        bj_chip_stack_pressed ((gint)(event->x), (gint)(event->y), &hstack, &chipid);
        if (hstack) {
                if (bj_chip_stack_is_source (hstack)) {
                        gtk_statusbar_pop (GTK_STATUSBAR (status_bar), context_id);

                        if (bj_hand_can_be_doubled ())
                                gtk_statusbar_push (GTK_STATUSBAR (status_bar), context_id,
                                                    _("Click to double your wager"));
                        else if (! bj_game_is_active ()) {
                                gfloat chip_value = 
                                        ((hchip_type)g_list_last (hstack->chips)->data)->value;
                                gchar *message = g_strdup_printf
                                        (_("Double click to increase your wager by %.2f"), 
                                         chip_value);
                                gtk_statusbar_push (GTK_STATUSBAR (status_bar), context_id,
                                                    message);
                                g_free (message);
                        }
                }
                else {
                        if (! bj_game_is_active ()) {
                                gtk_statusbar_pop (GTK_STATUSBAR (status_bar), context_id);

                                gfloat chip_value = ((hchip_type)g_list_last (hstack->chips)->data)->value;
                                gchar *message = g_strdup_printf (_("Double click to decrease your wager by %.2f"), 
                                                                  chip_value);
                                gtk_statusbar_push (GTK_STATUSBAR (status_bar), context_id,
                                                    message);
                                g_free (message);
                        }
                }
        }
        else {
                hslot_type hslot = NULL;
                gint cardid;
                bj_slot_pressed ((gint)event->x, (gint)event->y, &hslot, &cardid);
                if (hslot) {
                        gchar *message;
                        if (bj_game_is_active ()) { 
                                if (hslot == bj_hand_get_slot ())
                                        if (bj_hand_can_be_split ())
                                                message = g_strdup (_("Click to deal another card; drag card to split pair"));
                                        else
                                                message = g_strdup (_("Click to deal another card"));
                                else
                                        message = g_strdup (_("Click to finish adding cards to your hand"));
                        }
                        else
                                message = g_strdup (_("Click to deal a new hand"));
                        gtk_statusbar_pop (GTK_STATUSBAR (status_bar), context_id);
                        gtk_statusbar_push (GTK_STATUSBAR (status_bar), context_id,
                                            message);
                        g_free (message);
                }
                else {
                        // Not over anything we know about
                        gtk_statusbar_pop (GTK_STATUSBAR (status_bar), context_id);
                }
        }
        return FALSE;
}

static gint
handle_slot_motion_event (GtkWidget *widget, GdkEventMotion *event)
{
        if (press_data->status == STATUS_IS_DRAG) {
                gdk_window_move (press_data->moving_cards,  
                                 (gint)event->x - press_data->xoffset,
                                 (gint)event->y - press_data->yoffset);
                gdk_window_clear (press_data->moving_cards);
                set_cursor (CURSOR_CLOSED);
                return TRUE;
        }
        else if (press_data->status == STATUS_MAYBE_DRAG
                 && (fabs (press_data->xoffset - event->x) > 2.0 
                     || fabs (press_data->yoffset - event->y) > 2.0)) {
                press_data->status = STATUS_IS_DRAG;
                bj_press_data_generate ();
                bj_draw_take_snapshot();
                set_cursor (CURSOR_CLOSED);
                return TRUE;
        } else {
                set_cursor_by_location (CURSOR_OPEN, (int)event->x, (int)event->y);
        }
  
        return FALSE;
}

static gint
handle_chip_stack_motion_event (GtkWidget *widget, GdkEventMotion *event)
{
        if (chip_stack_press_data->status == STATUS_IS_DRAG) {
                gdk_window_move (chip_stack_press_data->moving_chips,  
                                 (gint)event->x - chip_stack_press_data->xoffset,
                                 (gint)event->y - chip_stack_press_data->yoffset);
                gdk_window_clear (chip_stack_press_data->moving_chips);
                set_cursor (CURSOR_CLOSED);
                return TRUE;
        }
        else if (chip_stack_press_data->status == STATUS_MAYBE_DRAG
                 && (fabs (chip_stack_press_data->xoffset - event->x) > 2.0 
                     || fabs (chip_stack_press_data->yoffset - event->y) > 2.0)) {
                chip_stack_press_data->status = STATUS_IS_DRAG;
                bj_chip_stack_press_data_generate ();
                bj_draw_take_snapshot();
                set_cursor (CURSOR_CLOSED);
                return TRUE;
        } else {
                set_cursor_by_location (CURSOR_OPEN, (int)event->x, (int)event->y);
        }
  
  
        return FALSE;
}

gint
bj_event_motion_notify (GtkWidget *widget, GdkEventMotion *event)
{
        if (! handle_slot_motion_event (widget, event))
                if (! handle_chip_stack_motion_event (widget, event))
                        handle_other_motion_event (widget, event);

        return TRUE;
}

gint
bj_event_enter_notify (GtkWidget *widget, GdkEventCrossing *event, void *d)
{
        gtk_widget_grab_focus (playing_area);

        return TRUE;
}

gint
bj_event_expose_callback (GtkWidget *widget, GdkEventExpose *event, void *d)
{
        bj_draw_refresh_screen ();

        return TRUE;
}

static GdkCursor *
make_cursor (char * data, char * mask_data)
{
        GdkColor fg = {0, 65535, 65535, 65535};
        GdkColor bg = {0, 0, 0, 0};
        GdkPixmap * source;
        GdkPixmap * mask;
        GdkCursor * cursor;
  	 
        /* Yeah, hard-coded sizes are bad. */
        source = gdk_bitmap_create_from_data (NULL, data, 20, 20);
        mask = gdk_bitmap_create_from_data (NULL, mask_data, 20, 20);
  	 
        cursor = gdk_cursor_new_from_pixmap (source, mask, &fg, &bg, 10, 10);
  	 
        g_object_unref (source);
        g_object_unref (mask);
  	 
        return cursor;
}

gint
bj_event_playing_area_configure (GtkWidget *widget, GdkEventConfigure *event)
{
        gint tmptime;
        GtkSettings *settings;

        if (!default_cursor)
                default_cursor = gdk_cursor_new(GDK_LEFT_PTR);
  	 
        if (!hand_closed_cursor)
                hand_closed_cursor = make_cursor (hand_closed_data_bits,
                                                  hand_closed_mask_bits);
  	 
        if (!hand_open_cursor)
                hand_open_cursor = make_cursor (hand_open_data_bits,
                                                hand_open_mask_bits);

        if (surface) {
                if (window_width == event->width && window_height == event->height)
                        return TRUE;
                g_object_unref (surface);
                surface = NULL;
        }
  
        if (!draw_gc) {
                draw_gc = gdk_gc_new (playing_area->window);
        }

        if (!slot_gc)
                slot_gc = gdk_gc_new (playing_area->window);

        if (!bg_gc) {
                bg_gc = gdk_gc_new (playing_area->window);
                if (get_background_pixmap ())
                        gdk_gc_set_tile (bg_gc, get_background_pixmap());
                gdk_gc_set_fill (bg_gc, GDK_TILED);
        }

        window_width = event->width;
        window_height = event->height;
                
        surface = gdk_pixmap_new (playing_area->window,
                                  event->width, event->height,
                                  gdk_drawable_get_visual (playing_area->window)->depth);

        bj_draw_rescale_cards ();
  
        bj_draw_refresh_screen ();

        /* Set up the double-click detection.*/
        if (!click_timer)
                click_timer = g_timer_new ();
        settings = gtk_settings_get_default ();
        g_object_get (settings, "gtk-double-click-time", &tmptime, NULL);
        dbl_click_time = tmptime / 1000.0;

        return FALSE;
}
