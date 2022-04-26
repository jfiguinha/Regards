#!/bin/bash
if [[ $OSTYPE == 'darwin'* ]]; then
	/opt/homebrew/bin/wx-config --cppflags
	#${HOME}/developpement/git/Regards/libextern/vcpkg-master/installed/arm64-osx/bin/wx-config --cppflags
	#/Users/jacques/developpement/git/Regards/libextern/vcpkg-master/installed/arm64-osx/tools/wxwidgets/wx-config --cppflags
else
#	${HOME}/ffmpeg_build/bin/wx-config --cppflags
    ${HOME}/developpement/git/Regards/libextern/wxWidgets/wx-config --cppflags
fi


