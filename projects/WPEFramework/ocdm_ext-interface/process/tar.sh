#!/bin/bash

rm -rf tar

mkdir -p tar/ocdm/include
cp ../*.h tar/ocdm/include

mkdir -p tar/ocdm/pdf
cp src.doxygen/output/latex/refman.pdf tar/ocdm/pdf/ocdm.pdf

cp -r src.doxygen/output/html tar/ocdm

cd tar

tar czf ocdm.tgz ocdm



