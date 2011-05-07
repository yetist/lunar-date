NULL=

INCLUDES =                                              \
	-I$(top_srcdir)					\
	-I$(top_srcdir)/lunar-date				\
        -I$(srcdir) 					\
	$(LUNAR_DATE_CFLAGS)				\
        -DLUNAR_HOLIDAYDIR=\""$(datadir)/liblunar/"\"     \
	$(NULL)

noinst_PROGRAMS =test-date

test_date_SOURCES = test-date.c

AM_CPPFLAGS =                		\
        -I.                  		\
        -I$(top_srcdir)      		\
        $(LUNAR_DATE_CFLAGS) 

LIBS =                       		\
        $(LUNAR_DATE_LIBS)       		\
        $(top_builddir)/lunar-date/liblunar-date-2.0.la

-include $(top_srcdir)/git.mk
