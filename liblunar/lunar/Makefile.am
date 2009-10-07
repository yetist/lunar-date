NULL=

export_symbols = $(LIBTOOL_EXPORT_OPTIONS)

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
lunardate.gir: $(INTROSPECTION_SCANNER) liblunar-1.la
	$(AM_V_GEN) $(INTROSPECTION_SCANNER) -v \
        --namespace lunardate --nsversion=1 \
        $(INCLUDES) \
        --c-include='lunar/lunar.h' \
        --include=GObject-2.0 \
        --library=liblunar-1.la \
        --libtool="$(top_builddir)/libtool" \
        --pkg gobject-2.0 \
        --output $@ \
        $(source_h) \
        $(source_c)

BUILT_GIRSOURCES = lunardate.gir

girdir = $(datadir)/gir-1.0
gir_DATA = $(BUILT_GIRSOURCES)

lunardate.typelib: lunardate.gir

typelibsdir = $(libdir)/girepository-1.0/

typelibs_DATA = $(BUILT_GIRSOURCES:.gir=.typelib)

%.typelib:%.gir $(INTROSPECTION_COMPILER)
	$(AM_V_GEN) LD_LIBRARY_PATH=.libs$${LD_LIBRARY_PATH:+:$$LD_LIBRARY_PATH} \
        $(INTROSPECTION_COMPILER) \
        --includedir=$(srcdir) \
        --includedir=. \
        $(INTROSPECTION_COMPILER_OPTS) $< -o $(@F)

CLEANFILES = $(BUILT_GIRSOURCES) $(typelibs_DATA)
endif

EXTRA_DIST =  lunar-enum-types.h.template lunar-enum-types.c.template lunar-date-private.h lunar-config.h.in

-include $(top_srcdir)/git.mk
