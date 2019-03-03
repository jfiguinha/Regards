#include "FaceData.h"


CFaceData::CFaceData()
{
	//detector = get_frontal_face_detector();
}

CFaceData::~CFaceData()
{

}

int CFaceData::LoadPredictor(const std::string & file_name)
{
	// We will also use a face landmarking model to align faces to a standard pose:  (see face_landmark_detection_ex.cpp for an introduction)
	deserialize(file_name) >> sp;
	return 0;
}

int CFaceData::LoadDescriptor(const std::string & file_name)
{
	// We will also use a face landmarking model to align faces to a standard pose:  (see face_landmark_detection_ex.cpp for an introduction)
	deserialize(file_name) >> net;
	return 0;
}
