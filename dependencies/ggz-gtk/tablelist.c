/*
 * File: tablelist.c
 * Author: GGZ Dev Team
 * Project: GGZ GTK Client
 * Date: 11/03/2002
 * $Id$
 * 
 * List of tables in the current room
 * 
 * Copyright (C) 2000-2002 Justin Zaun.
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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include <gtk/gtk.h>
#include <ggzcore.h>

#include "client.h"
#include "tablelist.h"
#include "server.h"
#include "support.h"

enum {
	TABLE_COLUMN_ID,
	TABLE_COLUMN_SEATS,
	TABLE_COLUMN_DESC,
	TABLE_COLUMNS
};

static GtkWidget *table_list;

static gboolean table_list_event(GtkWidget *widget, GdkEvent *event,
				 gpointer data)
{
	/* Check to see if the event was a mouse button press */
	if (event->type == GDK_2BUTTON_PRESS)
		client_start_table_join();
	return FALSE;
}


#if 0
static GtkWidget *create_mnu_table(void)
{
	GtkWidget *mnu_table;
	GtkWidget *join;
	GtkWidget *leave;
	GtkWidget *menuitem3;
	GtkWidget *info;

	mnu_table = gtk_menu_new();

	join = gtk_menu_item_new_with_label(_("Join"));
	gtk_container_add(GTK_CONTAINER(mnu_table), join);

	leave = gtk_menu_item_new_with_label(_("Leave"));
	gtk_container_add(GTK_CONTAINER(mnu_table), leave);

	menuitem3 = gtk_menu_item_new();
	gtk_container_add(GTK_CONTAINER(mnu_table), menuitem3);
	gtk_widget_set_sensitive(menuitem3, FALSE);

	info = gtk_menu_item_new_with_label(_("Info"));
	gtk_container_add(GTK_CONTAINER(mnu_table), info);

#if 0				/* not implemented */
	g_signal_connect(GTK_OBJECT(join), "activate",
			   GTK_SIGNAL_FUNC(client_join_table_activate),
			   GINT_TO_POINTER(table_num));
	g_signal_connect(GTK_OBJECT(leave), "activate",
			   GTK_SIGNAL_FUNC(client_leave_activate), NULL);
	g_signal_connect(GTK_OBJECT(info), "activate",
			   GTK_SIGNAL_FUNC(client_table_info_activate),
			   GINT_TO_POINTER(table_num));
#endif

	return mnu_table;
}
#endif

GGZTable *get_selected_table(void)
{
	GtkWidget *tree = table_list;
	GtkTreeSelection *select
	  = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree));
	GGZRoom *room = ggzcore_server_get_cur_room(server);
	GtkTreeModel *model;
	GtkTreeIter iter;
	gint id;

	if (!gtk_tree_selection_get_selected(select, &model, &iter)) {
		return NULL;
	}

	gtk_tree_model_get(model, &iter, TABLE_COLUMN_ID, &id, -1);

	return ggzcore_room_get_table_by_id(room, id);
}


void clear_table_list(void)
{
	GtkWidget *store = ggz_lookup_widget(table_list, "table_list_store");

	gtk_list_store_clear(GTK_LIST_STORE(store));
}

void sensitize_table_list(gboolean sensitive)
{
	gtk_widget_set_sensitive(table_list, sensitive);
}

void update_table_list(void)
{
	GtkListStore *store;
	GGZRoom *room = ggzcore_server_get_cur_room(server);
	int i;
	const int num = ggzcore_room_get_num_tables(room);

	/* Retrieve the player list widget. */
	store = GTK_LIST_STORE(ggz_lookup_widget(table_list, "table_list_store"));

	gtk_list_store_clear(store);

	for (i = 0; i < num; i++) {
		GtkTreeIter iter;
		gchar seats[128];
		int avail, total;
		const gchar *desc;
		GGZTable *t = ggzcore_room_get_nth_table(room, i);
		int id = ggzcore_table_get_id(t);

		avail = (ggzcore_table_get_seat_count(t, GGZ_SEAT_OPEN)
			 + ggzcore_table_get_seat_count(t, GGZ_SEAT_RESERVED));
		total = ggzcore_table_get_num_seats(t);
		snprintf(seats, sizeof(seats), "%d/%d", avail, total);

		desc = ggzcore_table_get_desc(t);
		if (!desc) {
			desc = _("No description available.");
		}

		gtk_list_store_append(store, &iter);
		gtk_list_store_set(store, &iter,
				   TABLE_COLUMN_ID, id,
				   TABLE_COLUMN_SEATS, seats,
				   TABLE_COLUMN_DESC, desc,
				   -1);

	}
}

GtkWidget *create_table_list(GtkWidget * window)
{
	GtkListStore *store;
	GtkWidget *tree;
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	GtkTreeSelection *select;

	assert(TABLE_COLUMNS == 3);
	store = gtk_list_store_new(TABLE_COLUMNS,
				   G_TYPE_INT,
				   G_TYPE_STRING,
				   G_TYPE_STRING);
	tree = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
	g_object_unref(store);

	renderer = gtk_cell_renderer_text_new();
        /* Translators: T# is short for 'Table Number'.  */
	column = gtk_tree_view_column_new_with_attributes(_("T#"), renderer,
				"text", TABLE_COLUMN_ID, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);

	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes(_("Seats"), renderer,
				"text", TABLE_COLUMN_SEATS, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);

	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes(_("Description"),
							  renderer,
				"text", TABLE_COLUMN_DESC, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);

	g_object_set_data(G_OBJECT(window), "table_list_store", store);
	g_object_set_data(G_OBJECT(tree), "table_list_store", store);
	gtk_widget_set_sensitive(tree, FALSE);
	GTK_WIDGET_UNSET_FLAGS(tree, GTK_CAN_FOCUS);

	select = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree));
	gtk_tree_selection_set_mode(select, GTK_SELECTION_SINGLE);

	g_signal_connect(tree, "button-press-event",
			 GTK_SIGNAL_FUNC(table_list_event), NULL);

	table_list = tree;

	return tree;
}
