/* -*- mode: C; indent-tabs-mode: t; tab-width: 2; c-basic-offset: 2; -*- */

/* ui.h : User interface code
 *
 * Copyright (c) 2004 by Callum McKenzie
 *
 */

#ifndef UI_H
#define UI_H

#include <gtk/gtk.h>

#define clear_message() set_message (0)

#define APPNAME_LONG N_("Same GNOME")

void show_score (gint score);
void set_message (gint count);
void set_message_general (gchar * message);
void game_over_dialog (gint place);
void new_frame_ratio (gint board_width, gint board_height);
void build_gui (void);
void set_undoredo_sensitive (gboolean undo, gboolean redo);

#endif /* UI_H */
