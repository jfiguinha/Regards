# Build script for freetype

MOD=freetype
VER=2.4.4
REV=1
ARCH=win64

THIS=${MOD}_${VER}-${REV}_${ARCH}

RUNZIP=${MOD}_${VER}-${REV}_${ARCH}.zip
DEVZIP=${MOD}-dev_${VER}-${REV}_${ARCH}.zip

# We use a string of hex digits to make it more evident that it is
# just a hash value and not supposed to be relevant at end-user
# machines.
HEX=`echo $THIS | md5sum | cut -d' ' -f1`
TARGET=c:/devel/target/$HEX

usedev
usemingw64
usemsvs9x64

(

set -x

# Don't let libtool do its relinking dance. Don't know how relevant
# this is, but it doesn't hurt anyway.

sed -e 's/need_relink=yes/need_relink=no # no way --tml/' <builds/unix/ltmain.sh >builds/unix/ltmain.temp && mv builds/unix/ltmain.temp builds/unix/ltmain.sh

# Do use the -export-symbols and -no-undefined libtool options.
sed -e 's/# -export/-export/' <builds/unix/unix-cc.in >builds/unix/unix-cc.in.temp && mv builds/unix/unix-cc.in.temp builds/unix/unix-cc.in

# Hide ftmac.h so that apinames won't generate export symbols for the
# functions in it, which are compiled on Macs only.
mv include/freetype/ftmac.h include/freetype/ftmac.h.bak
touch include/freetype/ftmac.h

# Don't use absolute paths in freetype-config.
sed -e 's!prefix=@prefix@!prefix=`dirname $0`/..!' -e 's/@prefix@/${prefix}/' <builds/unix/freetype-config.in >builds/unix/freetype-config.in.temp && mv builds/unix/freetype-config.in.temp builds/unix/freetype-config.in

# Don't use the $(value) function that is not present in the somewhat older GNU Make I use
sed -e 's/\$(value CFG)/$(CFG)/' <builds/unix/detect.mk >builds/unix/detect.mk.temp && mv builds/unix/detect.mk.temp builds/unix/detect.mk

# Note that even if we run the configure script with
# --prefix=/devel/target/$HEX, this does *not* mean that when
# deployed at an end-user machine, the package would have to be
# installed in such a path. With the change to freetype-config above,
# the package does not rely on any hardcoded paths.
CC='x86_64-w64-mingw32-gcc' \
LDFLAGS='-Wl,--enable-auto-image-base' \
CFLAGS=-O2 \
./configure --host=x86_64-w64-mingw32 --prefix=c:/devel/target/$HEX --disable-static &&

make -j3 &&

# Restore ftmac.h.
mv include/freetype/ftmac.h.bak include/freetype/ftmac.h &&

# Then make
make -j3 install &&

# Generate zip archive distribution

rm -f /tmp/$RUNZIP /tmp/$DEVZIP &&
(cd /devel/target/$HEX && 

pexports bin/libfreetype-6.dll >lib/freetype.def &&
lib.exe -machine:X64 -def:lib/freetype.def -out:lib/freetype.lib &&

zip /tmp/$RUNZIP bin/*.dll &&
zip /tmp/$DEVZIP bin/freetype-config &&
zip /tmp/$DEVZIP lib/libfreetype.dll.a lib/freetype.def lib/freetype.lib &&
zip /tmp/$DEVZIP lib/pkgconfig/freetype2.pc &&
zip -r -D /tmp/$DEVZIP include &&
zip -r -D /tmp/$DEVZIP share &&

: )

) 2>&1 | tee /devel/src/tml/packaging/$THIS.log

(cd /devel && zip /tmp/$DEVZIP src/tml/packaging/$THIS.{sh,log}) &&
manifestify /tmp/$RUNZIP /tmp/$DEVZIP &&

:
