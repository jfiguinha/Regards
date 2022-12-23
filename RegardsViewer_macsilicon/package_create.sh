if [ -d package ]; then echo "Exists"; else mkdir package;fi
cp -r ./Release/RegardsViewer.app ./package/RegardsViewer2/
rm -f ./package/RegardsViewer2.dmg
echo "Create Package"
hdiutil create -fs HFS+ -srcfolder ./package/RegardsViewer2 -volname RegardsViewer2 ./package/RegardsViewer2.dmg