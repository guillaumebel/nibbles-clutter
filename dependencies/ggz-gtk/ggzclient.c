/*
 * File: ggzclient.c
 * Author: Justin Zaun
 * Project: GGZ GTK Client
 * $Id$
 *
 * This is the main program body for the GGZ client
 *
 * Copyright (C) 2000-2005 GGZ Development Team.
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
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include <ggz.h>
#include <ggzcore.h>

#include "chat.h"
#include "client.h"
#include "game.h"
#include "ggz-gtk.h"
#include "ggzclient.h"
#include "launch.h"
#include "login.h"
#include "msgbox.h"
#include "motd.h"
#include "playerlist.h"
#include "roomlist.h"
#include "server.h"
#include "support.h"
#include "tablelist.h"
#include "xtext.h"


static guint server_tag, channel_tag;
static gboolean is_server, is_channel;

static gint numrooms;

/* Gdk input callbacks */
static void ggz_input_removed(gpointer data);


static gboolean ggz_check_fd(GIOChannel * source, GIOCondition cond,
			     gpointer data)
{
	gint fd = GPOINTER_TO_INT(data);

	return ggzcore_server_read_data(server, fd) >= 0;
}


static GGZHookReturn ggz_connected(GGZServerEvent id,
				   const void *event_data,
				   const void *user_data)
{
	int fd;

	if (id == GGZ_CONNECTED) {
		GIOChannel *channel;

		/* Add the fd to the gtk main loop */
		ggz_debug("connection", "We're connected.");
		fd = ggzcore_server_get_fd(server);
		assert(!is_server);
		channel = g_io_channel_unix_new(fd);
		server_tag =
		    g_io_add_watch_full(channel, G_PRIORITY_DEFAULT,
					G_IO_IN, ggz_check_fd,
					GINT_TO_POINTER(fd),
					ggz_input_removed);
		g_io_channel_unref(channel);
		is_server = TRUE;
	} else if (id == GGZ_CHANNEL_CONNECTED) {
		GIOChannel *channel;

		/* Add the fd to the gtk main loop */
		ggz_debug("connection", "Direct game channel connected.");
		fd = ggzcore_server_get_channel(server);
		assert(!is_channel);
		channel = g_io_channel_unix_new(fd);
		channel_tag = g_io_add_watch(channel, G_IO_IN,
					     ggz_check_fd,
					     GINT_TO_POINTER(fd));
		g_io_channel_unref(channel);
		is_channel = TRUE;
	}

	return GGZ_HOOK_OK;
}


static GGZHookReturn ggz_connect_fail(GGZServerEvent id,
				      const void *event_data,
				      const void *user_data)
{
	gchar *msg;
	const char *event_message = event_data;

	msg =
	    g_strdup_printf(_("Error connecting to server: %s"),
			    event_message);
	msgbox(msg, _("Error"),
	       MSGBOX_OKONLY, MSGBOX_STOP, MSGBOX_NORMAL);
	g_free(msg);

	login_connect_failed();

	return GGZ_HOOK_OK;
}


static GGZHookReturn ggz_negotiated(GGZServerEvent id,
				    const void *event_data,
				    const void *user_data)
{
	ggzcore_server_login(server);

	return GGZ_HOOK_OK;
}


static GGZHookReturn ggz_channel_ready(GGZGameEvent id,
				       const void *event_data,
				       const void *user_data)
{
	ggz_debug("connection", "Direct game channel ready for game");

	/* Remove channel from gdk input list */
	g_source_remove(channel_tag);
	is_channel = FALSE;

	game_channel_ready();

	return GGZ_HOOK_OK;
}


static GGZHookReturn ggz_auto_join(GGZServerEvent id,
				   const void *event_data,
				   const void *user_data)
{
	int i;
	const int numrooms = ggzcore_server_get_num_rooms(server);
	GGZRoom *lobby = NULL, *supported = NULL, *joinroom = NULL;

	for (i = 0; i < numrooms; i++) {
		GGZRoom *room = ggzcore_server_get_nth_room(server, i);
		GGZGameType *gt = ggzcore_room_get_gametype(room);

		if (ggzcore_room_get_closed(room)) {
			continue;
		}
		if (!gt) {
			lobby = room;
		} else if (can_launch_gametype(gt)) {
			supported = room;
		}
	}

	if (embedded_protocol_engine && supported) {
		joinroom = supported;
		/* For embedded GGZ:
		   first choice is to join a supported room. */
	} else if (lobby) {
		joinroom = lobby;
		/* First choice: join the lobby. */
	} else if (supported) {
		joinroom = supported;
		/* Second choice: join a supported room. */
	} else {
		return GGZ_HOOK_REMOVE;
		/* Otherwise: don't join any room */
	}

	ggzcore_server_join_room(server, joinroom);
	select_room(joinroom);

	return GGZ_HOOK_REMOVE;
}


