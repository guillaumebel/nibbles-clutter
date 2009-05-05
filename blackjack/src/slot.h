/* Blackjack - slot.h
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

#ifndef BJ_SLOT_H
#define BJ_SLOT_H

#include <glib.h>
#include "card.h"

/*
 * Data Structures
 */

typedef struct {

  gint id;
  GList *cards;
  double x;
  double y;
  double dx;
  double dy;
  gint pixelx;
  gint pixely;
  gint pixeldx;
  gint pixeldy;
  gint length;
  gint exposed;
  gint expansion_depth;
  gint width;
  gint height;

} slot_type;

typedef slot_type *hslot_type;

extern GList *slot_list;

/*
 * Functions
 */

void bj_slot_load_pixmaps (void);
void bj_slot_free_pixmaps (void);

GdkPixbuf *bj_slot_get_scaled_pixbuf (void);

void bj_slot_set_size (gint width, gint height);

void delete_surface ();
GdkPixmap *get_background_pixmap (void);

void bj_slot_add (gint);
hslot_type bj_slot_add_before_slot (hslot_type, gint);

void bj_slot_pressed (gint, gint, hslot_type *, gint *);

GList *bj_slot_get_list ();

void bj_slot_delete (hslot_type);
hslot_type bj_slot_get (gint);
void bj_slot_increase_length (hslot_type);
void bj_slot_reduce_length (hslot_type);
void bj_slot_add_cards (GList *, hslot_type hslot);
void bj_slot_update_length (hslot_type hslot);

#endif
