#!/bin/bash -e

for old in `ls`; do
	new=`echo ${old} | tr [:upper:] [:lower:]`
	if [ ${old} = ${new} ]; then
		continue;
	fi
	mv ${old} ${new}
done
