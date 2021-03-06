SUBDIRS = rules

if HAVE_GNOME
SUBDIRS += help
endif

NULL =

cardimagedir = $(pkgdatadir)/pixmaps
cardimage_DATA = \
	baize.png

scmdir  = $(pkgdatadir)/aisleriot/games
scm_DATA = \
	sol.scm

bin_PROGRAMS = sol

sol_SOURCES = \
	board.h		\
	board-noclutter.c \
	conf.c		\
	conf.h		\
	game.c		\
	game.h		\
	sol.c		\
	stats-dialog.c	\
	stats-dialog.h	\
	util.c		\
	util.h		\
	window.c	\
	window.h	\
	$(NULL)

if !HAVE_GUILE_1_8
sol_SOURCES += guile16-compat.h
endif
		
# FIXMEchpe: fix localedir for maemo? $PKG_CONFIG osso-af-settings --variable=localedir

sol_CPPFLAGS = \
	-I$(top_srcdir) \
	$(AM_CPPFLAGS)

sol_CFLAGS = \
	$(GTK_CFLAGS)	      \
	$(GUILE_CFLAGS)	      \
	$(AM_CFLAGS)

sol_LDADD = \
	$(top_builddir)/libgames-support/libgames-support.la \
	$(GTK_LIBS) \
	$(GUILE_LIBS)

if HAVE_HILDON
sol_CFLAGS += $(HILDON_CFLAGS)
sol_LDADD += $(HILDON_LIBS)
endif

if HAVE_GNOME
sol_CFLAGS += $(GNOME_CFLAGS)
sol_LDADD += $(GNOME_LIBS)
endif

if HAVE_RSVG
sol_CFLAGS += $(RSVG_CFLAGS)
sol_LDADD += $(RSVG_LIBS)
endif

if WITH_GTHREAD
sol_CFLAGS += $(GHTREAD_CFLAGS)
sol_LDADD += $(GTHREAD_LIBS)
endif

if HAVE_CLUTTER
if ENABLE_AISLERIOT_CLUTTER

noinst_PROGRAMS = sol-clutter

sol_clutter_SOURCES = \
	board.h		\
	board.c		\
	baize.c		\
	baize.h		\
	card.c          \
	card.h          \
	conf.c		\
	conf.h		\
	game.c		\
	game.h		\
	sol.c		\
	slot-renderer.c	\
	slot-renderer.h \
	stats-dialog.c	\
	stats-dialog.h	\
	util.c		\
	util.h		\
	window.c	\
	window.h	\
	$(NULL)

if !HAVE_GUILE_1_8
sol_clutter_SOURCES += guile16-compat.h
endif
		
sol_clutter_CPPFLAGS = $(sol_CPPFLAGS) -DHAVE_CLUTTER
sol_clutter_CFLAGS = $(sol_CFLAGS) $(CLUTTER_CFLAGS) $(CLUTTER_GTK_CFLAGS)
sol_clutter_LDADD = $(sol_LDADD) $(CLUTTER_LIBS) $(CLUTTER_GTK_LIBS)


endif # ENABLE_AISLERIOT_CLUTTER
endif # HAVE_CLUTTER

desktop_in_files = \
	sol.desktop.in.in

if !HAVE_HILDON
desktop_in_files += \
	freecell.desktop.in.in
endif

if HAVE_MAEMO
desktopdir = $(shell $(PKG_CONFIG) osso-af-settings --variable=desktopentrydir)
else
desktopdir = $(datadir)/applications
endif

desktop_DATA = $(desktop_in_files:.desktop.in.in=.desktop)

schemadir = $(GCONF_SCHEMA_FILE_DIR)
schema_in_files = aisleriot.schemas.in
schema_DATA = $(schema_in_files:.schemas.in=.schemas)

ossobackup_in_files = aisleriot-backup.conf
ossoscript_in_files = aisleriot-clean.sh
ossoservice_in_files = org.gnome.Games.AisleRiot.service.in

if HAVE_MAEMO
ossobackupdir = $(sysconfdir)/osso-backup/applications
ossobackup_DATA = $(ossobackup_in_files)

ossocuddir = $(sysconfdir)/osso-cud-scripts
ossocud_SCRIPTS = $(ossoscript_in_files)

servicedir = $(shell $(PKG_CONFIG) osso-af-settings --variable=dbusservicedir)
service_DATA = $(ossoservice_in_files:.service.in=.service)
endif

%.service: %.service.in Makefile
	sed -e "s|%bindir%|$(bindir)|" $< > $@

install-schemas-local: $(schema_DATA)
if GCONF_SCHEMAS_INSTALL
	if test -z "$(DESTDIR)" ; then \
		for p in $^ ; do \
			GCONF_CONFIG_SOURCE=$(GCONF_SCHEMA_CONFIG_SOURCE) $(GCONFTOOL) --makefile-install-rule $$p 2>&1 > /dev/null; \
		done \
	fi
endif

install-data-local: install-schemas-local

EXTRA_DIST = \
	AUTHORS			\
	README			\
	Rules.HOWTO		\
	TODO			\
	$(scm_DATA)		\
	$(cardimage_DATA)	\
	$(icon_DATA)		\
	$(allicons)		\
	$(ossobackup_in_files)	\
	$(ossoscript_in_files)	\
	$(ossoservice_in_files)	\
	$(schema_in_files)	\
	aisleriot.supp		\
	translatable_game_names.h

CLEANFILES = \
	$(desktop_DATA)	\
	$(schema_DATA)	\
	$(service_DATA)

DISTCLEANFILES = \
	$(desktop_DATA)	\
	$(schema_DATA)	\
	$(service_DATA)

@INTLTOOL_DESKTOP_RULE@
@INTLTOOL_SCHEMAS_RULE@
