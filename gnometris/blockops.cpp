/* -*- mode:C++; tab-width:8; c-basic-offset:8; indent-tabs-mode:true -*- */
/*
 * written by J. Marcin Gorycki <marcin.gorycki@intel.com>
 * massively altered for Clutter by Jason D. Clinton <me@jasonclinton.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For more details see the file COPYING.
 */

#include <config.h>
#include "blockops.h"
#include "blocks.h"
#include <clutter-gtk/clutter-gtk.h>

#define FONT "Sans Bold"

/* fuck C++ */
void
Block::reap_block (Block *block)
{
	delete block;
}

Block::Block ():
	what(EMPTY),
	actor(NULL),
	x(0),
	y(0),
	move_behaviour(NULL),
	fall_behaviour(NULL),
	explode_move_behaviour(NULL)
{}

Block::~Block ()
{
	if (actor)
		clutter_actor_destroy (CLUTTER_ACTOR(actor));
	if (move_behaviour)
		g_object_unref (move_behaviour);
	if (fall_behaviour)
		g_object_unref (fall_behaviour);
	if (explode_move_behaviour)
		g_object_unref (explode_move_behaviour);
}

void
Block::createActor (ClutterActor *chamber, ClutterActor *texture_source)
{
	if (actor)
		clutter_actor_destroy (CLUTTER_ACTOR(actor));
	actor = clutter_clone_new (CLUTTER_ACTOR(texture_source));
	clutter_group_add (CLUTTER_GROUP (chamber), actor);
	clutter_actor_set_position (CLUTTER_ACTOR(actor), x, y);
	clutter_actor_show (CLUTTER_ACTOR(actor));
}

void
Block::bindAnimations (BlockOps *f)
{
	move_behaviour = clutter_behaviour_path_new_with_knots (f->move_alpha,
								NULL, 0);
	fall_behaviour = clutter_behaviour_path_new_with_knots (f->fall_alpha,
								NULL, 0);
	explode_move_behaviour = clutter_behaviour_path_new_with_knots (f->explode_alpha,
									NULL, 0);
}

Block&
Block::move_from (Block& b, BlockOps *f)
{
	if (this != &b) {
		what = b.what;
		b.what = EMPTY;
		color = b.color;
		b.color = 0;
		if (actor) {
			//this shouldn't happen
			f->destroy_blocks = g_list_prepend (f->destroy_blocks, actor);
		}
		if (b.actor) {
			const ClutterKnot knot_line[] = {{b.x, b.y}, {x, y}};
			fall_behaviour = clutter_behaviour_path_new_with_knots (f->fall_alpha,
										knot_line, 2);
			clutter_behaviour_apply (fall_behaviour, b.actor);
		}
		actor = b.actor;
		b.actor = NULL;
	}
	return *this;
}

/******************************************************************************/

gboolean
BlockOps::move_end (ClutterTimeline *time, BlockOps *f)
{
	//clutter_path_clear (clutter_behaviour_path_get_path (f->move_behaviour));
	return FALSE;
}

gboolean
BlockOps::explode_end (ClutterTimeline *time, BlockOps *f)
{
	Block *block;
	g_list_foreach (f->destroy_blocks,
			(GFunc)Block::reap_block,
			block);
	g_list_free (f->destroy_blocks);
	return FALSE;
}

gboolean
BlockOps::fall_end (ClutterTimeline *tml, BlockOps *f)
{
	//After fall, start the earthquake effect
	ClutterPath *tmp_path = clutter_path_new ();
	clutter_path_add_move_to (tmp_path, f->center_anchor_x,
				  f->center_anchor_y);
	clutter_path_add_move_to (tmp_path, f->center_anchor_x,
				  f->center_anchor_y + f->cell_height *
				  f->quake_ratio);
	return FALSE;
}

