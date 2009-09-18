## Process this file with automake to produce Makefile.in

PYTHON_MODULES = lunar.la

BUILT_SOURCES = calendar.c

pythonsiteexecdir = $(pyexecdir)/gtk-2.0
pythonsiteexec_LTLIBRARIES = $(PYTHON_MODULES)

AM_LDFLAGS = $(PYTHON_LDFLAGS)

lunar_la_CFLAGS = $(LIBLUNAR_CFLAGS) $(PYTHON_INCLUDES) $(PYGTK_CFLAGS)
lunar_la_SOURCES = lunarmodule.c
lunar_la_LIBADD = $(top_builddir)/lunar/liblunar-1.la $(PYTHON_LIBS) $(LIBS) $(PYGTK_LIBS) $(LIBLUNAR_LIBS)
lunar_la_LDFLAGS = -module -avoid-version -export-symbols-regex initlunar
nodist_lunar_la_SOURCES = $(BUILT_SOURCES)

$(top_builddir)/lunar/lunar-enum-types.h:
	cd $(top_builddir)/lunar && $(MAKE) lunar-enum-types.h

calendar.c: calendar.defs calendar.override  $(top_builddir)/lunar/lunar-enum-types.h
	( cd $(srcdir) && $(PYGTK_CODEGEN) \
		--register $(PYGTK_DEFSDIR)/pango-types.defs \
		--register $(PYGTK_DEFSDIR)/gdk-types.defs \
		--register $(PYGTK_DEFSDIR)/gtk-types.defs \
		--override $*.override \
		--prefix py$* $(<F) ) > $@ 

BINDING_CALENDAR_HEADERS_SRCDIR_IN = \
       lunar/lunar-calendar.h

BINDING_HEADERS_BUILDDIR_IN = lunar/lunar-main.h

BINDING_CALENDAR_HEADERS_SRCDIR        := $(addprefix $(top_srcdir)/,$(BINDING_CALENDAR_HEADERS_SRCDIR_IN))
BINDING_HEADERS_BUILDDIR	:= $(addprefix $(top_builddir)/,$(BINDING_HEADERS_BUILDDIR_IN))

regenerate-python-binding:
	$(PYGTK_H2DEF) $(sort $(BINDING_CALENDAR_HEADERS_SRCDIR) $(BINDING_HEADERS_BUILDDIR)) > calendar.defs.new

EXTRA_DIST = \
	calendar.override	\
	calendar.defs		\
	lunar-demo.py

CLEANFILES = $(BUILT_SOURCES)

dist-hook:
	cd $(distdir); rm -f $(BUILT_SOURCES)

-include $(top_srcdir)/git.mk
