NULL=

INCLUDES =                                              \
	-I$(top_srcdir)					\
        -I$(srcdir) 					\
	$(LIBLUNAR_CFLAGS)				\
        -DLUNAR_HOLIDAYDIR=\""$(datadir)/liblunar/"\"     \
	$(NULL)

INST_H_FILES = 		\
	lunar.h			\
	lunar-date.h		\
	lunar-calendar.h		\
	lunar-main.h

BUILT_SOURCES =         	\
        lunar-enum-types.c        \
        lunar-enum-types.h

lib_LTLIBRARIES = liblunar-1.la

liblunar_1_includedir = $(includedir)/liblunar-1/lunar

liblunar_1_la_SOURCES =	\
	lunar-date.c		\
	lunar-calendar.c		\
	lunar-main.c		\
	$(BUILT_SOURCES)	\
	$(NULL)

liblunar_1_include_HEADERS =	\
	$(INST_H_FILES)		\
	$(NULL)

liblunar_1_la_LIBADD = 	\
	$(LIBLUNAR_LIBS)		\
	$(NULL)

lunar-enum-types.h: lunar-enum-types.h.template $(INST_H_FILES) $(GLIB_MKENUMS)
	(cd $(srcdir) && $(GLIB_MKENUMS) --template lunar-enum-types.h.template $(INST_H_FILES)) > $@
        
lunar-enum-types.c: lunar-enum-types.c.template $(INST_H_FILES) $(GLIB_MKENUMS)
	(cd $(srcdir) && $(GLIB_MKENUMS) --template lunar-enum-types.c.template $(INST_H_FILES)) > $@


EXTRA_DIST =  lunar-enum-types.h.template lunar-enum-types.c.template
