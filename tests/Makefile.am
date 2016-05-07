NULL=

AM_CPPFLAGS =                                           \
        -I.                  		\
	-I$(top_srcdir)					\
	-I$(top_srcdir)/lunar-date				\
        -I$(srcdir) 					\
	$(GIO_CFLAGS)				\
        -DLUNAR_HOLIDAYDIR=\""$(datadir)/liblunar/"\"     \
	$(NULL)

noinst_PROGRAMS =test-date

test_date_SOURCES = test-date.c


LIBS =                       		\
        $(GIO_LIBS)       		\
        $(top_builddir)/lunar-date/liblunar-date-3.0.la

-include $(top_srcdir)/git.mk
