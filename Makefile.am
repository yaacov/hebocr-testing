## Makefile.am -- Process this file with automake to produce Makefile.in
SUBDIRS = src tests
ACLOCAL_AMFLAGS = -I m4
EXTRA_DIST = Doxyfile

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = hebocr.pc

hebocrdocdir = ${prefix}/share/doc/hebocr
hebocrdoc_DATA = \
        README.md \
        AUTHORS \
        COPYING \
        ChangeLog \
        INSTALL \
        HACKING
