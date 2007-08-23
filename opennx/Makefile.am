AUTOMAKE_OPTIONS = foreign no-installinfo no-installman

SUBDIRS = m4 po

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

opennx_LDADD = $(WX_LIBS) $(APP_LDADD)
opennx_CPPFLAGS = $(WX_CPPFLAGS)
opennx_CXXFLAGS = $(WX_CXXFLAGS)
opennx_CFLAGS = $(WX_CFLAGS)

bootstrap:
	AUTOMAKE_VERSION=1.8 AUTOCONF_VERSION=2.59 autoreconf -isf

ACLOCAL_AMFLAGS = -I m4

macbundle:
	mkdir -p OpenNX.app/Contents/{MacOS,Resources}
	cp $(top_srcdir)/Info.plist OpenNX.app/Contents
	cp $(top_srcdir)/res/nx.icns OpenNX.app/Contents/Resources
	ln -s -f `pwd`/$(top_builddir)/opennx OpenNX.app/Contents/MacOS/opennx
	echo "APPL????" > OpenNX.app/Contents/PkgInfo

distclean-local:
	rm -f po/*~ *.bak *~ \#.* .#*
	rm -rf OpenNX.app

tmpres.zip: $(top_srcdir)/res/*.xrc $(top_srcdir)/res/*.png $(top_srcdir)/res/*.html $(top_srcdir)/res/*.txt $(top_srcdir)/res/*.rtf
	if test "$(top_srcdir)" != "$(top_builddir)" ; then $(LN_S) $(top_srcdir)/res $(top_builddir) ; fi
	zip -q9r $(top_builddir)/$@ res -i *.png -i *.xrc -i *.html -i *.txt -i *.rtf res/nx-desktop.icns -x res/.svn/*
	if test "$(top_srcdir)" != "$(top_builddir)" ; then $(RM) $(top_builddir)/res ; fi

memres.h: tmpres.zip bin2hdr
	./bin2hdr tmpres.zip memres.h mem_res

$(top_srcdir)/opennxApp.cpp: memres.h

opennxApp.cpp: memres.h

CLEANFILES = memres.h tmpres.zip *~

MAINTAINERCLEANFILES = Makefile.in configure aclocal.m4 conf/config.guess \
	conf/config.sub  conf/depcomp  conf/install-sh  conf/missing

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

EXTRA_DIST = INSTALL COPYING res share opennx.pjd opennx.dsw \
			 bin2hdr.dsp opennx.dsp Translations.dsp opennx.rc opennx.spec
