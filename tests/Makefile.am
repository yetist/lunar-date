include $(top_srcdir)/Makefile.decl

AM_CPPFLAGS =                             \
        -I.                  		\
	-I$(top_srcdir)				\
	-I$(top_srcdir)/lunar-date		\
        -I$(srcdir) 				\
	$(GIO_CFLAGS)


TEST_PROGS += $(noinst_PROGRAMS)
noinst_PROGRAMS = testing
testing_SOURCES  = testing.c
testing_LDADD    = $(GIO_LIBS)       		\
		    $(top_builddir)/lunar-date/liblunar-date-3.0.la

-include $(top_srcdir)/git.mk
