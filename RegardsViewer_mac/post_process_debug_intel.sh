SetFile -t APPL ./Release/RegardsViewer
mkdir -p ./Debug/RegardsViewer.app/Contents
mkdir -p ./Debug/RegardsViewer.app/Contents/Frameworks
mkdir -p ./Debug/RegardsViewer.app/Contents/MacOS
mkdir -p ./Debug/RegardsViewer.app/Contents/Resources
sed -e "s/IDENTIFIER/`echo . | sed -e 's,\.\./,,g' | sed -e 's,/,.,g'`/" \
	-e "s/EXECUTABLE/RegardsViewer/" \
	-e "s/VERSION/2.63.6/" \
	../Regards/mac/Info.plist.in >./Debug/RegardsViewer.app/Contents/Info.plist
/bin/echo "APPL????" >./Debug/RegardsViewer.app/Contents/PkgInfo
ln -f ./Release/RegardsViewer ./Debug/RegardsViewer.app/Contents/MacOS/RegardsViewer
cp -f ../Regards/mac/wxmac.icns ./Debug/RegardsViewer.app/Contents/Resources/wxmac.icns
cp -R ../Regards/Resources ./Debug/RegardsViewer.app/Contents/
cp ../Resource/theme/RegardsViewer.viewer.theme ./Debug/RegardsViewer.app/Contents/MacOs/RegardsViewer.viewer.theme
cp ../libextern/exiv2-0.27.3/build/lib/libexiv2.27.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
install_name_tool -change @rpath/libexiv2.27.dylib @executable_path/../Frameworks/libexiv2.27.dylib ./Debug/RegardsViewer.app/Contents/MacOS/RegardsViewer
cp /usr/local/opt/poppler/lib/libpoppler.105.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
install_name_tool -change /usr/local/opt/poppler/lib/libpoppler.105.dylib @executable_path/../Frameworks/libpoppler.105.dylib ./Debug/RegardsViewer.app/Contents/MacOS/RegardsViewer
cp /usr/local/opt/poppler/lib/libpoppler-cpp.0.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
install_name_tool -change /usr/local/opt/poppler/lib/libpoppler-cpp.0.dylib @executable_path/../Frameworks/libpoppler-cpp.0.dylib ./Debug/RegardsViewer.app/Contents/MacOS/RegardsViewer
cp $HOME/ffmpeg_build/lib/libopencv_calib3d.4.3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
install_name_tool -change @rpath/libopencv_calib3d.4.3.dylib @executable_path/../Frameworks/libopencv_calib3d.4.3.dylib ./Debug/RegardsViewer.app/Contents/MacOS/RegardsViewer
cp $HOME/ffmpeg_build/lib/libopencv_core.4.3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
install_name_tool -change @rpath/libopencv_core.4.3.dylib @executable_path/../Frameworks/libopencv_core.4.3.dylib ./Debug/RegardsViewer.app/Contents/MacOS/RegardsViewer
cp $HOME/ffmpeg_build/lib/libopencv_dnn.4.3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
install_name_tool -change @rpath/libopencv_dnn.4.3.dylib @executable_path/../Frameworks/libopencv_dnn.4.3.dylib ./Debug/RegardsViewer.app/Contents/MacOS/RegardsViewer
cp $HOME/ffmpeg_build/lib/libopencv_features2d.4.3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
install_name_tool -change @rpath/libopencv_features2d.4.3.dylib @executable_path/../Frameworks/libopencv_features2d.4.3.dylib ./Debug/RegardsViewer.app/Contents/MacOS/RegardsViewer
cp $HOME/ffmpeg_build/lib/libopencv_imgproc.4.3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
install_name_tool -change @rpath/libopencv_imgproc.4.3.dylib @executable_path/../Frameworks/libopencv_imgproc.4.3.dylib ./Debug/RegardsViewer.app/Contents/MacOS/RegardsViewer
cp $HOME/ffmpeg_build/lib/libopencv_imgcodecs.4.3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
install_name_tool -change @rpath/libopencv_imgcodecs.4.3.dylib @executable_path/../Frameworks/libopencv_imgcodecs.4.3.dylib ./Debug/RegardsViewer.app/Contents/MacOS/RegardsViewer
cp $HOME/ffmpeg_build/lib/libopencv_highgui.4.3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
install_name_tool -change @rpath/libopencv_highgui.4.3.dylib @executable_path/../Frameworks/libopencv_highgui.4.3.dylib ./Debug/RegardsViewer.app/Contents/MacOS/RegardsViewer
cp $HOME/ffmpeg_build/lib/libopencv_face.4.3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
install_name_tool -change @rpath/libopencv_face.4.3.dylib @executable_path/../Frameworks/libopencv_face.4.3.dylib ./Debug/RegardsViewer.app/Contents/MacOS/RegardsViewer
cp $HOME/ffmpeg_build/lib/libopencv_objdetect.4.3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
install_name_tool -change @rpath/libopencv_objdetect.4.3.dylib @executable_path/../Frameworks/libopencv_objdetect.4.3.dylib ./Debug/RegardsViewer.app/Contents/MacOS/RegardsViewer
cp /usr/local/opt/libjpeg-turbo/lib/libturbojpeg.0.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
install_name_tool -change /usr/local/opt/jpeg-turbo/lib/libturbojpeg.0.dylib @executable_path/../Frameworks/libturbojpeg.0.dylib ./Debug/RegardsViewer.app/Contents/MacOS/RegardsViewer
cp /usr/local/lib/libGLEW.2.2.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp /usr/local/lib/libcairo.2.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp /usr/local/lib/libjasper.4.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp $HOME/ffmpeg_build/lib/libavformat.58.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp $HOME/ffmpeg_build/lib/libavcodec.58.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp $HOME/ffmpeg_build/lib/libavdevice.58.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp $HOME/ffmpeg_build/lib/libavfilter.7.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp $HOME/ffmpeg_build/lib/libavutil.56.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp $HOME/ffmpeg_build/lib/libpostproc.55.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp $HOME/ffmpeg_build/lib/libswresample.3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp $HOME/ffmpeg_build/lib/libswscale.5.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp /usr/local/lib/libglib-2.0.0.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp ../Otherlib/libbpg/libRegardsBpg_mac/Release/libRegardsBpg.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp /usr/local/lib/libjpeg.9.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp /usr/local/lib/liblzma.5.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp /usr/local/lib/libSDL2-2.0.0.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp /usr/local/lib/libde265.0.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp /usr/local/lib/libexif.12.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp /usr/local/lib/libfontconfig.1.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp /usr/local/lib/libqpdf.28.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp /usr/local/lib/libtesseract.4.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp /usr/local/lib/liblept.5.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp /usr/local/lib/libtbb.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp /usr/local/lib/libIlmImf-2_5.25.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp /usr/local/lib/libIexMath-2_5.25.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp /usr/local/lib/libHalf-2_5.25.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp /usr/local/lib/libx265.192.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp /usr/local/lib/libtiff.5.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp /usr/local/lib/libpng16.16.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp /usr/local/lib/libraw_r.20.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp /usr/local/lib/libopenjp2.7.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp /usr/local/lib/libIex-2_5.25.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp /usr/local/lib/libpixman-1.0.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp /usr/local/lib/libfreetype.6.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp /usr/local/lib/libdav1d.4.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp /usr/local/lib/libfdk-aac.2.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp /usr/local/lib/libvorbis.0.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp /usr/local/lib/libvorbisenc.2.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp /usr/local/lib/libx264.161.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp /usr/local/lib/libpcre.1.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp /usr/local/lib/libintl.8.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp /usr/local/lib/liblcms2.2.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp /usr/local/lib/libgif.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp /usr/local/lib/libwebpmux.3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp /usr/local/lib/libwebp.7.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp /usr/local/lib/libopenjp2.7.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp /usr/local/lib/libImath-2_5.25.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp /usr/local/lib/libIlmThread-2_5.25.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp /usr/local/lib/libogg.0.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp /usr/local/lib/libomp.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp /usr/local/opt/nss/lib/libnss3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp /usr/local/opt/nss/lib/libnssutil3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp /usr/local/opt/nss/lib/libsmime3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp /usr/local/opt/nss/lib/libssl3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp /usr/local/opt/nspr/lib/libplds4.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp /usr/local/opt/nspr/lib/libplc4.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp /usr/local/opt/nspr/lib/libnspr4.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp /usr/local/opt/xz/lib/liblzma.5.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp /usr/local/opt/lame/lib/libmp3lame.0.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp /usr/local/opt/opus/lib/libopus.0.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp /usr/local/opt/theora/lib/libtheoraenc.1.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp /usr/local/opt/theora/lib/libtheoradec.1.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp /usr/local/opt/libogg/lib/libogg.0.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp $HOME/ffmpeg_build/lib/libopencv_flann.4.3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp $HOME/ffmpeg_build/lib/libopencv_videoio.4.3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp $HOME/ffmpeg_build/lib/libopencv_photo.4.3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
cp /usr/local/lib/liblept.5.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/
chmod +w ./Debug/RegardsViewer.app/Contents/Frameworks/*.*
install_name_tool -change /usr/local/opt/glew/lib/libGLEW.2.2.dylib @executable_path/../Frameworks/libGLEW.2.2.dylib ./Debug/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change /usr/local/opt/cairo/lib/libcairo.2.dylib @executable_path/../Frameworks/libcairo.2.dylib ./Debug/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change /usr/local/opt/jasper/lib/libjasper.4.dylib @executable_path/../Frameworks/libjasper.4.dylib ./Debug/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change $HOME/ffmpeg_build/lib/libavformat.58.dylib @executable_path/../Frameworks/libavformat.58.dylib ./Debug/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change $HOME/ffmpeg_build/lib/libavcodec.58.dylib @executable_path/../Frameworks/libavcodec.58.dylib ./Debug/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change $HOME/ffmpeg_build/lib/libavdevice.58.dylib @executable_path/../Frameworks/libavdevice.58.dylib ./Debug/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change $HOME/ffmpeg_build/lib/libavfilter.7.dylib @executable_path/../Frameworks/libavfilter.7.dylib ./Debug/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change $HOME/ffmpeg_build/lib/libavutil.56.dylib @executable_path/../Frameworks/libavutil.56.dylib ./Debug/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change $HOME/ffmpeg_build/lib/libpostproc.55.dylib @executable_path/../Frameworks/libpostproc.55.dylib ./Debug/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change $HOME/ffmpeg_build/lib/libswresample.3.dylib @executable_path/../Frameworks/libswresample.3.dylib ./Debug/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change /usr/local/opt/libomp/lib/libomp.dylib @executable_path/../Frameworks/libomp.dylib ./Debug/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change $HOME/ffmpeg_build/lib/libswscale.5.dylib @executable_path/../Frameworks/libswscale.5.dylib ./Debug/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change /usr/local/opt/glib/lib/libglib-2.0.0.dylib @executable_path/../Frameworks/libglib-2.0.0.dylib ./Debug/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change /usr/local/opt/jpeg/lib/libjpeg.9.dylib @executable_path/../Frameworks/libjpeg.9.dylib ./Debug/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change /usr/local/opt/xz/lib/liblzma.5.dylib @executable_path/../Frameworks/liblzma.5.dylib ./Debug/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change /usr/local/opt/sdl2/lib/libSDL2-2.0.0.dylib @executable_path/../Frameworks/libSDL2-2.0.0.dylib ./Debug/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change /usr/local/opt/libde265/lib/libde265.0.dylib @executable_path/../Frameworks/libde265.0.dylib ./Debug/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change /usr/local/opt/libexif/lib/libexif.12.dylib @executable_path/../Frameworks/libexif.12.dylib ./Debug/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change /usr/local/opt/fontconfig/lib/libfontconfig.1.dylib @executable_path/../Frameworks/libfontconfig.1.dylib ./Debug/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change /usr/local/opt/qpdf/lib/libqpdf.28.dylib @executable_path/../Frameworks/libqpdf.28.dylib ./Debug/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change /usr/local/opt/tesseract/lib/libtesseract.4.dylib @executable_path/../Frameworks/libtesseract.4.dylib ./Debug/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change /usr/local/opt/leptonica/lib/liblept.5.dylib @executable_path/../Frameworks/liblept.5.dylib ./Debug/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change /usr/local/opt/tbb/lib/libtbb.dylib @executable_path/../Frameworks/libtbb.dylib ./Debug/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change /usr/local/opt/openexr/lib/libIlmImf-2_5.25.dylib @executable_path/../Frameworks/libIlmImf-2_5.25.dylib ./Debug/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change /usr/local/opt/ilmbase/lib/libIexMath-2_5.25.dylib @executable_path/../Frameworks/libIexMath-2_5.25.dylib ./Debug/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change /usr/local/opt/ilmbase/lib/libHalf-2_5.25.dylib @executable_path/../Frameworks/libHalf-2_5.25.dylib ./Debug/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change /usr/local/opt/x265/lib/libx265.192.dylib @executable_path/../Frameworks/libx265.192.dylib ./Debug/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change /usr/local/opt/libtiff/lib/libtiff.5.dylib @executable_path/../Frameworks/libtiff.5.dylib ./Debug/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change /usr/local/opt/libpng/lib/libpng16.16.dylib @executable_path/../Frameworks/libpng16.16.dylib ./Debug/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change /usr/local/opt/libraw/lib/libraw_r.20.dylib @executable_path/../Frameworks/libraw_r.20.dylib ./Debug/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change /usr/local/opt/openjpeg/lib/libopenjp2.7.dylib @executable_path/../Frameworks/libopenjp2.7.dylib ./Debug/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change /usr/local/opt/ilmbase/lib/libIex-2_5.25.dylib @executable_path/../Frameworks/libIex-2_5.25.dylib ./Debug/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change @rpath/libopencv_features2d.4.3.dylib @executable_path/../Frameworks/libopencv_features2d.4.3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libopencv_calib3d.4.3.dylib
install_name_tool -change @rpath/libopencv_flann.4.3.dylib @executable_path/../Frameworks/libopencv_flann.4.3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libopencv_calib3d.4.3.dylib
install_name_tool -change @rpath/libopencv_imgproc.4.3.dylib @executable_path/../Frameworks/libopencv_imgproc.4.3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libopencv_calib3d.4.3.dylib
install_name_tool -change @rpath/libopencv_core.4.3.dylib @executable_path/../Frameworks/libopencv_core.4.3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libopencv_calib3d.4.3.dylib
#File : $HOME/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libpostproc.55.dylib
install_name_tool -change $HOME/ffmpeg_build/lib/libavutil.56.dylib @executable_path/../Frameworks/libavutil.56.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libpostproc.55.dylib
#File : $HOME/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libexif.12.dylib
install_name_tool -change /usr/local/opt/gettext/lib/libintl.8.dylib @executable_path/../Frameworks/libintl.8.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libexif.12.dylib
#File : $HOME/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libswscale.5.dylib
install_name_tool -change $HOME/ffmpeg_build/lib/libavutil.56.dylib @executable_path/../Frameworks/libavutil.56.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libswscale.5.dylib
#File : $HOME/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libcairo.2.dylib
install_name_tool -change /usr/local/opt/pixman/lib/libpixman-1.0.dylib @executable_path/../Frameworks/libpixman-1.0.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libcairo.2.dylib
install_name_tool -change /usr/local/opt/fontconfig/lib/libfontconfig.1.dylib @executable_path/../Frameworks/libfontconfig.1.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libcairo.2.dylib
install_name_tool -change /usr/local/opt/freetype/lib/libfreetype.6.dylib @executable_path/../Frameworks/libfreetype.6.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libcairo.2.dylib
install_name_tool -change /usr/local/opt/libpng/lib/libpng16.16.dylib @executable_path/../Frameworks/libpng16.16.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libcairo.2.dylib
#File : $HOME/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libjasper.4.dylib
install_name_tool -change /usr/local/opt/jpeg/lib/libjpeg.9.dylib @executable_path/../Frameworks/libjpeg.9.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libjasper.4.dylib
#File : $HOME/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libswresample.3.dylib
install_name_tool -change $HOME/ffmpeg_build/lib/libavutil.56.dylib @executable_path/../Frameworks/libavutil.56.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libswresample.3.dylib
#File : $HOME/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libIexMath-2_5.25.dylib
install_name_tool -change @rpath/libIex-2_5.25.dylib @executable_path/../Frameworks/libIex-2_5.25.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libIexMath-2_5.25.dylib
#File : $HOME/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/liblept.5.dylib
install_name_tool -change /usr/local/opt/libpng/lib/libpng16.16.dylib @executable_path/../Frameworks/libpng16.16.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/liblept.5.dylib
install_name_tool -change /usr/local/opt/jpeg/lib/libjpeg.9.dylib @executable_path/../Frameworks/libjpeg.9.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/liblept.5.dylib
install_name_tool -change /usr/local/opt/giflib/lib/libgif.dylib @executable_path/../Frameworks/libgif.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/liblept.5.dylib
install_name_tool -change /usr/local/opt/libtiff/lib/libtiff.5.dylib @executable_path/../Frameworks/libtiff.5.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/liblept.5.dylib
install_name_tool -change /usr/local/opt/webp/lib/libwebpmux.3.dylib @executable_path/../Frameworks/libwebpmux.3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/liblept.5.dylib
install_name_tool -change /usr/local/opt/webp/lib/libwebp.7.dylib @executable_path/../Frameworks/libwebp.7.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/liblept.5.dylib
install_name_tool -change /usr/local/opt/openjpeg/lib/libopenjp2.7.dylib @executable_path/../Frameworks/libopenjp2.7.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/liblept.5.dylib
#File : $HOME/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libopencv_features2d.4.3.dylib
install_name_tool -change @rpath/libopencv_flann.4.3.dylib @executable_path/../Frameworks/libopencv_flann.4.3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libopencv_features2d.4.3.dylib
install_name_tool -change @rpath/libopencv_imgproc.4.3.dylib @executable_path/../Frameworks/libopencv_imgproc.4.3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libopencv_features2d.4.3.dylib
install_name_tool -change @rpath/libopencv_core.4.3.dylib @executable_path/../Frameworks/libopencv_core.4.3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libopencv_features2d.4.3.dylib
#File : $HOME/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libavfilter.7.dylib
install_name_tool -change $HOME/ffmpeg_build/lib/libswscale.5.dylib @executable_path/../Frameworks/libswscale.5.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavfilter.7.dylib
install_name_tool -change $HOME/ffmpeg_build/lib/libpostproc.55.dylib @executable_path/../Frameworks/libpostproc.55.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavfilter.7.dylib
install_name_tool -change $HOME/ffmpeg_build/lib/libavformat.58.dylib @executable_path/../Frameworks/libavformat.58.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavfilter.7.dylib
install_name_tool -change $HOME/ffmpeg_build/lib/libavcodec.58.dylib @executable_path/../Frameworks/libavcodec.58.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavfilter.7.dylib
install_name_tool -change $HOME/ffmpeg_build/lib/libswresample.3.dylib @executable_path/../Frameworks/libswresample.3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavfilter.7.dylib
install_name_tool -change $HOME/ffmpeg_build/lib/libavutil.56.dylib @executable_path/../Frameworks/libavutil.56.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavfilter.7.dylib
install_name_tool -change /usr/local/opt/dav1d/lib/libdav1d.4.dylib @executable_path/../Frameworks/libdav1d.4.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavfilter.7.dylib
install_name_tool -change /usr/local/opt/fdk-aac/lib/libfdk-aac.2.dylib @executable_path/../Frameworks/libfdk-aac.2.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavfilter.7.dylib
install_name_tool -change /usr/local/opt/libvorbis/lib/libvorbis.0.dylib @executable_path/../Frameworks/libvorbis.0.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavfilter.7.dylib
install_name_tool -change /usr/local/opt/libvorbis/lib/libvorbisenc.2.dylib @executable_path/../Frameworks/libvorbisenc.2.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavfilter.7.dylib
install_name_tool -change /usr/local/opt/x264/lib/libx264.161.dylib @executable_path/../Frameworks/libx264.161.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavfilter.7.dylib
install_name_tool -change /usr/local/opt/x265/lib/libx265.192.dylib @executable_path/../Frameworks/libx265.192.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavfilter.7.dylib
install_name_tool -change /usr/local/opt/xz/lib/liblzma.5.dylib @executable_path/../Frameworks/liblzma.5.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavfilter.7.dylib
install_name_tool -change /usr/local/opt/lame/lib/libmp3lame.0.dylib @executable_path/../Frameworks/libmp3lame.0.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavfilter.7.dylib
install_name_tool -change /usr/local/opt/opus/lib/libopus.0.dylib @executable_path/../Frameworks/libopus.0.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavfilter.7.dylib
install_name_tool -change /usr/local/opt/theora/lib/libtheoraenc.1.dylib @executable_path/../Frameworks/libtheoraenc.1.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavfilter.7.dylib
install_name_tool -change /usr/local/opt/theora/lib/libtheoradec.1.dylib @executable_path/../Frameworks/libtheoradec.1.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavfilter.7.dylib
install_name_tool -change /usr/local/opt/libogg/lib/libogg.0.dylib @executable_path/../Frameworks/libogg.0.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavfilter.7.dylib
#File : $HOME/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libglib-2.0.0.dylib
install_name_tool -change /usr/local/opt/pcre/lib/libpcre.1.dylib @executable_path/../Frameworks/libpcre.1.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libglib-2.0.0.dylib
install_name_tool -change /usr/local/opt/gettext/lib/libintl.8.dylib @executable_path/../Frameworks/libintl.8.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libglib-2.0.0.dylib
#File : $HOME/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libavformat.58.dylib
install_name_tool -change $HOME/ffmpeg_build/lib/libavcodec.58.dylib @executable_path/../Frameworks/libavcodec.58.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavformat.58.dylib
install_name_tool -change $HOME/ffmpeg_build/lib/libswresample.3.dylib @executable_path/../Frameworks/libswresample.3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavformat.58.dylib
install_name_tool -change $HOME/ffmpeg_build/lib/libavutil.56.dylib @executable_path/../Frameworks/libavutil.56.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavformat.58.dylib
install_name_tool -change /usr/local/opt/dav1d/lib/libdav1d.4.dylib @executable_path/../Frameworks/libdav1d.4.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavformat.58.dylib
install_name_tool -change /usr/local/opt/fdk-aac/lib/libfdk-aac.2.dylib @executable_path/../Frameworks/libfdk-aac.2.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavformat.58.dylib
install_name_tool -change /usr/local/opt/libvorbis/lib/libvorbis.0.dylib @executable_path/../Frameworks/libvorbis.0.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavformat.58.dylib
install_name_tool -change /usr/local/opt/libvorbis/lib/libvorbisenc.2.dylib @executable_path/../Frameworks/libvorbisenc.2.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavformat.58.dylib
install_name_tool -change /usr/local/opt/x264/lib/libx264.161.dylib @executable_path/../Frameworks/libx264.161.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavformat.58.dylib
install_name_tool -change /usr/local/opt/x265/lib/libx265.192.dylib @executable_path/../Frameworks/libx265.192.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavformat.58.dylib
install_name_tool -change /usr/local/opt/xz/lib/liblzma.5.dylib @executable_path/../Frameworks/liblzma.5.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavformat.58.dylib
install_name_tool -change /usr/local/opt/lame/lib/libmp3lame.0.dylib @executable_path/../Frameworks/libmp3lame.0.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavformat.58.dylib
install_name_tool -change /usr/local/opt/opus/lib/libopus.0.dylib @executable_path/../Frameworks/libopus.0.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavformat.58.dylib
install_name_tool -change /usr/local/opt/theora/lib/libtheoraenc.1.dylib @executable_path/../Frameworks/libtheoraenc.1.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavformat.58.dylib
install_name_tool -change /usr/local/opt/theora/lib/libtheoradec.1.dylib @executable_path/../Frameworks/libtheoradec.1.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavformat.58.dylib
install_name_tool -change /usr/local/opt/libogg/lib/libogg.0.dylib @executable_path/../Frameworks/libogg.0.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavformat.58.dylib
#File : $HOME/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libopencv_face.4.3.dylib
install_name_tool -change @rpath/libopencv_photo.4.3.dylib @executable_path/../Frameworks/libopencv_photo.4.3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libopencv_face.4.3.dylib
install_name_tool -change @rpath/libopencv_objdetect.4.3.dylib @executable_path/../Frameworks/libopencv_objdetect.4.3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libopencv_face.4.3.dylib
install_name_tool -change @rpath/libopencv_calib3d.4.3.dylib @executable_path/../Frameworks/libopencv_calib3d.4.3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libopencv_face.4.3.dylib
install_name_tool -change @rpath/libopencv_features2d.4.3.dylib @executable_path/../Frameworks/libopencv_features2d.4.3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libopencv_face.4.3.dylib
install_name_tool -change @rpath/libopencv_flann.4.3.dylib @executable_path/../Frameworks/libopencv_flann.4.3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libopencv_face.4.3.dylib
install_name_tool -change @rpath/libopencv_imgproc.4.3.dylib @executable_path/../Frameworks/libopencv_imgproc.4.3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libopencv_face.4.3.dylib
install_name_tool -change @rpath/libopencv_core.4.3.dylib @executable_path/../Frameworks/libopencv_core.4.3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libopencv_face.4.3.dylib
#File : $HOME/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libpoppler.105.dylib
install_name_tool -change /usr/local/opt/freetype/lib/libfreetype.6.dylib @executable_path/../Frameworks/libfreetype.6.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libpoppler.105.dylib
install_name_tool -change /usr/local/opt/fontconfig/lib/libfontconfig.1.dylib @executable_path/../Frameworks/libfontconfig.1.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libpoppler.105.dylib
install_name_tool -change /usr/local/opt/jpeg/lib/libjpeg.9.dylib @executable_path/../Frameworks/libjpeg.9.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libpoppler.105.dylib
install_name_tool -change /usr/local/opt/openjpeg/lib/libopenjp2.7.dylib @executable_path/../Frameworks/libopenjp2.7.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libpoppler.105.dylib
install_name_tool -change /usr/local/opt/little-cms2/lib/liblcms2.2.dylib @executable_path/../Frameworks/liblcms2.2.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libpoppler.105.dylib
install_name_tool -change /usr/local/opt/libpng/lib/libpng16.16.dylib @executable_path/../Frameworks/libpng16.16.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libpoppler.105.dylib
install_name_tool -change /usr/local/opt/libtiff/lib/libtiff.5.dylib @executable_path/../Frameworks/libtiff.5.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libpoppler.105.dylib
install_name_tool -change /usr/local/opt/nss/lib/libnss3.dylib @executable_path/../Frameworks/libnss3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libpoppler.105.dylib
install_name_tool -change /usr/local/opt/nss/lib/libnssutil3.dylib @executable_path/../Frameworks/libnssutil3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libpoppler.105.dylib
install_name_tool -change /usr/local/opt/nss/lib/libsmime3.dylib @executable_path/../Frameworks/libsmime3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libpoppler.105.dylib
install_name_tool -change /usr/local/opt/nss/lib/libssl3.dylib @executable_path/../Frameworks/libssl3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libpoppler.105.dylib
install_name_tool -change /usr/local/opt/nspr/lib/libplds4.dylib @executable_path/../Frameworks/libplds4.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libpoppler.105.dylib
install_name_tool -change /usr/local/opt/nspr/lib/libplc4.dylib @executable_path/../Frameworks/libplc4.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libpoppler.105.dylib
install_name_tool -change /usr/local/opt/nspr/lib/libnspr4.dylib @executable_path/../Frameworks/libnspr4.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libpoppler.105.dylib
#File : $HOME/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libopencv_imgproc.4.3.dylib
install_name_tool -change @rpath/libopencv_core.4.3.dylib @executable_path/../Frameworks/libopencv_core.4.3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libopencv_imgproc.4.3.dylib
#File : $HOME/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libopencv_dnn.4.3.dylib
install_name_tool -change @rpath/libopencv_imgproc.4.3.dylib @executable_path/../Frameworks/libopencv_imgproc.4.3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libopencv_dnn.4.3.dylib
install_name_tool -change @rpath/libopencv_core.4.3.dylib @executable_path/../Frameworks/libopencv_core.4.3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libopencv_dnn.4.3.dylib
#File : $HOME/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libavdevice.58.dylib
install_name_tool -change $HOME/ffmpeg_build/lib/libavfilter.7.dylib @executable_path/../Frameworks/libavfilter.7.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavdevice.58.dylib
install_name_tool -change $HOME/ffmpeg_build/lib/libswscale.5.dylib @executable_path/../Frameworks/libswscale.5.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavdevice.58.dylib
install_name_tool -change $HOME/ffmpeg_build/lib/libpostproc.55.dylib @executable_path/../Frameworks/libpostproc.55.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavdevice.58.dylib
install_name_tool -change $HOME/ffmpeg_build/lib/libavformat.58.dylib @executable_path/../Frameworks/libavformat.58.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavdevice.58.dylib
install_name_tool -change $HOME/ffmpeg_build/lib/libavcodec.58.dylib @executable_path/../Frameworks/libavcodec.58.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavdevice.58.dylib
install_name_tool -change $HOME/ffmpeg_build/lib/libswresample.3.dylib @executable_path/../Frameworks/libswresample.3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavdevice.58.dylib
install_name_tool -change $HOME/ffmpeg_build/lib/libavutil.56.dylib @executable_path/../Frameworks/libavutil.56.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavdevice.58.dylib
install_name_tool -change /usr/local/opt/sdl2/lib/libSDL2-2.0.0.dylib @executable_path/../Frameworks/libSDL2-2.0.0.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavdevice.58.dylib
install_name_tool -change /usr/local/opt/dav1d/lib/libdav1d.4.dylib @executable_path/../Frameworks/libdav1d.4.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavdevice.58.dylib
install_name_tool -change /usr/local/opt/fdk-aac/lib/libfdk-aac.2.dylib @executable_path/../Frameworks/libfdk-aac.2.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavdevice.58.dylib
install_name_tool -change /usr/local/opt/libvorbis/lib/libvorbis.0.dylib @executable_path/../Frameworks/libvorbis.0.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavdevice.58.dylib
install_name_tool -change /usr/local/opt/libvorbis/lib/libvorbisenc.2.dylib @executable_path/../Frameworks/libvorbisenc.2.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavdevice.58.dylib
install_name_tool -change /usr/local/opt/x264/lib/libx264.161.dylib @executable_path/../Frameworks/libx264.161.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavdevice.58.dylib
install_name_tool -change /usr/local/opt/x265/lib/libx265.192.dylib @executable_path/../Frameworks/libx265.192.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavdevice.58.dylib
install_name_tool -change /usr/local/opt/xz/lib/liblzma.5.dylib @executable_path/../Frameworks/liblzma.5.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavdevice.58.dylib
install_name_tool -change /usr/local/opt/lame/lib/libmp3lame.0.dylib @executable_path/../Frameworks/libmp3lame.0.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavdevice.58.dylib
install_name_tool -change /usr/local/opt/opus/lib/libopus.0.dylib @executable_path/../Frameworks/libopus.0.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavdevice.58.dylib
install_name_tool -change /usr/local/opt/theora/lib/libtheoraenc.1.dylib @executable_path/../Frameworks/libtheoraenc.1.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavdevice.58.dylib
install_name_tool -change /usr/local/opt/theora/lib/libtheoradec.1.dylib @executable_path/../Frameworks/libtheoradec.1.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavdevice.58.dylib
install_name_tool -change /usr/local/opt/libogg/lib/libogg.0.dylib @executable_path/../Frameworks/libogg.0.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavdevice.58.dylib
#File : $HOME/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libfontconfig.1.dylib
install_name_tool -change /usr/local/opt/freetype/lib/libfreetype.6.dylib @executable_path/../Frameworks/libfreetype.6.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libfontconfig.1.dylib
#File : $HOME/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libpoppler-cpp.0.dylib
install_name_tool -change /usr/local/Cellar/poppler/20.12.1/lib/libpoppler.105.dylib @executable_path/../Frameworks/libpoppler.105.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libpoppler-cpp.0.dylib
#File : $HOME/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libqpdf.28.dylib
install_name_tool -change /usr/local/opt/jpeg/lib/libjpeg.9.dylib @executable_path/../Frameworks/libjpeg.9.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libqpdf.28.dylib
#File : $HOME/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libtiff.5.dylib
install_name_tool -change /usr/local/opt/jpeg/lib/libjpeg.9.dylib @executable_path/../Frameworks/libjpeg.9.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libtiff.5.dylib
#File : $HOME/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libopencv_highgui.4.3.dylib
install_name_tool -change @rpath/libopencv_videoio.4.3.dylib @executable_path/../Frameworks/libopencv_videoio.4.3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libopencv_highgui.4.3.dylib
install_name_tool -change @rpath/libopencv_imgcodecs.4.3.dylib @executable_path/../Frameworks/libopencv_imgcodecs.4.3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libopencv_highgui.4.3.dylib
install_name_tool -change @rpath/libopencv_imgproc.4.3.dylib @executable_path/../Frameworks/libopencv_imgproc.4.3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libopencv_highgui.4.3.dylib
install_name_tool -change @rpath/libopencv_core.4.3.dylib @executable_path/../Frameworks/libopencv_core.4.3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libopencv_highgui.4.3.dylib
#File : $HOME/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libraw_r.20.dylib
install_name_tool -change /usr/local/opt/libomp/lib/libomp.dylib @executable_path/../Frameworks/libomp.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libraw_r.20.dylib
install_name_tool -change /usr/local/opt/jpeg/lib/libjpeg.9.dylib @executable_path/../Frameworks/libjpeg.9.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libraw_r.20.dylib
install_name_tool -change /usr/local/opt/jasper/lib/libjasper.4.dylib @executable_path/../Frameworks/libjasper.4.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libraw_r.20.dylib
install_name_tool -change /usr/local/opt/little-cms2/lib/liblcms2.2.dylib @executable_path/../Frameworks/liblcms2.2.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libraw_r.20.dylib
#File : $HOME/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libopencv_objdetect.4.3.dylib
install_name_tool -change @rpath/libopencv_calib3d.4.3.dylib @executable_path/../Frameworks/libopencv_calib3d.4.3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libopencv_objdetect.4.3.dylib
install_name_tool -change @rpath/libopencv_features2d.4.3.dylib @executable_path/../Frameworks/libopencv_features2d.4.3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libopencv_objdetect.4.3.dylib
install_name_tool -change @rpath/libopencv_flann.4.3.dylib @executable_path/../Frameworks/libopencv_flann.4.3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libopencv_objdetect.4.3.dylib
install_name_tool -change @rpath/libopencv_imgproc.4.3.dylib @executable_path/../Frameworks/libopencv_imgproc.4.3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libopencv_objdetect.4.3.dylib
install_name_tool -change @rpath/libopencv_core.4.3.dylib @executable_path/../Frameworks/libopencv_core.4.3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libopencv_objdetect.4.3.dylib
#File : $HOME/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libopencv_imgcodecs.4.3.dylib
install_name_tool -change @rpath/libopencv_imgproc.4.3.dylib @executable_path/../Frameworks/libopencv_imgproc.4.3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libopencv_imgcodecs.4.3.dylib
install_name_tool -change /usr/local/opt/openjpeg/lib/libopenjp2.7.dylib @executable_path/../Frameworks/libopenjp2.7.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libopencv_imgcodecs.4.3.dylib
install_name_tool -change @rpath/libopencv_core.4.3.dylib @executable_path/../Frameworks/libopencv_core.4.3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libopencv_imgcodecs.4.3.dylib
#File : $HOME/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libavcodec.58.dylib
install_name_tool -change $HOME/ffmpeg_build/lib/libswresample.3.dylib @executable_path/../Frameworks/libswresample.3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavcodec.58.dylib
install_name_tool -change $HOME/ffmpeg_build/lib/libavutil.56.dylib @executable_path/../Frameworks/libavutil.56.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavcodec.58.dylib
install_name_tool -change /usr/local/opt/dav1d/lib/libdav1d.4.dylib @executable_path/../Frameworks/libdav1d.4.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavcodec.58.dylib
install_name_tool -change /usr/local/opt/fdk-aac/lib/libfdk-aac.2.dylib @executable_path/../Frameworks/libfdk-aac.2.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavcodec.58.dylib
install_name_tool -change /usr/local/opt/libvorbis/lib/libvorbis.0.dylib @executable_path/../Frameworks/libvorbis.0.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavcodec.58.dylib
install_name_tool -change /usr/local/opt/libvorbis/lib/libvorbisenc.2.dylib @executable_path/../Frameworks/libvorbisenc.2.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavcodec.58.dylib
install_name_tool -change /usr/local/opt/x264/lib/libx264.161.dylib @executable_path/../Frameworks/libx264.161.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavcodec.58.dylib
install_name_tool -change /usr/local/opt/x265/lib/libx265.192.dylib @executable_path/../Frameworks/libx265.192.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavcodec.58.dylib
install_name_tool -change /usr/local/opt/xz/lib/liblzma.5.dylib @executable_path/../Frameworks/liblzma.5.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavcodec.58.dylib
install_name_tool -change /usr/local/opt/lame/lib/libmp3lame.0.dylib @executable_path/../Frameworks/libmp3lame.0.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavcodec.58.dylib
install_name_tool -change /usr/local/opt/opus/lib/libopus.0.dylib @executable_path/../Frameworks/libopus.0.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavcodec.58.dylib
install_name_tool -change /usr/local/opt/theora/lib/libtheoraenc.1.dylib @executable_path/../Frameworks/libtheoraenc.1.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavcodec.58.dylib
install_name_tool -change /usr/local/opt/theora/lib/libtheoradec.1.dylib @executable_path/../Frameworks/libtheoradec.1.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavcodec.58.dylib
install_name_tool -change /usr/local/opt/libogg/lib/libogg.0.dylib @executable_path/../Frameworks/libogg.0.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libavcodec.58.dylib
#File : $HOME/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libtesseract.4.dylib
install_name_tool -change /usr/local/opt/leptonica/lib/liblept.5.dylib @executable_path/../Frameworks/liblept.5.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libtesseract.4.dylib
#File : $HOME/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libIlmImf-2_5.25.dylib
install_name_tool -change /usr/local/opt/ilmbase/lib/libImath-2_5.25.dylib @executable_path/../Frameworks/libImath-2_5.25.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libIlmImf-2_5.25.dylib
install_name_tool -change /usr/local/opt/ilmbase/lib/libIlmThread-2_5.25.dylib @executable_path/../Frameworks/libIlmThread-2_5.25.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libIlmImf-2_5.25.dylib
install_name_tool -change /usr/local/opt/ilmbase/lib/libHalf-2_5.25.dylib @executable_path/../Frameworks/libHalf-2_5.25.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libIlmImf-2_5.25.dylib
install_name_tool -change /usr/local/opt/ilmbase/lib/libIexMath-2_5.25.dylib @executable_path/../Frameworks/libIexMath-2_5.25.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libIlmImf-2_5.25.dylib
install_name_tool -change /usr/local/opt/ilmbase/lib/libIex-2_5.25.dylib @executable_path/../Frameworks/libIex-2_5.25.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libIlmImf-2_5.25.dylib
#File : $HOME/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libvorbisenc.2.dylib
install_name_tool -change /usr/local/Cellar/libvorbis/1.3.7/lib/libvorbis.0.dylib @executable_path/../Frameworks/libvorbis.0.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libvorbisenc.2.dylib
install_name_tool -change /usr/local/opt/libogg/lib/libogg.0.dylib @executable_path/../Frameworks/libogg.0.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libvorbisenc.2.dylib
#File : $HOME/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libfreetype.6.dylib
install_name_tool -change /usr/local/opt/libpng/lib/libpng16.16.dylib @executable_path/../Frameworks/libpng16.16.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libfreetype.6.dylib
#File : $HOME/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libopencv_flann.4.3.dylib
install_name_tool -change @rpath/libopencv_core.4.3.dylib @executable_path/../Frameworks/libopencv_core.4.3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libopencv_flann.4.3.dylib
#File : $HOME/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libwebpmux.3.dylib
install_name_tool -change /usr/local/Cellar/webp/1.1.0/lib/libwebp.7.dylib @executable_path/../Frameworks/libwebp.7.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libwebpmux.3.dylib
#File : $HOME/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libvorbis.0.dylib
install_name_tool -change /usr/local/opt/libogg/lib/libogg.0.dylib @executable_path/../Frameworks/libogg.0.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libvorbis.0.dylib
#File : $HOME/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libopencv_videoio.4.3.dylib
install_name_tool -change @rpath/libopencv_imgcodecs.4.3.dylib @executable_path/../Frameworks/libopencv_imgcodecs.4.3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libopencv_videoio.4.3.dylib
install_name_tool -change @rpath/libopencv_imgproc.4.3.dylib @executable_path/../Frameworks/libopencv_imgproc.4.3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libopencv_videoio.4.3.dylib
install_name_tool -change @rpath/libopencv_core.4.3.dylib @executable_path/../Frameworks/libopencv_core.4.3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libopencv_videoio.4.3.dylib
#File : $HOME/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libIlmThread-2_5.25.dylib
install_name_tool -change @rpath/libIex-2_5.25.dylib @executable_path/../Frameworks/libIex-2_5.25.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libIlmThread-2_5.25.dylib
#File : $HOME/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libImath-2_5.25.dylib
install_name_tool -change @rpath/libHalf-2_5.25.dylib @executable_path/../Frameworks/libHalf-2_5.25.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libImath-2_5.25.dylib
install_name_tool -change @rpath/libIexMath-2_5.25.dylib @executable_path/../Frameworks/libIexMath-2_5.25.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libImath-2_5.25.dylib
install_name_tool -change @rpath/libIex-2_5.25.dylib @executable_path/../Frameworks/libIex-2_5.25.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libImath-2_5.25.dylib
#File : $HOME/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libopencv_photo.4.3.dylib
install_name_tool -change @rpath/libopencv_imgproc.4.3.dylib @executable_path/../Frameworks/libopencv_imgproc.4.3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libopencv_photo.4.3.dylib
install_name_tool -change @rpath/libopencv_core.4.3.dylib @executable_path/../Frameworks/libopencv_core.4.3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libopencv_photo.4.3.dylib
#File : ../../RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libplds4.dylib
install_name_tool -change /usr/local/Cellar/nspr/4.29/lib/libnspr4.dylib @executable_path/../Frameworks/libnspr4.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libplds4.dylib
#File : ../../RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libsmime3.dylib
install_name_tool -change /usr/local/Cellar/nss/3.60/lib/libnss3.dylib @executable_path/../Frameworks/libnss3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libsmime3.dylib
install_name_tool -change /usr/local/Cellar/nss/3.60/lib/libnssutil3.dylib @executable_path/../Frameworks/libnssutil3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libsmime3.dylib
install_name_tool -change /usr/local/opt/nspr/lib/libplc4.dylib @executable_path/../Frameworks/libplc4.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libsmime3.dylib
install_name_tool -change /usr/local/opt/nspr/lib/libplds4.dylib @executable_path/../Frameworks/libplds4.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libsmime3.dylib
install_name_tool -change /usr/local/opt/nspr/lib/libnspr4.dylib @executable_path/../Frameworks/libnspr4.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libsmime3.dylib
#File : ../../RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libplc4.dylib
install_name_tool -change /usr/local/Cellar/nspr/4.29/lib/libnspr4.dylib @executable_path/../Frameworks/libnspr4.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libplc4.dylib
#File : ../../RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libssl3.dylib
install_name_tool -change /usr/local/Cellar/nss/3.60/lib/libnss3.dylib @executable_path/../Frameworks/libnss3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libssl3.dylib
install_name_tool -change /usr/local/Cellar/nss/3.60/lib/libnssutil3.dylib @executable_path/../Frameworks/libnssutil3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libssl3.dylib
install_name_tool -change /usr/local/opt/nspr/lib/libplc4.dylib @executable_path/../Frameworks/libplc4.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libssl3.dylib
install_name_tool -change /usr/local/opt/nspr/lib/libplds4.dylib @executable_path/../Frameworks/libplds4.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libssl3.dylib
install_name_tool -change /usr/local/opt/nspr/lib/libnspr4.dylib @executable_path/../Frameworks/libnspr4.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libssl3.dylib
#File : ../../RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libnssutil3.dylib
install_name_tool -change /usr/local/opt/nspr/lib/libplc4.dylib @executable_path/../Frameworks/libplc4.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libnssutil3.dylib
install_name_tool -change /usr/local/opt/nspr/lib/libplds4.dylib @executable_path/../Frameworks/libplds4.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libnssutil3.dylib
install_name_tool -change /usr/local/opt/nspr/lib/libnspr4.dylib @executable_path/../Frameworks/libnspr4.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libnssutil3.dylib
#File : ../../RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libnss3.dylib
install_name_tool -change /usr/local/Cellar/nss/3.60/lib/libnssutil3.dylib @executable_path/../Frameworks/libnssutil3.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libnss3.dylib
install_name_tool -change /usr/local/opt/nspr/lib/libplc4.dylib @executable_path/../Frameworks/libplc4.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libnss3.dylib
install_name_tool -change /usr/local/opt/nspr/lib/libplds4.dylib @executable_path/../Frameworks/libplds4.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libnss3.dylib
install_name_tool -change /usr/local/opt/nspr/lib/libnspr4.dylib @executable_path/../Frameworks/libnspr4.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libnss3.dylib
#File : ../../RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libRegardsBpg.dylib
install_name_tool -change /usr/local/opt/x265/lib/libx265.192.dylib @executable_path/../Frameworks/libx265.192.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libRegardsBpg.dylib
install_name_tool -change /usr/local/opt/libpng/lib/libpng16.16.dylib @executable_path/../Frameworks/libpng16.16.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libRegardsBpg.dylib
#File : /Users/toto/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libtheoraenc.1.dylib
install_name_tool -change /usr/local/opt/libogg/lib/libogg.0.dylib @executable_path/../Frameworks/libogg.0.dylib ./Debug/RegardsViewer.app/Contents/Frameworks/libtheoraenc.1.dylib