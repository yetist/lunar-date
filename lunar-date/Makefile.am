CLEANFILES =

AM_CPPFLAGS =                                          \
	-I$(top_srcdir)					\
        -I$(srcdir) 					\
	$(LUNAR_DATE_CFLAGS)				\
	$(WARN_CFLAGS)					\
        -DLUNAR_HOLIDAYDIR=\""$(datadir)/lunar-date/"\"

if PLATFORM_WIN32
no_undefined = -no-undefined
endif

if OS_WIN32
lunar_date_def = lunar-date.def
lunar_date_win32_symbols = -export-symbols $(srcdir)/lunar-date.def

lunar_date_win32_res = lunar-date-win32-res.o
lunar_date_win32_res_ldflag = -Wl,lunar-date-win32-res.o

lunar-date-win32-res.o : lunar-date-win32.rc
	$(WINDRES) lunar-date-win32.rc $@

install-def-file: lunar-date.def
	$(INSTALL) $(srcdir)/lunar-date.def $(DESTDIR)$(libdir)/lunar-date-win32-3.0.def

uninstall-def-file:
	-rm $(DESTDIR)$(libdir)/lunar-date-win32-3.0.def
else
install-def-file:
uninstall-def-file:
endif

if MS_LIB_AVAILABLE
noinst_DATA = lunar-date-3.0.lib

lunar-date-3.0.lib: liblunar-date-3.0.la lunar-date.def
	lib -machine:@LIB_EXE_MACHINE_FLAG@ -name:liblunar-date-3.0-$(LT_CURRENT_MINUS_AGE).dll -def:lunar-date.def -out:$@

install_ms_lib:
	$(INSTALL) lunar-date-3.0.lib $(DESTDIR)$(libdir)

uninstall_ms_lib:
	-rm $(DESTDIR)$(libdir)/lunar-date-3.0.lib
else
install-ms-lib:
uninstall-ms-lib:
endif

# This places the generated .def file in srcdir, since it is expected to be there.
# (The one from a tarball is)
lunar-date.def: lunar-date.symbols
	$(AM_V_GEN) (echo -e EXPORTS; $(CPP) -P -DINCLUDE_VARIABLES -DINCLUDE_INTERNAL_SYMBOLS -DG_OS_WIN32 -DALL_FILES - <$(srcdir)/lunar-date.symbols | sed -e '/^$$/d' -e 's/^/	/' -e 's/G_GNUC_[^ ]*//g') > $(srcdir)/lunar-date.def

deps =

libtool_opts =							\
  -version-info $(LT_VERSION_INFO)				\
  -export-dynamic $(no_undefined) $(LIBTOOL_EXPORT_OPTIONS)	\
  -rpath $(libdir) $(liblunar_date_target_ldflags)

source_h = 	\
	$(srcdir)/lunar-date.h		\
	$(srcdir)/lunar-version.h

source_c =	\
	$(srcdir)/lunar-date.c		\
	$(srcdir)/lunar-version.c	\
	$(srcdir)/lunar-date-private.c

BUILT_SOURCES =         	\
        lunar-date-enum-types.h		\
        lunar-date-enum-types.c		\
        lunar-date-resources.h		\
        lunar-date-resources.c

lib_LTLIBRARIES = liblunar-date-3.0.la

liblunar_date_3_0_includedir = $(includedir)/liblunar-3.0/lunar-date
liblunar_date_3_0_include_HEADERS = $(source_h)

liblunar_date_3_0_la_SOURCES =	$(source_c) $(BUILT_SOURCES)
liblunar_date_3_0_la_LIBADD = 	$(LUNAR_DATE_LIBS)
liblunar_date_3_0_la_LDFLAGS = $(libtool_opts)
liblunar_date_3_0_la_DEPENDENCIES = $(deps)

if OS_WIN32
liblunar_date_target_ldflags = $(lunar_date_win32_res_ldflag) $(lunar_date_win32_symbols)
liblunar_date_3_0_la_DEPENDENCIES += $(lunar_date_def) $(lunar_date_win32_res)
endif

install-data-local: install-ms-lib install-def-file

lunar-date-enum-types.h: lunar-date-enum-types.h.template $(source_h) $(GLIB_MKENUMS)
	$(AM_V_GEN) (cd $(srcdir) && $(GLIB_MKENUMS) --template lunar-date-enum-types.h.template $(source_h)) > $@
        
lunar-date-enum-types.c: lunar-date-enum-types.c.template $(source_h) $(GLIB_MKENUMS)
	$(AM_V_GEN) (cd $(srcdir) && $(GLIB_MKENUMS) --template lunar-date-enum-types.c.template $(source_h)) > $@

lunar-date-resources.c: lunar-date.gresource.xml $(shell $(GLIB_COMPILE_RESOURCES) --sourcedir=$(srcdir) --generate-dependencies $(srcdir)/lunar-date.gresource.xml)
	$(AM_V_GEN) $(GLIB_COMPILE_RESOURCES) --sourcedir=$(srcdir) --target=$@ --generate-source --c-name lunar_date $(srcdir)/lunar-date.gresource.xml

lunar-date-resources.h: lunar-date.gresource.xml $(shell $(GLIB_COMPILE_RESOURCES) --sourcedir=$(srcdir) --generate-dependencies $(srcdir)/lunar-date.gresource.xml)
	$(AM_V_GEN) $(GLIB_COMPILE_RESOURCES) --sourcedir=$(srcdir) --target=$@ --generate-header --c-name lunar_date $(srcdir)/lunar-date.gresource.xml

DISTCLEANFILES = lunar-date-enum-types.h lunar-date-enum-types.c lunar-date-resources.h lunar-date-resources.c

if HAVE_INTROSPECTION
-include $(INTROSPECTION_MAKEFILE)
INTROSPECTION_GIRS = LunarDate-3.0.gir
#INTROSPECTION_COMPILER_ARGS - Additional args to pass in to the compiler

LunarDate-3.0.gir: liblunar-date-3.0.la
INTROSPECTION_SCANNER_ARGS = -I$(top_srcdir) -I$(srcdir) --warn-all --identifier-prefix=Lunar
LunarDate_3_0_gir_NAMESPACE = LunarDate
LunarDate_3_0_gir_VERSION = 3.0
LunarDate_3_0_gir_LIBS = $(builddir)/liblunar-date-3.0.la
LunarDate_3_0_gir_FILES = $(source_h) $(source_c) $(BUILT_SOURCES)
LunarDate_3_0_gir_INCLUDES = GObject-2.0

girdir = $(datadir)/gir-1.0
dist_gir_DATA = $(INTROSPECTION_GIRS)

typelibdir = $(libdir)/girepository-1.0
typelib_DATA = $(INTROSPECTION_GIRS:.gir=.typelib)

CLEANFILES += $(dist_gir_DATA) $(typelib_DATA)
endif

EXTRA_DIST =  lunar-date-enum-types.h.template \
	      lunar-date-enum-types.c.template \
	      lunar-date-private.h \
	      lunar-version.h.in \
	      lunar-date.gresource.xml \
	      lunar-date-win32.rc.in \
	      lunar-date.symbols

-include $(top_srcdir)/git.mk
