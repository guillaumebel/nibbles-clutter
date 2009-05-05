/*
 * File: server.h
 * Author: Brent Hendricks
 * Project: GGZ Client
 * Date: 6/19/00
 * $Id$
 *
 * This file contains functions for handling server connection profiles
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


#ifndef _GGZ_SERVER_H
#define _GGZ_SERVER_H

/* 
 * The Server structure is meant to be a node in a linked list of
 * the GGZ server profiles
 */

#include <glib.h>

#include <ggzcore.h>

typedef struct Server {
	
	/* Name of profile */
	gchar* name;
	
	/* Hostname of GGZ server */
	gchar* host;

	/* Port on which GGZ server in running */
	guint port;

	/* Login type: one of GGZ_LOGIN, GGZ_LOGIN_GUEST, GGZ_LOGIN_NEW */
	guchar type;

	/* Login name on this server */
	gchar* login;

	/* Password for this server (optional) */
	gchar* password;

} Server;


/* Read (or re-read) server profiles from configuration files */
void server_profiles_load(void);


/* Save server profiles to configuration file */
void server_profiles_save(void);


/* Cleanup the server profiles structures */
void server_profiles_cleanup(void);

/* 
 * Add a server to the profile list.  server should be be a pointer to
 * dynamically allocated Server structure.  All string members should
 * also be dynmcially allocated 
 */
void server_list_add(Server* server);


/* 
 * Return list of profile names as a GList.  The list should be
 * free'd, but not the data elememts.  
 */
GList* server_get_name_list(void);


/*
 * Return the list of profile names as an argv-style array of strings
 * The array should be free'd, but not the array elements.
 */
char** server_get_names(void);


/*
 * Return a pointer to the server profile whose name matches 'name'
 * Do *not* free()!
 */
Server* server_get(const gchar* name);


/*
 * Remove the server profile whose name matches 'name'
 */
void server_list_remove(const gchar* name);

extern GGZServer *server;

#endif
