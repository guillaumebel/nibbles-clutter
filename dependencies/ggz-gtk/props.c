/*
 * File: props.c
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

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include <ggz.h>

#include "chat.h"
#include "ggzcore.h"
#include "login.h"
#include "msgbox.h"
#include "props.h"
#include "server.h"
#include "support.h"
#include "xtext.h"
#include "client.h"

enum {
	PROFILE_COLUMN_NAME,
	PROFILE_COLUMNS
};

static void props_update(void);
static void dlg_props_realize(GtkWidget *widget, gpointer user_data);
static void props_profile_box_realized(GtkWidget *widget, gpointer user_data);
static void props_profile_entry_changed(GtkWidget *widget, gpointer user_data);
static void props_normal_toggled(GtkWidget *widget, gpointer user_data);
static void props_add_button_clicked(GtkWidget *widget, gpointer user_data);
static void props_modify_button_clicked(GtkWidget *widget, gpointer user_data);
static void props_delete_button_clicked(GtkWidget *widget, gpointer user_data);
static void props_Font_button_clicked(GtkWidget *widget, gpointer user_data);
static void props_color_type_toggled(GtkWidget *widget, gpointer user_data);
static void props_ok_button_clicked(GtkWidget *widget, gpointer user_data);
static void props_apply_button_clicked(GtkWidget *widget, gpointer user_data);
static void props_cancel_button_clicked(GtkWidget *widget, gpointer user_data);
static void props_fok_button_clicked(GtkWidget *widget, gpointer user_data);
static void props_fcancel_button_clicked(GtkWidget *widget, gpointer user_data);
static void props_fapply_button_clicked(GtkWidget *widget, gpointer user_data);
static void props_profiles_reload(void);

static GtkWidget* create_dlg_props_font (void);

static GtkWidget *props_dialog;
static GtkWidget *props_font_dialog;
static gboolean raised;

gboolean props_is_raised(void)
{
	return raised;
}

void props_raise(void)
{
	raised = TRUE;
	main_activate();
}

static void props_update(void)
{
	GtkWidget *tmp;
	GtkXText *xtext = GTK_XTEXT(ggz_lookup_widget(win_main, "xtext_custom"));
	GList *items;
	const char *text;

	/* Save Changes */

	/* Set XText font */
	tmp = ggz_lookup_widget((props_dialog), "chat_font");
	ggzcore_conf_write_string("CHAT", "FONT", gtk_entry_get_text(GTK_ENTRY(tmp)));

	/* Auto-Indent */
	tmp = ggz_lookup_widget((props_dialog), "indent_check");
	ggzcore_conf_write_int("CHAT", "AUTO_INDENT", GTK_TOGGLE_BUTTON(tmp)->active);

	/* Timestamp */
	tmp = ggz_lookup_widget((props_dialog), "timestamp_check");
	ggzcore_conf_write_int("CHAT", "TIMESTAMP", GTK_TOGGLE_BUTTON(tmp)->active);

	/* Word Wrap */
	tmp = ggz_lookup_widget((props_dialog), "wrap_check");
	ggzcore_conf_write_int("CHAT", "WORD_WRAP", GTK_TOGGLE_BUTTON(tmp)->active);

	/* Sound */
	tmp = ggz_lookup_widget((props_dialog), "sound_check");
	ggzcore_conf_write_int("CHAT", "SOUND", GTK_TOGGLE_BUTTON(tmp)->active);

	/* Join/Part Messages */
	tmp = ggz_lookup_widget((props_dialog), "ignore_check");
	ggzcore_conf_write_int("CHAT", "IGNORE", GTK_TOGGLE_BUTTON(tmp)->active);

	/* Background Color */
	tmp = ggz_lookup_widget((props_dialog), "white_radio");
	ggzcore_conf_write_int("CHAT", "BG_COLOR", GTK_TOGGLE_BUTTON(tmp)->active);

	/* Friends Color */
	tmp = ggz_lookup_widget((props_dialog), "f_spin");
	ggzcore_conf_write_int("CHAT", "F_COLOR", gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(tmp)));

	/* Highlight Color */
	tmp = ggz_lookup_widget((props_dialog), "h_spin");
	ggzcore_conf_write_int("CHAT", "H_COLOR", gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(tmp)));

	/* Normal Color */
	tmp = ggz_lookup_widget((props_dialog), "n_spin");
	ggzcore_conf_write_int("CHAT", "N_COLOR", gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(tmp)));

	/* Name */
	tmp = ggz_lookup_widget((props_dialog), "info_name");
	ggzcore_conf_write_string("USER INFO", "NAME", gtk_entry_get_text(GTK_ENTRY(tmp)));

	/* City */
	tmp = ggz_lookup_widget((props_dialog), "info_city");
	ggzcore_conf_write_string("USER INFO", "CITY", gtk_entry_get_text(GTK_ENTRY(tmp)));

	/* State */
	tmp = ggz_lookup_widget((props_dialog), "info_state");
	ggzcore_conf_write_string("USER INFO", "STATE", gtk_entry_get_text(GTK_ENTRY(tmp)));

	/* Country */
	tmp = ggz_lookup_widget((props_dialog), "info_country");
	ggzcore_conf_write_string("USER INFO", "COUNTRY", gtk_entry_get_text(GTK_ENTRY(tmp)));

	/* Comments */
	tmp = ggz_lookup_widget((props_dialog), "info_comments");
	{
		GtkTextIter start, end;
		GtkTextBuffer *buffer = gtk_text_view_get_buffer(
			GTK_TEXT_VIEW(tmp));
		gtk_text_buffer_get_bounds(buffer, &start, &end);
		text = gtk_text_buffer_get_text(buffer,
						&start, &end, FALSE);
	}
	ggzcore_conf_write_string("USER INFO", "COMMENTS", text);

	/* Single click room entry */
	tmp = ggz_lookup_widget((props_dialog), "click_checkbutton");
	ggzcore_conf_write_int("OPTIONS", "ROOMENTRY", GTK_TOGGLE_BUTTON(tmp)->active);

	/* MOTD */
	tmp = ggz_lookup_widget((props_dialog), "motd_all_radio");
	if (GTK_TOGGLE_BUTTON(tmp)->active)
		ggzcore_conf_write_string("OPTIONS", "MOTD", "ALL");
	tmp = ggz_lookup_widget((props_dialog), "motd_new_radio");
	if (GTK_TOGGLE_BUTTON(tmp)->active)
		ggzcore_conf_write_string("OPTIONS", "MOTD", "NEW");
	tmp = ggz_lookup_widget((props_dialog), "motd_important_radio");
	if (GTK_TOGGLE_BUTTON(tmp)->active)
		ggzcore_conf_write_string("OPTIONS", "MOTD", "IMPORTANT");
	tmp = ggz_lookup_widget((props_dialog), "motd_none_radio");
	if (GTK_TOGGLE_BUTTON(tmp)->active)
		ggzcore_conf_write_string("OPTIONS", "MOTD", "NONE");


	server_profiles_save();
	ggzcore_conf_commit();



	/* Activate Changes */

	/* Set XText font */
	tmp = ggz_lookup_widget((props_dialog), "chat_font");

	gtk_xtext_set_font(xtext,
			   (char *)gtk_entry_get_text(GTK_ENTRY(tmp)));

	/* Auto-Indent */
	tmp = ggz_lookup_widget((props_dialog), "indent_check");
	if (GTK_TOGGLE_BUTTON(tmp)->active)
	{
		xtext->auto_indent = TRUE;
	}else{
		xtext->auto_indent = FALSE;
	}

	/* Timestamp */
	tmp = ggz_lookup_widget((props_dialog), "timestamp_check");
	if (GTK_TOGGLE_BUTTON(tmp)->active)
	{
		gtk_xtext_set_time_stamp(xtext->buffer, TRUE);
#if 0 /* Disabled */
		xtext->indent = xtext->indent + xtext->stamp_width;
#endif /* 0 */
	}else{
		gtk_xtext_set_time_stamp(xtext->buffer, FALSE);
#if 0 /* Disabled */
		xtext->indent = xtext->indent - xtext->stamp_width;
#endif /* 0 */
	}

	/* Word Wrap */
	tmp = ggz_lookup_widget((props_dialog), "wrap_check");
	xtext->wordwrap = GTK_TOGGLE_BUTTON(tmp)->active;

	/* Background Color */
	tmp = ggz_lookup_widget((props_dialog), "white_radio");
	if (GTK_TOGGLE_BUTTON(tmp)->active)
	{
		colors[18] = ColorBlack;
		colors[19] = ColorWhite;
	}else{
		colors[18] = ColorWhite;
		colors[19] = ColorBlack;
	}
	gtk_xtext_set_palette(xtext, colors);

	/* Refresh XText to make changed take affect */
	gtk_xtext_refresh(xtext, 0);

	/* Display a status Message */
	chat_display_local(CHAT_LOCAL_NORMAL, NULL, _("Properties Updated"));

	/* If the login dialog is open refill the combo box */
	if(login_dialog != NULL) {
		tmp = ggz_lookup_widget(login_dialog, "profile_combo");
		items = server_get_name_list();
		if (tmp && items)
			gtk_combo_set_popdown_strings(GTK_COMBO(tmp), items);
	}
}


