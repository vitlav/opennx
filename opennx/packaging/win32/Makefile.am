SUBDIRS = xming nx cygnx pulse

FNT1DIR = dejavu-fonts-ttf-2.30
FNT1BALL = $(FNT1DIR).tar.bz2
FNT1URL = http://sourceforge.net/projects/dejavu/files/dejavu/$(FNT1BALL)
FNT2DIR = font-misc-misc-1.1.0
FNT2BALL = $(FNT2DIR).tar.bz2
FNT2URL = ftp://ftp.gwdg.de/pub/x11/x.org/pub/X11R7.5/src/font/$(FNT2BALL)

.setupdir: findrequires.pl $(FNT1DIR) .miscfonts
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
	test $(BUILDXMING) = no || mkdir -p setupdir/share/Xming/fonts/TTF
	test $(BUILDXMING) = no || cp $(FNT1DIR)/ttf/* setupdir/share/Xming/fonts/TTF
	test $(BUILDXMING) = no || $(MAKE) -C $(FNT2DIR) DESTDIR=$(abs_builddir)/setupdir install
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
	"$(ISCC)" "$(srcdir)/opennx.iss" -dBUILDXMING=$(BUILDXMING)

crosssetup: .setupdir isccwrap.sh
	sh isccwrap.sh "$(srcdir)/opennx.iss" -dBUILDXMING=$(BUILDXMING)

setup: .setupdir  $(SETUP)

$(FNT1BALL):
	@echo downloading tarball $(FNT1BALL)
	@$(WGET) "$(FNT1URL)"

$(FNT1DIR): $(FNT1BALL)
	@tar xjf $<

$(FNT2BALL):
	@echo downloading tarball $(FNT2BALL)
	@$(WGET) "$(FNT2URL)"

$(FNT2DIR): $(FNT2BALL)
	@tar xjf $<

.miscfonts: $(FNT2DIR)
	@(cd $(FNT2DIR) && ./configure \
		--with-fontrootdir=/share/Xming/fonts \
		--with-compression=gzip )
	touch $@

EXTRA_DIST = opennx.iss
MAINTAINERCLEANFILES = Makefile.in $(FNT1BALL) $(FNT2BALL)
CLEANFILES = -r .setupdir setupdir $(FNT1DIR) $(FNT2DIR)
