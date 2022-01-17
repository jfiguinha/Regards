#pragma once
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#define wxUSE_PDF
#define TURBOJPEG
#define EXIV2
#define FFMPEG
#define USECURL
#define GLUT
//#define WX_PRECOMP

#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/ocl.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <mutex>
#include <thread>
#include <ImageLoadingFormat.h>
#include <PictureData.h>
using namespace std;