BlockOps::BlockOps() :
	destroy_blocks(NULL),
	quake_ratio(0.0),
	background(NULL),
	foreground(NULL),
	width(0),
	height(0),
	cell_width(0),
	cell_height(0),
	renderer(NULL),
	themeID(-1),
	blocknr(0),
	rot(0),
	color(0),
	backgroundImage(NULL),
	center_anchor_x(0),
	center_anchor_y(0)
{
	field = new Block*[COLUMNS];

	for (int i = 0; i < COLUMNS; ++i)
		field[i] = new Block[LINES];

	w = gtk_clutter_embed_new ();

	g_signal_connect (w, "size_allocate", G_CALLBACK (resize), this);

	gtk_widget_set_size_request (w, COLUMNS*190/LINES, 190);

	ClutterActor *stage;
	stage = gtk_clutter_embed_get_stage (GTK_CLUTTER_EMBED (w));

	playingField = clutter_group_new ();
	clutter_group_add (CLUTTER_GROUP(stage), CLUTTER_ACTOR(playingField));
	
	move_time = clutter_timeline_new_for_duration (60);
	g_signal_connect (move_time, "completed", G_CALLBACK
			  (BlockOps::move_end), this);
	move_alpha = clutter_alpha_new_full (move_time,
					     CLUTTER_EASE_OUT_QUAD);
	
	fall_time = clutter_timeline_new_for_duration (120);
	g_signal_connect (fall_time, "completed", G_CALLBACK
			  (BlockOps::fall_end), this);
	fall_alpha = clutter_alpha_new_full (fall_time,
					     CLUTTER_EASE_IN_QUAD);
					     
	explode_time = clutter_timeline_new_for_duration (120);
	g_signal_connect (explode_time, "completed", G_CALLBACK
			  (BlockOps::explode_end), this);
	explode_alpha = clutter_alpha_new_full (explode_time,
						CLUTTER_EASE_IN_QUAD);
	explode_fade_behaviour = clutter_behaviour_opacity_new (explode_alpha,
								255, 0);
	quake_time = clutter_timeline_new_for_duration (360);
	quake_alpha = clutter_alpha_new_full (quake_time,
					      CLUTTER_EASE_OUT_ELASTIC);
	quake_behaviour = clutter_behaviour_path_new_with_knots (quake_alpha,
								 NULL, 0);
}

BlockOps::~BlockOps()
{
	for (int i = 0; i < COLUMNS; ++i)
		delete[] field[i];

	delete[] field;
	
	g_object_unref (move_time);
	g_object_unref (move_alpha);
	g_object_unref (fall_time);
	g_object_unref (fall_alpha);
	g_object_unref (explode_time);
	g_object_unref (explode_alpha);
	g_object_unref (explode_fade_behaviour);
	g_object_unref (quake_time);
	g_object_unref (quake_alpha);
	g_object_unref (quake_behaviour);
}

bool
BlockOps::blockOkHere(int x, int y, int b, int r)
{
	x -= 2;

	for (int x1 = 0; x1 < 4; ++x1)
	{
		for (int y1 = 0; y1 < 4; ++y1)
		{
			if (blockTable[b][r][x1][y1] && (x1 + x < 0))
				return false;
			if (blockTable[b][r][x1][y1] && (x1 + x >= COLUMNS))
				return false;
			if (blockTable[b][r][x1][y1] && (y1 + y >= LINES))
				return false;
			if (blockTable[b][r][x1][y1] && field[x + x1][y1 + y].what == LAYING)
				return false;
		}
	}

	return true;
}

int
BlockOps::getLinesToBottom()
{
	int lines = LINES;

	for (int x = 0; x < 4; ++x)
	{
		for (int y = 3; y >= 0; --y)
		{
			if (!blockTable[blocknr][rot][x][y])
				continue;
			int yy = posy + y;
			for (; yy < LINES; ++yy)
			{
				if (field[posx + x - 2][yy].what == LAYING)
					break;
			}
			int tmp = yy - posy - y;
			if (lines > tmp)
				lines = tmp;
		}
	}

	return lines;
}

bool
BlockOps::moveBlockLeft()
{
	bool moved = false;

	if (blockOkHere(posx - 1, posy, blocknr, rot))
	{
		--posx;
		updateBlockInField();
		moved = true;
	}

	return moved;
}

