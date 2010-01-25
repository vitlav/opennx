# Enable USBIP support
%define with_usbip %{?_with_usbip: 1} %{?!_with_usbip: 0}

Summary: An OpenSource NX client
Name: opennx
Version: 0.16
Release: alt6.svn450
License: LGPL/GPL
Group: Networking/Remote access
Url: http://sourceforge.net/projects/opennx
Packager: Boris Savelev <boris@altlinux.org>

Source: %name-%version.tar

%if %with_usbip
Requires: usbip2-nxclient
%endif

# Automatically added by buildreq on Sat Sep 19 2009
BuildRequires: gcc-c++ imake libSM-devel libXmu-devel nx ImageMagick-tools
BuildRequires: libopensc-devel libsmbclient-devel
BuildRequires: libwxGTK-devel xorg-cf-files zip

%description
opennx is an OSS replacement for Nomachine's NX client.

%prep
%setup

test -d conf || mkdir conf
#Convince gettextize not to modify EXTRA_DIST
test -f conf/config.rpath || touch conf/config.rpath
#Convince gettextize not to modify AC_CONFIG_FILES
test -f po/Makefile.in.in || touch po/Makefile.in.in
#Tweak gettextize
#Stupid gettextize uses /dev/tty for interactively getting a
#confirm of it's "oh so incredibly important notes".
#YES, i've read them at least a dozen times and now, i REALLY
#don't want to hit RETURN anymore!
GETTEXTIZE=`which gettextize`
test -n "$GETTEXTIZE" && \
    sed -e 's@/dev/tty@/dev/null@' "$GETTEXTIZE" > gettextize.local
test -f gettextize.local && sh gettextize.local -f --no-changelog
rm -f gettextize.local
test -e conf/mkinstalldirs || touch conf/mkinstalldirs
%autoreconf

%build
%configure \
    --bindir=%_libdir/%name/bin \
    --datadir=%_datadir/%name \
    --localedir=%_datadir/locale \
%if %with_usbip
    --enable-usbip \
%endif
    --with-nxproto=3.3.0

%make_build

%install
%makeinstall_std

mkdir -p %buildroot{%_bindir,%_desktopdir}

ln -s ../..%_libdir/%name/bin/%name %buildroot%_bindir/%name
ln -s ../../share/%name %buildroot%_libdir/%name/share
for f in nxesd nxssh nxservice nxproxy ; do
    ln -s  ../../../bin/$f %buildroot%_libdir/%name/bin/$f
done

mkdir -p %buildroot%_libdir/%name/lib

for lib in libsmbclient.so libcups.so ; do
    ln -s %_libdir/`readlink %_libdir/$lib` %buildroot%_libdir/%name/lib/$lib
done

cp %buildroot%_datadir/%name/applnk/xdg/*.desktop %buildroot%_desktopdir
rm -rf %buildroot%_datadir/%name/applnk
rm -rf %buildroot%_datadir/%name/icons
subst "s|/usr/NX/bin|%_bindir|g" %buildroot%_desktopdir/*

install -d %buildroot{%_niconsdir,%_miconsdir,%_liconsdir}
for f in nx opennx-admin opennx-wizard ; do
    convert -size 16x16 ./extres/scalable/apps/$f.svg %buildroot%_miconsdir/$f.png
    convert -size 32x32 ./extres/scalable/apps/$f.svg %buildroot%_niconsdir/$f.png
    convert -size 48x48 ./extres/scalable/apps/$f.svg %buildroot%_liconsdir/$f.png
done

%if %with_usbip
install -d -m 755 %buildroot%_sysconfdir/udev/rules.d
install -m 644 etc/*.rules %buildroot%_sysconfdir/udev/rules.d
%endif

%find_lang %name

%if %with_usbip
%pre
%_sbindir/groupadd -r opennx || :
%endif

%files -f %name.lang
%doc COPYING INSTALL
%_bindir/%name
%_libdir/%name
%_datadir/%name
%_desktopdir/*.desktop
%_liconsdir/*.png
%_niconsdir/*.png
%_miconsdir/*.png
%if %with_usbip
%_sysconfdir/udev
%endif

%changelog
* Mon Jan 25 2010 Boris Savelev <boris@altlinux.org> 0.16-alt6.svn450
- update from trunk (work with proxy with authorization)

* Sat Jan 16 2010 Boris Savelev <boris@altlinux.org> 0.16-alt5.svn446
- update from trunk
- add russian localization

* Thu Nov 12 2009 Boris Savelev <boris@altlinux.org> 0.16-alt4.svn444
- update from trunk
- add symlink for nxproxy

* Sun Oct 11 2009 Boris Savelev <boris@altlinux.org> 0.16-alt3.svn442
- update from trunk
- add symlinks for cups and samba

* Thu Sep 24 2009 Boris Savelev <boris@altlinux.org> 0.16-alt2.svn427
- update buildreq
- fix repocop warning

* Sat Sep 19 2009 Boris Savelev <boris@altlinux.org> 0.16-alt1.svn418
- intial build for Sisyphus

* Sun Apr 19 2009 Fritz Elfert <fritz@fritz-elfert.de>
- Set prefix to /opt/lsb/%name for FHS compliance
* Wed Apr 15 2009 Michael Kromer <michael.kromer@millenux.com>
- Fixes for SuSE Plattform (openSuSE/SLES)
* Sun Jan  7 2007 Fritz Elfert <fritz@fritz-elfert.de>
- Initial package
