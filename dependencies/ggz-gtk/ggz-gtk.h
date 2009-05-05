/*
 * File: ggz-gtk.h
 * Author: Jason Short
 * Project: GGZ GTK Client Library
 * $Id$
 *
 * This library provides interface functions so external programs can embed
 * GGZ via a GTK interface
 *
 * Copyright (C) 2000-2006 GGZ Development Team
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#include <gtk/gtk.h>

#include <ggzcore.h>

void ggz_gtk_initialize(gboolean reconnect,
			void (*connected)(GGZServer *server),
			void (*launched)(void),
			void (*ggz_closed)(void),
			const char *protocol_engine,
			const char *protocol_version,
			const char *game_help_file,
			const char *default_profile);

GtkWidget *ggz_gtk_create_main_area(GtkWidget *main_window);
