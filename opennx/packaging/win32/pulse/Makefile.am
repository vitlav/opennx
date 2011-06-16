LIBSNDFILE            = libsndfile
LIBSNDFILE_VERSION    = 1.0.24
LIBSAMPLERATE         = libsamplerate
LIBSAMPLERATE_VERSION = 0.1.7
LIBATOMICOPS          = libatomic_ops
LIBATOMICOPS_VERSION  = 1.2
JSONC				  = json-c
JSONC_VERSION		  = 0.9
SPEEX                 = speex
SPEEX_VERSION         = 1.2rc1
PULSEAUDIO            = pulseaudio
PULSEAUDIO_VERSION    = 1.0-dev-366-g7d918

TMPDIST = $(abs_builddir)/dist

EXTRA_DIST = urls.txt patches $(LIBSNDFILE).md5sum $(SPEEX).md5sum $(JSONC).md5sum \
	$(LIBSAMPLERATE).md5sum $(LIBATOMICOPS).md5sum $(PULSEAUDIO).md5sum

CLEANFILES = -r $(TMPDIST) pkg-config \
	$(LIBSAMPLERATE) $(LIBSAMPLERATE)-$(LIBSAMPLERATE_VERSION) $(LIBSAMPLERATE)-build-$(PULSE_BTYPE) \
	$(LIBSAMPLERATE)-srcdir $(LIBSAMPLERATE)-checkarchive \
	$(LIBSNDFILE) $(LIBSNDFILE)-$(LIBSNDFILE_VERSION) $(LIBSNDFILE)-build-$(PULSE_BTYPE) \
	$(LIBSNDFILE)-srcdir $(LIBSNDFILE)-checkarchive \
	$(LIBATOMICOPS) $(LIBATOMICOPS)-$(LIBATOMICOPS_VERSION) $(LIBATOMICOPS)-build-$(PULSE_BTYPE) \
	$(LIBATOMICOPS)-srcdir $(LIBATOMICOPS)-checkarchive \
	$(JSONC) $(JSONC)-$(JSONC_VERSION) $(JSONC)-build-$(PULSE_BTYPE) \
	$(JSONC)-srcdir $(JSONC)-checkarchive \
	$(SPEEX) $(SPEEX)-$(SPEEX_VERSION) $(SPEEX)-build-$(PULSE_BTYPE) \
	$(SPEEX)-srcdir $(SPEEX)-checkarchive \
	$(PULSEAUDIO) $(PULSEAUDIO)-$(PULSEAUDIO_VERSION) $(PULSEAUDIO)-build-$(PULSE_BTYPE) \
	$(PULSEAUDIO)-srcdir $(PULSEAUDIO)-checkarchive

MAINTAINERCLEANFILES = Makefile.in \
	$(LIBSNDFILE)-$(LIBSNDFILE_VERSION).tar.gz $(LIBSAMPLERATE)-$(LIBSAMPLERATE_VERSION).tar.gz \
	$(LIBATOMICOPS)-$(LIBATOMICOPS_VERSION).tar.gz $(PULSEAUDIO)-$(PULSEAUDIO_VERSION).tar.gz \
	$(SPEEX)-$(SPEEX_VERSION).tar.gz $(JSONC)-$(JSONC_VERSION).tar.gz

CFG_GENERIC = --prefix=$(TMPDIST) --libdir=$(TMPDIST)/lib --bindir=$(TMPDIST)/bin \
			 --mandir=$(TMPDIST)/share/man --includedir=$(TMPDIST)/include \
			 --datadir=$(TMPDIST)/share --exec-prefix=$(TMPDIST)/libexec \
			 --libexecdir=$(TMPDIST)/libexec --sysconfdir=$(TMPDIST)/etc

CFG_speex = --enable-sse --enable-shared --disable-static
CFG_jsonc = --enable-shared --disable-static
CFG_libsndfile = --enable-shared --disable-static
CFG_libsamplerate = --enable-shared --disable-static 
CFG_libatomicops = --enable-shared --disable-static
CFG_pulseaudio = --enable-shared --disable-static --disable-glib2 --disable-openssl

PRECFG_speex = true
PRECFG_jsonc = autoreconf -f -i
PRECFG_libsndfile = true
PRECFG_libsamplerate = true
PRECFG_libatomic_ops = true
PRECFG_pulseaudio = autoreconf -f -i

POSTCFG_speex = true
POSTCFG_jsonc = true
POSTCFG_libsndfile = true
POSTCFG_libsamplerate = true
POSTCFG_libatomi_cops = true
POSTCFG_pulseaudio = true

