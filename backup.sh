#!/bin/bash -e

dest=../../Backup/GUI/"GUI_"`date +%Y-%m-%d_%H:%M:%S`.tar
src=./*
#echo ${dest}
tar cvf ${dest} ${src}

