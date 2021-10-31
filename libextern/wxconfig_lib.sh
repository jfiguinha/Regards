if [[ $OSTYPE == 'darwin'* ]]; then
	/usr/local/bin/wx-config --libs
else
	${HOME}/ffmpeg_build/bin/wx-config --libs
fi
