dnl ======================================
dnl GGZ Gaming Zone - Configuration Macros
dnl ======================================
dnl
dnl Copyright (C) 2001 - 2006 Josef Spillner, josef@ggzgamingzone.org
dnl This file has heavily been inspired by KDE's acinclude :)
dnl It is published under the conditions of the GNU General Public License.
dnl
dnl ======================================
dnl
dnl This file contains all autoconf macros needed for any security
dnl implementation such as TLS/SSL.
dnl
dnl ======================================
dnl
dnl History:
dnl 2002-02-10: lookup TLS libraries; taken code from acinclude.ggz
dnl 2002-02-24: default to GnuTLS; create conditional compile vars
dnl 2005-09-14: several cleanups due to newer autotools
dnl 2006-08-06: now includes gcrypt checks

dnl ------------------------------------------------------------------------
dnl Content of this file:
dnl ------------------------------------------------------------------------
dnl AC_GGZ_TLS - find a TLS implementation (support for gnutls, openssl and
dnl              none)
dnl AC_GGZ_GCRYPT - find the gcrypt encryption library
dnl
dnl Private macros:
dnl AC_PATH_SSL - OpenSSL implementation backend (code from kdelibs)
dnl AC_GGZ_GNUTLS - GNUTLS implementation backend
dnl

dnl ------------------------------------------------------------------------
dnl Find a single file
dnl ------------------------------------------------------------------------
dnl
AC_DEFUN([AC_GGZ_FIND_FILE],
[
$3=NO
for i in $2;
do
  for j in $1;
  do
    echo "configure: __oline__: $i/$j" >&AC_FD_CC
    if test -r "$i/$j"; then
      echo "taking that" >&AC_FD_CC
      $3=$i
      break 2
    fi
  done
done
])

dnl ------------------------------------------------------------------------
dnl Try to find the SSL headers and libraries.
dnl Exported are $(ssl_includes), $(ssl_libraries) and $(ssl_lib).
dnl ------------------------------------------------------------------------
dnl
AC_DEFUN([AC_PATH_SSL],
[
LIBSSL="-lssl -lcrypto"

ac_ssl_includes=NO ac_ssl_libraries=NO
ssl_libraries=""
ssl_includes=""
AC_ARG_WITH([ssl-dir],
  AC_HELP_STRING([--with-ssl-dir=DIR], [where the root of OpenSSL is installed]),
  [
    ac_ssl_includes="$withval"/include
    ac_ssl_libraries="$withval"/lib
  ],
  [])

AC_CACHE_VAL(ac_cv_have_ssl,
[#try to guess OpenSSL locations
  
  ssl_incdirs="/usr/include /usr/local/include /usr/ssl/include /usr/local/ssl/include $prefix/include $kde_extra_includes"
  ssl_incdirs="$ac_ssl_includes $ssl_incdirs"
  AC_GGZ_FIND_FILE(openssl/ssl.h, $ssl_incdirs, ssl_incdir)
  ac_ssl_includes="$ssl_incdir"

  ssl_libdirs="/usr/lib /usr/local/lib /usr/ssl/lib /usr/local/ssl/lib $prefix/lib $exec_prefix/lib $kde_extra_libs"
  if test ! "$ac_ssl_libraries" = "NO"; then
    ssl_libdirs="$ac_ssl_libraries $ssl_libdirs"
  fi

  test=NONE
  ssl_libdir=NONE
  for dir in $ssl_libdirs; do
    try="ls -1 $dir/libssl*"
    if test=`eval $try 2> /dev/null`; then ssl_libdir=$dir; break; else echo "tried $dir" >&AC_FD_CC ; fi
  done

  ac_ssl_libraries="$ssl_libdir"

  AC_LANG_SAVE
  AC_LANG_C

  ac_cflags_safe="$CFLAGS"
  ac_ldflags_safe="$LDFLAGS"
  ac_libs_safe="$LIBS"

  CFLAGS="$CFLAGS -I$ssl_incdir $all_includes"
  LDFLAGS="-L$ssl_libdir $all_libraries"
  LIBS="$LIBS $LIBSSL -lRSAglue -lrsaref"

  AC_TRY_LINK(,void RSAPrivateEncrypt(void);RSAPrivateEncrypt();,
  ac_ssl_rsaref="yes"
  ,
  ac_ssl_rsaref="no"
  )

  CFLAGS="$ac_cflags_safe"
  LDFLAGS="$ac_ldflags_safe"
  LIBS="$ac_libs_safe"

  AC_LANG_RESTORE

  if test "$ac_ssl_includes" = NO || test "$ac_ssl_libraries" = NO; then
    have_ssl=no
  else
    have_ssl=yes;
  fi

])

eval "$ac_cv_have_ssl"

if test "$have_ssl" = yes; then
  dnl Check for SSL version
  AC_CACHE_VAL(ac_cv_ssl_version,
  [
    AC_LANG_SAVE
    AC_LANG_C 

    cat >conftest.$ac_ext <<EOF
#include <openssl/opensslv.h>
#include <stdio.h>
    int main() {
 
#ifndef OPENSSL_VERSION_NUMBER
      printf("ssl_version=\\"error\\"\n");
#else
      if (OPENSSL_VERSION_NUMBER < 0x00906000)
        printf("ssl_version=\\"old\\"\n");
      else
        printf("ssl_version=\\"ok\\"\n");
#endif
     return (0);
    }
EOF

    ac_compile='${CC-gcc} $CFLAGS -I$ac_ssl_includes conftest.$ac_ext -o conftest'
    if AC_TRY_EVAL(ac_compile); then 

      if eval `./conftest 2>&5`; then
        if test $ssl_version = error; then
		  have_ssl=no
        else
          if test $ssl_version = old; then
            have_ssl=no
          fi
        fi
        ac_cv_ssl_version="ssl_version=$ssl_version"
      else
		have_ssl=no
      fi

    else
	  have_ssl=no
    fi 

    AC_LANG_RESTORE

  ])

  eval "$ac_cv_ssl_version"
fi

if test "$have_ssl" != yes; then
  LIBSSL="";
else
  AC_DEFINE(HAVE_SSL, 1, [If we are going to use OpenSSL])
  ac_cv_have_ssl="have_ssl=yes \
    ac_ssl_includes=$ac_ssl_includes ac_ssl_libraries=$ac_ssl_libraries ac_ssl_rsaref=$ac_ssl_rsaref"
  
  
  ssl_libraries="$ac_ssl_libraries"
  ssl_includes="$ac_ssl_includes"

  if test "$ac_ssl_rsaref" = yes; then
    LIBSSL="-lssl -lcrypto -lRSAglue -lrsaref" 
  fi

  if test $ssl_version = "old"; then
    AC_DEFINE(HAVE_OLD_SSL_API, 1, [Define if you have OpenSSL < 0.9.6])
  fi
fi

# got ssl_includes
# got ssl_libraries
ssl_lib=$LIBSSL

])

