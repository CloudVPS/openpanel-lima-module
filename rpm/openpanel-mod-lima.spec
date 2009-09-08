%define version 0.8.14

%define libpath /usr/lib
%ifarch x86_64
  %define libpath /usr/lib64
%endif

Summary: LIMA mailinglist service configuration module
Name: openpanel-mod-lima
Version: %version
Release: 1
License: GPLv2
Group: Development
Source: http://packages.openpanel.com/archive/openpanel-mod-lima-%{version}.tar.gz
Patch1: openpanel-mod-lima-00-makefile
BuildRoot: /var/tmp/%{name}-buildroot
Requires: openpanel-core >= 0.8.3
Requires: openpanel-mod-user
Requires: openpanel-mod-domain
Requires: openpanel-mod-postfixcourier
Requires: libgrace-querido
Requires: lima

%description
LIMA mailinglist service configuration module
OpenPanel module for driving LIMA on top of Postfix.

%prep
%setup -q -n openpanel-mod-lima-%version
%patch1 -p0 -b .buildroot

%build
BUILD_ROOT=$RPM_BUILD_ROOT
./configure
make

%install
BUILD_ROOT=$RPM_BUILD_ROOT
rm -rf ${BUILD_ROOT}
mkdir -p ${BUILD_ROOT}/var/opencore/modules/LIMA.module
mkdir -p ${BUILD_ROOT}/etc/openpanel
cp -rf ./limamodule.app ${BUILD_ROOT}/var/opencore/modules/LIMA.module/
cp module.xml techsupport.* ${BUILD_ROOT}/var/opencore/modules/LIMA.module/
ln -sf limamodule.app/exec ${BUILD_ROOT}/var/opencore/modules/LIMA.module/action
install -m 755 verify ${BUILD_ROOT}/var/opencore/modules/LIMA.module/verify

%post
touch /etc/postfix/openpanel/list_transports
touch /etc/postfix/openpanel/list_virtuals
/usr/sbin/postmap /etc/postfix/openpanel/list_transports /etc/postfix/openpanel/list_virtuals
/var/opencore/bin/postarray add transport_maps hash:/etc/postfix/openpanel/list_transports
/var/opencore/bin/postarray add virtual_mailbox_maps hash:/etc/postfix/openpanel/list_virtuals

%files
%defattr(-,root,root)
/
