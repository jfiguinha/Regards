if [ -d package ]; then echo "Exists"; else mkdir package;fi
cp -r ./Release/RegardsViewer.app ./package/RegardsViewer3/
rm -f ./package/RegardsViewer3.dmg
echo "Create Package"
hdiutil create -fs HFS+ -srcfolder ./package/RegardsViewer3 -volname RegardsViewer3 ./package/RegardsViewer3.dmg