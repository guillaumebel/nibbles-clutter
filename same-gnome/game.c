/* -*- mode: C; indent-tabs-mode: t; tab-width: 2; c-basic-offset: 2; -*- */

/* game.c: All the code pretaining to the actual game is here.
 *
 * Copyright (c) 2004 by Callum McKenzie
 *
 */

#include <config.h>

#include <string.h>

#include <glib/gi18n.h>

#include <libgames-support/games-conf.h>
#include <libgames-support/games-scores.h>

#include "same-gnome.h"

#include "drawing.h"
#include "game.h"
#include "ui.h"

typedef struct _game_state_record {
  gint score;
  gint8 *colours;
} game_state_record;

/* The blank colour. */
const gint NONE = -1;

game_cell *board = NULL;

gint last_board_width = -1;
gint last_board_height = -1;

guint32 score;

/* The stack used for the "floodfill" algorithm. */
coordinates *findstack = NULL;
/* The list of cells in the currently selected region. */
coordinates *selected = NULL;
/* The length of the selected list. */
gint count = 0;

gint game_state = GAME_IDLE;

/* The undo/redo list. It stores arrays of colours. */
GList *urlist = NULL;
/* This points the position of the current game in the list.
 * The next pointer points to the future, the prev pointer to
 * the past. */
GList *urptr;

int
calculate_score (gint nballs)
{
  if (nballs < 3)
    return 0;

  return (nballs - 2) * (nballs - 2);
}

static void
game_state_free (game_state_record * g)
{
  g_free (g->colours);
  g_free (g);
}

static void
free_urlist (GList * l)
{
  if (l == NULL)
    return;

  g_list_foreach (l, (GFunc) game_state_free, NULL);
  g_list_free (l);
}

static void
record_game_state (void)
{
  game_state_record *g;
  int i;
  game_cell *c;
  gint8 *p;

  g = g_malloc (sizeof (game_state_record));

  if (urptr) {
    if (urptr->next) {		/* Erase the future, if we have one. */
      free_urlist (urptr->next);
      urptr->next = NULL;
    }
    urptr = g_list_append (urptr, g);
    urptr = urptr->next;
  } else {			/* We have a new, blank, undo list. */
    urptr = g_list_append (urptr, g);
    urlist = urptr;
  }

  /* FIXME: This is a bit ugly, but necessary so we can keep
   * track of the beginning of the list. */
  if (urlist == NULL)
    urlist = urptr;

  g->score = score;
  g->colours = g_malloc (board_ncells * sizeof (gint8));

  p = g->colours;
  c = board;
  for (i = 0; i < board_ncells; i++) {
    *p = c->colour;
    p++;
    c++;
  }

  set_undoredo_sensitive (urptr->prev != NULL, urptr->next != NULL);
}

void
restore_game_state (void)
{
  game_cell *c;
  gint8 *p;
  int i;
  game_state_record *g;

  g = urptr->data;
  p = g->colours;
  c = board;

  for (i = 0; i < board_ncells; i++) {
    c->colour = *p;
    c->style = ANI_STILL;
    c->frame = 0;
    p++;
    c++;
  }

  score = g->score;
  show_score (score);

  game_state = GAME_IDLE;
}

void
undo (void)
{
  if (urptr->prev) {
    urptr = urptr->prev;
    restore_game_state ();
    set_undoredo_sensitive (urptr->prev != NULL, TRUE);
  }
}

void
redo (void)
{
  if (urptr->next) {
    urptr = urptr->next;
    restore_game_state ();
    set_undoredo_sensitive (TRUE, urptr->next != NULL);
  }
}

void
set_sizes (gint size)
{
  game_size = size;

  board_width = board_sizes[size][0];
  board_height = board_sizes[size][1];
  ncolours = board_sizes[size][2];

  board_ncells = board_height * board_width;

  games_conf_set_integer (NULL, KEY_SIZE, size);

  games_scores_set_category (highscores, scorecats[size - SMALL].key);

  new_frame_ratio (board_width, board_height);
}

game_cell *
get_game_cell (int x, int y)
{
  return board + y * board_width + x;
}

/* FIXME: The idiom used in push and pop is strange and may be a bad thing. */

static void
push (coordinates ** s, gint8 x, gint8 y)
{
  if ((x < 0) || (y < 0) || (x >= board_width) || (y >= board_height))
    return;

  (*s)->x = x;
  (*s)->y = y;
  (*s)++;
}

static void
pop (coordinates ** s, gint8 * x, gint8 * y)
{
  (*s)--;
  *x = (*s)->x;
  *y = (*s)->y;
}

