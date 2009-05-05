/* -*- mode: C; indent-tabs-mode: t; tab-width: 2; c-basic-offset: 2; -*- */

/* game.h: All the code pretaining to the actual game is here.
 *
 * Copyright (c) 2004 by Callum McKenzie
 *
 */

#ifndef GAME_H
#define GAME_H

#include <glib.h>

/* Animation frame details. */
#define ACCELERATION 0.4

/* FIXME: These should not be hard-coded. (But how do we extract 
 * them from a file ?) */
#define NFRAMESSPIN 16
#define NFRAMESDEST 8
#define DESTFRAMESOFS NFRAMESSPIN
#define NFRAMESMOVED 16
#define MOVEDFRAMESOFS (NFRAMESSPIN + NFRAMESDEST)
#define NFRAMESMOVEL 16
#define MOVELFRAMESOFS (NFRAMESSPIN + NFRAMESDEST + NFRAMESMOVED)
#define NFRAMES (NFRAMESSPIN + NFRAMESDEST + NFRAMESMOVED + NFRAMESMOVEL)

#define SAVE_FORMAT_ID 0
#define SAVE_FORMAT_DIVIDER " "

/* Animation styles. */
enum {
  ANI_STILL,
  ANI_REDRAW,
  ANI_SPIN,
  ANI_SPINBACK,
  ANI_DESTROY,
  ANI_MOVE_DOWN,
  ANI_MOVE_LEFT,
};

/* The blank colour. */
extern const gint NONE;

enum {
  GAME_IDLE,
  GAME_SELECTED,
  GAME_DESTROYING,
  GAME_MOVING_DOWN,
  GAME_MOVING_LEFT
};

extern gint game_state;

typedef struct {
  gint8 colour;			/* Colour of the ball. -1 = empty. */
  gint8 frame;			/* Frame of the animation. */
  guint8 style;			/* The animation style. i.e. what the ball is doing. */
  guint8 visited;		/* Scratch variable for connected component finding. */
} game_cell;

/* FIXME: We have implicitly assumed that the dimensions of the
	 board can't exceed 127 x 127 */
typedef struct {
  gint8 x, y;
} coordinates;

/* The list of cells in the currently selected region. */
extern coordinates *selected;
/* The length of the selected list. */
extern gint count;

/* The actual game board. */
extern game_cell *board;

/* The names of the various board sizes. */
extern const gchar *scorenames[];

void reset_undo (void);
gboolean load_game (void);
gboolean save_game (void);
void clear_savegame (void);
void new_game (void);
void restore_game_state (void);
gchar *serialise_board_to_string (void);
gboolean restore_board_from_string (gchar * state);
void undo (void);
void redo (void);
int calculate_score (gint nballs);
void set_sizes (gint size);
game_cell *get_game_cell (int x, int y);
void find_connected_component (int x, int y);
void destroy_balls (void);
/* FIXME: Could this return gboolean too ? */
gint mark_falling_balls (void);
gboolean mark_shifting_balls (void);
void fixup_marks (void);
void end_of_move (void);

#endif /* GAME_H */
