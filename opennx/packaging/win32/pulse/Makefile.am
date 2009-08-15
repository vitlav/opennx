EXTRA_DIST = urls.txt patches $(LIBOIL).md5sum $(LIBSNDFILE).md5sum \
	$(LIBSAMPLERATE).md5sum $(LIBATOMICOPS).md5sum $(PULSEAUDIO).md5sum

CLEANFILES = -r $(TMPDIST) pkg-config \
	$(LIBOIL) $(LIBOIL)-$(LIBOIL_VERSION) $(LIBOIL)-build-$(PULSE_BTYPE) \
	$(LIBOIL)-srcdir $(LIBOIL)-checkarchive \
	$(LIBSAMPLERATE) $(LIBSAMPLERATE)-$(LIBSAMPLERATE_VERSION) $(LIBSAMPLERATE)-build-$(PULSE_BTYPE) \
	$(LIBSAMPLERATE)-srcdir $(LIBSAMPLERATE)-checkarchive \
	$(LIBSNDFILE) $(LIBSNDFILE)-$(LIBSNDFILE_VERSION) $(LIBSNDFILE)-build-$(PULSE_BTYPE) \
	$(LIBSNDFILE)-srcdir $(LIBSNDFILE)-checkarchive \
	$(LIBATOMICOPS) $(LIBATOMICOPS)-$(LIBATOMICOPS_VERSION) $(LIBATOMICOPS)-build-$(PULSE_BTYPE) \
	$(LIBATOMICOPS)-srcdir $(LIBATOMICOPS)-checkarchive \
	$(PULSEAUDIO) $(PULSEAUDIO)-$(PULSEAUDIO_VERSION) $(PULSEAUDIO)-build-$(PULSE_BTYPE) \
	$(PULSEAUDIO)-srcdir $(PULSEAUDIO)-checkarchive

MAINTAINERCLEANFILES = Makefile.in $(LIBOIL)-$(LIBOIL_VERSION).tar.gz \
	$(LIBSNDFILE)-$(LIBSNDFILE_VERSION).tar.gz $(LIBSAMPLERATE)-$(LIBSAMPLERATE_VERSION).tar.gz \
	$(LIBATOMICOPS)-$(LIBATOMICOPS_VERSION).tar.gz $(PULSEAUDIO)-$(PULSEAUDIO_VERSION).tar.gz

LIBOIL                = liboil
LIBOIL_VERSION        = 0.3.16
LIBSNDFILE            = libsndfile
LIBSNDFILE_VERSION    = 1.0.20
LIBSAMPLERATE         = libsamplerate
LIBSAMPLERATE_VERSION = 0.1.7
LIBATOMICOPS          = libatomic_ops
LIBATOMICOPS_VERSION  = 1.2
PULSEAUDIO            = pulseaudio
PULSEAUDIO_VERSION    = 0.9.6

TMPDIST = $(abs_builddir)/dist

CFG_GENERIC = --prefix=$(TMPDIST) --libdir=$(TMPDIST)/lib --bindir=$(TMPDIST)/bin \
			 --mandir=$(TMPDIST)/share/man --includedir=$(TMPDIST)/include \
			 --datadir=$(TMPDIST)/share --exec-prefix=$(TMPDIST)/libexec \
			 --libexecdir=$(TMPDIST)/libexec --sysconfdir=$(TMPDIST)/etc

CFG_liboil = --disable-glib --with-html-dir=$(TMPDIST)/html
CFG_libsndfile =
CFG_libsamplerate =
CFG_libatomicops =
CFG_pulseaudio = --enable-ltdl-install --without-x --enable-static-bins

POSTCFG_liboil = true
POSTCFG_libsndfile = true
POSTCFG_libsamplerate = true
POSTCFG_libatomicops = true
POSTCFG_pulseaudio = cd $(subsrc); test -f libtool || cp libltdl/libtool .

