ACLOCAL_AMFLAGS = -I m4

SUBDIRS= \
	po \
	dependencies \
	icons \
	libgames-support \
	tests \
	$(gamelist)

if ENABLE_SOUND
SUBDIRS += sounds
endif

DIST_SUBDIRS = po dependencies icons sounds libgames-support tests $(allgames) $(staginggames)

EXTRA_DIST = \
	autogen.sh \
	COPYING-DOCS \
	xmldocs.make \
	omf.make \
	MAINTAINERS	\
	HACKING		 \
	gnome-doc-utils.make \
	ChangeLog

DISTCLEANFILES = \
	intltool-extract \
	intltool-merge \
	intltool-update \
	gnome-doc-utils.make

# Ignore score files, these *should* be left behind.
distuninstallcheck_listfiles = find . -type f -print | grep -v '.scores' | grep-v scrollkeeper

DISTCHECK_CONFIGURE_FLAGS = \
	--with-platform=gnome \
	--disable-schemas-install \
	--disable-scrollkeeper

# Build ChangeLog from git log
ChangeLog:
	@if test -f $(top_srcdir)/.git/HEAD; then \
		git log --stat --no-color --since=2008-06-21 > $@; \
	fi

dist: ChangeLog

.PHONY: ChangeLog
