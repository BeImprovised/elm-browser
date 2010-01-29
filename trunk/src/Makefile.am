## Process this file with automake to produce Makefile.in

## Created by Anjuta

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	$(VENTURA_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = ventura

ventura_SOURCES = \
	callbacks.c \
	callbacks.h \
	main.c \
	gui.c \
	gui.h \
	dbsqlite.c \
	dbsqlite.h

ventura_LDFLAGS = \
	-Wl,--export-dynamic

ventura_LDADD = $(VENTURA_LIBS)

