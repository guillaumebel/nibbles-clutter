/* 
 * File: launch.c
 * Author: Justin Zaun
 * Project: GGZ GTK Client
 * $Id$
 *
 * Code for launching games through the GTK client
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
#  include "config.h"
#endif

#include <assert.h>
#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <ggz.h>

#include "launch.h"
#include "support.h"
#include "chat.h"
#include "client.h"
#include "game.h"
#include "ggzcore.h"
#include "msgbox.h"
#include "pick_module.h"
#include "server.h"

/* The maximum number of characters in the reserved seat name (not counting
   termination) */
#define MAX_RESERVED_NAME_LEN 16

static void launch_fill_defaults(GtkWidget * widget, gpointer data);
static void launch_seats_changed(GtkWidget * widget, gpointer data);
static void launch_resv_toggle(GtkWidget * widget, gpointer data);
static void launch_start_game(GtkWidget * widget, gpointer data);
static void launch_cancel_button_clicked(GtkWidget * widget, gpointer data);
static void launch_seat_show(gint seat, gchar show);
static GtkWidget *create_dlg_launch(void);

static GtkWidget *launch_dialog;
static char _launching = 0;

/* Close the launch dialog.  This is done when switching rooms. */
void launch_dialog_close(void)
{
	if (!launch_dialog) return;

	/* Free up game we allocated but never launched */
	cancel_module_picking();
	game_destroy();

	gtk_widget_destroy(launch_dialog);
	launch_dialog = NULL;
}

void launch_create_or_raise(void)
{
	if (!launch_dialog) {
		/* Dialog for setting table seats */
		launch_dialog = create_dlg_launch();
		if (launch_dialog) {
			gtk_widget_show(launch_dialog);
		} else {
			/* We shouldn't have gotten this far - menu and
			   buttons should be disabled. */
			ggz_error_msg("Couldn't create launch dialog.");
		}
	} else {
		/* It already exists, so raise it */
		gdk_window_show(launch_dialog->window);
		gdk_window_raise(launch_dialog->window);
	}
}


static void launch_fill_defaults(GtkWidget * widget, gpointer data)
{
	GtkWidget *tmp;
	gchar *text;
	GGZRoom *room;
	GGZGameType *gt;
	GList *items = NULL;
	gint maxplayers, maxbots, x;

	room = ggzcore_server_get_cur_room(server);
	gt = ggzcore_room_get_gametype(room);

	/* Set the labels */
	tmp = g_object_get_data(G_OBJECT(launch_dialog), "type_label");
	text = g_strdup_printf(_("Game Type:  %s"),
			       ggzcore_gametype_get_name(gt));
	gtk_label_set_text(GTK_LABEL(tmp), text);
	g_free(text);

	tmp = g_object_get_data(G_OBJECT(launch_dialog), "author_label");
	text = g_strdup_printf(_("Author:  %s"),
			       ggzcore_gametype_get_author(gt));
	gtk_label_set_text(GTK_LABEL(tmp), text);
	g_free(text);

	tmp = g_object_get_data(G_OBJECT(launch_dialog),
				  "type_desc_label");
	text = g_strdup_printf(_("Description:  %s"),
			       ggzcore_gametype_get_desc(gt));
	gtk_label_set_text(GTK_LABEL(tmp), text);
	g_free(text);

	tmp = g_object_get_data(G_OBJECT(launch_dialog), "web_label");
	text = g_strdup_printf(_("Home Page:  %s"),
			       ggzcore_gametype_get_url(gt));
	gtk_label_set_text(GTK_LABEL(tmp), text);
	g_free(text);


	/* Set the number of players combo */
	maxplayers = ggzcore_gametype_get_max_players(gt);
	for (x = 1; x <= maxplayers; x++) {
		if (ggzcore_gametype_num_players_is_valid(gt, x))
			items = g_list_append(items,
					      g_strdup_printf("%d", x));
	}
	tmp = g_object_get_data(G_OBJECT(launch_dialog), "seats_combo");
	gtk_combo_set_popdown_strings(GTK_COMBO(tmp), items);

	/* Show seats that should be shown (i.e. all of them). */
	for (x = 1; x <= maxplayers; x++)
		launch_seat_show(x, TRUE);

	/* Default to reserving us a seat */
	tmp = g_object_get_data(G_OBJECT(launch_dialog), "seat1_name");
	gtk_entry_set_text(GTK_ENTRY(tmp), ggzcore_server_get_handle(server));

	/* FIXME: for guest users, the reservation won't work.  But it should
	   be easily converted by ggzd into an open seat, which should be
	   fine. */
	tmp = g_object_get_data(G_OBJECT(launch_dialog), "seat1_resv");
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(tmp), TRUE);

	/* If bots are allowed, default to bot players, otherwise open */
	maxbots = ggzcore_gametype_get_max_bots(gt);

	if (maxbots < maxplayers) {
		tmp = ggz_lookup_widget(launch_dialog, "seat1_bot");
		gtk_widget_set_sensitive(GTK_WIDGET(tmp), FALSE);
	}

	for (x = 2; x <= maxplayers; x++) {
		char text[128];

		if (maxbots >= x - 1) {
			snprintf(text, sizeof(text), "seat%d_bot", x);
		} else {
			snprintf(text, sizeof(text), "seat%d_bot", x);
			tmp = ggz_lookup_widget(launch_dialog, text);
			gtk_widget_set_sensitive(GTK_WIDGET(tmp), FALSE);

			snprintf(text, sizeof(text), "seat%d_open", x);
		}
		tmp = ggz_lookup_widget(launch_dialog, text);
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(tmp), TRUE);
	}
}


