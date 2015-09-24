## Makefile.am -- Process this file with automake to produce Makefile.in

# The libhocr library
INCLUDES = \
        -DVERSION=\"$(VERSION)\"

lib_LTLIBRARIES = libhebocr.la

libhebocr_la_SOURCES = \
        ho_pixbuf.c \
        ho_pixbuf_pnm.c \
        ho_pixbuf_filter.c \
        ho_pixbuf_op.c \
        ho_pixbuf_draw.c \
        ho_pixbuf_layout.c \
        ho_pixsum.c \
        ho_objmap.c \
        ho_objmap_filter.c \
        ho_objlist.c \
        ho_objlist_stat.c

libhebocr_la_LDFLAGS = -version-info $(libtoolversion)

libhebocr_la_LIBADD = 
        
library_includedir = $(includedir)/$(pkgincludedir)
library_include_HEADERS = \
        hebocr.h \
        ho_pixbuf.h \
        ho_pixbuf_pnm.h \
        ho_pixbuf_filter.h \
        ho_pixbuf_op.h \
        ho_pixbuf_draw.h \
        ho_pixbuf_layout.h \
        ho_pixsum.h \
        ho_objmap.h \
        ho_objmap_filter.h \
        ho_objlist.h \
        ho_objlist_stat.h
