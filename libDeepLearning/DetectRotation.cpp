#include "header.h"
#include "DetectRotation.h"
using namespace cv;
using namespace std;
using namespace cv::dnn;
using namespace Regards::OpenCV;

static fdeep::model _model;
bool CDetectRotation::isload = false;
mutex CDetectRotation::muLoading;

CDetectRotation::CDetectRotation()
{
	
}

CDetectRotation::~CDetectRotation()
{

}

int CDetectRotation::GetAngleOrientation(CImageLoadingFormat * imageLoadingFormat)
{
	int angle = DectectOrientation(imageLoadingFormat);
	return angle;
}

int CDetectRotation::GetExifOrientation(CImageLoadingFormat * imageLoadingFormat)
{
	int angle = DectectOrientation(imageLoadingFormat);
	return RotateToExifOrientation(angle);
}

int CDetectRotation::DectectOrientation(CImageLoadingFormat * imageLoadingFormat)
{
	bool isLoading = false;
	muLoading.lock();
	isLoading = isload;
	muLoading.unlock();

	if (isLoading)
	{
		int compressMethod;
		unsigned long size;
		uint8_t * src = imageLoadingFormat->GetJpegData(size, compressMethod);
		std::vector<char> data(src, src + size);
		cv::Mat image = imdecode(Mat(data), 1);

		const auto input = fdeep::tensor_from_bytes(image.ptr(),
			static_cast<std::size_t>(image.rows),
			static_cast<std::size_t>(image.cols),
			static_cast<std::size_t>(image.channels()),
			0.0f, 1.0f);

		delete src;
		return _model.predict_class({ input });
	}
	return 0;

}


void CDetectRotation::LoadModel(const string &model_path)
{
	_model = fdeep::load_model(model_path);
	muLoading.lock();
	isload = true;
	muLoading.unlock();
}

int CDetectRotation::RotateToExifOrientation(const int &angle)
{
	if (angle > 44 && angle < 135) //Rotate 90
		return 6;
	else if (angle > 134 && angle < 225) //Rotate 180
		return 3;
	else if (angle > 224 && angle < 315)
		return 8;
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