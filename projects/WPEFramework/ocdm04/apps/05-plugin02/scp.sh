#!/bin/bash

BOX_IP='192.168.1.248'

sshpass -p root scp build_nos/libWPEFrameworkPlugin02.so root@$BOX_IP:/usr/lib/wpeframework/plugins/

sshpass -p root scp configure/Plugin02.json root@$BOX_IP:/etc/WPEFramework/plugins/

cp -v build_nos/libWPEFrameworkPlugin02.so /home/sander/Projects/buildroot/nos/buildroot/output/staging/usr/lib/wpeframework/plugins
cp -v build_nos/libWPEFrameworkPlugin02.so /home/sander/Projects/buildroot/nos/buildroot/output/target/usr/lib/wpeframework/plugins

