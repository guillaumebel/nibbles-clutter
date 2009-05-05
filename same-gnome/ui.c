/* -*- mode: C; indent-tabs-mode: t; tab-width: 2; c-basic-offset: 2; -*- */

/* ui.c : User interface handling.
 *
 * Copyright (c) 2004 by Callum McKenzie
 *
 */

#include <config.h>

#include <gtk/gtk.h>
#include <glib/gi18n.h>

#include <libgames-support/games-conf.h>
#include <libgames-support/games-files.h>
#include <libgames-support/games-gridframe.h>
#include <libgames-support/games-help.h>
#include <libgames-support/games-scores-dialog.h>
#include <libgames-support/games-stock.h>
#include <libgames-support/games-runtime.h>

#include "same-gnome.h"

#include "drawing.h"
#include "game.h"
#include "input.h"
#include "ui.h"

#define DEFAULT_WINDOW_WIDTH 450
#define DEFAULT_WINDOW_HEIGHT 350

/* A collection of widgets we need to reference repeatedly. */
GtkWidget *application;
GtkWidget *messagewidget;
GtkWidget *scorewidget;
GtkWidget *gridframe = NULL;

GtkAction *undo_action;
GtkAction *redo_action;
GtkAction *fullscreen_action;
GtkAction *leave_fullscreen_action;
GtkToggleAction *animation_action;

/* The index for the current theme in the theme list. This shouldn't
 * really be a global variable, but C sucks. */
/* FIXME: C shouldn't suck. */
gint current_theme_index;
gint theme_index_counter;

/* All quit events must go through here to ensure consistant behaviour. */
static void
quit_cb (void)
{
  /* need mutex on game_state to avoid funky save data? */
  save_game ();

  gtk_main_quit ();
}

void
set_message (gint count)
{
  gchar *message;
  gint s;

  if (count < 2) {
    gtk_label_set_text (GTK_LABEL (messagewidget), "");
    return;
  }

  if (count == 2) {
    gtk_label_set_text (GTK_LABEL (messagewidget), _("No points"));
    return;
  }

  s = calculate_score (count);
  message = g_strdup_printf (ngettext ("%d point", "%d points", s), s);
  gtk_label_set_text (GTK_LABEL (messagewidget), message);
  g_free (message);
}

void
set_message_general (gchar * message)
{
  gtk_label_set_text (GTK_LABEL (messagewidget), message);
}

void
show_score (gint score)
{
  gchar *label;

  label = g_strdup_printf (_("Score: %d"), score);
  gtk_label_set_text (GTK_LABEL (scorewidget), label);
  g_free (label);
}

void
new_frame_ratio (gint board_width, gint board_height)
{
  /* FIXME: The flow should avoid the need for this test. */
  if (gridframe)
    games_grid_frame_set (GAMES_GRID_FRAME (gridframe), board_width,
			  board_height);
}

static void
about_cb (GtkWidget * widget)
{
  const gchar *const authors[] = { "Callum McKenzie", NULL };

  const gchar *const documenters[] = { "Callum McKenzie", NULL };

  gchar *license = games_get_license (_(APPNAME_LONG));


  gtk_show_about_dialog (GTK_WINDOW (application),
#if GTK_CHECK_VERSION (2, 11, 0)
                         "program-name", _(APPNAME_LONG),
#else
                         "name", _(APPNAME_LONG),
#endif
			 "authors", authors,
			 "documenters", documenters,
			 "comments",
			 _
			 ("I want to play that game! You know, they all go whirly-round and you click on them and they vanish!\n\nSame GNOME is a part of GNOME Games."),
			 "copyright", "Copyright \xc2\xa9 2008 Callum McKenzie",
                         "license", license,
                         "wrap-license", TRUE,
			 "translator-credits", _("translator-credits"),
			 "version", VERSION,
			 "logo-icon-name", "gnome-samegnome",
			 "website", "http://www.gnome.org/projects/gnome-games",
                         "website-label", _("GNOME Games web site"),
                         NULL);
  g_free (license);
}

static GtkWidget *
generate_scores_dialog (void)
{
  GtkWidget *dialog;

  dialog = games_scores_dialog_new (GTK_WINDOW (application), highscores, _("Same GNOME Scores"));

  games_scores_dialog_set_category_description (GAMES_SCORES_DIALOG (dialog),
						_("Size:"));

  return dialog;
}

