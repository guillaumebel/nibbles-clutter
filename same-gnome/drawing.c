/* -*- mode: C; indent-tabs-mode: t; tab-width: 2; c-basic-offset: 2; -*- */

/* drawing.c: Code for drawing the game (well, duh).
 *
 * Copyright (c) 2004 by Callum McKenzie
 *
 */

#include <config.h>

#include <string.h>

#include <glib/gi18n.h>
#include <gtk/gtk.h>

#include <libgames-support/games-conf.h>
#include <libgames-support/games-runtime.h>

#include "same-gnome.h"

#include "drawing.h"
#include "game.h"
#include "input.h"
#include "ui.h"

/* Record offsets for the moving tiles to avoid any possibility of
 * error due to rounding effects and havint to do cut and paste math
 * routines. */
guint downoffsets[NFRAMESMOVED];
guint leftoffsets[NFRAMESMOVEL];

/* Tiles are guaranteed to be square. */
gint tile_size;

/* This keeps track of the idle handler for redrawing pixmaps. */
guint idle_id = 0;

/* A flag for whether we have valid pixmap data. */
gboolean pixmaps_ready = FALSE;

GdkPixmap *pixmaps[MAX_COLOURS][NFRAMES];
GdkPixmap *blank_pixmap = NULL;

/* These are allocated on an as-needed basis, but never disposed of. */
GdkGC *gridgc = NULL;
GdkGC *bggc = NULL;
GdkGC *cursorgc = NULL;

/* FIXME: These should not be hard-coded. */
GdkColor gridcolor = { 0, 0x5050, 0x5050, 0x5050 };
GdkColor bgcolor = { 0, 0x1010, 0x1010, 0x1010 };
GdkColor cursorcolor = { 0, 0xffff, 0xb7b7, 0x4646 };

/* FIXME: Can we make this not global ? (redraw is the problem) */
GtkWidget *canvaswidget;

gint drawing_area_width = 0;;

gint cursor_x = -1;
gint cursor_y = -1;
gboolean draw_cursor = FALSE;

gboolean fast_animation = FALSE;

static gint animation_timer_id = 0;

void
pixels_to_logical (gint px, gint py, gint * lx, gint * ly)
{
  *lx = px / tile_size;
  *ly = py / tile_size;

  *lx = CLAMP (*lx, 0, board_width - 1);
  *ly = CLAMP (*ly, 0, board_height - 1);
}

void
redraw (void)
{
  gtk_widget_queue_draw (canvaswidget);
}

/* Note the dy is _subtracted_ from y and dx is _added_ to x.
 * This is the natural way given the directions the balls fall. */
static void
draw_ball_with_offset (GtkWidget * canvas, game_cell * p, int x,
                       int y, int dx, int dy)
{
  int colour, frame;
  GdkPixmap *pixmap;

  colour = p->colour;
  frame = p->frame;

  if (colour == NONE) {
    /* We don't add an offset to the blank tile since it is part of 
     * the background. The background movement is compensated for when
     * the other tiles were created. */
    gdk_draw_drawable (canvas->window, canvas->style->black_gc,
                       blank_pixmap, 0, 0, x * tile_size,
                       y * tile_size, tile_size, tile_size);
  } else {
    pixmap = pixmaps[colour][frame];
    gdk_draw_drawable (canvas->window, canvas->style->black_gc,
                       pixmap, 0, 0, x * tile_size + dx,
                       y * tile_size - dy, tile_size, tile_size);
  }

  /* Draw the cursor is needed. */
  if (draw_cursor && (x == cursor_x) && (y == cursor_y)) {
    if (cursorgc == NULL) {
      cursorgc = gdk_gc_new (canvaswidget->window);
      gdk_colormap_alloc_color (gdk_colormap_get_system (),
                                &cursorcolor, TRUE, TRUE);
      gdk_gc_set_foreground (cursorgc, &cursorcolor);
      gdk_gc_set_line_attributes (cursorgc, 2, GDK_LINE_SOLID,
                                  GDK_CAP_BUTT, GDK_JOIN_MITER);
    }

    gdk_draw_rectangle (canvaswidget->window, cursorgc,
                        FALSE, x * tile_size + 2, y * tile_size + 2,
                        tile_size - 3, tile_size - 3);
  }

}

