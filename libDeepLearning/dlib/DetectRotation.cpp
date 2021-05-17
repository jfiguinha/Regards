#include "header.h"
#include "DetectRotation.h"
#include "FaceDetector.h"
#include <RegardsBitmap.h>
using namespace cv;
using namespace std;
using namespace cv::dnn;
using namespace Regards::OpenCV;

#define WRITE_OUTPUT_SAMPLE

CDetectRotation::CDetectRotation()
{
	
}

CDetectRotation::~CDetectRotation()
{

}

int CDetectRotation::GetExifOrientation(CRegardsBitmap* pBitmap, const bool& fastDetection)
{
	CFaceDetector faceDetector(fastDetection);
	int angle = faceDetector.DectectOrientationByFaceDetector(pBitmap);
	return RotateToExifOrientation(angle);
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
