// -*- mode:C++; tab-width:2; c-basic-offset:2; indent-tabs-mode:nil -*-
/* Blackjack - events.h
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

#ifndef BJ_EVENTS_H
#define BJ_EVENTS_H

#include <gtk/gtk.h>

gboolean bj_event_key_press (GtkWidget *, GdkEventKey *, void *);
gint bj_event_button_press (GtkWidget *, GdkEventButton *, void *);
gint bj_event_button_release (GtkWidget *, GdkEventButton *, void *);
gint bj_event_playing_area_configure (GtkWidget *, GdkEventConfigure *);
gint bj_event_enter_notify (GtkWidget *, GdkEventCrossing *, void *);
gint bj_event_motion_notify (GtkWidget *, GdkEventMotion *);
gint bj_event_expose_callback (GtkWidget *, GdkEventExpose *, void *);

#endif
