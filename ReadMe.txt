//**************************************************************************	
Description
//**************************************************************************

Regards is a modern photo viewer. 
Support highDPI definition screen.
Support reading Picture format : JPEG, GIF, PNG, PCX, TGA, TIFF, BMP, Kodak Photo-Cd, JPEG2000, Photoshop PSD, PPM, PGM, Camera Raw, SVG, WEBP, HEIC, EXR, HDR and BPG.
Support reading Video format (DXVA2 acceleration support) : mpeg4, avi, mkv, webp, y4m, quicktime and AVCHD.
Support Exif and xmp file information.
Support New Apple Photo format for IOS 11 HEIF multithreading and Exif.
Support GPS informations from Photo and Apple Iphone Video. 
Hi Quality display interpolation. 
OpenGL Picture display and OpenCL post Effect.

Picture extension support :

pgm, bmp, pcx, jpg, jpe, jpeg, jfif, jif, jfi, tif,	gif, png, tga, pcd, webp, bpg, jp2, jpc, j2c, pgx, ppm, psd, pdd, mng, jng, iff, xpm, ico, cur, ani, svg, nef, crw, cr2, dng, arw, erf, 3fr, dcr, raw, x3f, mef, raf, mrw, pef, sr2, orf, heic, hdr, exr;

Video extension support :

mp4, dat, m4s, vob, mod, mpv2, mp2, m1v, mpe, mpg, mpeg, wtv, dvr-ms, m2ts, m2t, avi, wmv, asf, vm, mov, qt, vp8, vp9, webm, mkv, y4m

//**************************************************************************
//Installation
//**************************************************************************

Linux Mint 18 or Ubuntu 16.04 x64 Installation
- Please check if you have an opencl compatibility device :
Tape clinfo 
- if it is not working please consult this website to install a driver :
https://wiki.tiker.net/OpenCLHowTo
- Run deb package

Installation on Windows
- Run RegardsViewer2Setup.exe

Installation on Mac os X 10.10 and Later
- Open DMG file and Copy RegardsViewer to Application Folder.

//**************************************************************************
//Configuration
//**************************************************************************

Works on minimum windows 7 and above, Mac OS X 10.12 and above, Linux Mint 18 or Ubuntu 16.04 x64 and above. 
An OpenCL 1.2 compatible device is necessary to use this software.

//**************************************************************************
//Software Review
//**************************************************************************

SOFTPEDIA 3,5 / 5
FIND MY SOFT 3 / 5 

//**************************************************************************
//What's New
//**************************************************************************

News for version 2.18.1
- Bug Correction 
- Theme support -theme file save into Regards directory in documents folder
- Multi language support - Only English now but is ready for all language
- Compilation time improve by support of precompiled header
- Remove error log and debug information from wxWidgets
News for version 2.18
- Bug Correction 
- Performance Improvement 
- Support MAC OS Mojave
News for version 2.17
- Bug Correction 
- Add AV1 codec support
News for version 2.16.1
- Bug Correction 
- Dynamic Video Icon
News for version 2.16.0
- Bug Correction 
- Bug Correction on HEIC Format from Apple
- Multithreading decoding for HEIC File 
- Exif support for HEIC File 
News for version 2.15.3
- Bug Correction 
News for version 2.15.2
- Bug Correction for support of special character in linux folder 
- Correct Refresh crash bug in linux.
- Add Viewer mode - Hide all unnecessary window. 
- Better support of application integration into window, mac and linux. You can define RegardsViewer as default viewer.
News for version 2.15.1
- Bug Correction for Increase and Decrease Size
- Bug Correction for Folder and Criteria Menu Button
News for version 2.15.0
- On linux, move from gtk2 to gtk3
- On linux, support HiDPI
- Bug Correction
News for version 2.14.0
- Bug correction
- Improve multi-thread stability
- Add Process parameter into the menu Parameter / configuration
News for version 2.13.11
- Rewrite Video playback
- Rewrite OpenCL Device Manager
- Correct video performance on Mac OS on Fullscreen Mode
- Full Render Effect is done on OpenGL Shader for video
News for version 2.13.10
- Optimize Video playback
- Correct bug
- Add Bicubic Video Interpolation option
News for version 2.13.9
- Correct Video playback bug
News for version 2.13.8
- Correct crash and bug
News for version 2.13.7
- Correct crash and bug
- On MAC : compilation revert to llvm 4.0.1 because they are a compilation issue with llvm 5.0
- Update wxWidget to the lastest developpement version with bug correction for high sierra.
- Update source and libextern file 
News for version 2.13.6
- Correct crash with animation file
- On MAC : compilation update with llvm 5.0.0 and change dynamic lib symbolic link
- Support HDR file format in read only
- Support OpenEXR file format in read only
News for version 2.13.5
- Correct crash with animation file
News for version 2.13.4
- Update ffmpeg lib to 3.3.4
- Update dlib to 19.7
- Change diaporama to sequence mode
- Correct transition effect
News for version 2.13.3
- Correct PSD Crash
- Correct JPEG2000 Crash
- Picture Navigation Speed Enhance
- Add limitation for GPS infos to support geoplugin.net legal usage (120 geotag by minute)
News for version 2.13.2
- Correct GPS Info for Quicktime Iphone Movie
- Correct Thumbnail Refresh
News for version 2.13.1
- OpenCL Memory Available Test Correction
- HEIC Format Multithread Crash Correction
- Remove Rawspeed support
- Bug Correction
News for version 2.13.0
- Move all system compilation on codelite (gcc 7 for windows, llvm 4.0.1 MacOS, gcc 5.3 for linux mint)
- Bug Correction
- Video Color Correction for GPU not support OpenGL 4.5
- Crash Correction for OpenCL GPU that not support OpenGL Sharing
- Less cpu usage to optimize batery life
- Update Source Code
News for version 2.12.2
- Add Water Effect
- Add Index Generator on Thumbnail Right
- Correct Memory Bug
- Update Exiv library to 0.26 
- Support not avx processor
News for version 2.12.1
- Crash Correction for ANI animation and multipage TIFF file.
- Support the new HEIF Format (Heic extension)
- Multi Image GIF and TIFF support
- Bug Correction
News for version 2.11.1
- Add Thumbnail Size to improve Face Detection
- Bug Correction
News for version 2.11.0
- Add Face Recognition over thumbnail from photo and video with name label support
- Add Face Name Search
- Bug Correction
News for version 2.10.5
- Bug crash correction on corrupted video.
- Bug correction on video volume.
News for version 2.10.4
- Installation Update for Ubuntu and Debian System
- Bug correction on video player.
News for version 2.10.3 only for windows
- Bug correction on video presentation.
News
- Update Version for ubuntu and Code Source
News for version 2.10.2
- Add lib Media Info for video information.
- Update SQLite to 3.17.0
- Update LibRaw to 0.18.1
- Correct bug on jpeg decompression.
News for version 2.10.1
- Add Multi-Audio support for movie.
- Add Subtitle support for movie.
- To choose audio and subtitle click on effect window.
- Change default sort by file path.
News for version 2.10.0
- Add WEBP and BPG format support. 
- Add WEBM, MKV, Y4M format support. 
- Upgrade to FFMPEG 3.3 
News for version 2.9.2
- Video Memory Leak
- Thumbnail Memory Leak
- Video loading faster
- Add Folder select in criteria
For All version
- Raw developpement bug correction
- OpenGL render for Picture and Video
- Use TurboJpeg for jpeg file
- Use LibRaw for raw picture
- Bug Correction
- Add new filter : Raw Development Filter

Copyright
Figuinha Jacques - 2019 - Email Me For More information

All - Version

