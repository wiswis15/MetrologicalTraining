#!/bin/bash

mkdir -p backup

OUTPUT_INDEX=$(ls backup | wc -l)
OUTPUT_INDEX=$(($OUTPUT_INDEX + 1))
OUTPUT_INDEX=$(printf '%04d' $OUTPUT_INDEX)

OUTPUT_FILE="backup/working$OUTPUT_INDEX.tgz"

echo $OUTPUT_FILE

ls | grep -v backup | xargs tar czf $OUTPUT_FILE

