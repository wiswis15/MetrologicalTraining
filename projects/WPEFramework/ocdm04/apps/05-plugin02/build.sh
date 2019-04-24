#!/bin/bash

#!/bin/bash

BASE_DIR=$PWD

cd ../../staging/usr

PREFIX_DIR=$PWD

cd $BASE_DIR

rm -rf build
mkdir build
cd build

INC="-I$PREFIX_DIR/include -I$PREFIX_DIR/include/WPEFramework"
LIBS="-L$PREFIX_DIR/lib -lWPEFrameworkCore -lWPEFrameworkProtocols -lWPEFrameworkCryptalgo -lWPEFrameworkTracing"
GXX='g++ -m32 -std=c++11 -g -O0'

$GXX -c $INC ../eclipse/src/Module.cpp -o Module.o
$GXX -c $INC ../eclipse/src/Plugin02.cpp -o Plugin02.o

$GXX -shared -o libWPEFrameworkPlugin02.so Module.o Plugin02.o
chmod 0644 libWPEFrameworkPlugin02.so

cp ../configure/Plugin02.json $PREFIX_DIR/../etc/WPEFramework/plugins
cp libWPEFrameworkPlugin02.so $PREFIX_DIR/lib/wpeframework/plugins

