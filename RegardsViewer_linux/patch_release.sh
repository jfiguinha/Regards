patchelf --force-rpath --set-rpath '$ORIGIN' ./Release/RegardsViewer 
patchelf --force-rpath --set-rpath '$ORIGIN' ./Release/*.so.* 