static GGZHookReturn ggz_logged_in(GGZServerEvent id,
				   const void *event_data,
				   const void *user_data)
{
	const gchar *password;
	gchar *message;

	if (connected_cb) {
		connected_cb(server);
	}

	/* Set title */
	if (!embedded_protocol_engine) {
		gchar *title;

		title = g_strdup_printf("GGZ Gaming Zone - [%s:%d]",
					ggzcore_server_get_host(server),
					ggzcore_server_get_port(server));
		gtk_window_set_title(GTK_WINDOW(main_window), title);
		g_free(title);
	}

	main_activate();
	ggzcore_server_add_event_hook(server, GGZ_ROOM_LIST,
				      ggz_auto_join);
	ggzcore_server_list_gametypes(server, 1);
	ggzcore_server_list_rooms(server, -1, 1);


	/* If this was a first-time login, get the password from the server */
	if (ggzcore_server_get_type(server) == GGZ_LOGIN_NEW) {
		password = ggzcore_server_get_password(server);
		message =
		    g_strdup_printf(_("Your new password is %s"),
				    password);
		msgbox(message, _("New password"), MSGBOX_OKONLY,
		       MSGBOX_INFO, MSGBOX_NORMAL);
		g_free(message);
	}

	return GGZ_HOOK_OK;
}


static GGZHookReturn ggz_login_fail(GGZServerEvent id,
				    const void *event_data,
				    const void *user_data)
{
	const GGZErrorEventData *error = event_data;

	login_failed(error);

	return GGZ_HOOK_OK;
}

static void client_update_one_room(const GGZRoom *room)
{
	if (room) {
		update_one_room(room);
	}
}

static GGZHookReturn ggz_num_players_changed(GGZServerEvent id,
					     const void *event_data,
					     const void *user_data)
{
	GtkWidget *serverbar = ggz_lookup_widget(win_main, "serverbar");
	guint context;
	int players = ggzcore_server_get_num_players(server);
	char buf[128];

	snprintf(buf, sizeof(buf), _("Players on server: %d"), players);
	context = gtk_statusbar_get_context_id(GTK_STATUSBAR(serverbar),
					       "players");
	gtk_statusbar_pop(GTK_STATUSBAR(serverbar), context);
	gtk_statusbar_push(GTK_STATUSBAR(serverbar), context, buf);

	return GGZ_HOOK_OK;
}

static GGZHookReturn ggz_num_rooms_changed(GGZServerEvent id,
					     const void *event_data,
					     const void *user_data)
{
	update_room_list();
	return GGZ_HOOK_OK;
}

static GGZHookReturn ggz_entered(GGZServerEvent id, const void *event_data,
				 const void *user_data)
{
	GtkWidget *tmp;
	gchar *name;
	gchar *message;
	GGZRoom *room;
	GGZGameType *gt;

	/* Close any launch dialog that's currently open. */
	launch_dialog_close();

	/* Clear the player list */
	/* We do this here so that on slower links people
	 * don't think all the old people are in the old room */
	clear_player_list();
	clear_table_list();

	/* Get player list */
	/* FIXME: Player list should use the ggz update system */
	ggzcore_room_list_tables(ggzcore_server_get_cur_room(server), -1,
				 0);
	ggzcore_room_list_players(ggzcore_server_get_cur_room(server));


	/* Set the room label to current room */
	tmp = ggz_lookup_widget(win_main, "Current_room_label");
	name =
	    g_strdup_printf(_("Current Room: %s"),
			    ggzcore_room_get_name
			    (ggzcore_server_get_cur_room(server)));
	gtk_label_set_text(GTK_LABEL(tmp), name);
	g_free(name);

	/* Display message in chat area */
	message =
	    g_strdup_printf(_("You've joined room \"%s\"."),
			    ggzcore_room_get_name
			    (ggzcore_server_get_cur_room(server)));
	chat_display_local(CHAT_LOCAL_NORMAL, NULL, message);
	g_free(message);
	chat_display_local(CHAT_LOCAL_NORMAL, NULL,
			   ggzcore_room_get_desc
			   (ggzcore_server_get_cur_room(server)));

	/* Check what the current game type is */
	room = ggzcore_server_get_cur_room(server);
	gt = ggzcore_room_get_gametype(room);

	if (ggzcore_gametype_get_name(gt) == NULL) {
		tmp = ggz_lookup_widget(win_main, "table_vpaned");
		if (GTK_PANED(tmp)->child1_size != 0)
			g_object_set(G_OBJECT(tmp), "user_data",
				     GTK_PANED(tmp)->child1_size, NULL);
		gtk_paned_set_position(GTK_PANED(tmp), 0);
		tmp = ggz_lookup_widget(win_main, "launch_button");
		gtk_widget_set_sensitive(tmp, FALSE);
		tmp = ggz_lookup_widget(win_main, "join_button");
		gtk_widget_set_sensitive(tmp, FALSE);
		tmp = ggz_lookup_widget(win_main, "watch_button");
		gtk_widget_set_sensitive(tmp, FALSE);
	} else {
		int height;
		tmp = ggz_lookup_widget(win_main, "table_vpaned");
		g_object_get(G_OBJECT(tmp), "user_data", &height, NULL);
		gtk_paned_set_position(GTK_PANED(tmp), height);
		tmp = ggz_lookup_widget(win_main, "launch_button");
		gtk_widget_set_sensitive(tmp, TRUE);
		tmp = ggz_lookup_widget(win_main, "join_button");
		gtk_widget_set_sensitive(tmp, TRUE);
		tmp = ggz_lookup_widget(win_main, "watch_button");
		gtk_widget_set_sensitive(tmp, TRUE);
	}

	return GGZ_HOOK_OK;
}