static void
draw_ball (GtkWidget * canvas, game_cell * p, int x, int y)
{
  draw_ball_with_offset (canvas, p, x, y, 0, 0);
}

void
fast_animation_enable (gboolean state)
{
  fast_animation = state;

  games_conf_set_boolean (NULL, KEY_FAST_ANIMATION, state);
}

void
cursor_erase (void)
{
  draw_cursor = FALSE;
  draw_ball (canvaswidget, get_game_cell (cursor_x, cursor_y),
             cursor_x, cursor_y);
}

void
cursor_draw (gint x, gint y)
{
  draw_cursor = TRUE;
  cursor_x = x;
  cursor_y = y;
  draw_ball (canvaswidget, get_game_cell (x, y), x, y);
}

gboolean
expose_cb (GtkWidget * canvas, GdkEventExpose * event, gpointer data)
{
  int x, y;
  game_cell *p;
  game_cell dummy = { -1, 0, 0, 0 };

  if (gridgc == NULL) {
    gridgc = gdk_gc_new (canvas->window);
    gdk_colormap_alloc_color (gdk_colormap_get_system (),
                              &gridcolor, TRUE, TRUE);
    gdk_gc_set_foreground (gridgc, &gridcolor);

    bggc = gdk_gc_new (canvas->window);
    gdk_colormap_alloc_color (gdk_colormap_get_system (),
                              &bgcolor, TRUE, TRUE);
    gdk_gc_set_foreground (bggc, &bgcolor);
  }

  /* We consider two cases. One is here we have resized and the pixmaps
   * aren't ready yet, in that case (the else block) we just draw the
   * grid. Otherwise we draw everything using the tiles. */

  if (pixmaps_ready) {

    p = board;
    for (y = 0; y < board_height; y++) {
      for (x = 0; x < board_width; x++) {
        /* Only draw stationary frames. Fill the rest in as blanks 
         * and let the animation sort it out. */
        if (p->frame == 0)
          draw_ball (canvas, p, x, y);
        else
          draw_ball (canvas, &dummy, x, y);
        p++;
      }
    }

    /* Fixup the left and bottom lines. */
    if ((event->area.y + event->area.height + 1) >= board_height * tile_size) {
      gdk_draw_line (canvas->window, gridgc,
                     event->area.x, board_height * tile_size,
                     event->area.x + event->area.width - 2,
                     board_height * tile_size);
    }
    if ((event->area.x + event->area.width + 1) >= board_width * tile_size) {
      gdk_draw_line (canvas->window, gridgc,
                     board_width * tile_size, event->area.y,
                     board_width * tile_size,
                     event->area.y + event->area.height - 2);
    }

  } else {                        /* Draw only the grid. */

    gdk_draw_rectangle (canvas->window, bggc, TRUE,
                        event->area.x, event->area.y,
                        event->area.width, event->area.height);

    /* Vertical lines. */
    for (x = tile_size * (event->area.x / tile_size);
         x <= tile_size * ((event->area.x + event->area.width) / tile_size);
         x += tile_size) {
      gdk_draw_line (canvas->window, gridgc, x,
                     event->area.y, x, event->area.y + event->area.height);
    }

    /* Horizontal lines. */
    for (x = tile_size * (event->area.y / tile_size);
         x <= tile_size * ((event->area.y + event->area.height) / tile_size);
         x += tile_size) {
      gdk_draw_line (canvas->window, gridgc,
                     event->area.x, x, event->area.x + event->area.width, x);
    }

  }

  return TRUE;
}

/* FIXME: This should be a two-stage thing. There should be a flag to say, 
 * "I've done the static balls" so they can be drawn and then another
 * goes up when the animations are rendered so the animation can start.
 * Admittedly I think the bottleneck may all be in the self-imposed
 * delay: measure ! */

