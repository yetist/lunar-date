NULL=

INCLUDES =                                              \
	-I$(top_srcdir)					\
	-I$(top_srcdir)/lunar				\
        -I$(srcdir) 					\
	$(LIBLUNAR_CFLAGS)				\
        -DLUNAR_HOLIDAYDIR=\""$(datadir)/liblunar/"\"     \
	$(NULL)

noinst_PROGRAMS =test-date vala-date

test_date_SOURCES =          		\
        test-date.c

AM_CPPFLAGS =                		\
        -I.                  		\
        -I$(top_srcdir)      		\
        $(LIBLUNAR_CFLAGS) 

LIBS =                       		\
        $(LIBLUNAR_LIBS)       		\
        $(top_builddir)/lunar/liblunar-1.la

#BUILT_SOURCES = vala-date.vala.stamp

vala_date_VALASOURCES = date.vala
vala_date_SOURCES = \
	$(vala_date_VALASOURCES:.vala=.c) \
	$(NULL)

vala-date.vala.stamp: $(vala_date_VALASOURCES)
	$(VALAC) -c --pkg liblunar-1 --save-temps --basedir $(top_srcdir) $^
	touch $@

EXTRA_DIST = \
	$(vala_date_VALASOURCES) \
	vala-date.vala.stamp \
	$(NULL)

-include $(top_srcdir)/git.mk
