#!/bin/bash

GXX=g++
AS=as
NM=nm

# 1: no nameless namespace
$GXX -c -S 01-no_nameless/file01.cpp -o 01-no_nameless/file01.s
$AS 01-no_nameless/file01.s -o 01-no_nameless/file01.o
$NM 01-no_nameless/file01.o > 01-no_nameless/file01.symbols.txt

$GXX -c -S 01-no_nameless/file02.cpp -o 01-no_nameless/file02.s
$AS 01-no_nameless/file02.s -o 01-no_nameless/file02.o
$NM 01-no_nameless/file02.o > 01-no_nameless/file02.symbols.txt

$GXX -c 01-no_nameless/main.cpp -o 01-no_nameless/main.o

# Here we see order during linking matters
$GXX 01-no_nameless/main.o 01-no_nameless/file01.o 01-no_nameless/file02.o -o 01-no_nameless/build_01_02
$GXX 01-no_nameless/main.o 01-no_nameless/file02.o 01-no_nameless/file01.o -o 01-no_nameless/build_02_01

# 3: nameless namespace
$GXX -c 03-nameless/file01.cpp -o 03-nameless/file01.o
$NM 03-nameless/file01.o > 03-nameless/file01.symbols.txt

$GXX -c 03-nameless/file02.cpp -o 03-nameless/file02.o
$NM 03-nameless/file02.o > 03-nameless/file02.symbols.txt

$GXX -c 03-nameless/main.cpp -o 03-nameless/main.o

# Here we see order during linking matters
$GXX 03-nameless/main.o 03-nameless/file01.o 03-nameless/file02.o -o 03-nameless/build_01_02
$GXX 03-nameless/main.o 03-nameless/file02.o 03-nameless/file01.o -o 03-nameless/build_02_01

# 4: different class name
$GXX -c 04-class_name/file01.cpp -o 04-class_name/file01.o
$GXX -c 04-class_name/file02.cpp -o 04-class_name/file02.o
$GXX -c 04-class_name/main.cpp -o 04-class_name/main.o

$GXX 04-class_name/main.o 04-class_name/file01.o 04-class_name/file02.o -o 04-class_name/build_01_02
$GXX 04-class_name/main.o 04-class_name/file02.o 04-class_name/file01.o -o 04-class_name/build_02_01

# 5: two static in one .so-file with the same name
$GXX -c -fPIC 05-extern/func01.cpp -o 05-extern/func01.o
$GXX -c -fPIC 05-extern/func02.cpp -o 05-extern/func02.o
$GXX -c -fPIC 05-extern/func03.cpp -o 05-extern/func03.o
$GXX -shared 05-extern/func01.o 05-extern/func02.o 05-extern/func03.o -o 05-extern/liblib05.so

$GXX -c 05-extern/main.cpp -o 05-extern/main.o
$GXX 05-extern/main.cpp -o 05-extern/app05 -L$PWD/05-extern -llib05 -Wl,-rpath,$PWD/05-extern

$NM -D 05-extern/liblib05.so > 05-extern/liblib05.so.symbols.txt

# 6: missing extern
$GXX -c -fPIC 06-missing_extern/func01.cpp -o 06-missing_extern/func01.o
$GXX -c -fPIC 06-missing_extern/func02.cpp -o 06-missing_extern/func02.o
$GXX -c -fPIC 06-missing_extern/func03.cpp -o 06-missing_extern/func03.o
$GXX -shared 06-missing_extern/func01.o 06-missing_extern/func02.o 06-missing_extern/func03.o -o 06-missing_extern/liblib06.so

$GXX -c 06-missing_extern/main.cpp -o 06-missing_extern/main.o

# This one give this error:
#    .../06-missing_extern/liblib06.so: undefined reference to `g_iA'
#$GXX 06-missing_extern/main.cpp -o 06-missing_extern/app06 -L$PWD/06-missing_extern -llib06 -Wl,-rpath,$PWD/06-missing_extern

$NM -D 06-missing_extern/liblib06.so > 06-missing_extern/liblib06.so.symbols.txt

