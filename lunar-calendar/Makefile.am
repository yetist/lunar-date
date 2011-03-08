export_symbols = $(LIBTOOL_EXPORT_OPTIONS)

CLEANFILES =

INCLUDES =                                              \
	-I$(top_srcdir)					\
        -I$(srcdir) 					\
	$(LIBLUNAR_CFLAGS)				\
	$(WARN_CFLAGS)

source_h = lunar-calendar.h 
source_c = lunar-calendar.c

lib_LTLIBRARIES = liblunar-calendar-2.0.la liblunar-calendar-preload.la

lunarcalendardir=$(includedir)/liblunar-2.0
lunarcalendar_HEADERS = calendar.h

liblunar_calendar_2_0_includedir = $(includedir)/liblunar-2.0/lunar-calendar
liblunar_calendar_2_0_include_HEADERS =	$(source_h)
liblunar_calendar_2_0_la_SOURCES = $(source_c)
liblunar_calendar_2_0_la_LIBADD = $(LIBLUNAR_LIBS)
liblunar_calendar_2_0_la_LDFLAGS = \
	-version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) \
	-export-dynamic $(export_symbols)

liblunar_calendar_preload_includedir = $(includedir)/liblunar-2.0/lunar-calendar
liblunar_calendar_preload_include_HEADERS = $(source_h)
liblunar_calendar_preload_la_SOURCES = lunar-calendar-preload.c
liblunar_calendar_preload_la_LDFLAGS =  -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) 
liblunar_calendar_preload_la_LIBADD = 	\
	$(LIBLUNAR_LIBS)	\
	liblunar-calendar-2.0.la		\
	-ldl			\
	$(NULL)

if HAVE_INTROSPECTION
-include $(INTROSPECTION_MAKEFILE)
INTROSPECTION_GIRS = LunarCalendar-2.0.gir
INTROSPECTION_SCANNER_ARGS = -I$(top_srcdir) -I$(srcdir) $(LIBLUNAR_CFLAGS) --warn-all --identifier-prefix=Lunar
#INTROSPECTION_COMPILER_ARGS = $(LIBLUNAR_LIBS) liblunar-calendar-2.0.la

LunarCalendar-2.0.gir: liblunar-calendar-2.0.la
LunarCalendar_2_0_gir_NAMESPACE = LunarCalendar
LunarCalendar_2_0_gir_VERSION = 2.0
LunarCalendar_2_0_gir_LIBS = $(builddir)/liblunar-calendar-2.0.la
LunarCalendar_2_0_gir_FILES = $(source_h) $(source_c)
LunarCalendar_2_0_gir_INCLUDES = Gtk-2.0 LunarDate-2.0

girdir = $(datadir)/gir-1.0
dist_gir_DATA = $(INTROSPECTION_GIRS)

typelibdir = $(libdir)/girepository-1.0
typelib_DATA = $(INTROSPECTION_GIRS:.gir=.typelib)

CLEANFILES += $(dist_gir_DATA) $(typelib_DATA)
endif

-include $(top_srcdir)/git.mk
