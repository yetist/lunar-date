## Process this file with automake to produce Makefile.in

# see /gtk-doc/examples/Makefile.am

# The name of the module, e.g. 'glib'.
DOC_MODULE=lunar-date

# The top-level SGML file. You can change this if you want to.
DOC_MAIN_SGML_FILE=$(DOC_MODULE)-docs.xml

# The directory containing the source code. Relative to $(srcdir).
# gtk-doc will search all .c & .h files beneath here for inline comments
# documenting the functions and macros.
DOC_SOURCE_DIR=$(top_srcdir)/lunar-date

# Extra options to pass to gtkdoc-scangobj. Not normally needed.
SCANGOBJ_OPTIONS=

# Extra options to supply to gtkdoc-scan.
# e.g. SCAN_OPTIONS=--deprecated-guards="GTK_DISABLE_DEPRECATED" 
SCAN_OPTIONS=--deprecated-guards="GLIB_DEPRECATED"

# Extra options to supply to gtkdoc-mkdb.
MKDB_OPTIONS=--xml-mode --output-format=xml

# Extra options to supply to gtkdoc-mktmpl
# e.g. MKTMPL_OPTIONS=--only-section-tmpl
MKTMPL_OPTIONS=

# Extra options to supply to gtkdoc-fixref. Not normally needed.
# e.g. FIXXREF_OPTIONS=--extra-dir=../gdk-pixbuf/html --extra-dir=../gdk/html
FIXXREF_OPTIONS=

# Used for dependencies. The docs will be rebuilt if any of these change.
HFILE_GLOB=$(top_srcdir)/lunar-date/*.h
CFILE_GLOB=$(top_srcdir)/lunar-date/*.c

# Extra header to include when scanning, which are not under DOC_SOURCE_DIR
# # e.g. EXTRA_HFILES=$(top_srcdir}/contrib/extra.h
EXTRA_HFILES=

# Header files to ignore when scanning (These are internal to gedit).
IGNORE_HFILES= 

# These files are not part of gedit but were copied into it for some reason.
# Do not parse them to make the docs.


# Images to copy into HTML directory.
HTML_IMAGES=

# Extra files that are included by $(DOC_MAIN_SGML_FILE).
# e.g. content_files=running.xml building.xml changes-2.0.xml
content_files=

# Files where gtk-doc abbrevations (#GtkWidget) are expanded
# e.g. expand_content_files=running.xml
expand_content_files=

# CFLAGS and LDFLAGS for compiling gtkdoc-scangobj with your library.
# Only needed if you are using gtkdoc-scangobj to dynamically query widget
# signals and properties.
#
GTKDOC_CFLAGS =                                               \
        $(LUNAR_CFLAGS)                                  \
        -I$(top_srcdir)/lunar-date                        \
        -I$(top_builddir)/lunar-date			\
	$(GIO_CFLAGS)				\
        $(AM_CFLAGS)

GTKDOC_LIBS=                                            \
        $(top_builddir)/lunar-date/liblunar-date-3.0.la		\
        $(GIO_LIBS)

# This includes the standard gtk-doc make rules, copied by gtkdocize.
include $(top_srcdir)/gtk-doc.make

# Other files to distribute
# e.g. EXTRA_DIST += version.xml.in
EXTRA_DIST += version.xml.in

# Comment this out if you want 'make check' to test you doc status
# and run some sanity checks
if ENABLE_GTK_DOC
TESTS_ENVIRONMENT = \
  DOC_MODULE=$(DOC_MODULE) DOC_MAIN_SGML_FILE=$(DOC_MAIN_SGML_FILE) \
  SRCDIR=$(abs_srcdir) BUILDDIR=$(abs_builddir)
TESTS = $(GTKDOC_CHECK)
endif

-include $(top_srcdir)/git.mk