void dlg_props_realize(GtkWidget *widget, gpointer user_data)
{
	GtkWidget *tmp;
	char *old;

	/* Set XText font */
	tmp = ggz_lookup_widget((props_dialog), "chat_font");
	old = ggzcore_conf_read_string("CHAT", "FONT", DEFAULT_FONT);
	gtk_entry_set_text(GTK_ENTRY(tmp), old);
	ggz_free(old);

	/* Auto-Indent */
	tmp = ggz_lookup_widget((props_dialog), "indent_check");
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(tmp), ggzcore_conf_read_int("CHAT", "AUTO_INDENT", TRUE));

	/* Timestamp */
	tmp = ggz_lookup_widget((props_dialog), "timestamp_check");
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(tmp), ggzcore_conf_read_int("CHAT", "TIMESTAMP", FALSE));

	/* Word Wrap */
	tmp = ggz_lookup_widget((props_dialog), "wrap_check");
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(tmp), ggzcore_conf_read_int("CHAT", "WORD_WRAP", TRUE));

	/* Sound */
	tmp = ggz_lookup_widget((props_dialog), "sound_check");
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(tmp), ggzcore_conf_read_int("CHAT", "SOUND", TRUE));

	/* Join/Part Messages */
	tmp = ggz_lookup_widget((props_dialog), "ignore_check");
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(tmp), ggzcore_conf_read_int("CHAT", "IGNORE", FALSE));

	/* Background Color */
	tmp = ggz_lookup_widget((props_dialog), "white_radio");
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(tmp), ggzcore_conf_read_int("CHAT", "BG_COLOR", TRUE));
	tmp = ggz_lookup_widget((props_dialog), "black_radio");
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(tmp), 1 - ggzcore_conf_read_int("CHAT", "BG_COLOR", TRUE));

	/* Friends Color */
	tmp = ggz_lookup_widget((props_dialog), "f_spin");
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(tmp), ggzcore_conf_read_int("CHAT", "F_COLOR", 6));

	/* Highlight Color */
	tmp = ggz_lookup_widget((props_dialog), "h_spin");
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(tmp), ggzcore_conf_read_int("CHAT", "H_COLOR", 3));

	/* Normal Color */
	tmp = ggz_lookup_widget((props_dialog), "n_spin");
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(tmp), ggzcore_conf_read_int("CHAT", "N_COLOR", 2));

	/* Name */
	tmp = ggz_lookup_widget((props_dialog), "info_name");
	old = ggzcore_conf_read_string("USER INFO", "NAME", ".");
	gtk_entry_set_text(GTK_ENTRY(tmp), old);
	ggz_free(old);

	/* City */
	tmp = ggz_lookup_widget((props_dialog), "info_city");
	old = ggzcore_conf_read_string("USER INFO", "CITY", ".");
	gtk_entry_set_text(GTK_ENTRY(tmp), old);
	ggz_free(old);

	/* State */
	tmp = ggz_lookup_widget((props_dialog), "info_state");
	old = ggzcore_conf_read_string("USER INFO", "STATE", ".");
	gtk_entry_set_text(GTK_ENTRY(tmp), old);
	ggz_free(old);

	/* Country */
	tmp = ggz_lookup_widget((props_dialog), "info_country");
	old = ggzcore_conf_read_string("USER INFO", "COUNTRY", ".");
	gtk_entry_set_text(GTK_ENTRY(tmp), old);
	ggz_free(old);

	/* Comments */
	tmp = ggz_lookup_widget((props_dialog), "info_comments");
	old = ggzcore_conf_read_string("USER INFO", "COMMENTS", ".");
	gtk_text_buffer_set_text(gtk_text_view_get_buffer(GTK_TEXT_VIEW(tmp)),
				 old, strlen(old));
	ggz_free(old);

	/* Single click room entry */
	tmp = ggz_lookup_widget((props_dialog), "click_checkbutton");
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(tmp), ggzcore_conf_read_int("OPTIONS", "ROOMENTRY", FALSE));

	/* MOTD */
	old = ggzcore_conf_read_string("OPTIONS", "MOTD", "ALL");
	if (!strcmp(old, "ALL")) {
		tmp = ggz_lookup_widget((props_dialog), "motd_all_radio");
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(tmp), TRUE);
	} else if (!strcmp(old, "NEW")) {
		tmp = ggz_lookup_widget((props_dialog), "motd_new_radio");
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(tmp), TRUE);
	} else if (!strcmp(old, "IMPORTANT")) {
		tmp = ggz_lookup_widget((props_dialog), "motd_important_radio");
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(tmp), TRUE);
	} else if (!strcmp(old, "NONE")) {
		tmp = ggz_lookup_widget((props_dialog), "motd_none_radio");
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(tmp), TRUE);
	}
	ggz_free(old);
}


void props_profile_box_realized(GtkWidget *widget, gpointer user_data)
{
	GtkWidget* tmp;
	gchar *port;

	props_profiles_reload();

	tmp = ggz_lookup_widget(props_dialog, "add_button");
	gtk_widget_set_sensitive(tmp, FALSE);
	tmp = ggz_lookup_widget(props_dialog, "modify_button");
	gtk_widget_set_sensitive(tmp, FALSE);
	tmp = ggz_lookup_widget(props_dialog, "delete_button");
	gtk_widget_set_sensitive(tmp, FALSE);

	tmp = ggz_lookup_widget(props_dialog, "port_entry");  
	port = g_strdup_printf("%d", 5688);
	gtk_entry_set_text(GTK_ENTRY(tmp), port);
	g_free(port);
}


