#!/bin/bash

set -e

cd ../../..

mkdir -p staging/persistent-path/OCDM/playready/state

ln -sf $PWD/staging/etc/playready staging/persistent-path/OCDM/playready/playready

mkdir -p staging/tmp

ln -sf $PWD/staging/tmp staging/etc/playready/storage

