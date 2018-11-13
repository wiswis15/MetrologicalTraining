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
$GXX -c $INC ../eclipse/src/OutOfProcessImplementation.cpp -o OutOfProcessImplementation.o
$GXX -c $INC ../eclipse/src/OutOfProcessTest.cpp -o OutOfProcessTest.o

$GXX -shared -o libWPEFrameworkOutOfProcessTest.so Module.o OutOfProcessImplementation.o OutOfProcessTest.o
chmod 0644 libWPEFrameworkOutOfProcessTest.so

cp ../configure/OutOfProcessTest.json $PREFIX_DIR/../etc/WPEFramework/plugins
cp libWPEFrameworkOutOfProcessTest.so $PREFIX_DIR/lib/wpeframework/plugins

