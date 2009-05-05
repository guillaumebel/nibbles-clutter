/*
 * File: ggz-embed.h
 * Author: Jason Short
 * Project: GGZ GTK Client Library
 * $Id$
 *
 * This library provides interface functions so external programs can embed
 * GGZ.  This is intended to be used with ggz-gtk.h, but it provides
 * non-GTK-dependent portions of the library.
 *
 * Copyright (C) 2000-2006 GGZ Development Team
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

#include <ggzcore.h>

/* Actions */
void ggz_embed_leave_table(void);

void ggz_embed_ensure_server(const char *profile_name,
			     const char *host, unsigned int port,
			     const char *username);

/* Query functions */
GGZServer *ggz_embed_get_server(void);
