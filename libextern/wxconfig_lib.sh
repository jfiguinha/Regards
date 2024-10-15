#!/bin/bash
if [[ $OSTYPE == 'darwin'* ]]; then
	#/opt/homebrew/bin/wx-config --libs
	/Volumes/WD_BLACK/developpement/git/Regards/libextern/vcpkg/installed/arm64-osx/tools/wxwidgets/wx-config --libs
else
#	${HOME}/ffmpeg_build/bin/wx-config --libs
#    ${HOME}/developpement/git/Regards/libextern/vcpkg-master/installed/x64-linux/tools/wxwidgets/wx-config --libs
    ${HOME}/developpement/git/Regards/libextern/wxWidgets-3.2.3/build_new/wx-config --libs
fi
