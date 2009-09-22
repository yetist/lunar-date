SUBDIRS = lunar-gtk tests bindings docs
EXTRA_DIST =                    \
	autogen.sh		\
	liblunar-gtk.pc.in	\
        ChangeLog


pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = liblunar-gtk.pc

DISTCHECK_CONFIGURE_FLAGS = --enable-gtk-doc

-include $(top_srcdir)/git.mk
