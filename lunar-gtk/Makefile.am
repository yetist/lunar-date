export_symbols = $(LIBTOOL_EXPORT_OPTIONS)

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
lunarcalendar.gir: $(INTROSPECTION_SCANNER) liblunar-gtk.la
	$(AM_V_GEN) $(INTROSPECTION_SCANNER) -v \
        --namespace lunarcalendar --nsversion=1 \
        $(INCLUDES) \
        --c-include='lunar-gtk/lunar.h' \
        --include=GObject-2.0 \
	--include=Gtk+-2.0 \
        --library=liblunar-gtk.la \
        --libtool="$(top_builddir)/libtool" \
        --pkg gobject-2.0 \
        --pkg gtk+-2.0 \
        --output $@ \
        $(source_h) \
        $(source_c)

BUILT_GIRSOURCES = lunarcalendar.gir

girdir = $(datadir)/gir-1.0
gir_DATA = $(BUILT_GIRSOURCES)

lunarcalendar.typelib: lunarcalendar.gir

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

-include $(top_srcdir)/git.mk
