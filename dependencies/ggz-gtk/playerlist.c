/*
 * File: playerlist.c
 * Author: GGZ Dev Team
 * Project: GGZ GTK Client
 * Date: 11/03/2002
 * $Id$
 * 
 * List of players in the current room
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

#include <stdio.h>
#include <string.h>
#include <strings.h> /* For strcasecmp */

#include <gtk/gtk.h>
#include <ggzcore.h>

#include "chat.h"
#include "playerinfo.h"
#include "playerlist.h"
#include "server.h"
#include "support.h"

enum {
	PLAYER_COLUMN_TYPE,
	PLAYER_COLUMN_LAG,
	PLAYER_COLUMN_TABLE,
	PLAYER_COLUMN_STATS,
	PLAYER_COLUMN_NAME,
	PLAYER_COLUMNS
};

GtkWidget *player_list;

static void client_player_info_activate(GtkMenuItem * menuitem, gpointer data)
{
	/* Pop up an info dialog about the player */
	GGZPlayer *player = data;

	player_info_create_or_raise(player);
}

static void client_player_friends_click(GtkMenuItem * menuitem, gpointer data)
{
	GGZPlayer *player = data;

	if (GTK_CHECK_MENU_ITEM(menuitem)->active)
		chat_add_friend(ggzcore_player_get_name(player), TRUE);
	else
		chat_remove_friend(ggzcore_player_get_name(player));
}

static void client_player_ignore_click(GtkMenuItem * menuitem, gpointer data)
{
	GGZPlayer *player = data;

	if (GTK_CHECK_MENU_ITEM(menuitem)->active)
		chat_add_ignore(ggzcore_player_get_name(player), TRUE);
	else
		chat_remove_ignore(ggzcore_player_get_name(player));
}

static GtkWidget *create_mnu_player(GGZPlayer *player, gboolean is_friend,
				    gboolean is_ignore)
{
	GtkWidget *mnu_player;
	GtkWidget *info;
	GtkWidget *separator9;
	GtkWidget *friends;
	GtkWidget *ignore;

	mnu_player = gtk_menu_new();

	info = gtk_menu_item_new_with_label(_("Info"));
	gtk_container_add(GTK_CONTAINER(mnu_player), info);

	separator9 = gtk_menu_item_new();
	gtk_container_add(GTK_CONTAINER(mnu_player), separator9);
	gtk_widget_set_sensitive(separator9, FALSE);

	friends = gtk_check_menu_item_new_with_label(_("Friends"));
	gtk_container_add(GTK_CONTAINER(mnu_player), friends);
	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(friends),
				       is_friend);

	ignore = gtk_check_menu_item_new_with_label(_("Ignore"));
	gtk_container_add(GTK_CONTAINER(mnu_player), ignore);
	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(ignore),
				       is_ignore);

	g_signal_connect(GTK_OBJECT(info), "activate",
			 GTK_SIGNAL_FUNC(client_player_info_activate),
			 player);
	g_signal_connect(GTK_OBJECT(friends), "activate",
			 GTK_SIGNAL_FUNC(client_player_friends_click),
			 player);
	g_signal_connect(GTK_OBJECT(ignore), "activate",
			 GTK_SIGNAL_FUNC(client_player_ignore_click),
			 player);

	return mnu_player;
}

static gboolean player_list_event(GtkWidget *widget,
				  GdkEvent *event, gpointer data)
{
	GdkEventButton *buttonevent = (GdkEventButton *) event;
	GtkTreeView *tree = GTK_TREE_VIEW(widget);
	GtkTreeModel *model = gtk_tree_view_get_model(tree);
	GtkTreeIter iter;
	GGZPlayer *player = NULL;
	char *name;
	GtkTreePath *path = NULL;

	if (!gtk_tree_view_get_path_at_pos(tree,
					   buttonevent->x, buttonevent->y,
					   &path, NULL, NULL, NULL)) {
		return FALSE;
	}
	gtk_tree_model_get_iter(model, &iter, path);

	gtk_tree_model_get(model, &iter, PLAYER_COLUMN_NAME, &name, -1);
	player = ggzcore_server_get_player(server, name);
	g_free(name);
	if (event->type == GDK_BUTTON_PRESS
	    && buttonevent->button == 3
	    && player) {
		/* Right mouse button:
		 * Create and display the menu */
		char *name = ggzcore_player_get_name(player);
		int is_friend = chat_is_friend(name);
		int is_ignore = chat_is_ignore(name);
		GtkWidget *menu;

		menu = create_mnu_player(player, is_friend, is_ignore);
		gtk_widget_show_all(menu);
		gtk_menu_popup(GTK_MENU(menu), NULL, NULL, NULL,
			       NULL, buttonevent->button, 0);
	}

	return FALSE;
}

