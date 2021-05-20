#include "header.h"
#include "DeepLearning.h"
#include "FaceDetector.h"
#include "DetectRotation.h"
#include <ConfigParam.h>
#include <ParamInit.h>
#include <RegardsConfigParam.h>
#include <ViewerParamInit.h>
using namespace Regards::DeepLearning;
using namespace Regards::OpenCV;
using namespace Regards::Viewer;
#undef AUTO_ROTATE

static bool isload = false;
static std::mutex muLoading;

bool CDeepLearning::LockOpenCLDnn()
{
	return CFaceDetector::LockOpenCLDnn();
}

bool CDeepLearning::UnlockOpenCLDnn()
{
	return CFaceDetector::UnlockOpenCLDnn();
}

vector<int> CDeepLearning::FindFace(CRegardsBitmap * pictureData)
{
	bool fastDetection = true;
	bool isLoading = false;
	muLoading.lock();
	isLoading = isload;
	muLoading.unlock();

	CRegardsConfigParam* param = CParamInit::getInstance();
	if (param != nullptr)
		fastDetection = param->GetFastDetectionFace();

	if (isLoading)
	{
		CFaceDetector faceDetector(fastDetection);
		return faceDetector.FindFace(pictureData);
	}
	else
	{
		vector<int> list;
		return list;
	}
}


void CDeepLearning::LoadRessource(const string& config_file, const string& weight_file, const string& recognition, const string& face_landmark)
{
	//CDetectRotation::LoadModel(rotation_json);
	CFaceDetector::LoadModel(config_file, weight_file, recognition, face_landmark);
	muLoading.lock();
	isload = true;
	muLoading.unlock();
}


void CDeepLearning::DetectEyes(CRegardsBitmap * pBitmap)
{
	bool fastDetection = true;
	std::vector<wxRect> listEye;
	bool isLoading = false;
	muLoading.lock();
	isLoading = isload;
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
	bool isLoading = false;
	muLoading.lock();
	isLoading = isload;
	muLoading.unlock();
	return isLoading;
}

int CDeepLearning::GetExifOrientation(CRegardsBitmap* pBitmap)
{
	bool fastDetection = true;
	bool isLoading = false;
	muLoading.lock();
	isLoading = isload;
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


bool CDeepLearning::FindFaceCompatible(const int &numFace)
{
	bool returnValue = false;
	bool fastDetection = true;
	bool isLoading = false;
	muLoading.lock();
	isLoading = isload;
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