// -*- mode:C++; tab-width:8; c-basic-offset:8; indent-tabs-mode:nil -*-
/*
 * Blackjack - game.cpp
 *
 * Copyright (C) 2003-2004 William Jon McCann <mccann@jhu.edu>
 * Copyright © 2008 Christian Persch
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
#include <string.h>
#include <glib/gi18n.h>
#include <glib/gstdio.h>

#include <libgames-support/games-runtime.h>

#include "blackjack.h"
#include "events.h"
#include "draw.h"
#include "chips.h"
#include "splash.h"
#include "player.h"
#include "hand.h"
#include "game.h"

#define RULES_GROUP "Blackjack Rules"

#include <iostream>
using namespace std;

BJGameRules      *rules = NULL;
LoadablePlayer   *strategy = NULL;
Hand             *dealer = NULL;
Probabilities    *dealerProbabilities = NULL;
Shoe             *shoe = NULL;
BJShoe           *distribution = NULL;
PlayerHand       *player = NULL;

GList            *playerHands = NULL;
gfloat           lastWager = 5.0;
Card             tempCard;
PlayerHand       *tempHand = NULL;

gint             numDecks;
gint             dealerSpeed;

gint             numHands;

gchar            *game_file;
gchar            *game_name;

gboolean         allSettled = FALSE;
gboolean         game_is_done = FALSE;
gboolean         first_hand = TRUE;

GList *rules_list = NULL;

BJGameRules::BJGameRules (bool lhitSoft17, bool ldoubleAnyTotal, 
                          bool ldouble9, bool ldoubleSoft, 
                          bool ldoubleAfterHit, bool ldoubleAfterSplit,
                          bool lresplit, bool lresplitAces, 
                          bool llateSurrender, int lnumDecks, int ldealerSpeed)
        : BJRules (lhitSoft17, ldoubleAnyTotal, 
                   ldouble9, ldoubleSoft, 
                   ldoubleAfterHit, ldoubleAfterSplit,
                   lresplit, lresplitAces, 
                   llateSurrender)
{
        numDecks = lnumDecks;
        dealerSpeed = ldealerSpeed;
}

int
BJGameRules::getDealerSpeed ()
{
        return dealerSpeed;
}

int
BJGameRules::getNumDecks ()
{
        return numDecks;
}

void
bj_game_show_hand_counts ()
{
        GList *temptr;
        for (temptr = playerHands; temptr; temptr = temptr->next)
                ((PlayerHand*)temptr->data)->showCount ();
        if (! bj_game_is_active ())
                dealer->showCount ();
}

gchar *
bj_game_file_to_name (const gchar* file)
{
        char *p, *buf = g_path_get_basename (file);

        if ((p = strrchr (buf, '.')))
                *p = '\0';
        for (p = buf; p = strchr (p, '_'), p && *p;)
                *p = ' ';
        for (p = buf; p = strchr (p, '-'), p && *p;)
                *p = ' ';
        buf[0] = toupper (buf[0]);
        p = g_strdup (_(buf));

        g_free (buf);
        return p;
}

/* copied from gnome-util.c */
static const char *
extension_pointer (const char * path)
{
        const char * s, * t;
        
        g_return_val_if_fail (path != NULL, NULL);

        /* get the dot in the last element of the path */
        t = strrchr (path, G_DIR_SEPARATOR);
        if (t != NULL)
                s = strrchr (t, '.');
        else
                s = strrchr (path, '.');
        
        if (s == NULL)
                return path + strlen (path); /* There is no extension. */
        else {
                ++s;      /* pass the . */
                return s;
        }
}


void
bj_game_find_rules (const gchar *variation)
{
        GDir *dir;
        G_CONST_RETURN gchar* file_name;
        gint n_games = 0;
        const char *rulesdir;

        rulesdir = games_runtime_get_directory (GAMES_RUNTIME_GAME_GAMES_DIRECTORY);
        dir = g_dir_open (rulesdir, 0, NULL);
        if (dir == NULL)
                return;
  
        while ((file_name = g_dir_read_name (dir)) != NULL) {
                if (!g_str_has_suffix (file_name, ".rules"))
                        continue;

                n_games++;
                rules_list = g_list_append (rules_list, g_strdup (file_name));
                if (! g_ascii_strcasecmp (variation, file_name)) 
                        bj_set_game_variation (file_name);
        }

        g_dir_close (dir);
}

GList *
bj_game_get_rules_list ()
{
        return rules_list;
}

char *
bj_game_get_rules_name ()
{
        return game_name;
}

const char *
bj_game_get_rules_file ()
{
        return game_file;
}

gboolean
bj_game_is_active ()
{
        return (! (game_is_done || first_hand));
}

gboolean
bj_game_is_first_hand ()
{
        return (first_hand);
}

void
bj_game_set_active (gboolean value)
{
        if (value) {
                game_is_done = FALSE;
                first_hand = FALSE;
        }
        else
                game_is_done = TRUE;

}

gdouble
bj_get_deal_delay ()
{
        return (bj_get_quick_deal ()) ? 1 : dealerSpeed;
}

