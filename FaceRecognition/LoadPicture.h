#pragma once
#include "FaceData.h"

// This class is exported from the FaceRecognition.dll
class CLoadPicture {
public:
	static array2d<rgb_pixel> LoadPicture(uint8_t * data, const int &width, const int &height);
	static array2d<rgb_pixel> LoadPictureFromJpegBuffer(const unsigned char * data, const int &size);
	static array2d<rgb_pixel> LoadPicture(const char * filename);
};