/* Draw the pixmaps one at a time until they are all done. 
 * We employ a simple state machine to track what has to be done
 * between calls. This operates from the idle handler.  */

/* FIXME: Too much detail in one function. */
static gboolean
render_cb (GtkWidget * canvas)
{
  static enum { INIT, DRAW, DEST, MOVED, MOVEL, DONE } idle_state = INIT;
  static GdkPixbuf *file_pixbuf = NULL;
  static GdkPixbuf *bg_pixbuf = NULL;
  static gint last_tile_size = 0;
  static gint ftile_size;
  static gint n, m, x;
  GdkPixbuf *tile;
  int i, j, l;
  guchar *p, *bp;
  guint shift;
  gchar *filename;
  gchar *substring;
  gchar *suffix;
  gint length;
  GtkWidget *dialog;
  static guchar r, g, b, br, bg, bb;
  guchar tr, tg, tb;
    
  if (idle_state == INIT) {
    last_tile_size = tile_size;
    ftile_size = tile_size - 1;
    n = 0;
    m = 0;

    if (file_pixbuf != NULL)
      g_object_unref (file_pixbuf);
    if (bg_pixbuf != NULL)
      g_object_unref (bg_pixbuf);

    /* Draw up the background with the top and left grid lines. */
    bg_pixbuf = gdk_pixbuf_new (GDK_COLORSPACE_RGB, FALSE, 8, tile_size,
                                tile_size);
    /* FIXME: This is too large for this function, ship off else-where. */
    p = gdk_pixbuf_get_pixels (bg_pixbuf);
    l = gdk_pixbuf_get_rowstride (bg_pixbuf);
    r = gridcolor.red >> 8;
    g = gridcolor.green >> 8;
    b = gridcolor.blue >> 8;
    br = bgcolor.red >> 8;
    bg = bgcolor.green >> 8;
    bb = bgcolor.blue >> 8;
    for (i = 0; i < tile_size; i++) {
      *p++ = r;
      *p++ = g;
      *p++ = b;
    }
    for (j = 1; j < tile_size; j++) {
      p = p + l - 3 * tile_size;
      *p++ = r;
      *p++ = g;
      *p++ = b;
      for (i = 1; i < tile_size; i++) {
        *p++ = br;
        *p++ = bg;
        *p++ = bb;
      }
    }
    if (blank_pixmap != NULL)
      g_object_unref (blank_pixmap);
    blank_pixmap = gdk_pixmap_new (canvas->window, tile_size, tile_size, -1);
    gdk_draw_pixbuf (blank_pixmap, canvas->style->black_gc, bg_pixbuf,
                     0, 0, 0, 0, tile_size, tile_size, GDK_RGB_DITHER_NORMAL,
                     0, 0);

    /* This code is far from perfect. For starters it only checks to
     * see if the file exists before deciding that the filename is the
     * correct one. For example, if the user's copy of a file is
     * corrupt, it should move onto the system one. */

    /* First look in the players home directory. */
    filename = g_build_filename (localthemedir, theme, NULL);
    if (!g_file_test (filename, G_FILE_TEST_EXISTS)) {
      g_free (filename);
      /* Then look in the system directory. */
      filename = g_build_filename (games_runtime_get_directory (GAMES_RUNTIME_GAME_THEME_DIRECTORY), THEME_VERSION, theme, NULL);
      if (!g_file_test (filename, G_FILE_TEST_EXISTS)) {
        /* Some old themes had a -sync just before the suffix. This was
         * hidden from the user and has been eliminated from the names
         * of reworked themes. Check in case we had one of this form. */
        length = strlen (filename);
        substring = g_strstr_len (filename, length, "-sync");
        suffix = g_strrstr (filename, ".png");
        if (substring && suffix) {
          g_stpcpy (substring, suffix);
        }
        if (!g_file_test (filename, G_FILE_TEST_EXISTS)) {
          /* We have changed some themes from .png to .svg, try that. */
          suffix = g_strrstr (filename, ".png");
          if (suffix) {
            g_stpcpy (suffix, ".svg");
          }
          if (!g_file_test (filename, G_FILE_TEST_EXISTS)) {
            /* And finally fall back to the default. */
            g_free (filename);
            filename = g_build_filename (games_runtime_get_directory (GAMES_RUNTIME_GAME_THEME_DIRECTORY), THEME_VERSION, DEFAULT_THEME, NULL);
          }
        }
      }
    }

    file_pixbuf = gdk_pixbuf_new_from_file_at_size (filename,
                                                    ftile_size * NFRAMESSPIN,
                                                    ftile_size * MAX_COLOURS,
                                                    NULL);
    g_free (filename);

    idle_state = DRAW;

    /* Do something halfway sensible in the absense of any data. */
    if (file_pixbuf == NULL) {
      /* FIXME: We haven't got the parent window right. */
      dialog = gtk_message_dialog_new (NULL,
                                       GTK_DIALOG_MODAL |
                                       GTK_DIALOG_DESTROY_WITH_PARENT,
                                       GTK_MESSAGE_ERROR,
                                       GTK_BUTTONS_NONE,
                                       _("No theme data was found."));
      gtk_dialog_add_button (GTK_DIALOG (dialog), GTK_STOCK_QUIT,
                             GTK_RESPONSE_OK);
      gtk_message_dialog_format_secondary_text (GTK_MESSAGE_DIALOG (dialog),
                                                _
                                                ("It is impossible to play the game. Please check that the game has been installed correctly and try again."));
      gtk_dialog_run (GTK_DIALOG (dialog));
      gtk_main_quit ();
      return FALSE;
    }
  } else if (idle_state == DRAW) {
    if (last_tile_size != tile_size) {
      idle_state = INIT;
    } else {
      if (pixmaps[n][m] != NULL)
        g_object_unref (pixmaps[n][m]);

      tile = gdk_pixbuf_copy (bg_pixbuf);
      gdk_pixbuf_composite (file_pixbuf, tile, 1, 1, ftile_size,
                            ftile_size, 1.0 - m * ftile_size,
                            1.0 - n * ftile_size,
                            1.0, 1.0, GDK_INTERP_TILES, 255);

      pixmaps[n][m] = gdk_pixmap_new (canvas->window, tile_size, tile_size,
                                      -1);
      gdk_draw_pixbuf (pixmaps[n][m], canvas->style->black_gc, tile,
                       0, 0, 0, 0, tile_size, tile_size,
                       GDK_RGB_DITHER_NORMAL, 0, 0);

      g_object_unref (tile);

      m++;
      if (m == NFRAMESSPIN) {
        m = 0;
        n++;
        if (n == MAX_COLOURS) {
          idle_state = DEST;
          n = 0;
          m = DESTFRAMESOFS;
        }
      }
    }
  } else if (idle_state == DEST) {
    if (last_tile_size != tile_size) {
      idle_state = INIT;
    } else {
      if (pixmaps[n][m] != NULL)
        g_object_unref (pixmaps[n][m]);

      tile = gdk_pixbuf_copy (bg_pixbuf);

      /* FIXME: Split out ? */
      /* FIXME: Should be some sort of spinny stuff. */
      /* Instead we have a fade-out. */
      /* This method does avoid the "phase" problem of destroying
       * a spinning object at an arbitrary phase. */
      /* FIXME: This fade from white scheme only works for dark backgrounds. */
      bp = gdk_pixbuf_get_pixels (tile);
      l = gdk_pixbuf_get_rowstride (tile);
      /* Note that this avoids the border. */
      shift = (m - DESTFRAMESOFS) / 2;
      tr = br + ((0xff - br) >> shift);
      tg = bg + ((0xff - bg) >> shift);
      tb = bb + ((0xff - bb) >> shift);
      for (j = 1; j < tile_size; j++) {
        p = bp + j * l + 3;
        for (i = 1; i < tile_size; i++) {
          *p++ = tr;
          *p++ = tg;
          *p++ = tb;
        }
      }

      pixmaps[n][m] = gdk_pixmap_new (canvas->window, tile_size, tile_size,
                                      -1);
      gdk_draw_pixbuf (pixmaps[n][m], canvas->style->black_gc, tile,
                       0, 0, 0, 0, tile_size, tile_size,
                       GDK_RGB_DITHER_NORMAL, 0, 0);

      g_object_unref (tile);

      m++;
      if (m == MOVEDFRAMESOFS) {
        m = DESTFRAMESOFS;
        n++;
        if (n == MAX_COLOURS) {
          idle_state = MOVED;
          n = 0;
          m = MOVEDFRAMESOFS;
        }
      }
    }
  } else if (idle_state == MOVED) {
    if (last_tile_size != tile_size) {
      idle_state = INIT;
    } else {
      if (pixmaps[n][m] != NULL)
        g_object_unref (pixmaps[n][m]);

      x = m - MOVEDFRAMESOFS;
      tile = gdk_pixbuf_copy (bg_pixbuf);

      /* FIXME: Split out ? */
      /* No phase problems here. The falling stuff always has a phase of 0 */
      bp = gdk_pixbuf_get_pixels (tile);
      l = gdk_pixbuf_get_rowstride (tile);
      p = bp + 3;
      /* Remove the horizontal line... */
      for (i = 1; i < tile_size; i++) {
        *p++ = br;
        *p++ = bg;
        *p++ = bb;
      }
      j = tile_size - 1 - ((x + 1) * (tile_size - 1)) / (NFRAMESMOVED);
      downoffsets[x] = j;
      p = bp + j * l + 3;
      /* ...and redraw it in the correct place. */
      for (i = 1; i < tile_size; i++) {
        *p++ = r;
        *p++ = g;
        *p++ = b;
      }

      gdk_pixbuf_composite (file_pixbuf, tile, 1, 1, ftile_size,
                            ftile_size, 1.0, 1.0 - n * ftile_size,
                            1.0, 1.0, GDK_INTERP_TILES, 255);

      pixmaps[n][m] = gdk_pixmap_new (canvas->window, tile_size, tile_size,
                                      -1);
      gdk_draw_pixbuf (pixmaps[n][m], canvas->style->black_gc, tile,
                       0, 0, 0, 0, tile_size, tile_size,
                       GDK_RGB_DITHER_NORMAL, 0, 0);

      g_object_unref (tile);

      m++;
      if (m == MOVELFRAMESOFS) {
        m = MOVEDFRAMESOFS;
        n++;
        if (n == MAX_COLOURS) {
          idle_state = MOVEL;
          n = 0;
          m = MOVELFRAMESOFS;
        }
      }
    }
  } else if (idle_state == MOVEL) {
    if (last_tile_size != tile_size) {
      idle_state = INIT;
    } else {
      if (pixmaps[n][m] != NULL)
        g_object_unref (pixmaps[n][m]);

      x = m - MOVELFRAMESOFS;
      tile = gdk_pixbuf_copy (bg_pixbuf);

      /* FIXME: Split out ? */
      /* No phase problems here. The falling stuff always has a phase of 0 */
      bp = gdk_pixbuf_get_pixels (tile);
      l = gdk_pixbuf_get_rowstride (tile);
      p = bp + l;
      /* Remove the vertical line... */
      for (i = 1; i < tile_size; i++) {
        *p++ = br;
        *p++ = bg;
        *p = bb;
        p += l - 2;
      }
      j = ((x + 1) * (tile_size - 1)) / (NFRAMESMOVED);
      leftoffsets[x] = tile_size - j;
      p = bp + l + 3 * j;
      /* ...and redraw it in the correct place. */
      for (i = 1; i < tile_size; i++) {
        *p++ = r;
        *p++ = g;
        *p = b;
        p += l - 2;
      }

      gdk_pixbuf_composite (file_pixbuf, tile, 1, 1, ftile_size,
                            ftile_size, 1.0, 1.0 - n * ftile_size,
                            1.0, 1.0, GDK_INTERP_TILES, 255);

      pixmaps[n][m] = gdk_pixmap_new (canvas->window, tile_size, tile_size,
                                      -1);
      gdk_draw_pixbuf (pixmaps[n][m], canvas->style->black_gc, tile,
                       0, 0, 0, 0, tile_size, tile_size,
                       GDK_RGB_DITHER_NORMAL, 0, 0);

      g_object_unref (tile);

      m++;
      if (m == NFRAMES) {
        m = MOVELFRAMESOFS;
        n++;
        if (n == MAX_COLOURS) {
          idle_state = DONE;
          pixmaps_ready = TRUE;
        }
      }
    }
  } else if (idle_state == DONE) {
    g_object_unref (file_pixbuf);
    file_pixbuf = NULL;
    g_object_unref (bg_pixbuf);
    bg_pixbuf = NULL;
    idle_state = INIT;
    idle_id = 0;
    redraw ();

    if (pixmaps_ready)                /* Just in case this was reset after the last idle call. */
      return FALSE;
  }
  return TRUE;
}

