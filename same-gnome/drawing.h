/* -*- mode: C; indent-tabs-mode: t; tab-width: 2; c-basic-offset: 2; -*- */

/* drawing.h: Code for drawing the game (well, duh).
 *
 * Copyright (c) 2004 by Callum McKenzie
 *
 */

#include <gtk/gtk.h>

#ifndef DRAWING_H
#define DRAWING_H

gboolean expose_cb (GtkWidget * canvas, GdkEventExpose * event,
		    gpointer data);
gboolean configure_cb (GtkWidget * canvas, GdkEventConfigure * event);

void redraw (void);

void cursor_draw (gint x, gint y);
void cursor_erase (void);

void pixels_to_logical (gint px, gint py, gint * lx, gint * ly);

void start_animation (void);

void start_spinning (void);
void stop_spinning (void);

void change_theme (gchar * newtheme);
void resize_graphics (void);

void fast_animation_enable (gboolean);

#endif /* DRAWING_H */
