#!/usr/bin/env seed

// Configuration
var tiles = 5;
var tile_size = 75;

imports.gi.versions.Clutter = "0.8";
imports.gi.versions.GtkClutter = "0.8";

Gtk = imports.gi.Gtk;
Clutter = imports.gi.Clutter;
GtkClutter = imports.gi.GtkClutter;
GdkPixbuf = imports.gi.GdkPixbuf;
GConf = imports.gi.GConf;
GLib = imports.gi.GLib;
Pango = imports.gi.Pango;
GObject = imports.gi.GObject;

Clutter.init(null, null);
GConf.init(null, null);

var margin = 5;
var in_setup = false;
var board_size = (tile_size + margin) * tiles + margin;

Seed.include('/usr'+"/share/gnome-games/lightsoff/score.js");
Seed.include('/usr'+"/share/gnome-games/lightsoff/light.js");
Seed.include('/usr'+"/share/gnome-games/lightsoff/board.js");
Seed.include('/usr'+"/share/gnome-games/lightsoff/arrow.js");

var black = new Clutter.Color();
Clutter.color_parse("Black", black);

var stage = new Clutter.Stage();
// Needs to be translatable. I haven't figured this out yet.
stage.title = "Lights Off";
stage.signal.hide.connect(function () { Clutter.main_quit(); });
stage.color = black;

score = new Score();
board = new Board();
rect = new Clutter.Rectangle({color:black});
forward = new Arrow();
back = new Arrow();

score.set_position((board_size / 2) - (score.width / 2), board_size + margin);
stage.set_size(board_size, board_size + score.height + margin * 3);

rect.set_position(0, board_size);
rect.set_size(stage.width, stage.height);

back.set_position(score.x - back.width - 2*margin,
				  score.y + (.5 * score.height) - (.5 * back.height));

forward.flip();
forward.set_position(score.x + score.width + 2*margin,
					 score.y + (.5 * score.height) - (.5 * forward.height));

stage.add_actor(board);
stage.add_actor(rect);
stage.add_actor(score);
stage.add_actor(forward);
stage.add_actor(back);
stage.show_all();

Clutter.main();

