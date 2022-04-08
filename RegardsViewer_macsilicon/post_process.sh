#/bin/bash
mkdir Frameworks
rm change_dylib.sh
FILE=change_dylib.sh
./MacOsLibExplorer ./Release/RegardsViewer.app/Contents/MacOS ./Release/RegardsViewer.app/Contents/Frameworks change_dylib.sh 1

chmod +x change_dylib.sh
./change_dylib.sh
for i in {1..10}
do
   echo "Welcome $i times"
   rm change_dylib.sh
   ./MacOsLibExplorer ./Release/RegardsViewer.app/Contents/Frameworks ./Release/RegardsViewer.app/Contents/Frameworks change_dylib.sh
   chmod a+rw ./Release/RegardsViewer.app/Contents/Frameworks/*.dylib
 	chmod +x change_dylib.sh;
   	./change_dylib.sh;
done

codesign -fs - ./Release/RegardsViewer.app --deep

xattr -rc .