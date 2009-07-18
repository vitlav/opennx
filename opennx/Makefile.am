AM_LIBTOOLFLAGS = --silent
BUILDUSR = $(shell id -u):$(shell id -g)

SUBDIRS = po
DIST_SUBDIRS = po packaging/win32

noinst_PROGRAMS = bin2hdr
bin2hdr_SOURCES = bin2hdr.cpp

bin_PROGRAMS = opennx $(TRACELOG) $(WATCHREADER) $(WATCHUSBIP)

EXTRA_PROGRAMS = tracelog$(EXEEXT) watchreader$(EXEEXT) watchusbip$(EXEEXT)

opennx_SOURCES = AboutDialog.cpp AsyncProcess.cpp LibOpenSC.cpp \
	CardWaiterDialog.cpp ConnectDialog.cpp ExtHtmlWindow.cpp \
	ForeignFrame.cpp Icon.cpp LogDialog.cpp KeyDialog.cpp LibUSB.cpp \
	LoginDialog.cpp md5.c MyIPC.cpp MyDynlib.cpp MySession.cpp \
	MyValidator.cpp MyWizard.cpp MyXmlConfig.cpp opennxApp.cpp \
	osdep.c PanicDialog.cpp pwcrypt.cpp QuitDialog.cpp \
	RdpImageSettingsDialog.cpp RdpPropertyDialog.cpp ResumeDialog.cpp \
	SessionAdmin.cpp SessionList.cpp SessionProperties.cpp \
	ShareProperties.cpp TextViewer.cpp UnixImageSettingsDialog.cpp \
	UsbFilterDetailsDialog.cpp UsbIp.cpp VncImageSettingsDialog.cpp \
	VncPropertyDialog.cpp wxprec.cpp WinShare.cpp X11PropertyDialog.cpp \
	xh_richtext.cpp MacOSX.c

noinst_HEADERS = AboutDialog.h AboutDialog_symbols.h AsyncProcess.h \
	LibOpenSC.h CardWaiterDialog.h CardWaiterDialog_symbols.h \
	ConnectDialog.h ConnectDialog_symbols.h debugleak.h \
	DebugStringGrabber.h ExtHtmlWindow.h ForeignFrame.h \
	ForeignFrame_symbols.h Icon.h KeyDialog.h KeyDialog_symbols.h \
	LibUSB.h LogDialog.h LogDialog_symbols.h LoginDialog.h \
	LoginDialog_symbols.h md5.h memres.h MyIPC.h MyDynlib.h MySession.h \
	MyValidator.h MyWizard.h MyWizard_symbols.h MyXmlConfig.h opennxApp.h \
	osdep.h  PanicDialog.h PanicDialog_symbols.h pwcrypt.h QuitDialog.h \
	QuitDialog_symbols.h RdpImageSettingsDialog.h \
	RdpImageSettingsDialog_symbols.h RdpPropertyDialog.h \
	RdpPropertyDialog_symbols.h ResumeDialog.h ResumeDialog_symbols.h \
	SessionAdmin.h SessionAdmin_symbols.h SessionList.h \
	SessionProperties.h SessionProperties_symbols.h ShareProperties.h \
	ShareProperties_symbols.h TextViewer.h TextViewer_symbols.h \
	TraceLogFrame.h TraceLogFrame_symbols.h UnixImageSettingsDialog.h \
	UnixImageSettingsDialog_symbols.h UsbFilterDetailsDialog.h \
	UsbFilterDetailsDialog_symbols.h UsbIp.h \
	VncImageSettingsDialog.h VncImageSettingsDialog_symbols.h \
	VncPropertyDialog.h VncPropertyDialog_symbols.h WinShare.h \
	X11PropertyDialog.h X11PropertyDialog_symbols.h trace.h tracelogApp.h \
	watchReaderApp.h watchUsbIpApp.h xh_richtext.h

watchreader_SOURCES = watchReaderApp.cpp LibOpenSC.cpp MyDynlib.cpp MacOSX.c
watchreader_DEPENDENCIES = $(WATCHREADER_RES)
watchreader_LDADD = $(WATCHREADER_RES) $(WX_LIBS) $(X_LIBS) \
	$(X_PRE_LIBS) $(X_EXTRA_LIBS) $(APP_LDADD)
watchreader_CPPFLAGS = $(WX_CPPFLAGS) $(X_CFLAGS) -DAPP_WATCHREADER
watchreader_CXXFLAGS = $(WX_CXXFLAGS)
watchreader_CFLAGS = $(WX_CFLAGS)

watchusbip_SOURCES = watchUsbIpApp.cpp UsbIp.cpp Icon.cpp LibUSB.cpp \
	md5.c MyValidator.cpp MyXmlConfig.cpp osdep.c pwcrypt.cpp \
	UsbFilterDetailsDialog.cpp wxprec.cpp xh_richtext.cpp MyDynlib.cpp
watchusbip_LDADD = $(WX_LIBS) $(X_LIBS) $(X_PRE_LIBS) $(X_EXTRA_LIBS) \
	$(APP_LDADD)