static void launch_seats_changed(GtkWidget * widget, gpointer data)
{
	gpointer tmp;
	gint i, seats, max;
	GGZGameType *gt;
	GGZRoom *room;

	seats = atoi(gtk_entry_get_text(GTK_ENTRY(widget)));
	room = ggzcore_server_get_cur_room(server);
	gt = ggzcore_room_get_gametype(room);
	max = ggzcore_gametype_get_max_players(gt);

	/* Desensitize seats with numbers bigger than what we want and
	   sensitize others */
	for (i = 1; i <= seats; i++) {
		char text[128];
		snprintf(text, sizeof(text), "seat%d_box", i);
		tmp = g_object_get_data(G_OBJECT(launch_dialog), text);
		gtk_widget_set_sensitive(tmp, TRUE);
	}
	for (i = (seats + 1); i <= max; i++) {
		char text[128];
		snprintf(text, sizeof(text), "seat%d_box", i);
		tmp = g_object_get_data(G_OBJECT(launch_dialog), text);
		gtk_widget_set_sensitive(tmp, FALSE);
	}
}


static void launch_resv_toggle(GtkWidget * widget, gpointer data)
{
	if (GTK_TOGGLE_BUTTON(widget)->active) {
		gtk_widget_set_sensitive(GTK_WIDGET(data), TRUE);
		gtk_widget_grab_focus(data);
	} else
		gtk_widget_set_sensitive(GTK_WIDGET(data), FALSE);
}


char launch_in_process(void)
{
	return _launching;
}

