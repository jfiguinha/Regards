#!/bin/bash

PACKAGE_DIR="RegardsViewer_3.01.0.0-linux-x86_64"
PACKAGE_NAME="RegardsViewer_3.01.0.0_linux_amd64.deb"

rm -f "$PACKAGE_NAME"

find "$PACKAGE_DIR/opt" "$PACKAGE_DIR/usr" \
    -type f \
    -exec md5sum {} \; \
    | sed "s#${PACKAGE_DIR}/##" \
    > "$PACKAGE_DIR/DEBIAN/md5sums"

dpkg-deb --build \
    "$PACKAGE_DIR" \
    "$PACKAGE_NAME"
