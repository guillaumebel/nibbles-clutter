/*
 * File: first.c
 * Author: Justin Zaun
 * Project: GGZ GTK Client
 * $Id$
 *
 * Displayes information about the authors and the application.
 *
 * Copyright (C) 2000-2002 Justin Zaun.
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

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include <ggz.h>

#include "client.h"
#include "first.h"
#include "ggzclient.h"
#include "ggz-gtk.h"
#include "ggzcore.h"
#include "login.h"
#include "server.h"
#define gettext_noop
#include "support.h"

static void first_button_yes_activate(GtkButton *button, gpointer data);
static void first_button_no_activate(GtkButton *button, gpointer data);
static void first_generate_password(char *pw);

static char *pw_words[] = { N_("Wizard"),    N_("Deity"),   N_("Sentinel"),   N_("Captain"),
                            N_("Knight"),    N_("Angel"),   N_("Silverlord"), N_("Eagle"),
                            N_("Vampire"),   N_("Chief"),   N_("Colonel"),    N_("General"),
                            N_("Major"),     N_("Scout"),   N_("Lieutenant"), N_("Stalker"),
                            N_("Scientist"), N_("Scholar"), N_("Entity"),     N_("Creator")};

static struct {
	char *name;
	char *host;
	int port;
	int type;
} hosts[] = { /* {"Morat.net (Fast)", "ggz.morat.net", 5688, 1}, */
	      {N_("GGZ Community (fast)"), "live.ggzgamingzone.org", 5688, 1},
	      {N_("Local developer server"), "localhost", 5688, 1} };

static gboolean raised;

gboolean first_is_raised(void)
{
	return raised;
}

void first_raise(void)
{
	raised = TRUE;
	main_activate();
}

static void first_lower(void)
{
	ggzcore_conf_write_int("INIT", "FIRST", 1);
	ggzcore_conf_write_string("INIT", "VERSION", VERSION);
	raised = FALSE;
	ggz_sensitivity_init();
	main_activate();
}

/* first_create_or_raise() - Displays the dialog or raises the
 *                           current dialog
 *
 * Recieves:
 *
 * Returns:   
 */

static void first_button_yes_activate(GtkButton *button, gpointer data)
{
	char **profiles, name[17];
	int i, num_profiles, num_hosts = sizeof(hosts) / sizeof(hosts[0]);

	first_generate_password(name);

	ggzcore_conf_read_list("Servers", "ProfileList", &num_profiles,
			       &profiles);

	profiles = ggz_realloc(profiles,
			       (num_hosts + num_profiles) * sizeof(*profiles));

	for (i = 0; i < num_hosts; i++) {
		char *old;
		int oldi;

		old = ggzcore_conf_read_string(hosts[i].name, "Host",
					       hosts[i].host);
		ggzcore_conf_write_string(hosts[i].name, "Host", old);
		ggz_free(old);

		old = ggzcore_conf_read_string(hosts[i].name, "Login", name);
		ggzcore_conf_write_string(hosts[i].name, "Login", old);
		ggz_free(old);

		oldi = ggzcore_conf_read_int(hosts[i].name, "Port",
					     hosts[i].port);
		ggzcore_conf_write_int(hosts[i].name, "Port", oldi);

		oldi = ggzcore_conf_read_int(hosts[i].name, "Type",
					     hosts[i].type);
		ggzcore_conf_write_int(hosts[i].name, "Type", oldi);

		profiles[num_profiles + i] = ggz_strdup(_(hosts[i].name));
	}

	num_profiles += num_hosts;
	ggzcore_conf_write_list("Servers", "ProfileList",
				num_profiles, profiles);
	for (i = 0; i < num_profiles; i++)
		ggz_free(profiles[i]);
	ggz_free(profiles);

	server_profiles_load();

	first_lower();

	ggzcore_conf_commit();
}


static void first_button_no_activate(GtkButton *button, gpointer data)
{
	first_lower();
	ggzcore_conf_commit();
}

static void first_generate_password(char *pw)   
{
        int word, d1, d2;

	/* rand/srand are more portable than random/srandom */
	srand(time(NULL));
	word = rand() % (sizeof(pw_words) / sizeof(pw_words[0]));
	d1 = rand() % 10;
	d2 = rand() % 10;
        /* FIXME: shouldn't we translate the pw_words entry here?  --JDS */
        sprintf(pw, "%s%d%d", pw_words[word], d1, d2);
}




GtkWidget* create_dlg_first (void)
{
  GtkWidget *frame;
  GtkWidget *vbox3;
  GtkWidget *label5;
  GtkWidget *hbuttonbox2;
  GtkWidget *button_yes;
  GtkWidget *button_no;

  frame = gtk_frame_new("First login");

  vbox3 = gtk_vbox_new (FALSE, 0);
  gtk_container_add(GTK_CONTAINER(frame), vbox3);

  label5 = gtk_label_new (_("This is the first time you are running the GTK+ GGZ Gaming Zone client. Would you like to create some default server profiles?"));
  gtk_box_pack_start (GTK_BOX (vbox3), label5, TRUE, TRUE, 0);
  gtk_label_set_line_wrap (GTK_LABEL (label5), TRUE);

  hbuttonbox2 = gtk_hbutton_box_new ();
  gtk_box_pack_start (GTK_BOX (vbox3), hbuttonbox2, FALSE, TRUE, 0);

  button_yes = gtk_button_new_from_stock(GTK_STOCK_YES);
  gtk_container_add (GTK_CONTAINER (hbuttonbox2), button_yes);
  GTK_WIDGET_SET_FLAGS (button_yes, GTK_CAN_DEFAULT);

  button_no = gtk_button_new_from_stock(GTK_STOCK_NO);
  gtk_container_add (GTK_CONTAINER (hbuttonbox2), button_no);
  GTK_WIDGET_SET_FLAGS (button_no, GTK_CAN_DEFAULT);

  g_signal_connect (GTK_OBJECT (button_yes), "clicked",
                      GTK_SIGNAL_FUNC (first_button_yes_activate),
                      NULL);
  g_signal_connect (GTK_OBJECT (button_no), "clicked",
                      GTK_SIGNAL_FUNC (first_button_no_activate),
                      NULL);

  gtk_widget_show_all(frame);

  return frame;
}
