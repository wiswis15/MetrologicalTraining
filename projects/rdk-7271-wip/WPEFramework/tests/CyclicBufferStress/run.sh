#!/bin/bash

stressApp=$PWD/build/tests/CyclicBufferStress/CyclicBufferStress
waitTime=1

currentTime=$($stressApp -time 2> /dev/null)
startTime=$(echo "$currentTime + $waitTime" | bc)

echo "Stress test will start in $waitTime second(s)"

$stressApp -server $startTime &
$stressApp -client $startTime
sleep 2

