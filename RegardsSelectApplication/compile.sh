#!/bin/bash
NBPROC=$(nproc)
echo $NBPROC

export PROJECTPATH=$PWD
make -j$NBPROC -f compile_select.mk