ENV_speex =
ENV_jsonc =
ENV_libsndfile =
ENV_libsamplerate = env SNDFILE_CFLAGS="-I$(TMPDIST)/include" SNDFILE_LIBS="-L$(TMPDIST)/lib -lsndfile"
ENV_libatomi_cops =
ENV_pulseaudio = env LIBSNDFILE_CFLAGS="-I$(TMPDIST)/include" LIBSNDFILE_LIBS="-L$(TMPDIST)/lib -lsndfile" \
	LIBSAMPLERATE_CFLAGS="-I$(TMPDIST)/include" LIBSAMPLERATE_LIBS="-L$(TMPDIST)/lib -lsamplerate" \
	LIBJSON_CFLAGS="-I$(TMPDIST)/include/json" LIBJSON_LIBS="-L$(TMPDIST)/lib -ljson" \
	LIBSPEEX_CFLAGS="-I$(TMPDIST)/include" LIBSPEEX_LIBS="-L$(TMPDIST)/lib -lspeex -lspeexdsp" \
	CPPFLAGS="-I$(TMPDIST)/include" PATH="$(abs_builddir):$(PATH)"

tmpinstall:
	$(MAKE) subtarget=$(SPEEX) subvar=$(SPEEX) subsrc=$(SPEEX)-$(SPEEX_VERSION) build-$(PULSE_BTYPE)
	$(MAKE) subtarget=$(JSONC) subvar=jsonc subsrc=$(JSONC)-$(JSONC_VERSION) build-$(PULSE_BTYPE)
	$(MAKE) subtarget=$(LIBSNDFILE) subvar=$(LIBSNDFILE) subsrc=$(LIBSNDFILE)-$(LIBSNDFILE_VERSION) build-$(PULSE_BTYPE)
	$(MAKE) subtarget=$(LIBSAMPLERATE) subvar=$(LIBSAMPLERATE) subsrc=$(LIBSAMPLERATE)-$(LIBSAMPLERATE_VERSION) build-$(PULSE_BTYPE)
	$(MAKE) subtarget=$(LIBATOMICOPS) subvar=$(LIBATOMICOPS) subsrc=$(LIBATOMICOPS)-$(LIBATOMICOPS_VERSION) build-$(PULSE_BTYPE)
	@echo "#!/bin/sh" > pkg-config; \
		echo "case \$$* in *--exists*) exit 1 ;; *) exit 0 ;; esac" >> pkg-config; \
		chmod a+x pkg-config || true
	$(MAKE) subtarget=$(PULSEAUDIO) subvar=$(PULSEAUDIO) subsrc=$(PULSEAUDIO)-$(PULSEAUDIO_VERSION) build-$(PULSE_BTYPE)
	rm -rf $(TMPDIST)/share  $(TMPDIST)/libexec $(TMPDIST)/include $(TMPDIST)/lib/pkgconfig
	-mv $(TMPDIST)/lib/pulse-1.0/bin/*.dll $(TMPDIST)/bin
	rm -rf  $(TMPDIST)/lib/pulse-1.0/bin  $(TMPDIST)/bin/esdcompat
	find $(TMPDIST) -name "*.la" -o -name "*.a"|xargs rm -f
	rm -f $(TMPDIST)/bin/*-sndfile-*.exe $(TMPDIST)/bin/speex*.exe

build-cross: $(subtarget)-build-cross

build-native: $(subtarget)-build-native

$(subtarget)-build-cross: $(subtarget)-srcdir
	@echo Building $(subtarget)
	@cd $(subsrc) && $(PRECFG_$(subvar)) && $(ENV_$(subvar)) mingw32-configure $(CFG_GENERIC) $(CFG_$(subvar))
	$(POSTCFG_$(subvar))
	@$(MAKE) -C $(subsrc) install
	@touch $@

$(subtarget)-build-native: $(subtarget)-srcdir
	@echo Building $(subtarget)
	@cd $(subsrc) && $(ENV_$(subvar)) ./configure $(CFG_GENERIC) $(CFG_$(subvar))
	$(POSTCFG_$(subvar))
	@$(MAKE) -C $(subsrc) install
	@touch $@

$(subtarget)-srcdir: $(subtarget)-checkarchive
	@$(RM) -rf $(subsrc)
	@echo Unpacking $(subsrc).tar.gz
	@tar xzf $(subsrc).tar.gz
	@shopt -s nullglob; for p in $(srcdir)/patches/$(subvar)-*.patch ; do \
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
	@$(WGET) -nc $(shell grep $(subsrc).tar.gz $(srcdir)/urls.txt)
	@touch $@
