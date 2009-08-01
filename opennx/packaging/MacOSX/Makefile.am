BUILDUSR = $(shell id -u):$(shell id -g)

dmg: pkg
	rm -rf dmgroot
	mkdir dmgroot
	sudo cp -a OpenNX.pkg dmgroot
	cp $(srcdir)/OpenSC.webloc dmgroot
	$(srcdir)/mkdmg -o \
	    -b $(srcdir)/folderbg.jpg \
	    -f $(srcdir)/fileopts \
	    -i $(srcdir)/volicon.icns \
	    $(top_builddir)/OpenNX-$(VERSION).dmg dmgroot
	touch $@

pkgdir:
	rm -rf pkgroot
	mkdir -p pkgroot/Applications
	mkdir -p pkgroot$(prefix)
	$(srcdir)/mkbundle pkgroot/Applications/OpenNX.app OpenNXapp \
	    $(top_srcdir)/res/nx.icns $(bindir)/opennx \
	    $(srcdir)/opennx-main.plist "OPNX"
	cp $(top_srcdir)/res/nx-desktop.icns \
	    pkgroot/Applications/OpenNX.app/Contents/Resources
	$(srcdir)/mkbundle pkgroot/Applications/OpenNXAdmin.app OpenNXAdmin \
	    $(top_srcdir)/res/opennx-admin.icns $(bindir)/opennx \
	    $(srcdir)/opennx.plist "" --admin
	$(srcdir)/mkbundle pkgroot/Applications/OpenNXWizard.app OpenNXWizard \
	    $(top_srcdir)/res/opennx-wizard.icns $(bindir)/opennx \
	    $(srcdir)/opennx.plist "" --wizard
	$(srcdir)/mkbundle pkgroot/Library/OpenNX/Message.app OpenNXMessage \
	    $(top_srcdir)/res/nx.icns $(bindir)/opennx \
	    $(srcdir)/opennx.plist "" --dialog ok --style info --caption CARDREMOVED --message CARDREMOVED
	rm -rf scbuild
	mkdir scbuild
	(cd scbuild ; $(abs_srcdir)/buildopensc $(prefix) $(abs_builddir)/pkgroot$(libdir))
	sed -e "s!_PREFIX_!$(abs_builddir)/scbuild/dist/$(prefix)!" \
	    < $(srcdir)/opensc-config.tmpl > scbuild/dist$(prefix)/bin/opensc-config
	chmod a+x scbuild/dist$(prefix)/bin/opensc-config
	rm -rf nxbuild
	mkdir nxbuild
	(cd nxbuild ; DEST=$(abs_builddir)/pkgroot PFX=$(prefix) $(abs_srcdir)/buildnx)
	$(MAKE) -C $(top_builddir) DESTDIR=$(abs_builddir)/pkgroot install
	$(MAKE) -C $(top_builddir)/po DESTDIR=$(abs_builddir)/pkgroot install-data-yes
	$(INSTALL) -m 0755 $(srcdir)/opennx-uninstall \
	    pkgroot$(bindir)
	$(srcdir)/buildsmbclient $(prefix) \
	    $(abs_builddir)/pkgroot$(libdir)
	touch $@

pkg: pkgdir
	sudo chown -R root.wheel pkgroot
	rm -rf OpenNX.pkg
	cd $(builddir) && /Developer/Tools/packagemaker -build -p OpenNX.pkg -f pkgroot -v \
	    -r $(srcdir)/pkgResources \
	    -i $(srcdir)/pkg.plist \
	    -d $(srcdir)/Description.plist
	sudo chown -R $(BUILDUSR) pkgroot
	touch $@

MAINTAINERCLEANFILES = Makefile.in
CLEANFILES = -rf pkg pkgdir dmg dmgroot pkgroot nxbuild OpenNX.pkg scbuild
EXTRA_DIST = Description.plist OpenSC.webloc buildnx buildopensc \
	buildsmbclient fileopts folderbg.jpg mkbundle mkdmg opennx-uninstall \
	opensc-config.tmpl pkgResources libsmbclient.h volicon.icns volicon.png
