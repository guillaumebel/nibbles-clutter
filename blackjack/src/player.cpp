// -*- mode:C++; tab-width:8; c-basic-offset:8; indent-tabs-mode:nil -*-
/*
 * Blackjack - player.cpp
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

#include <string.h>
#include <stdlib.h>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <glib/gi18n.h>
#include <gdk/gdk.h>
#include <gdk/gdkkeysyms.h>

#include "blackjack.h"
#include "events.h"
#include "draw.h"
#include "splash.h"
#include "player.h"
#include "hand.h"
#include "game.h"

#include <iostream>
using namespace std;

Progress::Progress ()
{
        last = -1;
}

void
Progress::indicate (int percentComplete)
{
        float initial_splash_progress = 0.0;
        float splash_progress;
        if (percentComplete != last) {
                last = percentComplete;
                splash_progress = initial_splash_progress + (percentComplete/100.0);
                splash_update (_("Computing basic strategy..."), splash_progress);
        }
}

int 
Card::value ()
{
        return (rank < 10 ? rank : 10);
}

Shoe::Shoe (int lnumDecks)
{
        numDecks = lnumDecks;
        cards = new Card[52*lnumDecks];
        numCards = 0;
        for (int deck = 0; deck < lnumDecks; deck++)
                for (int rank = 1; rank <= 13; rank++)
                        for (int suit = 1; suit <= 4; suit++) {
                                cards[numCards].rank = rank;
                                cards[numCards].suit = suit;
                                numCards++;
                        }
        bj_show_shoe_cards (numCards);
        g_random_set_seed (seed);
        shuffle ();
}

Shoe::~Shoe ()
{
        delete[] cards;
}

void
Shoe::shuffle ()
{
        // perform multiple pass selection shuffle
        for (int i=0; i < 5; i++)
                for (int card = 52 * numDecks; card > 1; card--)
                        swap (card - 1, g_random_int_range (0, card));
        numCards = 52 * numDecks;

        // Shoe is now full
        bj_show_shoe_cards (numCards);
}

Card
Shoe::deal ()
{
        numCards--;
        bj_show_shoe_cards (numCards);
        return cards[numCards];
}

void
Shoe::swap (int card1, int card2)
{
        Card temp = cards[card1];
        cards[card1] = cards[card2];
        cards[card2] = temp;
}

void
Hand::reset ()
{
        GList* temptr;

        // Free the cards in the slot
        for (temptr = hslot->cards; temptr; temptr = temptr->next)
                g_free (temptr->data);
        g_list_free (hslot->cards);

        hslot->cards = NULL;

        BJHand::reset();
}

int
Hand::deal (Card card, bool faceUp)
{
        cards[getCards()] = card;
        draw (getCards(), faceUp);
        BJHand::deal (card.value());
        return card.value();
}

void
Hand::draw (int card, bool faceUp)
{
        GList *cardlist = NULL;
        hcard_type newcard;

        newcard = bj_card_new (cards[card].rank, cards[card].suit - 1, 
                               (faceUp) ? UP : DOWN);
        cardlist = g_list_append (cardlist, newcard);
        bj_slot_add_cards (cardlist, hslot);
        bj_draw_refresh_screen ();
}

void
Hand::redraw ()
{
        for (int card = 0; card < getCards(); card++)
                draw (card);
}

void
Hand::hideCount ()
{
}

// If blackjack is false, then a split hand with a 10-ace is not blackjack, but a soft 21.

void
Hand::showCount (bool blackjack)
{
        hideCount();
        gchar *results = NULL;
        gchar *message;
        gchar *markup;

        if (getCount () > 21) 
                message = g_strdup (_("Bust"));
        else if (blackjack && getCards() == 2 && getCount() == 21)
                message = g_strdup (_("Blackjack!"));
        else {
                message = g_strdup_printf ("%s%d",
                                           (getSoft() ? _("Soft") : ""), 
                                           getCount ());
                if (! (bj_game_is_active () || hslot->id == 0)) {
                        gint hand_results = bj_get_hand_results (dealer->getCount (), 
                                                                 getCount ());
                        switch (hand_results) {
                        case 1: results = g_strdup_printf (" - %s", _("Win")); break;
                        case 0: results = g_strdup_printf (" - %s", _("Push")); break;
                        case -1: results = g_strdup_printf (" - %s", _("Lose")); break;
                        default: break;
                        }
                }
        }
        if (results == NULL)
                results = g_strdup ("");

        markup = g_strdup_printf ("<span weight=\"bold\" size=\"small\" foreground=\"#eaeac1\">%s%s</span>", 
                                  message, results);
        g_free (message);
        g_free (results);
        if (getCount () > 0)
                bj_draw_playing_area_text (markup, hslot->pixelx + card_width / 2, 
                                           hslot->pixely + card_height + 2);
        g_free (markup);
}

void
PlayerHand::showWager ()
{
}

void
PlayerHand::showOption (int option)
{
}

Player::Player (int lnumDecks, BJRules *lrules, BJStrategy & lstrategy,
                Progress & progress)
        : BJPlayer(BJShoe(lnumDecks), *lrules, lstrategy, progress)
{
        rules = lrules;
}

Player::Player (const int lnumDecks[], BJRules *lrules, BJStrategy & lstrategy,
                Progress & progress)
        : BJPlayer(BJShoe(lnumDecks), *lrules, lstrategy, progress)
{
        rules = lrules;
}

void
Player::reset ()
{
}

int
Player::showOptions (Hand *lplayer, int upCard, int lnumHands)
{
        int bestOption, num_options, bestOptionNumber;
        double value, bestValue;
        gchar *markup = NULL;
        gchar *tmpstr = NULL;
        gchar *mark_list[7];

        reset ();

        num_options = 0;

        mark_list[num_options++] = g_strdup_printf ("<b>%s</b>\n", _("Player expected values"));

        // Player can always stand.

        bestValue = value = getValueStand (*lplayer, upCard);
        mark_list[num_options++] = g_strdup_printf ("  %s     %8.2lf%%\n", 
                                                    _("Stand"), value * 100);
        bestOption = KEY_S;
        bestOptionNumber = num_options;

        // Player can't hit split aces.
        if (bj_hand_can_be_hit ()) {
                value = getValueHit (*lplayer, upCard);
                
                mark_list[num_options++] = g_strdup_printf ("  %s       %8.2lf%%\n", 
                                                            _("Hit"), value * 100);
                if (value > bestValue) {
                        bestValue = value;
                        bestOption = KEY_H;
                        bestOptionNumber = num_options;
                }
      
                // Check if player can double down.
                if (bj_hand_can_be_doubled ()) {
                        value = getValueDoubleDown (*lplayer, upCard);
                        mark_list[num_options++] = g_strdup_printf ("  %s    %8.2lf%%\n", 
                                                                    _("Double"), value * 100);
                        if (value > bestValue) {
                                bestValue = value;
                                bestOption = KEY_D;
                                bestOptionNumber = num_options;
                        }
                }
        }
  
        // Check if player can split a pair.

        if (bj_hand_can_be_split ()) {
                value = getValueSplit(lplayer->cards[0].value (), upCard);
                mark_list[num_options++] = g_strdup_printf ("  %s     %8.2lf%%\n", 
                                                            _("Split"), value * 100);
                if (value > bestValue) {
                        bestValue = value;
                        bestOption = KEY_P;
                        bestOptionNumber = num_options;
                }
        }

        // Check if player can surrender.
        if (bj_hand_can_be_surrendered ()) {
                value = -0.5;
                mark_list[num_options++] = g_strdup_printf ("  %s %8.2lf%%\n", 
                                                            _("Surrender"), value * 100);
                if (value > bestValue) {
                        bestValue = value;
                        bestOption = KEY_R;
                        bestOptionNumber = num_options;
                }
        }

        tmpstr = mark_list[bestOptionNumber - 1];
        mark_list[bestOptionNumber - 1] = g_strdup_printf ("<b>%s</b>", tmpstr);
        g_free (tmpstr);

        switch (num_options) {
        case 1:  
                tmpstr = g_strdup (mark_list[0]);
                break;
        case 2:
                tmpstr = g_strconcat (mark_list[0], mark_list[1], NULL);
                break;
        case 3:
                tmpstr = g_strconcat (mark_list[0], mark_list[1], mark_list[2], NULL);
                break;
        case 4:
                tmpstr = g_strconcat (mark_list[0], mark_list[1], mark_list[2], 
                                      mark_list[3], NULL);
                break;
        case 5:
                tmpstr = g_strconcat (mark_list[0], mark_list[1], mark_list[2], 
                                      mark_list[3], mark_list[4], NULL);
                break;
        case 6:
                tmpstr = g_strconcat (mark_list[0], mark_list[1], mark_list[2], 
                                      mark_list[3], mark_list[4], mark_list[5], NULL);
                break;
        default:
                break;
        }
        
        if (tmpstr) {
                markup = g_strdup_printf ("<span size=\"small\" font_family=\"monospace\" "
                                          "foreground=\"#eaeac1\">%s</span>",
                                          tmpstr);
                g_free (tmpstr);
                for (int i = 0; i < num_options; i++)
                        g_free (mark_list[i]);
                
                bj_draw_set_player_text (markup);
                g_free (markup);
        }

        return bestOption;
}

int
Player::getBestOption (Hand *lplayer, int upCard, int lnumHands)
{
        int bestOption;
        double value, bestValue;

        reset ();

        // Player can always stand.

        bestValue = value = getValueStand (*lplayer, upCard);
        bestOption = KEY_S;

        // Player can't hit split aces.
        if (bj_hand_can_be_hit ()) {
                value = getValueHit (*lplayer, upCard);
                if (value > bestValue) {
                        bestValue = value;
                        bestOption = KEY_H;
                }
      
                // Check if player can double down.
                if (bj_hand_can_be_doubled ()) {
                        value = getValueDoubleDown (*lplayer, upCard);
                        if (value > bestValue) {
                                bestValue = value;
                                bestOption = KEY_D;
                        }
                }
        }
        
        // Check if player can split a pair.
        
        if (bj_hand_can_be_split ()) {
                value = getValueSplit(lplayer->cards[0].value (), upCard);
                if (value > bestValue) {
                        bestValue = value;
                        bestOption = KEY_P;
                }
        }

        // Check if player can surrender.
        if (bj_hand_can_be_surrendered ()) {
                value = -0.5;
                if (value > bestValue) {
                        bestValue = value;
                        bestOption = KEY_R;
                }
        }
        return bestOption;
}

Probabilities::Probabilities (bool lhitSoft17) : BJDealer(lhitSoft17)
{
}

void
Probabilities::reset ()
{
}

void
Probabilities::showProbabilities (BJShoe *ldistribution, int lupCard,
                                  bool condition)
{
        gchar *markup;
        gchar *mark_list[7];
  
        computeProbabilities (*ldistribution);
        double notBlackjack;
        if (condition)
                notBlackjack = (double)1 - getProbabilityBlackjack (lupCard);
        else
                notBlackjack = 1;
        reset ();

        mark_list[0] = g_strdup_printf ("<b>%s</b>\n", _("Dealer hand probabilities"));
        mark_list[1] = g_strdup_printf ("  %s         %5.2lf%%\n", _("Bust"),
                                        getProbabilityBust (lupCard) * 100.0 / notBlackjack);
        for (int count = 17; count <= 21; count++) {
                mark_list[count-15] = g_strdup_printf ("  %2d           %5.2lf%%\n", count,
                                                       getProbabilityCount (count, lupCard)
                                                       * 100.0 / notBlackjack);
        }

        markup = g_strconcat ("<span size=\"small\" font_family=\"monospace\" foreground=\"#eaeac1\">", 
                              mark_list[0], mark_list[1], mark_list[2], mark_list[3], 
                              mark_list[4], mark_list[5], mark_list[6],
                              "</span>", NULL);
  
        for (int i = 0; i < 7; i++)
                g_free (mark_list[i]);
        bj_draw_set_dealer_text (markup);
        g_free (markup);
}

const int allTens[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 4};

LoadablePlayer::LoadablePlayer (int lnumDecks, BJRules *lrules, BJStrategy & lstrategy,
                                Progress & progress, const char *filename = NULL)
        : Player (allTens, lrules, lstrategy, progress)
{
        gint ret = FALSE;

        if (filename != NULL)
                ret = reset (filename);

        if (!ret)
                BJPlayer::reset (BJShoe(lnumDecks), *lrules, lstrategy, progress);
}

gint
LoadablePlayer::reset (const char *filename)
{
        gint ret = FALSE;

        if (filename)
                ret = loadXML (filename);
        return ret;
}

void
LoadablePlayer::saveHand (int i)
{
        // cache playerHands[i] as well as index i
        gzwrite (fp, &i, sizeof (i));
        gzwrite (fp, &playerHands[i].cards, sizeof (playerHands[i].cards));
        gzwrite (fp, &playerHands[i].hitHand, sizeof (playerHands[i].hitHand));
        gzwrite (fp, &playerHands[i].nextHand, sizeof (playerHands[i].nextHand));
        gzwrite (fp, &playerHands[i].valueStand[0], sizeof (playerHands[i].valueStand[0]));
        gzwrite (fp, &playerHands[i].valueHit[0], sizeof (playerHands[i].valueHit[0]));
        gzwrite (fp, &playerHands[i].valueDoubleDown[0], sizeof (playerHands[i].valueDoubleDown[0]));
}

void
LoadablePlayer::saveHandXML (int i)
{
        // cache playerHands[i] as well as index i

        gzprintf (fp, "    <Hand n=\"%d\">\n", i);

        gzprintf (fp, "      <Cards d=\"");
        for (int j = 0; j < 10; j++)
                gzprintf (fp, "%d%s", playerHands[i].cards[j], (j < 9) ? "," : "");
        gzprintf (fp, "\"/>\n");

        gzprintf (fp, "      <Hit d=\"");
        for (int j = 0; j < 10; j++)
                gzprintf (fp, "%d%s", playerHands[i].hitHand[j], (j < 9) ? "," : "");
        gzprintf (fp, "\"/>\n");

        gzprintf (fp, "      <Next d=\"%d\"/>\n", playerHands[i].nextHand);

        gzprintf (fp, "      <ValueStand split=\"false\" d=\"");
        for (int j = 0; j < 10; j++)
                gzprintf (fp, "%f%s", playerHands[i].valueStand[0][j], (j < 9) ? "," : "");
        gzprintf (fp, "\"/>\n");

        gzprintf (fp, "      <ValueHit split=\"false\" d=\"");
        for (int j = 0; j < 10; j++)
                gzprintf (fp, "%f%s", playerHands[i].valueHit[0][j], (j < 9) ? "," : "");
        gzprintf (fp, "\"/>\n");
  
        gzprintf (fp, "      <ValueDouble split=\"false\" d=\"");
        for (int j = 0; j < 10; j++)
                gzprintf (fp, "%f%s", playerHands[i].valueDoubleDown[0][j], (j < 9) ? "," : "");
        gzprintf (fp, "\"/>\n");
  
        gzprintf (fp, "    </Hand>\n");
}

void
LoadablePlayer::saveCount (int count, bool soft)
{
        for (int i=playerHandCount[count][soft]; i; i=playerHands[i].nextHand)
                saveHand (i);
}

void
LoadablePlayer::saveCountXML (int count, bool soft)
{
        for (int i=playerHandCount[count][soft]; i; i=playerHands[i].nextHand)
                saveHandXML (i);
}

void
LoadablePlayer::savePlayerHandCountXML ()
{
        gzprintf (fp, "  <PlayerHandCount>\n");
        for (int i = 0; i < 22; i++) {
                gzprintf (fp, "    <Count n=\"%d\" hard=\"%d\" soft=\"%d\"/>\n",
                          i, playerHandCount[i][0], playerHandCount[i][1]);
        }
        gzprintf (fp, "  </PlayerHandCount>\n");
}

void
LoadablePlayer::saveValueSplitXML ()
{
        gzprintf (fp, "  <ValueSplit>\n");
        for (int i = 0; i < 10; i++)
                for (int j = 0; j < 10; j++) {
                        gzprintf (fp, "    <Split pair=\"%d\" up=\"%d\" d=\"%.20f\"/>\n",
                                  i, j, valueSplit[i][j]);
                }
        gzprintf (fp, "  </ValueSplit>\n");
}

void
LoadablePlayer::saveResplitXML ()
{
        gzprintf (fp, "  <Resplit d=\"");
        for (int i = 0; i < 10; i++)
                gzprintf (fp, "%d%s", resplit[i], (i < 9) ? "," : "");
        gzprintf (fp, "\"/>\n");
}

void
LoadablePlayer::saveOverallValuesXML ()
{
        gzprintf (fp, "  <OverallValues d=\"");
        for (int i = 0; i < 10; i++)
                gzprintf (fp, "%.20f%s", overallValues[i], (i < 9) ? "," : "");
        gzprintf (fp, "\"/>\n");
}

void
LoadablePlayer::saveOverallValueXML ()
{
        gzprintf (fp, "  <OverallValue d=\"");
        gzprintf (fp, "%.20f", overallValue);
        gzprintf (fp, "\"/>\n");
}

void
LoadablePlayer::savePlayerHandsXML ()
{
        int card, count;

        gzprintf (fp, "  <PlayerHands>\n");

        saveHandXML (0);
        for (card = 1; card <= 10; card++)
                saveHandXML (playerHands[0].hitHand[card - 1]);

        for (count = 21; count >= 11; count--)
                saveCountXML (count, false);
  
        for (count = 21; count >= 12; count--)
                saveCountXML (count, true);
  
        for (count = 10; count >= 4; count--)
                saveCountXML (count, false);

        gzprintf (fp, "  </PlayerHands>\n");
}

void
LoadablePlayer::save (const char *filename)
{
        int card, count;

        fp = gzopen (filename, "wb9");
        if (! fp) {
                cerr << "Could not open required file: " << filename << endl;
                return;
        }

        gzwrite (fp, &playerHandCount, sizeof (playerHandCount));
        gzwrite (fp, &valueSplit, sizeof (valueSplit));
        gzwrite (fp, &resplit, sizeof (resplit));
        gzwrite (fp, &overallValues, sizeof (overallValues));
        gzwrite (fp, &overallValue, sizeof (overallValue));

        saveHand (0);

        for (card = 1; card <= 10; card++)
                saveHand (playerHands[0].hitHand[card - 1]);

        for (count = 21; count >= 11; count--)
                saveCount (count, false);

        for (count = 21; count >= 12; count--)
                saveCount (count, true);

        for (count = 10; count >= 4; count--)
                saveCount (count, false);
}

void
LoadablePlayer::saveXML (const char *filename)
{

        fp = gzopen (filename, "wb9");
        if (! fp) {
                cerr << "Could not open required file: " << filename << endl;
                return;
        }

        gzprintf (fp, "<?xml version=\"1.0\"?>\n<BlackjackRule>\n");
        savePlayerHandCountXML ();
        saveValueSplitXML ();
        saveResplitXML ();
        saveOverallValuesXML ();
        saveOverallValueXML ();

        savePlayerHandsXML ();
        gzprintf (fp, "</BlackjackRule>\n");

        gzclose (fp);
}

gint
LoadablePlayer::load (const char *filename)
{
        gint i;
        // load numHands, playerHandCount, etc.
        // now load the playerHands array

        gzFile flp;

        flp = gzopen (filename, "rb");
        if (! flp) {
                cerr << "Could not open required file: " << filename << endl;
                return FALSE;
        }

        gzread (flp, &playerHandCount, sizeof (playerHandCount));
        gzread (flp, &valueSplit, sizeof (valueSplit));
        gzread (flp, &resplit, sizeof (resplit));
        gzread (flp, &overallValues, sizeof (overallValues));
        gzread (flp, &overallValue, sizeof (overallValue));

        while (! gzeof (flp)) {
                gzread (flp, &i, sizeof (i));
                gzread (flp, &playerHands[i].cards, sizeof (playerHands[i].cards));
                gzread (flp, &playerHands[i].hitHand, sizeof (playerHands[i].hitHand));
                gzread (flp, &playerHands[i].nextHand, sizeof (playerHands[i].nextHand));
                gzread (flp, &playerHands[i].valueStand[0], sizeof (playerHands[i].valueStand[0]));
                gzread (flp, &playerHands[i].valueHit[0], sizeof (playerHands[i].valueHit[0]));
                gzread (flp, &playerHands[i].valueDoubleDown[0], sizeof (playerHands[i].valueDoubleDown[0]));
                
        }

        gzclose (flp);
        return TRUE;
}

gint
LoadablePlayer::load_array_from_string (int *arr, const char *string, int n)
{
        gchar **strings;

        strings = g_strsplit (string, ",", n);

        for (int i = 0; strings[i] != NULL; i++) {
                long number;
                number = atoi (strings[i]);
                arr[i] = number;
        }
        g_strfreev (strings);
        return TRUE;
}

gint
LoadablePlayer::load_array_from_string (float *arr, const char *string, int n)
{
        gchar **strings;

        strings = g_strsplit (string, ",", n);

        for (int i = 0; strings[i] != NULL; i++) {
                double number;
                number = strtod (strings[i], NULL);
                arr[i] = (float) number;
        }
        g_strfreev (strings);
        return TRUE;
}

gint
LoadablePlayer::load_array_from_string (double *arr,
                                        const char *string,
                                        int n)
{
        gchar **strings;

        strings = g_strsplit (string, ",", n);

        for (int i = 0; strings[i] != NULL; i++) {
                double number;
                number = strtod (strings[i], NULL);
                arr[i] = number;
        }
        g_strfreev (strings);
        return TRUE;
}

gint
LoadablePlayer::parse_hand (xmlNodePtr node)
{
        xmlChar *text;
        xmlNodePtr cur;
        int index;

        text = xmlGetProp (node, (const xmlChar *) "n");
        index = atoi ((const char *)text);
        xmlFree (text);

        cur = node->children;
        while (cur != NULL) {
                if (!xmlStrcmp (cur->name, (const xmlChar *) "Cards")) {
                        text = xmlGetProp (cur, (const xmlChar *) "d");
                        if (!text) {
                                cerr << "Error reading file: invalid hand cards" << endl;
                                return FALSE;
                        }
                        load_array_from_string (playerHands[index].cards,
                                                (const char *)text, 10);
                        xmlFree (text);
                }
                else if (!xmlStrcmp (cur->name, (const xmlChar *) "Hit")) {
                        text = xmlGetProp (cur, (const xmlChar *) "d");
                        if (!text) {
                                cerr << "Error reading file: invalid hand hit index" << endl;
                                return FALSE;
                        }
                        load_array_from_string (playerHands[index].hitHand,
                                                (const char *)text, 10);
                        xmlFree (text);
                }
                else if (!xmlStrcmp (cur->name, (const xmlChar *) "Next")) {
                        text = xmlGetProp (cur, (const xmlChar *) "d");
                        if (!text) {
                                cerr << "Error reading file: invalid hand next value" << endl;
                                return FALSE;
                        }
                        playerHands[index].nextHand = atoi ((const char *)text);
                        xmlFree (text);
                }
                else if (!xmlStrcmp (cur->name, (const xmlChar *) "ValueStand")) {
                        text = xmlGetProp (cur, (const xmlChar *) "d");
                        if (!text) {
                                cerr << "Error reading file: invalid hand stand value" << endl;
                                return FALSE;
                        }
                        load_array_from_string (playerHands[index].valueStand[0],
                                                (const char *)text, 10);
                        xmlFree (text);
                }
                else if (!xmlStrcmp (cur->name, (const xmlChar *) "ValueHit")) {
                        text = xmlGetProp (cur, (const xmlChar *) "d");
                        if (!text) {
                                cerr << "Error reading file: invalid hand hit value" << endl;
                                return FALSE;
                        }
                        load_array_from_string (playerHands[index].valueHit[0],
                                                (const char *)text, 10);
                        xmlFree (text);
                }
                else if (!xmlStrcmp (cur->name, (const xmlChar *) "ValueDouble")) {
                        text = xmlGetProp (cur, (const xmlChar *) "d");
                        if (!text) {
                                cerr << "Error reading file: invalid hand double value" << endl;
                                return FALSE;
                        }
                        load_array_from_string (playerHands[index].valueDoubleDown[0],
                                                (const char *)text, 10);
                        xmlFree (text);
                }
                
                cur = cur->next;
        }
        return TRUE;
}

gint
LoadablePlayer::parse_hands (xmlNodePtr node)
{
        xmlNodePtr cur;

        cur = node->children;
        while (cur != NULL) {
                if (!xmlStrcmp (cur->name, (const xmlChar *) "Hand"))
                        parse_hand (cur);
                cur = cur->next;
        }

        return TRUE;
}

gint
LoadablePlayer::parse_hand_count (xmlNodePtr node)
{
        xmlNodePtr cur;
        xmlChar *text;
        int index, hard, soft;

        cur = node->children;
        while (cur != NULL) {
                if (!xmlStrcmp (cur->name, (const xmlChar *) "Count")) {
                        text = xmlGetProp (cur, (const xmlChar *) "n");
                        if (!text) {
                                cerr << "Error reading file: invalid count index" << endl;
                                return FALSE;
                        }
                        index = atoi ((const char *)text);
                        xmlFree (text);
                        
                        text = xmlGetProp (cur, (const xmlChar *) "hard");
                        if (!text) {
                                cerr << "Error reading file: invalid hard count" << endl;
                                return FALSE;
                        }
                        hard = atoi ((const char *)text);
                        xmlFree (text);
                        
                        text = xmlGetProp (cur, (const xmlChar *) "soft");
                        if (!text) {
                                cerr << "Error reading file: invalid soft count" << endl;
                                return FALSE;
                        }
                        soft = atoi ((const char *)text);
                        xmlFree (text);
                        playerHandCount[index][0] = hard;
                        playerHandCount[index][1] = soft;
                }
                cur = cur->next;
        }

        return TRUE;
}

gint
LoadablePlayer::parse_value_split (xmlNodePtr node)
{
        xmlNodePtr cur;
        xmlChar *text;
        int pair, up;
        double value;
  
        cur = node->children;
        while (cur != NULL) {
                if (!xmlStrcmp (cur->name, (const xmlChar *) "Split")) {
                        text = xmlGetProp (cur, (const xmlChar *) "pair");
                        if (!text) {
                                cerr << "Error reading file: invalid split pair index" << endl;
                                return FALSE;
                        }
                        pair = atoi ((const char *)text);
                        xmlFree (text);
                        
                        text = xmlGetProp (cur, (const xmlChar *) "up");
                        if (!text) {
                                cerr << "Error reading file: invalid split up index" << endl;
                                return FALSE;
                        }
                        up = atoi ((const char *)text);
                        xmlFree (text);
                        
                        text = xmlGetProp (cur, (const xmlChar *) "d");
                        if (!text) {
                                cerr << "Error reading file: invalid split value" << endl;
                                return FALSE;
                        }
                        value = strtod ((const char *)text, NULL);
                        xmlFree (text);
                        
                        valueSplit[pair][up] = value;
                }
                cur = cur->next;
        }

        return TRUE;
}

gint
LoadablePlayer::parse_resplit (xmlNodePtr node)
{
        xmlChar *text;

        if (!xmlStrcmp (node->name, (const xmlChar *) "Replit")) {
                text = xmlGetProp (node, (const xmlChar *) "d");
                if (!text) {
                        cerr << "Error reading file: invalid resplit value" << endl;
                        return FALSE;
                }
                load_array_from_string (resplit, (const char *)text, 10);
                xmlFree (text);
        }

        return TRUE;
}

gint
LoadablePlayer::parse_overall_values (xmlNodePtr node)
{
        xmlChar *text;

        if (!xmlStrcmp (node->name, (const xmlChar *) "OverallValues")) {
                text = xmlGetProp (node, (const xmlChar *) "d");
                if (!text) {
                        cerr << "Error reading file: invalid overall values" << endl;
                        return FALSE;
                }
                load_array_from_string (overallValues, (const char *)text, 10);
                xmlFree (text);
        }

        return TRUE;
}

gint
LoadablePlayer::parse_overall_value (xmlNodePtr node)
{
        xmlChar *text;

        if (!xmlStrcmp (node->name, (const xmlChar *) "OverallValue")) {
                text = xmlGetProp (node, (const xmlChar *) "d");
                if (!text) {
                        cerr << "Error reading file: invalid overall value" << endl;
                        return FALSE;
                }
                overallValue = strtod ((const char *)text, NULL);
                xmlFree (text);
        }

        return TRUE;
}

gint
LoadablePlayer::parse_document (xmlDocPtr doc)
{
        xmlNodePtr node;
        gint ret;

        node = xmlDocGetRootElement (doc);
        node = node->children;
        while (node != NULL) {
                ret = TRUE;
                if (!xmlStrcmp (node->name, (const xmlChar *) "PlayerHands"))
                        ret = parse_hands (node);
                else if (!xmlStrcmp (node->name, (const xmlChar *) "PlayerHandCount"))
                        ret = parse_hand_count (node);
                else if (!xmlStrcmp (node->name, (const xmlChar *) "ValueSplit"))
                        ret = parse_value_split (node);
                else if (!xmlStrcmp (node->name, (const xmlChar *) "Resplit"))
                        ret = parse_resplit (node);
                else if (!xmlStrcmp (node->name, (const xmlChar *) "OverallValues"))
                        ret = parse_overall_values (node);
                else if (!xmlStrcmp (node->name, (const xmlChar *) "OverallValue"))
                        ret = parse_overall_value (node);
                if (!ret)
                        return ret;
                node = node->next;
        }

        return TRUE;
}

gint
LoadablePlayer::loadXML (const char *filename)
{
        xmlDocPtr doc;
        gint ret;

        doc = xmlParseFile (filename);
        if (doc == NULL) {
                cerr << "Could not read required file: " << filename << endl;
                return FALSE;
        }

        if (!doc || !doc->children || !doc->children->name) {
                cerr << "Error reading required file: " << filename << endl;
                return FALSE;
        }
        
        ret = parse_document (doc);
    
        xmlFreeDoc (doc);
        return ret;
}