static void props_profile_list_select(GtkTreeSelection *select,
				      gpointer user_data)
{
	GtkWidget *tmp;
	gchar *profile_name;
	Server *profile;
	gchar *port;
	GtkTreeModel *model;
	GtkTreeIter iter;

	if (!gtk_tree_selection_get_selected(select, &model, &iter)) {
		return;
	}

	tmp = ggz_lookup_widget(props_dialog, "profile_list");
	gtk_tree_model_get(model, &iter,
			   PROFILE_COLUMN_NAME, &profile_name, -1);
	profile = server_get(profile_name);
	g_free(profile_name);
	if (!profile) {
		return;
	}

	tmp = ggz_lookup_widget(props_dialog, "add_button");
	gtk_widget_set_sensitive(tmp, FALSE);
	tmp = ggz_lookup_widget(props_dialog, "modify_button");
	gtk_widget_set_sensitive(tmp, TRUE);
	tmp = ggz_lookup_widget(props_dialog, "delete_button");
	gtk_widget_set_sensitive(tmp, TRUE);

	tmp = ggz_lookup_widget(props_dialog, "profile_entry");
	if (profile->name)
		gtk_entry_set_text(GTK_ENTRY(tmp), profile->name );
	else
		gtk_entry_set_text(GTK_ENTRY(tmp), "");

	tmp = ggz_lookup_widget(props_dialog, "server_entry");
	if (profile->host)
		gtk_entry_set_text(GTK_ENTRY(tmp), profile->host );
	else
		gtk_entry_set_text(GTK_ENTRY(tmp), ""); 

	tmp = ggz_lookup_widget(props_dialog, "port_entry");  
	port = g_strdup_printf("%d", profile->port);
	gtk_entry_set_text(GTK_ENTRY(tmp), port);
	g_free(port);

	tmp = ggz_lookup_widget(props_dialog, "username_entry");
	if (profile->login)
		gtk_entry_set_text(GTK_ENTRY(tmp), profile->login);
	else
		gtk_entry_set_text(GTK_ENTRY(tmp), "");

	tmp = ggz_lookup_widget(props_dialog, "password_entry");
	if (profile->password)
		gtk_entry_set_text(GTK_ENTRY(tmp), profile->password);
	else
		gtk_entry_set_text(GTK_ENTRY(tmp), "");

	if (profile->type == GGZ_LOGIN) {
		tmp = ggz_lookup_widget(props_dialog, "normal_radio");
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(tmp), TRUE);
	}
	if (profile->type == GGZ_LOGIN_GUEST) {
		tmp = ggz_lookup_widget(props_dialog, "guest_radio");
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(tmp), TRUE);
	}
}


void props_profile_entry_changed(GtkWidget *widget, gpointer user_data)
{
	GtkWidget *tmp;
	GList *names, *node;
	const gchar * profile;

	tmp = ggz_lookup_widget(props_dialog, "profile_entry");
	profile = gtk_entry_get_text(GTK_ENTRY(tmp));

	if (!strcmp(profile, "")) {
		tmp = ggz_lookup_widget(props_dialog, "add_button");
		gtk_widget_set_sensitive(GTK_WIDGET(tmp),FALSE);
		tmp = ggz_lookup_widget(props_dialog, "modify_button");
		gtk_widget_set_sensitive(GTK_WIDGET(tmp),FALSE);
		tmp = ggz_lookup_widget(props_dialog, "delete_button");
		gtk_widget_set_sensitive(GTK_WIDGET(tmp),FALSE);
		return;
	}

	names = server_get_name_list();
	for (node = names; node != NULL; node = node->next) {
		/* If we match one of the profiles */
		if(!strcmp(profile, (char*)(node->data))) {
			tmp = ggz_lookup_widget(props_dialog, "add_button");
			gtk_widget_set_sensitive(GTK_WIDGET(tmp),FALSE);
			tmp = ggz_lookup_widget(props_dialog, "modify_button");
			gtk_widget_set_sensitive(GTK_WIDGET(tmp),TRUE);
			tmp = ggz_lookup_widget(props_dialog, "delete_button");
			gtk_widget_set_sensitive(GTK_WIDGET(tmp),TRUE);
			break;
		}
	}

	/* If we didn't match anything */
	if (!node) {
		tmp = ggz_lookup_widget(props_dialog, "add_button");
		gtk_widget_set_sensitive(GTK_WIDGET(tmp),TRUE);
		tmp = ggz_lookup_widget(props_dialog, "modify_button");
		gtk_widget_set_sensitive(GTK_WIDGET(tmp),FALSE);
		tmp = ggz_lookup_widget(props_dialog, "delete_button");
		gtk_widget_set_sensitive(GTK_WIDGET(tmp),FALSE);
	}

	g_list_free(names);
}


void props_normal_toggled(GtkWidget *widget, gpointer user_data)
{
	GtkWidget* password;
	GtkWidget* confirm;           

	password = ggz_lookup_widget(GTK_WIDGET(user_data), "password_box");
	confirm = ggz_lookup_widget(GTK_WIDGET(user_data), "confirm_box");

	if (GTK_TOGGLE_BUTTON(widget)->active) {
		gtk_widget_show(password);
		gtk_widget_show(confirm);
	} else {
		gtk_widget_hide(password);
		gtk_widget_hide(confirm);
	}
}


void props_add_button_clicked(GtkWidget *widget, gpointer user_data)
{
	GtkWidget *tmp;
	Server *new_server;
	GtkTreeIter iter;

	new_server = g_malloc0(sizeof(Server));

	tmp = ggz_lookup_widget(props_dialog, "profile_entry");
	new_server->name = g_strdup(gtk_entry_get_text(GTK_ENTRY(tmp)));
	gtk_entry_set_text(GTK_ENTRY(tmp), "");

	tmp = ggz_lookup_widget(props_dialog, "server_entry");
	new_server->host = g_strdup(gtk_entry_get_text(GTK_ENTRY(tmp)));
	gtk_entry_set_text(GTK_ENTRY(tmp), "");

	tmp = ggz_lookup_widget(props_dialog, "port_entry");
	new_server->port = atoi(gtk_entry_get_text(GTK_ENTRY(tmp)));
	gtk_entry_set_text(GTK_ENTRY(tmp), "");

	tmp = ggz_lookup_widget(props_dialog, "username_entry");
	new_server->login = g_strdup(gtk_entry_get_text(GTK_ENTRY(tmp)));   
	gtk_entry_set_text(GTK_ENTRY(tmp), "");

	tmp = ggz_lookup_widget(props_dialog, "normal_radio");
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(tmp))) {
		new_server->type = GGZ_LOGIN;  
		tmp = ggz_lookup_widget(props_dialog, "password_entry");
		new_server->password = g_strdup(gtk_entry_get_text(GTK_ENTRY(tmp)));
		gtk_entry_set_text(GTK_ENTRY(tmp), "");
	}
	else    
		new_server->type = GGZ_LOGIN_GUEST;

	/* FIXME: check confirm password entry */
	server_list_add(new_server);

	/* Add profile to list */
	tmp = ggz_lookup_widget(props_dialog, "profile_list_store");
	gtk_list_store_append(GTK_LIST_STORE(tmp), &iter);
	gtk_list_store_set(GTK_LIST_STORE(tmp), &iter,
			   PROFILE_COLUMN_NAME, new_server->name, -1);
}


void props_modify_button_clicked(GtkWidget *widget, gpointer user_data)
{
	GtkWidget *tmp;
	Server *server;

	/* FIXME: check confirm password entry */
	/* FIXME: free() previous strings if necessary */

	tmp = ggz_lookup_widget(props_dialog, "profile_entry"); 
	server = server_get(gtk_entry_get_text(GTK_ENTRY(tmp)));

	tmp = ggz_lookup_widget(props_dialog, "server_entry");
	server->host = g_strdup(gtk_entry_get_text(GTK_ENTRY(tmp)));

	tmp = ggz_lookup_widget(props_dialog, "port_entry");  
	server->port = atoi(gtk_entry_get_text(GTK_ENTRY(tmp)));

	tmp = ggz_lookup_widget(props_dialog, "username_entry");
	server->login = g_strdup(gtk_entry_get_text(GTK_ENTRY(tmp)));

	tmp = ggz_lookup_widget(props_dialog, "normal_radio");
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(tmp))) {
		server->type = GGZ_LOGIN;
		tmp = ggz_lookup_widget(props_dialog, "password_entry");
		server->password = g_strdup(gtk_entry_get_text(GTK_ENTRY(tmp)));
	}
	else
		server->type = GGZ_LOGIN_GUEST;
}


void props_delete_button_clicked(GtkWidget *widget, gpointer user_data)
{
	GtkWidget *tmp;

	tmp = ggz_lookup_widget(props_dialog, "profile_entry");
	server_list_remove(gtk_entry_get_text(GTK_ENTRY(tmp)));
	gtk_entry_set_text(GTK_ENTRY(tmp), "");

	tmp = ggz_lookup_widget(props_dialog, "server_entry");
	gtk_entry_set_text(GTK_ENTRY(tmp), "");

	tmp = ggz_lookup_widget(props_dialog, "port_entry");
	gtk_entry_set_text(GTK_ENTRY(tmp), "");

	tmp = ggz_lookup_widget(props_dialog, "username_entry");
	gtk_entry_set_text(GTK_ENTRY(tmp), "");

	tmp = ggz_lookup_widget(props_dialog, "normal_radio");
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(tmp))) {
		tmp = ggz_lookup_widget(props_dialog, "password_entry");
		gtk_entry_set_text(GTK_ENTRY(tmp), "");
	}

	props_profiles_reload();
}


