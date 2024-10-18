patchelf --force-rpath --set-rpath '$ORIGIN' ./Release/RegardsViewer 
patchelf --force-rpath --set-rpath '$ORIGIN' ./Release/ffmpeg 
patchelf --force-rpath --set-rpath '$ORIGIN' ./Release/*.so.* 
#patchelf --force-rpath --set-rpath '.' ./Release/RegardsViewer 
#patchelf --force-rpath --set-rpath '.' ./Release/*.so.* 
