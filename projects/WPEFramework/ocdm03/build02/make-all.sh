#!/bin/bash

BASE_DIR=$PWD

cd build/wpeframework/build
make -j4 install
rc=$?
if [[ $rc != 0 ]]; then
   exit $rc
fi
cd $BASE_DIR

cd build/wpeframework-plugins/build
make -j4 install
rc=$?
if [[ $rc != 0 ]]; then
   exit $rc
fi
cd $BASE_DIR

cd build/playready/build
make -j4 install
rc=$?
if [[ $rc != 0 ]]; then
   exit $rc
fi
cd $BASE_DIR

cd build/ocdm-playready/build
make -j4 install
rc=$?
if [[ $rc != 0 ]]; then
   exit $rc
fi
cd $BASE_DIR

cd build/ocdm-null2/build
make -j4 install
rc=$?
if [[ $rc != 0 ]]; then
   exit $rc
fi
cd $BASE_DIR

cd build/ocdmtestapp03/build
make -j4 install
rc=$?
if [[ $rc != 0 ]]; then
   exit $rc
fi
cd $BASE_DIR

python/updateConfig.py

