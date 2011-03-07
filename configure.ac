AC_PREREQ(2.62)

m4_define([liblunar_major_version], [2])
m4_define([liblunar_minor_version], [2])
m4_define([liblunar_micro_version], [5])
m4_define([liblunar_interface_age], [1])
m4_define([liblunar_binary_age], [m4_eval(100 * liblunar_minor_version + liblunar_micro_version)])
m4_define([liblunar_version], [liblunar_major_version.liblunar_minor_version.liblunar_micro_version])

m4_define([liblunar_lt_current],
	            [m4_eval(100 * liblunar_minor_version + liblunar_micro_version - liblunar_interface_age)])
m4_define([liblunar_lt_revision], [liblunar_interface_age])
m4_define([liblunar_lt_age], [m4_eval(liblunar_binary_age - liblunar_interface_age)])

AC_INIT(liblunar, liblunar_version, yetist@gmail.com)
AC_COPYRIGHT([Copyright (C) 2007-2011 yetist@gmail.com])

AM_INIT_AUTOMAKE([1.8 dist-bzip2 no-define -Wno-portability])
AC_CONFIG_SRCDIR(lunar/lunar-date.h)
AM_CONFIG_HEADER(config.h)
AC_CONFIG_MACRO_DIR([m4])

LT_CURRENT=liblunar_lt_current
LT_REVISION=liblunar_lt_revision
LT_AGE=liblunar_lt_age
AC_SUBST(LT_CURRENT)
AC_SUBST(LT_REVISION)
AC_SUBST(LT_AGE)

LIBLUNAR_VERSION=liblunar_version
LIBLUNAR_BINARY_AGE=liblunar_binary_age
AC_SUBST(LIBLUNAR_VERSION)
AC_SUBST(LIBLUNAR_BINARY_AGE)

LIBTOOL_EXPORT_OPTIONS='-export-symbols-regex "^lunar.*"'
AC_SUBST(LIBTOOL_EXPORT_OPTIONS)

LIBLUNAR_MAJOR_VERSION=liblunar_major_version
LIBLUNAR_MINOR_VERSION=liblunar_minor_version
LIBLUNAR_MICRO_VERSION=liblunar_micro_version

AC_SUBST(LIBLUNAR_MAJOR_VERSION)
AC_SUBST(LIBLUNAR_MINOR_VERSION)
AC_SUBST(LIBLUNAR_MICRO_VERSION)

AC_DEFINE(LIBLUNAR_MAJOR_VERSION, [liblunar_major_version],
          [Define to the LIBLUNAR major version])
AC_DEFINE(LIBLUNAR_MINOR_VERSION, [liblunar_minor_version],
          [Define to the LIBLUNAR minor version])
AC_DEFINE(LIBLUNAR_MICRO_VERSION, [liblunar_micro_version],
          [Define to the LIBLUNAR micro version])
AC_DEFINE(LIBLUNAR_BINARY_AGE, [liblunar_binary_age],
	            [Define to the LIBLUNAR binary age])

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

AC_SUBST(ACLOCAL_AMFLAGS, "$ACLOCAL_FLAGS -I m4")
cur_date=`date -R`
CUR_DATE=${cur_date}
AC_SUBST(CUR_DATE)

dnl ================================================================
dnl check glib and gobject
dnl ================================================================

GLIB2_REQUIRED=2.4.0
GOBJECT_REQUIRED=2.4.0

PKG_CHECK_MODULES(LIBLUNAR, glib-2.0 >= $GLIB2_REQUIRED gobject-2.0 >= $GOBJECT_REQUIRED)

AC_SUBST(LIBLUNAR_CFLAGS)
AC_SUBST(LIBLUNAR_LIBS)

dnl ================================================================
dnl run in source tree
dnl ================================================================
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

dnl ================================================================
dnl vala bindings support
dnl ================================================================
AC_ARG_ENABLE(vala-bindings,
  AC_HELP_STRING([--enable-vala-bindings],
                 [build the EXPERIMENTAL Vala bindings]),
                  enable_vala_bindings=$enableval,
                  enable_vala_bindings=no )

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
dnl Python
dnl ================================================================

