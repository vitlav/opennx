man_MANS = pconnect.1

data_DATA = pconnect.html

pconnect.html: pconnect.1
	man -l -Thtml $< > $@

EXTRA_DIST = $(man_MANS) $(data_DATA)
MAINTAINERCLEANFILES = Makefile.in

.PRECIOUS: pconnect.html
