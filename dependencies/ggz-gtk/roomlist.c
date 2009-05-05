/*
 * File: roomlist.h
 * Author: GGZ Dev Team
 * Project: GGZ GTK Client
 * Date: 11/05/2004
 * $Id$
 * 
 * List of rooms in the server
 * 
 * Copyright (C) 2000-2004 GGZ Development Team
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
#include <string.h>

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <ggz.h>
#include <ggzcore.h>

#include "client.h"
#include "game.h"
#include "msgbox.h"
#include "roominfo.h"
#include "roomlist.h"
#include "server.h"
#include "support.h"

enum {
	ROOM_COLUMN_PTR,
	ROOM_COLUMN_NAME,
	ROOM_COLUMN_PLAYERS,
	ROOM_COLUMNS
};

static GtkTreeIter *room_iter, other_iter;
static GtkWidget *room_list;

static void client_join_room(GGZRoom *room)
{
	gchar *err_msg = NULL;
	gint singleclick, status = -1;

	if(ggzcore_server_get_cur_room(server) == room) {
		/* ignore silently that we're already in this room */
		return;
	}

	switch (ggzcore_server_get_state(server)) {
	case GGZ_STATE_OFFLINE:
	case GGZ_STATE_CONNECTING:
	case GGZ_STATE_ONLINE:
	case GGZ_STATE_LOGGING_IN:
	case GGZ_STATE_LOGGING_OUT:
		err_msg = _("You can't join a room; you're not logged in");
		break;
	case GGZ_STATE_BETWEEN_ROOMS:
	case GGZ_STATE_ENTERING_ROOM:
		err_msg = _("You're already in between rooms");
		break;
	case GGZ_STATE_JOINING_TABLE:
	case GGZ_STATE_AT_TABLE:
	case GGZ_STATE_LEAVING_TABLE:
		err_msg = _("You can't switch rooms while playing a game");
		break;
	case GGZ_STATE_IN_ROOM:
	case GGZ_STATE_LOGGED_IN:
		status = 0;
		break;
	default:
		err_msg = _("Unknown error");
		status = -1;
	}

	if (status == 0) {
		if (ggzcore_server_join_room(server, room) == 0) {
	
			/* Only desensitize with single click, dues to
	                some weird bug that freezes the mouse if we
        	        doubleclick and then desensitize */
			singleclick = ggzcore_conf_read_int("OPTIONS",
							    "ROOMENTRY",
							    FALSE);
			if (singleclick) {
				/* Set roomlist insensitive */
				sensitize_room_list(FALSE);
			}
			return;
		}
		else {
			err_msg = _("Error joining room");
		}
	}

	/* If we get here, there was an error */
	msgbox(err_msg, _("Error joining room"), MSGBOX_OKONLY, MSGBOX_STOP, 
	       MSGBOX_NORMAL);
}

static void client_room_join_activate(GtkMenuItem *menuitem, gpointer data)
{
	GGZRoom *room = data;

	client_join_room(room);
}

static void client_room_info_activate(GtkMenuItem *menuitem, gpointer data)
{
	/* Display room's info in a nice dialog */
	GGZRoom *room = data;

	room_info_create_or_raise(room);
}

static GtkWidget *create_mnu_room(GGZRoom *room)
{
	GtkWidget *mnu_room;
	GtkWidget *info;
	GtkWidget *join;
	GtkAccelGroup *accel_group;

	accel_group = gtk_accel_group_new ();

	mnu_room = gtk_menu_new ();

	info = gtk_menu_item_new_with_label (_("Info"));
	gtk_container_add (GTK_CONTAINER (mnu_room), info);
	gtk_widget_add_accelerator (info, "activate", accel_group,
				    GDK_I, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);

	join = gtk_menu_item_new_with_label (_("Join"));
	gtk_container_add (GTK_CONTAINER (mnu_room), join);
	gtk_widget_add_accelerator (join, "activate", accel_group,
				    GDK_J, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);

	g_signal_connect (GTK_OBJECT (info), "activate",
			  GTK_SIGNAL_FUNC (client_room_info_activate),
			  room);
	g_signal_connect (GTK_OBJECT (join), "activate",
			  GTK_SIGNAL_FUNC (client_room_join_activate),
			  room);

	gtk_menu_set_accel_group (GTK_MENU (mnu_room), accel_group);

	return mnu_room;
}

