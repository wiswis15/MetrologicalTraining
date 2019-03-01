#!/bin/bash

rm -rf build04

mkdir build04

cp /home/sander/Projects/Netflix/5.0/build/*.sh build04
cp -r /home/sander/Projects/Netflix/5.0/build/python build04
cp -r /home/sander/Projects/Netflix/5.0/build/patches build04

rm -rf ocdmtestapp04
cp -r /home/sander/Projects/WPEFramework/ocdmtestapp04/src ocdmtestapp04

rm -rf ocdmtestapp05
cp -r /home/sander/Projects/WPEFramework/ocdmtestapp05/src ocdmtestapp05

rm -rf OCDM-Null2-Ubuntu18
cp -r /home/sander/Projects/WPEFramework/OCDM-Null2/src OCDM-Null2-Ubuntu18

