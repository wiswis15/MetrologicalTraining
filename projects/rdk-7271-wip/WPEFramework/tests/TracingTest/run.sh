#!/bin/bash

rm -f core
ulimit -c unlimited

waitTime=1
binDir="$PWD/build/tests/TracingTest"

currentTime=$(date +%s)
startTime=$(echo "$currentTime + $waitTime" | bc)

killall -9 TracingTest{Server,Client} &> /dev/null
mkdir -p /tmp/tracebuffer01
rm -f /tmp/tracebuffer01/*

echo "Test will start in $waitTime second(s)"

export LD_LIBRARY_PATH=$PWD/staging/usr/lib

$binDir/client/TracingTestClient $startTime 2>&1 | tee /tmp/client.txt 1>&2 &
$binDir/server/TracingTestServer $startTime 2>&1 | tee /tmp/server.txt 1>&2

killall -9 TracingTest{Server,Client} &> /dev/null
 
sleep 2
