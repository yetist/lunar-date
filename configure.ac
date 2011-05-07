AC_PREREQ(2.62)

m4_define([lunar_calendar_major_version], [2])
m4_define([lunar_calendar_minor_version], [4])
m4_define([lunar_calendar_micro_version], [1])
m4_define([lunar_calendar_interface_age], [1])
m4_define([lunar_calendar_binary_age],    [m4_eval(100 * lunar_calendar_minor_version + lunar_calendar_micro_version)])
m4_define([lunar_calendar_version],       [lunar_calendar_major_version.lunar_calendar_minor_version.lunar_calendar_micro_version])
m4_define([lunar_calendar_lt_current],    [m4_eval(100 * lunar_calendar_minor_version + lunar_calendar_micro_version - lunar_calendar_interface_age)])
m4_define([lunar_calendar_lt_revision],   [lunar_calendar_interface_age])
m4_define([lunar_calendar_lt_age],        [m4_eval(lunar_calendar_binary_age - lunar_calendar_interface_age)])

AC_INIT(lunar-calendar, lunar_calendar_version, yetist@gmail.com)
AC_COPYRIGHT([Copyright (C) 2007-2011 yetist@gmail.com])

AM_INIT_AUTOMAKE([1.8 dist-bzip2 no-define -Wno-portability])

AC_CONFIG_SRCDIR(lunar-calendar/lunar-calendar.c)
AM_CONFIG_HEADER(config.h)
AC_CONFIG_MACRO_DIR([m4])

dnl for so version.
LT_CURRENT=lunar_calendar_lt_current
LT_REVISION=lunar_calendar_lt_revision
LT_AGE=lunar_calendar_lt_age
AC_SUBST(LT_CURRENT)
AC_SUBST(LT_REVISION)
AC_SUBST(LT_AGE)

LUNAR_CALENDAR_VERSION=lunar_calendar_version
AC_SUBST(LUNAR_CALENDAR_VERSION)

LIBTOOL_EXPORT_OPTIONS='-export-symbols-regex "^lunar.*"'
AC_SUBST(LIBTOOL_EXPORT_OPTIONS)

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

AC_PATH_PROG(GLIB_MKENUMS, glib-mkenums)

dnl ================================================================
dnl  Language Support 
dnl ================================================================
GETTEXT_PACKAGE=lunar-calendar
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, "$GETTEXT_PACKAGE", [Define the gettext package to be used])
AC_SUBST(GETTEXT_PACKAGE)
AM_GLIB_GNU_GETTEXT

AM_GLIB_DEFINE_LOCALEDIR(LUNAR_CALENDAR_LOCALEDIR)


dnl ================================================================
dnl check glib, gobject and liblunar
dnl ================================================================

LUNAR_DATE_REQUIRED=2.4.0
GTK2_REQUIRED=2.16
GDK2_REQUIRED=2.16

PKG_CHECK_MODULES(LUNAR_CALENDAR, lunar-date-2.0 >= $LUNAR_DATE_REQUIRED gtk+-2.0 >= $GTK2_REQUIRED  gdk-2.0 >= $GDK2_REQUIRED )

AC_SUBST(LUNAR_CALENDAR_CFLAGS)
AC_SUBST(LUNAR_CALENDAR_LIBS)

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


AC_CONFIG_FILES([
lunar-calendar-2.0.pc
lunar-calendar/Makefile
tests/Makefile
po/Makefile.in
docs/Makefile
docs/reference/Makefile
docs/reference/version.xml
Makefile
vapi/Makefile
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
