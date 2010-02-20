tmpinstall:
	mkdir -p dist/bin dist/lib dist/share
	cp '{app}'/bin/*.dll dist/lib
	cp '{app}'/bin/*.dll dist/bin
	for f in $(CYGBINS) ; do \
		cp '{app}'/bin/$$f.exe dist/bin ; \
	done
	for f in fonts keyboards rgb ; do \
		test $(BUILDXMING) = yes || cp -r '{app}'/share/$$f dist/share ; \
	done

CLEANFILES = -r dist
MAINTAINERCLEANFILES = Makefile.in
