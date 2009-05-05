#!/bin/sh
# Run this to generate all the initial makefiles, etc.

srcdir=`dirname $0`
test -z "$srcdir" && srcdir=.

PKG_NAME="Gnome Games"

(test -f $srcdir/configure.in \
  && test -d $srcdir/gnomine \
  && test -d $srcdir/same-gnome) || {
    echo -n "**Error**: Directory \"\`$srcdir\'\" does not look like the"
    echo " top-level gnome directory"
    exit 1
}

which gnome-autogen.sh || {
    echo "You need to install gnome-common from the GNOME CVS"
    exit 1
}

REQUIRED_AUTOMAKE_VERSION=1.9 
REQUIRED_GNOME_DOC_UTILS_VERSION=0.10.0 
REQUIRED_GETTEXT_VERSION=0.12
REQUIRED_INTLTOOL_VERSION=0.40.4

. gnome-autogen.sh
