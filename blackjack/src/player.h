// -*- mode:C++; tab-width:2; c-basic-offset:2; indent-tabs-mode:nil -*-
/* Blackjack - hand.h
 * Copyright (C) 2003 William Jon McCann <mccann@jhu.edu>
 *
 * This game is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef BJ_PLAYER_H
#define BJ_PLAYER_H

#include <zlib.h>
#include <libxml/tree.h>
#include <libxml/parser.h>

// Implement BJProgress interface.

class Progress:public BJProgress {
public:
  Progress ();
  void indicate (int percentComplete);
private:
  int last;
};

// A Card object includes rank and suit.

class Card {
public:
  int rank, suit;
  int value ();
};

// A Hand object represents a drawing of a blackjack hand.

class Hand:public BJHand {
public:
  Card cards[22];

  // pointer to the hslot the hand is in
  hslot_type hslot;

  int deal (Card card, bool faceUp = true);
  void draw (int card, bool faceUp = true);
  void redraw ();
  void hideCount ();
  void showCount (bool blackjack = true);
  void reset ();
};

// A Shoe object includes arrangement, not just distribution, of cards in the
// shoe.

class Shoe {
public:
  Shoe (int numDecks = 1);
   ~Shoe ();
  void shuffle ();
  Card deal ();
  int numCards;

private:
  int numDecks;
  Card *cards;
  void swap (int card1, int card2);

};

// A PlayerHand object represents a particular hand dealt on a single initial
// wager.

class PlayerHand:public Hand {
public:
  gfloat wager;
  PlayerHand *nextHand;
  void showWager ();
  void showOption (int option);
};

// A Player object displays expected values and determines optimal strategy.

class Player:public BJPlayer {
public:
  Player (int numDecks,
	  BJRules * rules, BJStrategy & strategy, Progress & progress);
    Player (const int numDecks[],
	    BJRules * rules, BJStrategy & strategy, Progress & progress);
  void reset ();
  int showOptions (Hand * player, int upCard, int numHands);
  int getBestOption (Hand * player, int upCard, int numHands);
private:
    BJRules * rules;
};

// A Probabilities object displays probabilities of outcomes of the dealer's
// hand.

class Probabilities:public BJDealer {
public:
  Probabilities (bool hitSoft17);
  void reset ();

  // If condition is false, then P(blackjack) may be non-zero.
  void showProbabilities (BJShoe * distribution, int upCard,
			  bool condition = true);
};

// Extend BJPlayer so that we can load cache files

class LoadablePlayer:public Player {
public:
  LoadablePlayer (int numDecks, BJRules * rules, BJStrategy & strategy,
		  Progress & progress, const char *filename);
  gint reset (const char *filename);
  gint load (const char *filename);
  gint loadXML (const char *filename);
  void save (const char *filename);
  void saveXML (const char *filename);
private:
    gzFile fp;
  void saveHand (int i);
  void saveCount (int count, bool soft);
  void savePlayerHandCountXML (void);
  void savePlayerHandsXML (void);
  void saveValueSplitXML (void);
  void saveResplitXML (void);
  void saveOverallValuesXML (void);
  void saveOverallValueXML (void);
  void saveHandXML (int i);
  void saveCountXML (int count, bool soft);
  gint parse_document (xmlDocPtr doc);
  gint parse_hand (xmlNodePtr node);
  gint parse_hands (xmlNodePtr node);
  gint parse_hand_count (xmlNodePtr node);
  gint parse_value_split (xmlNodePtr node);
  gint parse_resplit (xmlNodePtr node);
  gint parse_overall_values (xmlNodePtr node);
  gint parse_overall_value (xmlNodePtr node);
  gint load_array_from_string (int *arr, const char *string, int n);
  gint load_array_from_string (float *arr, const char *string, int n);
  gint load_array_from_string (double *arr, const char *string, int n);
};


#endif
