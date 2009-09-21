#SUBDIRS = src tests docs bindings
SUBDIRS = lunar-gtk tests bindings
EXTRA_DIST =                    \
	autogen.sh		\
	liblunar-gtk.pc.in	\
        ChangeLog               \
        intltool-extract.in     \
        intltool-merge.in       \
        intltool-update.in

DISTCLEANFILES =                \
        intltool-extract        \
        intltool-merge          \
        intltool-update

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = liblunar-gtk.pc

DISTCHECK_CONFIGURE_FLAGS = --enable-gtk-doc

-include $(top_srcdir)/git.mk
