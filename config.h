/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.in by autoheader.  */

/* AF_UNIX is available */
/* #undef AF_LOCAL */

/* SDL_Mixer support */
/* #undef AUDIO_SDL */

/* Define if card theme installer support is enabled */
/* #undef ENABLE_CARD_THEMES_INSTALLER */

/* Define to enable prerendered card theme format support */
/* #undef ENABLE_CARD_THEME_FORMAT_FIXED */

/* Define to enable KDE card theme format support */
/* #undef ENABLE_CARD_THEME_FORMAT_KDE */

/* Define to enable PySol card theme format support */
/* #undef ENABLE_CARD_THEME_FORMAT_PYSOL */

/* Define to enable pre-2.19 card theme format support */
/* #undef ENABLE_CARD_THEME_FORMAT_SLICED */

/* Define to enable SVG card theme format support */
#define ENABLE_CARD_THEME_FORMAT_SVG 1

/* Enable compilation of included GNU Chess as AI chess engine for glChess */
/* #undef ENABLE_GNUCHESS */

/* always defined to indicate that i18n is enabled */
#define ENABLE_NLS 1

/* Define if sound support is enabled */
#define ENABLE_SOUND 1

/* Support for asynchronous hostname lookups */
/* #undef GAI_A */

/* Path where to install the games */
#define GAMEDIR "/usr/lib/ggz"

/* The default card theme */
#define GAMES_CARD_THEME_DEFAULT "gnomangelo_bitmap.svg"

/* The default card theme format type string */
#define GAMES_CARD_THEME_DEFAULT_FORMAT_STRING "svg"

/* The gettext package name */
#define GETTEXT_PACKAGE "gnome-games"

/* Path where the games should look for their data files */
#define GGZDATADIR "/usr/share/ggz"

/* Game server data directory */
/* #undef GGZDDATADIR */

/* Path where the game registry is located */
#define GGZMODULECONFDIR "/usr/etc"

/* Client support for GGZ */
#define GGZ_CLIENT 1

/* Support for embedded GGZ through libggz-gtk */
#define GGZ_GTK 1

/* Check if local sockets are supported */
#define GGZ_HAVE_PF_LOCAL 1

/* Check if file descriptors can be sent */
#define GGZ_HAVE_SENDMSG 1

/* Check if syslog is present */
#define GGZ_HAVE_SYSLOG_H 1

/* Server support for GGZ */
/* #undef GGZ_SERVER */

/* Define if GNUTLS is to be used */
/* #undef GGZ_TLS_GNUTLS */

/* Define if no TLS is to be used */
#define GGZ_TLS_NONE 1

/* Define if OpenSSL is to be used */
/* #undef GGZ_TLS_OPENSSL */

/* Enable additional debugging at the expense of performance and size */
/* #undef GNOME_ENABLE_DEBUG */

/* Pixmap directo ry */
#define GNOME_ICONDIR "/usr/share/pixmaps"

/* Define to 1 if you have the `alarm' function. */
#define HAVE_ALARM 1

/* Define to 1 if you have the <arpa/inet.h> header file. */
#define HAVE_ARPA_INET_H 1

/* Define to 1 if you have the `bind_textdomain_codeset' function. */
#define HAVE_BIND_TEXTDOMAIN_CODESET 1

/* Define if libcanberra-gtk support is enabled */
#define HAVE_CANBERRA_GTK 1

/* Define to 1 if you have the `close' function. */
#define HAVE_CLOSE 1

/* Define if CMSG_ALIGN is available */
#define HAVE_CMSG_ALIGN 1

/* Define if CMSG_LEN is available */
#define HAVE_CMSG_LEN 1

/* Define if CMSG_SPACE is available */
#define HAVE_CMSG_SPACE 1

/* Define to 1 if you have the `dcgettext' function. */
#define HAVE_DCGETTEXT 1

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if you don't have `vprintf' but do have `_doprnt.' */
/* #undef HAVE_DOPRNT */

/* Define to 1 if you have the `dup2' function. */
#define HAVE_DUP2 1

/* Define to 1 if you have the <fcntl.h> header file. */
#define HAVE_FCNTL_H 1

/* Define to 1 if you have the `fork' function. */
#define HAVE_FORK 1

/* Define to 1 if you have the `getenv' function. */
#define HAVE_GETENV 1

/* Define to 1 if you have the `getpwuid' function. */
#define HAVE_GETPWUID 1

/* Define if the GNU gettext() function is already present or preinstalled. */
#define HAVE_GETTEXT 1

/* Define if GNOME support is enabled */
#define HAVE_GNOME 1

/* Define if guile version is 1.8.0 or above */
/* #undef HAVE_GUILE_1_8 */

/* Define if Hildon support is enabled */
/* #undef HAVE_HILDON */

/* Define to 1 if you have the `hstrerror' function. */
#define HAVE_HSTRERROR 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the `kill' function. */
#define HAVE_KILL 1

/* Define if your <locale.h> file defines LC_MESSAGES. */
#define HAVE_LC_MESSAGES 1

/* Define to 1 if you have the <limits.h> header file. */
#define HAVE_LIMITS_H 1

/* Define to 1 if you have the <locale.h> header file. */
#define HAVE_LOCALE_H 1

/* Define if Maemo support is enabled */
/* #undef HAVE_MAEMO */

/* Define if Maemo 3.x support is enabled */
/* #undef HAVE_MAEMO_3 */

/* Define to 1 if you have the `malloc' function. */
#define HAVE_MALLOC 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define if msghdr has a msg_controllen member */
#define HAVE_MSGHDR_MSG_CONTROL 1