static GGZHookReturn ggz_entered_fail(GGZServerEvent id,
				      const void *event_data,
				      const void *user_data)
{
	gchar *msg;
	const char *event_message = event_data;

	msg =
	    g_strdup_printf(_("Error joining room: %s"),
			    event_message);
	msgbox(msg, _("Error"),
	       MSGBOX_OKONLY, MSGBOX_STOP, MSGBOX_NORMAL);
	g_free(msg);

	return GGZ_HOOK_OK;
}


static GGZHookReturn ggz_logout(GGZServerEvent id, const void *event_data,
				const void *user_data)
{
	ggz_debug("connection", "Logged out.");

	if (ggzcore_server_get_state(server) != GGZ_STATE_RECONNECTING) {
		server_disconnect();
	}

	/* set title */
	if (!embedded_protocol_version) {
		gtk_window_set_title(GTK_WINDOW(main_window),
				     _("GGZ Gaming Zone"));
	}

	return GGZ_HOOK_OK;
}


static GGZHookReturn ggz_chat(GGZRoomEvent id, const void *event_data,
			      const void *user_data)
{
	const GGZChatEventData *chat_data = event_data;

	chat_display_server(chat_data->type,
			    chat_data->sender, chat_data->message);

	return GGZ_HOOK_OK;
}


static GGZHookReturn ggz_chat_fail(GGZRoomEvent id, const void *event_data,
				   const void *user_data)
{
	const GGZErrorEventData *error = event_data;
	char buf[512];

	switch (error->status) {
	case E_NOT_IN_ROOM:
		chat_display_local(CHAT_LOCAL_NORMAL, NULL,
				   _
				   ("You can't chat while not in a room."));
		break;
	case E_NO_PERMISSION:
		chat_display_local(CHAT_LOCAL_NORMAL, NULL,
				   _
				   ("You don't have permission to chat here."));
		break;
	case E_AT_TABLE:
		chat_display_local(CHAT_LOCAL_NORMAL, NULL,
				   _("No private chatting at a table!"));
		break;
	case E_USR_LOOKUP:
		chat_display_local(CHAT_LOCAL_NORMAL, NULL,
				   _("That player isn't in the room!"));
		break;
	case E_BAD_OPTIONS:
		chat_display_local(CHAT_LOCAL_NORMAL, NULL,
				   _
				   ("There was an error sending the chat."));
		break;
	case E_NO_TABLE:
		chat_display_local(CHAT_LOCAL_NORMAL, NULL,
				   _("You're not at a table."));
		break;
	default:
		snprintf(buf, sizeof(buf),
			 _("Chat failed: %s."), error->message);
		chat_display_local(CHAT_LOCAL_NORMAL, NULL, buf);
		break;
	}

	return GGZ_HOOK_OK;
}


static GGZHookReturn ggz_list_players(GGZRoomEvent id,
				      const void *event_data,
				      const void *user_data)
{
	const GGZRoom *room = event_data;

	update_player_list();
	client_update_one_room(room);
	return GGZ_HOOK_OK;
}

static GGZHookReturn ggz_info_player(GGZRoomEvent id,
				     const void *event_data,
				     const void *user_data)
{
	update_player_list();
	return GGZ_HOOK_OK;
}

static GGZHookReturn ggz_player_count(GGZRoomEvent id,
				      const void *event_data,
				      const void *user_data)
{
	const GGZRoom *room = event_data;

	client_update_one_room(room);
	return GGZ_HOOK_OK;
}

static GGZHookReturn ggz_room_enter(GGZRoomEvent id,
				    const void *event_data,
				    const void *user_data)
{
	const GGZRoomChangeEventData *data = event_data;

	client_update_one_room(data->to_room);
	client_update_one_room(data->from_room);
	update_player_list();
	chat_enter(data->player_name, data->rooms_known, data->from_room);

	return GGZ_HOOK_OK;
}


