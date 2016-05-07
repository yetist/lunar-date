if HAVE_VALA
bin_PROGRAMS = example-vala

AM_CPPFLAGS =	\
	-I.                  		\
	-I$(top_srcdir)			\
	-I$(top_srcdir)/lunar-date	\
	-I$(top_srcdir)/example 	\
	-include config.h	\
	$(GIO_CFLAGS)

AM_VALAFLAGS = \
	--vapidir=$(top_srcdir)/vapi --pkg config \
	--pkg glib-2.0 \
	--pkg lunar-date-3.0

example_vala_SOURCES = \
	example.vala

example_vala_LDADD = \
	$(GIO_LIBS) \
        $(top_builddir)/lunar-date/liblunar-date-3.0.la
endif

EXTRA_DIST 	=  \
	example.js	\
	example.py	\
	example.vala

DISTCLEANFILES = example.c

-include $(top_srcdir)/git.mk