void
find_connected_component (int x, int y)
{
  coordinates *stack;
  coordinates *list;
  guchar colour;
  game_cell *c;
  gint8 i, j;
  int k;

  c = board;
  for (k = 0; k < board_ncells; k++) {
    c->visited = 0;
    c++;
  }

  count = 0;

  c = get_game_cell (x, y);
  colour = c->colour;
  if (c->colour == NONE)
    return;

  list = selected;

  stack = findstack;
  push (&stack, x, y);

  while (stack > findstack) {
    pop (&stack, &i, &j);
    c = get_game_cell (i, j);
    if ((!c->visited) && (c->colour == colour)) {
      push (&list, i, j);
      count++;
      push (&stack, i - 1, j);
      push (&stack, i + 1, j);
      push (&stack, i, j - 1);
      push (&stack, i, j + 1);
    }
    c->visited = 1;
  }
  game_state = GAME_SELECTED;
}

void
destroy_balls (void)
{
  gint i;
  gint8 x, y;
  coordinates *list;
  game_cell *p;

  if ((game_state != GAME_SELECTED) || (count <= 1))
    return;

  list = selected + count;

  for (i = 0; i < count; i++) {
    pop (&list, &x, &y);
    p = get_game_cell (x, y);
    p->style = ANI_DESTROY;
  }

  game_state = GAME_DESTROYING;

  score += calculate_score (count);
  show_score (score);

  count = 0;

  /* We add the removed columns to the undo list later. */

  /* The end of game check is also not triggered here, we want the balls 
   * to settle first. It is called from the animation code. */
    
  start_animation();
}

gint
mark_falling_balls (void)
{
  gint i, j;
  game_cell *p, *q;
  gint count;

  /* Scan up from the second to last row. */
  p = board + (board_height - 1) * board_width - 1;
  count = 0;
  for (j = 0; j < (board_height - 1); j++)
    for (i = 0; i < board_width; i++) {
      q = p + board_width;
      if ((q->colour == NONE) && (p->colour != NONE)) {
	q->colour = p->colour;
	q->frame = MOVEDFRAMESOFS;
	q->style = ANI_MOVE_DOWN;

	p->colour = NONE;
	p->frame = MOVEDFRAMESOFS;
	p->style = ANI_MOVE_DOWN;

	count++;
      }
      p--;
    }
    
  if (count)
    start_animation();

  return count;
}

gboolean
mark_shifting_balls (void)
{
  gint i, j, k;
  game_cell *p, *q;

  /* We scan the last row to determine where to start moving balls.
   * Once we find an empty cell beside a full cell we start moving.
   * i.e. we collapse from the right. */

  p = board + board_ncells - 2;
  k = board_width - 2;
  for (i = 0; i < (board_width - 1); i++) {
    q = p + 1;
    if ((p->colour == NONE) && (q->colour != NONE))
      break;
    k--;
    p--;
  }

  if (k < 0)
    return FALSE;

  for (j = 0; j < board_height; j++) {
    p = board + j * board_width + k;
    for (i = k; i < board_width - 1; i++) {
      q = p + 1;
      p->colour = q->colour;
      p->frame = MOVELFRAMESOFS;
      p->style = ANI_MOVE_LEFT;
      p++;
    }
    p->colour = NONE;
    p->frame = MOVELFRAMESOFS;
    p->style = ANI_MOVE_LEFT;
  }
    
  start_animation();

  return TRUE;
}

static void
game_over (void)
{
  gint place;
  GamesScoreValue hiscore;

  hiscore.plain = score;

  place = games_scores_add_score (highscores, hiscore);

  game_over_dialog (place);
  clear_savegame ();
}

static void
end_of_game_check (void)
{
  int i, j;
  game_cell *p, *q;

  /* Check to see if the board has been cleared: that earns a 1000pt bonus */
  p = board + (board_height - 1) * board_width;
  if (p->colour == NONE) {
    score += 1000;
    show_score (score);
    /* FIXME: This is long enough to resize the window upwards when
     * the game finishes. */
    set_message_general (_("1000 point bonus for clearing the board!"));
    game_over ();
    return;
  }

  /* Look for horizontal neighbours. */
  for (j = 0; j < board_height; j++) {
    p = board + j * board_width;
    q = p + 1;
    for (i = 0; i < board_width - 1; i++) {
      if ((p->colour != NONE) && (p->colour == q->colour))
	return;
      p++;
      q++;
    }
  }

  /* Look for vertical neighbours. */
  for (j = 0; j < board_height - 1; j++) {
    p = board + j * board_width;
    q = p + board_width;
    for (i = 0; i < board_width; i++) {
      if ((p->colour != NONE) && (p->colour == q->colour))
	return;
      p++;
      q++;
    }
  }

  game_over ();

}

void
end_of_move (void)
{
  game_state = GAME_IDLE;
  end_of_game_check ();
  /* FIXME: This placement gives odd semantics when undo is called
   * during an animation. */
  record_game_state ();
}

gchar *
serialise_board_to_string (void)
{
  gint i;
  gchar buff[2];
  gchar *state = (gchar *) g_malloc0 (board_ncells + 1);

  for (i = 0; i < board_ncells; i++) {
    if (board[i].colour == NONE)
      g_strlcat (state, "_", board_ncells + 1);
    else {
      g_snprintf (buff, 2, "%x", board[i].colour);
      g_strlcat (state, buff, board_ncells + 1);
    }
  }

  return state;
}

