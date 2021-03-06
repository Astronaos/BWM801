Name:           bwm801
Version:        1.0.2
Release:        1%{?dist}
Summary:        Software / Video Game Application Framework and Engine

License:        LGPLv3+
URL:            https://github.com/Astronaos/BWM801
Source0:        https://astronaos.com/bwm801/bwm801-1.0.2.tar.gz

#The versions listed for requires here are based on what is on my developer
# system; the code may work with earlier versions of each of these libraries.
# If you have a system with an older version of one of these libraries and
# cannot update it, please try building the source; if you find that it
# compiles correctly, let me know and I will make the update to the RPM, or
# please generate a pull request on github
BuildRequires:  libX11-devel >= 1.6 freetype-devel >= 2.2 ftgl-devel >= 2.1  mesa-libGL-devel >= 10.0 mesa-libGLU-devel >= 9.0 libpng-devel >= 1.5
Requires:       libX11 >= 1.6 freetype >= 2.2 ftgl >= 2.1 mesa-libGL >= 10.0 mesa-libGLU >= 9.0 libpng >= 1.5

%description
Launcher for the bwm801 software / video game engine.

%package        devel
Summary:        Development needs for building apps that use the bwm801 engine.
Group:          Development/Libraries
Requires:       %{name} = %{version}-%{release}

%description    devel
This package contains headers and libraries required to build applications that
use the bwm801 engine.


%prep
%autosetup


%build
%configure
%make_build


%install
rm -rf $RPM_BUILD_ROOT
%make_install


%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig


%files
%defattr(-,root,root,-)
%license LICENSE
%doc README.md
%{_bindir}/*
%{_libdir}/*.so.*

%files devel
%defattr(-,root,root,-)
#%%doc examples
%license LICENSE
%doc README.md
%{_includedir}/*.h
%{_libdir}/*.a
%{_libdir}/*.la
%{_libdir}/*.so
%{_libdir}/pkgconfig/*.pc


%changelog
* Thu Sep 27 2018 Brian W. Mulligan <bwmulligan@astronaos.com>
- Initial RPM release


