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

Linux Mint 20 or Ubuntu 20.04 x64 Installation
- Please check if you have an opencl compatibility device :
Tape clinfo 
- if it is not working please consult this website to install a driver :
https://wiki.tiker.net/OpenCLHowTo
- Run deb package

Installation on Windows
- Run RegardsViewer2Setup.exe

Installation on Mac os X 10.13 and Later
- Open DMG file and Copy RegardsViewer to Application Folder.

//**************************************************************************
//Configuration
//**************************************************************************

Works on minimum windows 7 and above, Mac OS X 10.13 and above, Ubuntu 20.04 x64, Linux Manjaro and above. 
An OpenCL 1.2 compatible device is necessary to use this software.

//**************************************************************************
//Software Review
//**************************************************************************

SOFTPEDIA 3,5 / 5
FIND MY SOFT 3 / 5 
HOTPICKS LINUX FORMAT JUNE 2020

//**************************************************************************
//What's New
//**************************************************************************
<b>News for 2.80.0 :</b><br>
- Correct bug on avif format
- Update icon management
- Correct bug on video display orientation
- Correct crash if picture has a wrong date
- Correct bug and add some optimization
- Correct bug on filter preview
- Correct bug on video thumbnail
- Correct crash on some video
- Correct video metadata information
- Update opengl Texture management
- Update MediaInfo File access
- Some minor update for performance boost
- Correct bug on search criteria
- Update RegardsConverter program with new encoder option
- Linux : Add support for qsv and amf for video encoding
- Add Preset option for video encoding
- Update SVT-AV1 encoder to 1.8.0 (x2 faster)
<b>News for 2.79.0 :</b><br>
- Export video bug is corrected
- Correct some bugs on gps geolocalization
- Correct bug on update criteria
- Correct error on thumbnail effect
- Correct bug and add some optimizations
- Correct bug thumbnail picture generator
- Correct bug on cpu usage on video decoding
- Work on memory bug with opencv capture on linux
- Add Expand and contract for each separation bar for explorer mode
<b>News for 2.78.0 :</b><br>
- Correct bug and some optimization
- Correct bug on memory usage
- Correct bug on animation and video thumbnail
- Correct crash on linux
- Correct theme error
- Correct bug on video thumbnail rotation
- Correct crash on mac os with incorrect datetime on photo
- Correct bug with gps management
- Correct bug on diaporama and transition effect
- Correct bug on video thumbnail management
- Optimize access to video
- Stop Apple Intel Version
- Stop Linux Ubuntu 20
<b>News for 2.77.0 :</b><br>
- Support dark, light and auto mode
- Add support for AV1 encoding
- External application for video export
- External application for PDF and Scan
- Correct bug :
- Fullscreen mac os
- Correct bug on opengl opencl interop
- Folder focus correct mac os
- Crash on select or deselect folder update
- picture display correctly when you past an image path on command line
- thumbnail generation for video
- thumbnail generator stop
- webp animation
- exr crash
- Correct crash bug when you start the app with a file that doesn't exist.
- Correct zoom moving on video
- New version detection doesn't block app
- Correct bug on scan application
- Correct crash on export video
- Correct crash on video export when you choose a hardware encoder
<b>News for 2.76.0 :</b><br>
Bug Correction :
- Correct bug on effect thumbnail
- Correct screen tab bug show on fullscreen
- Correct bug on manage event when you watch a movie
- Correct bug on video display on opencl compatible device
- Correct a crash when video is played a long time on linux
- Correct bug with face thumbnail invert
- Correct crash with update thumbnail
- Correct font display
- Correct crash on Explorer Mode
- Correct crash on Unvalid Raw File
Optimisation :
- Optimisation for large amount of pictures
- Optimisation for large picture show
- Update lib version
- Update Face List management
- Improve video fluidity
- Add Create Face Name option
<b>News for 2.63.5 :</b><br>
- Add video encoder tool. Support mkv and mp4.<br>
- Update ffmpeg lib with the latest version<br>
- Correct memory bug on video decoder software.<br>
<b>News for 2.63.4 :</b><br>
- Correct bug on display bleeding<br>
- Correct rotation video bug on software decompression<br>
- Add new button for linking external program for video and picture edition<br>
<b>News for 2.63.3 :</b><br>
- Correct bug on video playback with dxva2<br>
- Correct bug with diaporama after video playback<br>
- Correct bug on Vertical Flip which call Save<br>
- Correct crash when you keep push down space bar after video thumbnail<br>
<b>News for 2.63.2 :</b><br>
- Correct bug on scrollbar disapear on Thumbnail video and Thumbnail effect<br>
- Add support for opengl interpolation for no opencl devices<br>
- Correct movie crash play on qemu linux distrib<br>
<b>News for 2.63.1 :</b><br>
- Correct bug on window display<br>
<b>News for 2.63.0 :</b><br>
Correct bug : <br>
- Correct background process Face, Thumbnail and Criteria<br>
- Correct event loop scrolling in thumbnail paint event<br>
- Correct Thumbnail Face orientation on Face mode<br>
- Correct bug on video ratio management<br>
- Correct several bug on window display<br>
Modification : <br>
- Modification on criteria management. Add Red cross for deleting<br>
<b>News for 2.62.9 :</b><br>
- Bug correction :<br>
- On linux, correct slider visibility for picture compression<br>
- Correct bug on showing dialog box for delete keyword<br>
- Correct bug on delete criteria<br>
- Correct bug on saving picture file<br>
- Update dialog box display<br>
<b>News for 2.62.8 :</b><br>
- New function : <br>
- Add zoom and rotation for video<br>
- Add saving option for video picture when the video is paused<br>
- Bug Correction : <br>
- Correct bug on image lib for saving and loading<br>
- Correct bug on playing video on mac os<br>
- Correct bug on stop video<br>
<b>News for 2.62.7 :</b><br>
- Correct audio crash on playing video<br>
<b>News for 2.62.6 :</b><br>
- Correct memory bug<br>
- Faster movie play<br>
<b>News for 2.62.5 :</b><br>
- Correct freeze bug on playing video<br>
News for 2.62.4 :
- Add new transition effect : none, fading, move, page curl.
- Add auto contrast correction on picture display.
- All this option is available on Regards configuration dialog.
- Correct DXVA2 video playback on windows.
News for 2.62.3 :
- Correct bug on window position
News for 2.62.1 :
- Correct bug on window position
- Prevent crash bug with update notification on linux
News for 2.62.0 :
- New denoise filter for video playback
- Add right panel in fullscreen mode for filter effect
News for 2.61.9 :
- Add Interpolation Filter selection for picture display
- libfreeimage integration to prevent crash
News for 2.61.8 :
- Correct crash bug with libRaw on mac os
News for 2.61.7 :
- Update libRaw to 0.20
- Update Sqlite to 3.30
- Show preview thumbnail on display
- Correct bug on display video at startup
- Correct bug on raw display
News for 2.61.6 :
- Saving last file view
- Correct bug on showing star in criteria
News for 2.61.5 :
- Correct bug on star<br>
- Panel fix on Infos or Criteria<br>
News for 2.61.4 :
- Add support for picture and video notation
- Add support for picture and video keyword
News for 2.61.3 :
- Correct bug on invalid picture
- Correct crash bug on category date detection
News for 2.61.2 :
- Update exiv2 to 0.27.3
- Add avif saving
- Correct left and right panel position if the position is incorrect
- Optimize picture navigation on thumbnail
News for 2.61.1 :
- Correct bug with dialog box on Windows
- Correct memory leak with HEIC format
- Correct crash with thumbnail on linux
- Add Support avif format read only
<b>News for 2.61.0 :
- Correct scan crash on windows
- Correct RegardsViewer option dialog crash
News for 2.60.9 :
- Correct crash bug on pdf incorrect file
- Add saving window position
- Regards start on Pictures Folder as Default folder in first time
- Correct bug on slider element
- Correct crash bug on scan option in windows
News for 2.60.8 :
- Correct bug on PDF saving.
- Correct bug on Scan PDF Save in color mode in Linux
News for 2.60.7 :
- Correct bug in map show
- Correct bug on scan button in scan module
- Correct Scanner functionnality for linux
News for 2.60.6 :
- Correct scan crash on linux
- Add Web browser in criteria
- Update button save management in bitmap toolbar
- Update for mouse wheel management in thumbnail. Mouse Wheel and CTRL for navigate in thumbnail
- Correct bug with Infos and File toolbar button
News for 2.60.5 :
- Correct bug with FreeImage on format recognition crash
- Correct display button on Diaporama Mode
News for 2.60.4 :
- Correct bug with opengl graphic card without support opencl context
- Correct bug with heic memory leak
- Correct bug with window move
- Correct bug with print button which has disappeared
News for 2.60.3 :
- Bug correction
News for 2.60.2 :
- Correct crash in linux when change window mode
- Correct bug on detectRotation selection in configuration window
- Delete entry unuse on menu
News for 2.60.1 :
- Screen Refresh Update
- Correct Criteria Delete
- Correct movie player bug
- Correct crash bug on Face Detection
- Correct installation package for mac os and Debian
- Bug correction
News for 2.60 :
- Opencv integration for face detection
- Picture Orientation Detection base on Face Detection in picture
- Rewrite Red Eye filter with eye detection and Face detection
- Bug correction
News for version 2.55
- Correct display mode corruption
News for version 2.54
- Correct direct display picture from explorer
- Add Association program for windows
News for version 2.53
Correct IHM bug
- Espace bar is disable when you check a folder to begin picture navigation
- Display picture when you stay push on espace bar
News for version 2.52
- Update library link
News for version 2.51
Under Scan Tools :
- Add PDF creation with multifile picture
- Add PDF compression choice with tiff or jpeg option

