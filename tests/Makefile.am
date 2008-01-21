NULL=

INCLUDES =                                              \
	-I$(top_srcdir)					\
        -I$(srcdir) 					\
	$(LIBGCL_CFLAGS)				\
        -DGCL_HOLIDAYDIR=\""$(datadir)/liblunar/"\"     \
	$(NULL)

noinst_PROGRAMS =			\
        test-date			\
	test-calendar

test_date_SOURCES =          		\
        test-date.c

test_calendar_SOURCES =          		\
	test-calendar.c

AM_CPPFLAGS =                		\
        -I.                  		\
        -I$(top_srcdir)      		\
        $(LIBGCL_CFLAGS) 

LIBS =                       		\
        $(LIBGCL_LIBS)       		\
        $(top_builddir)/gcl/libgcl-1.la

