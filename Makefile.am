SUBDIRS = po gcl docs tests

EXTRA_DIST =                    \
	autogen.sh		\
	libgcl-1.pc.in		\
        ChangeLog               \
        intltool-extract.in     \
        intltool-merge.in       \
        intltool-update.in

DISTCLEANFILES =                \
        intltool-extract        \
        intltool-merge          \
        intltool-update

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libgcl-1.pc

DISTCHECK_CONFIGURE_FLAGS =     \
	--enable-gtk-doc
