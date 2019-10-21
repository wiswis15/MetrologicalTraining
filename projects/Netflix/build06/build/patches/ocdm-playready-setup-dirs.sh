#!/bin/bash

set -e

cd ../../..

mkdir -p staging/persistent-path/OCDM

rm -rf staging/persistent-path/OCDM/playready

ln -sf $PWD/staging/etc/playready staging/persistent-path/OCDM/playready

mkdir -p staging/etc/playready/storage
mkdir -p staging/etc/playready/state