News for version 2.50
- Correct bug on default window position
- Add opengl preview render on filter effect

News for version 2.49
- Correct bug on window refresh

News for version 2.48
- Correct window visibility when you switch from picture mode and viewer mode
- Correct UTF8 crash on file name

News for version 2.47
- Correct animation thumbnail bug
- Correct crash on scan window

News for version 2.46
- Correct display bug on mac os
- Auto select opencl device on startup

News for version 2.45
- Correct crash on WaveFilter mac os
- Correct display correction mac os (HiDPI)

News for version 2.44
- Correct crash bug on thumbnail and video

News for version 2.43
- Correct bug on thumbnail

News for version 2.42
- Update to ffmpeg 4.2.2
- Correct video stop
- Correct display if video is the first element

News for version 2.41
- Correct bug on add folder
- Video engine rewrite

News for version 2.40
- new Window Manager
- Bug correction

News for version 2.35
- new Picture mode
- Add Tbb library for image optimization
- Add pfm format
- Bug Correction

News for version 2.34
- Bug correction
- Delete link to opengl lib on linux to prevent bug on Amd Graphic Card
- Correct bug with opencl on CPU that prevent crash

News for version 2.33
- Bug correction on scan application
- Add Categorie Usenet detection

News for version 2.32
- Add Scan button into viewer mode
- Bug Correction

News for version 2.31
- Add new saving format support
- Add support black mode for osx
- Bug Correction
News for version 2.30.1
- Add Scanner Application for mac os
- Correct bug on save picture
- Correct bug on refresh folder
- Add progressbox for folder selection
News for version 2.30
- Add PDF Support
- Add Scanner Application from Menu File-> Scanner
- Add new file support
- Add FreeImage Lib
- Rewrite Filter management support
- Add Multipage print
- Remove Face Recognition on the main program
- Add Face Detection into a dialog box
- Bug correction
News for version 2.20
- Bug Correction
- Add Folder Watcher
- Add Version Checker
- Use Thunderbird to send email in linux
News for version 2.19.1
- Bug Correction
- Mac OS Compilation for High Sierra for the moment
News for version 2.19.0
- New User Interface
- Cleaning Code source
- Bug Correction
- Mac OS Compilation for High Sierra for the moment
News for version 2.18.4
- Compatible with MacOS 10.10 or Superior 
- Bug Correction 
- New default Theme - you need to delete Regards.viewer.theme on Documents/Regards folder
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
Figuinha Jacques - 2020 - Email Me For More information

All - Version

