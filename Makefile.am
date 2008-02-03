ACLOCAL_AMFLAGS = -I m4
SUBDIRS = po gcl docs tests data bindings

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

dist-hook:
	for file in `find debian/ -type f -not -wholename 'debian/.svn*'`; \
	do \
	    path=$${file%/*};\
	    ddir=$(distdir)/$$path;\
	    if [ ! -d $$ddir ]; then \
	    	mkdir -p $$ddir;\
	    fi  ; \
	    cp $$file $$ddir; \
	done