void sensitize_player_list(gboolean sensitive)
{
	GtkWidget *tree = player_list;

	gtk_widget_set_sensitive(tree, sensitive);
}

/* Clear current list of players */
void clear_player_list(void)
{
	GtkWidget *store = ggz_lookup_widget(player_list, "player_list_store");

        gtk_list_store_clear(GTK_LIST_STORE(store));
}

#define LAG_CATEGORIES 6
gboolean pixmaps_initted = FALSE;
GdkPixbuf *lag[LAG_CATEGORIES], *guest, *registered, *admin, *bot;

void update_player_list(void)
{
	GtkListStore *store;
	gint i, num, l;
	GGZPlayer *p;
	GGZTable *table;
	GGZRoom *room = ggzcore_server_get_cur_room(server);
	GdkPixmap *pixmap;
	GdkBitmap *mask;
	int wins, losses, ties, forfeits, rating, ranking, highscore;
	char stats[512];

	/* Retrieve the player list widget. */
	store = GTK_LIST_STORE(ggz_lookup_widget(player_list,
					     "player_list_store"));

	/* Clear current list of players */
	gtk_list_store_clear(GTK_LIST_STORE(store));

	/* Display current list of players */
	num = ggzcore_room_get_num_players(room);

	/* Ensure the lag graphics are loaded. */
	if (!pixmaps_initted) {
		for (i = 0; i < LAG_CATEGORIES; i++) {
			gchar name[512];

#define ICON_SIZE 14
			snprintf(name, sizeof(name), "ggz_gtk_lag%d", i);
			lag[i] = load_svg_pixbuf(name, ICON_SIZE, ICON_SIZE);
		}
		guest = load_svg_pixbuf("ggz_gtk_guest", 16, 24);
		registered = load_svg_pixbuf("ggz_gtk_registered", 16, 24);
		admin = load_svg_pixbuf("ggz_gtk_admin", 16, 24);
		bot = load_svg_pixbuf("ggz_gtk_bot", 16, 24);
		pixmaps_initted = TRUE;
	}

	for (i = 0; i < num; i++) {
		GtkTreeIter iter;
		gchar tabletext[32];

		p = ggzcore_room_get_nth_player(room, i);
		table = ggzcore_player_get_table(p);

		gtk_list_store_append(store, &iter);

		/* Name */
		gtk_list_store_set(store, &iter, PLAYER_COLUMN_NAME,
				   ggzcore_player_get_name(p), -1);

		/* Table # */
		if (table <= 0) {
			snprintf(tabletext, sizeof(tabletext), "--");
		} else {
			snprintf(tabletext, sizeof(tabletext), "%d",
				 ggzcore_table_get_id(table));
		}
		gtk_list_store_set(store, &iter,
				   PLAYER_COLUMN_TABLE, tabletext, -1);

		/* Stats (ranking, etc.) */
		if (ggzcore_player_get_ranking(p, &ranking)) {
			snprintf(stats, sizeof(stats), _("#%d"), ranking);
		} else if (ggzcore_player_get_highscore(p, &highscore)) {
			snprintf(stats, sizeof(stats), "%d", highscore);
		} else if (ggzcore_player_get_rating(p, &rating)) {
			snprintf(stats, sizeof(stats), "%d", rating);
		} else if (ggzcore_player_get_record(p, &wins, &losses,
						     &ties, &forfeits)) {
			snprintf(stats, sizeof(stats), "%d-%d", wins, losses);
			if (ties > 0) {
				snprintf(stats + strlen(stats),
					 sizeof(stats) - strlen(stats),
					 "-%d", ties);
			}
			if (forfeits > 0) {
				snprintf(stats + strlen(stats),
					 sizeof(stats) - strlen(stats),
					 " (%d)", forfeits);
			}
		} else {
			snprintf(stats, sizeof(stats), "%s", "");
		}
		gtk_list_store_set(store, &iter,
				   PLAYER_COLUMN_STATS, stats, -1);

		/* Lag icon */
		l = ggzcore_player_get_lag(p);
		if (l >= 0 && l < LAG_CATEGORIES && lag[l]) {
			gtk_list_store_set(store, &iter,
					   PLAYER_COLUMN_LAG, lag[l], -1);
		}

		/* "Type" icon */
		switch (ggzcore_player_get_type(p)) {
		case GGZ_PLAYER_GUEST:
			gtk_list_store_set(store, &iter,
					   PLAYER_COLUMN_TYPE, guest, -1);
			break;
		case GGZ_PLAYER_NORMAL:
			gtk_list_store_set(store, &iter,
					   PLAYER_COLUMN_TYPE, registered, -1);
			break;
		case GGZ_PLAYER_HOST:
		case GGZ_PLAYER_ADMIN:
			gtk_list_store_set(store, &iter,
					   PLAYER_COLUMN_TYPE, admin, -1);
			break;
		case GGZ_PLAYER_BOT:
			gtk_list_store_set(store, &iter,
					   PLAYER_COLUMN_TYPE, bot, -1);
			break;
		case GGZ_PLAYER_UNKNOWN:
			pixmap = mask = NULL;
			break;
		}

	}
}

