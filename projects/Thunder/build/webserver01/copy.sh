#!/bin/bash

set -e

SRC_DIR=/home/sander/Projects/WPEFramework/build/webserver01

rm -rf build
mkdir build
cd build

cp $SRC_DIR/*.sh .
cp -r $SRC_DIR/python .

BASE_DIR=$PWD

HASH_FILE=$BASE_DIR/hashes.txt

cd $SRC_DIR/repos

for dir in *
do
  if [ -d $dir ]; then
     cd $dir
     repoHash=$(git rev-parse HEAD)
     cd ..
     echo "$dir $repoHash" >> $HASH_FILE
  fi
done

cd $BASE_DIR

