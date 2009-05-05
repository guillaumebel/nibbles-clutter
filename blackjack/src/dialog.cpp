// -*- mode:C++; tab-width:8; c-basic-offset:8; indent-tabs-mode:nil -*-
/* 
 * Blackjack - dialog.cpp
 *
 * Copyright (C) 2003-2004 William Jon McCann <mccann@jhu.edu>
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

#include <config.h>

#include <stdlib.h>
#include <glib/gi18n.h>
#include <gtk/gtk.h>
#include <libgames-support/games-frame.h>
#include "blackjack.h"
#include "menu.h"
#include "dialog.h"
#include "draw.h"
#include "events.h"

#include "game.h"
#include "hand.h"

using namespace std;

static GtkWidget *hint_dlg  = NULL;
static GtkWidget *deck_edit = NULL;

gboolean
get_insurance_choice (void)
{
        GtkWidget  *dialog;
        const char *message;
        const char *secondary_message;
        gboolean    choice = FALSE;

        message = _("Would you like insurance?");
        /* xgettext:no-c-format */
        secondary_message = _("Insurance is a side wager of 50% of the original wager"
                              " that the dealer has a natural 21 (aka blackjack) that is"
                              " offered when the dealer's face up card is an ace. If the"
                              " dealer has a natural 21 then the player is paid double.");
        
        dialog = gtk_message_dialog_new (GTK_WINDOW (toplevel_window),
                                         GTK_DIALOG_MODAL,
                                         GTK_MESSAGE_QUESTION,
                                         GTK_BUTTONS_YES_NO,
                                         "%s", message);

        gtk_message_dialog_format_secondary_text (GTK_MESSAGE_DIALOG (dialog),
                                                  "%s", secondary_message);

        gtk_container_set_border_width (GTK_CONTAINER (dialog), 6);
        gtk_window_set_title (GTK_WINDOW (dialog), "");
        gtk_dialog_set_default_response (GTK_DIALOG (dialog), GTK_RESPONSE_NO);

        /* add a stock icon? */ 
        switch (gtk_dialog_run (GTK_DIALOG (dialog))) {
        case GTK_RESPONSE_YES: 
                choice = TRUE;
                break;
        default:
                choice = FALSE;
                break;
        }

        gtk_widget_destroy (dialog);

        return choice;
}

static void
hint_destroy_callback (void)
{
        hint_dlg = NULL;
}

void
show_hint_dialog (void)
{
        char *message           = NULL;
        char *secondary_message = NULL;

        if (bj_game_is_first_hand ()) {
                message = g_strdup (_("Deal a new hand"));
                secondary_message = g_strdup (_("Set your wager and click in the white outline to deal a new hand."));
        } else if (! bj_game_is_active ()) {
                message = g_strdup (_("Deal a new hand"));
                secondary_message = g_strdup (_("Set your wager or click on the cards to deal a new hand."));
        } else {
                message = bj_hand_get_best_option_string (&secondary_message);
        }
  
        if (hint_dlg)
                gtk_widget_destroy (GTK_WIDGET (hint_dlg));

        hint_dlg = gtk_message_dialog_new (GTK_WINDOW (toplevel_window),
                                           GTK_DIALOG_DESTROY_WITH_PARENT,
                                           GTK_MESSAGE_INFO,
                                           GTK_BUTTONS_OK,
                                           "%s", message);

        gtk_message_dialog_format_secondary_text (GTK_MESSAGE_DIALOG (hint_dlg),
                                                  "%s", secondary_message);

        gtk_container_set_border_width (GTK_CONTAINER (hint_dlg), 6);
        gtk_window_set_title (GTK_WINDOW (hint_dlg), "");

        if (hint_dlg)
                g_signal_connect (hint_dlg,
                                  "destroy",
                                  (GCallback) hint_destroy_callback,
                                  NULL);

	gtk_dialog_run (GTK_DIALOG (hint_dlg));
	gtk_widget_destroy (hint_dlg);

        g_free (message);
        g_free (secondary_message);
}

void 
pref_dialog_response (GtkWidget *w, int response, gpointer data)
{
        gtk_widget_hide (w);
}

void
show_probabilities_toggle_cb (GtkToggleButton *w, gpointer data)
{
        gboolean is_on = gtk_toggle_button_get_active (w);
        bj_set_show_probabilities (is_on);
}

void
quick_deal_toggle_cb (GtkToggleButton *w, gpointer data)
{
        gboolean is_on = gtk_toggle_button_get_active (w);
        bj_set_quick_deal (is_on);
}

