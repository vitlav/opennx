SUBDIRS = xming

setup: findrequires.pl
	$(RM) -rf setupdir
	mkdir setupdir
	$(MAKE) -C $(top_builddir) DESTDIR=$(abs_builddir)/setupdir install
	$(MAKE) -C xming tmpinstall
	cp xming/dist/usr/X11R6/bin/*.exe setupdir/bin
	cp xming/dist/usr/X11R6/bin/*.dll setupdir/bin
	cp xming/dist/usr/X11R6/lib/X11/XErrorDB setupdir/bin
	cp xming/dist/usr/X11R6/lib/X11/XKeysymDB setupdir/bin
	cp xming/dist/usr/X11R6/lib/X11/xserver/SecurityPolicy setupdir/bin
	perl findrequires.pl "$(DLLPATH)" setupdir/bin/*.exe|xargs cp -t setupdir/bin

EXTRA_DIST = opennx.iss patches
MAINATINERCLEANFILES = Makefile.in

#WINEPREFIX="~/.wine" wine "C:/Program Files/InnoSetup 5/iscc.exe"
