#!/bin/bash
MOREF="$(grealpath ../libextern/libde265-master/libde265/libde265.dylib)"
echo $MOREF
install_name_tool -change $MOREF @executable_path/../Frameworks/libde265.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer