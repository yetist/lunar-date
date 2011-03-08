export_symbols = $(LIBTOOL_EXPORT_OPTIONS)

CLEANFILES =

INCLUDES =                                              \
	-I$(top_srcdir)					\
        -I$(srcdir) 					\
	$(LIBLUNAR_CFLAGS)				\
	$(WARN_CFLAGS)

source_h = 	\
	$(srcdir)/lunar.h		\
	$(srcdir)/lunar-calendar.h 

source_c = lunar-calendar.c

lib_LTLIBRARIES = liblunar-gtk-1.0.la liblunar-gtk-preload.la

liblunar_gtk_1_0_includedir = $(includedir)/liblunar-1.0/lunar-calendar

liblunar_gtk_1_0_la_SOURCES = $(source_c)

liblunar_gtk_1_0_include_HEADERS =	\
	$(source_h)		\
	$(NULL)

liblunar_gtk_1_0_la_LIBADD = 	\
	$(LIBLUNAR_LIBS)	\
	$(NULL)

liblunar_gtk_1_0_la_LDFLAGS = \
	-version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) \
	-export-dynamic $(export_symbols)


liblunar_gtk_preload_includedir = $(includedir)/liblunar-1.0/lunar-calendar

liblunar_gtk_preload_la_SOURCES = liblunar-gtk-preload.c

liblunar_gtk_preload_include_HEADERS =	\
	$(source_h)		\
	$(NULL)

liblunar_gtk_preload_la_LIBADD = 	\
	$(LIBLUNAR_LIBS)	\
	liblunar-gtk-1.0.la		\
	-ldl			\
	$(NULL)

liblunar_gtk_preload_la_LDFLAGS =  -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) 

if HAVE_INTROSPECTION
-include $(INTROSPECTION_MAKEFILE)
INTROSPECTION_GIRS = LunarCalendar-1.0.gir
INTROSPECTION_SCANNER_ARGS = -I$(top_srcdir) -I$(srcdir) $(LIBLUNAR_CFLAGS) --warn-all --identifier-prefix=Lunar
#INTROSPECTION_COMPILER_ARGS = $(LIBLUNAR_LIBS) liblunar-gtk-1.0.la

LunarCalendar-1.0.gir: liblunar-gtk-1.0.la
LunarCalendar_1_0_gir_NAMESPACE = LunarCalendar
LunarCalendar_1_0_gir_VERSION = 1.0
LunarCalendar_1_0_gir_LIBS = $(builddir)/liblunar-gtk-1.0.la
LunarCalendar_1_0_gir_FILES = $(source_h) $(source_c)
LunarCalendar_1_0_gir_INCLUDES = Gtk-2.0 LunarDate-1.0

girdir = $(datadir)/gir-1.0
dist_gir_DATA = $(INTROSPECTION_GIRS)

typelibdir = $(libdir)/girepository-1.0
typelib_DATA = $(INTROSPECTION_GIRS:.gir=.typelib)

CLEANFILES += $(dist_gir_DATA) $(typelib_DATA)
endif

-include $(top_srcdir)/git.mk
