NULL=

INCLUDES =                                              \
	-I$(top_srcdir)					\
	-I$(top_srcdir)/lunar-calendar			\
        -I$(srcdir) 					\
	$(LIBLUNAR_CFLAGS)				\
	$(NULL)

noinst_PROGRAMS = test-calendar

test_calendar_SOURCES =  test-calendar.c

AM_CPPFLAGS =                		\
        -I.                  		\
        -I$(top_srcdir)      		\
        $(LIBLUNAR_CFLAGS) 

LIBS =                       		\
        $(LIBLUNAR_LIBS)       		\
        $(top_builddir)/lunar-calendar/liblunar-calendar-2.0.la

-include $(top_srcdir)/git.mk
