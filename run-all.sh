#!/bin/bash
# bash run-all.sh

# Set some variables to make this more useful
DATADIR=data
OUTPUT=avicultural.txt
PROGNAME=./Source

# Delete the output file, because we're appending
rm $OUTPUT

# Process all the file in the data directory
for filename in $DATADIR/bhl-item-*.xml; do
    $PROGNAME $filename >> $OUTPUT
done;

