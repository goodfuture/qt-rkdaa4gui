#!/bin/bash -e

export QTDIR=/usr/local/arm/qt3-embedded
export PATH=${PATH}:/usr/local/arm/3.4.3-rockontrol/bin

xpath=/usr/local/arm/qt3-embedded
qmake=${xpath}/bin/qmake
flag="-spec ${PWD}/mkspecs/arm/"

if [ -n "$1" ]; then
${qmake} -project
rm -f Makefile && ${qmake} ${flag} -o Makefile
fi

make 
#2>&1 #| tee make.log

#sudo cp GUI /home/kangqi/Debug/shucaiyi_stable/GUI.bin
