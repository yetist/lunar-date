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
	lunar-main.h

BUILT_SOURCES = 

lib_LTLIBRARIES = liblunar-1.la

liblunar_1_includedir = $(includedir)/liblunar-1/lunar

liblunar_1_la_SOURCES =	\
	lunar-date.c		\
	lunar-main.c		\
	$(NULL)

liblunar_1_include_HEADERS =	\
	$(INST_H_FILES)		\
	$(NULL)

liblunar_1_la_LIBADD = 	\
	$(LIBLUNAR_LIBS)		\
	$(NULL)

-include $(top_srcdir)/git.mk