static gboolean room_list_event(GtkWidget *widget, GdkEvent *event,
				gpointer data)
{
	GdkEventButton *buttonevent = (GdkEventButton *) event;
	GtkTreeView *tree = GTK_TREE_VIEW(widget);
	GtkTreeModel *model = gtk_tree_view_get_model(tree);
	gboolean single_join;
	GtkWidget *menu;
	GtkTreeIter iter;
	GGZRoom *room;
	GtkTreePath *path = NULL;

	if (!gtk_tree_view_get_path_at_pos(tree,
					   buttonevent->x, buttonevent->y,
					   &path, NULL, NULL, NULL)) {
		return FALSE;
	}
	gtk_tree_model_get_iter(model, &iter, path);

	single_join = ggzcore_conf_read_int("OPTIONS", "ROOMENTRY", FALSE);

	gtk_tree_model_get(model, &iter, ROOM_COLUMN_PTR, &room, -1);

	if (!room) return FALSE;

	switch(event->type) { 
	case GDK_2BUTTON_PRESS:
		/* Double click */
		if (buttonevent->button == 1 && !single_join) {
			client_join_room(room);
			return TRUE;
		}
		break;
	case GDK_BUTTON_PRESS:
		/* Single click */
		if (buttonevent->button == 1 && single_join) {
			client_join_room(room);
			return TRUE;
		} else if (buttonevent->button == 3) {
			/* Right mouse button */
			/* Create and display the menu */
			menu = create_mnu_room(room);
			gtk_widget_show_all(menu);
			gtk_menu_popup(GTK_MENU(menu), NULL, NULL, NULL,
				       NULL, buttonevent->button, 0);
		}
		break;
	default:
		/* Some Other event */
		break;
	}

	return FALSE;
}

void sensitize_room_list(gboolean sensitive)
{
	GtkWidget *tree = room_list;

	gtk_widget_set_sensitive(tree, sensitive);
}

void clear_room_list(void)
{
	/* Clear current list of rooms */
	GtkTreeStore *store;

	store = GTK_TREE_STORE(ggz_lookup_widget(room_list, "room_list_store"));
	gtk_tree_store_clear(store);
}

void select_room(GGZRoom *room)
{
#if 0 /* FIXME */
	GtkTreeView *tree;
	GtkTreeStore *store;
	GtkTreeSelection *select;
	GtkTreeIter iter;
	int count = ggzcore_server_get_num_rooms(server), i;

	tree = GTK_TREE_VIEW(room_list);
	store = GTK_TREE_STORE(gtk_tree_view_get_model(tree));
	select = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree));

	if (!gtk_tree_model_get_iter_first(GTK_TREE_MODEL(store), &iter)) {
		return;
	}
	for (i = 0; i < id; i++) {
		if (!gtk_tree_model_iter_next(GTK_TREE_MODEL(store), &iter)) {
			return;
		}
	}

	gtk_tree_selection_select_iter(select, &iter);
#endif
}

static void update_iter_room(GtkTreeStore *store, GtkTreeIter *iter,
			     const GGZRoom *room)
{
	const char *roomname = ggzcore_room_get_name(room);
	char name[strlen(roomname) + 3];
	const int players = ggzcore_room_get_num_players(room);
	GGZGameType *gt = ggzcore_room_get_gametype(room);

	if (gt && !can_launch_gametype(gt)) {
		int old_players, other_players;

		gtk_tree_model_get(GTK_TREE_MODEL(store), iter,
				   ROOM_COLUMN_PLAYERS, &old_players, -1);
		gtk_tree_model_get(GTK_TREE_MODEL(store), &other_iter,
				   ROOM_COLUMN_PLAYERS, &other_players, -1);
		other_players += MAX(players, 0) - MAX(old_players, 0);
	  
		gtk_tree_store_set(store, &other_iter,
				   ROOM_COLUMN_PLAYERS, other_players, -1);
	}

	if (ggzcore_room_get_closed(room)) {
		snprintf(name, sizeof(name), "(%s)", roomname);
	} else {
		snprintf(name, sizeof(name), "%s", roomname);
	}

	gtk_tree_store_set(store, iter,
			   ROOM_COLUMN_PTR, room,
			   ROOM_COLUMN_NAME, name, -1);


	if (players >= 0) {
		gtk_tree_store_set(store, iter,
				   ROOM_COLUMN_PLAYERS, players, -1);
	}
}

#if 0
/* Updates rooms in the treestore. The user data can hold the room pointer
   for the room to be updated, or may be NULL for all rooms to be updated. */