bool
BlockOps::moveBlockRight()
{
	bool moved = false;

	if (blockOkHere(posx + 1, posy, blocknr, rot))
	{
		++posx;
		updateBlockInField();
		moved = true;
	}

	return moved;
}

bool
BlockOps::rotateBlock(bool rotateCCW)
{
	bool moved = false;

	int r = rot;

	if ( rotateCCW )
	{
		if (--r < 0) r = 3;
	}
	else
	{
		if (++r >= 4) r = 0;
	}

	if (blockOkHere(posx, posy, blocknr, r))
	{
		putBlockInField(EMPTY);
		rot = r;
		putBlockInField(FALLING);
		moved = true;
	}

	return moved;
}

bool
BlockOps::moveBlockDown()
{
	bool fallen = false;

	if (!blockOkHere(posx, posy + 1, blocknr, rot))
		fallen = true;

	if (!fallen)
	{
		++posy;
		updateBlockInField();
	}

	return fallen;
}

int
BlockOps::dropBlock()
{
	int count = 0;

	while (!moveBlockDown())
		count++;

	return count;
}

void
BlockOps::fallingToLaying()
{
	for (int x = 0; x < COLUMNS; ++x)
		for (int y = 0; y < LINES; ++y)
			if (field[x][y].what == FALLING)
				field[x][y].what = LAYING;
}

void
BlockOps::eliminateLine(int l)
{
	for (int x = 0; x < COLUMNS; ++x)
	{
		if (field[x][l].actor) {
			int cur_x, cur_y = 0;
			g_object_get (G_OBJECT (field[x][l].actor), "x", &cur_x, "y", &cur_y, NULL);
			clutter_actor_raise_top (field[x][l].actor);
//			clutter_effect_move (Block::explode_tmpl, field[x][l].actor,
//					cur_x + g_random_int_range(-60 - cell_width / 4, 60),
//					cur_y + g_random_int_range(-60 - cell_height / 4, 60),
//					NULL, NULL);
//			clutter_effect_scale (Block::explode_tmpl, field[x][l].actor,
//					1.5, 1.5, NULL, NULL);
			destroy_blocks = g_list_append (destroy_blocks,
							&field[x][l]);
		}
	}
}

bool
BlockOps::checkFullLine(int l)
{
	bool f = true;
	for (int x = 0; x < COLUMNS; ++x)
	{
		if (field[x][l].what != LAYING)
		{
			f = false;
			break;
		}
	}

	return f;
}

int
BlockOps::checkFullLines()
{
	// we can have at most 4 full lines (vertical block)
	int numFullLines = 0;
	int numCascades = 0;

	for (int y = MIN (posy + 4, LINES); y > 0; --y)
	{
		if (checkFullLine (y))
		{
			++numFullLines;
			eliminateLine(y);
		}
		else if (numFullLines > 0)
		{
			for (int x = 0; x < COLUMNS; ++x)
			{
				field[x][y+numFullLines].move_from (field[x][y], this);
			}
			++numCascades;
		}
	}

	if (numFullLines > 0)
	{
		clutter_timeline_start (fall_time);
		clutter_timeline_start (explode_time);
		quake_ratio = ((float) numCascades) / (float) LINES;
	}

	return numFullLines;
}

bool
BlockOps::generateFallingBlock()
{
	posx = COLUMNS / 2 + 1;
	posy = 0;

	posx_old = COLUMNS / 2 + 1;
	posy_old = 0;

	blocknr = blocknr_next == -1 ? g_random_int_range(0, tableSize) :
		blocknr_next;
	rot = rot_next == -1 ? g_random_int_range(0, 4) : rot_next;
	int cn = random_block_colors ? g_random_int_range(0, NCOLOURS) :
		blocknr % NCOLOURS;
	color = color_next == -1 ? cn : color_next;

	blocknr_next = g_random_int_range(0, tableSize);
	rot_next = g_random_int_range(0, 4);
	color_next = random_block_colors ? g_random_int_range(0, NCOLOURS) :
		blocknr_next % NCOLOURS;

	if (!blockOkHere(posx, posy, blocknr, rot))
		return false;

	return true;
}