dnl ------------------------------------------------------------------------
dnl Try to find the GNUTLS headers and libraries.
dnl Exported are $(gnutls_includes), $(gnutls_libraries) and $(gnutls_lib).
dnl ------------------------------------------------------------------------
dnl
AC_DEFUN([AC_GGZ_GNUTLS],
[
ac_gnutls_includes=NO ac_gnutls_libraries=NO
gnutls_libraries=""
gnutls_includes=""
gnutls_lib=""

AC_ARG_WITH([gnutls-dir],
  AC_HELP_STRING([--with-gnutls-dir=DIR], [gnutls installation prefix]),
  [
    ac_gnutls_includes="$withval"/include/gnutls
    ac_gnutls_libraries="$withval"/lib
  ],
  [])
AC_ARG_WITH([gnutls-includes],
    AC_HELP_STRING([--with-gnutls-includes=DIR], [where the gnutls includes are.]),
    [
      ac_gnutls_includes="$withval"
    ],
    [])
AC_ARG_WITH([gnutls-libraries],
    AC_HELP_STRING([--with-gnutls-libraries=DIR], [where the gnutls libs are.]),
    [
      ac_gnutls_libraries="$withval"
    ],
    [])

AC_CACHE_VAL(ac_cv_have_gnutls,
[
if test "x${prefix}" = "xNONE"; then
   prefix_incdir="${ac_default_prefix}/include"
   prefix_libdir="${ac_default_prefix}/lib"
else
   prefix_incdir="${prefix}/include"
   prefix_libdir="${prefix}/lib"
fi
gnutls_incdirs="$ac_gnutls_includes $prefix_incdir/gnutls /usr/local/include/gnutls /usr/include/gnutls"
gnutls_header=gnutls.h

AC_GGZ_FIND_FILE($gnutls_header, $gnutls_incdirs, gnutls_incdir)
ac_gnutls_includes="$gnutls_incdir"

gnutls_libdirs="$ac_gnutls_libraries $prefix_libdir /usr/local/lib /usr/lib"

test=NONE
gnutls_libdir=NONE
for dir in $gnutls_libdirs; do
  try="ls -1 $dir/libgnutls.*"
  if test -n "`$try 2> /dev/null`"; then gnutls_libdir=$dir; break; else echo "tried $dir" >&AC_FD_CC ; fi
done

ac_gnutls_libraries="$gnutls_libdir"

if test "$ac_gnutls_includes" = NO || test "$ac_gnutls_libraries" = NO; then
  ac_cv_have_gnutls="have_gnutls=no"
else
  have_gnutls="yes"
fi
])

eval "$ac_cv_have_gnutls"

if test "$have_gnutls" != yes; then
  have_gnutls=no
else
  ac_cv_have_gnutls="have_gnutls=yes \
    ac_gnutls_includes=$ac_gnutls_includes ac_gnutls_libraries=$ac_gnutls_libraries"

  gnutls_libraries="$ac_gnutls_libraries"
  gnutls_includes="$ac_gnutls_includes"
  gnutls_lib="-lgnutls"
fi

])

