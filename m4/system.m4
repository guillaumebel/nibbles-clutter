dnl ======================================
dnl GGZ Gaming Zone - Configuration Macros
dnl ======================================
dnl
dnl Copyright (C) 2006 Josef Spillner, josef@ggzgamingzone.org
dnl It is published under the conditions of the GNU General Public License.
dnl
dnl ======================================
dnl
dnl This file contains system library dependent macros such as asynchronous
dnl hostname lookups.
dnl As opposed to platform.m4, all optional features go here.
dnl
dnl ======================================
dnl

dnl ------------------------------------------------------------------------
dnl Content of this file:
dnl ------------------------------------------------------------------------
dnl AC_GGZ_ASYNC - find out if libanl is available which makes resolving
dnl                non-blocking
dnl                -> called automatically by AC_GGZ_PTHREADS
dnl AC_GGZ_PTHREADS - look for a pthreads implementation
dnl AC_GGZ_INTL - ensure proper i18n tools installation
dnl               -> this requires iconv.m4!
dnl

dnl ------------------------------------------------------------------------
dnl Check if asynchronous hostname lookups are possible
dnl ------------------------------------------------------------------------
dnl
AC_DEFUN([AC_GGZ_ASYNC],
[
  AC_ARG_ENABLE([anl],
    AC_HELP_STRING([--enable-anl], [Enable asynchronous hostname lookups]),
    [enable_anl=$enableval],
    [enable_anl=no])
  if test $enable_anl != "no"; then
    AC_CHECK_LIB(anl, getaddrinfo_a,
      [AC_DEFINE([GAI_A], 1, [Support for asynchronous hostname lookups])
       enable_anl=yes
       LIB_ASYNC="-lanl"],
      [enable_anl=no])
  fi

  AC_SUBST(LIB_ASYNC)
])

dnl ------------------------------------------------------------------------
dnl Check for pthreads
dnl ------------------------------------------------------------------------
dnl
AC_DEFUN([AC_GGZ_PTHREADS],
[
AC_ARG_ENABLE([threading],
  AS_HELP_STRING([--disable-threading], [Don't build in threading support]),
  [enable_threading=$enableval],
  [enable_threading=yes])

if test $enable_threading != "no"; then
  # Threading support is not available on all platforms.  It's dangerous
  # to disable it so this must be done explicitly.
  AC_CHECK_HEADER(pthread.h, [],
                  AC_MSG_ERROR([*** Cannot find pthread.h header]))
  AC_CHECK_LIB(pthread, pthread_create,
               [LDADD="$LDADD -lpthread"
                CFLAGS="$CFLAGS -D_REENTRANT"],
               [AC_CHECK_LIB(c_r, pthread_create,
                             [LDADD="$LDADD -pthread"
                              CFLAGS="$CFLAGS -D_THREAD_SAFE"],
               AC_MSG_ERROR([*** Cannot find pthread library]))])

  # If threading is available, maybe asynchronous hostname lookups
  # are as well, using libanl from glibc >= 2.3.
  AC_GGZ_ASYNC
else
  AC_DEFINE([NO_THREADING], 1, [Define if threading support is disabled])
fi

# FIXME: use 'LIBS'?
  AC_SUBST(LDADD)
])

dnl ------------------------------------------------------------------------
dnl Find internationalization tools
dnl ------------------------------------------------------------------------
dnl
AC_DEFUN([AC_GGZ_INTL],
[
AC_PATH_PROG(GETTEXT, xgettext)
AC_PATH_PROG(MSGFMT, msgfmt)
AC_PATH_PROG(MSGMERGE, msgmerge)

intl=1
if test "x$GETTEXT" = "x"; then intl=0; fi
if test "x$MSGFMT" = "x"; then intl=0; fi
if test "x$MSGMERGE" = "x"; then intl=0; fi
AM_ICONV
LIBS="$LIBICONV $LIBS"
AC_CHECK_LIB(intl, gettext, [LIBS="-lintl $LIBS"])
AC_CHECK_FUNCS([gettext ngettext], [], [intl=0])
AC_CHECK_HEADERS([libintl.h locale.h])
if test "$intl" = 0; then
  if test "x$2" = "xignore"; then
    AC_MSG_WARN([Internationalization tools missing. (ignored)])
  else
    AC_MSG_RESULT([Internationalization tools missing.])
    if test "x$2" = "x"; then
      AC_MSG_ERROR([Internationalization tools missing.])
    fi

    # Perform actions given by argument 2.
    $2
  fi
else
  AC_MSG_RESULT([Internationalization tools found.])

  XGETTEXT=$GETTEXT
  GMSGFMT=$MSGFMT

  AC_SUBST(XGETTEXT)
  AC_SUBST(GETTEXT)
  AC_SUBST(GMSGFMT)
  AC_SUBST(MSGFMT)
  AC_SUBST(MSGMERGE)

  AC_DEFINE(ENABLE_NLS, 1, [Define if NLS is enabled])

  # Perform actions given by argument 1.
  $1
fi

])