static GGZHookReturn ggz_room_leave(GGZRoomEvent id,
				    const void *event_data,
				    const void *user_data)
{
	const GGZRoomChangeEventData *data = event_data;

	client_update_one_room(data->to_room);
	client_update_one_room(data->from_room);
	update_player_list();
	chat_part(data->player_name, data->rooms_known, data->to_room);

	return GGZ_HOOK_OK;
}


static GGZHookReturn ggz_list_tables(GGZRoomEvent id,
				     const void *event_data,
				     const void *user_data)
{
	update_table_list();
	return GGZ_HOOK_OK;
}


static GGZHookReturn ggz_table_update(GGZRoomEvent id,
				      const void *event_data,
				      const void *user_data)
{
	update_table_list();
	update_player_list();
	return GGZ_HOOK_OK;
}


static GGZHookReturn ggz_table_launched(GGZRoomEvent id,
					const void *event_data,
					const void *user_data)
{
	/*output_text("-- Table launched"); */
	return GGZ_HOOK_OK;
}


static GGZHookReturn ggz_table_launch_fail(GGZRoomEvent id,
					   const void *event_data,
					   const void *user_data)
{
	gchar *msg;
	const char *event_message = event_data;

	msg =
	    g_strdup_printf(_("Error launching table: %s"),
			    event_message);
	msgbox(msg, _("Error"),
	       MSGBOX_OKONLY, MSGBOX_STOP, MSGBOX_NORMAL);
	g_free(msg);

	game_quit();

	return GGZ_HOOK_OK;
}


static GGZHookReturn ggz_table_joined(GGZRoomEvent id,
				      const void *event_data,
				      const void *user_data)
{
	const int *table_id = event_data;

	char message[1024];
	snprintf(message, sizeof(message),
		 _("You have joined table %d."), *table_id);
	chat_display_local(CHAT_LOCAL_NORMAL, NULL, message);

	return GGZ_HOOK_OK;
}


static GGZHookReturn ggz_table_join_fail(GGZRoomEvent id,
					 const void *event_data,
					 const void *user_data)
{
	gchar *msg;
	const char *event_message = event_data;

	msg =
	    g_strdup_printf(_("Error joining table: %s"),
			    event_message);
	msgbox(msg, _("Error"),
	       MSGBOX_OKONLY, MSGBOX_STOP, MSGBOX_NORMAL);
	g_free(msg);

	game_quit();

	return GGZ_HOOK_OK;
}


static GGZHookReturn ggz_table_left(GGZRoomEvent id,
				    const void *event_data,
				    const void *user_data)
{
	const GGZTableLeaveEventData *data = event_data;
	char message[1024] = "???";

	switch (data->reason) {
	case GGZ_LEAVE_BOOT:
		snprintf(message, sizeof(message),
			 _("You have been booted from the table by %s."),
			 data->player);
		break;
	case GGZ_LEAVE_NORMAL:
		snprintf(message, sizeof(message),
			 _("You have left the table."));
		break;
	case GGZ_LEAVE_GAMEOVER:
		snprintf(message, sizeof(message), _("The game is over."));
		break;
	case GGZ_LEAVE_GAMEERROR:
		snprintf(message, sizeof(message),
			 _("There was an error with the game server."));
		break;
	}
	chat_display_local(CHAT_LOCAL_NORMAL, NULL, message);

	game_quit();

	return GGZ_HOOK_OK;
}


static GGZHookReturn ggz_table_leave_fail(GGZRoomEvent id,
					  const void *event_data,
					  const void *user_data)
{
	gchar *msg;
	const char *event_message = event_data;

	msg =
	    g_strdup_printf(_("Error leaving table: %s"),
			    event_message);
	msgbox(msg, _("Error"),
	       MSGBOX_OKONLY, MSGBOX_STOP, MSGBOX_NORMAL);
	g_free(msg);

	return GGZ_HOOK_OK;
}


