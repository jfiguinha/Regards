#!/bin/bash
if [[ $OSTYPE == 'darwin'* ]]; then
	/usr/local/bin/wx-config --libs
	#${HOME}/developpement/git/Regards/libextern/vcpkg-master/installed/arm64-osx/bin/wx-config --libs
else
#	${HOME}/ffmpeg_build/bin/wx-config --libs
    ${HOME}/developpement/git/Regards/libextern/vcpkg-2022.08.15/installed/x64-linux/tools/wxwidgets/wx-config --libs
fi
