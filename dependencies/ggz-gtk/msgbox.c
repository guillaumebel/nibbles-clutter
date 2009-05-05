/*
 * File: msgbox.c
 * Author: Justin Zaun
 * Project: GGZ GTK Client
 * $Id$
 *
 * This is the main program body for the GGZ client
 *
 * Copyright (C) 2000, 2001 Justin Zaun.
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

#include "client.h"
#include "msgbox.h"
#include "support.h"

/* Creates a button with a stock icon and custom text.  Code stolen
   from FreeCiv.  Thanks Vasco (or glade; who knows?). */
GtkWidget *stockbutton_new(const gchar *stock, const gchar *label_text)
{
	GtkWidget *label;
	GtkWidget *image;
	GtkWidget *hbox;
	GtkWidget *align;
	GtkWidget *button;

	button = gtk_button_new();

	label = gtk_label_new_with_mnemonic(label_text);
	gtk_label_set_mnemonic_widget(GTK_LABEL(label), button);

	image = gtk_image_new_from_stock(stock, GTK_ICON_SIZE_BUTTON);
	hbox = gtk_hbox_new(FALSE, 2);

	align = gtk_alignment_new(0.5, 0.5, 0.0, 0.0);

	gtk_box_pack_start(GTK_BOX (hbox), image, FALSE, FALSE, 0);
	gtk_box_pack_end(GTK_BOX (hbox), label, FALSE, FALSE, 0);

	gtk_container_add(GTK_CONTAINER(button), align);
	gtk_container_add(GTK_CONTAINER(align), hbox);
	gtk_widget_show_all(align);

	g_object_set_data(G_OBJECT(button), "label", label);
	g_object_set_data(G_OBJECT(button), "image", image);

	return button;
}

void stockbutton_set_text(GtkWidget *button, const gchar *label_text)
{
	GtkWidget *label;

	label = g_object_get_data(G_OBJECT(button), "label");
	gtk_label_set_text(GTK_LABEL(label), label_text);
}

static MBReturn mb_status;

static void CloseTheApp (GtkWidget *window, gpointer data)
{
	gtk_main_quit();
}


static void DialogOKClicked (GtkButton *button, gpointer data)
{
	mb_status = MSGBOX_OK;
	gtk_widget_destroy (GTK_WIDGET (data));
}

static void DialogCancelClicked (GtkButton *button, gpointer data)
{
	mb_status = MSGBOX_CANCEL;
	gtk_widget_destroy (GTK_WIDGET (data));
}

static void DialogYesClicked (GtkButton *button, gpointer data)
{
	mb_status = MSGBOX_YES;
	gtk_widget_destroy (GTK_WIDGET (data));
}

static void DialogNoClicked (GtkButton *button, gpointer data)
{
	mb_status = MSGBOX_NO;
	gtk_widget_destroy (GTK_WIDGET (data));
}

static GtkWidget *AddWidget (GtkWidget *widget, GtkWidget *packingbox)
{
	gtk_box_pack_start (GTK_BOX (packingbox), widget, TRUE, TRUE, 2);
	return widget;
}