dnl ------------------------------------------------------------------------
dnl Try to find a suitable TLS implementation.
dnl Defines GGZ_TLS_NONE or GGZ_TLS_GNUTLS or GGZ_TLS_OPENSSL
dnl $(GGZTLS_LDFLAGS) will be -L ... (if needed)
dnl and $(GGZTLS_INCLUDES) will be -I ... (if needed)
dnl $(LIB_GGZTLS) will be the backend library to use against
dnl ------------------------------------------------------------------------
dnl
AC_DEFUN([AC_GGZ_TLS],
[
AC_MSG_CHECKING([for GGZ TLS implementation])

AC_ARG_WITH([tls],
  AC_HELP_STRING([--with-tls@<:@=ARG@:>@], [GnuTLS or OpenSSL - auto if no ARG]),
  [tls_type=$withval],
  [tls_type=no])

dnl None (defaults)
GGZTLS_INCLUDES=""
GGZTLS_LDFLAGS=""
LIB_GGZTLS=""
TLS_TYPE="no"

if test "$tls_type" = yes -o "$tls_type" = GnuTLS; then
  dnl GNUTLS check
  AC_GGZ_GNUTLS
  if test "$have_gnutls" = yes; then
    GGZTLS_INCLUDES="-I $gnutls_includes"
    GGZTLS_LDFLAGS="-L $gnutls_libraries"
    LIB_GGZTLS=$gnutls_lib
    AC_MSG_RESULT([using GnuTLS])
    AC_DEFINE_UNQUOTED([GGZ_TLS_GNUTLS], 1,
		       [Define if GNUTLS is to be used])
    TLS_TYPE="GnuTLS"
  fi
fi

if test \( "$tls_type" = yes -a "$have_gnutls" = no \) -o "$tls_type" = OpenSSL
then
  dnl OpenSSL check
  AC_PATH_SSL
  if test "$have_ssl" = yes; then
    GGZTLS_INCLUDES="-I $ssl_includes"
    GGZTLS_LDFLAGS="-L $ssl_libraries"
    LIB_GGZTLS=$ssl_lib
    AC_MSG_RESULT([using OpenSSL])
    AC_DEFINE_UNQUOTED([GGZ_TLS_OPENSSL], 1,
		      [Define if OpenSSL is to be used])
    TLS_TYPE="OpenSSL"
  fi
fi

if test "$TLS_TYPE" = no; then
  if test "$tls_type" = no; then
    AC_MSG_RESULT([no])
  else
    AC_MSG_WARN([No TLS implementation found - using 'no'!])
  fi
  AC_DEFINE_UNQUOTED([GGZ_TLS_NONE], 1,
		     [Define if no TLS is to be used])
fi


AC_SUBST(GGZTLS_INCLUDES)
AC_SUBST(GGZTLS_LDFLAGS)
AC_SUBST(LIB_GGZTLS)

])

dnl ------------------------------------------------------------------------
dnl Searches for the gcrypt library
dnl Defines USE_GCRYPT if found
dnl $(LIB_GCRYPT) will be set accordingly
dnl ------------------------------------------------------------------------
dnl
AC_DEFUN([AC_GGZ_GCRYPT],
[
AC_ARG_WITH([gcrypt],
  AS_HELP_STRING([--with-gcrypt], [Add encryption support]),
  [enable_gcrypt=$enableval],
  [enable_gcrypt=auto])

if test "$enable_gcrypt" != "no"; then
  AC_CHECK_LIB(gcrypt, gcry_check_version,
    [
      AC_CHECK_HEADER(gcrypt.h,
        [
          AC_DEFINE_UNQUOTED([USE_GCRYPT], 1,
                             [Define if you have the gcrypt library])
          LIB_GCRYPT="-lgcrypt"
        ],
        [
          if test "$enable_gcrypt" = "yes"; then
            AC_MSG_ERROR([*** No gcrypt-dev found])
          else
            AC_MSG_WARN([*** No gcrypt-dev found - compiling without encryption support])
            enable_gcrypt=no
          fi
        ])
    ],
    [
      if test "$enable_gcrypt" = "yes"; then
        AC_MSG_ERROR([*** No gcrypt found])
      else
        AC_MSG_WARN([*** No gcrypt found - compiling without encryption support])
        enable_gcrypt=no
      fi
    ])
fi

AC_SUBST(LIB_GCRYPT)
])

