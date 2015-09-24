dnl Process this file with autoconf to produce a configure script.

AC_INIT([hebocr], [0.12.0-testing], [Yaacov Zamir kobi.zamir@gmail.com])
AC_CONFIG_AUX_DIR([config])
AM_INIT_AUTOMAKE([1.9])
AC_CONFIG_MACRO_DIR([m4])

libtoolversion='0:12:0'
AC_SUBST([libtoolversion])

dnl ============================================================================
dnl config hebocr

AC_PROG_CC
AM_PROG_LIBTOOL

pkgincludedir='hebocr'
AC_SUBST([pkgincludedir])

dnl check for pkg-config
AC_PATH_PROG(PKG_CONFIG, pkg-config, no)

dnl ============================================================================
dnl output

AC_CONFIG_FILES([
Makefile
hebocr.pc
src/Makefile
tests/Makefile
tests/images/Makefile
])

AC_OUTPUT

dnl ============================================================================
dnl print status

echo
echo $PACKAGE $VERSION - Hebrew Optical Character Recognition library
echo libtool-version: $libtoolversion
echo OS: $host
echo compiler: $CC
echo prefix: $prefix
echo pkgincludedir: $pkgincludedir
echo pkg-config path: $PKG_CONFIG

