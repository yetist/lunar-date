NULL =

#bin_PROGRAMS = holybible
pkglibexec_PROGRAMS = lunar-date-daemon

common_CFLAGS = 	\
	-Wall		\
	-DLOCALE_DIR=\""$(localedir)"\" \
	-DPKG_DATA_DIR=\""$(pkgdatadir)"\"

lunar_date_daemon_CFLAGS =	\
	$(GIO_CFLAGS)				\
	$(GIO_UNIX_CFLAGS)

lunar_date_daemon_LDADD = $(GIO_LIBS)  $(GIO_UNIX_LIBS)

lunar_date_daemon_SOURCES =	\
	$(lunar_date_built_sources)	\
	date-daemon.c		\
	lunar-date-daemon.c		\
	$(NULL)

#lunar_date_CFLAGS = $(common_CFLAGS)	\
#		   $(GIO_CFLAGS)
#
#lunar_date_LDADD = $(GIO_LIBS) 
#
#lunar_date_SOURCES =	\
#	$(lunar_date_built_sources)	\
#	lunar-date.c	\
#	$(NULL)

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
