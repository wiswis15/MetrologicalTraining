#!/bin/bash

set -e

rm -rf repos zips

./cmake-fetch.sh
./repos-fetch.sh

