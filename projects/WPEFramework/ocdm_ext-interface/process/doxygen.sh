#!/bin/bash

rm -rf src.doxygen

mkdir -p src.doxygen/src
mkdir -p src.doxygen/output
mkdir -p src.doxygen/log
mkdir -p src.doxygen/settings

cp Doxyfile src.doxygen/settings
cp ../*.h src.doxygen/src

cd src.doxygen/src

doxygen ../settings/Doxyfile

cd ../output/latex
make

