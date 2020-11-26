#!/bin/bash
NBPROC=$(nproc)
echo $NBPROC

unzip project_linux.zip

sudo cp libextern/tesscallback.h ~/ffmpeg_build/include/tesseract

export PROJECTPATH=$PWD
make -j$NBPROC
