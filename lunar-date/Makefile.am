export_symbols = $(LIBTOOL_EXPORT_OPTIONS)

CLEANFILES =

INCLUDES =                                              \
	-I$(top_srcdir)					\
        -I$(srcdir) 					\
	$(LIBLUNAR_CFLAGS)				\
	$(WARN_CFLAGS)					\
        -DLUNAR_HOLIDAYDIR=\""$(datadir)/liblunar/"\"

source_h = 	\
	$(srcdir)/lunar-date.h		\
	$(srcdir)/lunar-config.h	\
	$(srcdir)/lunar-main.h

source_c =	\
	$(srcdir)/lunar-date.c		\
	$(srcdir)/lunar-date-private.c	\
	$(srcdir)/lunar-main.c

lunardatedir=$(includedir)/liblunar-1.0
lunardate_HEADERS = date.h


BUILT_SOURCES =         	\
        lunar-enum-types.c        \
        lunar-enum-types.h

lib_LTLIBRARIES = liblunar-date-1.0.la

liblunar_date_1_0_includedir = $(includedir)/liblunar-1.0/lunar-date
liblunar_date_1_0_include_HEADERS = $(source_h)
liblunar_date_1_0_la_SOURCES =	$(source_c) $(BUILT_SOURCES)
liblunar_date_1_0_la_LIBADD = 	$(LIBLUNAR_LIBS)
liblunar_date_1_0_la_LDFLAGS = \
	-version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) \
	-export-dynamic $(export_symbols)

lunar-enum-types.h: lunar-enum-types.h.template $(source_h) $(GLIB_MKENUMS)
	$(AM_V_GEN) (cd $(srcdir) && $(GLIB_MKENUMS) --template lunar-enum-types.h.template $(source_h)) > $@
        
lunar-enum-types.c: lunar-enum-types.c.template $(source_h) $(GLIB_MKENUMS)
	$(AM_V_GEN) (cd $(srcdir) && $(GLIB_MKENUMS) --template lunar-enum-types.c.template $(source_h)) > $@
DISTCLEANFILES = lunar-enum-types.h lunar-enum-types.c

if HAVE_INTROSPECTION
-include $(INTROSPECTION_MAKEFILE)
INTROSPECTION_GIRS = LunarDate-1.0.gir
#INTROSPECTION_COMPILER_ARGS - Additional args to pass in to the compiler


LunarDate-1.0.gir: liblunar-date-1.0.la
INTROSPECTION_SCANNER_ARGS = -I$(top_srcdir) -I$(srcdir) --warn-all --identifier-prefix=Lunar
LunarDate_1_0_gir_NAMESPACE = LunarDate
LunarDate_1_0_gir_VERSION = 1.0
LunarDate_1_0_gir_LIBS = $(builddir)/liblunar-date-1.0.la
LunarDate_1_0_gir_FILES = $(source_h) $(source_c) $(BUILT_SOURCES)
LunarDate_1_0_gir_INCLUDES = GObject-2.0

girdir = $(datadir)/gir-1.0
dist_gir_DATA = $(INTROSPECTION_GIRS)

typelibdir = $(libdir)/girepository-1.0
typelib_DATA = $(INTROSPECTION_GIRS:.gir=.typelib)

CLEANFILES += $(dist_gir_DATA) $(typelibs_DATA)
endif

EXTRA_DIST =  lunar-enum-types.h.template lunar-enum-types.c.template lunar-date-private.h lunar-config.h.in

-include $(top_srcdir)/git.mk
