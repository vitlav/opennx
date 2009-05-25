setup: findrequires.pl
	$(RM) -rf setupdir
	mkdir setupdir
	cp $(top_builddir)/{opennx,tracelog,watchreader}.exe setupdir
	perl findrequires.pl "$(DLLPATH)" setupdir/*.exe|xargs cp -t setupdir

#WINEPREFIX="~/.wine" wine "C:/Program Files/InnoSetup 5/iscc.exe"