static GGZHookReturn ggz_room_list(GGZServerEvent id,
				   const void *event_data,
				   const void *user_data)
{
	GGZRoom *room;
	gint i;

	/* Display current list of rooms */
	numrooms = ggzcore_server_get_num_rooms(server);

	update_room_list();

	for (i = 0; i < numrooms; i++) {
		room = ggzcore_server_get_nth_room(server, i);

		/* Hookup the chat functions to the new room */
		ggzcore_room_add_event_hook(room, GGZ_CHAT_EVENT,
					    ggz_chat);
		ggzcore_room_add_event_hook(room, GGZ_PLAYER_LIST,
					    ggz_list_players);
		ggzcore_room_add_event_hook(room, GGZ_PLAYER_COUNT,
					    ggz_player_count);
		ggzcore_room_add_event_hook(room, GGZ_TABLE_LIST,
					    ggz_list_tables);
		ggzcore_room_add_event_hook(room, GGZ_ROOM_ENTER,
					    ggz_room_enter);
		ggzcore_room_add_event_hook(room, GGZ_ROOM_LEAVE,
					    ggz_room_leave);
		ggzcore_room_add_event_hook(room, GGZ_TABLE_LAUNCHED,
					    ggz_table_launched);
		ggzcore_room_add_event_hook(room, GGZ_TABLE_LAUNCH_FAIL,
					    ggz_table_launch_fail);
		ggzcore_room_add_event_hook(room, GGZ_TABLE_JOINED,
					    ggz_table_joined);
		ggzcore_room_add_event_hook(room, GGZ_TABLE_JOIN_FAIL,
					    ggz_table_join_fail);
		ggzcore_room_add_event_hook(room, GGZ_TABLE_LEFT,
					    ggz_table_left);
		ggzcore_room_add_event_hook(room, GGZ_TABLE_LEAVE_FAIL,
					    ggz_table_leave_fail);
		ggzcore_room_add_event_hook(room, GGZ_TABLE_UPDATE,
					    ggz_table_update);
		ggzcore_room_add_event_hook(room, GGZ_PLAYER_LAG,
					    ggz_info_player);
		ggzcore_room_add_event_hook(room, GGZ_PLAYER_STATS,
					    ggz_info_player);
	}

	return GGZ_HOOK_OK;
}


void ggz_sensitivity_init(void)
{
	GtkWidget *tmp;

	/* set senditivity */
	/* Menu bar */

	/* Set the room label to current room */
	tmp = ggz_lookup_widget(win_main, "Current_room_label");
	gtk_label_set_text(GTK_LABEL(tmp), _("Current Room:"));
}

static GGZHookReturn ggz_motd_loaded(GGZServerEvent id,
				     const void *event_data,
				     const void *user_data)
{
	const GGZMotdEventData *motd = event_data;

	motd_create_or_raise();
	motd_print_line(motd->motd);

	return GGZ_HOOK_OK;
}


static GGZHookReturn ggz_state_change(GGZServerEvent id,
				      const void *event_data,
				      const void *user_data)
{
	guint context;
	GtkWidget *statebar;
	char *state = _("**none**");
	GGZStateID state_id;

	state_id = ggzcore_server_get_state(server);

	switch (state_id) {
	case GGZ_STATE_OFFLINE:
		state = _("Offline");
		break;
	case GGZ_STATE_CONNECTING:
		state = _("Connecting");
		break;
	case GGZ_STATE_RECONNECTING:
		state = _("Reconnecting");
		break;
	case GGZ_STATE_ONLINE:
		state = _("Online");
		break;
	case GGZ_STATE_LOGGING_IN:
		state = _("Logging In");
		break;
	case GGZ_STATE_LOGGED_IN:
		state = _("Logged In");
		break;
	case GGZ_STATE_BETWEEN_ROOMS:
	case GGZ_STATE_ENTERING_ROOM:
		state = _("--> Room");
		break;
	case GGZ_STATE_IN_ROOM:
		state = _("Chatting");
		break;
	case GGZ_STATE_LAUNCHING_TABLE:
	case GGZ_STATE_JOINING_TABLE:
		state = _("--> Table");
		break;
	case GGZ_STATE_AT_TABLE:
		state = _("Playing");
		break;
	case GGZ_STATE_LEAVING_TABLE:
		state = _("<-- Table");
		break;
	case GGZ_STATE_LOGGING_OUT:
		state = _("Logging Out");
		break;
	}

	statebar = ggz_lookup_widget(win_main, "statebar");
	context =
	    gtk_statusbar_get_context_id(GTK_STATUSBAR(statebar), "state");
	gtk_statusbar_pop(GTK_STATUSBAR(statebar), context);
	gtk_statusbar_push(GTK_STATUSBAR(statebar), context, state);

	return GGZ_HOOK_OK;
}


