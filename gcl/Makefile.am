NULL=

AM_CPPFLAGS = 			\
	-I.			\
	-I$(top_srcdir)		\
	$(LIBGCL_CFLAGS)		\
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
