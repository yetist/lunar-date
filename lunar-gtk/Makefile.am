INCLUDES =                                              \
	-I$(top_srcdir)					\
        -I$(srcdir) 					\
	$(LIBLUNAR_CFLAGS)

INST_H_FILES = lunar-calendar.h

lib_LTLIBRARIES = liblunar-gtk.la

liblunar_gtk_includedir = $(includedir)/liblunar-1/lunar-gtk

liblunar_gtk_la_SOURCES =	lunar-calendar.c

liblunar_gtk_include_HEADERS =	\
	$(INST_H_FILES)		\
	$(NULL)

liblunar_gtk_la_LIBADD = 	\
	$(LIBLUNAR_LIBS)	\
	$(NULL)

EXTRA_DIST =  lunar-enum-types.h.template lunar-enum-types.c.template

-include $(top_srcdir)/git.mk
