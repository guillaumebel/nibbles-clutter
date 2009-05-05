/*
 * File: motd.c
 * Author: Justin Zaun
 * Project: GGZ GTK Client
 * $Id$
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

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "chat.h"
#include "client.h"
#include "motd.h"
#include "support.h"
#include "ggz.h"

static GtkWidget *motd_dialog;
static GtkWidget *create_dlg_motd(void);


void motd_create_or_raise(void)
{
	GtkWidget *tmp;

	if (!motd_dialog) {
		motd_dialog = create_dlg_motd();

		tmp = ggz_lookup_widget(motd_dialog, "motd_text");
		gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(tmp),
					    GTK_WRAP_WORD);
		gtk_widget_show(motd_dialog);
	} else {
		gdk_window_show(motd_dialog->window);
		gdk_window_raise(motd_dialog->window);

		/* Clear out what is currently there */
		tmp = ggz_lookup_widget(motd_dialog, "motd_text");
		gtk_text_buffer_set_text(gtk_text_view_get_buffer
					 (GTK_TEXT_VIEW(tmp)), "", -1);
	}

}


void motd_print_line(const gchar * line)
{
	gchar *out = NULL;
	gint lindex = 0;
	gint oindex = 0;
	gint outlen = 0;
	GtkWidget *temp_widget;
	GdkColormap *cmap;
	gint color_index = 0;	/* Black */
	gint letter;

	/* Make shure the motd window it there */
	if (motd_dialog == NULL)
		return;

	cmap = gdk_colormap_get_system();
	if (!gdk_colormap_alloc_color(cmap, &colors[color_index],
				      FALSE, TRUE)) {
		g_error("couldn't allocate color");
	}

	temp_widget =
	    g_object_get_data(G_OBJECT(motd_dialog), "motd_text");

	while (line[lindex] != '\0') {
		if (oindex % 256 == 0) {
			outlen += 256;
			out = ggz_realloc(out, outlen);
		}

		if (line[lindex] == '%') {
			lindex++;
			if (line[lindex] == 'c') {
				lindex++;
				letter = atoi(&line[lindex]);
				if ((letter >= 0) && (letter <= 9)) {
					out[oindex] = '\0';
					/* FIXME: handle colors */
					gtk_text_buffer_insert_at_cursor
					    (gtk_text_view_get_buffer
					     (GTK_TEXT_VIEW(temp_widget)),
					     out, -1);
					color_index = atoi(&line[lindex]);
					cmap = gdk_colormap_get_system();
					if (!gdk_colormap_alloc_color(cmap,
								      &colors
								      [color_index],
								      FALSE,
								      TRUE))
					{
						g_error
						    ("couldn't allocate color");
					}
					oindex = 0;
					outlen = 0;
					lindex++;
				} else {
					lindex--;
					lindex--;
				}
			} else {
				lindex--;
			}
		}
		out[oindex] = line[lindex];
		lindex++;
		oindex++;
	}
	if (oindex % 256 == 0) {
		outlen += 1;
		out = ggz_realloc(out, outlen);
	}
	out[oindex] = '\0';

	/* FIXME: handle colors */
	gtk_text_buffer_insert_at_cursor(gtk_text_view_get_buffer
					 (GTK_TEXT_VIEW(temp_widget)), out,
					 -1);

	ggz_free(out);
}


static GtkWidget *create_dlg_motd(void)
{
	GtkWidget *dlg_motd;
	GtkWidget *dialog_vbox1;
	GtkWidget *motd_scrolledwindow;
	GtkWidget *motd_text;

	dlg_motd = gtk_dialog_new_with_buttons(_("Message of the Day"),
					       GTK_WINDOW(main_window), 0,
					       GTK_STOCK_CLOSE,
					       GTK_RESPONSE_CLOSE, NULL);
	g_object_set_data(G_OBJECT(dlg_motd), "dlg_motd", dlg_motd);
	gtk_widget_set_size_request(dlg_motd, 300, 455);
	gtk_window_set_resizable(GTK_WINDOW(dlg_motd), TRUE);

	dialog_vbox1 = GTK_DIALOG(dlg_motd)->vbox;
	g_object_set_data(G_OBJECT(dlg_motd), "dialog_vbox1",
			  dialog_vbox1);
	gtk_widget_show(dialog_vbox1);

	motd_scrolledwindow = gtk_scrolled_window_new(NULL, NULL);
	gtk_widget_ref(motd_scrolledwindow);
	g_object_set_data_full(G_OBJECT(dlg_motd), "motd_scrolledwindow",
			       motd_scrolledwindow,
			       (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show(motd_scrolledwindow);
	gtk_box_pack_start(GTK_BOX(dialog_vbox1), motd_scrolledwindow,
			   TRUE, TRUE, 0);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW
				       (motd_scrolledwindow),
				       GTK_POLICY_NEVER,
				       GTK_POLICY_AUTOMATIC);

	motd_text =
	    gtk_text_view_new_with_buffer(gtk_text_buffer_new(NULL));
	gtk_text_view_set_editable(GTK_TEXT_VIEW(motd_text), FALSE);
	gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(motd_text), FALSE);
	gtk_widget_ref(motd_text);
	g_object_set_data_full(G_OBJECT(dlg_motd), "motd_text", motd_text,
			       (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show(motd_text);
	gtk_container_add(GTK_CONTAINER(motd_scrolledwindow), motd_text);

	g_signal_connect(dlg_motd, "destroy",
			 GTK_SIGNAL_FUNC(gtk_widget_destroyed),
			 &motd_dialog);
	g_signal_connect(dlg_motd, "response",
			 GTK_SIGNAL_FUNC(gtk_widget_destroy), NULL);

	return dlg_motd;
}
