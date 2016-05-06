AC_PREREQ(2.62)

m4_define([lunar_date_major_version], [2])
m4_define([lunar_date_minor_version], [9])
m4_define([lunar_date_micro_version], [1])
m4_define([lunar_date_interface_age], [1])
m4_define([lunar_date_binary_age], [m4_eval(100 * lunar_date_minor_version + lunar_date_micro_version)])
m4_define([lunar_date_version], [lunar_date_major_version.lunar_date_minor_version.lunar_date_micro_version])

m4_define([lunar_date_lt_current],
	            [m4_eval(100 * lunar_date_minor_version + lunar_date_micro_version - lunar_date_interface_age)])
m4_define([lunar_date_lt_revision], [lunar_date_interface_age])
m4_define([lunar_date_lt_age], [m4_eval(lunar_date_binary_age - lunar_date_interface_age)])

AC_INIT(lunar-date, lunar_date_version, yetist@gmail.com)
AC_COPYRIGHT([Copyright (C) 2007-2016 yetist@gmail.com])

AM_INIT_AUTOMAKE([1.8 dist-bzip2 no-define -Wno-portability])
AC_CONFIG_SRCDIR(lunar-date/lunar-date.c)
AM_CONFIG_HEADER(config.h)
AC_CONFIG_MACRO_DIR([m4])

LT_VERSION_INFO="lunar_date_lt_current:lunar_date_lt_revision:lunar_date_lt_age"
LT_CURRENT_MINUS_AGE=m4_eval(lunar_date_lt_current - lunar_date_lt_age)
AC_SUBST(LT_VERSION_INFO)
AC_SUBST(LT_CURRENT_MINUS_AGE)

LUNAR_DATE_VERSION=lunar_date_version
LUNAR_DATE_BINARY_AGE=lunar_date_binary_age
AC_SUBST(LUNAR_DATE_VERSION)
AC_SUBST(LUNAR_BINARY_AGE)

LUNAR_DATE_MAJOR_VERSION=lunar_date_major_version
LUNAR_DATE_MINOR_VERSION=lunar_date_minor_version
LUNAR_DATE_MICRO_VERSION=lunar_date_micro_version

AC_SUBST(LUNAR_DATE_MAJOR_VERSION)
AC_SUBST(LUNAR_DATE_MINOR_VERSION)
AC_SUBST(LUNAR_DATE_MICRO_VERSION)

AC_DEFINE(LUNAR_DATE_MAJOR_VERSION, [lunar_date_major_version],
          [Define to the LUNAR major version])
AC_DEFINE(LUNAR_DATE_MINOR_VERSION, [lunar_date_minor_version],
          [Define to the LUNAR minor version])
AC_DEFINE(LUNAR_DATE_MICRO_VERSION, [lunar_date_micro_version],
          [Define to the LUNAR micro version])
AC_DEFINE(LUNAR_DATE_BINARY_AGE, [lunar_date_binary_age],
	            [Define to the LUNAR binary age])

AM_MAINTAINER_MODE
m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])

AC_PROG_CC
AC_ISC_POSIX
AC_HEADER_STDC

AC_PROG_INSTALL
AC_PROG_LIBTOOL

AM_PATH_PYTHON

IT_PROG_INTLTOOL([0.35.0])

GTK_DOC_CHECK([1.0],[--flavour=no-tmpl])

dnl make sure we keep ACLOCAL_FLAGS around for maintainer builds to work
AC_SUBST(ACLOCAL_AMFLAGS, "$ACLOCAL_FLAGS -I m4")

GOBJECT_INTROSPECTION_CHECK([0.6.7])

AC_PATH_PROG(GLIB_COMPILE_RESOURCES, glib-compile-resources)
AC_PATH_PROG(GLIB_MKENUMS, glib-mkenums)

AC_SUBST(ACLOCAL_AMFLAGS, "$ACLOCAL_FLAGS -I m4")
cur_date=`date -R`
CUR_DATE=${cur_date}
AC_SUBST(CUR_DATE)

dnl ================================================================
dnl check for win32 build
dnl ================================================================
AC_MSG_CHECKING([for some Win32 platform])
case "$host" in
  *-*-mingw*|*-*-cygwin*)
    platform_win32=yes
    os_win32=yes
    ;;
  *)
    platform_win32=no
    os_win32=no
    ;;
