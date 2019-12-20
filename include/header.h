#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#define wxUSE_PDF
#define TURBOJPEG
#define EXIV2
#define FFMPEG
#define USECURL
#define RENDEROPENGL
//#define WX_PRECOMP

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
using namespace std;

/*  check if the current version is at least major.minor.release */
#define wxCHECK_VERSION(major,minor,release) \
    (wxMAJOR_VERSION > (major) || \
    (wxMAJOR_VERSION == (major) && wxMINOR_VERSION > (minor)) || \
    (wxMAJOR_VERSION == (major) && wxMINOR_VERSION == (minor) && wxRELEASE_NUMBER >= (release)))

#define CL_TARGET_OPENCL_VERSION 120
