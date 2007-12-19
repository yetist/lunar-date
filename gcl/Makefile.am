NULL=

INCLUDES =                                              \
	-I$(top_srcdir)					\
        -I$(srcdir) 					\
	$(LIBGCL_CFLAGS)				\
        -DGCL_HOLIDAYDIR=\""$(datadir)/liblunar/"\"     \
	$(NULL)

lib_LTLIBRARIES = libgcl-1.la

libgcl_1_includedir = $(includedir)/libgcl-1/gcl
libgcl_1_la_SOURCES =	\
	gcl-date.c		\
	gcl-main.c		\
	$(NULL)

libgcl_1_include_HEADERS =	\
	gcl.h			\
	gcl-date.h		\
	gcl-main.h		\
	$(NULL)

libgcl_1_la_LIBADD = 	\
	$(LIBGCL_LIBS)		\
	$(NULL)

EXTRA_DIST = 
