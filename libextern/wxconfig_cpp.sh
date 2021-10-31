if [[ $OSTYPE == 'darwin'* ]]; then
	/usr/local/bin/wx-config --cppflags
else
	${HOME}/ffmpeg_build/bin/wx-config --cppflags
fi


