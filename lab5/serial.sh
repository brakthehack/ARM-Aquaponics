#!/bin/bash
#THIS FILE MUST HAVE SERIALT open to work!

DATETIME=$(date +%H,%M,%S,%m,%d,%Y)

# Port setting
stty -F /dev/ttyUSB0 raw speed 9600
echo $DATETIME > /dev/ttyUSB0

# Loop
#while [ 1 ]; do
#    READ=`dd if=/dev/ttyUSB0 count=1`
#    echo $READ
#done
