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

#ifndef BJ_HAND_H
#define BJ_HAND_H

void bj_hand_new (void);
void bj_hand_finish (void);
void bj_hand_cancel (void);
gboolean bj_hand_events_pending (void);

gboolean bj_hand_can_be_hit (void);
gboolean bj_hand_can_be_doubled (void);
gboolean bj_hand_can_be_split (void);
gboolean bj_hand_can_be_surrendered (void);
gint bj_get_hand_results (gint, gint);

void bj_hand_show_dealer_probabilities (void);
void bj_hand_show_options (void);

char *bj_hand_get_best_option_string (char **secondary_message);

void bj_deal_card_to_player (void);
void bj_deal_card_to_dealer (gboolean);
void bj_deal_card_to_dealer_distribution (gboolean);

hslot_type bj_hand_get_slot (void);

void bj_hand_stand (void);
void bj_hand_hit_with_delay (void);
void bj_hand_double (void);
void bj_hand_split (void);
void bj_hand_surrender (void);
void bj_hand_finish_play (void);
void bj_hand_new_deal (void);

#endif
