## Process this file with automake to produce Makefile.in

PYTHON_MODULES = gclcalendar.la

BUILT_SOURCES = calendar.c

pythonsiteexecdir = $(pyexecdir)/gtk-2.0
pythonsiteexec_LTLIBRARIES = $(PYTHON_MODULES)

AM_LDFLAGS = $(PYTHON_LDFLAGS)

gclcalendar_la_CFLAGS = $(LIBGCL_CFLAGS) $(PYTHON_INCLUDES) $(PYGTK_CFLAGS)
gclcalendar_la_SOURCES = gclmodule.c
gclcalendar_la_LIBADD = $(top_builddir)/gcl/libgcl-1.la $(PYTHON_LIBS) $(LIBS) $(PYGTK_LIBS) $(LIBGCL_LIBS)
gclcalendar_la_LDFLAGS = -module -avoid-version -export-symbols-regex initgclcalendar
nodist_gclcalendar_la_SOURCES = $(BUILT_SOURCES)

$(top_builddir)/gcl/gcl-enum-types.h:
	cd $(top_builddir)/gcl && $(MAKE) gcl-enum-types.h

calendar.c: calendar.defs calendar.override  $(top_builddir)/gcl/gcl-enum-types.h
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
	gcl-demo.py

CLEANFILES = $(BUILT_SOURCES)

dist-hook:
	cd $(distdir); rm -f $(BUILT_SOURCES)
