patchelf --force-rpath --set-rpath '/opt/RegardsViewer' ./Release/RegardsViewer 
patchelf --force-rpath --set-rpath '/opt/RegardsViewer' ./Release/ffmpeg 
patchelf --force-rpath --set-rpath '/opt/RegardsViewer' ./Release/*.so.* 
#patchelf --force-rpath --set-rpath '.' ./Release/RegardsViewer 
#patchelf --force-rpath --set-rpath '.' ./Release/*.so.* 
