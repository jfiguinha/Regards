#include "header.h"
#include "DeepLearning.h"
#include "FaceDetector.h"
#include "DetectRotation.h"
#include "FaceRecognition.h"
using namespace Regards::DeepLearning;
using namespace Regards::OpenCV;
#undef AUTO_ROTATE

static std::thread loadRot;
static std::thread loadModel;
static bool isload = false;
static std::mutex muLoading;


    
vector<int> CDeepLearning::FindFace(CRegardsBitmap * pictureData)
{
	bool isLoading = false;
	muLoading.lock();
	isLoading = isload;
	muLoading.unlock();

	if (isLoading)
	{
		CFaceDetector faceDetector;
		return faceDetector.FindFace(pictureData);
	}
	else
	{
		vector<int> list;
		return list;
	}
}


void CDeepLearning::LoadRessource(const string &config_file, const string &weight_file, const string &face_recognition, const string &rotation_json, const string &eye_detection, const string& landmarkPath)
{
	//CDetectRotation::LoadModel(rotation_json);
	CFaceDetector::LoadModel(config_file, weight_file, face_recognition, eye_detection, landmarkPath);
	muLoading.lock();
	isload = true;
	muLoading.unlock();
}


void CDeepLearning::DetectEyes(CRegardsBitmap * pBitmap)
{
	std::vector<wxRect> listEye;
	bool isLoading = false;
	muLoading.lock();
	isLoading = isload;
	muLoading.unlock();

	if (isLoading)
	{
		CFaceDetector faceDetector;
		faceDetector.DetectEyes(pBitmap);
	}
}

bool CDeepLearning::IsResourceReady()
{
	bool isLoading = false;
	muLoading.lock();
	isLoading = isload;
	muLoading.unlock();
	return isLoading;
}

int CDeepLearning::GetExifOrientation(CRegardsBitmap* pBitmap)
{
	bool isLoading = false;
	muLoading.lock();
	isLoading = isload;
	muLoading.unlock();

	if (isLoading)
	{
		CDetectRotation detectRotation;
		return detectRotation.GetExifOrientation(pBitmap);
	}
	return 0;

}





bool CDeepLearning::FindFaceCompatible(const int &numFace)
{
    CFaceRecognition faceRecognition;
    return faceRecognition.FindCompatibility(numFace);
}