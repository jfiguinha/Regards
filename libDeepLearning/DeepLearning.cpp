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
	if (IsResourceReady())
	{
		CFaceDetector faceDetector(fastDetection);
		return faceDetector.FindFace(pictureData, filename);
	}
	return vector<int>();
}


void CDeepLearning::LoadRessource(const bool& openCLCompatible, const bool& cudaCompatible)
{
	//CDetectRotation::LoadModel(rotation_json);
	CFaceDetector::LoadModel(openCLCompatible, cudaCompatible);
	try
	{
		std::lock_guard<std::mutex> lock(muLoading);
		isload = true;
	}
	catch (const std::exception& e)
	{
		std::cerr << "CDeepLearning LoadRessource Execution Error : " << e.what() << std::endl;
		return;
	}
}


void CDeepLearning::RemoveRedEyes(cv::Mat& pBitmap, const bool& fastDetection)
{
	std::vector<wxRect> listEye;
	if (IsResourceReady())
	{
		CFaceDetector faceDetector(fastDetection);
		faceDetector.RemoveRedEyes(pBitmap);
	}
}

bool CDeepLearning::IsResourceReady()
{
	bool isLoading = false;
	try
	{
		std::lock_guard<std::mutex> lock(muLoading);
		isLoading = isload;
	}
	catch (const std::exception& e)
	{
		std::cerr << "CDeepLearning::IsResourceReady Execution Error: " << e.what() << std::endl;
	}
	return isLoading;
}

int CDeepLearning::GetExifOrientation(const cv::Mat& pBitmap, const bool& fastDetection)
{

	if (IsResourceReady())
	{
		CDetectRotation detectRotation;
		return detectRotation.GetExifOrientation(pBitmap, fastDetection);
	}
	return 0;
}


bool CDeepLearning::FindFaceCompatible(const int& numFace, const bool& fastDetection)
{
	bool returnValue = false;

	if (IsResourceReady())
	{
		CFaceDetector faceDetector(fastDetection);
		returnValue = faceDetector.FaceRecognition(numFace);
	}

	return returnValue;
}
