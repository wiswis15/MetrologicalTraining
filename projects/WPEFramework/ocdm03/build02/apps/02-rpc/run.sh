#!/bin/bash

#!/bin/bash

BASE_DIR=$PWD

cd ../../staging/usr

PREFIX_DIR=$PWD

cd $BASE_DIR

export LD_LIBRARY_PATH="$PREFIX_DIR/lib"

build/server &
build/client &

sleep 12

