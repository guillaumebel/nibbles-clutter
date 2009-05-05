/*
 * File: server.c
 * Author: Brent Hendricks
 * Project: GGZ Client
 * Date: 6/19/00
 * $Id$
 *
 * This file contains functions for handling server client profiles
 *
 * Copyright (C) 1998 Brent Hendricks.
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
#  include <config.h>			/* Site-specific config */
#endif

#include <glib.h>
#include <stdio.h>
#include <string.h>

#include <ggzcore.h>
#include <ggz.h>
#include <server.h>
#include "support.h"

/* List of server profiles */
static GList* servers;
static GList* deleted;

/* Local functions for manipulating server profile list */
static void server_list_print(void);
static void server_print(gpointer server, gpointer data);
static void server_free_node(gpointer server, gpointer data);
static gint server_match_name(gconstpointer, gconstpointer);

GGZServer *server = NULL;

void server_profiles_cleanup(void)
{
	if(servers) {
		g_list_foreach(servers, server_free_node, NULL); 	
		g_list_free(servers);
		servers = NULL;
	}
}


void server_profiles_load(void)
{
	guint i, count; 
	char** profiles;
	Server* server;
	
	/* Clear any previous list */
	if (servers) {
		g_list_foreach(servers, server_free_node, NULL); 	
		g_list_free(servers);
		servers = NULL;
	}

	/* Clear list of deleted servers */
	if (deleted) {
		g_list_foreach(deleted, server_free_node, NULL); 	
		g_list_free(deleted);
		deleted = NULL;
	}

	ggzcore_conf_read_list("Servers", "ProfileList", (int*)&count, &profiles);

	for (i = 0; i < count; i++) {
		server = ggz_malloc(sizeof(Server));
		server->name = profiles[i];
		server->host = ggzcore_conf_read_string(server->name, "Host", NULL);
		server->port = ggzcore_conf_read_int(server->name, "Port", 5688);
		server->type = ggzcore_conf_read_int(server->name, "Type", 
					      GGZ_LOGIN_GUEST);
		server->login = ggzcore_conf_read_string(server->name, "Login", NULL);
		if (server->type == GGZ_LOGIN)
			server->password = ggzcore_conf_read_string(server->name, 
							     "Password", NULL);
		server_list_add(server);
	}
	server_list_print();

	if (profiles)
		ggz_free(profiles);
}


void server_profiles_save(void)
{
	int count;
	char** profiles;
	Server* server;
	GList* node;
	
	count = g_list_length(servers);
	profiles = server_get_names();
	
	ggzcore_conf_write_list("Servers", "ProfileList", count, profiles);	
	ggz_free(profiles);

	for (node = servers; node != NULL; node = node->next) {
		server = node->data;
		
		ggzcore_conf_write_string(server->name, "Host", server->host);
		ggzcore_conf_write_int(server->name, "Port", server->port);
		ggzcore_conf_write_int(server->name, "Type", server->type);
		ggzcore_conf_write_string(server->name, "Login", server->login);
		if (server->type == GGZ_LOGIN)
			ggzcore_conf_write_string(server->name, "Password", 
					   server->password);
	}

	for (node = deleted; node != NULL; node = node->next) {
		server = node->data;
		ggzcore_conf_remove_section(server->name);
	}
	
	/* Clear list of deleted servers */
	if (deleted) {
		g_list_foreach(deleted, server_free_node, NULL); 	
		g_list_free(deleted);
		deleted = NULL;
	}
}


void server_list_add(Server* server)
{
	servers = g_list_append(servers, (gpointer)server);
}


GList* server_get_name_list(void)
{
	GList* list = NULL;
	GList* current;
	
	/* Iterate through list, grabbing names */
	for (current = servers; current != NULL; current = current->next) {
		Server *server = current->data;

		list = g_list_append(list, server->name);
	}

	return list;
}


char** server_get_names(void)
{
	char** profiles;
	int i = 0;
	GList* node;

	profiles = ggz_malloc(sizeof(*profiles) * g_list_length(servers));
	node = servers;
	while (node) {
		Server *server = node->data;

		profiles[i++] = server->name;
		node = node->next;
	}

	return profiles;
}


Server* server_get(const gchar* name)
{
	GList* node;
	
	node = g_list_find_custom(servers, (gpointer)name, server_match_name);
	
	if (!node)
		return NULL;

	return node->data;
}


void server_list_remove(const gchar* name)
{
	GList* node;
	Server* server;

	
	node = g_list_find_custom(servers, (gpointer)name, server_match_name);
	/* name not found */
	if (!node)
		return;

	server = node->data;
	servers = g_list_remove_link(servers, node);
	g_list_free_1(node);

	deleted = g_list_append(deleted, (gpointer)server);
	server_list_print();
}


static void server_list_print(void)
{
	g_list_foreach(servers, server_print, NULL); 
}


static void server_print(gpointer server_ptr, gpointer data)
{
	const Server *server = server_ptr;

	ggz_debug("servers", "Profile name: %s\n", server->name);
        ggz_debug("servers", "  Host %s:%d\n", server->host, 
		  server->port);
        ggz_debug("servers", "  Login type: %d\n", server->type);
        ggz_debug("servers", "  Login: %s\n", server->login);
        if (server->type == GGZ_LOGIN)
                ggz_debug("servers",
			  "  Password: %s\n", server->password);
}


static void server_free_node(gpointer server_ptr, gpointer data)
{
	const Server *server = server_ptr;

	if (server->name)
		ggz_free(server->name);

	if (server->host)
		ggz_free(server->host);

	if (server->login)
		ggz_free(server->login);

	if (server->password)
		ggz_free(server->password);

	ggz_free(server);
}


static gint server_match_name(gconstpointer server_ptr, gconstpointer name)
{
	const Server *server = server_ptr;

	return strcmp(server->name, name);
}
