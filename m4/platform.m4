dnl ======================================
dnl GGZ Gaming Zone - Configuration Macros
dnl ======================================
dnl
dnl Copyright (C) 2005, 2006 Josef Spillner, josef@ggzgamingzone.org
dnl This file has been derived from libggz configuration macros.
dnl It is published under the conditions of the GNU General Public License.
dnl
dnl ======================================
dnl
dnl This file contains operating system specific checks such as for system
dnl libraries, header file flavours and non-POSIX/Unix handling.
dnl Macros defined herein are allowed to modify LDADD and LDFLAGS.
dnl Optional (advanced) checks go to system.m4 and should use LIB_* variables.
dnl
dnl ======================================
dnl
dnl History:
dnl 2005-09-14: created from libggz's configure.ac file

dnl ------------------------------------------------------------------------
dnl Content of this file:
dnl ------------------------------------------------------------------------
dnl AC_GGZ_PLATFORM_BASE - minimal required call
dnl AC_GGZ_PLATFORM_POSIX - checks for various types and functions
dnl AC_GGZ_PLATFORM_POSIX_NET - network-related checks
dnl AC_GGZ_PLATFORM_POSIX_LIBC - system libraries in addition to libc
dnl AC_GGZ_PLATFORM_WIN32 - for cross-compilation on win32 systems
dnl AC_GGZ_PLATFORM - convenience macro to call all of the above
dnl   (except for _BASE which MUST be called before AC_PROG_LIBTOOL)
dnl

dnl ------------------------------------------------------------------------
dnl AC_GGZ_PLATFORM_BASE
dnl ------------------------------------------------------------------------
dnl
AC_DEFUN([AC_GGZ_PLATFORM_BASE],
[
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_CPP
AC_LIBTOOL_WIN32_DLL
#AC_PROG_LIBTOOL # bug!
AC_PROG_RANLIB
AC_PROG_MAKE_SET
])

dnl ------------------------------------------------------------------------
dnl AC_GGZ_PLATFORM_WIN32
dnl ------------------------------------------------------------------------
dnl
AC_DEFUN([AC_GGZ_PLATFORM_WIN32],
[
  case $host_os in
    *mingw32* )
      MINGW32=yes
      ;;
    * )
      MINGW32=no
      ;;
  esac

  if test "$MINGW32" = "yes"; then
    LDFLAGS="$LDFLAGS -no-undefined"
    LDADD="$LDADD -lws2_32"
  fi

  AC_CHECK_HEADERS([winsock2.h])
])

dnl ------------------------------------------------------------------------
dnl AC_GGZ_PLATFORM_POSIX
dnl ------------------------------------------------------------------------
dnl
AC_DEFUN([AC_GGZ_PLATFORM_POSIX],
[
# Check for header files
# ======================
#AC_CHECK_HEADERS([fcntl.h sys/param.h],
#  [],
#  [AC_MSG_ERROR([cannot find required header file])])
#AC_CHECK_HEADERS([netdb.h sys/shm.h sys/socket.h])
#AC_CHECK_HEADERS([sys/uio.h sys/un.h sys/wait.h])
AC_CHECK_HEADERS([arpa/inet.h sys/types.h netinet/in.h])

# Check for functions
# ===================
#AC_CHECK_FUNCS([getpagesize memmove memset mkdir stat strcasecmp strerror strstr strchr],
#  [],
#  [AC_MSG_ERROR([cannot find required function])])
#AC_CHECK_FUNCS([gethostbyname socket]) # These fail; dunno why
#AC_CHECK_FUNCS([alarm getuid inet_pton])

# Check for more functions
# ========================
#AC_FUNC_FORK
##AC_FUNC_MALLOC # use ggz_malloc() always
#AC_FUNC_MEMCMP
#AC_FUNC_REALLOC
#AC_FUNC_STAT
#FUNC_MKDIR_TAKES_ONE_ARG # FIXME: external

# Check for typedefs, structures, and compiler characteristics
# ============================================================
#AC_C_CONST
#AC_TYPE_MODE_T
#AC_TYPE_PID_T
#AC_TYPE_SIGNAL
#AC_TYPE_SIZE_T
AC_C99_VARIABLE_ARRAYS # FIXME: external

# Check for debug types in syslog.h
# =================================
# If syslog is present, we take the log types from it.
# If not, define our own enumeration.
AC_CHECK_HEADER([syslog.h],
  [
    AC_DEFINE([GGZ_HAVE_SYSLOG_H], 1, [Check if syslog is present])
  ],
  [])
])

