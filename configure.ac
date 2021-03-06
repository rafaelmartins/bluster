AC_PREREQ([2.69])

AC_INIT([bluster], [m4_esyscmd(build-aux/git-version-gen .tarball-version)],
  [https://github.com/rafaelmartins/bluster/issues], [bluster],
  [https://github.com/rafaelmartins/bluster])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([1.13 foreign dist-bzip2 dist-xz subdir-objects serial-tests])
AC_CONFIG_HEADERS([config.h])
AM_SILENT_RULES([yes])
AM_MAINTAINER_MODE([enable])

LT_INIT

AC_PROG_CC_C99
AS_IF([test "x$ac_cv_prog_cc_c99" = "xno"], [
  AC_MSG_ERROR([no C99 compiler found, bluster requires a C99 compiler.])
])

AC_SEARCH_LIBS([mkd_string], [markdown], ,
  AC_MSG_ERROR([mkd_string not found. please install the discount markdown parser library.])
)

AC_ARG_WITH([valgrind], AS_HELP_STRING([--without-valgrind],
            [ignore presence of valgrind]))
AS_IF([test "x$with_valgrind" != "xno"], [
  AC_PATH_PROG([valgrind], [valgrind])
  AS_IF([test "x$ac_cv_path_valgrind" = "x"], [
    have_valgrind=no
  ], [
    have_valgrind=yes
  ])
])
AS_IF([test "x$have_valgrind" = "xyes"], , [
  AS_IF([test "x$with_valgrind" = "xyes"], [
    AC_MSG_ERROR([valgrind requested but not found])
  ])
])
AM_CONDITIONAL([USE_VALGRIND], [test "x$have_valgrind" = "xyes"])
VALGRIND="$ac_cv_path_valgrind"
AC_SUBST(VALGRIND)

PKG_PROG_PKG_CONFIG
PKG_CHECK_MODULES([BALDE], [balde > 0.1.2])
PKG_CHECK_MODULES([LIBCURL], [libcurl])
PKG_CHECK_MODULES([JSON_GLIB], [json-glib-1.0 >= 0.16.2])

GLIB_COMPILE_RESOURCES="`$PKG_CONFIG --variable glib_compile_resources gio-2.0`"
AC_SUBST(GLIB_COMPILE_RESOURCES)
BALDE_TEMPLATE_GEN="`$PKG_CONFIG --variable balde_template_gen balde`"
AC_SUBST(BALDE_TEMPLATE_GEN)

AC_CONFIG_FILES([
    Makefile
    tests/utils.c
])
AC_OUTPUT

AS_ECHO("
        ==== ${PACKAGE_STRING} ====

        prefix:       ${prefix}
        exec_prefix:  ${exec_prefix}
        bindir:       ${bindir}

        compiler:     ${CC}
        cflags:       ${CFLAGS}
        ldflags:      ${LDFLAGS}

        valgrind:     ${VALGRIND}
")