static void
start_renderer (void)
{
  pixmaps_ready = FALSE;

  if (idle_id == 0)
    idle_id = g_idle_add ((GSourceFunc) render_cb, canvaswidget);

}

void
change_theme (gchar * newtheme)
{
  if (g_utf8_collate (theme, newtheme) != 0) {

    g_free (theme);
    theme = g_strdup (newtheme);

    start_renderer ();
    redraw ();

    games_conf_set_string (NULL, KEY_THEME, theme);
  }
}

static gboolean
animation_timer (void)
{
  static float speed = 1.0;
  int i, j;
  game_cell *p;
  gboolean changestatep;
  gboolean animating = FALSE;

  if (!pixmaps_ready)
    return TRUE;

  changestatep = FALSE;

  for (j = 0; j < board_height; j++) {
    /* FIXME: Could we rearrange the board structure so it
     * runs from right to left? Making this simpler. */
    p = board + (j + 1) * board_width - 1;
    for (i = board_width - 1; i >= 0; i--) {
      switch (p->style) {
      case ANI_STILL:
        if (p->frame != 0)
          p->style = ANI_SPINBACK;
        break;
      case ANI_REDRAW:
        draw_ball (canvaswidget, p, i, j);
        p->style = ANI_STILL;
        break;
      case ANI_SPIN:
        p->frame = (p->frame + 1) % NFRAMESSPIN;
        draw_ball (canvaswidget, p, i, j);
        break;
      case ANI_SPINBACK:
        if (p->frame > 4)
          p->frame = 4;
        if (p->frame == 0)
          p->style = ANI_STILL;
        else
          p->frame--;
        draw_ball (canvaswidget, p, i, j);
        break;
      case ANI_DESTROY:
        if (fast_animation)
           p->frame = MOVEDFRAMESOFS;
        else
           p->frame++;
        if (p->frame < DESTFRAMESOFS) {
          p->frame = DESTFRAMESOFS;
        } else if (p->frame < MOVEDFRAMESOFS) {
              
        } else {
          p->colour = NONE;
          changestatep = TRUE;
          p->frame = 0;
          p->style = ANI_STILL;
        }
        draw_ball (canvaswidget, p, i, j);
        break;
      case ANI_MOVE_DOWN:
        if (game_state == GAME_MOVING_DOWN) {
          if (fast_animation)
            /* This allows a fast, but not instant, animation. */
            p->frame = MOVELFRAMESOFS;
          else
            p->frame += speed;
          if (p->frame >= MOVELFRAMESOFS) {
            changestatep = TRUE;
            p->frame = 0;
            p->style = ANI_STILL;
            draw_ball (canvaswidget, p, i, j);
          } else {
            draw_ball_with_offset (canvaswidget, p, i, j, 0,
                                   downoffsets[p->frame - MOVEDFRAMESOFS]);
          }
        }
        break;
      case ANI_MOVE_LEFT:
        if (game_state == GAME_MOVING_LEFT) {
          if (fast_animation)
            /* This allows a fast, but not instant, animation. */
            p->frame = NFRAMES;
          else
            p->frame += speed;
            if (p->frame >= NFRAMES) {
              changestatep = TRUE;
              p->frame = 0;
              p->style = ANI_STILL;
              draw_ball (canvaswidget, p, i, j);
            } else {
              draw_ball_with_offset (canvaswidget, p, i, j,
                                     leftoffsets[p->frame - MOVELFRAMESOFS], 0);
            }
        }
        break;
      }

      if(p->style != ANI_STILL)
        animating = TRUE;

      p--;
    }
  }

  /* Since this value is added on each cycle the effect is that
   * the balls drop quadratically. */
  speed += ACCELERATION;

  /* Various things in the previous loop can trigger a change in
   * game_state. However we only want to do these things once, so
   * we flag them and check them down here once the loop has finished. */
  if (changestatep) {
    animating = TRUE;
    /* FIXME: Is this really the place to change the game state ?
     * We do need to signal the end of the animation though so maybe ...
     * or maybe we move it over to game.c 
     * This question should be rephrased: is this really the file for
     * this function ? */
    switch (game_state) {
    case GAME_DESTROYING:
      if (mark_falling_balls () == 0)
        if (mark_shifting_balls ()) {
          game_state = GAME_MOVING_LEFT;
          speed = 1.0;
        } else {
          end_of_move ();
      } else {
        game_state = GAME_MOVING_DOWN;
        speed = 1.0;
      }
      break;
      /* FIXME: Because of the way we do the drawing (with the offset)
       * there is potential for overlap with a blank tile and hence 
       * flicker. */
    case GAME_MOVING_DOWN:
      if (mark_falling_balls () == 0) {
        if (mark_shifting_balls ()) {
          game_state = GAME_MOVING_LEFT;
        } else {
          end_of_move ();
          select_cells ();
        }
        speed = 1.0;
      } else {

      }
      break;
    case GAME_MOVING_LEFT:
      if (!mark_shifting_balls ()) {
        end_of_move ();
        select_cells ();
      }
      break;
    }
  }

  if (animating) {
    return TRUE;
  } else {
    animation_timer_id = 0;
    return FALSE;
  }
}

