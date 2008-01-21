AC_PREREQ(2.59)


m4_define([libgcl_major_version], [0])
m4_define([libgcl_minor_version], [2])
m4_define([libgcl_micro_version], [0])
m4_define([libgcl_real_version], [libgcl_major_version.libgcl_minor_version.libgcl_micro_version])
m4_define([libgcl_version], [0.2.0-rc1])


AC_INIT(libgcl, libgcl_version, yetist@gmail.com)
AC_COPYRIGHT([Copyright (C) 2007 yetist@gmail.com])

AM_INIT_AUTOMAKE(1.8 dist-bzip2 no-define)
AC_CONFIG_SRCDIR(gcl/gcl-date.h)
AM_CONFIG_HEADER(config.h)

AM_MAINTAINER_MODE

LIBGCL_VERSION=libgcl_real_version
AC_SUBST(LIBGCL_VERSION)

AC_PROG_CC
AC_ISC_POSIX
AC_HEADER_STDC

AC_PROG_INSTALL
AC_PROG_LIBTOOL

IT_PROG_INTLTOOL([0.35.0])

GTK_DOC_CHECK(1.0)

GLIB2_REQUIRED=2.4.0
GTK_REQUIRED=2.4.0

AC_SUBST(GLIB2_REQUIRED)

PKG_CHECK_MODULES(LIBGCL, glib-2.0 >= $GLIB2_REQUIRED gtk+-2.0 >= $GTK_REQUIRED)
AC_SUBST(LIBGCL_CFLAGS)
AC_SUBST(LIBGCL_LIBS)


dnl run in source tree
AC_ARG_ENABLE(run-in-source-tree,
	      AC_HELP_STRING([--disable-run-in-source-tree],[disable search for holiday.dat file in local directory ]),
	[case "${enableval}" in
	yes) ENABLE_RUN_IN_SOURCE_TREE=yes ;;
	no)  ENABLE_RUN_IN_SOURCE_TREE=no ;;
	*) AC_MSG_ERROR(bad value ${enableval} for --disable-run-in-source-tree) ;;
	esac],
[ENABLE_RUN_IN_SOURCE_TREE=yes]) dnl Default value
if test x$ENABLE_RUN_IN_SOURCE_TREE = xyes; then
	AC_DEFINE(RUN_IN_SOURCE_TREE, 1, [enable search for holiday.dat file in local directory])
fi

dnl  Language Support 
GETTEXT_PACKAGE=libgcl
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, "$GETTEXT_PACKAGE", [Define the gettext package to be used])
AC_SUBST(GETTEXT_PACKAGE)
AM_GLIB_GNU_GETTEXT

AM_GLIB_DEFINE_LOCALEDIR(LIBGCL_LOCALEDIR)

AC_CONFIG_FILES([
Makefile
gcl/Makefile
po/Makefile.in
tests/Makefile
data/Makefile
docs/Makefile
docs/reference/Makefile
docs/reference/version.xml
libgcl-1.pc
])

AC_OUTPUT

