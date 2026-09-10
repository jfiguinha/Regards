#/bin/bash
cp -r ../Regards/mac/lib  ./Release/RegardsViewer.app/Contents
./MacOsLibExplorer ./Release/RegardsViewer.app/Contents/MacOS ./Release/RegardsViewer.app/Contents/Frameworks change_dylib.sh 1
./MacOsLibExplorer ./Release/RegardsViewer.app/Contents/Frameworks ./Release/RegardsViewer.app/Contents/Frameworks change_dylib.sh 1
codesign --force --deep --sign "Apple Development" ./Release/RegardsViewer.app


