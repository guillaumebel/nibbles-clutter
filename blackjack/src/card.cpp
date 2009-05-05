// -*- mode:C++; tab-width:8; c-basic-offset:8; indent-tabs-mode:nil -*-
/* 
 * Blackjack - card.cpp
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

#include "blackjack.h"
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <libgames-support/games-card-theme.h>
#include <libgames-support/games-card-images.h>
#include <libgames-support/games-card-selector.h>
#include "card.h"
#include "chips.h"
#include "draw.h"

#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

using namespace std;

GdkBitmap *mask;

GamesCardThemes *theme_manager = NULL;
GamesCardTheme *theme = NULL;
GamesCardImages *images = NULL;

GdkPixmap *
bj_card_get_picture (gint suit, gint rank) 
{
        return games_card_images_get_card_pixmap_by_suit_and_rank (images,
                                                                   suit,
                                                                   (rank == 14) ? 1 : rank);
}

GdkPixmap *
bj_card_get_back_pixmap ()
{
        return games_card_images_get_card_pixmap_by_id (images, GAMES_CARD_BACK, FALSE);
}

GdkBitmap *
bj_card_get_mask ()
{
        return mask;
}

int
bj_card_get_vert_start ()
{
        return 30;
}

int
bj_card_get_horiz_start ()
{
        return 30;
}

GdkPixbuf *
get_pixbuf (const char *filename)
{
        GdkPixbuf *im;

        im = gdk_pixbuf_new_from_file (filename, NULL);

        return im;
}

GdkPixmap *
get_pixmap (const char *filename)
{
        GdkPixmap *ret;
        GdkPixbuf *im;

        im = gdk_pixbuf_new_from_file (filename, NULL);
        if (im != NULL) {
                gdk_pixbuf_render_pixmap_and_mask (im, &ret, NULL, 127);
                g_object_unref (im);
        } 
        else 
                ret = NULL;

        return ret;
}

hcard_type 
bj_card_new (gint value, gint suit, gint direction)
{
        hcard_type temp_card = (hcard_type) g_malloc (sizeof (card_type));

        temp_card->value = value;
        temp_card->suit = suit;
        temp_card->direction = direction;

        return temp_card;
}

void
bj_card_set_size (gint width, gint height)
{
        CardSize card_size;

        if (!theme_manager) {
                theme_manager = games_card_themes_new ();
        }
        if (!theme) {
                char *card_theme;

                card_theme = bj_get_card_style ();
                theme = games_card_themes_get_theme_by_name (theme_manager, card_theme);
                g_free (card_theme);

                if (!theme) {
                        /* Last-ditch fallback: try getting *any* theme */
                        theme = games_card_themes_get_theme_any (theme_manager);
                }
                if (!theme) {
                        /* No more options; quit. */
                        g_warning ("Failed to load any theme !");
                        exit (1);
                }

                images = games_card_images_new ();
                games_card_images_set_theme (images, theme);
                g_object_unref (theme);

                games_card_images_set_cache_mode (images, CACHE_PIXMAPS);
                games_card_images_set_drawable (images, playing_area->window);
        }

        games_card_theme_set_size (theme, width, height, 1.0);
        games_card_images_get_size (images, &card_size);

        bj_slot_set_size (card_size.width, card_size.height);
        bj_chip_set_size (card_size.width / 2, card_size.width / 2);

        mask = games_card_images_get_card_mask (images);
        gdk_gc_set_clip_mask (draw_gc, mask);
}

void
bj_card_set_theme (gchar *card_theme)
{
        GamesCardTheme *new_theme;

        g_assert (theme_manager != NULL);
        g_assert (theme != NULL);

        new_theme = games_card_themes_get_theme_by_name (theme_manager, card_theme);
        if (!new_theme) {
                g_warning ("Failed to load theme %s\n", card_theme);
                return;
        }

        theme = new_theme;
        games_card_images_set_theme (images, theme);
        g_object_unref (theme);

        bj_draw_rescale_cards ();
        mask = games_card_images_get_card_mask (images);
        gdk_gc_set_clip_mask (draw_gc, mask);
}

void
card_deck_options_changed (GtkWidget *w, GamesCardThemeInfo *info, gpointer data)
{
  g_assert (info != NULL);

  bj_set_card_style (games_card_theme_info_get_persistent_name (info));
}

GtkWidget *
bj_get_card_theme_selector ()
{
  GtkWidget *selector;

  if (!theme_manager || !theme)
    return NULL;

  selector = games_card_selector_new (theme_manager, games_card_theme_get_theme_info (theme));
  g_signal_connect (selector, "changed",
                    G_CALLBACK (card_deck_options_changed), NULL);

  return selector;
}
