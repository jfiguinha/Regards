Common Setup
============

You may need to disable antivirus software to run qpdf's test suite. Running Windows Defender on Windows 10 does not interfere with building or running qpdf or its test suite.

To be able to build qpdf and run its test suite, you must have MSYS2 installed. This replaces the old process of having a mixture of msys, mingw-w64, and ActiveState perl. It is now possible to do everything with just MSYS2.

Here's what I did on my system:

* Download msys2 (64-bit) from msys2.org
* Run the installer.
* Run msys2_shell.cmd by allowing the installer to start it.
* From the prompt:
  * Run `pacman -Syuu` and follow the instructions, which may tell you
    to close the window and rerun the command multiple times.
  * `pacman -S make base-devel git zip unzip`
  * `pacman -S mingw-w64-x86_64-toolchain mingw-w64-i686-toolchain`

If you would like to build with Microsoft Visual C++, install a suitable Microsoft Visual Studio edition. In early 2016, 2015 community edition with C++ support is fine. It may crash a few times during installation, but repeating the installation will allow it to finish, and the resulting software is stable.

To build qpdf with Visual Studio, start the msys2 mingw32 or mingw64 shell from a command window started from one of the Visual Studio shell windows. You must use the mingw shell for the same word size (32 or 64 bit) as the Windows compiler since the MSVC build uses objdump from the msys distribution. You must also have it inherit the path. For example:

* Start x64 native tools command prompt from msvc
* `set MSYS2_PATH_TYPE=inherit`
* `C:\msys64\mingw64`

Image comparison tests are disabled by default, but it is possible to run them on Windows. To do so, add `--enable-test-compare-images` from the configure statements given below and install some additional third-party dependencies. These may be provided in an environment such as MSYS or Cygwin or can be downloaded separately for other environments. You may extract or install the following software into separate folders each and add the `bin` folder to your `PATH` environment variable to make executables and DLLs available. If installers are provided, they might do that already by default.

