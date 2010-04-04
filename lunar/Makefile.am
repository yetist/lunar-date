NULL=

export_symbols = $(LIBTOOL_EXPORT_OPTIONS)

-include $(INTROSPECTION_MAKEFILE)
INTROSPECTION_GIRS =
INTROSPECTION_SCANNER_ARGS =
#        --add-include-path=../gdk
INTROSPECTION_COMPILER_ARGS = \
	--includedir=$(srcdir) \
	--includedir=.
CLEANFILES =

INCLUDES =                                              \
	-I$(top_srcdir)					\
        -I$(srcdir) 					\
	$(LIBLUNAR_CFLAGS)				\
        -DLUNAR_HOLIDAYDIR=\""$(datadir)/liblunar/"\"     \
	$(NULL)

source_h = 	\
	$(srcdir)/lunar.h		\
	$(srcdir)/lunar-date.h		\
	$(srcdir)/lunar-config.h	\
	$(srcdir)/lunar-main.h

source_c =	\
	$(srcdir)/lunar-date.c		\
	$(srcdir)/lunar-date-private.c	\
	$(srcdir)/lunar-main.c

BUILT_SOURCES = 

if ENABLE_PYTHON
BUILT_SOURCES +=         	\
        lunar-enum-types.c        \
        lunar-enum-types.h
endif

lib_LTLIBRARIES = liblunar-1.la

liblunar_1_includedir = $(includedir)/liblunar-1/lunar

liblunar_1_la_SOURCES =	\
	$(source_c)	\
	$(NULL)

liblunar_1_include_HEADERS =	\
	$(source_h)		\
	$(NULL)

liblunar_1_la_LIBADD = 	\
	$(LIBLUNAR_LIBS)	\
	$(NULL)

liblunar_1_la_LDFLAGS = \
	-version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) \
	-export-dynamic $(export_symbols)

if ENABLE_PYTHON
lunar-enum-types.h: lunar-enum-types.h.template $(source_h) $(GLIB_MKENUMS)
	$(AM_V_GEN) (cd $(srcdir) && $(GLIB_MKENUMS) --template lunar-enum-types.h.template $(source_h)) > $@
        
lunar-enum-types.c: lunar-enum-types.c.template $(source_h) $(GLIB_MKENUMS)
	$(AM_V_GEN) (cd $(srcdir) && $(GLIB_MKENUMS) --template lunar-enum-types.c.template $(source_h)) > $@
DISTCLEANFILES = lunar-enum-types.h lunar-enum-types.c
endif

if HAVE_INTROSPECTION
introspection_files = \
        $(source_h) \
        $(source_c)

LunarDate-1.0.gir: $(INTROSPECTION_SCANNER) liblunar-1.la Makefile
LunarDate_1_0_gir_SCANNERFLAGS = --strip-prefix=Lunar
LunarDate_1_0_gir_INCLUDES = GObject-2.0
LunarDate_1_0_gir_CFLAGS = $(INCLUDES)
LunarDate_1_0_gir_LIBS = liblunar-1.la
LunarDate_1_0_gir_FILES = $(introspection_files)
INTROSPECTION_GIRS += LunarDate-1.0.gir

girdir = $(datadir)/gir-1.0
dist_gir_DATA = $(INTROSPECTION_GIRS)

typelibsdir = $(libdir)/girepository-1.0
typelibs_DATA = $(INTROSPECTION_GIRS:.gir=.typelib)

CLEANFILES += $(dist_gir_DATA) $(typelibs_DATA)
endif

EXTRA_DIST =  lunar-enum-types.h.template lunar-enum-types.c.template lunar-date-private.h lunar-config.h.in

-include $(top_srcdir)/git.mk
