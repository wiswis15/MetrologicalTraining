#!/bin/bash

#rm -rf zips
#./netflix5-fetch.sh

rm -rf build staging

./copy-netflix5-deps.sh

./netflix5-unzip.sh
./netflix5-patch.sh
./netflix5-build.sh


