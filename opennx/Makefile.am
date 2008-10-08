SUBDIRS = po

noinst_PROGRAMS = bin2hdr
bin2hdr_SOURCES = bin2hdr.cpp

bin_PROGRAMS = opennx

opennx_SOURCES = AboutDialog.cpp AsyncProcess.cpp ConnectDialog.cpp ExtHtmlWindow.cpp \
	ForeignFrame.cpp Icon.cpp LogDialog.cpp KeyDialog.cpp LoginDialog.cpp \
	md5.c MyIPC.cpp MySession.cpp MyValidator.cpp MyWizard.cpp \
	MyXmlConfig.cpp opennxApp.cpp osdep.c PanicDialog.cpp pwcrypt.cpp \
	QuitDialog.cpp RdpImageSettingsDialog.cpp RdpPropertyDialog.cpp \
	ResumeDialog.cpp SessionAdmin.cpp SessionList.cpp \
	SessionProperties.cpp SmbShareProperties.cpp TextViewer.cpp \
	UnixImageSettingsDialog.cpp VncImageSettingsDialog.cpp \
	VncPropertyDialog.cpp wxprec.cpp WinShare.cpp X11PropertyDialog.cpp

noinst_HEADERS = AboutDialog.h AboutDialog_symbols.h AsyncProcess.h ConnectDialog.h \
	ConnectDialog_symbols.h debugleak.h ExtHtmlWindow.h \
	ForeignFrame.h ForeignFrame_symbols.h Icon.h \
	KeyDialog.h KeyDialog_symbols.h LogDialog.h LogDialog_symbols.h \
	LoginDialog.h LoginDialog_symbols.h md5.h memres.h MyIPC.h \
	MySession.h MyValidator.h MyWizard.h MyWizard_symbols.h MyXmlConfig.h \
	opennxApp.h osdep.h  PanicDialog.h PanicDialog_symbols.h pwcrypt.h \
	QuitDialog.h QuitDialog_symbols.h RdpImageSettingsDialog.h \
	RdpImageSettingsDialog_symbols.h RdpPropertyDialog.h \
	RdpPropertyDialog_symbols.h resource.h ResumeDialog.h \
	ResumeDialog_symbols.h SessionAdmin.h SessionAdmin_symbols.h \
	SessionList.h SessionProperties.h SessionProperties_symbols.h \
	SmbShareProperties.h SmbShareProperties_symbols.h TextViewer.h \
	TextViewer_symbols.h UnixImageSettingsDialog.h \
	UnixImageSettingsDialog_symbols.h VncImageSettingsDialog.h \
	VncImageSettingsDialog_symbols.h VncPropertyDialog.h \
	VncPropertyDialog_symbols.h WinShare.h X11PropertyDialog.h \
	X11PropertyDialog_symbols.h

EXTRA_HEADERS = resource.h res/opennx.rc

EXTRA_opennx_SOURCES = opennx.rc

opennx_LDADD = $(WIN_RES) $(WX_LIBS) $(X_LIBS) $(X_PRE_LIBS) $(X_EXTRA_LIBS) $(APP_LDADD)
opennx_DEPENDENCIES = $(WIN_RES)
opennx_CPPFLAGS = $(WX_CPPFLAGS) $(X_CFLAGS)
opennx_CXXFLAGS = $(WX_CXXFLAGS)
opennx_CFLAGS = $(WX_CFLAGS)


GETTEXTIZE = $(shell which gettextize)

bootstrap:
# 	Convince gettextize not to modify EXTRA_DIST
	test -f conf/config.rpath || touch conf/config.rpath
# 	Convince gettextize not to modify AC_CONFIG_FILES
	test -f po/Makefile.in.in || touch po/Makefile.in.in
#	Tweak gettextize
#	Stupid gettextize uses /dev/tty for interactively getting a
#	confirm of it's "oh so incredibly important notes".
#	YES, i've read them at least a dozen times and now, i REALLY
#	don't want to hit RETURN anymore!
	test -n "$(GETTEXTIZE)" && sed -e 's@/dev/tty@/dev/null@' "$(GETTEXTIZE)" > gettextize.local
	test -f gettextize.local && sh gettextize.local -f --no-changelog
	rm -f gettextize.local
	test -e conf/mkinstalldirs || touch conf/mkinstalldirs
	AUTOMAKE_VERSION=1.8 AUTOCONF_VERSION=2.59 autoreconf -if

ACLOCAL_AMFLAGS = -I m4

macbundle:
	mkdir -p OpenNX.app/Contents/{MacOS,Resources}
	cp $(top_srcdir)/Info.plist OpenNX.app/Contents
	cp $(top_srcdir)/res/nx.icns OpenNX.app/Contents/Resources
	ln -s -f `pwd`/$(top_builddir)/opennx OpenNX.app/Contents/MacOS/opennx
	echo "APPL????" > OpenNX.app/Contents/PkgInfo

#distclean-local:
#	rm -f po/*~ *.bak *~ \#.* .#*
#	rm -rf OpenNX.app

tmpres.zip: $(top_srcdir)/res/*.xrc $(top_srcdir)/res/*.png $(top_srcdir)/res/*.html $(top_srcdir)/res/*.txt $(top_srcdir)/res/*.rtf
	if test "$(top_srcdir)" != "$(top_builddir)" ; then $(LN_S) $(top_srcdir)/res $(top_builddir) ; fi
	zip -q9r $(top_builddir)/$@ res -i *.png -i *.xrc -i *.html -i *.txt -i *.rtf res/nx-desktop.icns -x res/.svn/*
	if test "$(top_srcdir)" != "$(top_builddir)" ; then $(RM) $(top_builddir)/res ; fi

memres.h: tmpres.zip bin2hdr
	./bin2hdr tmpres.zip memres.h mem_res

$(top_srcdir)/opennxApp.cpp: memres.h

opennxApp.cpp: memres.h

SUFFIXES = .rc

.rc.o:
	$(WX_RESCOMP) -o $@ $<

CLEANFILES = memres.h tmpres.zip *~ po/*~ *.bak *~ \#.* .\#* OpenNX.app

MAINTAINERCLEANFILES = ABOUT-NLS Makefile.in configure aclocal.m4 \
	conf/config.guess conf/config.sub conf/depcomp conf/install-sh conf/missing conf/mkinstalldirs \
	conf/ltmain.sh conf/config.rpath \
	m4/* \
	po/*.sin po/*.sed po/*.header po/Rules-quot po/*.template po/Makefile*.in

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

EXTRA_DIST = conf/config.rpath INSTALL INSTALL.generic COPYING m4 res share opennx.pjd opennx.dsw \
			 bin2hdr.dsp opennx.dsp Translations.dsp opennx.rc opennx.spec
