if [[ $OSTYPE == 'darwin'* ]]; then
	/usr/local/bin/wx-config --libs
else
#	${HOME}/ffmpeg_build/bin/wx-config --libs
    ${HOME}/developpement/git/Regards/libextern/vcpkg-master/installed/x64-linux/tools/wxwidgets/wx-config --libs
fi
