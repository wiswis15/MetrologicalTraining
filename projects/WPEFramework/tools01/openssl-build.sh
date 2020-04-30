#!/bin/bash

set -e

PREFIX="$PWD/staging/usr"

cd build/openssl/src

./Configure -m32 linux-generic32 --prefix=$PREFIX

make -j4

# Sometimes needs a few extra "make install" rounds
make install
make install
make install