* [LibJpeg](http://gnuwin32.sourceforge.net/packages/jpeg.htm): This archive provides some needed DLLs needed by LibTiff.
* [LibTiff](http://gnuwin32.sourceforge.net/packages/tiff.htm): This archive provides some needed binaries and DLLs if you want to use the image comparison tests. It depends on some DLLs from LibJpeg.
* [GhostScript](http://www.ghostscript.com/download/gsdnld.html): GhostScript is needed for image comparison tests. It's important that the binary is available as `gs`, while its default name is `gswin32[c].exe`. You can either copy one of the original files, use `mklink` to create a hard/softlink, or provide a custom `gs.cmd` wrapper that forwards all arguments to one of the original binaries. Using `mklink` with `gswin32c.exe` is probably the best choice.

# External Libraries

In order to build qpdf, you must have a copy of `zlib` and the `jpeg` library. The easy way to get it is to download the external libs from the qpdf download area. There are packages called `external-libs-bin.zip` and `external-libs-src.zip`. If you are building with MSVC 2015 or MINGW with MSYS2, you can just extract the `qpdf-external-libs-bin.zip` zip file into the top-level qpdf source tree. Note that you need the 2017-08-21 version (at least) to build qpdf 7.0 or greater since this includes jpeg. Passing `--enable-external-libs` to `./configure` (which is done automatically if you follow the instructions below) is sufficient to find them.

You can also obtain `zlib` and `jpeg` directly on your own and install them. If you are using mingw, you can just set `CPPFLAGS`, `LDFLAGS`, and `LIBS` when you run ./configure so that it can find the header files and libraries. If you are building with MSVC and you want to do this, it probably won't work because `./configure` doesn't know how to interpret `LDFLAGS` and `LIBS` properly for MSVC (though qpdf's own build system does). In this case, you can probably get away with cheating by passing `--enable-external-libs` to `./configure` and then just editing `CPPFLAGS`, `LDFLAGS`, `LIBS` in the generated autoconf.mk file. Note that you should use UNIX-like syntax (`-I`, `-L`, `-l`) even though this is not what cl takes on the command line. qpdf's build rules will fix it.

You can also download `qpdf-external-libs-src.zip` and follow the instructions in the README.txt there for how to build external libs.

# Building from version control

If you check out qpdf from version control, documentation files will not be generated or installed. Documentation files are included in the source distribution, but they can only be built from Linux.

# Building with MinGW

QPDF is known to build and pass its test suite with MSYS2 using the 32-bit and 64-bit compilers. MSYS2 is required to build as well in order to get make and other related tools. See common setup at the top of this file for installation and configuration of MSYS2. Then, from the suitable 32-bit or 64-bit environment, run

```
./config-mingw
make
```

Note that `./config-mingw` just runs `./configure` with specific arguments, so you can look at it, make adjustments, and manually run configure instead.

Add the absolute path to the `libqpdf/build` directory to your `PATH`. Make sure you can run the qpdf command by typing qpdf/build/qpdf and making sure you get a help message rather than an error loading the DLL or no output at all.  Run the test suite by typing

```
make check
```

If all goes well, you should get a passing test suite.

To create an installation directory, run `make install`.  This will create `install-mingw/qpdf-VERSION` and populate it.  The binary download of qpdf for Windows with mingw is created from this directory.

You can also take a look at `make_windows_releases` for reference.  This is how the distributed Windows executables are created.

# Building with MSVC 2015

These instructions would likely work with newer versions of MSVC and are known to have worked with versions as old as 2008 Express.

You should first set up your environment to be able to run MSVC from the command line.  There is usually a batch file included with MSVC that does this.  Make sure that you start a command line environment configured for whichever of 32-bit or 64-bit output that you intend to build for.

From that cmd prompt, you can start your MSYS2 shell with path inheritance as described above.

Configure and build as follows:

```
./config-msvc
make
```

Note that `./config-msvc` just runs `./configure` with specific arguments, so you can look at it, make adjustments, and manually run configure instead.

NOTE: automated dependencies are not generated with the msvc build. If you're planning on making modifications, you should probably work with mingw.  If there is a need, I can add dependency information to the msvc build, but since I only use it for generating release versions, I haven't bothered.

Once built, add the full path to the `libqpdf/build` directory to your path and run

```
make check
```

to run the test suite.

If you are building with MSVC and want to debug a crash in MSVC's debugger, first start an instance of Visual C++.  Then run qpdf.  When the abort/retry/ignore dialog pops up, first attach the process from within visual C++, and then click Retry in qpdf.

A release version of qpdf is built by default.  If you want to link against debugging libraries, you will have to change `/MD` to `/MDd` in `make/msvc.mk`.  Note that you must redistribute the Microsoft runtime DLLs.  Linking with static runtime (`/MT`) won't work; see "Static Runtime" below for details.

# Runtime DLLs

Both build methods create executables and DLLs that are dependent on the compiler's runtime DLLs.  When you run make install, the installation process will automatically detect the DLLs and copy them into the installation bin directory.  Look at the `copy_dlls` script for details on how this is accomplished.

Redistribution of the runtime DLL is unavoidable as of this writing; see "Static Runtime" below for details.

# Static Runtime

Building the DLL and executables with static runtime does not work with either Visual C++ .NET 2008 (a.k.a. vc9) using `/MT` or with mingw (at least as of 4.4.0) using `-static-libgcc`.  The reason is that, in both cases, there is static data involved with exception handling, and when the runtime is linked in statically, exceptions cannot be thrown across the DLL to EXE boundary.  Since qpdf uses exception handling extensively for error handling, we have no choice but to redistribute the C++ runtime DLLs.  Maybe this will be addressed in a future version of the compilers.  This has not been retested with the toolchain versions used to create qpdf >= 3.0 distributions. This has not been revisited since MSVC 2008, but redistributing runtime DLLs is extremely common and should not be a problem.
