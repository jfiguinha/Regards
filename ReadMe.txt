//**************************************************************************	
Description
//**************************************************************************

Regards is a modern photo viewer. 
Support highDPI definition screen.
Support reading Picture format :  BMP files, Dr. Halo CUT, DDS, EXR files, Raw Fax G3, GIF files, HDR files, ICO files, IFF, JBIG files, JNG files, JPEG/JIF files, JPEG-2000 File Format, JPEG-2000 codestream, JPEG-XR files, KOALA, Kodak PhotoCD, MNG, PCX, PBM/PGM/PPM files, PFM files, PNG files, Macintosh PICT, Photoshop PSD, RAW camera, Sun RAS, SGI, TARGA files, TIFF files, WBMP files, WebP files, XBM, XPM files, PFM, SVG, AVIF AND HEIC.
Support reading Video format (DXVA2 acceleration support) : mpeg4, avi, mkv, webp, y4m, AV1 format, quicktime and AVCHD.
Support Exif and xmp file information.
Support New Apple Photo format for IOS 11 HEIF multithreading and Exif.
Support GPS informations from Photo and Apple Iphone Video. 
Hi Quality display interpolation. 
OpenGL Picture display and OpenCL post Effect.

Picture extension support :

pgm, bmp, cut, dds, jxr, pcx, jpg, jpe, jpeg, jfif, jif, jfi, tif, gif, png, tga, pcd, webp, bpg, jp2, jpc, j2c, pgx, ppm, psd, pdd, mng, jng, iff, xpm, ico, cur, ani, svg, nef, crw, cr2, dng, arw, erf, 3fr, dcr, raw, x3f, mef, raf, mrw, pef, sr2, orf, heic, hdr, exr, wbmp, pict, ras, pff, sgi and KOALA;

Video extension support :

mp4, dat, m4s, vob, mod, mpv2, mp2, m1v, mpe, mpg, mpeg, wtv, dvr-ms, m2ts, m2t, avi, wmv, asf, vm, mov, qt, vp8, vp9, webm, mkv, y4m

//**************************************************************************
//Installation
//**************************************************************************

Linux Mint 22 or Ubuntu 24.04 x64 Installation
- Please check if you have an opencl compatibility device :
Tape clinfo 
- if it is not working please consult this website to install a driver :
https://wiki.tiker.net/OpenCLHowTo
- Run deb package

Installation on Windows
- Run RegardsViewer2Setup.exe

Installation on Mac os X 26 and Later
- Open DMG file and Copy RegardsViewer to Application Folder.

//**************************************************************************
//Configuration
//**************************************************************************

Works on minimum windows 10 and above, Mac OS X 26 and above, Ubuntu 24.04 x64, Linux Manjaro and above. 
An OpenCL 1.2 compatible device is necessary to use this software.

//**************************************************************************
//Geolocalisation
//**************************************************************************

Create an account via the following website : https://www.geoapify.com/
Create an API Key and add this key via the configuration dialog under RegardsViewer or open Regards.config file
under My Documents/Regards folder and change <ApiKey> value

//**************************************************************************
//Software Review
//**************************************************************************

SOFTPEDIA 3,5 / 5
FIND MY SOFT 3 / 5 
HOTPICKS LINUX FORMAT JUNE 2020

//**************************************************************************
//Data Model for Regards
//**************************************************************************

Model from :
https://github.com/Tencent/ncnn
Model From :
OpenCV

//**************************************************************************
//What's New
//**************************************************************************
News for 3.11.0 :
Correct bug on saving GPS Infos
News for 3.10.0 :
Correct bug on mac os on interop opencl opengl
Correct bug when you save a picture with an effect
Correct bug with opencl filter on video
Correct bug with some interpolation method and not compatible with ffmpeg
Correct bug on start when new pictures have been add to select folder
Correct bug on criteria insert
Correct crash on video play for some video
Correct window size initiator on linux
Correct crash for export video
Migrate to OpenGL 3.30 under Mac OS
Remove SDL2 library and use OpenAL library instead
Migrate to lunasvg for SVG management
Migrate all opengl code to core 3.30
Correct memory leak with ffmpeg
Add new transition effect
Speed up Remove Folder
Check if there is only one instance
Correct crash on video apply effect
optimise opengl shader code
optimise vertical and horizontal bar
correct bug with mouse cursor over directory
Correct crash with geotag video from android

Copyright
Figuinha Jacques - 2026 - Email Me For More information

All - Version

