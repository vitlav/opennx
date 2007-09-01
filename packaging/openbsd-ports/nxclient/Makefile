# $OpenBSD$

VERSION =					2.1.0
RELEASE =					p9
OPENNX_VERSION =			0.13
NXSUBDIRS =					nxcomp nxcompsh nxssh nxesd nxservice
OPENNXDIR =					opennx
ONLY_FOR_ARCHS =			i386

PKGNAME =					nxclient-${VERSION}${RELEASE}
COMMENT =					"a NoMachine client"
DISTNAME =					nxclient
WRKDIST =					${WRKDIR}
DISTFILES =					nxcomp-${VERSION}-6.tar.gz \
							nxcompsh-${VERSION}-2.tar.gz \
							nxservice-${VERSION}-5.tar.gz \
							nxesd-${VERSION}-2.tar.gz \
							nxssh-${VERSION}-1.tar.gz \
							opennx-${OPENNX_VERSION}.tar.gz:0
CATEGORIES =				net
MAINTAINER =				Fritz Elfert <felfert@users.sf.net>
HOMEPAGE =					http://www.nomachine.com/
USE_X11 =					Yes
USE_GMAKE =					Yes
NO_REGRESS =				Yes
VMEM_WARNING =				Yes
PERMIT_PACKAGE_CDROM =		Yes
PERMIT_PACKAGE_FTP =		Yes
PERMIT_DISTFILES_CDROM =	Yes
PERMIT_DISTFILES_FTP =		Yes
MASTER_SITES =				http://64.34.161.181/download/${VERSION}/sources/
MASTER_SITES0 =				http://prdownloads.sf.net/opennx/

BUILD_DEPENDS =	\
	:png-*:graphics/png \
	:jpeg-*:graphics/jpeg

LIB_DEPENDS = \
	gdk-x11-2.0.>=400.14,gdk_pixbuf-2.0.>=400.14,gtk-x11-2.0.>=400.14::x11/gtk+2 \
	glib-2.0.>=1000::devel/glib2

WANTLIB = \
	ICE SM X11 Xext c jpeg m stdc++ z

EXTRACT_CASES +=		opennx-*.tar.gz) gzip -dc ${FULLDISTDIR}/$$archive | tar -x -s '@^opennx-[0-9][^/]*@opennx@' -f -;;
PATCHORIG =				.orig.port
SHARED_LIBS =			Xcomp 1.5.0
CONFIGURE_STYLE =		autoconf no-autoheader
AUTOCONF_VERSION =		2.59
CONFIGURE_ENV =	\
	CPPFLAGS="-I${LOCALBASE}/include -I${LOCALBASE}/include/libpng -I${X11BASE}/include" \
	CXXFLAGS="-I${LOCALBASE}/include -I${LOCALBASE}/include/libpng -I${X11BASE}/include" \
	LDFLAGS="-L${LOCALBASE}/lib -L${X11BASE}/lib" \
	LIBS="-L${LOCALBASE}/lib -L${X11BASE}/lib"
.for _sd in ${NXSUBDIRS:L}
AUTOCONF_DIR += ${WRKSRC}/${_sd}
.endfor

do-configure:
.for _sd in ${NXSUBDIRS:L}
	${MAKE} WRKDIST=${WRKDIR}/${_sd} my-configure
.endfor
	${MAKE} WRKDIST=${WRKDIR}/${OPENNXDIR} \
	CONFIGURE_ARGS="--prefix=${PREFIX}/NX --enable-static" my-configure

do-build:
.for _sd in ${NXSUBDIRS:L}
	${MAKE} WRKDIST=${WRKDIR}/${_sd} my-build
.endfor
	${MAKE} WRKDIST=${WRKDIR}/${OPENNXDIR} my-build

do-install:
	${INSTALL_PROGRAM_DIR} ${PREFIX}/NX/{bin,lib}
	${INSTALL_DATA_DIR} ${PREFIX}/NX/share
	${INSTALL_PROGRAM} ${WRKBUILD}/nxcomp/libXcomp.so* ${PREFIX}/NX/lib
	${INSTALL_PROGRAM} ${WRKBUILD}/nxcompsh/libXcompsh.so* ${PREFIX}/NX/lib
	${INSTALL_PROGRAM} ${WRKBUILD}/nxssh/nxssh ${PREFIX}/NX/bin
	${INSTALL_PROGRAM} ${WRKBUILD}/nxesd/nxesd ${PREFIX}/NX/bin
	${INSTALL_PROGRAM} ${WRKBUILD}/nxservice/nxservice ${PREFIX}/NX/bin
	${MAKE} WRKDIST=${WRKDIR}/${OPENNXDIR} my-install
	cd ${PREFIX}/bin && \
		ln -sf ../NX/bin/opennx opennx && \
		ln -sf ../NX/bin/opennx nxclient

post-install:
	${INSTALL_DATA_DIR} ${PREFIX}/share/applications
	cd ${PREFIX}/share/applications && \
		ln -snf ../../NX/share/applnk/network-gnome/* .

.include <bsd.port.mk>

my-configure:
.  for _c in ${CONFIGURE_STYLE:L}
.    if defined(MOD${_c:U}_configure)
	@${MOD${_c:U}_configure}
.    endif
.  endfor

my-build:
	@cd ${WRKBUILD} && \
		exec ${_SYSTRACE_CMD} ${SETENV} ${MAKE_ENV} \
		${MAKE_PROGRAM} ${MAKE_FLAGS} -f ${MAKE_FILE} ${ALL_TARGET}

my-install:
	@cd ${WRKBUILD} && \
		exec ${SUDO} ${_SYSTRACE_CMD} ${SETENV} ${MAKE_ENV} ${_FAKE_SETUP} \
		${MAKE_PROGRAM} ${FAKE_FLAGS} -f ${MAKE_FILE} ${FAKE_TARGET}