static GGZHookReturn ggz_state_sensitivity(GGZServerEvent id,
					   const void *event_data,
					   const void *user_data)
{
	GtkWidget *tmp;
	GGZStateID state_id;

	state_id = ggzcore_server_get_state(server);

	switch (state_id) {
	case GGZ_STATE_OFFLINE:
	case GGZ_STATE_RECONNECTING:
		/* Re-enable connect button */
		if (login_dialog) {
			tmp =
			    ggz_lookup_widget(login_dialog, "connect_button");
			gtk_widget_set_sensitive(tmp, TRUE);
		}

		/* Menu bar */
		tmp = ggz_lookup_widget(win_main, "connect");
		gtk_widget_set_sensitive(tmp, TRUE);

		tmp = ggz_lookup_widget(win_main, "disconnect");
		gtk_widget_set_sensitive(tmp, FALSE);

		tmp = ggz_lookup_widget(win_main, "game");
		gtk_widget_set_sensitive(tmp, FALSE);

		tmp = ggz_lookup_widget(win_main, "view");
		gtk_widget_set_sensitive(tmp, FALSE);

		/* Tool bar */
		tmp = ggz_lookup_widget(win_main, "disconnect_button");
		gtk_widget_set_sensitive(tmp, FALSE);

		tmp = ggz_lookup_widget(win_main, "launch_button");
		gtk_widget_set_sensitive(tmp, FALSE);

		tmp = ggz_lookup_widget(win_main, "join_button");
		gtk_widget_set_sensitive(tmp, FALSE);

		tmp = ggz_lookup_widget(win_main, "watch_button");
		gtk_widget_set_sensitive(tmp, FALSE);

		tmp = ggz_lookup_widget(win_main, "leave_button");
		gtk_widget_set_sensitive(tmp, FALSE);

#ifdef STATS_BUTTON
		tmp = ggz_lookup_widget(win_main, "stats_button");
		gtk_widget_set_sensitive(tmp, FALSE);
#endif

		/* Client area */
		sensitize_room_list(FALSE);
		sensitize_player_list(FALSE);
		sensitize_table_list(FALSE);

		tmp = ggz_lookup_widget(win_main, "chat_entry");
		gtk_widget_set_sensitive(tmp, FALSE);

		tmp = ggz_lookup_widget(win_main, "send_button");
		gtk_widget_set_sensitive(tmp, FALSE);
		break;

	case GGZ_STATE_CONNECTING:
		if (login_dialog) {
			/* Desensitize the connect button */
			tmp =
			    ggz_lookup_widget(login_dialog, "connect_button");
			gtk_widget_set_sensitive(tmp, FALSE);
		}

		/* Menu bar */
		tmp = ggz_lookup_widget(win_main, "connect");
		gtk_widget_set_sensitive(tmp, FALSE);

		tmp = ggz_lookup_widget(win_main, "disconnect");
		gtk_widget_set_sensitive(tmp, TRUE);

		/* Tool bar */
		tmp = ggz_lookup_widget(win_main, "disconnect_button");
		gtk_widget_set_sensitive(tmp, TRUE);
		break;

	case GGZ_STATE_ONLINE:
		break;

	case GGZ_STATE_LOGGING_IN:
		if (login_dialog) {
			/* Desensitize the connect button */
			tmp =
			    ggz_lookup_widget(login_dialog, "connect_button");
			gtk_widget_set_sensitive(tmp, FALSE);
		}

		break;

	case GGZ_STATE_LOGGED_IN:
		/* set senditivity */
		/* Menu bar */
		tmp = ggz_lookup_widget(win_main, "view");
		gtk_widget_set_sensitive(tmp, TRUE);

		/* Tool bar */
#ifdef STATS_BUTTON
		tmp = ggz_lookup_widget(win_main, "stats_button");
		gtk_widget_set_sensitive(tmp, TRUE);
#endif

		/* Client area */
		sensitize_room_list(TRUE);
		break;

	case GGZ_STATE_BETWEEN_ROOMS:
	case GGZ_STATE_ENTERING_ROOM:
		tmp = ggz_lookup_widget(win_main, "chat_entry");
		gtk_widget_set_sensitive(tmp, FALSE);

		tmp = ggz_lookup_widget(win_main, "send_button");
		gtk_widget_set_sensitive(tmp, FALSE);
		break;

	case GGZ_STATE_IN_ROOM:
		/* Menu bar */
		tmp = ggz_lookup_widget(win_main, "game");
		gtk_widget_set_sensitive(tmp, TRUE);

		tmp = ggz_lookup_widget(win_main, "launch");
		gtk_widget_set_sensitive(tmp, TRUE);

		tmp = ggz_lookup_widget(win_main, "join");
		gtk_widget_set_sensitive(tmp, TRUE);

		/* Tool bar */
		tmp = ggz_lookup_widget(win_main, "launch_button");
		gtk_widget_set_sensitive(tmp, TRUE);

		tmp = ggz_lookup_widget(win_main, "join_button");
		gtk_widget_set_sensitive(tmp, TRUE);

		tmp = ggz_lookup_widget(win_main, "watch_button");
		gtk_widget_set_sensitive(tmp, TRUE);

		/* Client area */
		sensitize_room_list(TRUE);
		sensitize_player_list(TRUE);
		sensitize_table_list(TRUE);

		tmp = ggz_lookup_widget(win_main, "chat_entry");
		gtk_widget_set_sensitive(tmp, TRUE);

		tmp = ggz_lookup_widget(win_main, "send_button");
		gtk_widget_set_sensitive(tmp, TRUE);
		break;

	case GGZ_STATE_LAUNCHING_TABLE:
		/* Tool bar */
		tmp = ggz_lookup_widget(win_main, "launch_button");
		gtk_widget_set_sensitive(tmp, FALSE);

		tmp = ggz_lookup_widget(win_main, "join_button");
		gtk_widget_set_sensitive(tmp, FALSE);

		tmp = ggz_lookup_widget(win_main, "watch_button");
		gtk_widget_set_sensitive(tmp, FALSE);

		/* Menu bar */
		tmp = ggz_lookup_widget(win_main, "launch");
		gtk_widget_set_sensitive(tmp, FALSE);

		tmp = ggz_lookup_widget(win_main, "join");
		gtk_widget_set_sensitive(tmp, FALSE);
		break;

	case GGZ_STATE_JOINING_TABLE:
		/* Tool bar */
		tmp = ggz_lookup_widget(win_main, "launch_button");
		gtk_widget_set_sensitive(tmp, FALSE);

		tmp = ggz_lookup_widget(win_main, "join_button");
		gtk_widget_set_sensitive(tmp, FALSE);

		tmp = ggz_lookup_widget(win_main, "watch_button");
		gtk_widget_set_sensitive(tmp, FALSE);

		/* Menu bar */
		tmp = ggz_lookup_widget(win_main, "launch");
		gtk_widget_set_sensitive(tmp, FALSE);

		tmp = ggz_lookup_widget(win_main, "join");
		gtk_widget_set_sensitive(tmp, FALSE);
		break;

	case GGZ_STATE_AT_TABLE:
		/* Tool bar */
		tmp = ggz_lookup_widget(win_main, "leave_button");
		gtk_widget_set_sensitive(tmp, TRUE);

		/* Menu bar */
		tmp = ggz_lookup_widget(win_main, "leave");
		gtk_widget_set_sensitive(tmp, TRUE);
		break;

	case GGZ_STATE_LEAVING_TABLE:
		/* Tool bar */
		tmp = ggz_lookup_widget(win_main, "leave_button");
		gtk_widget_set_sensitive(tmp, FALSE);

		/* Menu bar */
		tmp = ggz_lookup_widget(win_main, "leave");
		gtk_widget_set_sensitive(tmp, FALSE);
		break;

	case GGZ_STATE_LOGGING_OUT:
		/* Menu bar */
		tmp = ggz_lookup_widget(win_main, "disconnect");
		gtk_widget_set_sensitive(tmp, FALSE);

		tmp = ggz_lookup_widget(win_main, "game");
		gtk_widget_set_sensitive(tmp, FALSE);

		tmp = ggz_lookup_widget(win_main, "view");
		gtk_widget_set_sensitive(tmp, FALSE);

		/* Tool bar */
		tmp = ggz_lookup_widget(win_main, "disconnect_button");
		gtk_widget_set_sensitive(tmp, FALSE);

		tmp = ggz_lookup_widget(win_main, "launch_button");
		gtk_widget_set_sensitive(tmp, FALSE);

		tmp = ggz_lookup_widget(win_main, "join_button");
		gtk_widget_set_sensitive(tmp, FALSE);

		tmp = ggz_lookup_widget(win_main, "watch_button");
		gtk_widget_set_sensitive(tmp, FALSE);

		tmp = ggz_lookup_widget(win_main, "leave_button");
		gtk_widget_set_sensitive(tmp, FALSE);

#ifdef STATS_BUTTON
		tmp = ggz_lookup_widget(win_main, "stats_button");
		gtk_widget_set_sensitive(tmp, FALSE);
#endif

		/* Client area */
		sensitize_room_list(FALSE);
		sensitize_player_list(FALSE);
		sensitize_table_list(FALSE);

		tmp = ggz_lookup_widget(win_main, "chat_entry");
		gtk_widget_set_sensitive(tmp, FALSE);

		tmp = ggz_lookup_widget(win_main, "send_button");
		gtk_widget_set_sensitive(tmp, FALSE);
		break;
	}

	return GGZ_HOOK_OK;
}


