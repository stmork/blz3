%define name blz3
%define ver 2.99.2
%define rel 13

Summary: A very fast raytracer named Blizzard III
Name: %{name}
Version: %{ver}
Release: %{rel}
Copyright: GPL
Source: blz3.tar.gz
Group: Applications/Multimedia
BuildRoot: /var/tmp/%{name}-buildroot

%package devel
Summary: Development environment for Blizzard III
Group: Development/Libraries

%package data
Summary: Data for Blizzard III
Group: Applications/Multimedia

%description
This is a very fast raytracer developed since the early 1990s.

%description devel
This package proviles C++ header files and libraries of Blizzard III.

%description data
This package contains Blizzard data files.

%prep
%setup -q -n %{name}
./configure\
	--prefix=$RPM_BUILD_ROOT\
	--exec-prefix=$RPM_BUILD_ROOT/usr\
	--datadir=$RPM_BUILD_ROOT/usr/share/blizzard

%build
make depend
make -j 3 RPM_OPT_FLAGS="$RPM_OPT_FLAGS"

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/etc/profile.d
mkdir -p $RPM_BUILD_ROOT/usr/bin
mkdir -p $RPM_BUILD_ROOT/usr/include
mkdir -p $RPM_BUILD_ROOT/usr/lib
mkdir -p $RPM_BUILD_ROOT/usr/share/blizzard

make install
install -m 755 bin/blz3.csh $RPM_BUILD_ROOT/etc/profile.d/blz3.csh

test -d $HOME/Blizzard && tar cf - -C $HOME/Blizzard Data Objects Textures Materials Bumps Conditions|tar xf - -C $RPM_BUILD_ROOT/usr/share/blizzard

cp -a include/blz3 $RPM_BUILD_ROOT/usr/include
cp -a include_unix/blz3 $RPM_BUILD_ROOT/usr/include
cp -a lib/lib*.a $RPM_BUILD_ROOT/usr/lib

(cd $RPM_BUILD_ROOT; find ./usr/bin               -type f|cut -b2- >/tmp/blz3-file-list; )
(cd $RPM_BUILD_ROOT; find ./usr/lib ./usr/include -type f|cut -b2- >/tmp/blz3-devel-file-list; )
(cd $RPM_BUILD_ROOT; find ./usr/share             -type f|cut -b2- >/tmp/blz3-data-file-list; )

%files -f /tmp/blz3-file-list
%defattr(-,root,root)

%files devel -f /tmp/blz3-devel-file-list
%defattr(-,root,root)

%files data -f /tmp/blz3-data-file-list
%defattr(-,root,root)
%config /etc/profile.d/blz3.csh

%clean
rm -rf $RPM_BUILD_ROOT /tmp/blz3*-file-list

%changelog
