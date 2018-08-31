#!/bin/bash

SOURCE_PREFIX=/usr/local/x86_64-netflix-linux-gnu-5.0/netflix
TARGET_PREFIX=$PWD/staging/usr

mkdir -p $TARGET_PREFIX/lib
mkdir -p $TARGET_PREFIX/include

ls -d $SOURCE_PREFIX/include/* | grep -v '/playready' | xargs -n1 -i cp -vr {} $TARGET_PREFIX/include
ls -d $SOURCE_PREFIX/lib/* | grep -v 'playready' | xargs -n1 -i cp -vr {} $TARGET_PREFIX/lib

cp /usr/lib/i386-linux-gnu/libX11.so.6.3.0 $TARGET_PREFIX/lib
cd $TARGET_PREFIX/lib
ln -s libX11.so.6.3.0 libX11.so


