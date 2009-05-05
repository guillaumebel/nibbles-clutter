/*
 * File: game.c
 * Author: Brent Hendricks
 * Project: GGZ Text Client 
 * Date: 3/1/01
 * $Id$
 *
 * Functions for handling game events
 *
 * Copyright (C) 2000 Brent Hendricks.
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

#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> /* For strcasecmp */

#include <ggzcore.h>

#include "game.h"
#include "chat.h"
#include "client.h"
#include "launch.h"
#include "msgbox.h"
#include "pick_module.h"
#include "server.h"
#include "support.h"

#include <ggz.h>	/* libggz */

static GGZGame *game;
static int fd = -1;
static guint game_tag;

static GGZModule *pick_module(GGZGameType * gt)
{
	const char *name = ggzcore_gametype_get_name(gt);
	const char *engine = ggzcore_gametype_get_prot_engine(gt);
	const char *version = ggzcore_gametype_get_prot_version(gt);
	int i, j, preserve;
	GGZModule *module;
	GGZModule **frontends;
	int *modulenumbers;
	char *preferred;
	int num;
	GGZModuleEnvironment env;

	/* Check how many modules are registered for this game type */
	ggzcore_reload();
	num = ggzcore_module_get_num_by_type(name, engine, version);

	if (num == 0) {
		gchar *message;
		message = g_strdup_printf(_("You don't have this game "
					    "installed. You can download\n"
					    "it from %s."),
					  ggzcore_gametype_get_url(gt));
		msgbox(message, _("Launch Error"), MSGBOX_OKONLY,
		       MSGBOX_STOP, MSGBOX_NORMAL);
		g_free(message);

		return NULL;
	}

	/* If there's only one choice, use it regardless of frontend */
	if (num == 1)
		return ggzcore_module_get_nth_by_type(name, engine,
						      version, 0);

	/* See if the user has a listed preference. */
	preferred = ggzcore_conf_read_string("GAME", name, NULL);
	if (preferred) {
		for (i = 0; i < num; i++) {
			const char *frontend;
			module =
			    ggzcore_module_get_nth_by_type(name, engine,
							   version, i);
			frontend = ggzcore_module_get_frontend(module);
			if (strcasecmp(preferred, frontend) == 0) {
				ggz_debug("modules",
					  "User preferred %s frontend for %s",
					  frontend, name);
				return module;
			}
		}
	}

	/* More than one frontend: pop up a window and let the player
	   choose. */
	frontends = ggz_malloc((num + 1) * sizeof(*frontends));
	modulenumbers = ggz_malloc((num + 1) * sizeof(int));

	j = 0;
	for (i = 0; i < num; i++) {
		module = ggzcore_module_get_nth_by_type(name, engine,
							version, i);
		env = ggzcore_module_get_environment(module);
		if ((env == GGZ_ENVIRONMENT_XWINDOW)
		    || (env == GGZ_ENVIRONMENT_XFULLSCREEN)) {
			frontends[j] = module;
			modulenumbers[j] = i;
			j++;
		}
	}
	frontends[j] = NULL;

	i = ask_user_to_pick_module(frontends, modulenumbers, &preserve);
	if (i < 0)
		return NULL;
	module = ggzcore_module_get_nth_by_type(name, engine, version, i);

	ggz_free(frontends);
	ggz_free(modulenumbers);

	if (preserve) {
		const char *frontend = ggzcore_module_get_frontend(module);
		ggzcore_conf_write_string("GAME", name, frontend);
		ggzcore_conf_commit();
	}

	return module;
}

int game_launch(void)
{
	gint status;

	/* Launch game */
	if ((status = ggzcore_game_launch(game) < 0)) {
		msgbox(_("Failed to execute game module.\n Launch aborted."),
		       _("Launch Error"), MSGBOX_OKONLY, MSGBOX_STOP,
		       MSGBOX_NORMAL);
		game_destroy();
		return -1;
	}

	return 0;
}


void game_channel_ready(void)
{
	ggzcore_game_set_server_fd(game,
				   ggzcore_server_get_channel(server));
}


void game_quit(void)
{
	if (fd != -1) {
		g_source_remove(game_tag);
		fd = -1;
	}
}


void game_destroy(void)
{
	ggzcore_game_free(ggzcore_server_get_cur_game(server));
	game = NULL;
}


static gboolean game_process(GIOChannel * source, GIOCondition condition,
			     gpointer data)
{
	return (game && ggzcore_game_read_data(game) >= 0);
}


/* GdkDestroyNotify function for server fd */
static void game_input_removed(gpointer data)
{
	game_destroy();
}


static GGZHookReturn game_launched(GGZGameEvent id, const void *event_data,
				   const void *user_data)
{
	GIOChannel *channel;

	chat_display_local(CHAT_LOCAL_NORMAL, NULL, _("Launched game"));

	fd = ggzcore_game_get_control_fd(game);
	channel = g_io_channel_unix_new(fd);
	game_tag = g_io_add_watch_full(channel, G_PRIORITY_DEFAULT,
				       G_IO_IN, game_process, server,
				       game_input_removed);
	g_io_channel_unref(channel);

	if (launched_cb) {
		launched_cb();
	}

	return GGZ_HOOK_OK;
}


