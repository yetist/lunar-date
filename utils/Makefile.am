NULL=

AM_CPPFLAGS =                                           \
        -I.                  		\
	-I$(top_srcdir)					\
	-I$(top_srcdir)/lunar-date				\
        -I$(srcdir) 					\
	$(LUNAR_DATE_CFLAGS)				\
	$(NULL)

noinst_PROGRAMS =test-date

test_date_SOURCES = test-date.c


LIBS =                       		\
        $(LUNAR_DATE_LIBS)       		\
        $(top_builddir)/lunar-date/liblunar-date-3.0.la

-include $(top_srcdir)/git.mk