ENV_liboil =
ENV_libsndfile =
ENV_libsamplerate = env SNDFILE_CFLAGS="-I$(TMPDIST)/include" SNDFILE_LIBS="-L$(TMPDIST)/lib -lsndfile"
ENV_libatomicops =
ENV_pulseaudio = env LIBSNDFILE_CFLAGS="-I$(TMPDIST)/include" LIBSNDFILE_LIBS="-L$(TMPDIST)/lib -lsndfile" \
	LIBSAMPLERATE_CFLAGS="-I$(TMPDIST)/include" LIBSAMPLERATE_LIBS="-L$(TMPDIST)/lib -lsamplerate" \
	LIBOIL_CFLAGS="-I$(TMPDIST)/include/liboil-0.3" LIBOIL_LIBS="-L$(TMPDIST)/lib -loil-0.3" \
	CPPFLAGS="-I$(TMPDIST)/include" PATH="$(abs_builddir):$(PATH)"

tmpinstall:
	$(MAKE) subtarget=$(LIBOIL) subsrc=$(LIBOIL)-$(LIBOIL_VERSION) build-$(PULSE_BTYPE)
	$(MAKE) subtarget=$(LIBSNDFILE) subsrc=$(LIBSNDFILE)-$(LIBSNDFILE_VERSION) build-$(PULSE_BTYPE)
	$(MAKE) subtarget=$(LIBSAMPLERATE) subsrc=$(LIBSAMPLERATE)-$(LIBSAMPLERATE_VERSION) build-$(PULSE_BTYPE)
	$(MAKE) subtarget=$(LIBATOMICOPS) subsrc=$(LIBATOMICOPS)-$(LIBATOMICOPS_VERSION) build-$(PULSE_BTYPE)
	@echo "#!/bin/sh" > pkg-config; \
		echo "case \$$* in *--exists*) exit 1 ;; *) exit 0 ;; esac" >> pkg-config; \
		chmod a+x pkg-config || true
	$(MAKE) subtarget=$(PULSEAUDIO) subsrc=$(PULSEAUDIO)-$(PULSEAUDIO_VERSION) build-$(PULSE_BTYPE)
	find $(TMPDIST)/lib -name "*.dll" -exec mv {} $(TMPDIST)/bin \;
	find $(TMPDIST)/etc -type f -exec mv {} $(TMPDIST)/bin \;
	rm -f $(TMPDIST)/bin/*-sndfile-*.exe $(TMPDIST)/bin/oil-bugreport.exe

build-cross: $(subtarget)-build-cross

build-native: $(subtarget)-build-native

$(subtarget)-build-cross: $(subtarget)-srcdir
	@echo Building $(subtarget)
	@cd $(subsrc) && $(ENV_$(subtarget)) mingw32-configure $(CFG_GENERIC) $(CFG_$(subtarget))
	$(POSTCFG_$(subtarget))
	@$(MAKE) -C $(subsrc) install
	@touch $@

$(subtarget)-build-native: $(subtarget)-srcdir
	@echo Building $(subtarget)
	@cd $(subsrc) && $(ENV_$(subtarget)) ./configure $(CFG_GENERIC) $(CFG_$(subtarget))
	$(POSTCFG_$(subtarget))
	@$(MAKE) -C $(subsrc) install
	@touch $@

$(subtarget)-srcdir: $(subtarget)-checkarchive
	@$(RM) -rf $(subsrc)
	@echo Unpacking $(subsrc).tar.gz
	@tar xzf $(subsrc).tar.gz
	@shopt -s nullglob; for p in $(srcdir)/patches/$(subtarget)-*.patch ; do \
		echo Applying `basename $$p`; \
		patch -p0 < $$p ; \
	done
	@touch $@

$(subtarget)-checkarchive: $(subsrc).tar.gz
	@echo Checking tarball $(subsrc).tar.gz
	@md5sum --status -c $(srcdir)/$(subtarget).md5sum
	@touch $@

$(subsrc).tar.gz: $(srcdir)/urls.txt
	@echo Downloading $@
	@wget $(shell grep $(subsrc).tar.gz $(srcdir)/urls.txt)
	@touch $@
