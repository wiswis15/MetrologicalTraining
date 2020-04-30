#!/bin/bash

mkdir -p zips
cd zips

rm -f openssl.tgz
wget -O openssl.tgz https://www.openssl.org/source/openssl-1.0.2k.tar.gz