MBReturn msgbox(gchar *textmessage, gchar *title, MBType type,
		MBIcon itype, MBModal modal)
{
	GtkWidget *dialogwindow;
	GtkWidget *vbox;
	GtkWidget *buttonbox;
	GtkWidget *hbox;
	GtkWidget *icon = NULL;
	GtkWidget *dialogwidget;
	GtkWidget *btnok;
	GtkWidget *btncancel;
	GtkWidget *btnyes;
	GtkWidget *btnno;

	dialogwindow = gtk_dialog_new();
	gtk_window_set_transient_for(GTK_WINDOW(dialogwindow),
				     GTK_WINDOW(main_window));
	gtk_window_set_title (GTK_WINDOW(dialogwindow), title);
	gtk_container_set_border_width (GTK_CONTAINER (dialogwindow), 10);
	gtk_window_set_position (GTK_WINDOW(dialogwindow), GTK_WIN_POS_CENTER);

	vbox = GTK_DIALOG(dialogwindow)->vbox;

	hbox = gtk_hbox_new (FALSE, 2);
	dialogwidget = AddWidget (hbox, vbox);

	switch (itype) {
	case MSGBOX_STOP:
		icon = gtk_image_new_from_stock(GTK_STOCK_DIALOG_ERROR,
						GTK_ICON_SIZE_DIALOG);
		break;
	case MSGBOX_INFO:
		icon = gtk_image_new_from_stock(GTK_STOCK_DIALOG_INFO,
						GTK_ICON_SIZE_DIALOG);
		break;
	case MSGBOX_QUESTION:
		icon = gtk_image_new_from_stock(GTK_STOCK_DIALOG_QUESTION,
						GTK_ICON_SIZE_DIALOG);
		break;
	case MSGBOX_NONE:
		icon = gtk_image_new_from_stock(GTK_STOCK_DIALOG_WARNING,
						GTK_ICON_SIZE_DIALOG);
		break;
	}

	if(icon) {
		dialogwidget = AddWidget (icon, hbox);
	}

	dialogwidget = AddWidget (gtk_label_new (textmessage), hbox);

	buttonbox = gtk_hbutton_box_new();
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialogwindow)->action_area),
			   buttonbox, FALSE, FALSE, 0);

	btnok = gtk_button_new_from_stock(GTK_STOCK_OK);
	GTK_WIDGET_SET_FLAGS (btnok, GTK_CAN_DEFAULT);

	btncancel = gtk_button_new_from_stock(GTK_STOCK_CANCEL);
	GTK_WIDGET_SET_FLAGS (btncancel, GTK_CAN_DEFAULT);

	btnyes = gtk_button_new_from_stock(GTK_STOCK_YES);
	GTK_WIDGET_SET_FLAGS (btnyes, GTK_CAN_DEFAULT);

	btnno = gtk_button_new_from_stock(GTK_STOCK_NO);
	GTK_WIDGET_SET_FLAGS (btnno, GTK_CAN_DEFAULT);

	if (type == MSGBOX_OKCANCEL)
	{
		dialogwidget = AddWidget (btnok, buttonbox);
		dialogwidget = AddWidget (btncancel, buttonbox);
		gtk_widget_grab_default (btnok);
	} else if (type == MSGBOX_YESNO) {
		dialogwidget = AddWidget (btnyes, buttonbox);
		dialogwidget = AddWidget (btnno, buttonbox);
		gtk_widget_grab_default (btnno);
	} else if (type == MSGBOX_OKONLY) {
		dialogwidget = AddWidget (btnok, buttonbox);
		gtk_widget_grab_default (btnok);
	}

	g_signal_connect (GTK_OBJECT (btnok),
				"clicked",
				GTK_SIGNAL_FUNC (DialogOKClicked),
				dialogwindow);
	g_signal_connect (GTK_OBJECT (btncancel),
				"clicked",
				GTK_SIGNAL_FUNC (DialogCancelClicked),
				dialogwindow);
	g_signal_connect (GTK_OBJECT (btnyes),
				"clicked",
				GTK_SIGNAL_FUNC (DialogYesClicked),
				dialogwindow);
	g_signal_connect (GTK_OBJECT (btnno),
				"clicked",
				GTK_SIGNAL_FUNC (DialogNoClicked),
				dialogwindow);
	g_signal_connect (GTK_OBJECT (dialogwidget),
				"destroy",
				GTK_SIGNAL_FUNC (CloseTheApp),
				dialogwindow);

	if (modal == MSGBOX_MODAL)
	{
		gtk_window_set_modal (GTK_WINDOW (dialogwindow), TRUE);
	}
	gtk_widget_show_all (dialogwindow);
	gtk_main();
	return mb_status;
}

