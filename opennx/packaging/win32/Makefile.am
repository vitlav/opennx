SUBDIRS = xming nx cygnx pulse

.setupdir: findrequires.pl
	$(RM) -rf setupdir
	mkdir setupdir
	$(MAKE) -C $(top_builddir) DESTDIR=$(abs_builddir)/setupdir \
		transform= prefix=/ bindir=/bin datadir=/share install
	test $(BUILDXMING) = no || $(MAKE) -C xming tmpinstall
	test $(BUILDXMING) = no || cp xming/dist/usr/X11R6/bin/*.exe setupdir/bin
	test $(BUILDXMING) = no || cp xming/dist/usr/X11R6/bin/*.dll setupdir/bin
	test $(BUILDXMING) = no || mkdir -p setupdir/share/Xming
	test $(BUILDXMING) = no || cp xming/dist/usr/X11R6/lib/X11/XErrorDB setupdir/share/Xming
	test $(BUILDXMING) = no || cp xming/dist/usr/X11R6/lib/X11/XKeysymDB setupdir/share/Xming
	test $(BUILDXMING) = no || cp xming/dist/usr/X11R6/lib/X11/xserver/SecurityPolicy setupdir/share/Xming
	test $(BUILDXMING) = no || cp -r xming/dist/usr/X11R6/lib/X11/locale setupdir/share/Xming/locale
	test $(BUILDXMING) = no || cp xming/xc/programs/rgb/rgb.txt setupdir/share/Xming
	$(MAKE) -C $(NXBUILD) tmpinstall
	cp $(NXBUILD)/dist/bin/*.exe setupdir/bin
	cp $(NXBUILD)/dist/bin/*.dll setupdir/bin
	cp $(NXBUILD)/dist/lib/*.dll setupdir/bin
	test $(BUILDXMING) = yes || cp -r $(NXBUILD)/dist/share/* setupdir/share
	perl findrequires.pl "$(DLLPATH)" setupdir/bin/*.exe|xargs cp -t setupdir/bin
	rm -rf setupdir/share/icons setupdir/share/applnk
	$(STRIP) --strip-unneeded setupdir/bin/*.exe
	for lang in `cat $(top_srcdir)/po/LINGUAS` ; do \
		cp /usr/share/locale/$$lang/LC_MESSAGES/wxstd.mo \
			setupdir/share/locale/$$lang/LC_MESSAGES/ ; \
	done
	touch $@

nativesetup: .setupdir
	"$(ISCC)" "$(srcdir)/opennx.iss"

crosssetup: .setupdir
	$(WINE) "$(ISCC)" "$(srcdir)/opennx.iss"

setup: .setupdir  $(SETUP)

EXTRA_DIST = opennx.iss
MAINTAINERCLEANFILES = Makefile.in
CLEANFILES = -r .setupdir setupdir