void
reset_undo (void)
{
  /* Free and reset the memory for the undo queue. */
  free_urlist (urlist);
  urlist = NULL;
  urptr = NULL;
  record_game_state ();

  set_undoredo_sensitive (FALSE, FALSE);
  clear_message ();
}

gboolean
restore_board_from_string (gchar * state)
{
  gint i;
  game_cell *p;

  if (!state)
    return FALSE;

  if (strlen (state) != board_ncells)
    return FALSE;

  for (i = 0; i < board_ncells; i++) {
    p = &board[i];
    if (state[i] == '_')
      p->colour = NONE;
    else {
      if (!g_ascii_isxdigit (state[i]))
	return FALSE;		/* n.b. board state screwed, reset req. */
      p->colour = g_ascii_tolower (state[i]) - '0';

    }
    p->frame = 0;
    p->style = ANI_STILL;
  }

  return TRUE;
}

void
clear_savegame (void)
{
  games_conf_set_string (NULL, KEY_SAVEGAME, "");
}

gboolean
load_game (void)
{
  gchar *savestring, **tokens;

  gint lsize, lscore, lversion;

  savestring = games_conf_get_string (NULL, KEY_SAVEGAME, NULL);

  if (!savestring || savestring[0] == '\0') {
    g_free (savestring);
    return FALSE;
  }

  tokens = g_strsplit (savestring, SAVE_FORMAT_DIVIDER, 4);

  if (g_strv_length (tokens) != 4) {
    g_strfreev (tokens);
    g_free (savestring);
    return FALSE;
  }

  lversion = (gint) g_ascii_strtoull (tokens[0], NULL, 10);
  lsize = (gint) g_ascii_strtoull (tokens[1], NULL, 10);
  lscore = (gint) g_ascii_strtoull (tokens[2], NULL, 10);

  score = lscore;
  set_sizes (lsize);

  if (board == NULL)
    board = g_new0 (game_cell, board_width * board_height);
  if (findstack == NULL)
    findstack = g_malloc (sizeof (coordinates) * (board_ncells * 4));
  if (selected == NULL)
    selected =
      g_malloc (sizeof (coordinates) * (board_ncells / ncolours + 1));

  if (!restore_board_from_string (g_strstrip (tokens[3]))) {
    g_strfreev (tokens);
    g_free (savestring);
    return FALSE;
  }

  g_strfreev (tokens);
  g_free (savestring);

  reset_undo ();
  redraw ();
  show_score (score);
  clear_savegame ();

  /* FIXME: This is all a bit messy, but it is the easiest way to 
   * start the game again if it was saved mid-fall. */
  if (mark_falling_balls ())
    game_state = GAME_MOVING_DOWN;
  else if (mark_shifting_balls ())
    game_state = GAME_MOVING_LEFT;
  else
    game_state = GAME_IDLE;

  return TRUE;
}

gboolean
save_game (void)
{
  gchar *boardstring, *savestring;

  boardstring = serialise_board_to_string ();

  savestring = g_strdup_printf ("%d%s%d%s%d%s%s", SAVE_FORMAT_ID,
				SAVE_FORMAT_DIVIDER, game_size,
				SAVE_FORMAT_DIVIDER, score,
				SAVE_FORMAT_DIVIDER, boardstring);

  games_conf_set_string (NULL, KEY_SAVEGAME, savestring);

  g_free (boardstring);
  g_free (savestring);

  return TRUE;
}

void
new_game (void)
{
  int i, j, c, l;
  game_cell *p;

  /* Reallocate the memory for the board if necessary. */
  if ((board_height != last_board_height) ||
      (board_width != last_board_width)) {
    if (board != NULL)
      g_free (board);
    if (findstack != NULL)
      g_free (findstack);
    if (selected != NULL)
      g_free (selected);
    board = g_new0 (game_cell, board_width * board_height);
    /* FIXME: *4 is way overkill, but may be the only way to be sure. */
    findstack = g_malloc (sizeof (coordinates) * (board_ncells * 4));
    selected =
      g_malloc (sizeof (coordinates) * (board_ncells / ncolours + 1));
    last_board_width = board_width;
    last_board_height = board_height;
  }

  /* Allocate equal numbers of each colour across the board. */
  p = board;
  c = 0;
  for (j = 0; j < board_height; j++)
    for (i = 0; i < board_width; i++) {
      p->colour = c;
      c = (c + 1) % ncolours;
      p->frame = 0;
      p->style = ANI_STILL;
      p++;
    }

  /* Randomise the colours. */
  l = board_ncells;
  for (i = 0; i < l; i++) {
    j = g_random_int_range (0, l);
    c = board[j].colour;
    board[j].colour = board[i].colour;
    board[i].colour = c;
  }

  game_state = GAME_IDLE;
  score = 0;
  show_score (score);

  reset_undo ();
  clear_savegame ();
  redraw ();
}
