NULL=

INCLUDES =                                              \
	-I$(top_srcdir)					\
        -I$(srcdir) 					\
	$(LIBGCL_CFLAGS)				\
        -DGCL_HOLIDAYDIR=\""$(datadir)/libgcl/"\"     \
	$(NULL)

INST_H_FILES = 		\
	gcl.h			\
	gcl-date.h		\
	gcl-calendar.h		\
	gcl-main.h

BUILT_SOURCES =         	\
        gcl-enum-types.c        \
        gcl-enum-types.h

lib_LTLIBRARIES = libgcl-1.la

libgcl_1_includedir = $(includedir)/libgcl-1/gcl

libgcl_1_la_SOURCES =	\
	gcl-date.c		\
	gcl-calendar.c		\
	gcl-main.c		\
	$(BUILT_SOURCES)	\
	$(NULL)

libgcl_1_include_HEADERS =	\
	$(INST_H_FILES)		\
	$(NULL)

libgcl_1_la_LIBADD = 	\
	$(LIBGCL_LIBS)		\
	$(NULL)

gcl-enum-types.h: gcl-enum-types.h.template $(INST_H_FILES) $(GLIB_MKENUMS)
	(cd $(srcdir) && $(GLIB_MKENUMS) --template gcl-enum-types.h.template $(INST_H_FILES)) > $@
        
gcl-enum-types.c: gcl-enum-types.c.template $(INST_H_FILES) $(GLIB_MKENUMS)
	(cd $(srcdir) && $(GLIB_MKENUMS) --template gcl-enum-types.c.template $(INST_H_FILES)) > $@


EXTRA_DIST = 
