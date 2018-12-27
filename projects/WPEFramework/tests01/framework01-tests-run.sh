#!/bin/bash

export LD_LIBRARY_PATH="$PWD/staging/usr/lib"

staging/usr/bin/framework01test_core "$@"

