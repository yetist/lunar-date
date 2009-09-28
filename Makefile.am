SUBDIRS = lunar-gtk tests bindings docs
EXTRA_DIST =                    \
	liblunar-gtk.pc.in	\
	sanity_check		\
	xmldocs.make 		\
	omf.make

MAINTAINERCLEANFILES = 		\
	aclocal.m4		\
	config.guess		\
	config.h.in		\
	config.sub		\
	depcomp			\
	gtk-doc.make		\
	install-sh		\
	ltmain.sh		\
	missing			\
	mkinstalldirs		\
	omf.make		\
	py-compile		\
	xmldocs.make		\
	m4/gtk-doc.m4		\
	m4/intltool.m4		\
	m4/libtool.m4		\
	m4/ltoptions.m4		\
	m4/ltsugar.m4		\
	m4/ltversion.m4		\
	m4/lt~obsolete.m4	\
	`find "$(srcdir)" -type f -name Makefile.in -print`

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = liblunar-gtk.pc

DISTCHECK_CONFIGURE_FLAGS = --enable-gtk-doc

.PHONY: release sanity snapshot snapcheck ChangeLog

release:
	googlecode_upload.py -s $(distdir).tar.gz -p liblunar -u yetist@gmail.com -l Featured,Type-Source,OpSys-Linux $(distdir).tar.gz

sanity:
	./sanity_check $(VERSION)

snapshot:
	$(MAKE) dist distdir=$(PACKAGE)-snap`date +"%Y%m%d"`

snapcheck:
	$(MAKE) distcheck distdir=$(PACKAGE)-snap`date +"%Y%m%d"`

CHANGELOG_START = tags/liblunar-2.0.1

ChangeLog:
	@echo Creating $@
	@if test -d "$(srcdir)/../.git"; then \
          (./missing --run git log $(CHANGELOG_START)^^.. --stat $(srcdir) ) | fmt --split-only > $@.tmp \
          && mv -f $@.tmp $@ \
          || ($(RM) $@.tmp; \
              echo Failed to generate ChangeLog, your ChangeLog may be outdated >&2; \
              (test -f $@ || echo git-log is required to generate this file >> $@)); \
        else \
          test -f $@ || \
          (echo A git checkout and git-log is required to generate ChangeLog >&2 && \
          echo A git checkout and git-log is required to generate this file >> $@); \
        fi

dist-hook:
	if test -f $(srcdir)/INSTALL.in && test -f $(srcdir)/README.in ; then \
		CONFIG_FILES="INSTALL:$(srcdir)/INSTALL.in README:$(srcdir)/README.in" \
		CONFIG_HEADERS= \
		$(SHELL) config.status \
		&& cp INSTALL README $(distdir) ; \
	fi

distclean-local:
	if test "$(srcdir)" = "."; then :; else \
                rm -f ChangeLog; \
        fi

-include $(top_srcdir)/git.mk
