// -*- mode:C++; tab-width:2; c-basic-offset:2; indent-tabs-mode:nil -*-
/*
 * Blackjack - blackjack.h
 *
 * Copyright (C) 2003-2005 William Jon McCann <mccann@jhu.edu>
 *
 * This game is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef BLACKJACK_H
#define BLACKJACK_H

#include <gtk/gtk.h>

#include "press_data.h"
#include "strategy.h"

// Constants

#define BJ_RULES_DIR DATADIR G_DIR_SEPARATOR_S "blackjack" G_DIR_SEPARATOR_S "rules"

#define GAME_EVENTS (GDK_EXPOSURE_MASK        |\
		                 GDK_BUTTON_PRESS_MASK    |\
		                 GDK_BUTTON_RELEASE_MASK  |\
                     GDK_POINTER_MOTION_MASK  |\
                     GDK_ENTER_NOTIFY_MASK )

#define KEY_S GDK_s
#define KEY_P GDK_p
#define KEY_H GDK_h
#define KEY_D GDK_d
#define KEY_R GDK_r
#define KEY_F1 GDK_F1
#define KEY_ENTER GDK_Return

#define DEALER_SLOT_ORIGIN_X 0.5
#define DEALER_SLOT_ORIGIN_Y 0.0
#define PLAYER_SLOT_ORIGIN_X 0.5
#define PLAYER_SLOT_ORIGIN_Y 0.45
#define PLAYER_SLOT_SPACING 0.1

#define KEY_SHOW_TOOLBAR       "toolbar"

#define KEY_GLOBAL_GROUP       "global"
#define KEY_BALANCE            "balance"

#define KEY_SETTINGS_GROUP     "settings"
#define KEY_GAME_VARIATION     "variation"
#define KEY_SHOW_PROBABILITIES "show_probabilities"
#define KEY_QUICK_DEAL         "quick_deal"
#define KEY_NEVER_INSURANCE    "never_insurance"

#define KEY_DECK_GROUP         "deck"
#define KEY_CARD_STYLE         "card_style"

// Global variables

extern GtkWidget *toplevel_window;
extern GtkWidget *playing_area;
extern GtkWidget *option_dialog;
extern GdkGC *draw_gc;
extern GdkGC *bg_gc;
extern GdkGC *slot_gc;
extern GdkPixmap *surface;

extern GObject *card_deck;
extern gchar *card_style;

extern GtkProgressBar *progress_bar;
extern GtkWidget *status_bar;

extern guint timeout;
extern guint seed;

extern guint x_spacing;
extern guint y_spacing;
extern double x_expanded_offset;
extern double y_expanded_offset;

gboolean bj_quit_app (void);
void bj_make_window_title (gchar * game_name);
void bj_update_control_menu (void);

void bj_show_balance (gfloat);
void bj_show_shoe_cards (gfloat);
void bj_gui_show_toolbar (gboolean do_toolbar);

gdouble bj_get_wager (void);
void bj_set_wager (gdouble);
void bj_adjust_wager (gdouble);

void bj_adjust_balance (gdouble);
gdouble bj_get_balance (void);
void bj_set_balance (gdouble);

gchar *bj_get_card_style (void);
void bj_set_card_style (const char *);
gboolean bj_get_show_probabilities (void);
void bj_set_show_probabilities (gboolean);
gboolean bj_get_show_toolbar (void);
void bj_set_show_toolbar (gboolean);
gboolean bj_get_quick_deal (void);
void bj_set_quick_deal (gboolean);
gboolean bj_get_never_insurance (void);
void bj_set_never_insurance (gboolean);
const gchar *bj_get_game_variation (void);
void bj_set_game_variation (const gchar *);

extern gint dealerSpeed;
extern gfloat lastWager;

extern gint numHands;
extern gboolean allSettled;

extern gchar *start_game;

#endif