watchusbip_CPPFLAGS = $(WX_CPPFLAGS) $(X_CFLAGS) -DAPP_WATCHUSBIP
watchusbip_CXXFLAGS = $(WX_CXXFLAGS)
watchusbip_CFLAGS = $(WX_CFLAGS)

tracelog_SOURCES = tracelogApp.cpp TraceLogFrame.cpp \
	DebugStringGrabber.cpp wxprec.cpp Icon.cpp
tracelog_LDADD = $(TRACELOG_RES) $(WX_LIBS) $(X_LIBS) $(X_PRE_LIBS) \
	$(X_EXTRA_LIBS) $(APP_LDADD)
tracelog_DEPENDENCIES = $(TRACELOG_RES)
tracelog_CPPFLAGS = $(WX_CPPFLAGS) $(X_CFLAGS) -DAPP_TRACELOG
tracelog_CXXFLAGS = $(WX_CXXFLAGS)
tracelog_CFLAGS = $(WX_CFLAGS)

EXTRA_HEADERS = res/opennx.rc

EXTRA_opennx_SOURCES = opennx.rc
EXTRA_tracelog_SOURCES = tracelog.rc
EXTRA_watchreader_SOURCES = watchreader.rc

opennx_LDADD = $(OPENNX_RES) $(WX_LIBS) $(X_LIBS) $(X_PRE_LIBS) \
	$(X_EXTRA_LIBS) $(APP_LDADD)
opennx_DEPENDENCIES = $(OPENNX_RES)
opennx_CPPFLAGS = $(WX_CPPFLAGS) $(X_CFLAGS) -DAPP_OPENNX
opennx_CXXFLAGS = $(WX_CXXFLAGS)
opennx_CFLAGS = $(WX_CFLAGS)


GETTEXTIZE = $(shell which gettextize)

bootstrap:
#   Update ChangeLog from SVN
	svn2cl -i --break-before-msg
# 	Convince gettextize not to modify EXTRA_DIST
	test -f conf/config.rpath || touch conf/config.rpath
# 	Convince gettextize not to modify AC_CONFIG_FILES
	test -f po/Makefile.in.in || touch po/Makefile.in.in
#	Tweak gettextize
#	Stupid gettextize uses /dev/tty for interactively getting a
#	confirm of it's "oh so incredibly important notes".
#	YES, i've read them at least a dozen times and now, i REALLY
#	don't want to hit RETURN anymore!
	test -n "$(GETTEXTIZE)" && \
		sed -e 's@/dev/tty@/dev/null@' "$(GETTEXTIZE)" > gettextize.local
	test -f gettextize.local && sh gettextize.local -f --no-changelog
	rm -f gettextize.local
	test -e conf/mkinstalldirs || touch conf/mkinstalldirs
	AUTOMAKE_VERSION=1.8 AUTOCONF_VERSION=2.59 autoreconf -if

ACLOCAL_AMFLAGS = -I m4

# The following assumes a package prefix of /Library/OpenNX
pkg: all
	rm -rf pkgroot
	mkdir -p pkgroot/{Applications,Library}
	$(top_srcdir)/packaging/MacOSX/mkbundle pkgroot/Applications/OpenNX.app OpenNXapp \
	    $(top_srcdir)/res/nx.icns /Library/OpenNX/bin/opennx \
	    $(top_srcdir)/packaging/MacOSX/opennx-main.plist "OPNX"
	cp $(top_srcdir)/res/nx-desktop.icns \
	    pkgroot/Applications/OpenNX.app/Contents/Resources
	$(top_srcdir)/packaging/MacOSX/mkbundle pkgroot/Applications/OpenNXAdmin.app OpenNXAdmin \
	    $(top_srcdir)/res/opennx-admin.icns /Library/OpenNX/bin/opennx \
	    $(top_srcdir)/packaging/MacOSX/opennx.plist "" --admin
	$(top_srcdir)/packaging/MacOSX/mkbundle pkgroot/Applications/OpenNXWizard.app OpenNXWizard \
	    $(top_srcdir)/res/opennx-wizard.icns /Library/OpenNX/bin/opennx \
	    $(top_srcdir)/packaging/MacOSX/opennx.plist "" --wizard
	rm -rf nxbuild
	mkdir nxbuild
	(cd nxbuild ; DEST=$(abs_builddir)/pkgroot $(abs_srcdir)/packaging/MacOSX/buildnx)
	$(MAKE) DESTDIR=$(abs_builddir)/pkgroot install
	$(INSTALL) -m 0755 $(top_srcdir)/packaging/MacOSX/opennx-uninstall \
	    $(abs_builddir)/pkgroot$(bindir)
	sudo chown -R root.wheel pkgroot
	rm -rf OpenNX.pkg
	/Developer/Tools/packagemaker -build -p OpenNX.pkg -f pkgroot -v \
	    -r packaging/MacOSX/pkgResources \
	    -i packaging/MacOSX/pkg.plist \
	    -d packaging/MacOSX/Description.plist
	sudo chown -R $(BUILDUSR) pkgroot

