NULL=

INCLUDES =                                              \
	-I$(top_srcdir)					\
        -I$(srcdir) 					\
	$(LIBLUNAR_CFLAGS)				\
        -DLUNAR_HOLIDAYDIR=\""$(datadir)/liblunar/"\"     \
	$(NULL)

if ENABLE_GTK
INCLUDES += -DLIBLUNAR_ENABLE_GTK
endif
INST_H_FILES = 		\
	lunar.h			\
	lunar-date.h		\
	lunar-main.h

BUILT_SOURCES = 

if ENABLE_GTK
INST_H_FILES += 		\
	lunar-calendar.h

BUILT_SOURCES +=         	\
        lunar-enum-types.c        \
        lunar-enum-types.h
endif

lib_LTLIBRARIES = liblunar-1.la

liblunar_1_includedir = $(includedir)/liblunar-1/lunar

liblunar_1_la_SOURCES =	\
	lunar-date.c		\
	lunar-main.c		\
	$(BUILT_SOURCES)	\
	$(NULL)

if ENABLE_GTK
liblunar_1_la_SOURCES +=	\
	lunar-calendar.c
endif

liblunar_1_include_HEADERS =	\
	$(INST_H_FILES)		\
	$(NULL)

liblunar_1_la_LIBADD = 	\
	$(LIBLUNAR_LIBS)		\
	$(NULL)

if ENABLE_GTK
lunar-enum-types.h: lunar-enum-types.h.template $(INST_H_FILES) $(GLIB_MKENUMS)
	(cd $(srcdir) && $(GLIB_MKENUMS) --template lunar-enum-types.h.template $(INST_H_FILES)) > $@
        
lunar-enum-types.c: lunar-enum-types.c.template $(INST_H_FILES) $(GLIB_MKENUMS)
	(cd $(srcdir) && $(GLIB_MKENUMS) --template lunar-enum-types.c.template $(INST_H_FILES)) > $@
endif


EXTRA_DIST =  lunar-enum-types.h.template lunar-enum-types.c.template
