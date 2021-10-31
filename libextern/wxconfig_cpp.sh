if [[ $OSTYPE == 'darwin'* ]]; then
	/opt/homebrew/bin/wx-config --cppflags
else
	${HOME}/ffmpeg_build/bin/wx-config --cppflags
fi