void props_Font_button_clicked(GtkWidget *widget, gpointer user_data)
{
        /* If it already exists, bring it to the front */
        if (props_font_dialog) {
                gdk_window_show(props_font_dialog->window);
                gdk_window_raise(props_font_dialog->window);
        } else {
                props_font_dialog = create_dlg_props_font();
		gtk_widget_show_all(props_font_dialog);
        }
}



void props_color_type_toggled(GtkWidget *widget, gpointer user_data)
{

}


void props_ok_button_clicked(GtkWidget *widget, gpointer user_data)
{
	/* update the settings */
	props_update();

        /* Close font selector if open */
        if (props_font_dialog)
                gtk_widget_destroy(props_font_dialog);
	raised = FALSE;
	main_activate();
}


void props_apply_button_clicked(GtkWidget *widget, gpointer user_data)
{
	/* update the settings */
	props_update();
}


void props_cancel_button_clicked(GtkWidget *widget, gpointer user_data)
{
        /* Close font selector if open */
        if (props_font_dialog)
                gtk_widget_destroy(props_font_dialog);
	raised = FALSE;
	main_activate();
}


void props_fok_button_clicked(GtkWidget *widget, gpointer user_data)
{
	GtkWidget *tmp;

	/* Set font */
	tmp = ggz_lookup_widget(props_dialog, "chat_font");
	if (gtk_font_selection_dialog_get_font_name(GTK_FONT_SELECTION_DIALOG(props_font_dialog)) != NULL )
	{
		gtk_entry_set_text(GTK_ENTRY(tmp),
			gtk_font_selection_dialog_get_font_name(GTK_FONT_SELECTION_DIALOG(props_font_dialog)));
	}

	/* Close the font selector */
	gtk_widget_destroy(props_font_dialog);
}


void props_fcancel_button_clicked(GtkWidget *widget, gpointer user_data)
{
	/* Close the font selector */
	gtk_widget_destroy(props_font_dialog);
}


void props_fapply_button_clicked(GtkWidget *widget, gpointer user_data)
{
	GtkWidget *tmp;

	/* Set font */
	tmp = ggz_lookup_widget(props_dialog, "chat_font");
	if (gtk_font_selection_dialog_get_font_name(GTK_FONT_SELECTION_DIALOG(props_font_dialog)) != NULL )
	{
		gtk_entry_set_text(GTK_ENTRY(tmp),
			gtk_font_selection_dialog_get_font_name(GTK_FONT_SELECTION_DIALOG(props_font_dialog)));
	}
}


static void props_profiles_reload(void)
{
	GList *names, *node;
	GtkListStore *store;

	store = GTK_LIST_STORE(ggz_lookup_widget(props_dialog,
					     "profile_list_store"));
	gtk_list_store_clear(store);

	names = server_get_name_list();
	for (node = names; node != NULL; node = node->next) {
		GtkTreeIter iter;

		gtk_list_store_append(store, &iter);
		gtk_list_store_set(store, &iter,
				   PROFILE_COLUMN_NAME, node->data, -1);
		
	}

	g_list_free(names);
}

static GtkWidget *tree_new(GtkWidget *window)
{
	GtkListStore *store;
	GtkWidget *tree;
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	GtkTreeSelection *select;

	assert(PROFILE_COLUMNS == 1);
	store = gtk_list_store_new(PROFILE_COLUMNS,
				   G_TYPE_STRING);

	tree = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
	g_object_unref(store);

	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("Profile", renderer,
				"text", PROFILE_COLUMN_NAME, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);

	gtk_widget_ref(tree);
	g_object_set_data_full(G_OBJECT(window), "profile_list", tree,
			       (GtkDestroyNotify) gtk_widget_unref);
	g_object_set_data(G_OBJECT(window), "profile_list_store", store);

	select = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree));
	gtk_tree_selection_set_mode(select, GTK_SELECTION_SINGLE);
	g_signal_connect(select, "changed",
			 GTK_SIGNAL_FUNC(props_profile_list_select),
			 store);

	return tree;
}

