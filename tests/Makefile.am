NULL=

INCLUDES =                                              \
	-I$(top_srcdir)					\
	-I$(top_srcdir)/lunar				\
        -I$(srcdir) 					\
	$(LIBLUNAR_CFLAGS)				\
        -DLUNAR_HOLIDAYDIR=\""$(datadir)/liblunar/"\"     \
	$(NULL)

noinst_PROGRAMS =test-date

test_date_SOURCES = test-date.c

AM_CPPFLAGS =                		\
        -I.                  		\
        -I$(top_srcdir)      		\
        $(LIBLUNAR_CFLAGS) 

LIBS =                       		\
        $(LIBLUNAR_LIBS)       		\
        $(top_builddir)/lunar/liblunar-1.0.la

-include $(top_srcdir)/git.mk
