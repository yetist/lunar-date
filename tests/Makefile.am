NULL=

INCLUDES =                                              \
	-I$(top_srcdir)					\
	-I$(top_srcdir)/src				\
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
        $(top_builddir)/lunar-gtk/liblunar-gtk.la

-include $(top_srcdir)/git.mk
