/*
 * File: msgbox.h
 * Author: Justin Zaun
 * Project: GGZ GTK Client
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

/* Creates a button with a stock icon and custom text. */
GtkWidget *stockbutton_new(const gchar *stock, const gchar *label_text);
void stockbutton_set_text(GtkWidget *button, const gchar *label_text);

/*
 * A very easily cusomisible message box. This message box can be used to 
 * give the user information or get answer to questions.
 *
 * If you can't figure it out from here take a look in client.c to see
 * how it is used
 */


typedef enum {
	MSGBOX_YES	= 1,
	MSGBOX_NO	= 2,
	MSGBOX_OK	= 3,
	MSGBOX_CANCEL	= 4
}	MBReturn;

typedef enum {
	MSGBOX_YESNO	= 1,
	MSGBOX_OKCANCEL	= 2,
	MSGBOX_OKONLY	= 3
}	MBType;

typedef enum {
	MSGBOX_NONE	= 1,
	MSGBOX_STOP	= 2,
	MSGBOX_INFO	= 3,
	MSGBOX_QUESTION	= 4
}	MBIcon;

typedef enum {
	MSGBOX_NORMAL	= 1,
	MSGBOX_MODAL	= 2,
}	MBModal;

MBReturn msgbox(gchar *textmessage, gchar *title, MBType type,
		MBIcon itype, MBModal modal);