void
never_insurance_toggle_cb (GtkToggleButton *w, gpointer data)
{
        gboolean is_on = gtk_toggle_button_get_active (w);
        bj_set_never_insurance (is_on);
}

enum {
        NAME_STRING,
        NDECKS_STRING,
        HIT_SOFT17_STRING,
        DOUBLE_ANY_STRING,
        DOUBLE_9_STRING,
        DOUBLE_SOFT_STRING,
        DOUBLE_AFTER_HIT_STRING,
        DOUBLE_AFTER_SPLIT_STRING,
        RESPLIT_STRING,
        RESPLIT_ACES_STRING,
        SURRENDER_STRING,
        DEALER_SPEED_STRING,
        FILENAME_STRING
}; /* Column indices for list */

void
select_rule_cb (GtkTreeSelection *select, gpointer data)
{
	GtkTreeModel *model;
	GtkTreeIter iter;
        gchar *filename = NULL;

	if (gtk_tree_selection_get_selected (select, &model, &iter)) {
                gtk_tree_model_get (model, &iter, FILENAME_STRING, &filename, -1);
                if (g_ascii_strcasecmp (filename, bj_game_get_rules_file ())) {
                        if (data != NULL)
                                gtk_widget_set_sensitive (GTK_WIDGET (data), FALSE);
                        bj_game_new (filename, NULL);
                        if (data != NULL)
                                gtk_widget_set_sensitive (GTK_WIDGET (data), TRUE);
                }
        }
}

static void
reset_button_cb (GtkWidget *widget, gpointer data)
{
        bj_set_balance (500.0);
}

