/* -*- mode: C; indent-tabs-mode: t; tab-width: 2; c-basic-offset: 2; -*- */

/* input.h: Handles mouse and keyboard events on the main canvas.
 *
 * Copyright (c) 2004 by Callum McKenzie
 *
 */

#ifndef INPUT_H
#define INPUT_H

#include <gtk/gtk.h>

gboolean mouse_movement_cb (GtkWidget * widget, GdkEventMotion * e,
			    gpointer data);
gboolean mouse_click_cb (GtkWidget * widget, GdkEventButton * e,
			 gpointer data);
gboolean mouse_enter_cb (GtkWidget * widget, GdkEventCrossing * e,
			 gpointer data);
gboolean mouse_leave_cb (GtkWidget * widget, GdkEventCrossing * e,
			 gpointer data);
gboolean keyboard_cb (GtkWidget * widget, GdkEventKey * event, gpointer data);

void select_cells (void);

#endif /* INPUT_H */
