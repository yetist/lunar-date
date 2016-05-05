bin_PROGRAMS = example-vala

AM_CPPFLAGS =	\
	-I.                  		\
	-I$(top_srcdir)			\
	-I$(top_srcdir)/lunar-date	\
	-I$(srcdir) 			\
	-include config.h	\
	$(LUNAR_DATE_CFLAGS)

AM_VALAFLAGS = \
	--vapidir=$(srcdir)/../vapi --pkg config \
	--pkg glib-2.0 \
	--pkg lunar-date-3.0

example_vala_SOURCES = \
	example.vala

example_vala_LDADD = \
	$(LUNAR_DATE_LIBS) \
        $(top_builddir)/lunar-date/liblunar-date-3.0.la

DISTCLEANFILES = example.c

-include $(top_srcdir)/git.mk