void
start_animation (void)
{
  /* 16 frames/second. */
  if (animation_timer_id == 0)
    animation_timer_id = g_timeout_add (62, (GSourceFunc) animation_timer, NULL);    
}

void
resize_graphics (void)
{
  int size, n, m;
  static gboolean first_run = TRUE;

  if (first_run) {
    for (n = 0; n < MAX_COLOURS; n++)
      for (m = 0; m < NFRAMES; m++)
        pixmaps[n][m] = NULL;

    first_run = FALSE;
  }

  size = drawing_area_width / board_width;

  if (tile_size != size) {
    tile_size = size;

    start_renderer ();
  }
}

gboolean
configure_cb (GtkWidget * canvas, GdkEventConfigure * event)
{
  canvaswidget = canvas;
  drawing_area_width = event->width;
  resize_graphics ();

  return FALSE;
}

void
start_spinning (void)
{
  int i;
  game_cell *p;
  coordinates *list;

  set_message (count);

  /* FIXME: Is this bit of policy distributed too far ? */
  if (count <= 1)
    return;

  list = selected;
  for (i = 0; i < count; i++) {
    p = get_game_cell (list->x, list->y);
    p->style = ANI_SPIN;
    p->frame = 0;
    list++;
  }
    
  start_animation();
}

void
stop_spinning (void)
{
  int i;
  game_cell *p;
  coordinates *list;

  list = selected;
  for (i = 0; i < count; i++) {
    p = get_game_cell (list->x, list->y);
    p->style = ANI_SPINBACK;
    list++;
  }
    
  start_animation();
}
