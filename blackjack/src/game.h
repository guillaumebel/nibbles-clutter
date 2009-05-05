// -*- mode:C++; tab-width:2; c-basic-offset:2; indent-tabs-mode:nil -*-
/* Blackjack - game.h
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

#ifndef BJ_GAME_H
#define BJ_GAME_H

#include "player.h"

void bj_game_find_rules (const gchar * variation);

gchar *bj_game_file_to_name (const gchar *);
void bj_game_new (const gchar *, guint *);
void bj_clear_table (void);

void bj_game_show_hand_counts (void);

gdouble bj_get_deal_delay (void);

gboolean bj_game_is_active (void);
void bj_game_set_active (gboolean);
gboolean bj_game_is_first_hand (void);

char *bj_game_get_rules_name (void);
const char *bj_game_get_rules_file (void);
GList *bj_game_get_rules_list (void);

// Extend BJRules
class BJGameRules:public BJRules {
public:
  BJGameRules (bool hitSoft17, bool doubleAnyTotal, bool double9,
	       bool doubleSoft, bool doubleAfterHit, bool doubleAfterSplit,
	       bool resplit, bool resplitAces, bool lateSurrender,
	       int numDecks, int dealerSpeed);
  int getDealerSpeed (void);
  int getNumDecks (void);

protected:
  int numDecks, dealerSpeed;
};

BJGameRules *bj_game_read_rules (const gchar * filename);
BJGameRules *bj_game_find_and_read_rules (const gchar * filename);

// GAME GLOBALS

extern BJGameRules *rules;
extern LoadablePlayer *strategy;
extern Hand *dealer;
extern Probabilities *dealerProbabilities;
extern Shoe *shoe;
extern BJShoe *distribution;
extern gint numDecks;

extern PlayerHand *player;
extern GList *playerHands;

extern Card tempCard;
extern PlayerHand *tempHand;

#endif
