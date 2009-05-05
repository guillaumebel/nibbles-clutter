/* -*- mode: C; indent-tabs-mode: t; tab-width: 2; c-basic-offset: 2; -*- */

/* same-gnome.h : The global state of the game.
 *
 * Copyright (c) 2004 by Callum McKenzie
 *
 */

#ifndef GLOBALS_H
#define GLOBALS_H

#include <glib.h>
#include <libgames-support/games-scores.h>

#define APPNAME "same-gnome"

#define DEFAULT_GAME_SIZE SMALL

#define MINIMUM_CANVAS_WIDTH 120
#define MINIMUM_CANVAS_HEIGHT 80

#define DEFAULT_THEME "planets.png"

#define THEME_VERSION "2.10"

#define MAX_COLOURS 4

#define KEY_THEME           "tileset"
#define KEY_SIZE            "size"
#define KEY_CUSTOM_WIDTH    "custom_width"
#define KEY_CUSTOM_HEIGHT   "custom_height"
#define KEY_FAST_ANIMATION  "fast_animation"
#define KEY_SAVEGAME        "savegame"

/* We start at 1 so we can distinguish "unset" from a valid
 * value. */
enum {
  UNSET = 0,
  CUSTOM = 1,			/* FIXME: Are we going to use this. */
  SMALL,
  MEDIUM,
  LARGE,
  MAX_SIZE,
};

extern GamesScores *highscores;
extern const GamesScoresCategory scorecats[];

/* A list of board sizes of the form: width, height, colours.  This has
 * to be kept in sync with the above enum and so has some empty entries. */
extern gint board_sizes[MAX_SIZE][3];

/* The parameters for the current game. */
extern gint board_width;
extern gint board_height;
extern gint board_ncells;
extern gint ncolours;

/* Where the user can stash their themes. */
extern gchar *localthemedir;

extern gchar *theme;

extern gint game_size;

#endif /* GLOBALS_H */