void launch_table(void)
{
	GtkWidget *tmp;
	GGZRoom *room;
	GGZTable *table;
	GGZGameType *gt;
	gint seats;
	int x, status;

	_launching = 0;

	/* Grab the number of seats */
	if (!launch_dialog)
		ggz_error_msg("Trying to launch table when "
			      "there is no launch dialog.");
	tmp = ggz_lookup_widget(launch_dialog, "seats_combo");
	seats = atoi(gtk_entry_get_text(GTK_ENTRY(GTK_COMBO(tmp)->entry)));

	/* Create a table for sending to the server */
	table = ggzcore_table_new();
	room = ggzcore_server_get_cur_room(server);
	gt = ggzcore_room_get_gametype(room);
	tmp = ggz_lookup_widget(launch_dialog, "desc_entry");
	ggzcore_table_init(table, gt, gtk_entry_get_text(GTK_ENTRY(tmp)),
			   seats);

	for (x = 0; x < seats; x++) {
		/* Check to see if the seat is a bot. */
		char text[128];
		snprintf(text, sizeof(text), "seat%d_bot", x + 1);
		tmp = ggz_lookup_widget(launch_dialog, text);
		if (GTK_TOGGLE_BUTTON(tmp)->active)
			if (ggzcore_table_set_seat(table, x,
						   GGZ_SEAT_BOT, NULL) < 0)
				assert(0);

		/* Check to see if the seat is reserved. */
		snprintf(text, sizeof(text), "seat%d_resv", x + 1);
		tmp = ggz_lookup_widget(launch_dialog, text);
		if (GTK_TOGGLE_BUTTON(tmp)->active) {
			const gchar *name;
			snprintf(text, sizeof(text), "seat%d_name", x + 1);
			tmp = ggz_lookup_widget(launch_dialog, text);
			name = gtk_entry_get_text(GTK_ENTRY(tmp));

			if (ggzcore_table_set_seat(table, x,
						   GGZ_SEAT_RESERVED,
						   name) < 0)
				assert(0);
		}
	}

	status = ggzcore_room_launch_table(room, table);
	ggzcore_table_free(table);

	if (status < 0) {
		msgbox(_("Failed to launch table.\n Launch aborted."),
		       _("Launch Error"), MSGBOX_OKONLY, MSGBOX_STOP,
		       MSGBOX_NORMAL);
		game_destroy();
	}

	gtk_widget_destroy(launch_dialog);
}



static void launch_start_game(GtkWidget * widget, gpointer data)
{
	GtkWidget *tmp;
	GGZRoom *room;
	GGZGameType *gt;
	gint x, seats, bots;

	/* Grab the number of seats */
	tmp = ggz_lookup_widget(launch_dialog, "seats_combo");
	seats = atoi(gtk_entry_get_text(GTK_ENTRY(GTK_COMBO(tmp)->entry)));

	/* Let's go bot counting.... */
	bots = 0;
	for (x = 0; x < seats; x++) {
		char text[128];
		snprintf(text, sizeof(text), "seat%d_bot", x + 1);
		tmp = ggz_lookup_widget(launch_dialog, text);
		if (GTK_TOGGLE_BUTTON(tmp)->active)
			bots++;
	}

	room = ggzcore_server_get_cur_room(server);
	gt = ggzcore_room_get_gametype(room);

	if (!ggzcore_gametype_num_bots_is_valid(gt, bots)) {
		msgbox(_("Invalid number of bots specified"), _("Error"),
		       MSGBOX_OKONLY, MSGBOX_STOP, MSGBOX_NORMAL);

		return;
	}

	/* Create new game object */
	if (game_initialize(0) == 0) {
		if (game_launch() < 0) {
			msgbox(_("Error launching game module."),
			       _("Game Error"), MSGBOX_OKONLY, MSGBOX_INFO,
			       MSGBOX_NORMAL);
			game_destroy();
		} else
			_launching = 1;
	}
}


static void launch_cancel_button_clicked(GtkWidget * widget, gpointer data)
{
	launch_dialog_close();
}


static void launch_seat_show(gint seat, gchar show)
{
	gpointer tmp;
	char text[128];

	/* Show seat's hbox */
	snprintf(text, sizeof(text), "seat%d_box", seat);
	tmp = g_object_get_data(G_OBJECT(launch_dialog), text);
	if (show)
		gtk_widget_show(GTK_WIDGET(tmp));
	else
		gtk_widget_hide(GTK_WIDGET(tmp));

}