static GGZHookReturn ggz_server_error(GGZServerEvent id,
				      const void *event_data,
				      const void *user_data)
{
	gchar *msg;
	const char *event_message = event_data;

	ggz_debug("connection", "Server error.");

	if (ggzcore_server_get_state(server) != GGZ_STATE_RECONNECTING) {
		server_disconnect();
	} else {
		g_source_remove(server_tag);
		is_server = FALSE;
	}

	/* Should we clear the list of rooms/players/tables? */

	if (ggzcore_server_get_state(server) != GGZ_STATE_RECONNECTING) {
		msg = g_strdup_printf(_("Server error: %s"), event_message);
		msgbox(msg, _("Error"),
		       MSGBOX_OKONLY, MSGBOX_STOP, MSGBOX_NORMAL);
		g_free(msg);
	}

	return GGZ_HOOK_OK;
}

static GGZHookReturn ggz_net_error(GGZServerEvent id,
				   const void *event_data,
				   const void *user_data)
{
	ggz_debug("connection", "Net error.");

	server_disconnect();

	clear_room_list();
	clear_player_list();
	clear_table_list();

	return GGZ_HOOK_OK;
}


/* The below function are used to
 * add and remove servers to the 
 * main loop
 */

/* GdkDestroyNotify function for server fd */
static void ggz_input_removed(gpointer data)
{
	if (!server) {
		/* This function should only be called once when we
		   disconnect; calling it more than once would attempt to
		   free the server twice.  See server_disconnect().  It's
		   possible this could cause some errors since the
		   ggzcore_server_free() doesn't get called immediately.
		   The alternative is to put this code into
		   server_disconnect(), and drop this function
		   entirely.

		   Actually, this function can be called after the server
		   data has already been deallocated: if ggzd spontaneously
		   shuts down, for instance.  Just one more but of ugliness...

		   --JDS */
		return;
	}

	if (ggzcore_server_get_state(server) == GGZ_STATE_RECONNECTING) {
		return;
	}

	ggzcore_server_free(server);
	server = NULL;
}


