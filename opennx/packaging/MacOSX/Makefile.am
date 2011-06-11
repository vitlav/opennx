BUILDUSR = $(shell id -u):$(shell id -g)

dmg: pkg
	rm -rf dmgroot
	mkdir dmgroot
	sudo cp -R OpenNX.pkg dmgroot
	cp $(srcdir)/OpenSC.webloc dmgroot
	$(srcdir)/mkdmg -o \
	    -b $(srcdir)/folderbg.jpg \
	    -f $(srcdir)/fileopts \
	    -i $(srcdir)/volicon.icns \
	    $(top_builddir)/OpenNX-$(FULLVERSION).dmg dmgroot
	sudo rm -rf dmgroot
	touch $@

createpkgroot:
	rm -rf pkgroot
	mkdir -p pkgroot/Applications/OpenNX
	mkdir -p pkgroot$(prefix)
	$(srcdir)/mkbundle pkgroot/Applications/OpenNX/OpenNX.app OpenNXapp \
	    $(top_srcdir)/extres/nx.icns $(bindir)/opennx \
	    $(srcdir)/opennx-main.plist "OPNX"
	cp $(top_srcdir)/extres/nx-desktop.icns \
	    pkgroot/Applications/OpenNX/OpenNX.app/Contents/Resources
	$(srcdir)/mkbundle "pkgroot/Applications/OpenNX/Session Administrator.app" OpenNXAdmin \
	    $(top_srcdir)/extres/opennx-admin.icns $(bindir)/opennx \
	    $(srcdir)/opennx.plist "" --admin
	$(srcdir)/mkbundle "pkgroot/Applications/OpenNX/Connection Wizard.app" OpenNXWizard \
	    $(top_srcdir)/extres/opennx-wizard.icns $(bindir)/opennx \
	    $(srcdir)/opennx.plist "" --wizard
	$(srcdir)/mkbundle "pkgroot/Applications/OpenNX/Uninstall OpenNX.app" "" \
	    $(top_srcdir)/extres/opennx-uninstall.icns $(bindir)/macuninstall \
	    $(srcdir)/opennx.plist ""
	$(srcdir)/mkbundle pkgroot/Library/OpenNX/Message.app OpenNXMessage \
	    $(top_srcdir)/extres/nx.icns $(bindir)/opennx \
	    $(srcdir)/opennx.plist "" --dialog ok --style info --caption CARDREMOVED --message CARDREMOVED

pkgdir: createpkgroot
	rm -rf nxbuild
	mkdir nxbuild
	(cd nxbuild ; DEST=$(abs_builddir)/pkgroot PFX=$(prefix) CC=$(CC) CXX=$(CXX) $(abs_srcdir)/buildnx)
	$(MAKE) -C $(top_builddir) DESTDIR=$(abs_builddir)/pkgroot install
	for lang in $(shell cat $(top_srcdir)/po/LINGUAS) ; do \
		$(INSTALL_DATA) $(WX_PREFIX)/share/locale/$$lang/LC_MESSAGES/wxstd.mo \
			pkgroot/$(localedir)/$$lang/LC_MESSAGES/ ; \
	done
	touch $@

pkg: pkgdir
	sudo chown -R root:admin pkgroot
	rm -rf OpenNX.pkg
	cd $(builddir) && $(PACKAGEMAKER) -build -p OpenNX.pkg -f pkgroot -v \
	    -r $(srcdir)/pkgResources \
	    -i $(srcdir)/pkg.plist \
	    -d $(srcdir)/Description.plist
	sudo chown -R $(BUILDUSR) pkgroot
	touch $@

MAINTAINERCLEANFILES = Makefile.in
CLEANFILES = -rf pkg pkgdir dmg dmgroot pkgroot nxbuild OpenNX.pkg scbuild
EXTRA_DIST = Description.plist OpenSC.webloc buildnx buildopensc \
	buildsmbclient fileopts folderbg.jpg mkbundle mkdmg \
	pkgResources libsmbclient.h volicon.icns volicon.png
