rulesdir      = $(pkgdatadir)/blackjack/games
rules_DATA    = Ameristar.rules		\
		Atlantic_City.rules	\
		Vegas_Downtown.rules	\
		Vegas_Strip.rules

sysdir = $(datadir)/applications
sys_in_files = blackjack.desktop.in.in
sys_DATA = $(sys_in_files:.desktop.in.in=.desktop)
@INTLTOOL_DESKTOP_RULE@

schemadir   = @GCONF_SCHEMA_FILE_DIR@
schema_in_files = blackjack.schemas.in
schema_DATA = $(schema_in_files:.schemas.in=.schemas)
@INTLTOOL_SCHEMAS_RULE@

EXTRA_DIST    = $(rules_DATA)		\
		$(schema_in_files)

DISTCLEANFILES = $(schema_DATA) $(sys_DATA)

install-data-local:
if GCONF_SCHEMAS_INSTALL
	GCONF_CONFIG_SOURCE=$(GCONF_SCHEMA_CONFIG_SOURCE) $(GCONFTOOL) --makefile-install-rule $(top_builddir)/blackjack/data/$(schema_DATA)
endif
