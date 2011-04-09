NULL=

INCLUDES =                                              \
	-I$(top_srcdir)					\
	-I$(top_srcdir)/lunar-calendar			\
        -I$(srcdir) 					\
	$(LUNAR_CALENDAR_CFLAGS)				\
	$(NULL)

noinst_PROGRAMS = test-calendar

test_calendar_SOURCES =  test-calendar.c

AM_CPPFLAGS =                		\
        -I.                  		\
        -I$(top_srcdir)      		\
        $(LUNAR_CALENDAR_CFLAGS) 

LIBS =                       		\
        $(LUNAR_CALENDAR_LIBS)       		\
        $(top_builddir)/lunar-calendar/liblunar-calendar-2.0.la

-include $(top_srcdir)/git.mk
