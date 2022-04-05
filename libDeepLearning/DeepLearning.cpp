#include "header.h"
#include "DeepLearning.h"
#include "FaceDetector.h"
#include "DetectRotation.h"
#include <ParamInit.h>
#include <RegardsConfigParam.h>
#include <ViewerParamInit.h>
using namespace Regards::DeepLearning;
using namespace Regards::OpenCV;
using namespace Regards::Viewer;
#undef AUTO_ROTATE

static bool isload = false;
static std::mutex muLoading;

void CDeepLearning::CleanRecognition()
{
	CFaceDetector::CleanBase();
}

vector<int> CDeepLearning::FindFace(CRegardsBitmap* pictureData)
{
	bool fastDetection = true;
	muLoading.lock();
	const bool isLoading = isload;
	muLoading.unlock();

	CRegardsConfigParam* param = CParamInit::getInstance();
	if (param != nullptr)
		fastDetection = param->GetFastDetectionFace();

	if (isLoading)
	{
		CFaceDetector faceDetector(fastDetection);
		return faceDetector.FindFace(pictureData);
	}
	vector<int> list;
	return list;
}


void CDeepLearning::LoadRessource()
{
	//CDetectRotation::LoadModel(rotation_json);
	CFaceDetector::LoadModel();
	muLoading.lock();
	isload = true;
	muLoading.unlock();
}


void CDeepLearning::DetectEyes(cv::Mat& pBitmap)
{
	bool fastDetection = true;
	std::vector<wxRect> listEye;
	muLoading.lock();
	const bool isLoading = isload;
	muLoading.unlock();

	CRegardsConfigParam* param = CParamInit::getInstance();
	if (param != nullptr)
		fastDetection = param->GetFastDetectionFace();

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

int CDeepLearning::GetExifOrientation(CRegardsBitmap* pBitmap)
{
	bool fastDetection = true;
	muLoading.lock();
	const bool isLoading = isload;
	muLoading.unlock();

	CRegardsConfigParam* param = CParamInit::getInstance();
	if (param != nullptr)
		fastDetection = param->GetFastDetectionFace();

	if (isLoading)
	{
		CDetectRotation detectRotation;
		return detectRotation.GetExifOrientation(pBitmap, fastDetection);
	}
	return 0;
}


bool CDeepLearning::FindFaceCompatible(const int& numFace)
{
	bool returnValue = false;
	bool fastDetection = true;
	muLoading.lock();
	const bool isLoading = isload;
	muLoading.unlock();

	CRegardsConfigParam* param = CParamInit::getInstance();
	if (param != nullptr)
		fastDetection = param->GetFastDetectionFace();

	if (isLoading)
	{
		CFaceDetector faceDetector(fastDetection);
		returnValue = faceDetector.FaceRecognition(numFace);
	}

	return returnValue;
}
