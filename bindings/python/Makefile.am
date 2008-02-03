## Process this file with automake to produce Makefile.in

# python scripts & extension modules
pkgpythondir = $(pyexecdir)/gcl-1
pkgpyexecdir = $(pyexecdir)/gcl-1

# gtk python extension modules
pygtkexecdir = $(pkgpyexecdir)/gcl
pygtkexec_LTLIBRARIES = _gcl.la

# gtk python scripts
pygcldir = $(pkgpythondir)/gcl
pygcl_PYTHON =                  \
        __init__.py             \
        _lazyutils.py           \
        compat.py                       \
        deprecation.py          \
        keysyms.py

common_ldflags = -module -avoid-version

BUILT_SOURCES = \
	calendar.c		\
	date.c

_gcl_la_SOURCES = \
	$(BUILT_SOURCES)	\
	gclmodule.c

_gcl_la_LDFLAGS = $(common_ldflags) -export-symbols-regex init_gcl

_gcl_la_LIBADD = \
	$(top_builddir)/gcl/libgcl-1.la
	$(PYTHON_LIB_LOC)       \
	$(PYTHON_LIBS)		\
	$(PYTHON_EXTRA_LIBS)	\
	$(PYGTK_LIBS)

_gcl_la_CFLAGS = \
	-I$(top_srcdir)			\
	-I$(top_builddir)		\
	-I$(top_srcdir)/gcl		\
	-I$(top_builddir)/gcl		\
	$(LIBGCL_CFLAGS)		\
	$(PYGTK_CFLAGS)			\
	$(PYTHON_CFLAGS)		\
	$(AM_CFLAGS)

$(top_builddir)/gcl/gcl-enum-types.h:
	cd $(top_builddir)/gcl && $(MAKE) gcl-enum-types.h

calendar.c: calendar.defs calendar.override  $(top_builddir)/gcl/gcl-enum-types.h
	( cd $(srcdir) && $(PYGTK_CODEGEN) \
		--register $(PYGTK_DEFSDIR)/pango-types.defs \
		--register $(PYGTK_DEFSDIR)/gdk-types.defs \
		--register $(PYGTK_DEFSDIR)/gtk-types.defs \
		--override $*.override \
		--prefix py$* $(<F) ) > $@ 

date.c: date.defs date.override $(top_builddir)/gcl/gcl-enum-types.h
	( cd $(srcdir) && $(PYGTK_CODEGEN) \
		--override $*.override \
		--prefix py$* $(<F) ) > $@ 

BINDING_CALENDAR_HEADERS_SRCDIR_IN = \
	gcl/gcl-calendar.h

BINDING_DATE_HEADERS_SRCDIR_IN = \
	gcl/gcl-date.h

BINDING_HEADERS_BUILDDIR_IN = gcl/gcl-main.h

BINDING_CALENDAR_HEADERS_SRCDIR	:= $(addprefix $(top_srcdir)/,$(BINDING_CALENDAR_HEADERS_SRCDIR_IN))
BINDING_DATE_HEADERS_SRCDIR	:= $(addprefix $(top_srcdir)/,$(BINDING_DATE_HEADERS_SRCDIR_IN))

BINDING_HEADERS_BUILDDIR	:= $(addprefix $(top_builddir)/,$(BINDING_HEADERS_BUILDDIR_IN))

regenerate-python-binding:
	$(PYGTK_H2DEF) $(sort $(BINDING_CALENDAR_HEADERS_SRCDIR) $(BINDING_HEADERS_BUILDDIR)) > calendar.defs.new
	$(PYGTK_H2DEF) $(sort $(BINDING_DATE_HEADERS_SRCDIR) $(BINDING_HEADERS_BUILDDIR)) > date.defs.new


EXTRA_DIST = \
	calendar.override		\
	calendar.defs		\
	date.override	\
	date.defs

CLEANFILES = $(BUILT_SOURCES)

dist-hook:
	cd $(distdir); rm -f $(BUILT_SOURCES)
