SUBDIRS = xming

setup: findrequires.pl
	$(RM) -rf setupdir
	mkdir setupdir
	cp $(top_builddir)/{opennx,tracelog,watchreader}.exe setupdir
	$(MAKE) -C xming tmpinstall
	cp xming/dist/usr/X11R6/bin/*.exe setupdir
	cp xming/dist/usr/X11R6/bin/*.dll setupdir
	cp xming/dist/usr/X11R6/lib/X11/XErrorDB setupdir
	cp xming/dist/usr/X11R6/lib/X11/XKeysymDB setupdir
	cp xming/dist/usr/X11R6/lib/X11/xserver/SecurityPolicy setupdir
	perl findrequires.pl "$(DLLPATH)" setupdir/*.exe|xargs cp -t setupdir

EXTRA_DIST = opennx.iss
MAINATINERCLEANFILES = Makefile.in

#WINEPREFIX="~/.wine" wine "C:/Program Files/InnoSetup 5/iscc.exe"
