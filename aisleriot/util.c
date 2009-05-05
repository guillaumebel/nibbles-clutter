/* 
 * Copyright © 1998, 2001, 2003, 2006 Jonathan Blandford <jrb@alum.mit.edu>
 * Copyright © 2007 Christian Persch
 *
 * This game is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307
 * USA
 */

#include <config.h>

#include <string.h>

#include <glib/gi18n.h>

#include <gtk/gtk.h>

#include "util.h"

static AisleriotHelpFunc help_hook;
static gpointer help_hook_data;

void
aisleriot_util_set_help_func (AisleriotHelpFunc func,
                              gpointer user_data)
{
  help_hook = func;
  help_hook_data = user_data;
}

void
aisleriot_display_help (GtkWindow *parent,
                        const char *game_file)
{
  if (!help_hook)
    return;

  help_hook (parent, game_file, help_hook_data);
}
