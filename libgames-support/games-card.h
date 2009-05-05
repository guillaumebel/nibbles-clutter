/*
   Copyright © 2004 Callum McKenzie
   Copyright © 2007, 2008 Christian Persch

   This library is free software; you can redistribute it and'or modify
   it under the terms of the GNU Library General Public License as published 
   by the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* Authors:   Callum McKenzie <callum@physics.otago.ac.nz> */

/* Common definitions for all card handling routines. */

#ifndef GAMES_CARD_H
#define GAMES_CARD_H

#include <glib.h>

G_BEGIN_DECLS

/* __attribute__((__packed__)) is needed on some archs to make
 * the Card type below fit into one byte. See bug #512799.
 */
#if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ > 6)
#define GNOME_GAMES_GNUC_PACKED \
  __attribute__((__packed__))
#else
#define GNOME_GAMES_GNUC_PACKED
#endif  /* !__GNUC__ */

/* A card */

/* Black Joker: value = 0, suit = spade or club
 * Red Joker: value = 0, suit = heart or diamond
 */
typedef union {
  guint8 value;
  struct {
    guint8 face_down : 1;
    guint8 suit : 2;
    guint8 rank : 4;
  } attr;
} GNOME_GAMES_GNUC_PACKED Card;

typedef int _games_card_size_assert[sizeof (Card) == sizeof (guint8) ? 1 : -1]; /* static assertion */

#define CARD(c)               ((Card) c)
#define CARD_UINT(c)          (c.value)

#define CARD_GET_SUIT(c)      (c.attr.suit)
#define CARD_GET_RANK(c)      (c.attr.rank)
#define CARD_GET_FACE_DOWN(c) (c.attr.face_down)

#define POINTER_TO_CARD(ptr)  ((Card) (guint8) GPOINTER_TO_UINT (ptr))
#define CARD_TO_POINTER(card) (GUINT_TO_POINTER((guint) card.value))

/* Some defines */

typedef enum {
  /* Cards */
  GAMES_CARD_JOKER = 0,
  GAMES_CARD_ACE = 1,
  GAMES_CARD_TWO = 2,
  GAMES_CARD_THREE = 3,
  GAMES_CARD_FOUR = 4,
  GAMES_CARD_FIVE = 5,
  GAMES_CARD_SIX = 6,
  GAMES_CARD_SEVEN = 7,
  GAMES_CARD_EIGHT = 8,
  GAMES_CARD_NINE = 9,
  GAMES_CARD_TEN = 10,
  GAMES_CARD_JACK = 11,
  GAMES_CARD_QUEEN = 12,
  GAMES_CARD_KING = 13,
  GAMES_CARD_ACE_HIGH = 14,

  /* Suites */
  GAMES_CARDS_CLUBS = 0,
  GAMES_CARDS_DIAMONDS = 2,
  GAMES_CARDS_HEARTS = 1,
  GAMES_CARDS_SPADES = 3,

  /* Jokers */
  GAMES_CARD_BLACK_JOKER = 52,
  GAMES_CARD_RED_JOKER = 53,

  /* Special */
  GAMES_CARD_BACK = 54,
  GAMES_CARD_SLOT = 55,
  GAMES_CARDS_TOTAL = 56,
} GamesCardIDType;

int games_card_get_node_by_suit_and_rank_snprintf (char *buffer,
                                                   gsize bufsize,
                                                   int suit,
                                                   int rank);

int games_card_get_node_by_id_snprintf (char *buffer,
                                        gsize bufsize,
                                        int card_id);

int games_card_get_name_by_id_snprintf (char *buffer,
                                        gsize bufsize,
                                        int card_id);

char * games_card_get_name_by_id (int card_id);

const char * games_card_get_localised_rank_symbol (int rank);

G_END_DECLS

#endif /* !GAMES_CARD_H */