bundle:
	$(top_srcdir)/packaging/MacOSX/mkbundle OpenNX.app OpenNXapp \
	    $(top_srcdir)/res/nx.icns $(abs_builddir)/opennx \
	    $(top_srcdir)/packaging/MacOSX/opennx-main.plist "OPNX"
	cp $(top_srcdir)/res/nx-desktop.icns OpenNX.app/Contents/Resources
	$(top_srcdir)/packaging/MacOSX/mkbundle OpenNXAdmin.app OpenNXAdmin \
	    $(top_srcdir)/res/opennx-admin.icns $(abs_builddir)/opennx \
	    $(top_srcdir)/packaging/MacOSX/opennx.plist "" --admin
	$(top_srcdir)/packaging/MacOSX/mkbundle OpenNXWizard.app OpenNXWizard \
	    $(top_srcdir)/res/opennx-wizard.icns $(abs_builddir)/opennx \
	    $(top_srcdir)/packaging/MacOSX/opennx.plist "" --wizard

all-local: $(BUNDLE)

clean-local:
	rm -rf OpenNX.app OpenNXAdmin.app OpenNXWizard.app

distclean-local:
	rm -f po/stamp-po

RESDEPS = $(wildcard $(srcdir)/res/*.xrc $(srcdir)/res/*.png \
		  $(srcdir)/res/*.html $(srcdir)/res/*.xml $(OSRESDEPS))
RESSRC = $(subst $(srcdir)/,,$(RESDEPS))

tmpres.zip: $(RESDEPS)
	cd $(srcdir) && zip -q9r $(abs_builddir)/$(@F) $(RESSRC)

memres.h: tmpres.zip bin2hdr
	./bin2hdr tmpres.zip memres.h mem_res

$(top_srcdir)/opennxApp.cpp: memres.h

opennxApp.cpp: memres.h

tracelogApp.cpp: memres.h

watchUsbIpApp.cpp: memres.h

SUFFIXES = .rc

.rc.o:
	$(WX_RESCOMP) -I$(top_builddir) -o $@ $<

CLEANFILES = memres.h tmpres.zip *~ po/*~ *.bak *~ \#.* .\#* OpenNX.app

MAINTAINERCLEANFILES = ABOUT-NLS Makefile.in configure aclocal.m4 \
	conf/config.guess conf/config.sub conf/depcomp conf/install-sh \
	conf/missing conf/mkinstalldirs conf/ltmain.sh conf/config.rpath \
	m4/*~ m4/gettext.m4 m4/iconv.m4 m4/lib-*.m4 m4/nls.m4 m4/po.m4 \
	m4/progtest.m4 po/*.sin po/*.sed po/*.header po/Rules-quot \
	po/*.template po/Makefile*.in

DISTCLEANFILES = res/*.prv.*

data_DATA = share/keyboards

keydir = $(datadir)/keys
key_DATA = $(top_srcdir)/share/keys/*

cupsdir = $(datadir)/cups
cups_DATA = $(top_srcdir)/share/cups/*

icondir = $(datadir)/icons
icon_DATA = $(top_srcdir)/res/nx.png $(top_srcdir)/res/nx-desktop.png \
	$(top_srcdir)/res/opennx-admin.png $(top_srcdir)/res/opennx-wizard.png

install-data-local:
	$(INSTALL) -d $(DESTDIR)$(datadir)/applnk
	cd $(top_srcdir)/share/applnk && \
	for d in * ; do \
		$(INSTALL) -d $(DESTDIR)$(datadir)/applnk/$$d ; \
		$(INSTALL_DATA) $$d/* $(DESTDIR)$(datadir)/applnk/$$d ; \
		if test -f $$d/.directory ; then $(INSTALL_DATA) $$d/.directory $(DESTDIR)$(datadir)/applnk/$$d ; fi ; \
	done

uninstall-local:
	cd $(top_srcdir)/share/applnk && \
	for d in * ; do \
		$(RM) -r $(DESTDIR)$(datadir)/applnk/$$d ; \
	done
	-$(RMDIR) $(DESTDIR)$(datadir)/applnk

dist-hook:
	find $(distdir) -type d -name .svn | xargs $(RM) -r

setup: all
	$(MAKE) -C packaging/win32 setup

EXTRA_DIST = conf/config.rpath INSTALL INSTALL.generic COPYING share \
	etc/60-opennx.rules opennx.pjd opennx.rc tracelog.rc watchreader.rc \
	opennx.spec patches getnxsrcpkg $(RESDEPS) ChangeLog \
	packaging/win32/findrequires.pl packaging/win32/opennx.iss \
	res/nx-desktop.ico res/nx.ico res/opennx-admin.ico \
	res/opennx-wizard.ico res/nx-desktop.icns res/nx.icns \
	res/opennx-admin.icns res/opennx-wizard.icns
