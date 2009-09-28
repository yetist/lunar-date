INCLUDES =                                              \
	-I$(top_srcdir)					\
        -I$(srcdir) 					\
	$(LIBLUNAR_CFLAGS)

INST_H_FILES = \
	       lunar.h	\
	       lunar-calendar.h 

lib_LTLIBRARIES = liblunar-gtk.la liblunar-gtk-preload.la

liblunar_gtk_includedir = $(includedir)/liblunar-1/lunar-gtk

liblunar_gtk_la_SOURCES =	lunar-calendar.c

liblunar_gtk_include_HEADERS =	\
	$(INST_H_FILES)		\
	$(NULL)

liblunar_gtk_la_LIBADD = 	\
	$(LIBLUNAR_LIBS)	\
	$(NULL)

liblunar_gtk_preload_includedir = $(includedir)/liblunar-1/lunar-gtk

liblunar_gtk_preload_la_SOURCES = liblunar-gtk-preload.c

liblunar_gtk_preload_include_HEADERS =	\
	$(INST_H_FILES)		\
	$(NULL)

liblunar_gtk_preload_la_LIBADD = 	\
	$(LIBLUNAR_LIBS)	\
	-ldl			\
	$(NULL)

-include $(top_srcdir)/git.mk
