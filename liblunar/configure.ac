AC_PREREQ(2.59)

m4_define([liblunar_major_version], [2])
m4_define([liblunar_minor_version], [0])
m4_define([liblunar_micro_version], [2])
m4_define([liblunar_real_version], [liblunar_major_version.liblunar_minor_version.liblunar_micro_version])
m4_define([liblunar_version], [2.0.2])

AC_INIT(liblunar, liblunar_version, yetist@gmail.com)
AC_COPYRIGHT([Copyright (C) 2007-2009 yetist@gmail.com])

AM_INIT_AUTOMAKE(1.8 dist-bzip2 no-define)
AC_CONFIG_SRCDIR(lunar/lunar-date.h)
AM_CONFIG_HEADER(config.h)
AC_CONFIG_MACRO_DIR([m4])

AM_MAINTAINER_MODE

LIBLUNAR_VERSION=liblunar_real_version
AC_SUBST(LIBLUNAR_VERSION)

AC_PROG_CC
AC_ISC_POSIX
AC_HEADER_STDC

AC_PROG_INSTALL
AC_PROG_LIBTOOL

IT_PROG_INTLTOOL([0.35.0])

GTK_DOC_CHECK(1.0)

AC_SUBST(ACLOCAL_AMFLAGS, "$ACLOCAL_FLAGS -I m4")
cur_date=`date -R`
CUR_DATE=${cur_date}
AC_SUBST(CUR_DATE)

GLIB2_REQUIRED=2.4.0
GOBJECT_REQUIRED=2.4.0

PKG_CHECK_MODULES(LIBLUNAR, glib-2.0 >= $GLIB2_REQUIRED gobject-2.0 >= $GOBJECT_REQUIRED)

AC_SUBST(LIBLUNAR_CFLAGS)
AC_SUBST(LIBLUNAR_LIBS)

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


dnl ================================================================
dnl Python
dnl ================================================================

AC_MSG_CHECKING([whether Python support is requested])
AC_ARG_ENABLE([python],
	AS_HELP_STRING([--enable-python],[Enable python binding]),
	[enable_python=$enableval have_python=$enableval],
	[enable_python=autodetect have_python=yes])
AC_MSG_RESULT([$enable_python])

if test "x$have_python" != "xno"; then
	AM_PATH_PYTHON([2.3],[],[no])
	if test "x$PYTHON" = "x:"; then
		have_python=no
	fi
fi

if test "x$have_python" != "xno"; then
	AM_CHECK_PYTHON_HEADERS([],[have_python=no])
fi

if test "x$have_python" != "xno"; then
	PY_PREFIX=`$PYTHON -c 'import sys ; print sys.prefix'`
	PY_EXEC_PREFIX=`$PYTHON -c 'import sys ; print sys.exec_prefix'`
	PYTHON_LIBS="-lpython$PYTHON_VERSION"
	PYTHON_LIB_LOC="-L$PY_EXEC_PREFIX/lib/python$PYTHON_VERSION/config"
	PYTHON_CFLAGS="-I$PY_PREFIX/include/python$PYTHON_VERSION"
	PYTHON_MAKEFILE="$PY_EXEC_PREFIX/lib/python$PYTHON_VERSION/config/Makefile"
	PYTHON_LOCALMODLIBS=`sed -n -e 's/^LOCALMODLIBS=\(.*\)/\1/p' $PYTHON_MAKEFILE`
	PYTHON_BASEMODLIBS=`sed -n -e 's/^BASEMODLIBS=\(.*\)/\1/p' $PYTHON_MAKEFILE`
	PYTHON_OTHER_LIBS=`sed -n -e 's/^LIBS=\(.*\)/\1/p' $PYTHON_MAKEFILE`
	PYTHON_EXTRA_LIBS="$PYTHON_LOCALMODLIBS $PYTHON_BASEMODLIBS $PYTHON_OTHER_LIBS"
	AC_SUBST([PYTHON_LIBS])
	AC_SUBST([PYTHON_LIB_LOC])
	AC_SUBST([PYTHON_CFLAGS])
	AC_SUBST([PYTHON_EXTRA_LIBS])
fi

