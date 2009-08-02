setup: findrequires.pl
	$(RM) -rf setupdir
	mkdir setupdir
	cp $(top_builddir)/{opennx,tracelog,watchreader}.exe setupdir
	perl findrequires.pl "$(DLLPATH)" setupdir/*.exe|xargs cp -t setupdir

EXTRA_DIST = xming opennx.iss
MAINATINERCLEANFILES = Makefile.in

#WINEPREFIX="~/.wine" wine "C:/Program Files/InnoSetup 5/iscc.exe"