static GGZHookReturn game_launch_fail(GGZGameEvent id,
				      const void *event_data,
				      const void *user_data)
{
	chat_display_local(CHAT_LOCAL_HIGH, NULL, _("Launch failed"));

	launch_dialog_close();

	return GGZ_HOOK_OK;
}


static GGZHookReturn game_negotiated(GGZGameEvent id,
				     const void *event_data,
				     const void *user_data)
{
	ggz_debug("game", "Game module ready (creating channel)");

#ifndef HAVE_WINSOCK_H
	ggzcore_server_create_channel(server);
#endif

	return GGZ_HOOK_OK;
}


static GGZHookReturn game_negotiate_fail(GGZGameEvent id,
					 const void *event_data,
					 const void *user_data)
{
	return GGZ_HOOK_OK;
}


static GGZHookReturn game_playing(GGZGameEvent id, const void *event_data,
				  const void *user_data)
{
	ggz_debug("game",
		  "Game module connected to server over game channel");
	if (launch_in_process())
		launch_table();
	else
		client_join_table();

	return GGZ_HOOK_OK;
}


int game_play(void)
{
	if (fd != -1)
		return TRUE;
	return FALSE;
}

static void game_register(GGZGame * game)
{
	ggzcore_game_add_event_hook(game, GGZ_GAME_LAUNCHED,
				    game_launched);
	ggzcore_game_add_event_hook(game, GGZ_GAME_LAUNCH_FAIL,
				    game_launch_fail);
	ggzcore_game_add_event_hook(game, GGZ_GAME_NEGOTIATED,
				    game_negotiated);
	ggzcore_game_add_event_hook(game, GGZ_GAME_NEGOTIATE_FAIL,
				    game_negotiate_fail);
	ggzcore_game_add_event_hook(game, GGZ_GAME_PLAYING, game_playing);
}


/* Spectate is 1 if we're spectating; 0 if we're playing. */
int game_initialize(int spectate)
{
	GGZRoom *room;
	GGZGameType *gt;
	GGZModule *module = NULL;

	/* Make sure we aren't already in a game */
	if (game) {
		msgbox(_("You can only play one game at a time."),
		       _("Game Error"), MSGBOX_OKONLY, MSGBOX_INFO,
		       MSGBOX_NORMAL);
		return -1;
	}

	/* Make sure we're actually in a room and not already at a table */
	if (ggzcore_server_get_state(server) != GGZ_STATE_IN_ROOM) {
		msgbox(_("You're still at a table."),
		       _("Game Error"), MSGBOX_OKONLY, MSGBOX_INFO,
		       MSGBOX_NORMAL);
		return -1;
	}

	/* Make sure we're in a room */
	room = ggzcore_server_get_cur_room(server);
	if (!room) {
		msgbox(_("You must be in a room to launch a game.\n"
			 "Launch aborted"),
		       _("Launch Error"), MSGBOX_OKONLY, MSGBOX_STOP,
		       MSGBOX_NORMAL);
		return -1;
	}

	/* Get game type for this room */
	gt = ggzcore_room_get_gametype(room);
	if (!gt) {
		msgbox(_("No game types defined for this server.\n"
			 "Launch aborted."),
		       _("Launch Error"), MSGBOX_OKONLY, MSGBOX_STOP,
		       MSGBOX_NORMAL);
		return -1;
	}

	/* In principle this should have been checked earlier, but we didn't
	   know the game type then. */
	if (spectate && !ggzcore_gametype_get_spectators_allowed(gt)) {
		msgbox(_("This game doesn't support spectators."),
		       _("Launch Error"),
		       MSGBOX_OKONLY, MSGBOX_INFO, MSGBOX_NORMAL);
		return -1;
	}

	if (embedded_protocol_engine && embedded_protocol_version) {
		const char *engine = ggzcore_gametype_get_prot_engine(gt);
		const char *version = ggzcore_gametype_get_prot_version(gt);

		module = NULL;
		if (strcmp(engine, embedded_protocol_engine) != 0
		    || strcmp(version, embedded_protocol_version) != 0) {
		  printf("%s %s vs %s %s\n", engine, version,
			 embedded_protocol_engine, embedded_protocol_version);
			msgbox(_("You need to launch the GGZ client directly\n"
				 "to be able to play this game."),
			       _("Launch Error"),
			       MSGBOX_OKONLY, MSGBOX_STOP, MSGBOX_NORMAL);
			return -1;
		}
	} else {
		module = pick_module(gt);
		if (!module)
			return -1;
	}

	/* Create new game using this module */
	game = ggzcore_game_new();
	ggzcore_game_init(game, server, module);

	/* Register callbacks */
	game_register(game);

	return 0;
}

gboolean can_launch_gametype(GGZGameType *gt)
{
	const char *game = ggzcore_gametype_get_name(gt);
	const char *engine = ggzcore_gametype_get_prot_engine(gt);
	const char *version = ggzcore_gametype_get_prot_version(gt);
	const int num = ggzcore_module_get_num_by_type(game, engine, version);

	if (embedded_protocol_engine && embedded_protocol_version) {
	  return (strcmp(engine, embedded_protocol_engine) == 0
		  && strcmp(version, embedded_protocol_version) == 0);
	} else {
	  return (num > 0);
	}
}