GtkWidget *create_dlg_launch(void)
{
	GtkWidget *dlg_launch;
	GtkWidget *vbox1;
	GtkWidget *main_box;
	GtkWidget *game_box;
	GtkWidget *seat_box;
	GtkWidget *type_label;
	GtkWidget *game_label;
	GtkWidget *seats_combo;
	GList *seats_combo_items = NULL;
	GtkWidget *seats_entry;
	GtkWidget *players_label;
	GtkWidget *author_box;
	GtkWidget *author_label;
	GtkWidget *type_desc_box;
	GtkWidget *type_desc_label;
	GtkWidget *web_box;
	GtkWidget *web_label;
	GtkWidget *separator;
	GtkWidget *desc_box;
	GtkWidget *game_desc_label;
	GtkWidget *desc_entry;
	GtkWidget *hbox11;
	GtkWidget *button_box;
	GtkWidget *launch_button;
	GtkWidget *cancel_button;

	GGZRoom *room = ggzcore_server_get_cur_room(server);
	GGZGameType *gametype = ggzcore_room_get_gametype(room);
	const int num_seats = ggzcore_gametype_get_max_players(gametype);
	struct {
		GtkWidget *box;
		GtkWidget *label;
		GSList *group;
		GtkWidget *bot;
		GtkWidget *open;
		GtkWidget *resv;
		GtkWidget *name;
	} seats[num_seats];
	int i;

	/* Sanity check.  This can actually fail if GGZ gets confused
	   elsewhere. */
	if (num_seats <= 0) {
		ggz_error_msg("Tried to launch table for invalid game.");
		return NULL;
	}

	dlg_launch = gtk_dialog_new();
	gtk_window_set_transient_for(GTK_WINDOW(dlg_launch),
				     GTK_WINDOW(main_window));
	g_object_set_data(G_OBJECT(dlg_launch), "dlg_launch", dlg_launch);
	gtk_window_set_title(GTK_WINDOW(dlg_launch), _("Seat Assignments"));
	gtk_window_set_resizable(GTK_WINDOW(dlg_launch), TRUE);

	vbox1 = GTK_DIALOG(dlg_launch)->vbox;
	g_object_set_data(G_OBJECT(dlg_launch), "vbox1", vbox1);
	gtk_widget_show(vbox1);

	main_box = gtk_vbox_new(FALSE, 3);
	gtk_widget_ref(main_box);
	g_object_set_data_full(G_OBJECT(dlg_launch), "main_box", main_box,
				 (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show(main_box);
	gtk_box_pack_start(GTK_BOX(vbox1), main_box, TRUE, TRUE, 0);
	gtk_container_set_border_width(GTK_CONTAINER(main_box), 10);

	game_box = gtk_hbox_new(FALSE, 10);
	gtk_widget_ref(game_box);
	g_object_set_data_full(G_OBJECT(dlg_launch), "game_box",
				 game_box,
				 (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show(game_box);
	gtk_box_pack_start(GTK_BOX(main_box), game_box, FALSE, FALSE, 5);

	type_label = gtk_label_new(_("Game Type:"));
	gtk_widget_ref(type_label);
	g_object_set_data_full(G_OBJECT(dlg_launch), "type_label",
				 type_label,
				 (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show(type_label);
	gtk_box_pack_start(GTK_BOX(game_box), type_label, FALSE, FALSE, 0);

	game_label = gtk_label_new("");
	gtk_widget_ref(game_label);
	g_object_set_data_full(G_OBJECT(dlg_launch), "game_label",
				 game_label,
				 (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show(game_label);
	gtk_box_pack_start(GTK_BOX(game_box), game_label, FALSE, FALSE, 0);

	seats_combo = gtk_combo_new();
	gtk_widget_ref(seats_combo);
	g_object_set_data_full(G_OBJECT(dlg_launch), "seats_combo",
				 seats_combo,
				 (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show(seats_combo);
	gtk_box_pack_end(GTK_BOX(game_box), seats_combo, FALSE, TRUE, 0);
	gtk_combo_set_value_in_list(GTK_COMBO(seats_combo), TRUE, FALSE);
	seats_combo_items = g_list_append(seats_combo_items, (gpointer) "");
	gtk_combo_set_popdown_strings(GTK_COMBO(seats_combo),
				      seats_combo_items);
	g_list_free(seats_combo_items);

	seats_entry = GTK_COMBO(seats_combo)->entry;
	gtk_widget_ref(seats_entry);
	g_object_set_data_full(G_OBJECT(dlg_launch), "seats_entry",
				 seats_entry,
				 (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show(seats_entry);

	players_label = gtk_label_new(_("Number of seats"));
	gtk_widget_ref(players_label);
	g_object_set_data_full(G_OBJECT(dlg_launch), "players_label",
				 players_label,
				 (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show(players_label);
	gtk_box_pack_end(GTK_BOX(game_box), players_label, FALSE, FALSE, 0);

	author_box = gtk_hbox_new(FALSE, 0);
	gtk_widget_ref(author_box);
	g_object_set_data_full(G_OBJECT(dlg_launch), "author_box",
				 author_box,
				 (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show(author_box);
	gtk_box_pack_start(GTK_BOX(main_box), author_box, FALSE, FALSE, 0);

	author_label = gtk_label_new(_("Author:"));
	gtk_widget_ref(author_label);
	g_object_set_data_full(G_OBJECT(dlg_launch), "author_label",
				 author_label,
				 (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show(author_label);
	gtk_box_pack_start(GTK_BOX(author_box), author_label, FALSE, FALSE,
			   0);
	gtk_label_set_justify(GTK_LABEL(author_label), GTK_JUSTIFY_LEFT);

	type_desc_box = gtk_hbox_new(FALSE, 0);
	gtk_widget_ref(type_desc_box);
	g_object_set_data_full(G_OBJECT(dlg_launch), "type_desc_box",
				 type_desc_box,
				 (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show(type_desc_box);
	gtk_box_pack_start(GTK_BOX(main_box), type_desc_box, FALSE, FALSE, 0);

	type_desc_label = gtk_label_new(_("Description:"));
	gtk_widget_ref(type_desc_label);
	g_object_set_data_full(G_OBJECT(dlg_launch), "type_desc_label",
				 type_desc_label,
				 (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show(type_desc_label);
	gtk_box_pack_start(GTK_BOX(type_desc_box), type_desc_label, FALSE,
			   FALSE, 0);
	gtk_label_set_justify(GTK_LABEL(type_desc_label), GTK_JUSTIFY_LEFT);

	web_box = gtk_hbox_new(FALSE, 0);
	gtk_widget_ref(web_box);
	g_object_set_data_full(G_OBJECT(dlg_launch), "web_box", web_box,
				 (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show(web_box);
	gtk_box_pack_start(GTK_BOX(main_box), web_box, FALSE, FALSE, 0);

	web_label = gtk_label_new(_("Homepage:"));
	gtk_widget_ref(web_label);
	g_object_set_data_full(G_OBJECT(dlg_launch), "web_label",
				 web_label,
				 (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show(web_label);
	gtk_box_pack_start(GTK_BOX(web_box), web_label, FALSE, FALSE, 0);
	gtk_label_set_justify(GTK_LABEL(web_label), GTK_JUSTIFY_LEFT);

	separator = gtk_hseparator_new();
	gtk_widget_ref(separator);
	g_object_set_data_full(G_OBJECT(dlg_launch), "separator",
				 separator,
				 (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show(separator);
	gtk_box_pack_start(GTK_BOX(main_box), separator, FALSE, FALSE, 3);

	if (num_seats > 10) {
		GtkWidget *scroll;

		scroll = gtk_scrolled_window_new(NULL, NULL);
		gtk_widget_ref(scroll);
		g_object_set_data_full(G_OBJECT(dlg_launch),
					 "scrolledwindow", scroll,
					 (GtkDestroyNotify) gtk_widget_unref);
		gtk_widget_show(scroll);
		gtk_box_pack_start(GTK_BOX(main_box), scroll, TRUE, TRUE, 0);
		gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll),
					       GTK_POLICY_AUTOMATIC,
					       GTK_POLICY_AUTOMATIC);

		seat_box = gtk_vbox_new(FALSE, 3);
		gtk_widget_ref(seat_box);
		g_object_set_data_full(G_OBJECT(dlg_launch),
					 "seat_box", seat_box,
					 (GtkDestroyNotify) gtk_widget_unref);
		gtk_widget_show(seat_box);
		gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW
						      (scroll), seat_box);
	} else
		seat_box = main_box;

	memset(seats, 0, sizeof(seats));
	for (i = 0; i < num_seats; i++) {
		char text[128];

		seats[i].box = gtk_hbox_new(FALSE, 0);
		gtk_widget_ref(seats[i].box);
		snprintf(text, sizeof(text), "seat%d_box", i + 1);
		g_object_set_data_full(G_OBJECT(dlg_launch), text,
					 seats[i].box,
					 (GtkDestroyNotify) gtk_widget_unref);
		gtk_widget_show(seats[i].box);
		gtk_box_pack_start(GTK_BOX(seat_box), seats[i].box, FALSE,
				   FALSE, 0);

		snprintf(text, sizeof(text), _("Seat %d:"), i + 1);
		seats[i].label = gtk_label_new(text);
		gtk_widget_ref(seats[i].label);
		snprintf(text, sizeof(text), "seat%d_lable", i + 1);
		g_object_set_data_full(G_OBJECT(dlg_launch), text,
					 seats[i].label,
					 (GtkDestroyNotify) gtk_widget_unref);
		gtk_widget_show(seats[i].label);
		gtk_box_pack_start(GTK_BOX(seats[i].box), seats[i].label,
				   FALSE, FALSE, 0);

		seats[i].bot = gtk_radio_button_new_with_label(seats[i].group,
							       _("Computer"));
		seats[i].group =
			gtk_radio_button_get_group(GTK_RADIO_BUTTON
					       (seats[i].bot));
		gtk_widget_ref(seats[i].bot);
		snprintf(text, sizeof(text), "seat%d_bot", i + 1);
		g_object_set_data_full(G_OBJECT(dlg_launch), text,
					 seats[i].bot,
					 (GtkDestroyNotify) gtk_widget_unref);
		gtk_widget_show(seats[i].bot);
		gtk_box_pack_start(GTK_BOX(seats[i].box), seats[i].bot, FALSE,
				   FALSE, 3);

		seats[i].open =
			gtk_radio_button_new_with_label(seats[i].group,
							_("Open"));
		seats[i].group =
			gtk_radio_button_get_group(GTK_RADIO_BUTTON
					       (seats[i].open));
		gtk_widget_ref(seats[i].open);
		snprintf(text, sizeof(text), "seat%d_open", i + 1);
		g_object_set_data_full(G_OBJECT(dlg_launch), text,
					 seats[i].open,
					 (GtkDestroyNotify) gtk_widget_unref);
		gtk_widget_show(seats[i].open);
		gtk_box_pack_start(GTK_BOX(seats[i].box), seats[i].open,
				   FALSE, FALSE, 0);

		seats[i].resv =
			gtk_radio_button_new_with_label(seats[i].group,
							_("Reserved for"));
		seats[i].group =
			gtk_radio_button_get_group(GTK_RADIO_BUTTON
					       (seats[i].resv));
		gtk_widget_ref(seats[i].resv);
		snprintf(text, sizeof(text), "seat%d_resv", i + 1);
		g_object_set_data_full(G_OBJECT(dlg_launch), text,
					 seats[i].resv,
					 (GtkDestroyNotify) gtk_widget_unref);
		gtk_widget_show(seats[i].resv);
		gtk_box_pack_start(GTK_BOX(seats[i].box), seats[i].resv,
				   FALSE, FALSE, 0);
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(seats[i].resv),
					     TRUE);

		seats[i].name = gtk_entry_new();
		gtk_entry_set_max_length(GTK_ENTRY(seats[i].name),
					 MAX_RESERVED_NAME_LEN);
		gtk_widget_ref(seats[i].name);
		snprintf(text, sizeof(text), "seat%d_name", i + 1);
		g_object_set_data_full(G_OBJECT(dlg_launch), text,
					 seats[i].name,
					 (GtkDestroyNotify) gtk_widget_unref);
		gtk_widget_show(seats[i].name);
		gtk_box_pack_start(GTK_BOX(seats[i].box), seats[i].name,
				   FALSE, TRUE, 0);
	}

	desc_box = gtk_hbox_new(FALSE, 0);
	gtk_widget_ref(desc_box);
	g_object_set_data_full(G_OBJECT(dlg_launch), "desc_box", desc_box,
				 (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show(desc_box);
	gtk_box_pack_start(GTK_BOX(main_box), desc_box, FALSE, FALSE, 0);

	game_desc_label = gtk_label_new(_("Game Description   "));
	gtk_widget_ref(game_desc_label);
	g_object_set_data_full(G_OBJECT(dlg_launch), "game_desc_label",
				 game_desc_label,
				 (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show(game_desc_label);
	gtk_box_pack_start(GTK_BOX(desc_box), game_desc_label, FALSE, FALSE,
			   0);

	desc_entry = gtk_entry_new();
	gtk_widget_ref(desc_entry);
	g_object_set_data_full(G_OBJECT(dlg_launch), "desc_entry",
				 desc_entry,
				 (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show(desc_entry);
	gtk_box_pack_start(GTK_BOX(desc_box), desc_entry, TRUE, TRUE, 0);

	hbox11 = GTK_DIALOG(dlg_launch)->action_area;
	g_object_set_data(G_OBJECT(dlg_launch), "hbox11", hbox11);
	gtk_widget_show(hbox11);
	gtk_container_set_border_width(GTK_CONTAINER(hbox11), 10);

	button_box = gtk_hbutton_box_new();
	gtk_widget_ref(button_box);
	g_object_set_data_full(G_OBJECT(dlg_launch), "button_box",
				 button_box,
				 (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show(button_box);
	gtk_box_pack_start(GTK_BOX(hbox11), button_box, TRUE, TRUE, 0);

	launch_button = gtk_button_new_with_label(_("Launch"));
	gtk_widget_ref(launch_button);
	g_object_set_data_full(G_OBJECT(dlg_launch), "launch_button",
				 launch_button,
				 (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show(launch_button);
	gtk_container_add(GTK_CONTAINER(button_box), launch_button);
	GTK_WIDGET_SET_FLAGS(launch_button, GTK_CAN_DEFAULT);

	cancel_button = gtk_button_new_from_stock(GTK_STOCK_CANCEL);
	gtk_widget_ref(cancel_button);
	g_object_set_data_full(G_OBJECT(dlg_launch), "cancel_button",
				 cancel_button,
				 (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show(cancel_button);
	gtk_container_add(GTK_CONTAINER(button_box), cancel_button);
	GTK_WIDGET_SET_FLAGS(cancel_button, GTK_CAN_DEFAULT);

	g_signal_connect(GTK_OBJECT(dlg_launch), "destroy",
			   GTK_SIGNAL_FUNC(gtk_widget_destroyed),
			   &launch_dialog);
	g_signal_connect(GTK_OBJECT(dlg_launch), "realize",
			   GTK_SIGNAL_FUNC(launch_fill_defaults), NULL);
	g_signal_connect(GTK_OBJECT(seats_entry), "changed",
			   GTK_SIGNAL_FUNC(launch_seats_changed), NULL);
	for (i = 0; i < num_seats; i++) {
		g_signal_connect(GTK_OBJECT(seats[i].resv), "toggled",
				   GTK_SIGNAL_FUNC(launch_resv_toggle),
				   seats[i].name);
	}
	g_signal_connect(GTK_OBJECT(launch_button), "clicked",
			   GTK_SIGNAL_FUNC(launch_start_game), NULL);
	g_signal_connect(GTK_OBJECT(cancel_button), "clicked",
			   GTK_SIGNAL_FUNC(launch_cancel_button_clicked),
			   NULL);

	return dlg_launch;
}
