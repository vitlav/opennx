AUTOMAKE_OPTIONS = foreign no-installinfo no-installman

SUBDIRS = m4 po

noinst_PROGRAMS = bin2hdr
bin2hdr_SOURCES = bin2hdr.cpp

bin_PROGRAMS = opennx

opennx_SOURCES = AboutDialog.cpp ConnectDialog.cpp Icon.cpp LogDialog.cpp \
	KeyDialog.cpp LoginDialog.cpp opennxApp.cpp MyIPC.cpp MySession.cpp \
	MyValidator.cpp MyWizard.cpp MyXmlConfig.cpp pwcrypt.cpp \
	RdpImageSettingsDialog.cpp RdpPropertyDialog.cpp SessionAdmin.cpp \
	SessionList.cpp SessionProperties.cpp SmbShareProperties.cpp \
	TextViewer.cpp UnixImageSettingsDialog.cpp VncImageSettingsDialog.cpp \
	VncPropertyDialog.cpp wxprec.cpp WinShare.cpp X11PropertyDialog.cpp \
	md5.c

noinst_HEADERS = AboutDialog.h AboutDialog_symbols.h ConnectDialog.h \
	ConnectDialog_symbols.h debugleak.h Icon.h KeyDialog.h KeyDialog_symbols.h \
	LogDialog.h LogDialog_symbols.h LoginDialog.h LoginDialog_symbols.h opennxApp.h \
	MyIPC.h MyValidator.h MyWizard.h MyWizard_symbols.h MyXmlConfig.h \
	pwcrypt.h RdpImageSettingsDialog.h RdpImageSettingsDialog_symbols.h \
	RdpPropertyDialog.h RdpPropertyDialog_symbols.h resource.h \
	SessionAdmin.h SessionAdmin_symbols.h SessionList.h \
	SessionProperties.h SessionProperties_symbols.h \
	SmbShareProperties.h SmbShareProperties_symbols.h \
	TextViewer.h TextViewer_symbols.h \
	UnixImageSettingsDialog.h UnixImageSettingsDialog_symbols.h \
	VncImageSettingsDialog.h VncImageSettingsDialog_symbols.h \
	VncPropertyDialog.h VncPropertyDialog_symbols.h WinShare.h \
	X11PropertyDialog.h X11PropertyDialog_symbols.h md5.h MySession.h memres.h

PROGRAM = opennx

opennx_LDADD = -lstdc++

bootstrap:
	AUTOMAKE_VERSION=1.8 AUTOCONF_VERSION=2.59 autoreconf -is

ACLOCAL_AMFLAGS = -I m4

distclean-local:
	rm -f po/*~ *.bak *~ \#.* .#*

tmpres.zip: res/opennx.xrc
	zip -q9r $@ res -i *.png -i *.xrc -i *.ico

memres.h: tmpres.zip bin2hdr
	./bin2hdr tmpres.zip memres.h mem_res

opennxApp.cpp: memres.h

CLEANFILES = tmpres.zip *~

MAINTAINERCLEANFILES = Makefile.in configure aclocal.m4 conf/config.guess \
	conf/config.sub  conf/depcomp  conf/install-sh  conf/missing

DISTCLEANFILES = res/*.prv.*

EXTRA_DIST = conf/* res/* opennx.pjd opennx.dsw *.dsp \
	opennx.rc