BJGameRules *
bj_game_read_rules (const char *filename)
{
        BJGameRules *ruleset;
        gboolean hitSoft17,
                doubleAnyTotal,
                double9,
                doubleSoft,
                doubleAfterHit,
                doubleAfterSplit,
                resplit,
                resplitAces,
                lateSurrender;
        gint lnumDecks,
                ldealerSpeed;
        GKeyFile *key_file;

        lnumDecks = 6;
        hitSoft17 = FALSE;
        doubleAnyTotal = TRUE;
        double9 = TRUE;
        doubleSoft = TRUE;
        doubleAfterHit = FALSE;
        doubleAfterSplit = TRUE;
        resplit = TRUE;
        resplitAces = TRUE;
        lateSurrender = TRUE;
        ldealerSpeed = 500;

        key_file = g_key_file_new ();
        if (g_key_file_load_from_file (key_file, filename, GKeyFileFlags(0), NULL) &&
            g_key_file_has_group (key_file, RULES_GROUP)) {
                GError *error = NULL;
                int value;

#define CHECK_AND_ASSIGN(variable) \
                if (error) {\
                        g_printerr ("Failed to read value from rules file: %s\n", error->message);\
                        g_clear_error(&error);\
                } else {\
                        variable = value;\
                }

                value = g_key_file_get_integer (key_file, RULES_GROUP, "NumberOfDecks", &error);
                CHECK_AND_ASSIGN (lnumDecks);
                value = g_key_file_get_integer (key_file, RULES_GROUP, "DealerSpeed", &error);
                CHECK_AND_ASSIGN (ldealerSpeed);
                value = g_key_file_get_boolean (key_file, RULES_GROUP, "DealerHitsSoft17", &error);
                CHECK_AND_ASSIGN (hitSoft17);
                value = g_key_file_get_boolean (key_file, RULES_GROUP, "DoubleDownAnyTotal", &error);
                CHECK_AND_ASSIGN (doubleAnyTotal);
                value = g_key_file_get_boolean (key_file, RULES_GROUP, "DoubleDown9", &error);
                CHECK_AND_ASSIGN (double9);
                value = g_key_file_get_boolean (key_file, RULES_GROUP, "DoubleDownSoft", &error);
                CHECK_AND_ASSIGN (doubleSoft);
                value = g_key_file_get_boolean (key_file, RULES_GROUP, "DoubleDownAfterHit", &error);
                CHECK_AND_ASSIGN (doubleAfterHit);
                value = g_key_file_get_boolean (key_file, RULES_GROUP, "DoubleDownAfterSplit", &error);
                CHECK_AND_ASSIGN (doubleAfterSplit);
                value = g_key_file_get_boolean (key_file, RULES_GROUP, "ResplitAllowed", &error);
                CHECK_AND_ASSIGN (resplit);
                value = g_key_file_get_boolean (key_file, RULES_GROUP, "ResplitAcesAllowed", &error);
                CHECK_AND_ASSIGN (resplitAces);
                value = g_key_file_get_boolean (key_file, RULES_GROUP, "SurrenderAllowed", &error);
                CHECK_AND_ASSIGN (lateSurrender);

#undef CHECK_AND_ASSIGN
        }

        g_key_file_free (key_file);

        // Compute basic strategy.
        ruleset = new BJGameRules (hitSoft17, doubleAnyTotal, 
                                   double9, doubleSoft, 
                                   doubleAfterHit, doubleAfterSplit,
                                   resplit, resplitAces, 
                                   lateSurrender, lnumDecks, ldealerSpeed);
  
        return ruleset;
}


BJGameRules *
bj_game_find_and_read_rules (const gchar *filename)
{
        gchar *installed_filename;
        BJGameRules *ruleset = NULL;
        const char *rulesdir;

        rulesdir = games_runtime_get_directory (GAMES_RUNTIME_GAME_GAMES_DIRECTORY);
        installed_filename = g_build_filename (rulesdir, filename, NULL);

        if (g_file_test (installed_filename, G_FILE_TEST_EXISTS))
                ruleset = bj_game_read_rules (installed_filename);

        g_free (installed_filename);

        return ruleset;
}

static gchar *
bj_game_get_config_dir (void)
{
#define GNOME_DOT_GNOME            ".gnome2/"
        gchar *conf_dir = NULL;

        conf_dir = g_build_filename (g_get_home_dir (),
                                     GNOME_DOT_GNOME,
                                     "blackjack.d",
                                     NULL);
        return conf_dir;
}

static void
bj_game_ensure_config_dir_exists (const char *dir)
{
        if (g_file_test (dir, G_FILE_TEST_IS_DIR) == FALSE) {
                if (g_file_test (dir, G_FILE_TEST_EXISTS) == TRUE) {
                        // FIXME: use a dialog
                        cerr << dir << " exists, please move it out of the way." << endl;
                }
                if (g_mkdir (dir, 488) != 0)
                        cerr << "Failed to create directory " << dir << endl;
        }
}