void
game_over_dialog (gint place)
{
  static GtkWidget *baddialog = NULL;
  static GamesScoresDialog *gooddialog;
  GtkDialog *dialog;
  gchar *message;

  if (place == 0) {		/* Case 1: It was not a top-ten score. */
    if (baddialog) {
      gtk_window_present (GTK_WINDOW (baddialog));
    } else {
      baddialog =
	gtk_message_dialog_new_with_markup (GTK_WINDOW (application),
					    GTK_DIALOG_DESTROY_WITH_PARENT,
					    GTK_MESSAGE_INFO,
					    GTK_BUTTONS_NONE,
					    _("Game over!"));
      gtk_message_dialog_format_secondary_text (GTK_MESSAGE_DIALOG
						(baddialog),
						_
						("Unfortunately your score did not make the top ten."));
      gtk_dialog_add_buttons (GTK_DIALOG (baddialog), GTK_STOCK_QUIT,
			      GTK_RESPONSE_REJECT, _("_New Game"),
			      GTK_RESPONSE_ACCEPT, NULL);
      gtk_dialog_set_default_response (GTK_DIALOG (baddialog),
				       GTK_RESPONSE_ACCEPT);
    }
    dialog = GTK_DIALOG (baddialog);
  } else {			/* Case 2: It was a top ten score. */
    if (gooddialog) {
      gtk_window_present (GTK_WINDOW (gooddialog));
    } else {
      /* We build two, slightly different, high score dialogs. Once here and
       * once for the scores menu item. */
      gooddialog = GAMES_SCORES_DIALOG (generate_scores_dialog ());

      games_scores_dialog_set_buttons (gooddialog,
				       GAMES_SCORES_QUIT_BUTTON |
				       GAMES_SCORES_NEW_GAME_BUTTON);
      message = g_strdup_printf ("<b>%s</b>\n\n%s", _("Congratulations!"),
				 place == 1 ? _("Your score is the best!") :
                                 _("Your score has made the top ten."));
      games_scores_dialog_set_message (gooddialog, message);
      g_free (message);

      gtk_widget_show_all (GTK_WIDGET (gooddialog));
    }
    games_scores_dialog_set_hilight (gooddialog, place);
    dialog = GTK_DIALOG (gooddialog);
  }

  switch (gtk_dialog_run (dialog)) {
  case GTK_RESPONSE_REJECT:
    quit_cb ();
    break;
  case GTK_RESPONSE_ACCEPT:
  default:
    new_game ();
    break;
  }
  gtk_widget_hide (GTK_WIDGET (dialog));
}

static void
new_game_cb (void)
{
  new_game ();
}

static void
change_theme_cb (GtkTreeSelection * selection)
{
  gchar *filename;
  GtkTreeModel *model;
  GtkTreeIter iter;

  gtk_tree_selection_get_selected (selection, &model, &iter);
  gtk_tree_model_get (model, &iter, 1, &filename, -1);

  change_theme (filename);

  g_free (filename);
}

static void
fill_list (gchar * name, GtkListStore * list)
{
  gchar *uiname;
  gchar *suffix;
  GtkTreeIter iter;

  if (g_utf8_collate (name, theme) == 0)
    current_theme_index = theme_index_counter;
  theme_index_counter++;

  /* We strip the final suffix for the displayed name. */
  suffix = g_strrstr (name, ".");
  if (suffix)
    uiname = g_strndup (name, suffix - name);
  else
    uiname = g_strdup (name);

  /* Capitalise the first letter. This won't work wonderfully in
   * general, but it will be just fine for the themes we currently
   * supply. */
  *uiname = g_unichar_toupper (*uiname);

  gtk_list_store_append (list, &iter);
  gtk_list_store_set (list, &iter, 0, uiname, 1, name, -1);

  /* the store keeps a copy */
  g_free (uiname);
}

