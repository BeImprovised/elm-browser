## Process this file with automake to produce Makefile.in

## Created by Anjuta

uidir = $(datadir)/browser/ui

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	$(BROWSER_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = browser

browser_SOURCES = \
	callbacks.c \
	callbacks.h \
	main.c \
	gui.c \
	gui.h

browser_LDFLAGS = \
	-Wl,--export-dynamic

browser_LDADD = $(BROWSER_LIBS)


# Remove ui directory on uninstall
uninstall-local:
	-rm -r $(datadir)/browser