void
BlockOps::emptyField(int filled_lines, int fill_prob)
{
	int blank;

	for (int y = 0; y < LINES; ++y)
	{
		// Allow for at least one blank per line
		blank = g_random_int_range(0, COLUMNS);

		for (int x = 0; x < COLUMNS; ++x)
		{
			field[x][y].what = EMPTY;
			if (field[x][y].actor) {
				clutter_actor_destroy (CLUTTER_ACTOR(field[x][y].actor));
				field[x][y].actor = NULL;
			}

			if ((y>=(LINES - filled_lines)) && (x != blank) &&
			    ((g_random_int_range(0, 10)) < fill_prob)) {
				guint tmpColor = g_random_int_range(0, NCOLOURS);
				field[x][y].what = LAYING;
				field[x][y].color = tmpColor;
				field[x][y].createActor (playingField, renderer->getCacheCellById (tmpColor));
				clutter_actor_set_position (CLUTTER_ACTOR(field[x][y].actor),
							    x*(cell_width), y*(cell_height));
			}
		}
	}
}

void
BlockOps::emptyField(void)
{
	emptyField(0,5);
}

void
BlockOps::putBlockInField (SlotType fill)
{
	for (int x = 0; x < 4; ++x) {
		for (int y = 0; y < 4; ++y) {
			if (blockTable[blocknr][rot][x][y]) {
				int i = posx - 2 + x;
				int j = y + posy;

				field[i][j].what = fill;
				field[i][j].color = color;
				if (fill == FALLING) {
					field[i][j].createActor (playingField,
								 renderer->getCacheCellById (color));
				} else {
					//FIXME jclinton these seem redundant
					if (field[i][j].actor) {
						clutter_actor_destroy (CLUTTER_ACTOR(field[i][j].actor));
						field[i][j].actor = NULL;
					}
				}
			}
		}
	}
}

void
BlockOps::updateBlockInField ()
{
	int shift_x = posx - posx_old;
	int shift_y = posy - posy_old;
	if (shift_x == 0 && shift_y == 0)
		return;

	ClutterActor *blocks_actor[4][4] = {{0, }};

	for (int x = 0; x < 4; ++x) {
		for (int y = 0; y < 4; ++y) {
			if (blockTable[blocknr][rot][x][y]) {
				int i = posx - 2 + x;
				int j = y + posy;

				blocks_actor[x][y] = field[i-shift_x][j-shift_y].actor;
				field[i-shift_x][j-shift_y].what = EMPTY;
				field[i-shift_x][j-shift_y].color = 0;
				field[i-shift_x][j-shift_y].actor = NULL;
			}
		}
	}
	for (int x = 0; x < 4; ++x) {
		for (int y = 0; y < 4; ++y) {
			if (blockTable[blocknr][rot][x][y]) {
				int i = posx - 2 + x;
				int j = y + posy;

				field[i][j].what = FALLING;
				field[i][j].color = color;
				field[i][j].actor = blocks_actor[x][y];
				if (field[i][j].actor) {
					int cur_x, cur_y = 0;
					g_object_get (G_OBJECT (field[i][j].actor), "x", &cur_x, "y", &cur_y, NULL);
					const ClutterKnot knot_line[] = {{cur_x, cur_y}, {field[i][j].x, field[i][j].y}};
//					clutter_behaviour_remove_all (move_path[x][y]);
//					clutter_behaviour_path_clear ((ClutterBehaviourPath *) move_path[x][y]);
//					clutter_behaviour_path_append_knots ((ClutterBehaviourPath *) move_path[x][y], &knot_line[0], &knot_line[1], NULL);
//					clutter_behaviour_apply (move_path[x][y], field[i][j].actor);
				}
			}
		}
	}
	clutter_timeline_start (move_time);
	posx_old = posx;
	posy_old = posy;
}

