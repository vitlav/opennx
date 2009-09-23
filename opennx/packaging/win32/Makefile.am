SUBDIRS = xming nx pulse

setup: findrequires.pl
	$(RM) -rf setupdir
	mkdir setupdir
	$(MAKE) -C $(top_builddir) DESTDIR=$(abs_builddir)/setupdir \
		transform= prefix=/ bindir=/bin datadir=/share install
	$(MAKE) -C xming tmpinstall
	cp xming/dist/usr/X11R6/bin/*.exe setupdir/bin
	cp xming/dist/usr/X11R6/bin/*.dll setupdir/bin
	cp xming/dist/usr/X11R6/lib/X11/XErrorDB setupdir/bin
	cp xming/dist/usr/X11R6/lib/X11/XKeysymDB setupdir/bin
	cp xming/dist/usr/X11R6/lib/X11/xserver/SecurityPolicy setupdir/bin
	$(MAKE) -C nx tmpinstall
	cp nx/dist/bin/*.exe setupdir/bin
	cp nx/dist/lib/*.dll setupdir/bin
	perl findrequires.pl "$(DLLPATH)" setupdir/bin/*.exe|xargs cp -t setupdir/bin

EXTRA_DIST = opennx.iss
MAINATINERCLEANFILES = Makefile.in

#WINEPREFIX="~/.wine" wine "C:/Program Files/InnoSetup 5/iscc.exe"
