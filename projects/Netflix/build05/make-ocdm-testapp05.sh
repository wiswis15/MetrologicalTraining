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

cd ocdm-null2/build
make -j4 install
rc=$?
if [[ $rc != 0 ]]; then
   exit $rc
fi
cd ../..

cd ocdmtestapp05/build
make -j4 install
rc=$?
if [[ $rc != 0 ]]; then
   exit $rc
fi
cd ../..

cd ..
python/updateConfig.py Null2