static gboolean tree_model_update_room(GtkTreeModel *store,
				       GtkTreePath *path,
				       GtkTreeIter *iter,
				       gpointer data)
{
	GGZRoom *room = data, *thisroom;

	gtk_tree_model_get(GTK_TREE_MODEL(store), iter,
			   ROOM_COLUMN_PTR, &thisroom, -1);

	if (!thisroom || (room && room != thisroom)) {
		return FALSE;
	}

	update_iter_room(GTK_TREE_STORE(store), iter, room);
	return (room != NULL);
}
#endif

void update_one_room(const GGZRoom *room)
{
	GtkTreeStore *store;
	int num = ggzcore_server_get_room_num(server, room);

	/* Retrieve the player list widget. */
	store = GTK_TREE_STORE(ggz_lookup_widget(room_list, "room_list_store"));

	update_iter_room(store, &room_iter[num], room);
}

void update_room_list(void)
{
	GtkTreeStore *store;
	int i;
	const int numrooms = ggzcore_server_get_num_rooms(server);

	/* Retrieve the player list widget. */
	store = GTK_TREE_STORE(ggz_lookup_widget(room_list, "room_list_store"));

	clear_room_list();

	gtk_tree_store_append(store, &other_iter, NULL);
	gtk_tree_store_set(store, &other_iter,
			   ROOM_COLUMN_PTR, NULL,
			   ROOM_COLUMN_NAME, _("Other Rooms"),
			   ROOM_COLUMN_PLAYERS, 0, -1);

	room_iter = ggz_realloc(room_iter, numrooms * sizeof(*room_iter));
	for (i = 0; i < numrooms; i++) {
		GGZRoom *room = ggzcore_server_get_nth_room(server, i);
		GGZGameType *gt = ggzcore_room_get_gametype(room);
		GtkTreeIter *iter = &room_iter[i];

		if (gt && !can_launch_gametype(gt)) {
			gtk_tree_store_append(store, iter, &other_iter);
		} else {
			gtk_tree_store_insert_before(store, iter,
						     NULL, &other_iter);
		}

		update_iter_room(store, iter, room);
	}
}

static gint room_sort_func(GtkTreeModel *model,
			   GtkTreeIter *a, GtkTreeIter *b, gpointer data)
{
	GGZRoom *roomA, *roomB;
	GGZGameType *gtA, *gtB;

	gtk_tree_model_get(model, a, ROOM_COLUMN_PTR, &roomA, -1);
	gtk_tree_model_get(model, b, ROOM_COLUMN_PTR, &roomB, -1);

	if (!roomA || !roomB) {
	  return (roomA ? -1 : 0) - (roomB ? -1 : 0);
	}

	gtA = ggzcore_room_get_gametype(roomA);
	gtB = ggzcore_room_get_gametype(roomB);
	if (!gtA || !gtB) {
	  return (gtA ? 1 : 0) - (gtB ? 1 : 0);
	}

	return strcasecmp(ggzcore_room_get_name(roomA),
			  ggzcore_room_get_name(roomB));
}

GtkWidget *create_room_list(GtkWidget *window)
{
	GtkTreeStore *store;
	GtkWidget *tree;
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	GtkTreeSelection *select;


	assert(ROOM_COLUMNS == 3);
	store = gtk_tree_store_new(ROOM_COLUMNS,
				   G_TYPE_POINTER,
				   G_TYPE_STRING,
				   G_TYPE_INT);
	tree = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
	g_object_unref(store);

	gtk_tree_sortable_set_sort_column_id(GTK_TREE_SORTABLE(store),
					     ROOM_COLUMN_NAME,
					     GTK_SORT_ASCENDING);
	gtk_tree_sortable_set_sort_func(GTK_TREE_SORTABLE(store),
					ROOM_COLUMN_NAME,
					room_sort_func,
					NULL, NULL);

	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("#", renderer,
				"text", ROOM_COLUMN_PLAYERS, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);

	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes(_("Room"),
							  renderer,
				"text", ROOM_COLUMN_NAME, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);

	g_object_set_data(G_OBJECT(tree), "room_list_store", store);
	g_object_set_data(G_OBJECT(window), "room_list_store", store);
	gtk_widget_set_sensitive(tree, FALSE);
	GTK_WIDGET_UNSET_FLAGS(tree, GTK_CAN_FOCUS);

	select = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree));
	gtk_tree_selection_set_mode(select, GTK_SELECTION_SINGLE);

	g_signal_connect(tree, "button-press-event",
			 GTK_SIGNAL_FUNC(room_list_event), NULL);

	room_list = tree;
	return tree;
}
