INCLUDES =                                              \
	-I$(top_srcdir)					\
	-I$(top_srcdir)/lunar-gtk			\
        -I$(srcdir) 					\
	$(LIBLUNAR_CFLAGS)

pythonsiteexecdir = $(pyexecdir)/gtk-2.0/liblunar
pythonsiteexec_LTLIBRARIES = _lunarcalendar.la

BUILT_SOURCES = lunarcalendar.c
AM_LDFLAGS = $(PYTHON_LDFLAGS)

_lunarcalendar_la_CFLAGS = $(LIBLUNAR_CFLAGS) $(PYTHON_INCLUDES) $(PYGTK_CFLAGS)
_lunarcalendar_la_LDFLAGS = -module -avoid-version -export-symbols-regex init_lunarcalendar
_lunarcalendar_la_LIBADD = $(top_builddir)/lunar-gtk/liblunar-gtk.la $(PYTHON_LIBS) $(LIBS) $(PYGTK_LIBS) $(LIBLUNAR_LIBS)
_lunarcalendar_la_SOURCES = lunarmodule.c
nodist__lunarcalendar_la_SOURCES = $(BUILT_SOURCES)

lunarcalendar.c: lunarcalendar.defs lunarcalendar.override
	( cd $(srcdir) && $(PYGTK_CODEGEN) \
		--register $(PYGTK_DEFSDIR)/pango-types.defs \
		--register $(PYGTK_DEFSDIR)/gdk-types.defs \
		--register $(PYGTK_DEFSDIR)/gtk-types.defs \
		--override $*.override \
		--prefix py$* $(<F) ) > $@ 

BINDING_CALENDAR_HEADERS_SRCDIR_IN = lunar-gtk/lunar-calendar.h

BINDING_CALENDAR_HEADERS_SRCDIR        := $(addprefix $(top_srcdir)/,$(BINDING_CALENDAR_HEADERS_SRCDIR_IN))

regenerate-python-binding:
	$(PYGTK_H2DEF) $(sort $(BINDING_CALENDAR_HEADERS_SRCDIR)) > lunarcalendar.defs.new

EXTRA_DIST = \
	lunarcalendar.override	\
	lunarcalendar.defs		\
	lunar-demo.py

CLEANFILES = $(BUILT_SOURCES)

dist-hook:
	cd $(distdir); rm -f $(BUILT_SOURCES)

all: $(pythonsiteexec_LTLIBRARIES:.la=.so)

clean-local:
	rm -f $(pythonsiteexec_LTLIBRARIES:.la=.so)

.la.so:
	        $(LN_S) .libs/$@ $@ || true

