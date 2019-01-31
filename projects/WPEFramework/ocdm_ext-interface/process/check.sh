#!/bin/bash

mkdir -p src.check

cp ../*.h src.check

gcc -x c -c src.check/open_cdm_ext.h -o /dev/null
