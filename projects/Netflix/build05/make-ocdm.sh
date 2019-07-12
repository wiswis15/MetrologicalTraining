cd build

echo 'Making WPEFramework'

cd wpeframework/build
make -j4 install
rc=$?
if [[ $rc != 0 ]]; then
   exit $rc
fi
cd ../..

echo 'Making WPEFramework Plugins'

cd wpeframework-plugins/build
make -j4 install
rc=$?
if [[ $rc != 0 ]]; then
   exit $rc
fi
cd ../..

echo 'Making OCDM-Playready'

cd ocdm-playready/build
make -j4 install
rc=$?
if [[ $rc != 0 ]]; then
   exit $rc
fi
cd ../..

echo 'Making Netflix 5'

cd netflix5/build
make -j4
rc=$?
if [[ $rc != 0 ]]; then
   exit $rc
fi
cd ../..

cd ..
python/updateConfig.py

