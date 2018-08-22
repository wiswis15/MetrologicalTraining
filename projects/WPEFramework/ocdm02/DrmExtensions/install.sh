#!/bin/bash

BUILDROOT_DIR=/home/sander/Projects/buildroot/nos/buildroot.master
BUILDROOT_INCLUDE="$BUILDROOT_DIR/output/staging/usr/include"

mkdir -p $BUILDROOT_INCLUDE/drm-extensions
cp -v *.h $BUILDROOT_INCLUDE/drm-extensions

