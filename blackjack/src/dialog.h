// -*- mode:C++; tab-width:2; c-basic-offset:2; indent-tabs-mode:nil -*-
/* Blackjack - dialog.h
 * Copyright (C) 2003 William Jon McCann (mccann@jhu.edu)
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

#ifndef BJ_DIALOG_H
#define BJ_DIALOG_H

gboolean get_insurance_choice (void);

void show_preferences_dialog (void);
void show_hint_dialog (void);

void pref_dialog_response (GtkWidget *, gint, gpointer);

#endif