GtkWidget* create_props_dlg(void)
{
  GtkWidget *dlg_props;
  GtkWidget *notebook;
  GtkWidget *props_profile_box;
  GtkWidget *scrolledwindow4;
  GtkWidget *profile_list;
  GtkWidget *edit_box;
  GtkWidget *data_box;
  GtkWidget *profile_box;
  GtkWidget *profile_label;
  GtkWidget *profile_entry;
  GtkWidget *server_box;
  GtkWidget *server_label;
  GtkWidget *server_entry;
  GtkWidget *port_label;
  GtkWidget *port_entry;
  GtkWidget *user_box;
  GtkWidget *user_data_box;
  GtkWidget *username_box;
  GtkWidget *username_label;
  GtkWidget *username_entry;
  GtkWidget *password_box;
  GtkWidget *password_label;
  GtkWidget *password_entry;
  GtkWidget *confirm_box;
  GtkWidget *confirm_label;
  GtkWidget *confirm_entry;
  GtkWidget *radio_box;
  GSList *login_type_group = NULL;
  GtkWidget *normal_radio;
  GtkWidget *guest_radio;
  GtkWidget *buttonbox;
  GtkWidget *add_button;
  GtkWidget *modify_button;
  GtkWidget *delete_button;
  GtkWidget *label1;
  GtkWidget *chat_box;
  GtkWidget *hbox8;
  GtkWidget *label12;
  GtkWidget *chat_font;
  GtkWidget *Font_button;
  GtkWidget *chat_table;
  GtkWidget *ignore_check;
  GtkWidget *sound_check;
  GtkWidget *indent_check;
  GtkWidget *timestamp_check;
  GtkWidget *wrap_check;
  GtkWidget *frame1;
  GtkWidget *vbox9;
  GtkWidget *color_table;
  GtkObject *h_spin_adj;
  GtkWidget *h_spin;
  GtkObject *n_spin_adj;
  GtkWidget *n_spin;
  GtkObject *f_spin_adj;
  GtkWidget *f_spin;
  GtkWidget *f_label;
  GtkWidget *h_label;
  GtkWidget *n_label;
  GtkWidget *label26;
  GtkWidget *label25;
  GtkWidget *label24;
  GSList *Chat_Color_group = NULL;
  GtkWidget *black_radio;
  GtkWidget *white_radio;
  GtkWidget *label2;
  GtkWidget *userinfo_box;
  GtkWidget *label16;
  GtkWidget *vbox7;
  GtkWidget *hbox12;
  GtkWidget *label17;
  GtkWidget *info_name;
  GtkWidget *vbox8;
  GtkWidget *hbox13;
  GtkWidget *label18;
  GtkWidget *info_city;
  GtkWidget *hbox14;
  GtkWidget *label19;
  GtkWidget *info_state;
  GtkWidget *hbox15;
  GtkWidget *label20;
  GtkWidget *info_country;
  GtkWidget *label21;
  GtkWidget *scrolledwindow1;
  GtkWidget *info_comments;
  GtkWidget *label3;
  GtkWidget *table1;
  GtkWidget *click_checkbutton;
  GtkWidget *frame2;
  GtkWidget *table2;
  GSList *motd_group = NULL;
  GtkWidget *motd_all_radio;
  GtkWidget *motd_new_radio;
  GtkWidget *motd_important_radio;
  GtkWidget *motd_none_radio;
  GtkWidget *label67;
  GtkWidget *hbuttonbox1;
  GtkWidget *button1;
  GtkWidget *button2;
  GtkWidget *button3;

  dlg_props = gtk_vbox_new(FALSE, 0);
  props_dialog = dlg_props;

  notebook = gtk_notebook_new ();
  gtk_widget_ref (notebook);
  g_object_set_data_full(G_OBJECT (dlg_props), "notebook", notebook,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start(GTK_BOX(dlg_props), notebook, TRUE, TRUE, 0);

  props_profile_box = gtk_hbox_new (FALSE, 10);
  gtk_widget_ref (props_profile_box);
  g_object_set_data_full(G_OBJECT (dlg_props), "props_profile_box", props_profile_box,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_container_add (GTK_CONTAINER (notebook), props_profile_box);
  gtk_container_set_border_width (GTK_CONTAINER (props_profile_box), 10);

  scrolledwindow4 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_ref (scrolledwindow4);
  g_object_set_data_full(G_OBJECT (dlg_props), "scrolledwindow4", scrolledwindow4,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (props_profile_box), scrolledwindow4, TRUE, TRUE, 0);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow4), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

  profile_list = tree_new(dlg_props);
  gtk_container_add (GTK_CONTAINER (scrolledwindow4), profile_list);

  edit_box = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (edit_box);
  g_object_set_data_full(G_OBJECT (dlg_props), "edit_box", edit_box,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (props_profile_box), edit_box, TRUE, TRUE, 0);

  data_box = gtk_vbox_new (FALSE, 7);
  gtk_widget_ref (data_box);
  g_object_set_data_full(G_OBJECT (dlg_props), "data_box", data_box,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (edit_box), data_box, TRUE, FALSE, 0);

  profile_box = gtk_hbox_new (FALSE, 5);
  gtk_widget_ref (profile_box);
  g_object_set_data_full(G_OBJECT (dlg_props), "profile_box", profile_box,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (data_box), profile_box, FALSE, FALSE, 0);

  profile_label = gtk_label_new (_("Profile:"));
  gtk_widget_ref (profile_label);
  g_object_set_data_full(G_OBJECT (dlg_props), "profile_label", profile_label,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (profile_box), profile_label, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (profile_label), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (profile_label), 1, 0.5);

  profile_entry = gtk_entry_new();
  gtk_entry_set_max_length(GTK_ENTRY(profile_entry), 256);
  gtk_widget_ref (profile_entry);
  g_object_set_data_full(G_OBJECT (dlg_props), "profile_entry", profile_entry,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (profile_box), profile_entry, TRUE, TRUE, 5);

  server_box = gtk_hbox_new (FALSE, 5);
  gtk_widget_ref (server_box);
  g_object_set_data_full(G_OBJECT (dlg_props), "server_box", server_box,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (data_box), server_box, FALSE, FALSE, 0);

  server_label = gtk_label_new (_("Server:"));
  gtk_widget_ref (server_label);
  g_object_set_data_full(G_OBJECT (dlg_props), "server_label", server_label,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (server_box), server_label, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (server_label), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (server_label), 1, 0.5);

  server_entry = gtk_entry_new();
  gtk_entry_set_max_length(GTK_ENTRY(server_entry), 256);
  gtk_widget_ref (server_entry);
  g_object_set_data_full(G_OBJECT (dlg_props), "server_entry", server_entry,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (server_box), server_entry, TRUE, TRUE, 5);

  port_label = gtk_label_new (_("Port:"));
  gtk_widget_ref (port_label);
  g_object_set_data_full(G_OBJECT (dlg_props), "port_label", port_label,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (server_box), port_label, FALSE, TRUE, 2);
  gtk_misc_set_alignment (GTK_MISC (port_label), 1, 0.5);

  port_entry = gtk_entry_new();
  gtk_entry_set_max_length(GTK_ENTRY(port_entry), 6);
  gtk_widget_ref (port_entry);
  g_object_set_data_full(G_OBJECT (dlg_props), "port_entry", port_entry,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (server_box), port_entry, FALSE, TRUE, 5);

  user_box = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (user_box);
  g_object_set_data_full(G_OBJECT (dlg_props), "user_box", user_box,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (data_box), user_box, FALSE, FALSE, 0);

  user_data_box = gtk_vbox_new (FALSE, 10);
  gtk_widget_ref (user_data_box);
  g_object_set_data_full(G_OBJECT (dlg_props), "user_data_box", user_data_box,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (user_box), user_data_box, FALSE, FALSE, 0);

  username_box = gtk_hbox_new (FALSE, 5);
  gtk_widget_ref (username_box);
  g_object_set_data_full(G_OBJECT (dlg_props), "username_box", username_box,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (user_data_box), username_box, TRUE, TRUE, 0);

  username_label = gtk_label_new (_("Username:"));
  gtk_widget_ref (username_label);
  g_object_set_data_full(G_OBJECT (dlg_props), "username_label", username_label,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (username_box), username_label, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (username_label), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (username_label), 1, 0.5);

  username_entry = gtk_entry_new();
  gtk_entry_set_max_length(GTK_ENTRY(username_entry), 16);
  gtk_widget_ref (username_entry);
  g_object_set_data_full(G_OBJECT (dlg_props), "username_entry", username_entry,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (username_box), username_entry, FALSE, FALSE, 5);

  password_box = gtk_hbox_new (FALSE, 5);
  gtk_widget_ref (password_box);
  g_object_set_data_full(G_OBJECT (dlg_props), "password_box", password_box,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (user_data_box), password_box, TRUE, TRUE, 0);

  password_label = gtk_label_new (_("Password:"));
  gtk_widget_ref (password_label);
  g_object_set_data_full(G_OBJECT (dlg_props), "password_label", password_label,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (password_box), password_label, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (password_label), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (password_label), 1, 0.5);

  password_entry = gtk_entry_new ();
  gtk_widget_ref (password_entry);
  g_object_set_data_full(G_OBJECT (dlg_props), "password_entry", password_entry,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (password_box), password_entry, FALSE, FALSE, 5);
  gtk_entry_set_visibility (GTK_ENTRY (password_entry), FALSE);

  confirm_box = gtk_hbox_new (FALSE, 5);
  gtk_widget_ref (confirm_box);
  g_object_set_data_full(G_OBJECT (dlg_props), "confirm_box", confirm_box,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (user_data_box), confirm_box, TRUE, TRUE, 0);

  confirm_label = gtk_label_new (_("Confirm:"));
  gtk_widget_ref (confirm_label);
  g_object_set_data_full(G_OBJECT (dlg_props), "confirm_label", confirm_label,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (confirm_box), confirm_label, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (confirm_label), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (confirm_label), 1, 0.5);

  confirm_entry = gtk_entry_new ();
  gtk_widget_ref (confirm_entry);
  g_object_set_data_full(G_OBJECT (dlg_props), "confirm_entry", confirm_entry,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (confirm_box), confirm_entry, FALSE, FALSE, 5);
  gtk_entry_set_visibility (GTK_ENTRY (confirm_entry), FALSE);

  radio_box = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (radio_box);
  g_object_set_data_full(G_OBJECT (dlg_props), "radio_box", radio_box,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (user_box), radio_box, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (radio_box), 15);

  normal_radio = gtk_radio_button_new_with_label (login_type_group, _("Normal Login"));
  login_type_group = gtk_radio_button_get_group(GTK_RADIO_BUTTON (normal_radio));
  gtk_widget_ref (normal_radio);
  g_object_set_data_full(G_OBJECT (dlg_props), "normal_radio", normal_radio,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (radio_box), normal_radio, FALSE, FALSE, 0);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (normal_radio), TRUE);

  guest_radio = gtk_radio_button_new_with_label (login_type_group, _("Guest Login"));
  login_type_group = gtk_radio_button_get_group(GTK_RADIO_BUTTON (guest_radio));
  gtk_widget_ref (guest_radio);
  g_object_set_data_full(G_OBJECT (dlg_props), "guest_radio", guest_radio,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (radio_box), guest_radio, FALSE, FALSE, 0);

  buttonbox = gtk_hbutton_box_new ();
  gtk_widget_ref (buttonbox);
  g_object_set_data_full(G_OBJECT (dlg_props), "buttonbox", buttonbox,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_end (GTK_BOX (edit_box), buttonbox, FALSE, FALSE, 0);

  add_button = gtk_button_new_from_stock(GTK_STOCK_ADD);
  gtk_widget_ref (add_button);
  g_object_set_data_full(G_OBJECT (dlg_props), "add_button", add_button,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_container_add (GTK_CONTAINER (buttonbox), add_button);
  GTK_WIDGET_SET_FLAGS (add_button, GTK_CAN_DEFAULT);

  modify_button = stockbutton_new(GTK_STOCK_SAVE, _("Modify"));
  gtk_widget_ref (modify_button);
  g_object_set_data_full(G_OBJECT (dlg_props), "modify_button", modify_button,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_container_add (GTK_CONTAINER (buttonbox), modify_button);
  GTK_WIDGET_SET_FLAGS (modify_button, GTK_CAN_DEFAULT);

  delete_button = gtk_button_new_from_stock(GTK_STOCK_DELETE);
  gtk_widget_ref (delete_button);
  g_object_set_data_full(G_OBJECT (dlg_props), "delete_button", delete_button,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_container_add (GTK_CONTAINER (buttonbox), delete_button);
  GTK_WIDGET_SET_FLAGS (delete_button, GTK_CAN_DEFAULT);

  label1 = gtk_label_new (_("Servers"));
  gtk_widget_ref (label1);
  g_object_set_data_full(G_OBJECT (dlg_props), "label1", label1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook), 0), label1);

  chat_box = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (chat_box);
  g_object_set_data_full(G_OBJECT (dlg_props), "chat_box", chat_box,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_container_add (GTK_CONTAINER (notebook), chat_box);
  gtk_container_set_border_width (GTK_CONTAINER (chat_box), 5);

  hbox8 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox8);
  g_object_set_data_full(G_OBJECT (dlg_props), "hbox8", hbox8,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (chat_box), hbox8, FALSE, TRUE, 0);

  label12 = gtk_label_new (_("Chat Font:"));
  gtk_widget_ref (label12);
  g_object_set_data_full(G_OBJECT (dlg_props), "label12", label12,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (hbox8), label12, FALSE, FALSE, 0);

  chat_font = gtk_entry_new ();
  gtk_widget_ref (chat_font);
  g_object_set_data_full(G_OBJECT (dlg_props), "chat_font", chat_font,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (hbox8), chat_font, TRUE, TRUE, 5);
  gtk_editable_set_editable(GTK_EDITABLE(chat_font), FALSE);

#ifdef GTK_STOCK_EDIT
  Font_button = gtk_button_new_from_stock(GTK_STOCK_EDIT);
#else
  Font_button = gtk_button_new_with_label (_("Change"));
#endif
  gtk_widget_ref (Font_button);
  g_object_set_data_full(G_OBJECT (dlg_props), "Font_button", Font_button,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (hbox8), Font_button, FALSE, FALSE, 0);

  chat_table = gtk_table_new (3, 2, TRUE);
  gtk_widget_ref (chat_table);
  g_object_set_data_full(G_OBJECT (dlg_props), "chat_table", chat_table,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (chat_box), chat_table, FALSE, FALSE, 4);

  ignore_check = gtk_check_button_new_with_label (_("Ignore Join/Part Messages"));
  gtk_widget_ref (ignore_check);
  g_object_set_data_full(G_OBJECT (dlg_props), "ignore_check", ignore_check,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_table_attach (GTK_TABLE (chat_table), ignore_check, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  sound_check = gtk_check_button_new_with_label (_("Play Sounds"));
  gtk_widget_ref (sound_check);
  g_object_set_data_full(G_OBJECT (dlg_props), "sound_check", sound_check,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_table_attach (GTK_TABLE (chat_table), sound_check, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  indent_check = gtk_check_button_new_with_label (_("Auto Indent"));
  gtk_widget_ref (indent_check);
  g_object_set_data_full(G_OBJECT (dlg_props), "indent_check", indent_check,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_table_attach (GTK_TABLE (chat_table), indent_check, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  timestamp_check = gtk_check_button_new_with_label (_("Timestamp Chats"));
  gtk_widget_ref (timestamp_check);
  g_object_set_data_full(G_OBJECT (dlg_props), "timestamp_check", timestamp_check,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_table_attach (GTK_TABLE (chat_table), timestamp_check, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  wrap_check = gtk_check_button_new_with_label (_("Word Wrap"));
  gtk_widget_ref (wrap_check);
  g_object_set_data_full(G_OBJECT (dlg_props), "wrap_check", wrap_check,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_table_attach (GTK_TABLE (chat_table), wrap_check, 0, 1, 2, 3,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  frame1 = gtk_frame_new (_("Chat Color"));
  gtk_widget_ref (frame1);
  g_object_set_data_full(G_OBJECT (dlg_props), "frame1", frame1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (chat_box), frame1, TRUE, TRUE, 0);

  vbox9 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox9);
  g_object_set_data_full(G_OBJECT (dlg_props), "vbox9", vbox9,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_container_add (GTK_CONTAINER (frame1), vbox9);

  color_table = gtk_table_new (3, 4, FALSE);
  gtk_widget_ref (color_table);
  g_object_set_data_full(G_OBJECT (dlg_props), "color_table", color_table,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (vbox9), color_table, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (color_table), 3);

  h_spin_adj = gtk_adjustment_new (1, 0, 15, 1, 1, 0);
  h_spin = gtk_spin_button_new (GTK_ADJUSTMENT (h_spin_adj), 1, 0);
  gtk_widget_ref (h_spin);
  g_object_set_data_full(G_OBJECT (dlg_props), "h_spin", h_spin,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_table_attach (GTK_TABLE (color_table), h_spin, 2, 3, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (h_spin), TRUE);

  n_spin_adj = gtk_adjustment_new (1, 0, 15, 1, 1, 0);
  n_spin = gtk_spin_button_new (GTK_ADJUSTMENT (n_spin_adj), 1, 0);
  gtk_widget_ref (n_spin);
  g_object_set_data_full(G_OBJECT (dlg_props), "n_spin", n_spin,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_table_attach (GTK_TABLE (color_table), n_spin, 2, 3, 2, 3,
                    (GtkAttachOptions) (GTK_EXPAND),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (n_spin), TRUE);

  f_spin_adj = gtk_adjustment_new (1, 0, 15, 1, 1, 0);
  f_spin = gtk_spin_button_new (GTK_ADJUSTMENT (f_spin_adj), 1, 0);
  gtk_widget_ref (f_spin);
  g_object_set_data_full(G_OBJECT (dlg_props), "f_spin", f_spin,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_table_attach (GTK_TABLE (color_table), f_spin, 2, 3, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (f_spin), TRUE);

  f_label = gtk_label_new (_("Default chat color assigned to your friends"));
  gtk_widget_ref (f_label);
  g_object_set_data_full(G_OBJECT (dlg_props), "f_label", f_label,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_table_attach (GTK_TABLE (color_table), f_label, 3, 4, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (f_label), 7.45058e-09, 0.5);

  h_label = gtk_label_new (_("Chat color used when your name is typed"));
  gtk_widget_ref (h_label);
  g_object_set_data_full(G_OBJECT (dlg_props), "h_label", h_label,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_table_attach (GTK_TABLE (color_table), h_label, 3, 4, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (h_label), 0, 0.5);

  n_label = gtk_label_new (_("Chat color used for all other chats"));
  gtk_widget_ref (n_label);
  g_object_set_data_full(G_OBJECT (dlg_props), "n_label", n_label,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_table_attach (GTK_TABLE (color_table), n_label, 3, 4, 2, 3,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (n_label), 0, 0.5);

  label26 = gtk_label_new (_("Normal Color"));
  gtk_widget_ref (label26);
  g_object_set_data_full(G_OBJECT (dlg_props), "label26", label26,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_table_attach (GTK_TABLE (color_table), label26, 1, 2, 2, 3,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 8, 0);
  gtk_label_set_justify (GTK_LABEL (label26), GTK_JUSTIFY_LEFT);
  gtk_label_set_line_wrap (GTK_LABEL (label26), TRUE);
  gtk_misc_set_alignment (GTK_MISC (label26), 1, 0.5);

  label25 = gtk_label_new (_("Highlight Color"));
  gtk_widget_ref (label25);
  g_object_set_data_full(G_OBJECT (dlg_props), "label25", label25,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_table_attach (GTK_TABLE (color_table), label25, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 7, 0);
  gtk_label_set_justify (GTK_LABEL (label25), GTK_JUSTIFY_LEFT);
  gtk_label_set_line_wrap (GTK_LABEL (label25), TRUE);
  gtk_misc_set_alignment (GTK_MISC (label25), 1, 0.5);

  label24 = gtk_label_new (_("Friend Color"));
  gtk_widget_ref (label24);
  g_object_set_data_full(G_OBJECT (dlg_props), "label24", label24,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_table_attach (GTK_TABLE (color_table), label24, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 7, 0);
  gtk_label_set_justify (GTK_LABEL (label24), GTK_JUSTIFY_LEFT);
  gtk_label_set_line_wrap (GTK_LABEL (label24), TRUE);
  gtk_misc_set_alignment (GTK_MISC (label24), 1, 0.5);

  black_radio = gtk_radio_button_new_with_label (Chat_Color_group, _("Black Background"));
  Chat_Color_group = gtk_radio_button_get_group(GTK_RADIO_BUTTON (black_radio));
  gtk_widget_ref (black_radio);
  g_object_set_data_full(G_OBJECT (dlg_props), "black_radio", black_radio,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_table_attach (GTK_TABLE (color_table), black_radio, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 15, 0);

  white_radio = gtk_radio_button_new_with_label (Chat_Color_group, _("White Background"));
  Chat_Color_group = gtk_radio_button_get_group(GTK_RADIO_BUTTON (white_radio));
  gtk_widget_ref (white_radio);
  g_object_set_data_full(G_OBJECT (dlg_props), "white_radio", white_radio,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_table_attach (GTK_TABLE (color_table), white_radio, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 15, 0);

  label2 = gtk_label_new (_("Chat"));
  gtk_widget_ref (label2);
  g_object_set_data_full(G_OBJECT (dlg_props), "label2", label2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook), 1), label2);

  userinfo_box = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (userinfo_box);
  g_object_set_data_full(G_OBJECT (dlg_props), "userinfo_box", userinfo_box,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_container_add (GTK_CONTAINER (notebook), userinfo_box);

  label16 = gtk_label_new (_("All of the following information is optional."));
  gtk_widget_ref (label16);
  g_object_set_data_full(G_OBJECT (dlg_props), "label16", label16,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (userinfo_box), label16, FALSE, FALSE, 2);

  vbox7 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox7);
  g_object_set_data_full(G_OBJECT (dlg_props), "vbox7", vbox7,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (userinfo_box), vbox7, TRUE, TRUE, 0);

  hbox12 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox12);
  g_object_set_data_full(G_OBJECT (dlg_props), "hbox12", hbox12,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (vbox7), hbox12, FALSE, TRUE, 0);

  label17 = gtk_label_new (_("Name:"));
  gtk_widget_ref (label17);
  g_object_set_data_full(G_OBJECT (dlg_props), "label17", label17,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (hbox12), label17, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label17), GTK_JUSTIFY_LEFT);
  gtk_label_set_line_wrap (GTK_LABEL (label17), TRUE);
  gtk_misc_set_padding (GTK_MISC (label17), 5, 0);

  info_name = gtk_entry_new ();
  gtk_widget_ref (info_name);
  g_object_set_data_full(G_OBJECT (dlg_props), "info_name", info_name,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (hbox12), info_name, TRUE, TRUE, 5);

  vbox8 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox8);
  g_object_set_data_full(G_OBJECT (dlg_props), "vbox8", vbox8,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (vbox7), vbox8, FALSE, TRUE, 5);

  hbox13 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox13);
  g_object_set_data_full(G_OBJECT (dlg_props), "hbox13", hbox13,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (vbox8), hbox13, FALSE, TRUE, 0);

  label18 = gtk_label_new (_("City:"));
  gtk_widget_ref (label18);
  g_object_set_data_full(G_OBJECT (dlg_props), "label18", label18,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (hbox13), label18, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label18), GTK_JUSTIFY_LEFT);
  gtk_label_set_line_wrap (GTK_LABEL (label18), TRUE);
  gtk_misc_set_padding (GTK_MISC (label18), 5, 0);

  info_city = gtk_entry_new ();
  gtk_widget_ref (info_city);
  g_object_set_data_full(G_OBJECT (dlg_props), "info_city", info_city,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (hbox13), info_city, TRUE, TRUE, 5);

  hbox14 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox14);
  g_object_set_data_full(G_OBJECT (dlg_props), "hbox14", hbox14,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (vbox7), hbox14, FALSE, TRUE, 0);

  label19 = gtk_label_new (_("State:"));
  gtk_widget_ref (label19);
  g_object_set_data_full(G_OBJECT (dlg_props), "label19", label19,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (hbox14), label19, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label19), GTK_JUSTIFY_LEFT);
  gtk_label_set_line_wrap (GTK_LABEL (label19), TRUE);
  gtk_misc_set_padding (GTK_MISC (label19), 5, 0);

  info_state = gtk_entry_new ();
  gtk_widget_ref (info_state);
  g_object_set_data_full(G_OBJECT (dlg_props), "info_state", info_state,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (hbox14), info_state, TRUE, TRUE, 5);

  hbox15 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox15);
  g_object_set_data_full(G_OBJECT (dlg_props), "hbox15", hbox15,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (vbox7), hbox15, FALSE, TRUE, 5);

  label20 = gtk_label_new (_("Country:"));
  gtk_widget_ref (label20);
  g_object_set_data_full(G_OBJECT (dlg_props), "label20", label20,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (hbox15), label20, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label20), GTK_JUSTIFY_LEFT);
  gtk_label_set_line_wrap (GTK_LABEL (label20), TRUE);
  gtk_misc_set_padding (GTK_MISC (label20), 5, 0);

  info_country = gtk_entry_new ();
  gtk_widget_ref (info_country);
  g_object_set_data_full(G_OBJECT (dlg_props), "info_country", info_country,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (hbox15), info_country, TRUE, TRUE, 5);

  label21 = gtk_label_new (_("Comments, Hobbies, Etc."));
  gtk_widget_ref (label21);
  g_object_set_data_full(G_OBJECT (dlg_props), "label21", label21,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (vbox7), label21, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label21), GTK_JUSTIFY_LEFT);
  gtk_label_set_line_wrap (GTK_LABEL (label21), TRUE);

  scrolledwindow1 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_ref (scrolledwindow1);
  g_object_set_data_full(G_OBJECT (dlg_props), "scrolledwindow1", scrolledwindow1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (vbox7), scrolledwindow1, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (scrolledwindow1), 5);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow1), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);

  info_comments = gtk_text_view_new_with_buffer(gtk_text_buffer_new(NULL));
  gtk_widget_ref (info_comments);
  g_object_set_data_full(G_OBJECT (dlg_props), "info_comments", info_comments,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_container_add (GTK_CONTAINER (scrolledwindow1), info_comments);

  label3 = gtk_label_new (_("User Information"));
  gtk_widget_ref (label3);
  g_object_set_data_full(G_OBJECT (dlg_props), "label3", label3,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook), 2), label3);

  table1 = gtk_table_new (3, 2, FALSE);
  gtk_container_add (GTK_CONTAINER (notebook), table1);

  click_checkbutton = gtk_check_button_new_with_label (_("Single Click Room Entry"));
  g_object_set_data(G_OBJECT (dlg_props),
		    "click_checkbutton", click_checkbutton);
  gtk_table_attach_defaults(GTK_TABLE(table1), click_checkbutton, 0, 2, 1, 2);

  frame2 = gtk_frame_new (_("MOTD"));
  gtk_table_attach_defaults(GTK_TABLE(table1), frame2, 0, 2, 2, 3);

  table2 = gtk_table_new (2, 2, TRUE);
  gtk_widget_ref (table2);
  g_object_set_data_full(G_OBJECT (dlg_props), "table2", table2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_container_add (GTK_CONTAINER (frame2), table2);

  motd_all_radio = gtk_radio_button_new_with_label (motd_group, _("Display All"));
  motd_group = gtk_radio_button_get_group(GTK_RADIO_BUTTON (motd_all_radio));
  gtk_widget_ref (motd_all_radio);
  g_object_set_data_full(G_OBJECT (dlg_props), "motd_all_radio", motd_all_radio,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_table_attach (GTK_TABLE (table2), motd_all_radio, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  motd_new_radio = gtk_radio_button_new_with_label (motd_group, _("Display New"));
  motd_group = gtk_radio_button_get_group(GTK_RADIO_BUTTON (motd_new_radio));
  gtk_widget_ref (motd_new_radio);
  g_object_set_data_full(G_OBJECT (dlg_props), "motd_new_radio", motd_new_radio,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_table_attach (GTK_TABLE (table2), motd_new_radio, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  motd_important_radio = gtk_radio_button_new_with_label (motd_group, _("Display Important"));
  motd_group = gtk_radio_button_get_group(GTK_RADIO_BUTTON (motd_important_radio));
  gtk_widget_ref (motd_important_radio);
  g_object_set_data_full(G_OBJECT (dlg_props), "motd_important_radio", motd_important_radio,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_table_attach (GTK_TABLE (table2), motd_important_radio, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  motd_none_radio = gtk_radio_button_new_with_label (motd_group, _("Display None"));
  motd_group = gtk_radio_button_get_group(GTK_RADIO_BUTTON (motd_none_radio));
  gtk_widget_ref (motd_none_radio);
  g_object_set_data_full(G_OBJECT (dlg_props), "motd_none_radio", motd_none_radio,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_table_attach (GTK_TABLE (table2), motd_none_radio, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  label67 = gtk_label_new (_("Options"));
  gtk_widget_ref (label67);
  g_object_set_data_full(G_OBJECT (dlg_props), "label67", label67,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook), 3), label67);

  hbuttonbox1 = gtk_hbutton_box_new ();
  gtk_widget_ref (hbuttonbox1);
  g_object_set_data_full(G_OBJECT (dlg_props), "hbuttonbox1", hbuttonbox1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (dlg_props), hbuttonbox1, TRUE, TRUE, 0);

  button1 = gtk_button_new_from_stock(GTK_STOCK_OK);
  gtk_widget_ref (button1);
  g_object_set_data_full(G_OBJECT (dlg_props), "button1", button1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_container_add (GTK_CONTAINER (hbuttonbox1), button1);
  GTK_WIDGET_SET_FLAGS (button1, GTK_CAN_DEFAULT);

  button2 = gtk_button_new_from_stock(GTK_STOCK_APPLY);
  gtk_widget_ref (button2);
  g_object_set_data_full(G_OBJECT (dlg_props), "button2", button2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_container_add (GTK_CONTAINER (hbuttonbox1), button2);
  GTK_WIDGET_SET_FLAGS (button2, GTK_CAN_DEFAULT);

  button3 = gtk_button_new_from_stock(GTK_STOCK_CANCEL);
  gtk_widget_ref (button3);
  g_object_set_data_full(G_OBJECT (dlg_props), "button3", button3,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_container_add (GTK_CONTAINER (hbuttonbox1), button3);
  GTK_WIDGET_SET_FLAGS (button3, GTK_CAN_DEFAULT);

  g_signal_connect (GTK_OBJECT (dlg_props), "realize",
                      GTK_SIGNAL_FUNC (dlg_props_realize),
                      NULL);
  g_signal_connect (GTK_OBJECT (dlg_props), "destroy",
                      GTK_SIGNAL_FUNC (gtk_widget_destroyed),
                      &props_dialog);
  g_signal_connect (GTK_OBJECT (props_profile_box), "realize",
                      GTK_SIGNAL_FUNC (props_profile_box_realized),
                      NULL);
  g_signal_connect (GTK_OBJECT (profile_entry), "changed",
                      GTK_SIGNAL_FUNC (props_profile_entry_changed),
                      NULL);
  g_signal_connect (GTK_OBJECT (normal_radio), "toggled",
                      GTK_SIGNAL_FUNC (props_normal_toggled),
                      dlg_props);
  g_signal_connect (GTK_OBJECT (add_button), "clicked",
                      GTK_SIGNAL_FUNC (props_add_button_clicked),
                      NULL);
  g_signal_connect (GTK_OBJECT (modify_button), "clicked",
                      GTK_SIGNAL_FUNC (props_modify_button_clicked),
                      NULL);
  g_signal_connect (GTK_OBJECT (delete_button), "clicked",
                      GTK_SIGNAL_FUNC (props_delete_button_clicked),
                      NULL);
  g_signal_connect (GTK_OBJECT (Font_button), "clicked",
                      GTK_SIGNAL_FUNC (props_Font_button_clicked),
                      NULL);
  g_signal_connect (GTK_OBJECT (black_radio), "toggled",
                      GTK_SIGNAL_FUNC (props_color_type_toggled),
                      NULL);
  g_signal_connect (GTK_OBJECT (white_radio), "toggled",
                      GTK_SIGNAL_FUNC (props_color_type_toggled),
                      NULL);
  g_signal_connect (GTK_OBJECT (button1), "clicked",
                      GTK_SIGNAL_FUNC (props_ok_button_clicked),
                      NULL);
  g_signal_connect (GTK_OBJECT (button2), "clicked",
                      GTK_SIGNAL_FUNC (props_apply_button_clicked),
                      NULL);
  g_signal_connect (GTK_OBJECT (button3), "clicked",
                      GTK_SIGNAL_FUNC (props_cancel_button_clicked),
                      NULL);

  gtk_widget_show_all(dlg_props);

  return dlg_props;
}

GtkWidget*
create_dlg_props_font (void)
{
  GtkWidget *dlg_props_font;
  GtkWidget *ok_button1;
  GtkWidget *cancel_button1;
  GtkWidget *apply_button1;

  dlg_props_font = gtk_font_selection_dialog_new (_("Select Font"));
  g_object_set_data(G_OBJECT (dlg_props_font), "dlg_props_font", dlg_props_font);
  gtk_container_set_border_width (GTK_CONTAINER (dlg_props_font), 4);
  gtk_window_set_resizable(GTK_WINDOW(dlg_props_font), TRUE);

  ok_button1 = GTK_FONT_SELECTION_DIALOG (dlg_props_font)->ok_button;
  g_object_set_data(G_OBJECT (dlg_props_font), "ok_button1", ok_button1);
  GTK_WIDGET_SET_FLAGS (ok_button1, GTK_CAN_DEFAULT);

  cancel_button1 = GTK_FONT_SELECTION_DIALOG (dlg_props_font)->cancel_button;
  g_object_set_data(G_OBJECT (dlg_props_font), "cancel_button1", cancel_button1);
  GTK_WIDGET_SET_FLAGS (cancel_button1, GTK_CAN_DEFAULT);

  apply_button1 = GTK_FONT_SELECTION_DIALOG (dlg_props_font)->apply_button;
  g_object_set_data(G_OBJECT (dlg_props_font), "apply_button1", apply_button1);
  GTK_WIDGET_SET_FLAGS (apply_button1, GTK_CAN_DEFAULT);

  g_signal_connect (GTK_OBJECT (dlg_props_font), "destroy",
                      GTK_SIGNAL_FUNC (gtk_widget_destroyed),
                      &props_font_dialog);
  g_signal_connect (GTK_OBJECT (ok_button1), "clicked",
                      GTK_SIGNAL_FUNC (props_fok_button_clicked),
                      NULL);
  g_signal_connect (GTK_OBJECT (cancel_button1), "clicked",
                      GTK_SIGNAL_FUNC (props_fcancel_button_clicked),
                      NULL);
  g_signal_connect (GTK_OBJECT (apply_button1), "clicked",
                      GTK_SIGNAL_FUNC (props_fapply_button_clicked),
                      NULL);

  return dlg_props_font;
}

