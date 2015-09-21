## Process this file with automake to produce Makefile.in

SUBDIRS = src

icplddocdir = ${prefix}/doc/icpld-1.1.5
icplddoc_DATA = \
	README\
	COPYING\
	AUTHORS\
	ChangeLog\
	INSTALL\
	NEWS\
	TODO

EXTRA_DIST = $(icplddoc_DATA)


man_MANS = man/icpld.1
# Copy all the spec files. Of course, only one is actually used.
dist-hook:
	for specfile in *.spec; do \
		if test -f $$specfile; then \
			cp -p $$specfile $(distdir); \
		fi \
	done

