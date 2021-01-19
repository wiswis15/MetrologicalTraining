BASE_DIR=$PWD

STAGING_DIR="$PWD/staging"
PREFIX="$STAGING_DIR/usr"
mkdir -p $PREFIX

CMAKE_MODULE_PATH="$PWD/staging/share/cmake/Modules"
mkdir -p $CMAKE_MODULE_PATH

CMAKE="$PREFIX/bin/cmake"

export PKG_CONFIG_PATH="$PREFIX/lib/pkgconfig"

