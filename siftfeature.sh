#!/bin/bash -e
# Usage: siftfeature.sh [input directory] [output filename]
DIR=$1
OUTPUT=$2
PROG=build/bin/sift3d_qifp

# Write the CSV header
echo "Patient ID,Segmentation UID,sift3d`seq -s ,sift3d 0 767`" > $OUTPUT

# Loop over each DICOM file
for f in `ls $DIR/*\.dcm`
do
        # Check if the file is a DSO
        if dcmdump $f | grep '(0008,0016).*=SegmentationStorage'
        then
                echo "Processing DSO file $f"

                # Get the SOPInstanceUID
                INSTANCEUID=`dcmdump $f | grep '(0008,0018)' | grep -o '\[.*\]' | tr -d '[]'`

                # Get the Patient ID
                PATIENTID=`dcmdump $f | grep '(0010,0020)' | grep -o '\[.*\]' | tr -d '[]'`

                # Run the program
                TMP=mktemp || exit 1
                $PROG $f $TMP

                # Remove the first 3 columns of the output, which give the feature coordinates
                FEATURE=`cut -d, -f4- $TMP`

                # Add a row to the feature table
                echo "$INSTANCEUID,$PATIENTID,$FEATURE" >> $OUTPUT
        fi
done