static gint player_sort_func(GtkTreeModel *model,
			   GtkTreeIter *a, GtkTreeIter *b, gpointer data)
{
	char *nameA, *nameB;

	gtk_tree_model_get(model, a, PLAYER_COLUMN_NAME, &nameA, -1);
	gtk_tree_model_get(model, b, PLAYER_COLUMN_NAME, &nameB, -1);

	return (strcasecmp(nameA, nameB));
}

GtkWidget *create_player_list(GtkWidget *parent)
{
	GtkListStore *store;
	GtkWidget *tree;
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	GtkTreeSelection *select;

	store = gtk_list_store_new(PLAYER_COLUMNS,
				   GDK_TYPE_PIXBUF,
				   GDK_TYPE_PIXBUF,
				   G_TYPE_STRING, /* For now */
				   G_TYPE_STRING,
				   G_TYPE_STRING);
	tree = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
	g_object_unref(store);

	gtk_tree_sortable_set_sort_column_id(GTK_TREE_SORTABLE(store),
					     PLAYER_COLUMN_NAME,
					     GTK_SORT_ASCENDING);
	gtk_tree_sortable_set_sort_func(GTK_TREE_SORTABLE(store),
					PLAYER_COLUMN_NAME,
					player_sort_func,
					NULL, NULL);

	renderer = gtk_cell_renderer_pixbuf_new();
	column = gtk_tree_view_column_new_with_attributes("", renderer,
				"pixbuf", PLAYER_COLUMN_TYPE, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);

	renderer = gtk_cell_renderer_pixbuf_new();
        /* Translators: L is short for 'Network Latency'.  */
	column = gtk_tree_view_column_new_with_attributes(_("L"), renderer,
				"pixbuf", PLAYER_COLUMN_LAG, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);

	renderer = gtk_cell_renderer_text_new();
        /* Translators: T# is short for 'Table Number'.  */
	column = gtk_tree_view_column_new_with_attributes(_("T#"), renderer,
				"text", PLAYER_COLUMN_TABLE, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);

	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes(_("Stats"),
							  renderer,
				"text", PLAYER_COLUMN_STATS, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);

	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes(_("Name"),
							  renderer,
				"text", PLAYER_COLUMN_NAME, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);

	g_object_set_data(G_OBJECT(tree), "player_list_store", store);
	g_object_set_data(G_OBJECT(parent), "player_list_store", store);
	gtk_widget_set_sensitive(tree, FALSE);
	GTK_WIDGET_UNSET_FLAGS(tree, GTK_CAN_FOCUS);

	select = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree));
	gtk_tree_selection_set_mode(select, GTK_SELECTION_SINGLE);

	g_signal_connect(tree, "button-press-event",
			 GTK_SIGNAL_FUNC(player_list_event), NULL);

	player_list = tree;
	return tree;
}
