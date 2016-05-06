NULL =

bin_PROGRAMS = holybible
pkglibexec_PROGRAMS = holybible-daemon

common_CFLAGS = 	\
	-Wall		\
	-DLOCALE_DIR=\""$(localedir)"\" \
	-DPKG_DATA_DIR=\""$(pkgdatadir)"\"

lunar_date_daemon_CFLAGS =	\
	$(common_CFLAGS)	\
	$(GIO_CFLAGS)
	$(SQLITE_CFLAGS)

lunar_date_daemon_LDADD = $(GIO_LIBS)  $(SQLITE_LIBS)

lunar_date_daemon_SOURCES =	\
	$(lunar_date_built_sources)	\
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
	lunar-date-dbus-generated.h	\
	lunar-date-dbus-generated.c	\
	$(NULL)

$(lunar_date_built_sources) : Makefile.am $(srcdir)/org.chinese.lunar.date.xml
	$(AM_V_GEN) gdbus-codegen \
	 	--interface-prefix org.chinese. \
		--c-namespace Lunar \
		--generate-c-code lunar-date-dbus-generated \
		$(srcdir)/org.chinese.lunar.date.xml

EXTRA_DIST = 			\
	org.chinese.lunar.date.xml 	\
	$(NULL)

-include $(top_srcdir)/git.mk
