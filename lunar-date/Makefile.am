export_symbols = $(LIBTOOL_EXPORT_OPTIONS)

CLEANFILES =

AM_CPPFLAGS = -DLIBLUNAR_DISABLE_DEPRECATED

INCLUDES =                                              \
	-I$(top_srcdir)					\
        -I$(srcdir) 					\
	$(LUNAR_DATE_CFLAGS)				\
	$(WARN_CFLAGS)					\
        -DLUNAR_HOLIDAYDIR=\""$(datadir)/liblunar/"\"

source_h = 	\
	$(srcdir)/lunar-date.h		\
	$(srcdir)/lunar-version.h

source_c =	\
	$(srcdir)/lunar-date.c		\
	$(srcdir)/lunar-version.c	\
	$(srcdir)/lunar-date-private.c

BUILT_SOURCES =         	\
        lunar-date-enum-types.c        \
        lunar-date-enum-types.h

lib_LTLIBRARIES = liblunar-date-2.0.la

liblunar_date_2_0_includedir = $(includedir)/liblunar-2.0/lunar-date
liblunar_date_2_0_include_HEADERS = $(source_h)
liblunar_date_2_0_la_SOURCES =	$(source_c) $(BUILT_SOURCES)
liblunar_date_2_0_la_LIBADD = 	$(LUNAR_DATE_LIBS)
liblunar_date_2_0_la_LDFLAGS = \
	-version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) \
	-export-dynamic $(export_symbols)

lunar-date-enum-types.h: lunar-date-enum-types.h.template $(source_h) $(GLIB_MKENUMS)
	$(AM_V_GEN) (cd $(srcdir) && $(GLIB_MKENUMS) --template lunar-date-enum-types.h.template $(source_h)) > $@
        
lunar-date-enum-types.c: lunar-date-enum-types.c.template $(source_h) $(GLIB_MKENUMS)
	$(AM_V_GEN) (cd $(srcdir) && $(GLIB_MKENUMS) --template lunar-date-enum-types.c.template $(source_h)) > $@
DISTCLEANFILES = lunar-date-enum-types.h lunar-date-enum-types.c

if HAVE_INTROSPECTION
-include $(INTROSPECTION_MAKEFILE)
INTROSPECTION_GIRS = LunarDate-2.0.gir
#INTROSPECTION_COMPILER_ARGS - Additional args to pass in to the compiler


LunarDate-2.0.gir: liblunar-date-2.0.la
INTROSPECTION_SCANNER_ARGS = -I$(top_srcdir) -I$(srcdir) --warn-all --identifier-prefix=Lunar
LunarDate_2_0_gir_NAMESPACE = LunarDate
LunarDate_2_0_gir_VERSION = 2.0
LunarDate_2_0_gir_LIBS = $(builddir)/liblunar-date-2.0.la
LunarDate_2_0_gir_FILES = $(source_h) $(source_c) $(BUILT_SOURCES)
LunarDate_2_0_gir_INCLUDES = GObject-2.0

girdir = $(datadir)/gir-1.0
dist_gir_DATA = $(INTROSPECTION_GIRS)

typelibdir = $(libdir)/girepository-1.0
typelib_DATA = $(INTROSPECTION_GIRS:.gir=.typelib)

CLEANFILES += $(dist_gir_DATA) $(typelib_DATA)
endif

EXTRA_DIST =  lunar-date-enum-types.h.template lunar-date-enum-types.c.template lunar-date-private.h lunar-version.h.in

-include $(top_srcdir)/git.mk
