Summary: A very fast raytracer named Blizzard III
Name: blz3
Version: 2.99.1
Release: 4pre
Copyright: GPL
Source: blz3.tar.gz
Group: Applications/Multimedia
BuildRoot: /var/tmp/%{name}-buildroot

%description
This is a very fast raytracer developed since the early 1990s.

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
mkdir -p $RPM_BUILD_ROOT/usr/bin
mkdir -p $RPM_BUILD_ROOT/etc/profile.d
mkdir -p $RPM_BUILD_ROOT/usr/share/blizzard

make install
install -m 755 bin/blz3.csh $RPM_BUILD_ROOT/etc/profile.d/blz3.csh

test -f data.tar.gz && gunzip -c data.tar.gz |tar xf - -C $RPM_BUILD_ROOT/usr/share/blizzard

(cd $RPM_BUILD_ROOT; find ./ -type f|cut -b2- >/tmp/blz3-file-list; )

%clean
rm -rf $RPM_BUILD_ROOT
test -f /tmp/blz3-file-list && rm -f /tmp/blz3-file-list

%files -f /tmp/blz3-file-list
%defattr(-,root,root)

%changelog