/* Define to 1 if you have the <netdb.h> header file. */
#define HAVE_NETDB_H 1

/* Define to 1 if you have the <netinet/in.h> header file. */
#define HAVE_NETINET_IN_H 1

/* Define if you have OpenSSL < 0.9.6 */
/* #undef HAVE_OLD_SSL_API */

/* Define to 1 if your system has a GNU libc compatible `realloc' function,
   and to 0 otherwise. */
#define HAVE_REALLOC 1

/* Refine if librsvg is available */
#define HAVE_RSVG 1

/* Define if librsvg has gnome-vfs support */
/* #undef HAVE_RSVG_GNOMEVFS */

/* Define if SDL_Mixer is available */
/* #undef HAVE_SDL_MIXER */

/* Define to 1 if you have the `select' function. */
#define HAVE_SELECT 1

/* Define to 1 if you have the `setenv' function. */
#define HAVE_SETENV 1

/* Define to 1 if you have the `socketpair' function. */
#define HAVE_SOCKETPAIR 1

/* If we are going to use OpenSSL */
/* #undef HAVE_SSL */

/* Define to 1 if stdbool.h conforms to C99. */
#define HAVE_STDBOOL_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the `strcasecmp' function. */
#define HAVE_STRCASECMP 1

/* Define to 1 if you have the `strchr' function. */
#define HAVE_STRCHR 1

/* Define to 1 if you have the `strcoll' function and it is properly defined.
   */
#define HAVE_STRCOLL 1

/* Define to 1 if you have the `strerror' function. */
#define HAVE_STRERROR 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define if the SUN_LEN macro exists */
#define HAVE_SUN_LEN 1

/* Define to 1 if you have the <sys/select.h> header file. */
#define HAVE_SYS_SELECT_H 1

/* Define to 1 if you have the <sys/socket.h> header file. */
#define HAVE_SYS_SOCKET_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/time.h> header file. */
#define HAVE_SYS_TIME_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have <sys/wait.h> that is POSIX.1 compatible. */
#define HAVE_SYS_WAIT_H 1

/* Define to 1 if you have the <time.h> header file. */
#define HAVE_TIME_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to 1 if you have the `vfork' function. */
#define HAVE_VFORK 1

/* Define to 1 if you have the <vfork.h> header file. */
/* #undef HAVE_VFORK_H */

/* Define to 1 if you have the `vprintf' function. */
#define HAVE_VPRINTF 1

/* Define to 1 if you have the <winsock2.h> header file. */
/* #undef HAVE_WINSOCK2_H */

/* Define to 1 if `fork' works. */
#define HAVE_WORKING_FORK 1

/* Define to 1 if `vfork' works. */
#define HAVE_WORKING_VFORK 1

/* Define to 1 if the system has the type `_Bool'. */
#define HAVE__BOOL 1

/* The full distribution description */
#define LSB_DISTRIBUTION ""Fedora release 10.93 (Leonidas)""

/* The distributor ID */
#define LSB_DISTRIBUTOR "Fedora"

/* Define to the sub-directory in which libtool stores uninstalled libraries.
   */
#define LT_OBJDIR ".libs/"

/* Define to 1 if your C compiler doesn't accept -c and -o together. */
/* #undef NO_MINUS_C_MINUS_O */

/* Define if threading support is disabled */
/* #undef NO_THREADING */

/* Name of package */
#define PACKAGE "gnome-games"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "http://bugzilla.gnome.org/enter_bug.cgi?product=gnome-games"

/* Define to the full name of this package. */
#define PACKAGE_NAME "GNOME Games"

/* Path where the source is located */
#define PACKAGE_SOURCE_DIR "/home/guillaume/Builds/gnome-games"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "GNOME Games 2.27.2"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "gnome-games"

/* Define to the version of this package. */
#define PACKAGE_VERSION "2.27.2"

/* PF_UNIX is available */
/* #undef PF_LOCAL */

/* Define as the return type of signal handlers (`int' or `void'). */
#define RETSIGTYPE void

/* SDL is used */
/* #undef SDL */

/* Define to the type of arg 1 for `select'. */
#define SELECT_TYPE_ARG1 int

/* Define to the type of args 2, 3 and 4 for `select'. */
#define SELECT_TYPE_ARG234 (fd_set *)

/* Define to the type of arg 5 for `select'. */
#define SELECT_TYPE_ARG5 (struct timeval *)

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Define to 1 if you can safely include both <sys/time.h> and <time.h>. */
#define TIME_WITH_SYS_TIME 1

/* Define if you have the gcrypt library */
#define USE_GCRYPT 1

/* Version number of package */
#define VERSION "2.27.2"

/* Define if smclient is enabled */
/* #undef WITH_SMCLIENT */

/* Define to empty if `const' does not conform to ANSI C. */
/* #undef const */

/* Define to `__inline__' or `__inline' if that's what the C compiler
   calls it, or to nothing if 'inline' is not supported under any name.  */
#ifndef __cplusplus
/* #undef inline */
#endif

/* Define to `int' if <sys/types.h> does not define. */
/* #undef pid_t */

/* Define to rpl_realloc if the replacement function should be used. */
/* #undef realloc */

/* Define to `unsigned int' if <sys/types.h> does not define. */
/* #undef size_t */

/* Define as `fork' if `vfork' does not work. */
/* #undef vfork */

/* Define to empty if the keyword `volatile' does not work. Warning: valid
   code using `volatile' can become incorrect without. Disable with care. */
/* #undef volatile */
