losvgdir = $(datadir)/pixmaps/lightsoff
losvg_DATA = \
	light-off.svg \
	light-on.svg \
	0.svg \
	1.svg \
	2.svg \
	3.svg \
	4.svg \
	5.svg \
	6.svg \
	7.svg \
	8.svg \
	9.svg \
	lcd-back.svg \
	lcd-front.svg \
	lcd-off.svg \
	arrow.svg

lojsdir = $(pkgdatadir)/lightsoff
lojs_DATA = \
	arrow.js \
	board.js \
	light.js \
	main.js \
	score.js

bin_SCRIPTS = \
	lightsoff

desktop_in_files = lightsoff.desktop.in.in
desktopdir = $(datadir)/applications
desktop_DATA = $(desktop_in_files:.desktop.in.in=.desktop)
@INTLTOOL_DESKTOP_RULE@

CLEANFILES = $(lojs_DATA) $(desktop_DATA)

EXTRA_DIST = \
	board.js.in \
	light.js.in \
	main.js.in \
	light-off.svg \
	light-on.svg \
	0.svg \
	1.svg \
	2.svg \
	3.svg \
	4.svg \
	5.svg \
	6.svg \
	7.svg \
	8.svg \
	9.svg \
	lcd-back.svg \
	lcd-front.svg \
	score.js.in \
	lcd-off.svg \
	arrow.svg \
	lightsoff.in \
	arrow.js.in