static void
theme_cb (void)
{
  static GtkWidget *dialog = NULL;
  GtkWidget *listview;
  GtkWidget *scroll;
  GtkTreeViewColumn *column;
  GtkTreeSelection *selection;
  GtkListStore *list;
  GamesFileList *filelist;
  char *themedir;

  if (dialog) {
    gtk_window_present (GTK_WINDOW (dialog));
  } else {
    dialog = gtk_dialog_new_with_buttons (_("Same GNOME Theme"),
					  GTK_WINDOW (application),
					  GTK_DIALOG_DESTROY_WITH_PARENT |
					  GTK_DIALOG_NO_SEPARATOR,
					  GTK_STOCK_CLOSE,
					  GTK_RESPONSE_ACCEPT, NULL);
    gtk_container_set_border_width (GTK_CONTAINER (dialog), 5);
    gtk_box_set_spacing (GTK_BOX (GTK_DIALOG (dialog)->vbox), 2);
    gtk_dialog_set_default_response (GTK_DIALOG (dialog),
				     GTK_RESPONSE_ACCEPT);
    g_signal_connect (G_OBJECT (dialog), "response",
		      G_CALLBACK (gtk_widget_hide), NULL);
    g_signal_connect (G_OBJECT (dialog), "delete_event",
		      G_CALLBACK (gtk_widget_hide), NULL);

    scroll = gtk_scrolled_window_new (NULL, NULL);
    gtk_container_set_border_width (GTK_CONTAINER (scroll), 5);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scroll),
				    GTK_POLICY_AUTOMATIC,
				    GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scroll),
					 GTK_SHADOW_ETCHED_IN);
    gtk_widget_set_size_request (scroll, 250, 250);
    gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dialog)->vbox), scroll,
			TRUE, TRUE, 0);

    list = gtk_list_store_new (2, G_TYPE_STRING, G_TYPE_STRING);

    current_theme_index = -1;
    theme_index_counter = 0;
    themedir = g_build_filename (games_runtime_get_directory (GAMES_RUNTIME_GAME_THEME_DIRECTORY), THEME_VERSION, NULL);
    filelist = games_file_list_new_images (themedir, localthemedir, NULL);
    games_file_list_transform_basename (filelist);
    games_file_list_for_each (filelist, (GFunc) fill_list, list);
    g_object_unref (filelist);
    g_free (themedir);

    listview = gtk_tree_view_new_with_model (GTK_TREE_MODEL (list));
    g_object_unref (list);

    gtk_container_add (GTK_CONTAINER (scroll), listview);
    column = gtk_tree_view_column_new_with_attributes (_("Theme"),
						       gtk_cell_renderer_text_new
						       (), "text", 0, NULL);
    gtk_tree_view_append_column (GTK_TREE_VIEW (listview),
				 GTK_TREE_VIEW_COLUMN (column));
    selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (listview));
    gtk_tree_selection_set_mode (selection, GTK_SELECTION_BROWSE);
    if (current_theme_index >= 0) {
      GtkTreePath *path =
	gtk_tree_path_new_from_indices (current_theme_index, -1);
      gtk_tree_selection_select_path (selection, path);
      gtk_tree_path_free (path);
    } else {
      gtk_tree_selection_unselect_all (selection);
    }

    g_signal_connect (G_OBJECT (selection), "changed",
		      G_CALLBACK (change_theme_cb), NULL);

    gtk_widget_show_all (dialog);
  }
}

static void
set_fullscreen_actions (gboolean is_fullscreen)
{
  gtk_action_set_sensitive (leave_fullscreen_action, is_fullscreen);
  gtk_action_set_visible (leave_fullscreen_action, is_fullscreen);

  gtk_action_set_sensitive (fullscreen_action, !is_fullscreen);
  gtk_action_set_visible (fullscreen_action, !is_fullscreen);
}

static void
fullscreen_cb (GtkAction * action)
{
  if (action == fullscreen_action) {
    gtk_window_fullscreen (GTK_WINDOW (application));
  } else {
    gtk_window_unfullscreen (GTK_WINDOW (application));
  }
}

static void
animation_cb (GtkToggleAction * action)
{
  fast_animation_enable (gtk_toggle_action_get_active (action));
}

/* Just in case something else takes us to/from fullscreen. */
static gboolean
window_state_cb (GtkWidget * widget, GdkEventWindowState * event)
{
  if (!(event->changed_mask & GDK_WINDOW_STATE_FULLSCREEN))
    return FALSE;

  set_fullscreen_actions (event->new_window_state &
			  GDK_WINDOW_STATE_FULLSCREEN);
    
  return FALSE;
}

static void
scores_cb (void)
{
  static GtkWidget *dialog = NULL;

  if (dialog) {
    gtk_window_present (GTK_WINDOW (dialog));
  } else {
    dialog = generate_scores_dialog ();
  }

  gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_hide (dialog);
}

static void
undo_cb (void)
{
  /* If we are in the middle of the animations we don't do a proper undo 
   * (because we haven't worked out the new state yet). Instead we reset
   * to the currently recorded state. */
  if ((game_state == GAME_DESTROYING) ||
      (game_state == GAME_MOVING_DOWN) || (game_state == GAME_MOVING_LEFT))
    restore_game_state ();
  else
    undo ();
  redraw ();
  select_cells ();
}

