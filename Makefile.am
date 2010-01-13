## Process this file with automake to produce Makefile.in
## Created by Anjuta

SUBDIRS = src 

browserdocdir = ${prefix}/doc/browser
browserdoc_DATA = \
	README\
	COPYING\
	AUTHORS\
	ChangeLog\
	INSTALL\
	NEWS


EXTRA_DIST = $(browserdoc_DATA)


# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(browserdocdir)
