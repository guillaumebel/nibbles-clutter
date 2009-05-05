/*
 * File: info.c
 * Author: Justin Zaun
 * Project: GGZ GTK Client
 * $Id$
 *
 * This dialog is used to display information about a selected room to
 * the user. 
 *
 * Copyright (C) 2000 Justin Zaun.
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

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>
#include <ggzcore.h>

#include "client.h"
#include "roominfo.h"
#include "support.h"


static GtkWidget *dialog;


static GtkWidget *create_dlg_info(GtkWidget *parent)
{
	GtkWidget *dlg_info;
	GtkWidget *dialog_vbox;
	GtkWidget *display_table;
	GtkWidget *game_pixmap;
	GtkWidget *name_label;
	GtkWidget *name;
	GtkWidget *author_label;
	GtkWidget *author;
	GtkWidget *www_label;
	GtkWidget *www;
	GtkWidget *desc_label;
	GtkWidget *desc;

	dlg_info = gtk_dialog_new_with_buttons(_("Room Information"),
					       GTK_WINDOW(parent), 0,
					       GTK_STOCK_CLOSE,
					       GTK_RESPONSE_CLOSE, NULL);

	dialog_vbox = GTK_DIALOG(dlg_info)->vbox;

	display_table = gtk_table_new(4, 3, FALSE);
	gtk_box_pack_start(GTK_BOX(dialog_vbox), display_table,
			   FALSE, TRUE, 0);

	game_pixmap = gtk_image_new_from_stock(GTK_STOCK_DIALOG_QUESTION,
					       GTK_ICON_SIZE_DIALOG);
	gtk_table_attach(GTK_TABLE(display_table), game_pixmap,
			 0, 1, 0, 4, 0, 0, 0, 0);

	/* Add 'name' label and text. */
	name_label = gtk_label_new(_("Game Name:"));
	gtk_table_attach(GTK_TABLE(display_table), name_label,
			 1, 2, 0, 1, 0, 0, 0, 0);

	name = gtk_label_new(NULL);
	g_object_set_data(G_OBJECT(dlg_info), "name", name);
	gtk_table_attach(GTK_TABLE(display_table), name,
			 2, 3, 0, 1, GTK_EXPAND, 0, 0, 0);

	/* Add 'author' label and text. */
	author_label = gtk_label_new(_("Author:"));
	gtk_table_attach(GTK_TABLE(display_table), author_label,
			 1, 2, 1, 2, 0, 0, 0, 0);

	author = gtk_label_new(NULL);
	g_object_set_data(G_OBJECT(dlg_info), "author", author);
	gtk_table_attach(GTK_TABLE(display_table), author,
			 2, 3, 1, 2, GTK_EXPAND, 0, 0, 0);

	/* Add 'homepage' label and text. */
	www_label = gtk_label_new(_("Homepage:"));
	gtk_table_attach(GTK_TABLE(display_table), www_label,
			 1, 2, 2, 3, 0, 0, 0, 0);

	www = gtk_label_new(NULL);
	g_object_set_data(G_OBJECT(dlg_info), "www", www);
	gtk_table_attach(GTK_TABLE(display_table), www,
			 2, 3, 2, 3, GTK_EXPAND, 0, 0, 0);

	/* Add 'description' label and text. */
	desc_label = gtk_label_new(_("Room Description:"));
	gtk_table_attach(GTK_TABLE(display_table), desc_label,
			 1, 2, 3, 4, 0, 0, 0, 0);

	desc = gtk_label_new(NULL);
	g_object_set_data(G_OBJECT(dlg_info), "desc", desc);
	gtk_table_attach(GTK_TABLE(display_table), desc,
			 2, 3, 3, 4, GTK_EXPAND, 0, 0, 0);

	g_signal_connect(dlg_info, "destroy",
			 GTK_SIGNAL_FUNC(gtk_widget_destroyed), &dialog);
	g_signal_connect(dlg_info, "response",
			 GTK_SIGNAL_FUNC(gtk_widget_destroy), NULL);

	return dlg_info;
}


/* info_create_or_raise() - Displays the dialog or updates current dialog
 *                          with new room's information
 *
 * Recieves:
 * gint		room	: Room number to display info about
 *
 * Returns:
 */

void room_info_create_or_raise(GGZRoom * room)
{
	GtkWidget *tmp;
	GGZGameType *gt = ggzcore_room_get_gametype(room);
	const char *text;

	if (!dialog) {
		dialog = create_dlg_info(main_window);
		gtk_widget_show_all(dialog);
	} else {
		gdk_window_show(dialog->window);
		gdk_window_raise(dialog->window);
	}

	tmp = ggz_lookup_widget(dialog, "name");
	if (gt)
		text = ggzcore_gametype_get_name(gt);
	else
		text = _("This room has no game");
	gtk_label_set_text(GTK_LABEL(tmp), text);

	tmp = ggz_lookup_widget(dialog, "author");
	if (gt)
		text = ggzcore_gametype_get_author(gt);
	else
		text = _("N/A");
	gtk_label_set_text(GTK_LABEL(tmp), text);

	tmp = ggz_lookup_widget(dialog, "www");
	if (gt)
		text = ggzcore_gametype_get_url(gt);
	else
		text = _("N/A");
	gtk_label_set_text(GTK_LABEL(tmp), text);

	tmp = ggz_lookup_widget(dialog, "desc");
	text = ggzcore_room_get_name(room);
	if (!text)
		text = _("Unknown room");
	gtk_label_set_text(GTK_LABEL(tmp), text);
}
