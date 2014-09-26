#~/bin/bash -e

export QTDIR=/usr/share/qt3
qmake=qmake-qt3
flag="-spec ${PWD}/mkspecs/x86/"

if [ -n "$1" ] ; then
make clean && ${qmake} -project
rm -f Makefile && ${qmake} ${flag} -o Makefile
fi

make
