NULL=

export_symbols = $(LIBTOOL_EXPORT_OPTIONS)

INCLUDES =                                              \
	-I$(top_srcdir)					\
        -I$(srcdir) 					\
	$(LIBLUNAR_CFLAGS)				\
        -DLUNAR_HOLIDAYDIR=\""$(datadir)/liblunar/"\"     \
	$(NULL)

INST_H_FILES = 		\
	lunar.h			\
	lunar-date.h		\
	lunar-config.h		\
	lunar-main.h

BUILT_SOURCES = 

if ENABLE_PYTHON
BUILT_SOURCES +=         	\
        lunar-enum-types.c        \
        lunar-enum-types.h
endif

lib_LTLIBRARIES = liblunar-1.la

liblunar_1_includedir = $(includedir)/liblunar-1/lunar

liblunar_1_la_SOURCES =	\
	lunar-date.c		\
	lunar-date-private.c	\
	lunar-main.c		\
	$(NULL)

liblunar_1_include_HEADERS =	\
	$(INST_H_FILES)		\
	$(NULL)

liblunar_1_la_LIBADD = 	\
	$(LIBLUNAR_LIBS)	\
	$(NULL)

liblunar_1_la_LDFLAGS = \
	-version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) \
	-export-dynamic $(export_symbols)

if ENABLE_PYTHON
lunar-enum-types.h: lunar-enum-types.h.template $(INST_H_FILES) $(GLIB_MKENUMS)
	(cd $(srcdir) && $(GLIB_MKENUMS) --template lunar-enum-types.h.template $(INST_H_FILES)) > $@
        
lunar-enum-types.c: lunar-enum-types.c.template $(INST_H_FILES) $(GLIB_MKENUMS)
	(cd $(srcdir) && $(GLIB_MKENUMS) --template lunar-enum-types.c.template $(INST_H_FILES)) > $@
DISTCLEANFILES = lunar-enum-types.h lunar-enum-types.c
endif

EXTRA_DIST =  lunar-enum-types.h.template lunar-enum-types.c.template lunar-date-private.h lunar-config.h.in

-include $(top_srcdir)/git.mk