if test "x$have_python" != "xyes"; then
	if test "x$enable_python" = "xyes"; then
		AC_MSG_ERROR([Python not found])
	elif test "x$enable_python" = "xautodetect"; then
		enable_python=no
		AC_MSG_WARN([Python not found, disabling python support])
	fi
fi

if test "x$have_python" != "xno"; then
	PYGOBJECT_REQUIRED=2.15.4
	PYGTK_REQUIRED=2.12.0
	PYGTKSOURCEVIEW_REQUIRED=2.2.0

	PKG_CHECK_MODULES([PYGTK], [
		pygobject-2.0 >= $PYGOBJECT_REQUIRED
		pygtk-2.0 >= $PYGTK_REQUIRED
		pygtksourceview-2.0 >= $PYGTKSOURCEVIEW_REQUIRED],
		[],
		[
		have_python=no
		if test "x$enable_python" = "xyes"; then
			AC_MSG_ERROR([$PYGTK_PKG_ERRORS])
		elif test "x$enable_python" = "xautodetect"; then
                        enable_python=no
			AC_MSG_WARN([$PYGTK_PKG_ERRORS])
			AC_MSG_WARN([Disabling python support])
		fi
		])

	AC_SUBST([PYGTK_CFLAGS])
	AC_SUBST([PYGTK_LIBS])
fi

if test "x$have_python" != "xno"; then
	AC_MSG_CHECKING([for pygtk defs])
	PYGTK_DEFSDIR=`$PKG_CONFIG --variable=defsdir pygtk-2.0`
	AC_MSG_RESULT([$PYGTK_DEFSDIR])

	AC_MSG_CHECKING([for pygtk codegen])
	PYGTK_CODEGEN="$PYTHON `$PKG_CONFIG --variable=codegendir pygtk-2.0`/codegen.py"
	AC_MSG_RESULT([$PYGTK_CODEGEN])

	AC_MSG_CHECKING([for pygtk h2def])
	PYGTK_H2DEF="$PYTHON `$PKG_CONFIG --variable=codegendir pygtk-2.0`/h2def.py"
	AC_MSG_RESULT([$PYGTK_H2DEF])

	AC_SUBST([PYGTK_DEFSDIR])
	AC_SUBST([PYGTK_CODEGEN])
	AC_SUBST([PYGTK_H2DEF])

	dnl Check for -fno-strict-aliasing
	FLAGS="-fno-strict-aliasing"
	save_CFLAGS="$CFLAGS"
	CFLAGS="$CFLAGS $FLAGS"
	AC_MSG_CHECKING([whether [$]CC understands $FLAGS])
	AC_TRY_COMPILE([], [], [compiler_has_option=yes], [compiler_has_option=no])
	CFLAGS="$save_CFLAGS"
	AC_MSG_RESULT($compiler_has_option)
	if test $compiler_has_option = yes; then
		NO_STRICT_ALIASING_CFLAGS="$FLAGS"
	fi
	AC_SUBST([NO_STRICT_ALIASING_CFLAGS])
fi

if test "x$have_python" != "xno" -a "x$enable_python" != "xno"; then
	enable_python=yes
	AC_DEFINE([ENABLE_PYTHON],[1],[Define to compile with python support])
fi

AM_CONDITIONAL([ENABLE_PYTHON],[test "x$enable_python" = "xyes"])

dnl This allows the bug-report script to know whether python has been enabled
AC_SUBST(enable_python)

dnl  Language Support 
GETTEXT_PACKAGE=liblunar
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, "$GETTEXT_PACKAGE", [Define the gettext package to be used])
AC_SUBST(GETTEXT_PACKAGE)
AM_GLIB_GNU_GETTEXT

AM_GLIB_DEFINE_LOCALEDIR(LIBLUNAR_LOCALEDIR)

AC_CONFIG_FILES([
liblunar-1.pc
lunar/Makefile
tests/Makefile
data/Makefile
po/Makefile.in
docs/Makefile
docs/reference/Makefile
docs/reference/version.xml
Makefile
])

AC_OUTPUT
echo "

Configuration:

        Source code location:   ${srcdir}
	Compiler:		${CC}
	Python binding Support: $enable_python
"

