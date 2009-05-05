/*
 * File: info.c
 * Author: Justin Zaun
 * Project: GGZ GTK Client
 * $Id$
 *
 * This dialog is used to display information about a selected room to
 * the user.
 *
 * Copyright (C) 2001 Justin Zaun.
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
#  include <config.h>	/* Site-specific config */
#endif

#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>
#include <ggzcore.h>

#include "client.h"
#include "types.h"
#include "msgbox.h"
#include "server.h"
#include "support.h"

enum {
	TYPE_COLUMN_NAME,
	TYPE_COLUMN_AUTHOR,
	TYPE_COLUMN_WEB,
	TYPE_COLUMN_DESC,
	TYPE_COLUMNS
};

static GtkWidget *types_dialog;
static GtkWidget *create_dlg_types(void);

static void update_dlg_types(void)
{
	GtkWidget *tree = ggz_lookup_widget(types_dialog, "types_list");
	GtkTreeModel *store = gtk_tree_view_get_model(GTK_TREE_VIEW(tree));
	int i, num;

	gtk_list_store_clear(GTK_LIST_STORE(store));
	num = ggzcore_server_get_num_gametypes(server);
	for (i = 0; i < num; i++) {
		GGZGameType *gt =
		    ggzcore_server_get_nth_gametype(server, i);
		GtkTreeIter iter;
		const gchar *name = ggzcore_gametype_get_name(gt);
		const gchar *author = ggzcore_gametype_get_author(gt);
		const gchar *url = ggzcore_gametype_get_url(gt);
		const gchar *desc = ggzcore_gametype_get_desc(gt);

		gtk_list_store_append(GTK_LIST_STORE(store), &iter);
		gtk_list_store_set(GTK_LIST_STORE(store), &iter,
				   TYPE_COLUMN_NAME, name,
				   TYPE_COLUMN_AUTHOR, author,
				   TYPE_COLUMN_WEB, url,
				   TYPE_COLUMN_DESC, desc, -1);

#ifdef FILTER
		GtkWidget *tmp, *menu, *menuitem;

		tmp = ggz_lookup_widget(types_dialog, "filter_optionmenu");
		menu = gtk_option_menu_get_menu(GTK_OPTION_MENU(tmp));
		menuitem = gtk_menu_item_new_with_label(name);
		gtk_menu_append(GTK_MENU(menu), menuitem);
#endif
	}
}

/* types_create_or_raise() - Displays the dialog or updates current dialog
 *                          with game list
 *
 * Recieves:
 *
 * Returns:
 */

void types_create_or_raise(void)
{
	if (!types_dialog) {
		types_dialog = create_dlg_types();
		update_dlg_types();

#ifdef FILTER
		GtkWidget *tmp, *menu;
		GtkWidget *menuitem;

		tmp = ggz_lookup_widget(types_dialog, "filter_optionmenu");
		menu = gtk_option_menu_get_menu(GTK_OPTION_MENU(tmp));
		menuitem = gtk_menu_item_new_with_label(_("None"));
		gtk_menu_append(GTK_MENU(menu), menuitem);
#endif

		gtk_widget_show_all(types_dialog);
	} else {
		gdk_window_raise(types_dialog->window);
	}
}

#if 0	/* currently unused */
static gboolean types_list_events(GtkWidget * widget, GdkEvent * event,
				  gpointer data)
{

	return FALSE;
}


static void types_upgrade(GtkMenuItem * menuitem, gpointer data)
{

}


static void types_URL(GtkMenuItem * menuitem, gpointer data)
{

}
#endif


#ifdef FILTER
static void types_filter_button(GtkWidget * widget, gpointer data)
{
	msgbox(_("Room filtering is not implemented yet. If\n"
		 "you would like to help head over to\n"
		 "http://www.ggzgamingzone.org/"),
	       _("Not Implemented"),
	       MSGBOX_OKONLY, MSGBOX_NONE, MSGBOX_NORMAL);

}
#endif