int ggz_connection_query(void)
{
	return is_server;
}

/* Handle the ggz-gtk end of disconnecting. */
void server_disconnect(void)
{
	assert(server);

	/* This can happen if we get disconnected from a server that we never
	   actually connected to (for instance if there's no server listening
	   on the host/port we connected to).  We still have to free the
	   server variable, but not remove the input handler.  This is ugly.
	   See ggz_input_removed(). */
	if (!is_server && server) {
		ggzcore_server_free(server);
		server = NULL;
		return;
	}

	/* Removing the input handler prompts the calling of 
	   ggz_input_removed, which frees the server data.   This is
	   a bit kludgy, but... */
	g_source_remove(server_tag);
	is_server = FALSE;

	chat_display_local(CHAT_LOCAL_HIGH, NULL,
			   _("Disconnected from server."));
	main_activate();
	if (connected_cb) {
		connected_cb(NULL);
	}
}


void ggz_event_init(GGZServer * Server)
{
	ggzcore_server_add_event_hook(Server,
				      GGZ_CONNECTED, ggz_connected);
	ggzcore_server_add_event_hook(Server,
				      GGZ_CONNECT_FAIL, ggz_connect_fail);
	ggzcore_server_add_event_hook(Server,
				      GGZ_NEGOTIATED, ggz_negotiated);
	ggzcore_server_add_event_hook(Server,
				      GGZ_NEGOTIATE_FAIL,
				      ggz_connect_fail);
	ggzcore_server_add_event_hook(Server, GGZ_LOGGED_IN,
				      ggz_logged_in);
	ggzcore_server_add_event_hook(Server, GGZ_LOGIN_FAIL,
				      ggz_login_fail);
	ggzcore_server_add_event_hook(Server, GGZ_MOTD_LOADED,
				      ggz_motd_loaded);
	ggzcore_server_add_event_hook(Server, GGZ_ROOM_LIST,
				      ggz_room_list);
	/* Unused: GGZ_TYPE_LIST */
	ggzcore_server_add_event_hook(Server, GGZ_SERVER_PLAYERS_CHANGED,
				      ggz_num_players_changed);
	ggzcore_server_add_event_hook(Server, GGZ_SERVER_ROOMS_CHANGED,
				      ggz_num_rooms_changed);
	ggzcore_server_add_event_hook(Server, GGZ_ENTERED, ggz_entered);
	ggzcore_server_add_event_hook(Server,
				      GGZ_ENTER_FAIL, ggz_entered_fail);
	ggzcore_server_add_event_hook(Server, GGZ_LOGOUT, ggz_logout);
	ggzcore_server_add_event_hook(Server, GGZ_NET_ERROR,
				      ggz_net_error);
	ggzcore_server_add_event_hook(Server, GGZ_PROTOCOL_ERROR,
				      ggz_server_error);
	ggzcore_server_add_event_hook(Server, GGZ_CHAT_FAIL,
				      ggz_chat_fail);
	ggzcore_server_add_event_hook(Server, GGZ_STATE_CHANGE,
				      ggz_state_change);
	ggzcore_server_add_event_hook(Server, GGZ_STATE_CHANGE,
				      ggz_state_sensitivity);
	ggzcore_server_add_event_hook(Server, GGZ_CHANNEL_CONNECTED,
				      ggz_connected);
	ggzcore_server_add_event_hook(Server, GGZ_CHANNEL_READY,
				      ggz_channel_ready);
	ggzcore_server_add_event_hook(Server, GGZ_CHANNEL_FAIL,
				      ggz_connect_fail);
}