esac
AC_MSG_RESULT([$platform_win32])
AM_CONDITIONAL(PLATFORM_WIN32, test "$platform_win32" = "yes")

AM_CONDITIONAL(OS_WIN32, test "$os_win32" = "yes")
AM_CONDITIONAL(OS_UNIX, test "$os_win32" != "yes")

#LIBTOOL_EXPORT_OPTIONS='-export-symbols-regex "^lunar.*"'
if test "$os_win32" != yes; then
    # libtool option to control which symbols are exported
    # right now, symbols starting with _ are not exported
    LIBTOOL_EXPORT_OPTIONS='-export-symbols-regex "^[[^_]].*"'
else
    # We currently use .def files on Windows (for gdk and gtk)
    LIBTOOL_EXPORT_OPTIONS=
fi
AC_SUBST(LIBTOOL_EXPORT_OPTIONS)

if test "$os_win32" = "yes"; then
  AC_CHECK_TOOL(WINDRES, windres, no)
  if test "$WINDRES" = no; then
    AC_MSG_ERROR([*** Could not find an implementation of windres in your PATH.])
  fi
  AC_CHECK_PROG(ms_librarian, lib.exe, yes, no)
fi
AM_CONDITIONAL(MS_LIB_AVAILABLE, test x$ms_librarian = xyes)

dnl ================================================================
dnl check glib and gobject
dnl ================================================================

GIO_REQUIRED=2.32
GIO_UNIX_REQUIRED=2.32
PKG_CHECK_MODULES(GIO, gio-2.0 >= $GIO_REQUIRED)
AC_SUBST(GIO_CFLAGS)
AC_SUBST(GIO_LIBS)

PKG_CHECK_MODULES(GIO_UNIX, gio-unix-2.0 >= $GIO_REQUIRED)
AC_SUBST(GIO_UNIX_CFLAGS)
AC_SUBST(GIO_UNIX_LIBS)

dnl ================================================================
dnl vala bindings support
dnl ================================================================
AC_ARG_ENABLE(vala-bindings,
  AC_HELP_STRING([--enable-vala-bindings],
                 [build the EXPERIMENTAL Vala bindings]),
                  enable_vala_bindings=$enableval,
                  enable_vala_bindings=no )
have_vala=no
if test x$enable_vala_bindings = xyes; then
  if test "x$INTROSPECTION_SCANNER" = x; then
    AC_MSG_ERROR([GObject-Introspection must be enabled for Vala bindings])
  fi

  AM_PROG_VALAC([0.10.0])
  have_vala=yes

  AC_PATH_PROG(VALAC, valac, valac)
  AC_SUBST(VALAC)
  AC_SUBST(VALAFLAGS)

  AC_PATH_PROG([VAPIGEN], [vapigen], [false])

  if test "x$VAPIGEN" = "xfalse"; then
    AC_MSG_ERROR([vapigen not found. Was vala compiled with --enable-vapigen?])
  fi

  AC_SUBST(VAPIGEN)
fi

AM_CONDITIONAL([HAVE_VALA], [test "x$have_vala" = "xyes"])
AC_SUBST(HAVE_VALA)

dnl ================================================================
dnl  Language Support 
dnl ================================================================
GETTEXT_PACKAGE=lunar-date
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, "$GETTEXT_PACKAGE", [Define the gettext package to be used])
AC_SUBST(GETTEXT_PACKAGE)
AM_GLIB_GNU_GETTEXT

AM_GLIB_DEFINE_LOCALEDIR(LUNAR_DATE_LOCALEDIR)

AC_CONFIG_FILES([
lunar-date-3.0.pc
lunar-date/lunar-date-win32.rc
lunar-date/lunar-version.h
lunar-date/Makefile
tests/Makefile
example/Makefile
data/Makefile
po/Makefile.in
docs/Makefile
docs/reference/Makefile
docs/reference/version.xml
Makefile
vapi/Makefile
service/Makefile
])

AC_OUTPUT
AC_MSG_RESULT([
Build options:
  Version                   ${VERSION}
  Install prefix            ${prefix}
  Build shared libs         ${enable_shared}
  Build static libs         ${enable_static}
  CFLAGS                    ${CFLAGS}
  Build introspection       ${found_introspection}
  Build vala binding        ${have_vala}
  Build document            ${enable_gtk_doc}
])
