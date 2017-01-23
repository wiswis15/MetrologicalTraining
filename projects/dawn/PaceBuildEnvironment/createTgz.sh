#!/bin/bash

BUILDROOT_STRIP=/home/sander/Projects/buildroot/dawn/buildroot/output/host/usr/bin/mipsel-buildroot-linux-uclibc-strip
PACESDKDIR=../../METROLOGICAL_3.1.11/
USERNAME=sander

if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root" 1>&2
   exit 1
fi

echo 'Copying symbol files'
mkdir symbol-files
cp -v $PACESDKDIR/packages/qtwebkit/qtwebkit-opensource-src-current/lib/libQt5WebKit*.so.5.4.1 symbol-files/
cp -v $PACESDKDIR/packages/qtbrowser/qtbrowser-current/qtbrowser symbol-files/

echo 'Creating stripped versions'
cp -r symbol-files stripped
cd stripped
$BUILDROOT_STRIP -d *
cd ..

chown -R $USERNAME stripped symbol-files

echo 'Creating stripped tar'
cd stripped
tar czf stripped.tgz *
cd ..
mv stripped/stripped.tgz .
chown $USERNAME *.tgz

echo 'DONE creating stripped tar'

echo 'Creating symbol files tar'
tar czf symbol-files.tgz symbol-files/

chown $USERNAME *.tgz

echo 'Copying staging'
cp -r $PACESDKDIR/build/DMC7002KLG_CADA_MW_RELEASE/staging/ .

echo 'Tarring staging'
tar czf staging.tgz staging/

mkdir source-files

echo 'Copying sources'
echo 'Copying qtbrowser...'
cp -r $PACESDKDIR/packages/qtbrowser/ source-files/

echo 'Copying qtbase...'
cp -r $PACESDKDIR/packages/qtbase/ source-files/

echo 'Copying qtwebkit...'
cp -r $PACESDKDIR/packages/qtwebkit/ source-files/

echo 'Removing non-source files'
cd source-files
find -type f | grep -vEi '\.(c|cc|cpp|h|H|hh|inl|mm|mmp|moc|py|s|S|sh|y)$' > /tmp/deleted_files.txt

while IFS='' read -r line || [[ -n "$line" ]]; do
    rm "$line"
done < /tmp/deleted_files.txt

cd ..

echo 'Tarring sources'
tar czf source-files.tgz source-files/

chown $USERNAME *.tgz

