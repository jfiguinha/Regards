#!/bin/bash
if [[ $OSTYPE == 'darwin'* ]]; then
	/usr/local/bin/wx-config --cppflags
	#${HOME}/developpement/git/Regards/libextern/vcpkg-master/installed/arm64-osx/bin/wx-config --cppflags
	#/Users/jacques/developpement/git/Regards/libextern/vcpkg-master/installed/arm64-osx/tools/wxwidgets/wx-config --cppflags
else
#	${HOME}/ffmpeg_build/bin/wx-config --cppflags
    ${HOME}/developpement/git/Regards/libextern/vcpkg-master/installed/x64-linux/tools/wxwidgets/wx-config --cppflags
fi


