/* -*- mode:C++; tab-width:8; c-basic-offset:8; indent-tabs-mode:true -*- */
#ifndef __blockops_h__
#define __blockops_h__

/*
 * written by J. Marcin Gorycki <marcin.gorycki@intel.com>
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

#include "tetris.h"
#include "renderer.h"
#include <clutter/clutter.h>

enum SlotType {
	EMPTY,
	FALLING,
	LAYING
};

class Block {
public:
	Block ();
	~Block ();

	Block& move_from (Block &b, BlockOps *f);

	SlotType what;
	guint color;
	ClutterActor *actor;

	int x;
	int y;

	void createActor (ClutterActor *chamber, ClutterActor *texture_source);
	void bindAnimations (BlockOps *f);

	/* Every block will have a unique position
	 * These can be continuously cleared and repopulated with new paths */
	ClutterBehaviour *move_behaviour;
	ClutterBehaviour *fall_behaviour;
	ClutterBehaviour *explode_move_behaviour;


	static void reap_block (Block *block);
};

class BlockOps {
	friend class Block;
public:
	BlockOps ();
	~BlockOps ();

	bool moveBlockLeft ();
	bool moveBlockRight ();
	bool moveBlockDown ();
	bool rotateBlock (bool);
	int dropBlock ();
	void fallingToLaying ();
	int checkFullLines ();
	bool generateFallingBlock ();
	void emptyField (void);
	void emptyField (int filled_lines, int fill_prob);
	void putBlockInField (SlotType fill);
	int getLinesToBottom ();
	bool isFieldEmpty (void);

	void setBackground (GdkPixbuf * bgImage); //, bool tiled); fixme: move tiling here.
	void setBackground (GdkColor * bgColor);
	void placeBlock (int x, int y, int bcolor, bool remove);
	void showPauseMessage ();
	void hidePauseMessage ();
	void showGameOverMessage ();
	void hideGameOverMessage ();
	void setTheme (gint id);
	void drawMessage ();

	GList *destroy_blocks;

	/* These are here because you only need one timeline that can
	 * be shared among all blocks and because 'fade' affects all blocks
	 * equally regardless of their position, so why have more than one since
	 * a behaviour can have many actors? */
	ClutterTimeline *move_time;
	ClutterAlpha *move_alpha;
	ClutterTimeline *fall_time;
	ClutterAlpha *fall_alpha;
	ClutterTimeline *explode_time;
	ClutterAlpha *explode_alpha;
	ClutterBehaviour *explode_fade_behaviour;
	static gboolean move_end (ClutterTimeline *timeline, BlockOps *f);
	static gboolean fall_end (ClutterTimeline *timeline, BlockOps *f);
	static gboolean explode_end (ClutterTimeline *timeline, BlockOps *f);

	/* These are for playing field, not blocks */
	ClutterTimeline *quake_time;
	ClutterAlpha *quake_alpha;
	float quake_ratio;
	ClutterBehaviour *quake_behaviour;

	GtkWidget *getWidget () {
		return w;
	}

private:
	bool blockOkHere (int x, int y, int b, int r);
	void eliminateLine (int l);
	bool checkFullLine(int l);

	GtkWidget * w;

	ClutterActor *background;
	ClutterActor *foreground;
	guint width;
	guint height;
	guint cell_width;
	guint cell_height;
	Renderer *renderer;
	gint themeID;

	Block **field;

	int blocknr;
	int rot;
	int color;

	bool showPause;
	bool showGameOver;

	GdkPixbuf *backgroundImage;
	bool backgroundImageTiled;
	bool useBGImage;
	GdkColor *backgroundColor;

	void rescaleField ();
	void rescaleBlockPos ();
	void updateBlockInField ();

	int posx;
	int posy;
	int posx_old;
	int posy_old;

	ClutterActor *playingField;

	int center_anchor_x;
	int center_anchor_y;

	static gboolean resize (GtkWidget *widget, GtkAllocation *event,
				BlockOps *field);
};

#endif //__blockops_h__
