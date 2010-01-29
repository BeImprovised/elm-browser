## Process this file with automake to produce Makefile.in
## Created by Anjuta

SUBDIRS = src 

venturadocdir = ${datadir}/doc/ventura
venturadoc_DATA = \
	README\
	COPYING\
	AUTHORS\
	ChangeLog\
	INSTALL\
	NEWS


EXTRA_DIST = $(venturadoc_DATA)


# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(venturadocdir)