static void
redo_cb (void)
{
  /* FIXME: If we undo in the middle of the animation then we can't redo
   * that move. This is an artifact of how we work out the next state 
   * (during the animation) and our work-around so undo behaves sanely.
   * At the moment this is a trade-off. Ideally we work out the new state
   * immediately (this will also allow us to make te animation quicker
   * by allowing already fallen columns to collapse leftward). However
   * this would make the animation code more complicated. */
  redo ();
  redraw ();
  select_cells ();
}

static void
size_change_cb (GtkRadioAction * action, gpointer data)
{
  set_sizes (gtk_radio_action_get_current_value (action));
  resize_graphics ();
  new_game ();
}

static void
help_cb (void)
{
  games_help_display (application, "same-gnome", NULL);
}

/* FIXME: Will we ever want this ? */
#if 0
static void
custom_size_cb (void)
{

}
#endif

static const GtkActionEntry actions[] = {
  {"GameMenu", NULL, N_("_Game")},
  {"ViewMenu", NULL, N_("_View")},
  {"SizeMenu", NULL, N_("_Size")},
  {"HelpMenu", NULL, N_("_Help")},

  {"NewGame", GAMES_STOCK_NEW_GAME, NULL, NULL, NULL,
   G_CALLBACK (new_game_cb)},
  {"Scores", GAMES_STOCK_SCORES, NULL, NULL, NULL, G_CALLBACK (scores_cb)},
  {"UndoMove", GAMES_STOCK_UNDO_MOVE, NULL, NULL, NULL, G_CALLBACK (undo_cb)},
  {"RedoMove", GAMES_STOCK_REDO_MOVE, NULL, NULL, NULL, G_CALLBACK (redo_cb)},
  {"Quit", GTK_STOCK_QUIT, NULL, NULL, NULL, G_CALLBACK (quit_cb)},

  {"Fullscreen", GAMES_STOCK_FULLSCREEN, NULL, NULL, NULL,
   G_CALLBACK (fullscreen_cb)},
  {"LeaveFullscreen", GAMES_STOCK_LEAVE_FULLSCREEN, NULL, NULL, NULL,
   G_CALLBACK (fullscreen_cb)},
  {"Theme", NULL, N_("_Theme..."), NULL, NULL, G_CALLBACK (theme_cb)},

  {"Contents", GAMES_STOCK_CONTENTS, NULL, NULL, NULL, G_CALLBACK (help_cb)},
  {"About", GTK_STOCK_ABOUT, NULL, NULL, NULL, G_CALLBACK (about_cb)}
};

const GtkRadioActionEntry radio_actions[] = {
  {"SizeSmall", NULL, N_("_Small"), NULL, NULL, SMALL},
  {"SizeMedium", NULL, N_("_Medium"), NULL, NULL, MEDIUM},
  {"SizeLarge", NULL, N_("_Large"), NULL, NULL, LARGE}
};

const GtkToggleActionEntry toggle_actions[] = {
  {"Animation", NULL, N_("_Fast Animation"), NULL, NULL,
   G_CALLBACK (animation_cb)}
};

static const char ui_description[] =
  "<ui>"
  "  <menubar name='MainMenu'>"
  "    <menu action='GameMenu'>"
  "      <menuitem action='NewGame'/>"
  "      <menuitem action='Scores'/>"
  "      <separator/>"
  "      <menuitem action='UndoMove'/>"
  "      <menuitem action='RedoMove'/>"
  "      <separator/>"
  "      <menuitem action='Quit'/>"
  "    </menu>"
  "    <menu action='ViewMenu'>"
  "      <menuitem action='Fullscreen'/>"
  "      <menuitem action='LeaveFullscreen'/>"
  "      <menuitem action='Animation'/>"
  "      <menuitem action='Theme'/>"
  "    </menu>"
  "    <menu action='SizeMenu'>"
  "      <menuitem action='SizeSmall'/>"
  "      <menuitem action='SizeMedium'/>"
  "      <menuitem action='SizeLarge'/>"
  "    </menu>"
  "    <menu action='HelpMenu'>"
  "      <menuitem action='Contents'/>"
  "      <menuitem action='About'/>"
  "    </menu>"
  "  </menubar>"
  "</ui>";

