#pragma once
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#ifndef __REGARDS__
#define __REGARDS__
#endif

#ifndef wxUSE_PDF
#define wxUSE_PDF
#endif

#ifndef TURBOJPEG
#define TURBOJPEG
#endif

#ifndef EXIV2
#define EXIV2
#endif

#ifndef FFMPEG
#define FFMPEG
#endif

#ifndef USECURL
#define USECURL
#endif

#ifndef GLUT
#define GLUT
#endif

#ifndef USE_TBB
#define USE_TBB
#endif

#ifndef OPENCV_OPENCL
#define OPENCV_OPENCL
#endif


//#define WX_PRECOMP
#if defined(__WXMSW__) && defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#ifdef USE_TBB
#include <tbb/concurrent_vector.h>
#include <tbb/parallel_for.h>
#endif

#include <thread>
#include <mutex>
#include <algorithm>
#include <stdint.h>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cfloat>
#include <cmath>
#include <stdexcept>
#include <cassert>
#include <cerrno>
#include <sys/stat.h>
#include <wx/version.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/ocl.hpp>
#include <opencv2/core/mat.hpp>
#include <wx/mstream.h>
using namespace std;

/*  check if the current version is at least major.minor.release */
#define wxCHECK_VERSION(major,minor,release) \
    (wxMAJOR_VERSION > (major) || \
    (wxMAJOR_VERSION == (major) && wxMINOR_VERSION > (minor)) || \
    (wxMAJOR_VERSION == (major) && wxMINOR_VERSION == (minor) && wxRELEASE_NUMBER >= (release)))

#define CL_TARGET_OPENCL_VERSION 120