dnl AC_MSG_CHECKING([whether Python support is requested])
dnl AC_ARG_ENABLE([python],
dnl 	AS_HELP_STRING([--enable-python],[Enable python binding]),
dnl 	[enable_python=$enableval have_python=$enableval],
dnl 	[enable_python=autodetect have_python=yes])
dnl AC_MSG_RESULT([$enable_python])
dnl 
dnl if test "x$have_python" != "xno"; then
dnl 	AM_PATH_PYTHON([2.3],[],[no])
dnl 	if test "x$PYTHON" = "x:"; then
dnl 		have_python=no
dnl 	fi
dnl fi
dnl 
dnl if test "x$have_python" != "xno"; then
dnl 	AM_CHECK_PYTHON_HEADERS([],[have_python=no])
dnl fi
dnl 
dnl if test "x$have_python" != "xno"; then
dnl 	PY_PREFIX=`$PYTHON -c 'import sys ; print sys.prefix'`
dnl 	PY_EXEC_PREFIX=`$PYTHON -c 'import sys ; print sys.exec_prefix'`
dnl 	PYTHON_LIBS="-lpython$PYTHON_VERSION"
dnl 	PYTHON_LIB_LOC="-L$PY_EXEC_PREFIX/lib/python$PYTHON_VERSION/config"
dnl 	PYTHON_CFLAGS="-I$PY_PREFIX/include/python$PYTHON_VERSION"
dnl 	PYTHON_MAKEFILE="$PY_EXEC_PREFIX/lib/python$PYTHON_VERSION/config/Makefile"
dnl 	PYTHON_LOCALMODLIBS=`sed -n -e 's/^LOCALMODLIBS=\(.*\)/\1/p' $PYTHON_MAKEFILE`
dnl 	PYTHON_BASEMODLIBS=`sed -n -e 's/^BASEMODLIBS=\(.*\)/\1/p' $PYTHON_MAKEFILE`
dnl 	PYTHON_OTHER_LIBS=`sed -n -e 's/^LIBS=\(.*\)/\1/p' $PYTHON_MAKEFILE`
dnl 	PYTHON_EXTRA_LIBS="$PYTHON_LOCALMODLIBS $PYTHON_BASEMODLIBS $PYTHON_OTHER_LIBS"
dnl 	AC_SUBST([PYTHON_LIBS])
dnl 	AC_SUBST([PYTHON_LIB_LOC])
dnl 	AC_SUBST([PYTHON_CFLAGS])
dnl 	AC_SUBST([PYTHON_EXTRA_LIBS])
dnl fi
dnl 
dnl if test "x$have_python" != "xyes"; then
dnl 	if test "x$enable_python" = "xyes"; then
dnl 		AC_MSG_ERROR([Python not found])
dnl 	elif test "x$enable_python" = "xautodetect"; then
dnl 		enable_python=no
dnl 		AC_MSG_WARN([Python not found, disabling python support])
dnl 	fi
dnl fi
dnl 
dnl if test "x$have_python" != "xno"; then
dnl 	PYGOBJECT_REQUIRED=2.15.4
dnl 
dnl 	PKG_CHECK_MODULES([PYGOBJECT], [ pygobject-2.0 >= $PYGOBJECT_REQUIRED ], [],
dnl 		[
dnl 		have_python=no
dnl 		if test "x$enable_python" = "xyes"; then
dnl 			AC_MSG_ERROR([$PYGOBJECT_PKG_ERRORS])
dnl 		elif test "x$enable_python" = "xautodetect"; then
dnl                         enable_python=no
dnl 			AC_MSG_WARN([$PYGOBJECT_PKG_ERRORS])
dnl 			AC_MSG_WARN([Disabling python support])
dnl 		fi
dnl 		])
dnl 
dnl 	AC_SUBST([PYGOBJECT_CFLAGS])
dnl 	AC_SUBST([PYGOBJECT_LIBS])
dnl fi
dnl 
dnl if test "x$have_python" != "xno"; then
dnl 
dnl 	AC_MSG_CHECKING([for pygobject codegen])
dnl 	PYGOBJECT_CODEGEN="$PYTHON `$PKG_CONFIG --variable=codegendir pygobject-2.0`/codegen.py"
dnl 	AC_MSG_RESULT([$PYGOBJECT_CODEGEN])
dnl 
dnl 	AC_MSG_CHECKING([for pygobject h2def])
dnl 	PYGOBJECT_H2DEF="$PYTHON `$PKG_CONFIG --variable=codegendir pygobject-2.0`/h2def.py"
dnl 	AC_MSG_RESULT([$PYGOBJECT_H2DEF])
dnl 
dnl 	AC_SUBST([PYGOBJECT_CODEGEN])
dnl 	AC_SUBST([PYGOBJECT_H2DEF])
dnl 
dnl 	dnl Check for -fno-strict-aliasing
dnl 	FLAGS="-fno-strict-aliasing"
dnl 	save_CFLAGS="$CFLAGS"
dnl 	CFLAGS="$CFLAGS $FLAGS"
dnl 	AC_MSG_CHECKING([whether [$]CC understands $FLAGS])
dnl 	AC_TRY_COMPILE([], [], [compiler_has_option=yes], [compiler_has_option=no])
dnl 	CFLAGS="$save_CFLAGS"
dnl 	AC_MSG_RESULT($compiler_has_option)
dnl 	if test $compiler_has_option = yes; then
dnl 		NO_STRICT_ALIASING_CFLAGS="$FLAGS"
dnl 	fi
dnl 	AC_SUBST([NO_STRICT_ALIASING_CFLAGS])
dnl fi
dnl 
dnl if test "x$have_python" != "xno" -a "x$enable_python" != "xno"; then
dnl 	enable_python=yes
dnl 	AC_DEFINE([ENABLE_PYTHON],[1],[Define to compile with python support])
dnl fi
dnl 
dnl AM_CONDITIONAL([ENABLE_PYTHON],[test "x$enable_python" = "xyes"])
dnl 
dnl dnl This allows the bug-report script to know whether python has been enabled
dnl AC_SUBST(enable_python)


dnl ================================================================
dnl  Language Support 
dnl ================================================================
GETTEXT_PACKAGE=liblunar
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, "$GETTEXT_PACKAGE", [Define the gettext package to be used])
AC_SUBST(GETTEXT_PACKAGE)
AM_GLIB_GNU_GETTEXT

AM_GLIB_DEFINE_LOCALEDIR(LIBLUNAR_LOCALEDIR)

AC_CONFIG_FILES([
liblunar-1.0.pc
lunar/lunar-config.h
lunar/Makefile
tests/Makefile
data/Makefile
po/Makefile.in
docs/Makefile
docs/reference/Makefile
docs/reference/version.xml
Makefile
vala/Makefile
])

AC_OUTPUT
echo "

Configuration:

        Source code location:   ${srcdir}
	Compiler:		${CC}
	Python binding Support: $enable_python
"
