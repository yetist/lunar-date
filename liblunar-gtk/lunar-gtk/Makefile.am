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
	$(LIBLUNAR_CFLAGS)

source_h = 	\
	$(srcdir)/lunar.h		\
	$(srcdir)/lunar-calendar.h 

source_c = lunar-calendar.c

lib_LTLIBRARIES = liblunar-gtk.la liblunar-gtk-preload.la

liblunar_gtk_includedir = $(includedir)/liblunar-1/lunar-gtk

liblunar_gtk_la_SOURCES = $(source_c)

liblunar_gtk_include_HEADERS =	\
	$(source_h)		\
	$(NULL)

liblunar_gtk_la_LIBADD = 	\
	$(LIBLUNAR_LIBS)	\
	$(NULL)

liblunar_gtk_la_LDFLAGS = \
	-version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) \
	-export-dynamic $(export_symbols)


liblunar_gtk_preload_includedir = $(includedir)/liblunar-1/lunar-gtk

liblunar_gtk_preload_la_SOURCES = liblunar-gtk-preload.c

liblunar_gtk_preload_include_HEADERS =	\
	$(source_h)		\
	$(NULL)

liblunar_gtk_preload_la_LIBADD = 	\
	$(LIBLUNAR_LIBS)	\
	liblunar-gtk.la		\
	-ldl			\
	$(NULL)

liblunar_gtk_preload_la_LDFLAGS =  -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) 

if HAVE_INTROSPECTION
introspection_files = \
        $(source_h) \
        $(source_c)

LunarCalendar-1.gir: $(INTROSPECTION_SCANNER) liblunar-gtk.la Makefile
LunarCalendar_1_gir_SCANNERFLAGS = --strip-prefix=Lunar
LunarCalendar_1_gir_INCLUDES = GObject-2.0 Gtk-2.0
LunarCalendar_1_gir_CFLAGS = $(INCLUDES)
LunarCalendar_1_gir_LIBS = liblunar-gtk.la
LunarCalendar_1_gir_FILES = $(introspection_files)
INTROSPECTION_GIRS += LunarCalendar-1.gir

girdir = $(datadir)/gir-1.0
dist_gir_DATA = $(INTROSPECTION_GIRS)

typelibsdir = $(libdir)/girepository-1.0
typelibs_DATA = $(INTROSPECTION_GIRS:.gir=.typelib)

CLEANFILES += $(dist_gir_DATA) $(typelibs_DATA)
endif

-include $(top_srcdir)/git.mk
