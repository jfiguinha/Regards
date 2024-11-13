#include "header.h"
#include "DeepLearning.h"
#include "FaceDetector.h"
#include "DetectRotation.h"
using namespace Regards::DeepLearning;
using namespace Regards::OpenCV;
#undef AUTO_ROTATE

static bool isload = false;
static std::mutex muLoading;

void CDeepLearning::CleanRecognition()
{
	CFaceDetector::CleanBase();
}

vector<int> CDeepLearning::FindFace(const cv::Mat& pictureData, const wxString& filename, const bool& fastDetection)
{
	//bool fastDetection = true;
	muLoading.lock();
	const bool isLoading = isload;
	muLoading.unlock();
	/*
	CRegardsConfigParam* param = CParamInit::getInstance();
	if (param != nullptr)
		fastDetection = param->GetFastDetectionFace();
*/
	if (isLoading)
	{
		CFaceDetector faceDetector(fastDetection);
		return faceDetector.FindFace(pictureData, filename);
	}
	vector<int> list;
	return list;
}


void CDeepLearning::LoadRessource(const bool& openCLCompatible, const bool& cudaCompatible)
{
	//CDetectRotation::LoadModel(rotation_json);
	CFaceDetector::LoadModel(openCLCompatible, cudaCompatible);
	muLoading.lock();
	isload = true;
	muLoading.unlock();
}


void CDeepLearning::DetectEyes(const cv::Mat& pBitmap, const bool& fastDetection)
{
	std::vector<wxRect> listEye;
	muLoading.lock();
	const bool isLoading = isload;
	muLoading.unlock();


	if (isLoading)
	{
		CFaceDetector faceDetector(fastDetection);
		faceDetector.DetectEyes(pBitmap);
	}
}

bool CDeepLearning::IsResourceReady()
{
	muLoading.lock();
	const bool isLoading = isload;
	muLoading.unlock();
	return isLoading;
}

int CDeepLearning::GetExifOrientation(const cv::Mat& pBitmap, const bool& fastDetection)
{
	muLoading.lock();
	const bool isLoading = isload;
	muLoading.unlock();


	if (isLoading)
	{
		CDetectRotation detectRotation;
		return detectRotation.GetExifOrientation(pBitmap, fastDetection);
	}
	return 0;
}


bool CDeepLearning::FindFaceCompatible(const int& numFace, const bool& fastDetection)
{
	bool returnValue = false;

	muLoading.lock();
	const bool isLoading = isload;
	muLoading.unlock();

	if (isLoading)
	{
		CFaceDetector faceDetector(fastDetection);
		returnValue = faceDetector.FaceRecognition(numFace);
	}

	return returnValue;
}
