if [[ $OSTYPE == 'darwin'* ]]; then
	/opt/homebrew/bin/wx-config --cppflags
else
#	${HOME}/ffmpeg_build/bin/wx-config --cppflags
    ${HOME}/developpement/git/Regards/libextern/vcpkg-master/installed/x64-linux/tools/wxwidgets/wx-config --cppflags
fi


