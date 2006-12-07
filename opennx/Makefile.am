AUTOMAKE_OPTIONS = foreign dist-bzip2 no-dist-gzip no-installinfo no-installman

SUBDIRS = m4 po

noinst_PROGRAMS = bin2hdr
bin2hdr_SOURCES = bin2hdr.cpp

bin_PROGRAMS = Opennx

Opennx_SOURCES = AboutDialog.cpp ConnectDialog.cpp Icon.cpp LogDialog.cpp \
	LoginDialog.cpp OpennxApp.cpp MxIPC.cpp MxSession.cpp \
	MxValidator.cpp MxWizard.cpp MxXmlConfig.cpp pwcrypt.cpp \
	RdpImageSettingsDialog.cpp RdpPropertyDialog.cpp SessionAdmin.cpp \
	SessionList.cpp SessionProperties.cpp SmbShareProperties.cpp \
	TextViewer.cpp UnixImageSettingsDialog.cpp VncImageSettingsDialog.cpp \
	VncPropertyDialog.cpp wxprec.cpp WinShare.cpp X11PropertyDialog.cpp \
	md5.c

noinst_HEADERS = AboutDialog.h AboutDialog_symbols.h ConnectDialog.h \
	ConnectDialog_symbols.h debugleak.h Icon.h LogDialog.h \
	LogDialog_symbols.h LoginDialog.h LoginDialog_symbols.h OpennxApp.h \
	MxIPC.h MxValidator.h MxWizard.h MxWizard_symbols.h MxXmlConfig.h \
	pwcrypt.h RdpImageSettingsDialog.h RdpImageSettingsDialog_symbols.h \
	RdpPropertyDialog.h RdpPropertyDialog_symbols.h resource.h \
	SessionAdmin.h SessionAdmin_symbols.h SessionList.h \
	SessionProperties.h SessionProperties_symbols.h \
	SmbShareProperties.h SmbShareProperties_symbols.h \
	TextViewer.h TextViewer_symbols.h \
	UnixImageSettingsDialog.h UnixImageSettingsDialog_symbols.h \
	VncImageSettingsDialog.h VncImageSettingsDialog_symbols.h \
	VncPropertyDialog.h VncPropertyDialog_symbols.h WinShare.h \
	X11PropertyDialog.h X11PropertyDialog_symbols.h md5.h MxSession.h memres.h

PROGRAM = Opennx

Opennx_LDADD = -lstdc++

bootstrap:
	AUTOMAKE_VERSION=1.8 AUTOCONF_VERSION=2.59 autoreconf -is

ACLOCAL_AMFLAGS = -I m4

distclean-local:
	rm -f po/*~ *.bak *~ \#.* .#*

tmpres.zip: res/Opennx.xrc
	zip -q9r $@ res -i *.png -i *.xrc -i *.ico

memres.h: tmpres.zip bin2hdr
	./bin2hdr tmpres.zip memres.h mem_res

OpennxApp.cpp: memres.h

CLEANFILES = tmpres.zip *~

EXTRA_DIST = conf/config.rpath conf/mkinstalldirs  conf/config.rpath conf/mkinstalldirs Opennx.pjd Opennx.dsw \
	Opennx.dsp Opennx.rc res/Opennx.rc res/Opennx.xrc \
	Translations.dsp PackedResources.dsp
