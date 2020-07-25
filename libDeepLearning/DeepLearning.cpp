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

vector<int> CDeepLearning::FindFace(CPictureData * pictureData)
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

void CDeepLearning::LoadRessource(const string &config_file, const string &weight_file, const string &face_recognition, const string &rotation_json)
{
	CDetectRotation::LoadModel(rotation_json);
	CFaceDetector::LoadModel(config_file, weight_file, face_recognition);
	muLoading.lock();
	isload = true;
	muLoading.unlock();
}

bool CDeepLearning::IsResourceReady()
{
	bool isLoading = false;
	muLoading.lock();
	isLoading = isload;
	muLoading.unlock();
	return isLoading;
}

int CDeepLearning::GetExifOrientation(CImageLoadingFormat * imageLoadingFormat)
{
	bool isLoading = false;
	muLoading.lock();
	isLoading = isload;
	muLoading.unlock();

	if (isLoading)
	{
		CDetectRotation detectRotation;
		return detectRotation.GetExifOrientation(imageLoadingFormat);
	}
	return 0;

}

int CDeepLearning::GetAngleOrientation(CImageLoadingFormat * imageLoadingFormat)
{
	bool isLoading = false;
	muLoading.lock();
	isLoading = isload;
	muLoading.unlock();

	if (isLoading)
	{
		CDetectRotation detectRotation;
		return detectRotation.GetAngleOrientation(imageLoadingFormat);
	}
	return 0;
}

bool CDeepLearning::FindFaceCompatible(const int &numFace)
{
	bool isLoading = false;
	muLoading.lock();
	isLoading = isload;
	muLoading.unlock();

	if (isLoading)
	{
		CFaceRecognition faceRecognition;
		return faceRecognition.FindCompatibility(numFace);
	}
	return false;
}