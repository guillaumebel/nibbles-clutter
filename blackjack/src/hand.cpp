// -*- mode:C++; tab-width:8; c-basic-offset:8; indent-tabs-mode:nil -*-
/*
 * Blackjack - hand.cpp
 *
 * Copyright (C) 2003-2005 William Jon McCann <mccann@jhu.edu>
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
#include <gdk/gdk.h>
#include <gdk/gdkkeysyms.h>

#include "blackjack.h"
#include "events.h"
#include "dialog.h"
#include "draw.h"
#include "player.h"
#include "game.h"
#include "hand.h"
#include "chips.h"

using namespace std;

guint deal_timeout_id = 0;
guint finish_timeout_id = 0;
guint hit_timeout_id = 0;

static void bj_hand_finish_internal (void);

gboolean
bj_hand_events_pending (void)
{
        gboolean pending = FALSE;

        pending = (deal_timeout_id != 0
                   || hit_timeout_id != 0
                   || finish_timeout_id != 0);

        return pending;
}

void
bj_hand_cancel ()
{
        if (deal_timeout_id != 0) {
                g_source_remove (deal_timeout_id);
                deal_timeout_id = 0;
        }

        if (hit_timeout_id != 0) {
                g_source_remove (hit_timeout_id);
                hit_timeout_id = 0;
        }
  
        if (finish_timeout_id != 0) {
                g_source_remove (finish_timeout_id);
                finish_timeout_id = 0;
        }
}

hslot_type
bj_hand_get_slot ()
{
        if (player == NULL)
                return NULL;

        return player->hslot;
}

void
bj_hand_show_options ()
{
        if (strategy != NULL
            && (player->getCards () > 1) 
            && (dealer->getCards () > 1) )
                strategy->showOptions (player,
                                       dealer->cards[0].value(), 
                                       numHands);
}

void
bj_hand_show_dealer_probabilities ()
{
        if (dealer->getCards () > 0)
                dealerProbabilities->showProbabilities (distribution,
                                                        dealer->cards[0].value());
}

char *
bj_hand_get_best_option_string (char **secondary_message)
{
        gchar *option_str = NULL;
        gint bestOption;

        if (strategy == NULL)
                return NULL;

        bestOption = strategy->getBestOption (player, 
                                              dealer->cards[0].value (),
                                              numHands);

        switch (bestOption) {
        case KEY_S : option_str = g_strdup (_("The best option is to stand"));
                if (secondary_message)
                        *secondary_message = g_strdup (_("To stand means to stop adding cards to your hand.  "
                                                         "Do this by clicking on the dealer's cards or by selecting the option from the Control menu."));
                break;
        case KEY_H : option_str = g_strdup (_("The best option is to hit"));
                if (secondary_message)
                        *secondary_message = g_strdup (_("To hit means to add another card to your hand.  "
                                                         "Do this by clicking once on your cards or by selecting the option from the Control menu."));
                break;
        case KEY_D : option_str = g_strdup (_("The best option is to double down"));
                if (secondary_message)
                        *secondary_message = g_strdup (_("To double down means to double the initial wager and receive exactly one more card.  "
                                                         "Do this by clicking once on the chips at the bottom of the window or by selecting the option from the Control menu."));
                break;
        case KEY_P : option_str = g_strdup (_("The best option is to split"));
                if (secondary_message)
                        *secondary_message = g_strdup (_("To split means to divide your current hand into two separate hands.  Do this by dragging one of your cards and dropping it off to the side or by selecting the option from the Control menu."));
                break;
        case KEY_R : option_str = g_strdup (_("The best option is to surrender"));
                if (secondary_message)
                        *secondary_message = g_strdup (_("To surrender means to give up half your wager and not complete the hand.  "
                                                         "Do this by selecting the option from the Control menu."));
                break;
        default:
                break;
        }

        return option_str;
}

gint
bj_get_hand_results (gint dealer_count, gint player_count)
{
        // (Win == 1; Lose == -1; Push == 0)
        gint results = -1;

        if (dealer_count > 21)
                dealer_count = 0;
        if (player_count <= 21) {
                if (player_count > dealer_count)
                        results = 1;
                else if (player_count == dealer_count)
                        results = 0;
        }

        return results;
}


gboolean
bj_hand_can_be_doubled ()
{
        if (! bj_game_is_active ())
                return FALSE;

        // must be able to hit to double down
        if (! bj_hand_can_be_hit ())
                return FALSE;

        if ((numHands == 1 && rules->getDoubleDown (*player))
            || (numHands > 1 && rules->getDoubleAfterSplit (*player)))
                return TRUE;
        else 
                return FALSE;
}

gboolean
bj_hand_can_be_hit ()
{
        if (! bj_game_is_active ())
                return FALSE;

        if (numHands == 1 || player->cards[0].value() != 1)
                return TRUE;
        else
                return FALSE;
}

gboolean
bj_hand_can_be_surrendered ()
{
        if (! bj_game_is_active ())
                return FALSE;

        if (rules->getLateSurrender () 
            && player->getCards () == 2
            && numHands == 1)
                return TRUE;
        else
                return FALSE;
}

gboolean
bj_hand_can_be_split ()
{
        if (! bj_game_is_active ())
                return FALSE;

        // Check if player can split a pair.

        int card = player->cards[0].value ();
        if (player->getCards () == 2 && card == player->cards[1].value ()
            && numHands < rules->getResplit (card))
                return TRUE;
        else
                return FALSE;
}

void
bj_deal_card_to_player ()
{
        distribution->deal (player->deal (shoe->deal ()));
}

void
bj_deal_card_to_dealer (gboolean faceup=TRUE)
{
        dealer->deal (shoe->deal (), faceup);
}

void
bj_deal_card_to_dealer_distribution (gboolean faceup=TRUE)
{
        distribution->deal (dealer->deal (shoe->deal ()));
}

static gboolean
bj_hand_new5 (gpointer data)
{

        player->showCount ();
        allSettled = FALSE;

        // Ask for insurance if the up card is an ace.

        bool insurance = FALSE;
        if (!bj_get_never_insurance () && (dealer->cards[0].value () == 1 || dealer->cards[0].value () == 10)) {
                if (bj_get_show_probabilities ()) {
                        dealerProbabilities->showProbabilities (distribution,
                                                                dealer->cards[0].value (),
                                                                FALSE);
                }
                if (dealer->cards[0].value () == 1) {
                        insurance = get_insurance_choice ();
                        if (insurance)
                                bj_adjust_balance (-1 * player->wager / 2);
                }
        }

        // Check for dealer blackjack.
        
        if (dealer->getCards () == 2 && dealer->getCount () == 21) {
                allSettled = TRUE;
                if (insurance)
                        bj_adjust_balance (player->wager / 2 + (player->wager / 2) * 2);
            
                if ( (player->getCards () == 2) && (player->getCount () == 21) )
                        bj_adjust_balance (player->wager);
                bj_game_set_active (FALSE);
                bj_hand_finish_internal ();
                deal_timeout_id = 0;
                return FALSE;
        }
        
        // Dealer does not have blackjack; collect insurance and check for player
        // blackjack.

        else {
                if ( (player->getCards () == 2) && (player->getCount () == 21) ) {
                        allSettled = TRUE;
                        bj_adjust_balance (player->wager + player->wager * 3 / 2);
                }
        }
        
        // Finish player hand.

        if (!allSettled) {
                dealerProbabilities->showProbabilities (distribution,
                                                        dealer->cards[0].value ());
                allSettled = TRUE;
                bj_game_set_active (TRUE);
        }

        if (player->getCount () == 21) {
                bj_hand_finish_internal ();
                deal_timeout_id = 0;
                return FALSE;
        }
        else {
                if (strategy != NULL)
                        strategy->showOptions (player,
                                               dealer->cards[0].value (), 
                                               numHands);
        }

        bj_update_control_menu ();
        bj_draw_refresh_screen ();
        deal_timeout_id = 0;
        return FALSE;
}

static gboolean
bj_hand_new4 (gpointer data)
{
        bj_deal_card_to_dealer (FALSE);
        bj_draw_refresh_screen ();

        deal_timeout_id = g_timeout_add ((gint)bj_get_deal_delay (),
                                         bj_hand_new5,
                                         NULL);

        return FALSE;
}

static gboolean
bj_hand_new3 (gpointer data)
{
        bj_deal_card_to_player ();
        bj_draw_refresh_screen ();

        deal_timeout_id = g_timeout_add ((gint)bj_get_deal_delay (),
                                         bj_hand_new4,
                                         NULL);

        return FALSE;
}

static gboolean
bj_hand_new2 (gpointer data)
{
        bj_deal_card_to_dealer ();
        bj_draw_refresh_screen ();

        deal_timeout_id = g_timeout_add ((gint)bj_get_deal_delay (),
                                         bj_hand_new3,
                                         NULL);

        return FALSE;
}

static gboolean
bj_hand_new1 (gpointer data)
{
        bj_deal_card_to_player ();
        bj_draw_refresh_screen ();

        deal_timeout_id = g_timeout_add ((gint)bj_get_deal_delay (),
                                         bj_hand_new2,
                                         NULL);

        return FALSE;
}

void
bj_hand_new ()
{
        if (bj_hand_events_pending ())
                return;

        // Reshuffle if necessary.
        if (shoe->numCards < 52) {
                shoe->shuffle ();
                distribution->reset ();
        }

        player->wager = bj_get_wager ();
        bj_adjust_balance (-1 * player->wager);
        bj_game_set_active (TRUE);
  
        player->showWager ();
        lastWager = player->wager;

        bj_hand_new1 (NULL);
}

static gint
bj_count_live_hands ()
{
	// Live hands compete with the dealer hand.
	gint live_hands = 0; 
	player = (PlayerHand*) g_list_nth_data (playerHands,0);
	while (player != NULL) {
		if ((player->getCount () < 22 ) 
		    && !((player->getCards () == 2) && (player->getCount () == 21)))
			live_hands++;
		player = player->nextHand;
	}
	return live_hands;
}

static gboolean
bj_hand_finish1 (gpointer data)
{
        if (!allSettled
	    && bj_count_live_hands () 
	    && (dealer->getCount () < 17 
                || (rules->getHitSoft17 () 
                    && dealer->getCount () == 17 
                    && dealer->getSoft ()))) {
                // Finish dealer hand.
                bj_deal_card_to_dealer_distribution ();
                bj_draw_refresh_screen ();
                return TRUE;
        }
        else {
                // Settle remaining wagers.
                if (!allSettled) {
                        int dealer_count;
                        if ( (dealer_count = dealer->getCount ()) > 21 )
                                dealer_count = 0;
                        player = (PlayerHand*) g_list_nth_data (playerHands,0);
                        while (player != NULL) {
                                if (player->getCount () <= 21) {
                                        if (player->getCount () > dealer_count)
                                                bj_adjust_balance (player->wager * 2);
                                        else if (player->getCount () == dealer_count)
                                                bj_adjust_balance (player->wager);
                                }
                                player = player->nextHand;
                        }
                }
                bj_draw_refresh_screen ();
                finish_timeout_id = 0;
                return FALSE;
        }
}

static void
bj_hand_finish_internal (void)
{
        hcard_type card;

        bj_game_set_active (FALSE);
        // Turn dealer hole card.

        card = (hcard_type) g_list_nth_data (dealer->hslot->cards, 1);
        if (card)
                card->direction = UP;

        dealer->showCount ();
        bj_update_control_menu ();
        bj_draw_refresh_screen ();

        finish_timeout_id = g_timeout_add ((gint)bj_get_deal_delay (), 
                                           bj_hand_finish1, NULL);
}

void
bj_hand_finish ()
{
        if (bj_hand_events_pending ())
                return;

        bj_hand_finish_internal ();
}

void
bj_hand_stand ()
{
        if (bj_hand_events_pending ())
                return;

        if (bj_game_is_active ()) {
                allSettled = FALSE;
                if ((player = player->nextHand) == NULL) {
                        bj_hand_finish_internal ();
                }
                bj_hand_finish_play ();
        }
}

static void
bj_hand_hit_internal ()
{
        if (bj_hand_can_be_hit ()) {
                bj_deal_card_to_player ();
                
                player->showCount ();
                if (player->getCount () >= 21)
                        if ((player = player->nextHand) == NULL) {
                                allSettled = FALSE;
                                bj_hand_finish_internal ();
                                return;
                        }
                bj_hand_finish_play ();
        }
}

static gboolean
bj_hand_hit_delay_cb (gpointer data)
{
        bj_hand_hit_internal ();
        hit_timeout_id = 0;
        return FALSE;
}

void
bj_hand_hit_with_delay (void)
{
        if (bj_hand_events_pending ())
                return;

        hit_timeout_id = g_timeout_add ((gint)bj_get_deal_delay (),
                                        bj_hand_hit_delay_cb, NULL);
}
  
void
bj_hand_double ()
{
        if (bj_hand_events_pending ())
                return;

        if (bj_hand_can_be_doubled ()) {
                bj_adjust_balance (-1 * player->wager);
                player->wager *= 2;
                player->showWager ();
                
                bj_deal_card_to_player ();
                
                player->showCount ();
                if (player->getCount () <= 21)
                        allSettled = FALSE;
                if ((player = player->nextHand) == NULL)
                        bj_hand_finish ();
                else
                        bj_hand_finish_play ();
        }
}

void
bj_hand_split ()
{
        hslot_type hslot;
        gint slot_start_x;

        if (bj_hand_events_pending ())
                return;

        if (bj_hand_can_be_split ()) {
                // Put card value back into shoe
                tempCard = player->cards[1];
                
                player->undeal (tempCard.value ());
      
                // Remove card from slot
                hslot = player->hslot;
                hcard_type card = (hcard_type) g_list_last (hslot->cards)->data;
                if (card)
                        hslot->cards = g_list_remove (hslot->cards,card);
      
                // set slot origin for split hands
                if (numHands == 1) {
                        // Two hands centered on the initial slot position
                        slot_start_x = (gint)(PLAYER_SLOT_ORIGIN_X - (PLAYER_SLOT_SPACING / 2));
                }
                else
                        slot_start_x = (gint)(PLAYER_SLOT_ORIGIN_X - PLAYER_SLOT_SPACING);
                
                // Add another slot and hand
                PlayerHand *newHand = (PlayerHand*)g_malloc (sizeof (PlayerHand));
                newHand->nextHand = player->nextHand;
                player->nextHand = newHand;
                newHand->wager = player->wager;
                GList* tempptr;
                tempptr = g_list_find (playerHands, newHand->nextHand);
                
                playerHands = g_list_insert_before (playerHands,
                                                    tempptr,
                                                    newHand);
                numHands++;

                hslot_type new_hslot = bj_slot_add_before_slot ((tempptr) ? newHand->nextHand->hslot : NULL,
                                                                numHands);
      
                newHand->hslot = new_hslot;
                newHand->reset ();
                
                // Also need to recreate chip stacks
                bj_chip_stack_delete_all_wagers ();

                // Reposition all slots
                gint i = 1;
                for (tempptr = playerHands; tempptr; tempptr = tempptr->next) {
                        hslot_type slot = ((PlayerHand*)tempptr->data)->hslot;
                        slot->x = (double)i / (numHands + 1);
                        bj_chip_stack_new_with_value (bj_get_wager (),
                                                      slot->x - 0.15,
                                                      slot->y + 0.2);
                        i++;
                }
 
                bj_draw_set_geometry (numHands, 2);

                bj_adjust_balance (-1 * player->wager);
      
                newHand->deal (tempCard);
        }
        bj_hand_finish_play ();
}

void
bj_hand_surrender ()
{
        if (bj_hand_events_pending ())
                return;

        if (bj_hand_can_be_surrendered ()) {
                bj_adjust_balance (player->wager / 2);
                if ((player = player->nextHand) == NULL)
                        bj_hand_finish_internal ();
        }
}

void
bj_hand_new_deal ()
{
        if (bj_hand_events_pending ())
                return;

        if (! bj_game_is_active ()) {
                bj_clear_table ();
                bj_hand_new ();
        }
}

void
bj_hand_finish_play ()
{
        gboolean check_splits;

        if (bj_game_is_active ()) {
                // Deal another card to a split hand if necessary.
                // need to loop for the case where splits get blackjack
                // FIXME - this is less than elegant (get rid of while loop)
                check_splits = TRUE;
                while (check_splits) {
                        if (player->getCards () == 1)
                                bj_deal_card_to_player ();
                        else
                                check_splits = FALSE;

                        if (player->getCount () >= 21) {
                                if (player->nextHand == NULL) {
                                        check_splits = FALSE;
                                        allSettled = FALSE;
                                        bj_hand_finish_internal ();
                                }
                                else
                                        player = player->nextHand;
                        }
                        
                        bj_hand_show_dealer_probabilities ();
                        bj_hand_show_options ();
                }
        }

        bj_update_control_menu ();
        bj_draw_refresh_screen ();
}