void
show_preferences_dialog (void) 
{
        static GtkWidget* pref_dialog = NULL;
        GtkWidget *frame;
        GtkWidget *top_vbox;
        GtkWidget *vbox;
        GtkWidget *toggle;
        GtkWidget *button;
        gboolean show_probabilities = false;
        gboolean quick_deal = false;
        gboolean never_insurance = false;
        static GtkListStore* list;
        static GtkWidget* list_view;
        GtkWidget* scrolled_window;
        GtkTreeViewColumn* column;
        GtkCellRenderer* renderer;
        GtkTreeSelection* select;
        GtkTreeIter iter;

        if (!pref_dialog) {
                pref_dialog = gtk_dialog_new_with_buttons (_("Blackjack Preferences"),
                                                           GTK_WINDOW (toplevel_window), 
                                                           GTK_DIALOG_DESTROY_WITH_PARENT,
                                                           GTK_STOCK_CLOSE, 
                                                           GTK_RESPONSE_CLOSE,
                                                           NULL);
                gtk_dialog_set_has_separator (GTK_DIALOG (pref_dialog), FALSE);
                gtk_container_set_border_width (GTK_CONTAINER (pref_dialog), 5);
                gtk_box_set_spacing (GTK_BOX (GTK_DIALOG (pref_dialog)->vbox), 2);
                
                top_vbox = gtk_vbox_new (FALSE, 18);
                gtk_container_set_border_width (GTK_CONTAINER (top_vbox), 5);
                gtk_container_add (GTK_CONTAINER (GTK_DIALOG (pref_dialog)->vbox),
                                   top_vbox);
                

                frame = games_frame_new (_("Game"));
                vbox = gtk_vbox_new (FALSE, 6);
                gtk_container_add (GTK_CONTAINER (frame), vbox);
                gtk_box_pack_start (GTK_BOX (top_vbox), frame, FALSE, FALSE, 0);
                
                // Show probabilities
                show_probabilities = bj_get_show_probabilities ();
                toggle = gtk_check_button_new_with_mnemonic (_("_Display hand probabilities"));
                gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (toggle),
                                              show_probabilities);

                gtk_box_pack_start (GTK_BOX (vbox), toggle, FALSE, FALSE, 0);
                g_signal_connect (toggle, "toggled",
                                  G_CALLBACK (show_probabilities_toggle_cb), NULL);
                
                // Quick deal
                quick_deal = bj_get_quick_deal ();
                toggle = gtk_check_button_new_with_mnemonic (_("_Quick deals (no delay between each card)"));
                gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (toggle),
                                              quick_deal);
                gtk_box_pack_start (GTK_BOX (vbox), toggle, FALSE, FALSE, 0);
                g_signal_connect (toggle, "toggled",
                                  G_CALLBACK (quick_deal_toggle_cb), NULL);

                // No insurance
                never_insurance = bj_get_never_insurance ();
                toggle = gtk_check_button_new_with_mnemonic (_("_Never take insurance"));
                gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (toggle),
                                              never_insurance);
                gtk_box_pack_start (GTK_BOX (vbox), toggle, FALSE, FALSE, 0);
                g_signal_connect (toggle, "toggled",
                                  G_CALLBACK (never_insurance_toggle_cb), NULL); 
               
                button = gtk_button_new_with_mnemonic (_("_Reset Balance"));
                gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 0);
                g_signal_connect (button, "clicked",
                                  G_CALLBACK (reset_button_cb), NULL);               
                
                // Rules Tab
                frame = games_frame_new (_("Rules"));
                vbox = gtk_vbox_new (FALSE, 6);
                gtk_container_add (GTK_CONTAINER (frame), vbox);
                gtk_box_pack_start (GTK_BOX (top_vbox), frame, TRUE, TRUE, 0);

                list = gtk_list_store_new (13, 
                                           G_TYPE_STRING,  // Name
                                           G_TYPE_INT,     // Decks
                                           G_TYPE_BOOLEAN, // Hit Soft 17
                                           G_TYPE_BOOLEAN, // Double Any Total
                                           G_TYPE_BOOLEAN, // Double 9
                                           G_TYPE_BOOLEAN, // Double soft
                                           G_TYPE_BOOLEAN, // Double after hit
                                           G_TYPE_BOOLEAN, // Double after split
                                           G_TYPE_BOOLEAN, // Resplit
                                           G_TYPE_BOOLEAN, // Resplit aces
                                           G_TYPE_BOOLEAN, // Surrender
                                           G_TYPE_INT,     // Dealer speed
                                           G_TYPE_STRING); // Filename
                list_view = gtk_tree_view_new_with_model (GTK_TREE_MODEL (list));
                gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (list_view), TRUE);
                g_object_unref (list);
    
                renderer = gtk_cell_renderer_text_new ();
                column = gtk_tree_view_column_new_with_attributes
                        (_("Name"), renderer, "text", NAME_STRING, NULL);
                gtk_tree_view_append_column (GTK_TREE_VIEW (list_view), column);
                
                renderer = gtk_cell_renderer_text_new ();
                column = gtk_tree_view_column_new_with_attributes
                        (_("Decks"), renderer, "text", NDECKS_STRING, NULL);
                gtk_tree_view_append_column (GTK_TREE_VIEW (list_view), column);
                
                renderer = gtk_cell_renderer_text_new ();
                column = gtk_tree_view_column_new_with_attributes
                        (_("Hit Soft 17"), renderer, "text", HIT_SOFT17_STRING, NULL);
                gtk_tree_view_append_column (GTK_TREE_VIEW (list_view), column);
                
                renderer = gtk_cell_renderer_text_new ();
                column = gtk_tree_view_column_new_with_attributes
                        (_("Double Any Total"), renderer, "text", DOUBLE_ANY_STRING, NULL);
                gtk_tree_view_append_column (GTK_TREE_VIEW (list_view), column);
                
                renderer = gtk_cell_renderer_text_new ();
                column = gtk_tree_view_column_new_with_attributes
                        (_("Double 9"), renderer, "text", DOUBLE_9_STRING, NULL);
                gtk_tree_view_append_column (GTK_TREE_VIEW (list_view), column);
                
                renderer = gtk_cell_renderer_text_new ();
                column = gtk_tree_view_column_new_with_attributes
                        (_("Double Soft"), renderer, "text", DOUBLE_SOFT_STRING, NULL);
                gtk_tree_view_append_column (GTK_TREE_VIEW (list_view), column);
                
                renderer = gtk_cell_renderer_text_new ();
                column = gtk_tree_view_column_new_with_attributes
                        (_("Double After Hit"), renderer, "text", DOUBLE_AFTER_HIT_STRING, NULL);
                gtk_tree_view_append_column (GTK_TREE_VIEW (list_view), column);
                
                renderer = gtk_cell_renderer_text_new ();
                column = gtk_tree_view_column_new_with_attributes
                        (_("Double After Split"), renderer, "text", DOUBLE_AFTER_SPLIT_STRING, NULL);
                gtk_tree_view_append_column (GTK_TREE_VIEW (list_view), column);
                
                renderer = gtk_cell_renderer_text_new ();
                column = gtk_tree_view_column_new_with_attributes
                        (_("Resplit"), renderer, "text", RESPLIT_STRING, NULL);
                gtk_tree_view_append_column (GTK_TREE_VIEW (list_view), column);
                
                renderer = gtk_cell_renderer_text_new ();
                column = gtk_tree_view_column_new_with_attributes
                        (_("Resplit Aces"), renderer, "text", RESPLIT_ACES_STRING, NULL);
                gtk_tree_view_append_column (GTK_TREE_VIEW (list_view), column);
                
                renderer = gtk_cell_renderer_text_new ();
                column = gtk_tree_view_column_new_with_attributes
                        (_("Surrender"), renderer, "text", SURRENDER_STRING, NULL);
                gtk_tree_view_append_column (GTK_TREE_VIEW (list_view), column);
                
                renderer = gtk_cell_renderer_text_new ();
                column = gtk_tree_view_column_new_with_attributes
                        (_("Dealer Speed"), renderer, "text", DEALER_SPEED_STRING, NULL);
                gtk_tree_view_append_column (GTK_TREE_VIEW (list_view), column);
                
                select = gtk_tree_view_get_selection (GTK_TREE_VIEW (list_view));
                gtk_tree_selection_set_mode (select, GTK_SELECTION_BROWSE); 
                
                scrolled_window = gtk_scrolled_window_new (NULL, NULL);
                gtk_scrolled_window_set_shadow_type 
                        (GTK_SCROLLED_WINDOW (scrolled_window), GTK_SHADOW_IN);
                gtk_widget_set_size_request (scrolled_window, 300, 150);
                gtk_container_add (GTK_CONTAINER (scrolled_window), list_view);
                
                gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
                                                GTK_POLICY_AUTOMATIC,
                                                GTK_POLICY_AUTOMATIC);
                
                gtk_box_pack_end (GTK_BOX (vbox), scrolled_window, TRUE, TRUE, 0);
                
                g_signal_connect (select, "changed", 
                                  G_CALLBACK (select_rule_cb), (gpointer) pref_dialog);
                
                const gchar *current_rule = bj_get_game_variation ();
                gint i = 0;
                BJGameRules *ruleset;
                for (GList *temptr = bj_game_get_rules_list (); temptr; temptr=temptr->next) {
                        gchar *text;
                        ruleset = bj_game_find_and_read_rules ((gchar*)temptr->data);
                        if (ruleset) {
                                text = bj_game_file_to_name ((gchar*)temptr->data);
                                gtk_list_store_append (GTK_LIST_STORE (list), &iter);
                                gtk_list_store_set (GTK_LIST_STORE (list), &iter, 
                                                    NAME_STRING, text, 
                                                    NDECKS_STRING, ruleset->getNumDecks (),
                                                    HIT_SOFT17_STRING, ruleset->getHitSoft17 (),
                                                    DOUBLE_ANY_STRING, ruleset->getDoubleAnyTotal (),
                                                    DOUBLE_9_STRING, ruleset->getDouble9 (),
                                                    DOUBLE_SOFT_STRING, ruleset->getDoubleSoft (),
                                                    DOUBLE_AFTER_HIT_STRING, ruleset->getDoubleAfterHit (),
                                                    DOUBLE_AFTER_SPLIT_STRING, ruleset->getDoubleAfterSplit (),
                                                    RESPLIT_STRING, ruleset->getResplit (),
                                                    RESPLIT_ACES_STRING, ruleset->getResplitAces (),
                                                    SURRENDER_STRING, ruleset->getLateSurrender (),
                                                    DEALER_SPEED_STRING, ruleset->getDealerSpeed (),
                                                    FILENAME_STRING, (gchar*)temptr->data, -1);
                                delete ruleset;
                                if (current_rule && g_ascii_strcasecmp (current_rule, (gchar*)temptr->data) == 0) {
                                        /* FIXMEchpe: this is soo wrong */
                                        gtk_tree_view_set_cursor (GTK_TREE_VIEW (list_view),
                                                                  gtk_tree_path_new_from_indices (i, -1),
                                                                  NULL, FALSE);
                                }
                                i++;
                        }
                }
                
                // Cards Tab
                deck_edit = bj_get_card_theme_selector ();
                gtk_box_pack_start (GTK_BOX (top_vbox), deck_edit, FALSE, FALSE, 0);

                // General signals
                g_signal_connect (pref_dialog, "response",
                                  G_CALLBACK (pref_dialog_response), NULL);
                
                g_signal_connect (pref_dialog, "delete_event",
                                  G_CALLBACK (gtk_widget_hide), NULL);
        }
        
        if (pref_dialog && !GTK_WIDGET_VISIBLE (pref_dialog)) {
                // Set card style?

                gtk_widget_show_all (pref_dialog);
        }
        gtk_window_present (GTK_WINDOW (pref_dialog));
}
