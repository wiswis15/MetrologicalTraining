#!/bin/bash

if [[ -v MAKE_PROC_COUNT ]]; then
   EXTRA_MAKE_ARG="-j$MAKE_PROC_COUNT"
fi

make -C build/wpeframework/build $EXTRA_MAKE_ARG install


