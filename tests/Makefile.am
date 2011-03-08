NULL=

INCLUDES =                                              \
	-I$(top_srcdir)					\
	-I$(top_srcdir)/src				\
        -I$(srcdir) 					\
	$(LIBLUNAR_CFLAGS)				\
	$(NULL)

noinst_PROGRAMS = test-calendar vala-calendar

test_calendar_SOURCES =  test-calendar.c

AM_CPPFLAGS =                		\
        -I.                  		\
        -I$(top_srcdir)      		\
        $(LIBLUNAR_CFLAGS) 

LIBS =                       		\
        $(LIBLUNAR_LIBS)       		\
        $(top_builddir)/lunar-gtk/liblunar-gtk-1.0.la

#BUILT_SOURCES = vala-calendar.vala.stamp

vala_calendar_VALASOURCES = calendar.vala
vala_calendar_SOURCES = \
	$(vala_calendar_VALASOURCES:.vala=.c) \
	$(NULL)

vala-calendar.vala.stamp: $(vala_calendar_VALASOURCES)
	$(VALAC) -c --pkg liblunar-gtk --save-temps --basedir $(top_srcdir) $^
	touch $@

EXTRA_DIST = \
	$(vala_calendar_VALASOURCES) \
	vala-calendar.vala.stamp \
	$(NULL)

-include $(top_srcdir)/git.mk
