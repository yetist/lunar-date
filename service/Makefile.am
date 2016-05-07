NULL =

pkglibexec_PROGRAMS = lunar-date-daemon

common_CFLAGS = -Wall

lunar_date_daemon_CFLAGS =	\
	-I.                  		\
	-I$(top_srcdir)			\
	-I$(top_srcdir)/lunar-date	\
	-I$(top_srcdir)/service		\
	$(GIO_CFLAGS)		\
	$(GIO_UNIX_CFLAGS)

lunar_date_daemon_LDADD = $(GIO_LIBS)  $(GIO_UNIX_LIBS) \
        $(top_builddir)/lunar-date/liblunar-date-3.0.la

lunar_date_daemon_SOURCES =	\
	$(lunar_date_built_sources)	\
	date-daemon.h		\
	date-daemon.c		\
	lunar-date-daemon.c	\
	$(NULL)

lunar_date_built_sources =	\
	date-dbus-generated.h	\
	date-dbus-generated.c	\
	$(NULL)

$(lunar_date_built_sources) : Makefile.am $(srcdir)/org.chinese.lunar.date.xml
	$(AM_V_GEN) gdbus-codegen \
	 	--interface-prefix org.chinese. \
		--c-namespace Bus \
		--generate-c-code date-dbus-generated \
		$(srcdir)/org.chinese.lunar.date.xml

EXTRA_DIST = 			\
	org.chinese.lunar.date.xml 	\
	$(NULL)

-include $(top_srcdir)/git.mk
