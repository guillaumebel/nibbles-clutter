/*
 * File: chat.h
 * Author: Justin Zaun
 * Project: GGZ GTK Client
 * $Id$
 *
 * This is the main program body for the GGZ client
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

#ifndef _CHAT_
#define _CHAT_

#include <gtk/gtk.h>

#include <ggzcore.h>

#include "xtext.h"

/* Local chat types */
typedef enum {
	CHAT_LOCAL_NORMAL,
	CHAT_SEND_PERSONAL,
	CHAT_LOCAL_HIGH
} LocalChatType;

void chat_init(void);
void chat_display_server(GGZChatType type,
			 const char *player, const char *message);
void chat_display_local(LocalChatType type,
			const char *player, const char *message);
void chat_send(const gchar *message);
void chat_enter(const gchar *player, gboolean room_known, GGZRoom *from_room);
void chat_part(const gchar *player, gboolean room_known, GGZRoom *to_room);
int chat_checkurl(GtkXText *xtext, char *word);
void chat_word_clicked(GtkXText *xtext, char *word,
	GdkEventButton *event);
void chat_add_friend(const gchar *name, gint display);
void chat_remove_friend(const gchar *name);
void chat_add_ignore(const gchar *name, gint display);
void chat_remove_ignore(const gchar *name);
void chat_save_lists(void);
gchar *chat_complete_name(const gchar *name, int *perfect);
gint chat_is_friend(const gchar *name);
gint chat_is_ignore(const gchar *name);
void chat_lists_cleanup(void);

/* Array of colors used for chat, motd, 'about' dialog, etc. */
extern GdkColor colors[];
extern GdkColor ColorWhite;
extern GdkColor ColorBlack;

#endif
