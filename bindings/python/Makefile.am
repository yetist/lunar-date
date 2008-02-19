## Process this file with automake to produce Makefile.in

PYTHON_MODULES = lunarcalendar.la

BUILT_SOURCES = calendar.c

pythonsiteexecdir = $(pyexecdir)/gtk-2.0
pythonsiteexec_LTLIBRARIES = $(PYTHON_MODULES)

AM_LDFLAGS = $(PYTHON_LDFLAGS)

lunarcalendar_la_CFLAGS = $(LIBLUNAR_CFLAGS) $(PYTHON_INCLUDES) $(PYGTK_CFLAGS)
lunarcalendar_la_SOURCES = lunarmodule.c
lunarcalendar_la_LIBADD = $(top_builddir)/lunar/liblunar-1.la $(PYTHON_LIBS) $(LIBS) $(PYGTK_LIBS) $(LIBLUNAR_LIBS)
lunarcalendar_la_LDFLAGS = -module -avoid-version -export-symbols-regex initlunarcalendar
nodist_lunarcalendar_la_SOURCES = $(BUILT_SOURCES)

$(top_builddir)/lunar/lunar-enum-types.h:
	cd $(top_builddir)/lunar && $(MAKE) lunar-enum-types.h

calendar.c: calendar.defs calendar.override  $(top_builddir)/lunar/lunar-enum-types.h
	( cd $(srcdir) && $(PYGTK_CODEGEN) \
		--register $(PYGTK_DEFSDIR)/pango-types.defs \
		--register $(PYGTK_DEFSDIR)/gdk-types.defs \
		--register $(PYGTK_DEFSDIR)/gtk-types.defs \
		--override $*.override \
		--prefix py$* $(<F) ) > $@ 

regenerate-python-binding:
	$(PYGTK_H2DEF) $(sort $(BINDING_CALENDAR_HEADERS_SRCDIR) $(BINDING_HEADERS_BUILDDIR)) > calendar.defs.new

EXTRA_DIST = \
	calendar.override	\
	calendar.defs		\
	lunar-demo.py

CLEANFILES = $(BUILT_SOURCES)

dist-hook:
	cd $(distdir); rm -f $(BUILT_SOURCES)