static void
bj_game_eval_installed_file (const gchar *file)
{
        const char *rulesdir;
        char *installed_filename;

        if (g_file_test (file, G_FILE_TEST_EXISTS))
                return;
  
        rulesdir = games_runtime_get_directory (GAMES_RUNTIME_GAME_GAMES_DIRECTORY);
        installed_filename = g_build_filename (rulesdir, file, NULL);

        if (g_file_test (installed_filename, G_FILE_TEST_EXISTS)) {
                rules = bj_game_read_rules (installed_filename);
                g_free (installed_filename);
                
                // set globals
                numDecks = rules->getNumDecks ();
                dealerSpeed = rules->getDealerSpeed ();

                BJStrategy maxValueStrategy;
                Progress progress;

                gchar *cache_filename = g_strdup_printf ("%s%s", file, ".dat");

                char *config_dir = bj_game_get_config_dir ();
                bj_game_ensure_config_dir_exists (config_dir);
                installed_filename = g_build_filename (config_dir, cache_filename, NULL);
                g_free (config_dir);
                        
                gboolean use_cache = FALSE;
                if (g_file_test (installed_filename, G_FILE_TEST_EXISTS))
                        use_cache = TRUE;

                if (! use_cache)
                        splash_new ();
                strategy = new LoadablePlayer (numDecks, rules, 
                                               maxValueStrategy, 
                                               progress, 
                                               (use_cache) ? installed_filename : NULL);

                if (! use_cache) {
                        strategy->saveXML (installed_filename);
                        splash_destroy ();
                }

                g_free (cache_filename);
        } 
        else {
                gchar *message = g_strdup_printf ("%s\n %s", _("Blackjack can't load the requested file"),
                                                  installed_filename);
                gchar *message2 = _("Please check your Blackjack installation");
                GtkWidget *w = gtk_message_dialog_new_with_markup (GTK_WINDOW (toplevel_window),
                                                                   GTK_DIALOG_DESTROY_WITH_PARENT,
                                                                   GTK_MESSAGE_ERROR,
                                                                   GTK_BUTTONS_CLOSE,
                                                                   "<span weight=\"bold\" size=\"larger\">%s</span>\n\n%s",
                                                                   message, message2);
                gtk_container_set_border_width (GTK_CONTAINER (w), 6);

                gtk_dialog_run (GTK_DIALOG (w));
                gtk_widget_destroy (w);
                g_free (message);
                exit (1);
        }
        g_free (installed_filename);
}

void 
bj_game_cancel ()
{
        bj_hand_cancel ();
        bj_game_set_active (FALSE);
        bj_press_data_free ();
        bj_chip_stack_press_data_free ();
}

void
bj_game_new (const gchar* file, guint *seedp )
{
        gint min_w, min_h;

        bj_game_cancel ();

        first_hand = TRUE;

        bj_show_balance (bj_get_balance ());

        if (file && (!game_file || strcmp (file, game_file) != 0)) {
                char *old_game_file = game_file;

                game_file = g_strdup (file);
                g_free (old_game_file);
                
                bj_game_eval_installed_file (game_file);
                game_name = bj_game_file_to_name (game_file);
                
                if (option_dialog) {
                        gtk_widget_destroy (option_dialog);
                        option_dialog = NULL;
                }

                bj_set_game_variation (game_file);
        }

        if (seedp)
                seed = *seedp;
        else
                seed = g_random_int ();

        g_random_set_seed (seed);

        min_w = 600;
        min_h = 400;
        gtk_widget_set_size_request (playing_area, min_w, min_h);
        if (surface)
                bj_draw_refresh_screen ();

        // Prepare to play blackjack.

        if (dealer)
                delete dealer;
        dealer = new Hand;
        dealer->hslot = NULL;
        if (dealerProbabilities)
                delete dealerProbabilities;
        dealerProbabilities = new Probabilities (rules->getHitSoft17 ());
        if (shoe)
                delete shoe;
        shoe = new Shoe (numDecks);
        if (distribution)
                delete distribution;
        distribution = new BJShoe (numDecks);

        bj_clear_table ();

        bj_make_window_title (game_name);
        bj_update_control_menu ();
}

void
bj_clear_table ()
{
        GList* temptr;

        // delete all slots except 0,1
        delete_surface ();
        numHands = 1;

        for (temptr = playerHands; temptr; temptr = temptr->next)
                g_free (temptr->data);
        g_list_free (playerHands);
        playerHands = NULL;
        player = (PlayerHand*)g_malloc (sizeof (PlayerHand));
        playerHands = g_list_append (playerHands, player);
  
        // Create slots
        bj_slot_add (0);
        bj_slot_add (1);
        bj_draw_set_geometry (1, 2);

        // Clear the table.
        dealer->hslot = (hslot_type) g_list_nth_data (slot_list, 0);
        player->hslot = (hslot_type) g_list_nth_data (slot_list, 1);

        bj_chip_stack_new_with_value (bj_get_wager (),
                                      player->hslot->x - 0.2,
                                      player->hslot->y + 0.2);

        // Create source chip stacks 
        bj_chip_stack_create_sources ();

        bj_draw_rescale_cards ();

        player->nextHand = NULL;
        player->reset ();
        dealer->reset ();
        dealerProbabilities->reset ();

        first_hand = TRUE;
        gtk_widget_queue_draw (playing_area);
}

