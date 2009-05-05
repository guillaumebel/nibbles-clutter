/*
 * File: pick_module.c
 * Author: GGZ Dev Team
 * Project: GGZ Text Client 
 * Date: 11/5/2002
 * $Id$
 *
 * Dialog window to pick a module for your game
 *
 * Copyright (C) 2002 GGZ Development Team.
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

#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include <strings.h> /* For strcasecmp */

#include <ggzcore.h>

#include "client.h"
#include "pick_module.h"
#include "support.h"


static GtkWidget *dialog = NULL;
static int choice = 0;
static int preserve = 0;

static void on_pickmodule_ok_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{
	gtk_widget_destroy(dialog);
	gtk_main_quit();
}

static void on_button_toggled(GtkToggleButton * widget, gpointer data)
{
	if (widget->active)
		choice = GPOINTER_TO_INT(data);
}

static void on_preserve_toggled(GtkToggleButton * widget, gpointer data)
{
	preserve = widget->active;
}

static GtkWidget *create_pick_module_dlg(GGZModule **modules, int *modulenumbers)
{
	GtkWidget *pick_module_dlg;
	GtkWidget *vbox1;
	GtkWidget *header_label;
	GSList *group = NULL;
	GtkWidget *frontend;
	GtkWidget *hseparator1;
	GtkWidget *preserve;
	GtkWidget *hbuttonbox1;
	GtkWidget *button1;
	int i;

	pick_module_dlg = gtk_dialog_new();
	gtk_window_set_transient_for(GTK_WINDOW(pick_module_dlg),
				     GTK_WINDOW(main_window));
	g_object_set_data(G_OBJECT(pick_module_dlg), "pick_module_dlg",
			  pick_module_dlg);
	gtk_window_set_resizable(GTK_WINDOW(pick_module_dlg), FALSE);

	vbox1 = GTK_DIALOG(pick_module_dlg)->vbox;
	gtk_container_set_border_width(GTK_CONTAINER(vbox1), 5);

	header_label = gtk_label_new
		(_("Which client would you like to use to play this game?"));
	gtk_widget_ref(header_label);
	g_object_set_data_full(G_OBJECT(pick_module_dlg), "header_label",
				 header_label, (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show(header_label);
	gtk_box_pack_start(GTK_BOX(vbox1), header_label, FALSE, FALSE, 10);

	for (i = 0; modules[i]; i++) {
		const char *fe = ggzcore_module_get_frontend(modules[i]);
		const char *name = ggzcore_module_get_name(modules[i]);
		char text[512];
		snprintf(text, sizeof(text), "%s (%s)", name, fe);

		frontend = gtk_radio_button_new_with_label(group, text);
		group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(frontend));
		gtk_widget_ref(frontend);
		g_object_set_data_full(G_OBJECT(pick_module_dlg),
					 fe,
					 frontend,
					 (GtkDestroyNotify) gtk_widget_unref);
		gtk_widget_show(frontend);
		gtk_box_pack_start(GTK_BOX(vbox1), frontend, FALSE, FALSE, 0);
		gtk_container_set_border_width(GTK_CONTAINER(frontend), 2);

		if (i == 0 || strcasecmp(fe, "gtk") == 0) {
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(frontend), TRUE);
			choice = modulenumbers[i];
		}

		g_signal_connect(GTK_OBJECT(frontend), "toggled",
				   GTK_SIGNAL_FUNC(on_button_toggled),
				   GINT_TO_POINTER(modulenumbers[i]));
	}

	hseparator1 = gtk_hseparator_new();
	gtk_widget_ref(hseparator1);
	g_object_set_data_full(G_OBJECT(pick_module_dlg), "hseparator1",
				 hseparator1,
				 (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show(hseparator1);
	gtk_box_pack_start(GTK_BOX(vbox1), hseparator1, TRUE, TRUE, 0);

	preserve = gtk_check_button_new_with_label(_("Don't ask me again."));
	gtk_widget_ref(preserve);
	g_object_set_data_full(G_OBJECT(pick_module_dlg), "preserve",
				 preserve,
				 (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show(preserve);
	gtk_box_pack_start(GTK_BOX(vbox1), preserve, TRUE, TRUE, 2);

	hbuttonbox1 = gtk_hbutton_box_new();
	gtk_widget_ref(hbuttonbox1);
	g_object_set_data_full(G_OBJECT(pick_module_dlg), "hbuttonbox1",
				 hbuttonbox1,
				 (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show(hbuttonbox1);
	gtk_container_add(GTK_CONTAINER(GTK_DIALOG(pick_module_dlg)->action_area), hbuttonbox1);

	button1 = gtk_button_new_from_stock(GTK_STOCK_OK);
	gtk_widget_ref(button1);
	g_object_set_data_full(G_OBJECT(pick_module_dlg), "button1",
				 button1,
				 (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show(button1);
	gtk_container_add(GTK_CONTAINER(hbuttonbox1), button1);
	GTK_WIDGET_SET_FLAGS(button1, GTK_CAN_DEFAULT);

	g_signal_connect(GTK_OBJECT(preserve), "toggled",
			   GTK_SIGNAL_FUNC(on_preserve_toggled), NULL);
	g_signal_connect(GTK_OBJECT(button1), "clicked",
			   GTK_SIGNAL_FUNC(on_pickmodule_ok_clicked), NULL);


	return pick_module_dlg;
}


int ask_user_to_pick_module(GGZModule **modules, int *modulenumbers, int *dopreserve)
{
	if (dialog)
		return -1; /* FIXME: this shouldn't be allowed to happen */

	preserve = 0;
	dialog = create_pick_module_dlg(modules, modulenumbers);

	gtk_widget_show(dialog);

	gtk_main();

	dialog = NULL;
	*dopreserve = preserve;

	return choice;
}

void cancel_module_picking(void)
{
	if (dialog) {
		gtk_widget_destroy(dialog);
		gtk_main_quit();
		choice = -1;
	}
}
