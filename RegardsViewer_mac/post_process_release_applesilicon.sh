SetFile -t APPL ./Release/RegardsViewer
mkdir -p ./Release/RegardsViewer.app/Contents
mkdir -p ./Release/RegardsViewer.app/Contents/Frameworks
mkdir -p ./Release/RegardsViewer.app/Contents/MacOS
mkdir -p ./Release/RegardsViewer.app/Contents/Resources
sed -e "s/IDENTIFIER/`echo . | sed -e 's,\.\./,,g' | sed -e 's,/,.,g'`/" \
	-e "s/EXECUTABLE/RegardsViewer/" \
	-e "s/VERSION/2.63.6/" \
	../Regards/mac/Info.plist.in >./Release/RegardsViewer.app/Contents/Info.plist
/bin/echo "APPL????" >./Release/RegardsViewer.app/Contents/PkgInfo
ln -f ./Release/RegardsViewer ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
cp -f ../Regards/mac/wxmac.icns ./Release/RegardsViewer.app/Contents/Resources/wxmac.icns
cp -R ../Regards/Resources ./Release/RegardsViewer.app/Contents/
cp ../Resource/theme/RegardsViewer.viewer.theme ./Release/RegardsViewer.app/Contents/MacOs/RegardsViewer.viewer.theme
cp ../libextern/exiv2-0.27.3/build/lib/libexiv2.27.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
install_name_tool -change @rpath/libexiv2.27.dylib @executable_path/../Frameworks/libexiv2.27.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
cp /opt/homebrew/opt/poppler/lib/libpoppler.105.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
install_name_tool -change /opt/homebrew/opt/poppler/lib/libpoppler.105.dylib @executable_path/../Frameworks/libpoppler.105.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
cp /opt/homebrew/opt/poppler/lib/libpoppler-cpp.0.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
install_name_tool -change /opt/homebrew/opt/poppler/lib/libpoppler-cpp.0.dylib @executable_path/../Frameworks/libpoppler-cpp.0.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
cp $HOME/ffmpeg_build/lib/libopencv_calib3d.4.3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
install_name_tool -change @rpath/libopencv_calib3d.4.3.dylib @executable_path/../Frameworks/libopencv_calib3d.4.3.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
cp $HOME/ffmpeg_build/lib/libopencv_core.4.3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
install_name_tool -change @rpath/libopencv_core.4.3.dylib @executable_path/../Frameworks/libopencv_core.4.3.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
cp $HOME/ffmpeg_build/lib/libopencv_dnn.4.3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
install_name_tool -change @rpath/libopencv_dnn.4.3.dylib @executable_path/../Frameworks/libopencv_dnn.4.3.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
cp $HOME/ffmpeg_build/lib/libopencv_features2d.4.3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
install_name_tool -change @rpath/libopencv_features2d.4.3.dylib @executable_path/../Frameworks/libopencv_features2d.4.3.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
cp $HOME/ffmpeg_build/lib/libopencv_imgproc.4.3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
install_name_tool -change @rpath/libopencv_imgproc.4.3.dylib @executable_path/../Frameworks/libopencv_imgproc.4.3.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
cp $HOME/ffmpeg_build/lib/libopencv_imgcodecs.4.3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
install_name_tool -change @rpath/libopencv_imgcodecs.4.3.dylib @executable_path/../Frameworks/libopencv_imgcodecs.4.3.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
cp $HOME/ffmpeg_build/lib/libopencv_highgui.4.3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
install_name_tool -change @rpath/libopencv_highgui.4.3.dylib @executable_path/../Frameworks/libopencv_highgui.4.3.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
cp $HOME/ffmpeg_build/lib/libopencv_face.4.3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
install_name_tool -change @rpath/libopencv_face.4.3.dylib @executable_path/../Frameworks/libopencv_face.4.3.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
cp $HOME/ffmpeg_build/lib/libopencv_objdetect.4.3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
install_name_tool -change @rpath/libopencv_objdetect.4.3.dylib @executable_path/../Frameworks/libopencv_objdetect.4.3.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
cp /opt/homebrew/opt/libjpeg-turbo/lib/libturbojpeg.0.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
install_name_tool -change @rpath/libturbojpeg.0.dylib @executable_path/../Frameworks/libturbojpeg.0.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
cp /opt/homebrew/lib/libGLEW.2.2.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp /opt/homebrew/lib/libcairo.2.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp /opt/homebrew/lib/libjasper.4.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp $HOME/ffmpeg_build/lib/libavformat.58.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp $HOME/ffmpeg_build/lib/libavcodec.58.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp $HOME/ffmpeg_build/lib/libavdevice.58.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp $HOME/ffmpeg_build/lib/libavfilter.7.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp $HOME/ffmpeg_build/lib/libavutil.56.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp $HOME/ffmpeg_build/lib/libpostproc.55.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp $HOME/ffmpeg_build/lib/libswresample.3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp $HOME/ffmpeg_build/lib/libswscale.5.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp /opt/homebrew/lib/libglib-2.0.0.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp ../Otherlib/libbpg/libRegardsBpg_mac/Release/libRegardsBpg.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp /opt/homebrew/lib/libjpeg.9.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp /opt/homebrew/lib/liblzma.5.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp /opt/homebrew/lib/libSDL2-2.0.0.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp /opt/homebrew/lib/libde265.0.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp /opt/homebrew/lib/libexif.12.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp /opt/homebrew/lib/libfontconfig.1.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp /opt/homebrew/lib/libqpdf.28.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp /opt/homebrew/lib/libtesseract.4.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp /opt/homebrew/lib/liblept.5.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp /opt/homebrew/lib/libtbb.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp /opt/homebrew/lib/libIlmImf-2_5.25.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp /opt/homebrew/lib/libIexMath-2_5.25.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp /opt/homebrew/lib/libHalf-2_5.25.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp /opt/homebrew/lib/libx265.192.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp /opt/homebrew/lib/libtiff.5.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp /opt/homebrew/lib/libpng16.16.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp /opt/homebrew/lib/libraw_r.20.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp /opt/homebrew/lib/libopenjp2.7.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp /opt/homebrew/lib/libIex-2_5.25.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp /opt/homebrew/lib/libpixman-1.0.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp /opt/homebrew/lib/libfreetype.6.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp /opt/homebrew/lib/libdav1d.4.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp /opt/homebrew/lib/libfdk-aac.2.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp /opt/homebrew/lib/libvorbis.0.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp /opt/homebrew/lib/libvorbisenc.2.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp /opt/homebrew/lib/libx264.161.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp /opt/homebrew/lib/libpcre.1.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp /opt/homebrew/lib/libintl.8.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp /opt/homebrew/lib/liblcms2.2.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp /opt/homebrew/lib/libgif.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp /opt/homebrew/lib/libwebpmux.3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp /opt/homebrew/lib/libwebp.7.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp /opt/homebrew/lib/libopenjp2.7.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp /opt/homebrew/lib/libImath-2_5.25.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp /opt/homebrew/lib/libIlmThread-2_5.25.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp /opt/homebrew/lib/libogg.0.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp /opt/homebrew/lib/libomp.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp /opt/homebrew/opt/nss/lib/libnss3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp /opt/homebrew/opt/nss/lib/libnssutil3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp /opt/homebrew/opt/nss/lib/libsmime3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp /opt/homebrew/opt/nss/lib/libssl3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp /opt/homebrew/opt/nspr/lib/libplds4.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp /opt/homebrew/opt/nspr/lib/libplc4.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp /opt/homebrew/opt/nspr/lib/libnspr4.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp $HOME/ffmpeg_build/lib/libopencv_flann.4.3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp $HOME/ffmpeg_build/lib/libopencv_videoio.4.3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp $HOME/ffmpeg_build/lib/libopencv_photo.4.3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
cp /opt/homebrew/lib/liblept.5.dylib ./Release/RegardsViewer.app/Contents/Frameworks/
install_name_tool -change /opt/homebrew/opt/glew/lib/libGLEW.2.2.dylib @executable_path/../Frameworks/libGLEW.2.2.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change /opt/homebrew/opt/cairo/lib/libcairo.2.dylib @executable_path/../Frameworks/libcairo.2.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change /opt/homebrew/opt/jasper/lib/libjasper.4.dylib @executable_path/../Frameworks/libjasper.4.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change /Users/jacques/ffmpeg_build/lib/libavformat.58.dylib @executable_path/../Frameworks/libavformat.58.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change /Users/jacques/ffmpeg_build/lib/libavcodec.58.dylib @executable_path/../Frameworks/libavcodec.58.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change /Users/jacques/ffmpeg_build/lib/libavdevice.58.dylib @executable_path/../Frameworks/libavdevice.58.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change /Users/jacques/ffmpeg_build/lib/libavfilter.7.dylib @executable_path/../Frameworks/libavfilter.7.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change /Users/jacques/ffmpeg_build/lib/libavutil.56.dylib @executable_path/../Frameworks/libavutil.56.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change /Users/jacques/ffmpeg_build/lib/libpostproc.55.dylib @executable_path/../Frameworks/libpostproc.55.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change /Users/jacques/ffmpeg_build/lib/libswresample.3.dylib @executable_path/../Frameworks/libswresample.3.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change /opt/homebrew/opt/libomp/lib/libomp.dylib @executable_path/../Frameworks/libomp.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change /Users/jacques/ffmpeg_build/lib/libswscale.5.dylib @executable_path/../Frameworks/libswscale.5.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change /opt/homebrew/opt/glib/lib/libglib-2.0.0.dylib @executable_path/../Frameworks/libglib-2.0.0.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change /opt/homebrew/opt/jpeg/lib/libjpeg.9.dylib @executable_path/../Frameworks/libjpeg.9.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change /opt/homebrew/opt/xz/lib/liblzma.5.dylib @executable_path/../Frameworks/liblzma.5.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change /opt/homebrew/opt/sdl2/lib/libSDL2-2.0.0.dylib @executable_path/../Frameworks/libSDL2-2.0.0.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change /opt/homebrew/opt/libde265/lib/libde265.0.dylib @executable_path/../Frameworks/libde265.0.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change /opt/homebrew/opt/libexif/lib/libexif.12.dylib @executable_path/../Frameworks/libexif.12.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change /opt/homebrew/opt/fontconfig/lib/libfontconfig.1.dylib @executable_path/../Frameworks/libfontconfig.1.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change /opt/homebrew/opt/qpdf/lib/libqpdf.28.dylib @executable_path/../Frameworks/libqpdf.28.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change /opt/homebrew/opt/tesseract/lib/libtesseract.4.dylib @executable_path/../Frameworks/libtesseract.4.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change /opt/homebrew/opt/leptonica/lib/liblept.5.dylib @executable_path/../Frameworks/liblept.5.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change /opt/homebrew/opt/tbb/lib/libtbb.dylib @executable_path/../Frameworks/libtbb.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change /opt/homebrew/opt/openexr/lib/libIlmImf-2_5.25.dylib @executable_path/../Frameworks/libIlmImf-2_5.25.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change /opt/homebrew/opt/ilmbase/lib/libIexMath-2_5.25.dylib @executable_path/../Frameworks/libIexMath-2_5.25.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change /opt/homebrew/opt/ilmbase/lib/libHalf-2_5.25.dylib @executable_path/../Frameworks/libHalf-2_5.25.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change /opt/homebrew/opt/x265/lib/libx265.192.dylib @executable_path/../Frameworks/libx265.192.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change /opt/homebrew/opt/libtiff/lib/libtiff.5.dylib @executable_path/../Frameworks/libtiff.5.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change /opt/homebrew/opt/libpng/lib/libpng16.16.dylib @executable_path/../Frameworks/libpng16.16.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change /opt/homebrew/opt/libraw/lib/libraw_r.20.dylib @executable_path/../Frameworks/libraw_r.20.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change /opt/homebrew/opt/openjpeg/lib/libopenjp2.7.dylib @executable_path/../Frameworks/libopenjp2.7.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change /opt/homebrew/opt/ilmbase/lib/libIex-2_5.25.dylib @executable_path/../Frameworks/libIex-2_5.25.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
install_name_tool -change @rpath/libopencv_features2d.4.3.dylib @executable_path/../Frameworks/libopencv_features2d.4.3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libopencv_calib3d.4.3.dylib
install_name_tool -change @rpath/libopencv_flann.4.3.dylib @executable_path/../Frameworks/libopencv_flann.4.3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libopencv_calib3d.4.3.dylib
install_name_tool -change @rpath/libopencv_imgproc.4.3.dylib @executable_path/../Frameworks/libopencv_imgproc.4.3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libopencv_calib3d.4.3.dylib
install_name_tool -change @rpath/libopencv_core.4.3.dylib @executable_path/../Frameworks/libopencv_core.4.3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libopencv_calib3d.4.3.dylib
#File : /Users/jacques/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libpostproc.55.dylib
install_name_tool -change /Users/jacques/ffmpeg_build/lib/libavutil.56.dylib @executable_path/../Frameworks/libavutil.56.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libpostproc.55.dylib
#File : /Users/jacques/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libexif.12.dylib
install_name_tool -change /opt/homebrew/opt/gettext/lib/libintl.8.dylib @executable_path/../Frameworks/libintl.8.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libexif.12.dylib
#File : /Users/jacques/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libswscale.5.dylib
install_name_tool -change /Users/jacques/ffmpeg_build/lib/libavutil.56.dylib @executable_path/../Frameworks/libavutil.56.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libswscale.5.dylib
#File : /Users/jacques/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libcairo.2.dylib
install_name_tool -change /opt/homebrew/opt/pixman/lib/libpixman-1.0.dylib @executable_path/../Frameworks/libpixman-1.0.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libcairo.2.dylib
install_name_tool -change /opt/homebrew/opt/fontconfig/lib/libfontconfig.1.dylib @executable_path/../Frameworks/libfontconfig.1.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libcairo.2.dylib
install_name_tool -change /opt/homebrew/opt/freetype/lib/libfreetype.6.dylib @executable_path/../Frameworks/libfreetype.6.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libcairo.2.dylib
install_name_tool -change /opt/homebrew/opt/libpng/lib/libpng16.16.dylib @executable_path/../Frameworks/libpng16.16.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libcairo.2.dylib
#File : /Users/jacques/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libjasper.4.dylib
install_name_tool -change /opt/homebrew/opt/jpeg/lib/libjpeg.9.dylib @executable_path/../Frameworks/libjpeg.9.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libjasper.4.dylib
#File : /Users/jacques/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libswresample.3.dylib
install_name_tool -change /Users/jacques/ffmpeg_build/lib/libavutil.56.dylib @executable_path/../Frameworks/libavutil.56.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libswresample.3.dylib
#File : /Users/jacques/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libIexMath-2_5.25.dylib
install_name_tool -change @rpath/libIex-2_5.25.dylib @executable_path/../Frameworks/libIex-2_5.25.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libIexMath-2_5.25.dylib
#File : /Users/jacques/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/liblept.5.dylib
install_name_tool -change /opt/homebrew/opt/libpng/lib/libpng16.16.dylib @executable_path/../Frameworks/libpng16.16.dylib ./Release/RegardsViewer.app/Contents/Frameworks/liblept.5.dylib
install_name_tool -change /opt/homebrew/opt/jpeg/lib/libjpeg.9.dylib @executable_path/../Frameworks/libjpeg.9.dylib ./Release/RegardsViewer.app/Contents/Frameworks/liblept.5.dylib
install_name_tool -change /opt/homebrew/opt/giflib/lib/libgif.dylib @executable_path/../Frameworks/libgif.dylib ./Release/RegardsViewer.app/Contents/Frameworks/liblept.5.dylib
install_name_tool -change /opt/homebrew/opt/libtiff/lib/libtiff.5.dylib @executable_path/../Frameworks/libtiff.5.dylib ./Release/RegardsViewer.app/Contents/Frameworks/liblept.5.dylib
install_name_tool -change /opt/homebrew/opt/webp/lib/libwebpmux.3.dylib @executable_path/../Frameworks/libwebpmux.3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/liblept.5.dylib
install_name_tool -change /opt/homebrew/opt/webp/lib/libwebp.7.dylib @executable_path/../Frameworks/libwebp.7.dylib ./Release/RegardsViewer.app/Contents/Frameworks/liblept.5.dylib
install_name_tool -change /opt/homebrew/opt/openjpeg/lib/libopenjp2.7.dylib @executable_path/../Frameworks/libopenjp2.7.dylib ./Release/RegardsViewer.app/Contents/Frameworks/liblept.5.dylib
#File : /Users/jacques/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libopencv_features2d.4.3.dylib
install_name_tool -change @rpath/libopencv_flann.4.3.dylib @executable_path/../Frameworks/libopencv_flann.4.3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libopencv_features2d.4.3.dylib
install_name_tool -change @rpath/libopencv_imgproc.4.3.dylib @executable_path/../Frameworks/libopencv_imgproc.4.3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libopencv_features2d.4.3.dylib
install_name_tool -change @rpath/libopencv_core.4.3.dylib @executable_path/../Frameworks/libopencv_core.4.3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libopencv_features2d.4.3.dylib
#File : /Users/jacques/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libavfilter.7.dylib
install_name_tool -change /Users/jacques/ffmpeg_build/lib/libswscale.5.dylib @executable_path/../Frameworks/libswscale.5.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libavfilter.7.dylib
install_name_tool -change /Users/jacques/ffmpeg_build/lib/libpostproc.55.dylib @executable_path/../Frameworks/libpostproc.55.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libavfilter.7.dylib
install_name_tool -change /Users/jacques/ffmpeg_build/lib/libavformat.58.dylib @executable_path/../Frameworks/libavformat.58.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libavfilter.7.dylib
install_name_tool -change /Users/jacques/ffmpeg_build/lib/libavcodec.58.dylib @executable_path/../Frameworks/libavcodec.58.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libavfilter.7.dylib
install_name_tool -change /Users/jacques/ffmpeg_build/lib/libswresample.3.dylib @executable_path/../Frameworks/libswresample.3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libavfilter.7.dylib
install_name_tool -change /Users/jacques/ffmpeg_build/lib/libavutil.56.dylib @executable_path/../Frameworks/libavutil.56.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libavfilter.7.dylib
install_name_tool -change /opt/homebrew/opt/dav1d/lib/libdav1d.4.dylib @executable_path/../Frameworks/libdav1d.4.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libavfilter.7.dylib
install_name_tool -change /opt/homebrew/opt/fdk-aac/lib/libfdk-aac.2.dylib @executable_path/../Frameworks/libfdk-aac.2.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libavfilter.7.dylib
install_name_tool -change /opt/homebrew/opt/libvorbis/lib/libvorbis.0.dylib @executable_path/../Frameworks/libvorbis.0.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libavfilter.7.dylib
install_name_tool -change /opt/homebrew/opt/libvorbis/lib/libvorbisenc.2.dylib @executable_path/../Frameworks/libvorbisenc.2.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libavfilter.7.dylib
install_name_tool -change /opt/homebrew/opt/x264/lib/libx264.161.dylib @executable_path/../Frameworks/libx264.161.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libavfilter.7.dylib
install_name_tool -change /opt/homebrew/opt/x265/lib/libx265.192.dylib @executable_path/../Frameworks/libx265.192.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libavfilter.7.dylib
#File : /Users/jacques/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libglib-2.0.0.dylib
install_name_tool -change /opt/homebrew/opt/pcre/lib/libpcre.1.dylib @executable_path/../Frameworks/libpcre.1.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libglib-2.0.0.dylib
install_name_tool -change /opt/homebrew/opt/gettext/lib/libintl.8.dylib @executable_path/../Frameworks/libintl.8.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libglib-2.0.0.dylib
#File : /Users/jacques/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libavformat.58.dylib
install_name_tool -change /Users/jacques/ffmpeg_build/lib/libavcodec.58.dylib @executable_path/../Frameworks/libavcodec.58.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libavformat.58.dylib
install_name_tool -change /Users/jacques/ffmpeg_build/lib/libswresample.3.dylib @executable_path/../Frameworks/libswresample.3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libavformat.58.dylib
install_name_tool -change /Users/jacques/ffmpeg_build/lib/libavutil.56.dylib @executable_path/../Frameworks/libavutil.56.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libavformat.58.dylib
install_name_tool -change /opt/homebrew/opt/dav1d/lib/libdav1d.4.dylib @executable_path/../Frameworks/libdav1d.4.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libavformat.58.dylib
install_name_tool -change /opt/homebrew/opt/fdk-aac/lib/libfdk-aac.2.dylib @executable_path/../Frameworks/libfdk-aac.2.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libavformat.58.dylib
install_name_tool -change /opt/homebrew/opt/libvorbis/lib/libvorbis.0.dylib @executable_path/../Frameworks/libvorbis.0.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libavformat.58.dylib
install_name_tool -change /opt/homebrew/opt/libvorbis/lib/libvorbisenc.2.dylib @executable_path/../Frameworks/libvorbisenc.2.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libavformat.58.dylib
install_name_tool -change /opt/homebrew/opt/x264/lib/libx264.161.dylib @executable_path/../Frameworks/libx264.161.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libavformat.58.dylib
install_name_tool -change /opt/homebrew/opt/x265/lib/libx265.192.dylib @executable_path/../Frameworks/libx265.192.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libavformat.58.dylib
#File : /Users/jacques/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libopencv_face.4.3.dylib
install_name_tool -change @rpath/libopencv_photo.4.3.dylib @executable_path/../Frameworks/libopencv_photo.4.3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libopencv_face.4.3.dylib
install_name_tool -change @rpath/libopencv_objdetect.4.3.dylib @executable_path/../Frameworks/libopencv_objdetect.4.3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libopencv_face.4.3.dylib
install_name_tool -change @rpath/libopencv_calib3d.4.3.dylib @executable_path/../Frameworks/libopencv_calib3d.4.3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libopencv_face.4.3.dylib
install_name_tool -change @rpath/libopencv_features2d.4.3.dylib @executable_path/../Frameworks/libopencv_features2d.4.3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libopencv_face.4.3.dylib
install_name_tool -change @rpath/libopencv_flann.4.3.dylib @executable_path/../Frameworks/libopencv_flann.4.3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libopencv_face.4.3.dylib
install_name_tool -change @rpath/libopencv_imgproc.4.3.dylib @executable_path/../Frameworks/libopencv_imgproc.4.3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libopencv_face.4.3.dylib
install_name_tool -change @rpath/libopencv_core.4.3.dylib @executable_path/../Frameworks/libopencv_core.4.3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libopencv_face.4.3.dylib
#File : /Users/jacques/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libpoppler.105.dylib
install_name_tool -change /opt/homebrew/opt/freetype/lib/libfreetype.6.dylib @executable_path/../Frameworks/libfreetype.6.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libpoppler.105.dylib
install_name_tool -change /opt/homebrew/opt/fontconfig/lib/libfontconfig.1.dylib @executable_path/../Frameworks/libfontconfig.1.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libpoppler.105.dylib
install_name_tool -change /opt/homebrew/opt/jpeg/lib/libjpeg.9.dylib @executable_path/../Frameworks/libjpeg.9.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libpoppler.105.dylib
install_name_tool -change /opt/homebrew/opt/openjpeg/lib/libopenjp2.7.dylib @executable_path/../Frameworks/libopenjp2.7.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libpoppler.105.dylib
install_name_tool -change /opt/homebrew/opt/little-cms2/lib/liblcms2.2.dylib @executable_path/../Frameworks/liblcms2.2.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libpoppler.105.dylib
install_name_tool -change /opt/homebrew/opt/libpng/lib/libpng16.16.dylib @executable_path/../Frameworks/libpng16.16.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libpoppler.105.dylib
install_name_tool -change /opt/homebrew/opt/libtiff/lib/libtiff.5.dylib @executable_path/../Frameworks/libtiff.5.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libpoppler.105.dylib
install_name_tool -change /opt/homebrew/opt/nss/lib/libnss3.dylib @executable_path/../Frameworks/libnss3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libpoppler.105.dylib
install_name_tool -change /opt/homebrew/opt/nss/lib/libnssutil3.dylib @executable_path/../Frameworks/libnssutil3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libpoppler.105.dylib
install_name_tool -change /opt/homebrew/opt/nss/lib/libsmime3.dylib @executable_path/../Frameworks/libsmime3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libpoppler.105.dylib
install_name_tool -change /opt/homebrew/opt/nss/lib/libssl3.dylib @executable_path/../Frameworks/libssl3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libpoppler.105.dylib
install_name_tool -change /opt/homebrew/opt/nspr/lib/libplds4.dylib @executable_path/../Frameworks/libplds4.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libpoppler.105.dylib
install_name_tool -change /opt/homebrew/opt/nspr/lib/libplc4.dylib @executable_path/../Frameworks/libplc4.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libpoppler.105.dylib
install_name_tool -change /opt/homebrew/opt/nspr/lib/libnspr4.dylib @executable_path/../Frameworks/libnspr4.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libpoppler.105.dylib
#File : /Users/jacques/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libopencv_imgproc.4.3.dylib
install_name_tool -change @rpath/libopencv_core.4.3.dylib @executable_path/../Frameworks/libopencv_core.4.3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libopencv_imgproc.4.3.dylib
#File : /Users/jacques/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libopencv_dnn.4.3.dylib
install_name_tool -change @rpath/libopencv_imgproc.4.3.dylib @executable_path/../Frameworks/libopencv_imgproc.4.3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libopencv_dnn.4.3.dylib
install_name_tool -change @rpath/libopencv_core.4.3.dylib @executable_path/../Frameworks/libopencv_core.4.3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libopencv_dnn.4.3.dylib
#File : /Users/jacques/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libavdevice.58.dylib
install_name_tool -change /Users/jacques/ffmpeg_build/lib/libavfilter.7.dylib @executable_path/../Frameworks/libavfilter.7.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libavdevice.58.dylib
install_name_tool -change /Users/jacques/ffmpeg_build/lib/libswscale.5.dylib @executable_path/../Frameworks/libswscale.5.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libavdevice.58.dylib
install_name_tool -change /Users/jacques/ffmpeg_build/lib/libpostproc.55.dylib @executable_path/../Frameworks/libpostproc.55.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libavdevice.58.dylib
install_name_tool -change /Users/jacques/ffmpeg_build/lib/libavformat.58.dylib @executable_path/../Frameworks/libavformat.58.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libavdevice.58.dylib
install_name_tool -change /Users/jacques/ffmpeg_build/lib/libavcodec.58.dylib @executable_path/../Frameworks/libavcodec.58.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libavdevice.58.dylib
install_name_tool -change /Users/jacques/ffmpeg_build/lib/libswresample.3.dylib @executable_path/../Frameworks/libswresample.3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libavdevice.58.dylib
install_name_tool -change /Users/jacques/ffmpeg_build/lib/libavutil.56.dylib @executable_path/../Frameworks/libavutil.56.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libavdevice.58.dylib
install_name_tool -change /opt/homebrew/opt/sdl2/lib/libSDL2-2.0.0.dylib @executable_path/../Frameworks/libSDL2-2.0.0.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libavdevice.58.dylib
install_name_tool -change /opt/homebrew/opt/dav1d/lib/libdav1d.4.dylib @executable_path/../Frameworks/libdav1d.4.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libavdevice.58.dylib
install_name_tool -change /opt/homebrew/opt/fdk-aac/lib/libfdk-aac.2.dylib @executable_path/../Frameworks/libfdk-aac.2.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libavdevice.58.dylib
install_name_tool -change /opt/homebrew/opt/libvorbis/lib/libvorbis.0.dylib @executable_path/../Frameworks/libvorbis.0.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libavdevice.58.dylib
install_name_tool -change /opt/homebrew/opt/libvorbis/lib/libvorbisenc.2.dylib @executable_path/../Frameworks/libvorbisenc.2.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libavdevice.58.dylib
install_name_tool -change /opt/homebrew/opt/x264/lib/libx264.161.dylib @executable_path/../Frameworks/libx264.161.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libavdevice.58.dylib
install_name_tool -change /opt/homebrew/opt/x265/lib/libx265.192.dylib @executable_path/../Frameworks/libx265.192.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libavdevice.58.dylib
#File : /Users/jacques/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libfontconfig.1.dylib
install_name_tool -change /opt/homebrew/opt/freetype/lib/libfreetype.6.dylib @executable_path/../Frameworks/libfreetype.6.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libfontconfig.1.dylib
#File : /Users/jacques/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libpoppler-cpp.0.dylib
install_name_tool -change /opt/homebrew/Cellar/poppler/20.12.1/lib/libpoppler.105.dylib @executable_path/../Frameworks/libpoppler.105.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libpoppler-cpp.0.dylib
#File : /Users/jacques/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libqpdf.28.dylib
install_name_tool -change /opt/homebrew/opt/jpeg/lib/libjpeg.9.dylib @executable_path/../Frameworks/libjpeg.9.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libqpdf.28.dylib
#File : /Users/jacques/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libtiff.5.dylib
install_name_tool -change /opt/homebrew/opt/jpeg/lib/libjpeg.9.dylib @executable_path/../Frameworks/libjpeg.9.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libtiff.5.dylib
#File : /Users/jacques/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libopencv_highgui.4.3.dylib
install_name_tool -change @rpath/libopencv_videoio.4.3.dylib @executable_path/../Frameworks/libopencv_videoio.4.3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libopencv_highgui.4.3.dylib
install_name_tool -change @rpath/libopencv_imgcodecs.4.3.dylib @executable_path/../Frameworks/libopencv_imgcodecs.4.3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libopencv_highgui.4.3.dylib
install_name_tool -change @rpath/libopencv_imgproc.4.3.dylib @executable_path/../Frameworks/libopencv_imgproc.4.3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libopencv_highgui.4.3.dylib
install_name_tool -change @rpath/libopencv_core.4.3.dylib @executable_path/../Frameworks/libopencv_core.4.3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libopencv_highgui.4.3.dylib
#File : /Users/jacques/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libraw_r.20.dylib
install_name_tool -change /opt/homebrew/opt/libomp/lib/libomp.dylib @executable_path/../Frameworks/libomp.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libraw_r.20.dylib
install_name_tool -change /opt/homebrew/opt/jpeg/lib/libjpeg.9.dylib @executable_path/../Frameworks/libjpeg.9.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libraw_r.20.dylib
install_name_tool -change /opt/homebrew/opt/jasper/lib/libjasper.4.dylib @executable_path/../Frameworks/libjasper.4.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libraw_r.20.dylib
install_name_tool -change /opt/homebrew/opt/little-cms2/lib/liblcms2.2.dylib @executable_path/../Frameworks/liblcms2.2.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libraw_r.20.dylib
#File : /Users/jacques/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libopencv_objdetect.4.3.dylib
install_name_tool -change @rpath/libopencv_calib3d.4.3.dylib @executable_path/../Frameworks/libopencv_calib3d.4.3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libopencv_objdetect.4.3.dylib
install_name_tool -change @rpath/libopencv_features2d.4.3.dylib @executable_path/../Frameworks/libopencv_features2d.4.3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libopencv_objdetect.4.3.dylib
install_name_tool -change @rpath/libopencv_flann.4.3.dylib @executable_path/../Frameworks/libopencv_flann.4.3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libopencv_objdetect.4.3.dylib
install_name_tool -change @rpath/libopencv_imgproc.4.3.dylib @executable_path/../Frameworks/libopencv_imgproc.4.3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libopencv_objdetect.4.3.dylib
install_name_tool -change @rpath/libopencv_core.4.3.dylib @executable_path/../Frameworks/libopencv_core.4.3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libopencv_objdetect.4.3.dylib
#File : /Users/jacques/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libopencv_imgcodecs.4.3.dylib
install_name_tool -change @rpath/libopencv_imgproc.4.3.dylib @executable_path/../Frameworks/libopencv_imgproc.4.3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libopencv_imgcodecs.4.3.dylib
install_name_tool -change /opt/homebrew/opt/openjpeg/lib/libopenjp2.7.dylib @executable_path/../Frameworks/libopenjp2.7.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libopencv_imgcodecs.4.3.dylib
install_name_tool -change @rpath/libopencv_core.4.3.dylib @executable_path/../Frameworks/libopencv_core.4.3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libopencv_imgcodecs.4.3.dylib
#File : /Users/jacques/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libavcodec.58.dylib
install_name_tool -change /Users/jacques/ffmpeg_build/lib/libswresample.3.dylib @executable_path/../Frameworks/libswresample.3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libavcodec.58.dylib
install_name_tool -change /Users/jacques/ffmpeg_build/lib/libavutil.56.dylib @executable_path/../Frameworks/libavutil.56.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libavcodec.58.dylib
install_name_tool -change /opt/homebrew/opt/dav1d/lib/libdav1d.4.dylib @executable_path/../Frameworks/libdav1d.4.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libavcodec.58.dylib
install_name_tool -change /opt/homebrew/opt/fdk-aac/lib/libfdk-aac.2.dylib @executable_path/../Frameworks/libfdk-aac.2.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libavcodec.58.dylib
install_name_tool -change /opt/homebrew/opt/libvorbis/lib/libvorbis.0.dylib @executable_path/../Frameworks/libvorbis.0.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libavcodec.58.dylib
install_name_tool -change /opt/homebrew/opt/libvorbis/lib/libvorbisenc.2.dylib @executable_path/../Frameworks/libvorbisenc.2.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libavcodec.58.dylib
install_name_tool -change /opt/homebrew/opt/x264/lib/libx264.161.dylib @executable_path/../Frameworks/libx264.161.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libavcodec.58.dylib
install_name_tool -change /opt/homebrew/opt/x265/lib/libx265.192.dylib @executable_path/../Frameworks/libx265.192.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libavcodec.58.dylib
#File : /Users/jacques/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libtesseract.4.dylib
install_name_tool -change /opt/homebrew/opt/leptonica/lib/liblept.5.dylib @executable_path/../Frameworks/liblept.5.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libtesseract.4.dylib
#File : /Users/jacques/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libIlmImf-2_5.25.dylib
install_name_tool -change /opt/homebrew/opt/ilmbase/lib/libImath-2_5.25.dylib @executable_path/../Frameworks/libImath-2_5.25.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libIlmImf-2_5.25.dylib
install_name_tool -change /opt/homebrew/opt/ilmbase/lib/libIlmThread-2_5.25.dylib @executable_path/../Frameworks/libIlmThread-2_5.25.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libIlmImf-2_5.25.dylib
install_name_tool -change /opt/homebrew/opt/ilmbase/lib/libHalf-2_5.25.dylib @executable_path/../Frameworks/libHalf-2_5.25.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libIlmImf-2_5.25.dylib
install_name_tool -change /opt/homebrew/opt/ilmbase/lib/libIexMath-2_5.25.dylib @executable_path/../Frameworks/libIexMath-2_5.25.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libIlmImf-2_5.25.dylib
install_name_tool -change /opt/homebrew/opt/ilmbase/lib/libIex-2_5.25.dylib @executable_path/../Frameworks/libIex-2_5.25.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libIlmImf-2_5.25.dylib
#File : /Users/jacques/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libvorbisenc.2.dylib
install_name_tool -change /opt/homebrew/Cellar/libvorbis/1.3.7/lib/libvorbis.0.dylib @executable_path/../Frameworks/libvorbis.0.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libvorbisenc.2.dylib
install_name_tool -change /opt/homebrew/opt/libogg/lib/libogg.0.dylib @executable_path/../Frameworks/libogg.0.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libvorbisenc.2.dylib
#File : /Users/jacques/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libfreetype.6.dylib
install_name_tool -change /opt/homebrew/opt/libpng/lib/libpng16.16.dylib @executable_path/../Frameworks/libpng16.16.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libfreetype.6.dylib
#File : /Users/jacques/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libopencv_flann.4.3.dylib
install_name_tool -change @rpath/libopencv_core.4.3.dylib @executable_path/../Frameworks/libopencv_core.4.3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libopencv_flann.4.3.dylib
#File : /Users/jacques/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libwebpmux.3.dylib
install_name_tool -change /opt/homebrew/Cellar/webp/1.1.0/lib/libwebp.7.dylib @executable_path/../Frameworks/libwebp.7.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libwebpmux.3.dylib
#File : /Users/jacques/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libvorbis.0.dylib
install_name_tool -change /opt/homebrew/opt/libogg/lib/libogg.0.dylib @executable_path/../Frameworks/libogg.0.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libvorbis.0.dylib
#File : /Users/jacques/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libopencv_videoio.4.3.dylib
install_name_tool -change @rpath/libopencv_imgcodecs.4.3.dylib @executable_path/../Frameworks/libopencv_imgcodecs.4.3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libopencv_videoio.4.3.dylib
install_name_tool -change @rpath/libopencv_imgproc.4.3.dylib @executable_path/../Frameworks/libopencv_imgproc.4.3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libopencv_videoio.4.3.dylib
install_name_tool -change @rpath/libopencv_core.4.3.dylib @executable_path/../Frameworks/libopencv_core.4.3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libopencv_videoio.4.3.dylib
#File : /Users/jacques/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libIlmThread-2_5.25.dylib
install_name_tool -change @rpath/libIex-2_5.25.dylib @executable_path/../Frameworks/libIex-2_5.25.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libIlmThread-2_5.25.dylib
#File : /Users/jacques/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libImath-2_5.25.dylib
install_name_tool -change @rpath/libHalf-2_5.25.dylib @executable_path/../Frameworks/libHalf-2_5.25.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libImath-2_5.25.dylib
install_name_tool -change @rpath/libIexMath-2_5.25.dylib @executable_path/../Frameworks/libIexMath-2_5.25.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libImath-2_5.25.dylib
install_name_tool -change @rpath/libIex-2_5.25.dylib @executable_path/../Frameworks/libIex-2_5.25.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libImath-2_5.25.dylib
#File : /Users/jacques/developpement/git/Regards/RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libopencv_photo.4.3.dylib
install_name_tool -change @rpath/libopencv_imgproc.4.3.dylib @executable_path/../Frameworks/libopencv_imgproc.4.3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libopencv_photo.4.3.dylib
install_name_tool -change @rpath/libopencv_core.4.3.dylib @executable_path/../Frameworks/libopencv_core.4.3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libopencv_photo.4.3.dylib
#File : ../../RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libplds4.dylib
install_name_tool -change /opt/homebrew/Cellar/nspr/4.29/lib/libnspr4.dylib @executable_path/../Frameworks/libnspr4.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libplds4.dylib
#File : ../../RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libsmime3.dylib
install_name_tool -change /opt/homebrew/Cellar/nss/3.60/lib/libnss3.dylib @executable_path/../Frameworks/libnss3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libsmime3.dylib
install_name_tool -change /opt/homebrew/Cellar/nss/3.60/lib/libnssutil3.dylib @executable_path/../Frameworks/libnssutil3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libsmime3.dylib
install_name_tool -change /opt/homebrew/opt/nspr/lib/libplc4.dylib @executable_path/../Frameworks/libplc4.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libsmime3.dylib
install_name_tool -change /opt/homebrew/opt/nspr/lib/libplds4.dylib @executable_path/../Frameworks/libplds4.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libsmime3.dylib
install_name_tool -change /opt/homebrew/opt/nspr/lib/libnspr4.dylib @executable_path/../Frameworks/libnspr4.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libsmime3.dylib
#File : ../../RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libplc4.dylib
install_name_tool -change /opt/homebrew/Cellar/nspr/4.29/lib/libnspr4.dylib @executable_path/../Frameworks/libnspr4.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libplc4.dylib
#File : ../../RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libssl3.dylib
install_name_tool -change /opt/homebrew/Cellar/nss/3.60/lib/libnss3.dylib @executable_path/../Frameworks/libnss3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libssl3.dylib
install_name_tool -change /opt/homebrew/Cellar/nss/3.60/lib/libnssutil3.dylib @executable_path/../Frameworks/libnssutil3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libssl3.dylib
install_name_tool -change /opt/homebrew/opt/nspr/lib/libplc4.dylib @executable_path/../Frameworks/libplc4.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libssl3.dylib
install_name_tool -change /opt/homebrew/opt/nspr/lib/libplds4.dylib @executable_path/../Frameworks/libplds4.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libssl3.dylib
install_name_tool -change /opt/homebrew/opt/nspr/lib/libnspr4.dylib @executable_path/../Frameworks/libnspr4.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libssl3.dylib
#File : ../../RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libnssutil3.dylib
install_name_tool -change /opt/homebrew/opt/nspr/lib/libplc4.dylib @executable_path/../Frameworks/libplc4.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libnssutil3.dylib
install_name_tool -change /opt/homebrew/opt/nspr/lib/libplds4.dylib @executable_path/../Frameworks/libplds4.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libnssutil3.dylib
install_name_tool -change /opt/homebrew/opt/nspr/lib/libnspr4.dylib @executable_path/../Frameworks/libnspr4.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libnssutil3.dylib
#File : ../../RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libnss3.dylib
install_name_tool -change /opt/homebrew/Cellar/nss/3.60/lib/libnssutil3.dylib @executable_path/../Frameworks/libnssutil3.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libnss3.dylib
install_name_tool -change /opt/homebrew/opt/nspr/lib/libplc4.dylib @executable_path/../Frameworks/libplc4.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libnss3.dylib
install_name_tool -change /opt/homebrew/opt/nspr/lib/libplds4.dylib @executable_path/../Frameworks/libplds4.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libnss3.dylib
install_name_tool -change /opt/homebrew/opt/nspr/lib/libnspr4.dylib @executable_path/../Frameworks/libnspr4.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libnss3.dylib
#File : ../../RegardsViewer_mac/Release/RegardsViewer.app/Contents/Frameworks/libRegardsBpg.dylib
install_name_tool -change /opt/homebrew/opt/x265/lib/libx265.192.dylib @executable_path/../Frameworks/libx265.192.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libRegardsBpg.dylib
install_name_tool -change /opt/homebrew/opt/libpng/lib/libpng16.16.dylib @executable_path/../Frameworks/libpng16.16.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libRegardsBpg.dylib
codesign -fs - ./Release/RegardsViewer.app  --deep
