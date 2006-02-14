AUTOMAKE_OPTIONS = foreign dist-bzip2 no-dist-gzip no-installinfo no-installman

SUBDIRS = m4 po

bin_PROGRAMS = mxclient

mxclient_SOURCES = AboutDialog.cpp ConnectDialog.cpp Icon.cpp LogDialog.cpp \
	LoginDialog.cpp mxclientApp.cpp MxIPC.cpp MxSession.cpp \
	MxValidator.cpp MxWizard.cpp MxXmlConfig.cpp pwcrypt.cpp \
	RdpImageSettingsDialog.cpp RdpPropertyDialog.cpp SessionAdmin.cpp \
	SessionList.cpp SessionProperties.cpp SmbShareProperties.cpp \
	UnixImageSettingsDialog.cpp VncImageSettingsDialog.cpp \
	VncPropertyDialog.cpp wxprec.cpp WinShare.cpp X11PropertyDialog.cpp \
	md5.c

noinst_HEADERS = AboutDialog.h AboutDialog_symbols.h ConnectDialog.h \
	ConnectDialog_symbols.h debugleak.h Icon.h LogDialog.h \
	LogDialog_symbols.h LoginDialog.h LoginDialog_symbols.h mxclientApp.h \
	MxIPC.h MxValidator.h MxWizard.h MxWizard_symbols.h MxXmlConfig.h \
	pwcrypt.h RdpImageSettingsDialog.h RdpImageSettingsDialog_symbols.h \
	RdpPropertyDialog.h RdpPropertyDialog_symbols.h resource.h \
	SessionAdmin.h SessionAdmin_symbols.h SessionList.h \
	SessionProperties.h SessionProperties_symbols.h \
	SmbShareProperties.h SmbShareProperties_symbols.h \
	UnixImageSettingsDialog.h UnixImageSettingsDialog_symbols.h \
	VncImageSettingsDialog.h VncImageSettingsDialog_symbols.h \
	VncPropertyDialog.h VncPropertyDialog_symbols.h WinShare.h \
	X11PropertyDialog.h X11PropertyDialog_symbols.h md5.h MxSession.h

PROGRAM = mxclient

bootstrap:
	gettextize || :
	aclocal -I m4
	automake --add-missing
	autoconf

ACLOCAL_AMFLAGS = -I m4

distclean-local:
	rm -f po/*~ *.bak *~ \#.* .#*

all-local: mxclient.rsc

mxclient.rsc: res/mxclient.xrc $(mxclient_SOURCES) $(noinst_HEADERS)
	zip -q9r mxclient.rsc res -i *.png -i *.xrc -i *.ico

EXTRA_DIST = conf/config.rpath conf/mkinstalldirs  conf/config.rpath conf/mkinstalldirs mxclient.pjd mxclient.dsw \
	mxclient.dsp mxclient.rc res/mxclient.rc res/mxclient.xrc \
	Translations.dsp PackedResources.dsp
