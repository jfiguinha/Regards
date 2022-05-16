#/bin/bash
mkdir Frameworks
rm change_dylib.sh
FILE=change_dylib.sh
./MacOsLibExplorer ./Release/RegardsViewer.app/Contents/MacOS ./Release/RegardsViewer.app/Contents/Frameworks change_dylib.sh 1
codesign -fs - ./Release/RegardsViewer.app --deep
