#!/bin/bash -e

for fname in `ls`; do
new=`echo ${fname} | awk -F "." '{print $1}'`_arm.png
#echo ${new}
cp ${fname} ${new}
done
