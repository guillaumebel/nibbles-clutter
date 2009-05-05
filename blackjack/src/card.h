/* Blackjack - card.h
 * Copyright (C) 2003 William Jon McCann <mccann@jhu.edu>
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

#ifndef BJ_CARD_H
#define BJ_CARD_H

#include <gdk/gdk.h>

/*
 * Constants
 */

#define UP FALSE
#define DOWN TRUE

/*
 * Data Structures
 */

typedef struct {
  gint suit;
  gint value;
  gboolean direction;
} card_type;
typedef card_type *hcard_type;

/*
 * Functions
 */

GdkPixmap *bj_card_get_picture (gint, gint);
GdkPixmap *bj_card_get_back_pixmap (void);

GdkBitmap *bj_card_get_mask (void);

int bj_card_get_width (void);
int bj_card_get_height (void);
int bj_card_get_horiz_offset (void);
int bj_card_get_vert_offset (void);
int bj_card_get_vert_start (void);
int bj_card_get_horiz_start (void);

hcard_type bj_card_new (gint, gint, gint);

GdkPixbuf *get_pixbuf (const char *);
GdkPixmap *get_pixmap (const char *);

void bj_card_set_size (gint width, gint height);
void bj_card_set_theme (gchar * theme);

GtkWidget *bj_get_card_theme_selector ();

#endif