dnl ------------------------------------------------------------------------
dnl AC_GGZ_PLATFORM_POSIX_NET
dnl ------------------------------------------------------------------------
dnl
AC_DEFUN([AC_GGZ_PLATFORM_POSIX_NET],
[
# Check for sendmsg
# =================
# If sendmsg() is present, we can send file descriptors over a local socket
# via ggz_write_fd.  If not we disable this functionality.
#AC_MSG_CHECKING([for network feature: sendmsg])
AC_CHECK_FUNC([sendmsg],
  [
    AC_DEFINE([GGZ_HAVE_SENDMSG], 1, [Check if file descriptors can be sent])
  ],
  [])

# Check for PF_LOCAL/PF_UNIX
# ==========================
# There should be an autoconf macro to check this???
AC_MSG_CHECKING([for network feature: PF_LOCAL])
have_localsockets=0
AC_COMPILE_IFELSE(
  AC_LANG_PROGRAM([
    [#include <sys/types.h>
     #include <sys/socket.h>]
  ], [[int a = PF_LOCAL;]]),
  [have_localsockets=1],
  [
    AC_COMPILE_IFELSE(
      AC_LANG_PROGRAM([
        [#include <sys/types.h>
         #include <sys/socket.h>]
      ], [[int a = PF_UNIX;]]),
      [
        AC_DEFINE([PF_LOCAL], PF_UNIX, [PF_LOCAL is available via PF_UNIX])
        AC_DEFINE([AF_LOCAL], AF_UNIX, [AF_LOCAL is available via AF_UNIX])
        have_localsockets=1
      ],
      [])
  ])
if test "x$have_localsockets" = "x1"; then
  AC_DEFINE([GGZ_HAVE_PF_LOCAL], 1, [Check if local sockets are supported])
  AC_MSG_RESULT(yes)
else
  AC_MSG_RESULT(no)
fi

# Check for SUN_LEN
# =================
AC_MSG_CHECKING([for network feature: SUN_LEN])
AC_LINK_IFELSE(
  AC_LANG_PROGRAM(
    [[
      #include<sys/types.h>
      #include<sys/un.h>
    ]],
    [[struct sockaddr_un su; int i = SUN_LEN(&su);]]),
  [
    AC_MSG_RESULT(yes)
    AC_DEFINE([HAVE_SUN_LEN], 1, [Define if the SUN_LEN macro exists])
  ],
  [AC_MSG_RESULT(no)])

# Check for msghdr member msg_controllen and friends
# ==================================================
AC_MSG_CHECKING([for network feature: msg_controllen])
have_controllen=0
AC_COMPILE_IFELSE(
  AC_LANG_PROGRAM([
    [#include <sys/types.h>
     #include <sys/socket.h>]
   ],
    [[struct msghdr m; m.msg_controllen = 0;]]),
  [have_controllen=1],
  [])

if test "x$have_controllen" = "x1"; then
  AC_DEFINE([HAVE_MSGHDR_MSG_CONTROL], 1, [Define if msghdr has a msg_controllen member])
  AC_MSG_RESULT(yes)

  AC_MSG_CHECKING([for network feature: CMSG_ALIGN])
  AC_LINK_IFELSE(
    AC_LANG_PROGRAM(
      [[
        #include<sys/types.h>
        #include<sys/socket.h>
      ]],
      [[int i = CMSG_ALIGN(0);]]),
    [
      AC_MSG_RESULT(yes)
      AC_DEFINE([HAVE_CMSG_ALIGN], 1, [Define if CMSG_ALIGN is available])
    ],
    [AC_MSG_RESULT(no)])

  AC_MSG_CHECKING([for network feature: CMSG_LEN])
  AC_LINK_IFELSE(
    AC_LANG_PROGRAM(
      [[
        #include<sys/types.h>
        #include<sys/socket.h>
      ]],
      [[int i = CMSG_LEN(0);]]),
    [
      AC_MSG_RESULT(yes)
      AC_DEFINE([HAVE_CMSG_LEN], 1, [Define if CMSG_LEN is available])
    ],
    [AC_MSG_RESULT(no)])

  AC_MSG_CHECKING([for network feature: CMSG_SPACE])
  AC_LINK_IFELSE(
    AC_LANG_PROGRAM(
      [[
        #include<sys/types.h>
        #include<sys/socket.h>
      ]],
      [[int i = CMSG_SPACE(0);]]),
    [
      AC_MSG_RESULT(yes)
      AC_DEFINE([HAVE_CMSG_SPACE], 1, [Define if CMSG_SPACE is available])
    ],
    [AC_MSG_RESULT(no)])
else
  AC_MSG_RESULT(no)
fi
])

dnl ------------------------------------------------------------------------
dnl AC_GGZ_PLATFORM_POSIX_LIBC
dnl ------------------------------------------------------------------------
dnl
AC_DEFUN([AC_GGZ_PLATFORM_POSIX_LIBC],
[
# Check for library functions
# ===========================
# On some systems additional libraries may be needed.  Simply include them
# if they're there.
AC_CHECK_LIB(socket, socket, [LDADD="$LDADD -lsocket"])
AC_CHECK_LIB(nsl, gethostbyname, [LDADD="$LDADD -lnsl"])
])

dnl ------------------------------------------------------------------------
dnl AC_GGZ_PLATFORM
dnl ------------------------------------------------------------------------
dnl
AC_DEFUN([AC_GGZ_PLATFORM],
[
#AC_GGZ_PLATFORM_BASE
AC_GGZ_PLATFORM_POSIX
AC_GGZ_PLATFORM_POSIX_NET
AC_GGZ_PLATFORM_POSIX_LIBC
AC_GGZ_PLATFORM_WIN32
])