bool
BlockOps::isFieldEmpty (void)
{
	for (int x = 0; x < COLUMNS; x++) {
		if (field[x][LINES-1].what != EMPTY)
			return false;
	}

	return true;
}

gboolean
BlockOps::resize(GtkWidget *widget, GtkAllocation *allocation, BlockOps *field)
{
	field->width = allocation->width;
	field->height = allocation->height;
	if (field->width == 0 || field->height == 0)
		return FALSE;
	field->cell_width = field->width/COLUMNS;
	field->cell_height = field->height/LINES;
	field->rescaleField ();
	return FALSE;
}

void
BlockOps::rescaleBlockPos ()
{
	for (int y = 0; y < LINES; ++y) {
		for (int x = 0; x < COLUMNS; ++x) {
			if (field[x][y].actor) {
				clutter_actor_set_position (CLUTTER_ACTOR(field[x][y].actor),
							    x*(cell_width), y*(cell_height));
				// FIXME jclinton - is this needed?
				//clutter_clone_texture_set_parent_texture (CLUTTER_CLONE_TEXTURE(field[x][y].actor),
				//					  CLUTTER_TEXTURE(renderer->getCacheCellById (field[x][y].color)));
			}
			field[x][y].x = x*(cell_width);
			field[x][y].y = y*(cell_height);
		}
	}
}

void
BlockOps::rescaleField ()
{
	ClutterActor *stage;
	stage = gtk_clutter_embed_get_stage (GTK_CLUTTER_EMBED (w));

	cairo_t *bg_cr;

	if (renderer)
		renderer->rescaleCache (cell_width, cell_height);
	else {
		renderer = rendererFactory (themeID, cell_width, cell_height);
	}

	if (background) {
//		clutter_actor_set_size (CLUTTER_ACTOR(background), width, height);
//		clutter_cairo_texture_set_size (CLUTTER_CAIRO_TEXTURE(background),
//					      width, height);
	} else {
		background = clutter_cairo_texture_new (width, height);
		/*FIXME jclinton: eventually allow solid color background
		 * for software rendering case */
		ClutterColor stage_color = { 0x61, 0x64, 0x8c, 0xff };
		clutter_stage_set_color (CLUTTER_STAGE (stage),
					 &stage_color);
		clutter_group_add (CLUTTER_GROUP (stage),
				   background);
	}

	rescaleBlockPos ();

	if (foreground) {
		clutter_actor_set_size (CLUTTER_ACTOR(foreground),
					width, height);
//		clutter_cairo_surface_resize (CLUTTER_CAIRO(foreground),
//					      width, height);
	} else {
		foreground = clutter_cairo_texture_new (width, height);
		clutter_group_add (CLUTTER_GROUP (stage),
				   foreground);
	}

	bg_cr = clutter_cairo_texture_create (CLUTTER_CAIRO_TEXTURE(background));
	cairo_set_operator (bg_cr, CAIRO_OPERATOR_CLEAR);
	cairo_paint(bg_cr);
	cairo_set_operator (bg_cr, CAIRO_OPERATOR_OVER);

	if (useBGImage && backgroundImage) {
		gdouble xscale, yscale;
		cairo_matrix_t m;

		/* FIXME: This doesn't handle tiled backgrounds in the obvious way. */
		gdk_cairo_set_source_pixbuf (bg_cr, backgroundImage, 0, 0);
		xscale = 1.0*gdk_pixbuf_get_width (backgroundImage)/width;
		yscale = 1.0*gdk_pixbuf_get_height (backgroundImage)/height;
		cairo_matrix_init_scale (&m, xscale, yscale);
		cairo_pattern_set_matrix (cairo_get_source (bg_cr), &m);
	} else if (backgroundColor)
		gdk_cairo_set_source_color (bg_cr, backgroundColor);
	else
		cairo_set_source_rgb (bg_cr, 0., 0., 0.);

	cairo_paint (bg_cr);
	cairo_destroy (bg_cr);
	drawMessage ();

	clutter_actor_set_position (CLUTTER_ACTOR(background), 0, 0);
	clutter_actor_lower_bottom (CLUTTER_ACTOR(background));
	clutter_actor_set_position (CLUTTER_ACTOR(foreground), 0, 0);
	clutter_actor_raise_top (CLUTTER_ACTOR(foreground));
	center_anchor_x = (width - (cell_width * COLUMNS)) / 2;
	center_anchor_y = (height - (cell_height * LINES)) / 2;
	clutter_actor_set_position (CLUTTER_ACTOR (playingField),
			center_anchor_x, center_anchor_y);
	clutter_actor_raise (CLUTTER_ACTOR (playingField),
			CLUTTER_ACTOR(background));

	clutter_actor_show_all (stage);
}

