TARBALL = Xming_69031_source.tar.bz2
DLURL = http://sourceforge.net/projects/xming/files/Xming-source/6.9.0.31/Xming_69031_source.tar.bz2/download
TOPLEVEL = $(shell pwd)
TMPDIST = $(TOPLEVEL)/dist

all: tmpinstall

cleanbuild: tmpinstall cleansrc

buildfromscratch: distclean tmpinstall

cleansrc:
	rm -rf xc ccbin

clean: cleansrc
	rm -f tmpinstall buildxc patchedsrc srcdir checktarball ccdir
	rm -rf dist

distclean: clean
	rm -f $(TARBALL)

tmpinstall: buildxc
	@echo installing Xming to $(TMPDIST)
	@rm -rf $(TMPDIST)
	@mkdir $(TMPDIST)
	@make -C xc install CROSSCOMPILEDIR=$(TOPLEVEL)/ccbin DESTDIR=$(TMPDIST)
	@touch $@
	@echo applying post install fixes
	@for f in $(TMPDIST)/usr/X11R6/bin/* ; do \
		FT="`file $$f`" ; \
		case "$$FT" in \
			*symbolic?link*) rm -f $$f ;; \
			*script*) rm -f $$f ;; \
		esac ; \
	done
	@touch $@

buildxc: ccdir patchedsrc
	@echo building Xming
	@make -C xc World CROSSCOMPILEDIR=$(TOPLEVEL)/ccbin
	@touch $@

patchedsrc: srcdir
	@echo applying build fixes
	@for p in patches/xming-build*.patch ; do \
    	echo applying `basename $$p` ; \
		patch -p0 < $$p ; \
	done
	@cp xc/programs/Xserver/hw/xwin/X.ico xc/programs/xkbcomp/
	@touch $@

srcdir: checktarball
	@rm -rf xc
	@echo unpacking tarball
	@tar xjf $(TARBALL)
	@touch $@

checktarball: $(TARBALL)
	@echo checking tarball $(TARBALL)
	@md5sum --status -c $(TARBALL).md5sum 2>/dev/null
	@touch $@

$(TARBALL):
	@echo downloading tarball $(TARBALL)
	@wget -nv "$(DLURL)"

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

