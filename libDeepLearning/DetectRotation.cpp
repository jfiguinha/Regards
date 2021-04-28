#include "header.h"
#ifndef __APPLE__
#include "DetectRotation.h"
#include "FaceDetector.h"
#include <RegardsBitmap.h>
using namespace cv;
using namespace std;
using namespace cv::dnn;
using namespace Regards::OpenCV;

#define WRITE_OUTPUT_SAMPLE

bool CDetectRotation::isload = false;
mutex CDetectRotation::muLoading;

CDetectRotation::CDetectRotation()
{
	
}

CDetectRotation::~CDetectRotation()
{

}
/*
int CDetectRotation::GetAngleOrientation(CPictureData * pictureDat)
{
	int angle = DectectOrientationByFaceDetector(pictureDat);
	return angle;
}
*/
int CDetectRotation::GetExifOrientation(CRegardsBitmap* pBitmap)
{
	int angle = DectectOrientationByFaceDetector(pBitmap);
	return RotateToExifOrientation(angle);
}


int CDetectRotation::DectectOrientationByFaceDetector(CRegardsBitmap* pBitmap)
{
	int angle = 0;
	int selectAngle = 0;
	int maxFace = 0;
	//cv::Mat image = cv::imdecode(cv::Mat(1, pictureData->GetSize(), CV_8UC1, pictureData->GetData()), IMREAD_UNCHANGED);
	cv::Mat image(pBitmap->GetBitmapHeight(), pBitmap->GetBitmapWidth(), CV_8UC4, pBitmap->GetPtBitmap());
	cv::cvtColor(image, image, cv::COLOR_BGRA2BGR);
	float confidence = 0;
	RotateOpenCV(angle, maxFace, confidence, selectAngle, image);
	angle += 90;
	RotateOpenCV(angle, maxFace, confidence, selectAngle, image);
	angle += 90;
	RotateOpenCV(angle, maxFace, confidence, selectAngle, image);
	angle += 90;
	RotateOpenCV(angle, maxFace, confidence, selectAngle, image);
	return selectAngle;
}

void CDetectRotation::RotateOpenCV(const float &angle, int & maxFace, float & confidence, int & selectAngle,const cv::Mat & image)
{
	float bestConfidence = 0;
	cv::Mat dst;
	CFaceDetector faceDetector;
	faceDetector.RotateCorrectly(image, dst, angle);
	int nbFace = faceDetector.FindNbFace(dst, bestConfidence, 0.7);
	if (nbFace > 0 && (nbFace > maxFace || bestConfidence > confidence))
	{
		maxFace = nbFace;
		selectAngle = angle;
		confidence = bestConfidence;
		bestConfidence = 0.0;
	}
}


int CDetectRotation::RotateToExifOrientation(const int &angle)
{
	if (angle > 44 && angle < 135) //Rotate 90
		return 8;
	else if (angle > 134 && angle < 225) //Rotate 180
		return 3;
	else if (angle > 224 && angle < 315)
		return 6;
	return 0;
}

int CDetectRotation::RotateToOrientation(const int &angle)
{
	if (angle > 44 && angle < 135) //Rotate 90
		return 90;
	else if (angle > 134 && angle < 225) //Rotate 180
		return 180;
	else if (angle > 224 && angle < 315)
		return 270;
	return 0;
}

/*
int DeterminePictureAngle(cv::Mat & image)
{
	cv::Size size(224, 224);//the dst image size,e.g.100x100
	cv::Mat dst;//dst image
	cv::resize(image, dst, size);
	assert(image.isContinuous());
	const auto model = fdeep::load_model("C:\\developpement\\git_gcc\\Rotnet\\Rotnetcpp\\model\\rotnet_street_view_resnet50_keras2.json");
	// Use the correct scaling, i.e., low and high.
	const auto input = fdeep::tensor_from_bytes(dst.ptr(),
		static_cast<std::size_t>(dst.rows),
		static_cast<std::size_t>(dst.cols),
		static_cast<std::size_t>(dst.channels()),
		0.0f, 1.0f);
	return model.predict_class({ input });
}
*/
#endif