#include "header.h"
#include "DeepLearning.h"
#include "FaceDetector.h"
#include "DetectRotation.h"
#include "FaceRecognition.h"
using namespace Regards::DeepLearning;
using namespace Regards::OpenCV;
#undef AUTO_ROTATE

int CDeepLearning::GetExifOrientation(CPictureData * pictureData)
{
	CDetectRotation detectRotation;
	return detectRotation.GetExifOrientation(pictureData);
}


int CDeepLearning::GetAngleOrientation(CPictureData * pictureData)
{

	CDetectRotation detectRotation;
	return detectRotation.GetAngleOrientation(pictureData);
}

bool CDeepLearning::FindFaceCompatible(const int &numFace)
{
	CFaceRecognition faceRecognition;
	return faceRecognition.FindCompatibility(numFace);
}