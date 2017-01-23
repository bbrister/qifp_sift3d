#!/bin/sh
# Usage: siftfeature.sh [dso filename] [output filename]
DSO=$1
OUTPUT=$2
PROG=build/bin/sift3d_qifp
# Run the program
$PROG $DSO $OUTPUT
# Remove the first 3 columns of the output, which give the feature coordinates
TMP=mktemp || exit 1
cut -d, -f4- $OUTPUT > $TMP
cat $TMP > $OUTPUT
