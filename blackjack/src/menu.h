/* Blackjack - menu.h
 * Copyright (C) 2003-2004 William Jon McCann <mccann@jhu.edu>
 * Copyright (C) 1998 Jonathan Blandford <jrb@mit.edu>
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

#ifndef BJ_MENU_H
#define BJ_MENU_H

void on_help_contents_activate (GtkAction * action, gpointer data);
void on_help_about_activate (GtkAction * action, gpointer data);
void on_game_quit_activate (GtkAction * action, gpointer data);
void on_game_new_activate (GtkAction * action, gpointer data);
void on_game_restart_activate (GtkAction * action, gpointer data);
void on_game_hint_activate (GtkAction * action, gpointer data);
void on_toolbar_activate (GtkToggleAction * action, gpointer data);
void on_preferences_activate (GtkAction * action, gpointer data);
void on_control_deal_activate (GtkAction * action, gpointer data);
void on_control_hit_activate (GtkAction * action, gpointer data);
void on_control_stand_activate (GtkAction * action, gpointer data);
void on_control_surrender_activate (GtkAction * action, gpointer data);
void on_control_split_activate (GtkAction * action, gpointer data);
void on_control_double_activate (GtkAction * action, gpointer data);

#endif