void
build_gui (void)
{
  GtkWidget *hbox;
  GtkWidget *canvas;
  GtkWidget *vbox;
  GtkUIManager *ui_manager;
  GtkActionGroup *action_group;

  gtk_window_set_default_icon_name ("gnome-samegnome");

  application = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (application), _(APPNAME_LONG));
  gtk_window_set_default_size (GTK_WINDOW (application),
                               DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
  games_conf_add_window (GTK_WINDOW (application), NULL);

  g_signal_connect (G_OBJECT (application), "delete_event",
		    G_CALLBACK (quit_cb), NULL);
  g_signal_connect (G_OBJECT (application), "window_state_event",
		    G_CALLBACK (window_state_cb), NULL);

  vbox = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (application), vbox);

  action_group = gtk_action_group_new ("MenuActions");
  gtk_action_group_set_translation_domain (action_group, GETTEXT_PACKAGE);
  gtk_action_group_add_actions (action_group, actions, G_N_ELEMENTS (actions),
				NULL);
  gtk_action_group_add_radio_actions (action_group, radio_actions,
				      G_N_ELEMENTS (radio_actions),
				      game_size,
				      G_CALLBACK (size_change_cb), NULL);
  gtk_action_group_add_toggle_actions (action_group, toggle_actions,
				       G_N_ELEMENTS (toggle_actions), NULL);

  undo_action = gtk_action_group_get_action (action_group, "UndoMove");
  redo_action = gtk_action_group_get_action (action_group, "RedoMove");
  fullscreen_action =
    gtk_action_group_get_action (action_group, "Fullscreen");
  leave_fullscreen_action =
    gtk_action_group_get_action (action_group, "LeaveFullscreen");
  animation_action =
    GTK_TOGGLE_ACTION (gtk_action_group_get_action
		       (action_group, "Animation"));
  set_fullscreen_actions (FALSE);

  gtk_toggle_action_set_active (animation_action,
                                games_conf_get_boolean (NULL, KEY_FAST_ANIMATION, NULL));

  ui_manager = gtk_ui_manager_new ();
  gtk_ui_manager_insert_action_group (ui_manager, action_group, 1);
  gtk_ui_manager_add_ui_from_string (ui_manager, ui_description, -1, NULL);

  gtk_window_add_accel_group (GTK_WINDOW (application),
			      gtk_ui_manager_get_accel_group (ui_manager));

  gtk_box_pack_start (GTK_BOX (vbox),
		      gtk_ui_manager_get_widget (ui_manager, "/MainMenu"),
		      FALSE, FALSE, 0);

  gridframe = games_grid_frame_new (board_width, board_height);
  games_grid_frame_set_padding (GAMES_GRID_FRAME (gridframe), 1, 1);
  gtk_box_pack_start (GTK_BOX (vbox), gridframe, TRUE, TRUE, 0);

  canvas = gtk_drawing_area_new ();
  g_object_set (G_OBJECT (canvas), "can-focus", TRUE, NULL);
  gtk_widget_add_events (canvas, GDK_KEY_PRESS_MASK |
			 GDK_POINTER_MOTION_MASK |
			 GDK_BUTTON_PRESS_MASK | GDK_LEAVE_NOTIFY_MASK);
  g_signal_connect (G_OBJECT (canvas), "configure_event",
		    G_CALLBACK (configure_cb), NULL);
  g_signal_connect (G_OBJECT (canvas), "expose_event",
		    G_CALLBACK (expose_cb), NULL);
  g_signal_connect (G_OBJECT (canvas), "key_press_event",
		    G_CALLBACK (keyboard_cb), NULL);
  g_signal_connect (G_OBJECT (canvas), "motion_notify_event",
		    G_CALLBACK (mouse_movement_cb), NULL);
  g_signal_connect (G_OBJECT (canvas), "leave_notify_event",
		    G_CALLBACK (mouse_leave_cb), NULL);
  g_signal_connect (G_OBJECT (canvas), "button_press_event",
		    G_CALLBACK (mouse_click_cb), NULL);
  gtk_widget_set_size_request (canvas, MINIMUM_CANVAS_WIDTH,
			       MINIMUM_CANVAS_HEIGHT);
  gtk_container_add (GTK_CONTAINER (gridframe), canvas);

  gtk_box_pack_start (GTK_BOX (vbox), gtk_hseparator_new (), FALSE, FALSE, 0);

  hbox = gtk_hbox_new (TRUE, 0);
  gtk_box_pack_end (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);

  messagewidget = gtk_label_new (NULL);
  gtk_box_pack_start (GTK_BOX (hbox), messagewidget, TRUE, FALSE, 0);

  scorewidget = gtk_label_new (NULL);
  gtk_box_pack_start (GTK_BOX (hbox), scorewidget, TRUE, FALSE, 0);

  gtk_widget_show_all (application);
}

void
set_undoredo_sensitive (gboolean undo, gboolean redo)
{
  gtk_action_set_sensitive (undo_action, undo);
  gtk_action_set_sensitive (redo_action, redo);
}
