bin_PROGRAMS = example-vala

INCLUDES = \
        -I.                  		\
	-I$(top_srcdir)					\
	-I$(top_srcdir)/lunar-date				\
        -I$(srcdir) 					\
	-include config.h \
	$(LUNAR_DATE_CFLAGS)

VALAFLAGS = \
	--vapidir=$(srcdir) --pkg config \
	--pkg glib-2.0 \
	--pkg lunar-date-3.0

example_vala_SOURCES = \
	example.vala

example_vala_LDADD = \
	$(LUNAR_DATE_LIBS) \
        $(top_builddir)/lunar-date/liblunar-date-3.0.la

