/* -*- mode: C; indent-tabs-mode: t; tab-width: 2; c-basic-offset: 2; -*- */

/* input.h: Handles mouse and keyboard events on the main canvas.
 *
 * Copyright (c) 2004 by Callum McKenzie
 *
 */

#include <config.h>

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include "same-gnome.h"

#include "drawing.h"
#include "game.h"
#include "input.h"
#include "ui.h"

gint kb_xloc = 0;
gint kb_yloc = 0;

gint pt_xloc = 0;
gint pt_yloc = 0;

gboolean kb_visible = FALSE;

static gboolean
in_selected_cells (gint x, gint y)
{
  gint i;

  for (i = 0; i < count; i++)
    if ((x == selected[i].x) && (y == selected[i].y))
      return TRUE;

  return FALSE;
}

void
select_cells (void)
{
  int x, y;

  if ((game_state != GAME_IDLE) && (game_state != GAME_SELECTED))
    return;

  if (kb_visible) {
    x = kb_xloc;
    y = kb_yloc;
  } else {
    x = pt_xloc;
    y = pt_yloc;
  }

  /* We must search the entire list to stop the animation glitches when 
   * we move from ball to ball in an existing set. We automatically search 
   * on GAME_IDLE to catch some cases where the keyboard and mouse controls
   * can interact badly. */
  if (!in_selected_cells (x, y) || (game_state == GAME_IDLE)) {
    if (game_state == GAME_SELECTED)
      stop_spinning ();
    find_connected_component (x, y);
    start_spinning ();
    game_state = GAME_SELECTED;
  }
}

gboolean
mouse_movement_cb (GtkWidget * widget, GdkEventMotion * e, gpointer data)
{
  gint x, y;

  pixels_to_logical (e->x, e->y, &x, &y);
  pt_xloc = x;
  pt_yloc = y;

  select_cells ();

  return FALSE;
}

gboolean
mouse_click_cb (GtkWidget * widget, GdkEventButton * e, gpointer data)
{
  if (e->type != GDK_BUTTON_PRESS)
    return FALSE;

  destroy_balls ();

  return FALSE;
}

gboolean
mouse_leave_cb (GtkWidget * widget, GdkEventCrossing * e, gpointer data)
{
  /* Because sloppy focus is evil and produces extra leave and enter
   * events. */
  if (e->mode != GDK_CROSSING_NORMAL)
    return FALSE;

  clear_message ();

  if (game_state == GAME_SELECTED) {
    stop_spinning ();
    game_state = GAME_IDLE;
  }

  return FALSE;
}

static void
keyboard_move (gint dx, gint dy)
{
  if (kb_visible)
    cursor_erase ();
  kb_visible = TRUE;
  kb_xloc += dx;
  kb_yloc += dy;
  kb_xloc = (kb_xloc + board_width) % board_width;
  kb_yloc = (kb_yloc + board_height) % board_height;
  cursor_draw (kb_xloc, kb_yloc);
  select_cells ();
}

gboolean
keyboard_cb (GtkWidget * widget, GdkEventKey * event, gpointer data)
{
  /* FIXME: Diagonal keys. */

  switch (event->keyval) {
  case GDK_Up:
  case GDK_KP_Up:
  case GDK_KP_8:
    keyboard_move (0, -1);
    break;
  case GDK_Down:
  case GDK_KP_Down:
  case GDK_KP_2:
    keyboard_move (0, +1);
    break;
  case GDK_Right:
  case GDK_KP_Right:
  case GDK_KP_6:
    keyboard_move (+1, 0);
    break;
  case GDK_Left:
  case GDK_KP_Left:
  case GDK_KP_4:
    keyboard_move (-1, 0);
    break;
  case GDK_space:
  case GDK_Return:
  case GDK_KP_Enter:
  case GDK_KP_0:
  case GDK_KP_Insert:
    destroy_balls ();
    break;
  }

  return FALSE;
}
