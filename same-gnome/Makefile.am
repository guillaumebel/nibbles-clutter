SUBDIRS = help

NULL =

themedir = $(pkgdatadir)/same-gnome/themes/2.10
theme_DATA = \
	planets.png	\
	stones.png	\
	$(NULL)

bin_PROGRAMS = same-gnome

same_gnome_SOURCES = 	\
	drawing.c \
	drawing.h \
	game.c \
	game.h \
	input.c \
	input.h \
	same-gnome.c \
	same-gnome.h \
	ui.c \
	ui.h

same_gnome_CPPFLAGS = \
	-I$(top_srcdir) \
	$(AM_CPPFLAGS)

same_gnome_CFLAGS = \
	$(GTK_CFLAGS)	\
	$(AM_CFLAGS)
		
same_gnome_LDADD = \
	$(top_builddir)/libgames-support/libgames-support.la \
	$(GTK_LIBS)	\
	$(INTLLIBS)	\
	$(NULL)

if HAVE_GNOME
same_gnome_CFLAGS += $(GNOME_CFLAGS)
same_gnome_LDADD += $(GNOME_LIBS)
endif

if HAVE_RSVG
same_gnome_CFLAGS += $(RSVG_CFLAGS)
same_gnome_LDADD += $(RSVG_LIBS)
endif

if WITH_GTHREAD
same_gnome_CFLAGS += $(GHTREAD_CFLAGS)
same_gnome_LDADD += $(GTHREAD_LIBS)
endif

schema_in_files = same-gnome.schemas.in
schemadir = $(GCONF_SCHEMA_FILE_DIR)
schema_DATA = $(schema_in_files:.schemas.in=.schemas)

desktop_in_files = same-gnome.desktop.in.in
desktopdir = $(datadir)/applications
desktop_DATA = $(desktop_in_files:.desktop.in.in=.desktop)

EXTRA_DIST = \
	$(theme_DATA)		\
	$(schema_in_files)	\
	$(NULL)

CLEANFILES = $(desktop_DATA) $(schema_DATA)
DISTCLEANFILES = $(desktop_DATA) $(schema_DATA)

SCOREFILES = Small Medium Large

install-schemas-local: $(schema_DATA)
if GCONF_SCHEMAS_INSTALL
	if test -z "$(DESTDIR)" ; then \
		for p in $^ ; do \
			GCONF_CONFIG_SOURCE=$(GCONF_SCHEMA_CONFIG_SOURCE) $(GCONFTOOL) --makefile-install-rule $$p 2>&1 > /dev/null; \
		done \
	fi
endif

install-scorefiles-local:
	-$(mkinstalldirs) $(DESTDIR)$(scoredir)
	## Migrate to the new scoring system (with a link for dual-version
	## installs).
	-if [ -f $(DESTDIR)$(scoredir)/same-gnome.scores -a ! -f $(DESTDIR)$(scoredir)/same-gnome.Small.scores ] ; then \
	  mv -f $(DESTDIR)$(scoredir)/same-gnome.scores $(DESTDIR)$(scoredir)/same-gnome.Small.scores ; \
	  ln -s $(DESTDIR)$(scoredir)/same-gnome.Small.scores $(DESTDIR)$(scoredir)/same-gnome.scores ; \
	fi
	-for i in ${SCOREFILES} ; do \
	  touch $(DESTDIR)$(scoredir)/same-gnome.$$i.scores; \
	  chown $(scores_user):$(scores_group) $(DESTDIR)$(scoredir)/same-gnome.$$i.scores; \
	  chmod 664 $(DESTDIR)$(scoredir)/same-gnome.$$i.scores; \
	done
	-if test "x$(setgid)" = "xtrue"; then chgrp $(scores_group) $(DESTDIR)$(bindir)/same-gnome && chmod 2555 $(DESTDIR)$(bindir)/same-gnome ; fi

install-data-local: install-schemas-local install-scorefiles-local

@INTLTOOL_SCHEMAS_RULE@
@INTLTOOL_DESKTOP_RULE@
