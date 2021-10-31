if [[ $OSTYPE == 'darwin'* ]]; then
	/opt/homebrew/bin/wx-config --libs
else
	${HOME}/ffmpeg_build/bin/wx-config --libs
fi
