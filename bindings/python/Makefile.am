INCLUDES =                                              \
	-I$(top_srcdir)					\
	-I$(top_srcdir)/lunar				\
        -I$(srcdir) 					\
	$(LIBLUNAR_CFLAGS)

BUILT_SOURCES = lunardate.c 

pythonsiteexecdir = $(pyexecdir)/gtk-2.0/liblunar
pythonsiteexec_LTLIBRARIES = _lunardate.la

# liblunar python scripts
pyliblunardir = $(pythonsiteexecdir)
pyliblunar_PYTHON = __init__.py

AM_LDFLAGS = $(PYTHON_LDFLAGS)

_lunardate_la_CFLAGS = $(LIBLUNAR_CFLAGS) $(PYTHON_INCLUDES) $(PYGOBJECT_CFLAGS)
_lunardate_la_LDFLAGS = -module -avoid-version -export-symbols-regex init_lunardate
_lunardate_la_LIBADD = $(top_builddir)/lunar/liblunar-1.la $(PYTHON_LIBS) $(LIBS) $(LIBLUNAR_LIBS)
_lunardate_la_SOURCES = lunarmodule.c $(top_builddir)/lunar/lunar-enum-types.c
nodist__lunardate_la_SOURCES = lunardate.c

$(top_builddir)/lunar/lunar-enum-types.h:
	cd $(top_builddir)/lunar && $(MAKE) lunar-enum-types.h

lunardate.c: lunardate.defs lunardate.override  $(top_builddir)/lunar/lunar-enum-types.h
	( cd $(srcdir) && $(PYGOBJECT_CODEGEN) \
		--override $*.override \
		--prefix py$* $(<F) ) > $@ 

BINDING_LUNAR_HEADERS_SRCDIR_IN = lunar/lunar-date.h
BINDING_HEADERS_BUILDDIR_IN = lunar/lunar-main.h

BINDING_LUNAR_HEADERS_SRCDIR        := $(addprefix $(top_srcdir)/,$(BINDING_LUNAR_HEADERS_SRCDIR_IN))
BINDING_HEADERS_BUILDDIR	:= $(addprefix $(top_builddir)/,$(BINDING_HEADERS_BUILDDIR_IN))

regenerate-python-binding:
	$(PYGOBJECT_H2DEF) $(sort $(BINDING_LUNAR_HEADERS_SRCDIR) $(BINDING_HEADERS_BUILDDIR)) gdate.h > lunardate.defs.new; \
	sed -i	\
	-e 's/GDateYear/guint16/g'	\
	-e 's/GDateDay/guint8/g'	\
	-e 's/GDateHour/guint8/g'	\
	lunardate.defs.new;	\
	mv lunardate.defs.new lunardate.defs;

EXTRA_DIST = \
	lunardate.override	\
	lunardate.defs		\
	lunardate-demo.py	\
	gdate.h			\
	README

CLEANFILES = $(BUILT_SOURCES)

dist-hook:
	cd $(distdir); rm -f $(BUILT_SOURCES)

all: $(pythonsiteexec_LTLIBRARIES:.la=.so)

clean-local:
	rm -f $(pythonsiteexec_LTLIBRARIES:.la=.so)

.la.so:
	        $(LN_S) .libs/$@ $@ || true

-include $(top_srcdir)/git.mk
