#pragma once
#include "FaceData.h"

class CFaceDetector
{
public:
	CFaceDetector();
	~CFaceDetector();
	int GetNbFaceFound();
	void GetFaceData(const int &numFace, long &width, long &height, double &pertinence, uint8_t * data, long &size);
	void SaveFaceData(const int &numFace, long &width, long &height, double &pertinence, const char * savePath);
	std::string GetFaceDescriptor(const int &numFace);
	float IsCompatibleFace(std::string const& dataface1, std::string const& dataface2);
	int DetectFace(array2d<rgb_pixel> & img, const int &quality, CFaceData * faceData);
private:
	
	std::vector<double> face_pertinence;
	std::vector<matrix<rgb_pixel>> faces;
	std::vector<matrix<float, 0, 1>> face_descriptors;
};

