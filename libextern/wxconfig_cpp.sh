#!/bin/bash
if [[ $OSTYPE == 'darwin'* ]]; then
	#/opt/homebrew/bin/wx-config --cppflags
	/Volumes/WD_BLACK/developpement/git/Regards/libextern/vcpkg/installed/arm64-osx/tools/wxwidgets/wx-config --cppflags
else
#	${HOME}/ffmpeg_build/bin/wx-config --cppflags
    ${HOME}/developpement/git/Regards/libextern/vcpkg/installed/x64-linux/tools/wxwidgets/wx-config --cppflags
#   ${HOME}/developpement/git/Regards/libextern/vcpkg-2022.08.15/installed/x64-linux/tools/wxwidgets/wx-config --cppflags
#   ${HOME}/developpement/git/Regards/libextern/wxWidgets-3.2.3/build_new/wx-config --cppflags
fi