void
BlockOps::drawMessage()
{
	PangoLayout *dummy_layout;
	PangoLayout *layout;
	PangoFontDescription *desc;
	int lw, lh;
	cairo_t *cr;
	char *msg;

	cr = clutter_cairo_texture_create (CLUTTER_CAIRO_TEXTURE(foreground));
	clutter_actor_raise_top (CLUTTER_ACTOR(foreground));
	cairo_set_operator (cr, CAIRO_OPERATOR_CLEAR);
	cairo_paint(cr);
	cairo_set_operator (cr, CAIRO_OPERATOR_OVER);

	if (showPause)
		msg =  _("Paused");
	else if (showGameOver)
		msg = _("Game Over");
	else {
		cairo_destroy (cr);
		return;
	}

	// Center coordinates
	cairo_translate (cr, width / 2, height / 2);

	desc = pango_font_description_from_string(FONT);

	layout = pango_cairo_create_layout (cr);
	pango_layout_set_text (layout, msg, -1);

	dummy_layout = pango_layout_copy (layout);
	pango_layout_set_font_description (dummy_layout, desc);
	pango_layout_get_size (dummy_layout, &lw, &lh);
	g_object_unref (dummy_layout);

	// desired height : lh = widget width * 0.9 : lw
	pango_font_description_set_absolute_size (desc, ((float) lh / lw) * PANGO_SCALE * width * 0.8);
	pango_layout_set_font_description (layout, desc);
	pango_font_description_free (desc);

	pango_layout_get_size (layout, &lw, &lh);
	cairo_move_to (cr, -((double)lw / PANGO_SCALE) / 2, -((double)lh / PANGO_SCALE) / 2);
	pango_cairo_layout_path (cr, layout);
	cairo_set_source_rgb (cr, 1.0, 1.0, 1.0);
	cairo_fill_preserve (cr);
	cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);
	/* A linewidth of 2 pixels at the default size. */
	cairo_set_line_width (cr, width/220.0);
	cairo_stroke (cr);

	g_object_unref(layout);
	cairo_destroy (cr);
}

void
BlockOps::setBackground(GdkPixbuf *bgImage)//, bool tiled)
{
	backgroundImage = (GdkPixbuf *) g_object_ref(bgImage);
	useBGImage = true;
//	backgroundImageTiled = tiled;
}

void
BlockOps::setBackground(GdkColor *bgColor)
{
	backgroundColor = gdk_color_copy(bgColor);
	if (backgroundImage) {
		g_object_unref (backgroundImage);
		backgroundImage = NULL;
	}
	useBGImage = false;
}

void
BlockOps::showPauseMessage()
{
	showPause = true;

	drawMessage ();
}

void
BlockOps::hidePauseMessage()
{
	showPause = false;

	drawMessage ();
}

void
BlockOps::showGameOverMessage()
{
	showGameOver = true;

	drawMessage ();
}

void
BlockOps::hideGameOverMessage()
{
	showGameOver = false;

	drawMessage ();
}

void
BlockOps::setTheme (gint id)
{
	// don't waste time if theme is the same (like from initOptions)
	if (themeID == id)
		return;

	themeID = id;
	if (renderer) {
		delete renderer;
		renderer = rendererFactory (themeID, cell_width,
					    cell_height);
	} else {
		renderer = rendererFactory (themeID, cell_width,
					    cell_height);
	}
	rescaleBlockPos();
}
