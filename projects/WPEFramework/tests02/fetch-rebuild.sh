#!/bin/bash

set -e

./gtest-fetch.sh
./wpeframework-fetch.sh

./rebuild.sh

