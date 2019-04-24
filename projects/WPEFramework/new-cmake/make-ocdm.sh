cd build

cd wpeframework/build
make -j4 install
rc=$?
if [[ $rc != 0 ]]; then
   exit $rc
fi
cd ../..

cd wpeframework-plugins/build
make -j4 install
rc=$?
if [[ $rc != 0 ]]; then
   exit $rc
fi
cd ../..

cd ocdm-playready/build
make -j4 install
rc=$?
if [[ $rc != 0 ]]; then
   exit $rc
fi
cd ../..

cd netflix5/build
make -j4
rc=$?
if [[ $rc != 0 ]]; then
   exit $rc
fi
cd ../..

cd ..
python/updateConfig.py

