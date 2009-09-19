ACLOCAL_AMFLAGS = -I m4
SUBDIRS = po lunar data tests docs bindings

EXTRA_DIST =                    \
	autogen.sh		\
	liblunar-1.pc.in		\
        ChangeLog               \
        intltool-extract.in     \
        intltool-merge.in       \
        intltool-update.in

DISTCLEANFILES =                \
        intltool-extract        \
        intltool-merge          \
        intltool-update

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = liblunar-1.pc

DISTCHECK_CONFIGURE_FLAGS = --enable-gtk-doc

dist-hook:
	$(MAKE) -C "$(srcdir)/po" "$(GETTEXT_PACKAGE).pot"
	cp "$(srcdir)/po/$(GETTEXT_PACKAGE).pot" "$(distdir)/po/"

-include $(top_srcdir)/git.mk
