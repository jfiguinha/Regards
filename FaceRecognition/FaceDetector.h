#pragma once
#include "FaceData.h"

class CFaceDetector
{
public:
	CFaceDetector();
	~CFaceDetector();
	int LoadPicture(uint8_t * data, const int &width, const int &height, const int &size, const int &quality, CFaceData * faceData);
	int LoadPicture(const char * filename, const int &quality, CFaceData * faceData);
	int LoadPictureFromJpegBuffer(const unsigned char * data, const int &size, const int &quality, CFaceData * faceData);
	int GetNbFaceFound();
	void GetFaceData(const int &numFace, long &width, long &height, double &pertinence, uint8_t * data, long &size);
	void SaveFaceData(const int &numFace, long &width, long &height, double &pertinence, const char * savePath);
	std::string GetFaceDescriptor(const int &numFace);
	float IsCompatibleFace(std::string const& dataface1, std::string const& dataface2);
private:
	int DetectFace(array2d<rgb_pixel> & img, const int &quality, CFaceData * faceData);
	std::vector<double> face_pertinence;
	std::vector<matrix<rgb_pixel>> faces;
	std::vector<matrix<float, 0, 1>> face_descriptors;
};

