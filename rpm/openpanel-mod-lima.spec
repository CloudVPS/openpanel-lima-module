# This file is part of OpenPanel - The Open Source Control Panel
# OpenPanel is free software: you can redistribute it and/or modify it 
# under the terms of the GNU General Public License as published by the Free 
# Software Foundation, using version 3 of the License.
#
# Please note that use of the OpenPanel trademark may be subject to additional 
# restrictions. For more information, please visit the Legal Information 
# section of the OpenPanel website on http://www.openpanel.com/

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
mkdir -p ${BUILD_ROOT}/var/openpanel/modules/LIMA.module
mkdir -p ${BUILD_ROOT}/etc/openpanel
cp -rf ./limamodule.app ${BUILD_ROOT}/var/openpanel/modules/LIMA.module/
cp module.xml techsupport.* ${BUILD_ROOT}/var/openpanel/modules/LIMA.module/
ln -sf limamodule.app/exec ${BUILD_ROOT}/var/openpanel/modules/LIMA.module/action
install -m 755 verify ${BUILD_ROOT}/var/openpanel/modules/LIMA.module/verify

%post
touch /etc/postfix/openpanel/list_transports
touch /etc/postfix/openpanel/list_virtuals
/usr/sbin/postmap /etc/postfix/openpanel/list_transports /etc/postfix/openpanel/list_virtuals
/var/openpanel/bin/postarray add transport_maps hash:/etc/postfix/openpanel/list_transports
/var/openpanel/bin/postarray add virtual_mailbox_maps hash:/etc/postfix/openpanel/list_virtuals

%files
%defattr(-,root,root)
/
