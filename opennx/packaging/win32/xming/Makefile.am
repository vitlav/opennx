TARBALL = Xming_69031_source.tar.bz2
DLURL = http://sourceforge.net/projects/xming/files/Xming-source/6.9.0.31/Xming_69031_source.tar.bz2/download
TMPDIST = $(abs_builddir)/dist

MAINTAINERCLEANFILES = Makefile.in
CLEANFILES = -rf tmpinstall buildxc patchedsrc srcdir checktarball ccdir \
	xc ccbin
DISTCLEANFILES = -rf dist $(TARBALL)
EXTRA_DIST = version.def Xming_69031_source.tar.bz2.md5sum patches

buildfromscratch: distclean tmpinstall

install-cross: build-cross
	@make -C xc install CROSSCOMPILEDIR=$(abs_builddir)/ccbin DESTDIR=$(TMPDIST)

install-native: build-native
	@make -C xc install DESTDIR=$(TMPDIST)

xming-distdir:
	@echo installing Xming to $(TMPDIST)
	@rm -rf $(TMPDIST)
	@mkdir $(TMPDIST)

tmpinstall: xming-distdir install-$(XMING_BTYPE)
	@echo applying post install fixes
	@for f in $(TMPDIST)/usr/X11R6/bin/* ; do \
		FT="`file $$f`" ; \
		case "$$FT" in \
			*symbolic?link*) rm -f $$f ;; \
			*script*) rm -f $$f ;; \
		esac ; \
	done
	@touch $@

build-cross: ccdir patchedsrc
	@echo building Xming
	@make -C xc World CROSSCOMPILEDIR=$(abs_builddir)/ccbin
	@touch $@

build-native: patchedsrc
	@echo building Xming
	@make -C xc World BOOTSTRAPCFLAGS="-DCROSSCOMPILE_CPP"
	@touch $@

patchedsrc: srcdir version.def
	@echo applying build fixes
	@for p in $(srcdir)/patches/xming-build*.patch ; do \
    	echo applying `basename $$p` ; \
		patch -p0 < $$p ; \
	done
	@cp xc/programs/Xserver/hw/xwin/X.ico xc/programs/xkbcomp/
	@touch $@

version.def: $(srcdir)/version.def
	@cp $(srcdir)/version.def $@

srcdir: checktarball
	@rm -rf xc
	@echo unpacking tarball
	@tar xjf $(TARBALL)
	@touch $@

checktarball: $(TARBALL)
	@echo checking tarball $(TARBALL)
	@md5sum --status -c $(srcdir)/$(TARBALL).md5sum 2>/dev/null
	@touch $@

$(TARBALL):
	@echo downloading tarball $(TARBALL)
	@wget "$(DLURL)"

ccdir:
	@echo creating CROSSCOMPILEDIR
	@rm -rf ccbin
	@mkdir ccbin
	@for f in /usr/bin/i686-pc-mingw32-* ; do \
		bn=`basename $$f|sed -e 's/i686-pc-mingw32-//'` ; \
		ln -s $$f ccbin/$$bn ; \
	done
	@ln -s gcc ccbin/cc
	@touch $@

