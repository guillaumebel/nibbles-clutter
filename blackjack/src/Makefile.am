NULL =

bin_PROGRAMS = blackjack

blackjack_SOURCES = 	\
	blackjack.cpp	\
	blackjack.h	\
	card.cpp	\
	card.h		\
	chips.cpp	\
	chips.h		\
	dialog.cpp	\
	dialog.h	\
	draw.cpp	\
	draw.h		\
	events.cpp	\
	events.h	\
	game.cpp	\
	game.h		\
	hand.cpp	\
	hand.h		\
	menu.cpp	\
	menu.h		\
        player.cpp	\
	player.h	\
	press_data.cpp  \
	press_data.h    \
	slot.cpp	\
	slot.h		\
	splash.cpp	\
	splash.h	\
	strategy.cpp	\
	strategy.h	\
	$(NULL)

blackjack_CPPFLAGS = \
	-I$(top_srcdir) \
	$(AM_CPPFLAGS)

blackjack_CXXFLAGS = \
	$(GTK_CFLAGS) \
	$(XML_CFLAGS) \
	$(AM_CXXFLAGS)

blackjack_LDADD = \
	$(top_builddir)/libgames-support/libgames-support.la \
	$(GTK_LIBS) \
	$(XML_LIBS) \
	-lz
# FIXMEchpe: what's this -lz about??

if HAVE_GNOME
blackjack_CXXFLAGS += $(GNOME_CFLAGS)
blackjack_LDADD += $(GNOME_LIBS)
endif

if HAVE_RSVG
blackjack_CXXFLAGS += $(RSVG_CFLAGS)
blackjack_LDADD += $(RSVG_LIBS)
endif

if WITH_GTHREAD
blackjack_CXXFLAGS += $(GHTREAD_CFLAGS)
blackjack_LDADD += $(GTHREAD_LIBS)
endif
