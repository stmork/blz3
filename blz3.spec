%define name blz3
%define ver 2.99.2
%define rel 19

Summary: A very fast raytracer named Blizzard III
Name: %{name}
Version: %{ver}
Release: %{rel}
Copyright: Copyright (C) Steffen A. Mork
Source: blz3.tar.gz
Group: Productivity/Graphics/Visualization/Raytracers
BuildRoot: /var/tmp/%{name}-buildroot
URL: http://www.morknet.de/BlizzardIII/
Vendor: MORKNet Dortmund

%package devel
Summary: Development environment for Blizzard III
Group: Development/Libraries

%package data
Summary: Data for Blizzard III
Group: Productivity/Graphics/Visualization/Raytracers

%package divx
Summary: Tool for creating DivX movies from single frames.
Group: Applications/Multimedia

%description
This is a very fast raytracer developed since the early 1990s. It
contains some tools for image conversion.

%description devel
This package proviles C++ header files and libraries of Blizzard III.

%description data
This package contains Blizzard data files.

%description divx
This package contains a tool for creating DivX movies from
single images.

%prep
%setup -q -n %{name}
./configure\
	--prefix=$RPM_BUILD_ROOT\
	--exec-prefix=$RPM_BUILD_ROOT/usr\
	--datadir=$RPM_BUILD_ROOT%_datadir/blizzard

%build
make depend
make -j 3 RPM_OPT_FLAGS="$RPM_OPT_FLAGS"

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/etc/profile.d
mkdir -p $RPM_BUILD_ROOT%_bindir
mkdir -p $RPM_BUILD_ROOT%_includedir
mkdir -p $RPM_BUILD_ROOT%_libdir
mkdir -p $RPM_BUILD_ROOT%_datadir/blizzard

make install
install -m 755 bin/blz3.csh $RPM_BUILD_ROOT/etc/profile.d/blz3.csh
install -m 755 bin/blz3.sh  $RPM_BUILD_ROOT/etc/profile.d/blz3.sh

test -d $HOME/Blizzard && tar chf - -C $HOME/Blizzard Data Objects Textures Materials Bumps Conditions|tar xf - -C $RPM_BUILD_ROOT/usr/share/blizzard

cp -a include/blz3      $RPM_BUILD_ROOT/usr/include
cp -a include_unix/blz3 $RPM_BUILD_ROOT/usr/include
cp -a lib/lib*.a        $RPM_BUILD_ROOT%_libdir

(cd $RPM_BUILD_ROOT; find .%_bindir               -type f | fgrep -v divx | cut -b2- >/tmp/blz3-file-list; )
(cd $RPM_BUILD_ROOT; find .%_libdir .%_includedir -type f |                 cut -b2- >/tmp/blz3-devel-file-list; )
(cd $RPM_BUILD_ROOT; find .%_datadir              -type f |                 cut -b2- >/tmp/blz3-data-file-list; )

%files -f /tmp/blz3-file-list
%defattr(-,root,root)

%files devel -f /tmp/blz3-devel-file-list
%defattr(-,root,root)

%files data -f /tmp/blz3-data-file-list
%defattr(-,root,root)
%config /etc/profile.d/blz3.csh
%config /etc/profile.d/blz3.sh

%files divx
%defattr(-,root,root)
%_bindir/divx

%clean
rm -rf $RPM_BUILD_ROOT /tmp/blz3*-file-list

%changelog
