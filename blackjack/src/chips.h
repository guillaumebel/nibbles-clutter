/* Blackjack - chips.h
 * Copyright (C) 2003 William Jon McCann <mccann@jhu.edu>
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

#ifndef BJ_CHIPS_H
#define BJ_CHIPS_H

#include <glib.h>

/*
 * Data Structures
 */

typedef struct {

  gint id;
  GList *chips;
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

} chip_stack_type;

typedef chip_stack_type *hstack_type;

typedef struct {
  gfloat value;
} chip_type;

typedef chip_type *hchip_type;

extern GList *chip_stack_list;
extern GdkPixbuf *chip_pixbuf[4];

/*
 * Functions
 */

gint bj_chip_get_width (void);
gint bj_chip_get_id (gfloat value);

void bj_chip_set_size (gint width, gint height);

GdkPixbuf *bj_chip_get_scaled_pixbuf (gint chip);

gdouble bj_chip_get_value (gint);

gfloat bj_chip_stack_get_chips_value (GList * chips);

void bj_chip_stack_add (gint, double, double);
hchip_type bj_chip_new (gfloat);

void bj_chip_stack_pressed (gint, gint, hstack_type *, gint *);

GList *bj_chip_stack_get_list (void);

void bj_chip_stack_delete (hstack_type);
void bj_chip_stack_delete_all (void);
void bj_chip_stack_delete_all_wagers (void);

void bj_chip_stack_add_chips (GList *, hstack_type);
void bj_chip_stack_update_length (hstack_type);

void bj_chip_stack_decompose_value (gfloat value, gint * blacks,
				    gint * greens, gint * reds,
				    gint * whites);

void bj_chip_stack_new_with_id_value (gint id, gfloat value, double x,
				      double y);
void bj_chip_stack_new_with_value (gfloat value, double x, double y);
void bj_chip_stack_new_source_with_value (gfloat value, double x, double y);

void bj_chip_stack_create_sources (void);

gboolean bj_chip_stack_is_source (hstack_type hstack);

enum {
  CHIP_100,
  CHIP_25,
  CHIP_5,
  CHIP_1
};

#endif