static GtkWidget *tree_new(GtkWidget * parent)
{
	GtkListStore *store;
	GtkWidget *tree;
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	GtkTreeSelection *select;

	assert(TYPE_COLUMNS == 4);
	store = gtk_list_store_new(TYPE_COLUMNS,
				   G_TYPE_STRING,
				   G_TYPE_STRING,
				   G_TYPE_STRING, G_TYPE_STRING);
	tree = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
	g_object_unref(store);

	renderer = gtk_cell_renderer_text_new();
	column =
	    gtk_tree_view_column_new_with_attributes(_("Name"), renderer,
						     "text",
						     TYPE_COLUMN_NAME,
						     NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);

	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes(_("Description"),
							  renderer,
							  "text",
							  TYPE_COLUMN_DESC,
							  NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);

	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes(_("Web Address"),
							  renderer,
							  "text",
							  TYPE_COLUMN_WEB,
							  NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);

	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes(_("Author"),
							  renderer,
							  "text",
							  TYPE_COLUMN_AUTHOR,
							  NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);

	g_object_set_data(G_OBJECT(parent), "types_list", tree);
	g_object_set_data(G_OBJECT(parent), "table_list_store", store);
	GTK_WIDGET_UNSET_FLAGS(tree, GTK_CAN_FOCUS);

	select = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree));
	gtk_tree_selection_set_mode(select, GTK_SELECTION_SINGLE);

	return tree;
}


GtkWidget *create_dlg_types(void)
{
	GtkWidget *dlg_types;
	GtkWidget *vbox;
	GtkWidget *scrolledwindow;
	GtkWidget *types_list;
#ifdef FILTER
	GtkWidget *hbox;
	GtkWidget *filter_label;
	GtkWidget *filter_optionmenu;
	GtkWidget *filter_optionmenu_menu;
	GtkWidget *filter_button;
#endif
	GtkWidget *action_area, *button_box, *close_button;

	dlg_types = gtk_dialog_new();
	gtk_window_set_transient_for(GTK_WINDOW(dlg_types),
				     GTK_WINDOW(main_window));
	g_object_set_data(G_OBJECT(dlg_types), "dlg_types", dlg_types);
	gtk_widget_set_size_request(dlg_types, 600, 300);
	gtk_window_set_title(GTK_WINDOW(dlg_types), _("Game Types"));

	vbox = GTK_DIALOG(dlg_types)->vbox;

	scrolledwindow = gtk_scrolled_window_new(NULL, NULL);
	gtk_box_pack_start(GTK_BOX(vbox), scrolledwindow, TRUE, TRUE, 0);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledwindow),
				       GTK_POLICY_AUTOMATIC,
				       GTK_POLICY_AUTOMATIC);

	types_list = tree_new(dlg_types);
	gtk_container_add(GTK_CONTAINER(scrolledwindow), types_list);

#ifdef FILTER
	hbox = gtk_hbox_new(FALSE, 4);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

	filter_label = gtk_label_new(_("Room List Filter:"));
	gtk_box_pack_start(GTK_BOX(hbox), filter_label, FALSE, FALSE, 0);

	filter_optionmenu = gtk_option_menu_new();
	g_object_set_data(G_OBJECT(dlg_types), "filter_optionmenu",
			  filter_optionmenu);
	gtk_box_pack_start(GTK_BOX(hbox), filter_optionmenu, FALSE, FALSE, 0);

	filter_optionmenu_menu = gtk_menu_new();
	gtk_option_menu_set_menu(GTK_OPTION_MENU(filter_optionmenu),
				 filter_optionmenu_menu);

	filter_button = gtk_button_new_with_label(_("Set"));
	gtk_box_pack_start(GTK_BOX(hbox), filter_button, FALSE, FALSE, 0);
#endif

	action_area = GTK_DIALOG(dlg_types)->action_area;

	button_box = gtk_hbutton_box_new();
	gtk_box_pack_start(GTK_BOX(action_area), button_box, TRUE, TRUE,
			   0);

	close_button = gtk_button_new_from_stock(GTK_STOCK_CLOSE);
	gtk_container_add(GTK_CONTAINER(button_box), close_button);
	GTK_WIDGET_SET_FLAGS(close_button, GTK_CAN_DEFAULT);

	g_signal_connect(dlg_types, "destroy",
			 GTK_SIGNAL_FUNC(gtk_widget_destroyed),
			 &types_dialog);
#ifdef FILTER
	g_signal_connect(filter_button, "clicked",
			 GTK_SIGNAL_FUNC(types_filter_button), NULL);
#endif
	g_signal_connect_swapped(close_button, "clicked",
				 GTK_SIGNAL_FUNC(gtk_widget_destroy),
				 dlg_types);

	return dlg_types;
}
