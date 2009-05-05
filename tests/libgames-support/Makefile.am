TESTS = test_libgames_support

check_PROGRAMS = test_libgames_support

test_libgames_support_SOURCES = test-libgames-support.c 

test_libgames_support_CPPFLAGS = -I$(top_srcdir)
test_libgames_support_CFLAGS = \
	$(CHECK_CFLAGS) \
	$(GTK_CFLAGS) \
	$(AM_CFLAGS)

test_libgames_support_LDADD = \
	$(top_builddir)/libgames-support/libgames-support.la \
	$(GTK_LIBS) \
	$(INTLLIBS) \
	$(CHECK_LIBS)

if HAVE_GNOME
test_libgames_support_CFLAGS += $(GNOME_CFLAGS)
test_libgames_support_LDADD += $(GNOME_LIBS)
endif

if HAVE_RSVG
test_libgames_support_CFLAGS += $(RSVG_CFLAGS)
test_libgames_support_LDADD += $(RSVG_LIBS)
endif

if WITH_GTHREAD
test_libgames_support_CFLAGS += $(GHTREAD_CFLAGS)
test_libgames_support_LDADD += $(GTHREAD_LIBS)
endif
