Summary: A very fast raytracer named Blizzard III
Name: blz3
Version: 2.99.1
Release: 3
Copyright: GPL
Source: blz3.tar.gz
Group: Applications/Multimedia
BuildRoot: /var/tmp/%{name}-buildroot

%description
This is a very fast raytracer developed since the early 1990s.

%prep
%setup -q -n %{name}
./configure

%build
make depend
make RPM_OPT_FLAGS="$RPM_OPT_FLAGS"

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/usr/bin

install -s -m 755 bin/brt3 $RPM_BUILD_ROOT/usr/bin/brt3

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)

/usr/bin/brt3

%changelog
