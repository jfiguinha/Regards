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

void vec_append(fdeep::float_vec& results, const fdeep::float_vec& x) {
	results.insert(std::end(results), std::begin(x), std::end(x));
	return;
}


int CDetectRotation::DectectOrientation(CImageLoadingFormat * imageLoadingFormat)
{
	int angle = 0;
	bool isLoading = false;
	muLoading.lock();
	isLoading = isload;
	muLoading.unlock();

	if (isLoading)
	{
        int minpourcent = 50;
		int compressMethod;
		unsigned long size;
		uint8_t * src = imageLoadingFormat->GetJpegData(size, compressMethod);
		//std::vector<char> data(src, src + size);
		//cv::Mat image = imdecode(Mat(data), 1);
		cv::Mat image = cv::imdecode(cv::Mat(1, size, CV_8UC1, src), IMREAD_UNCHANGED);
		cv::Mat dst;//dst image
		cv::cvtColor(image, dst, cv::COLOR_BGR2RGB);
		cv::flip(dst, dst, 1);
		cv::Point2f pt(image.cols / 2., image.rows / 2.);          //point from where to rotate    
		cv::Mat r;       //Mat object for storing after rotation
			///applie an affine transforation to image.
		//r = cv::getRotationMatrix2D(pt, 180, 1.0);
		warpAffine(dst, image, r, cv::Size(image.cols, image.rows));

		imshow("Display window", image);                   // Show our image inside it.     
		cv::waitKey(0);

		int sizeCrop = 0;

		if (image.cols > image.rows)
			sizeCrop = image.rows;
		else
			sizeCrop = image.cols;

		int width = 224;
		int height = 224;

		int x1 = int((image.cols - sizeCrop) * 0.5);
		//int x2 = int(image_center[0] + width * 0.5);
		int y1 = int((image.rows - sizeCrop) * 0.5);
		//int y2 = int(image_center[1] + height * 0.5);
		cv::Rect myROI(x1, y1, sizeCrop, sizeCrop);

		try
		{
			cv::Mat croppedImage = image(myROI);
			cv::resize(croppedImage, dst, cv::Size(224, 224));
		}
		catch (cv::Exception& e)
		{
			const char* err_msg = e.what();
			std::cout << "exception caught: " << err_msg << std::endl;
			std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
		}

		// cv::resize(image,dst,size);
		assert(dst.isContinuous());
		//const auto model = fdeep::load_model("/home/figuinha/developpement/git/model/fdeep_model.json");

		// Use the correct scaling, i.e., low and high.
	   const auto result = _model.predict({fdeep::tensor(fdeep::tensor_from_bytes(dst.ptr(), static_cast<std::size_t>(dst.rows), static_cast<std::size_t>(dst.cols), static_cast<std::size_t>(dst.channels()), 0.0f, 1.0f))});

		fdeep::float_vec all_results = {};
		vec_append(all_results, *result[0].as_vector());
		//const std::vector<float> result_vec = result.front().as_vector();

        float result0 = all_results[0];
        float result180 = all_results[1];
        float result270 = all_results[2];
        float result90 = all_results[3];
        
       std::cout << "show_tensor Result Tensor : " << std::endl;
       std::cout << fdeep::show_tensors(result) << std::endl;            
        
        if(!(result0 > minpourcent || result180 > minpourcent || result270 > minpourcent || result90 > minpourcent))
            return 0;
            


		//cv::Mat resizeMat;

		if (result0 > result90 && result0 > result180 && result0 > result270)
		{
			angle = 0;
		}
		else if (result90 > result0 && result90 > result180 && result90 > result270)
		{
			angle = 90;
		}
		else if (result180 > result0 && result180 > result90 && result180 > result270)
		{
			angle = 180;
		}
		else
		{
			angle = 270;
		}

		/*
		cv::Size size_input(224, 224);//the dst image size,e.g.100x100
		cv::Mat dst;//dst image
		cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
		cv::resize(image, dst, size_input);
		const auto input = fdeep::tensor_from_bytes(dst.ptr(),
			static_cast<std::size_t>(dst.rows),
			static_cast<std::size_t>(dst.cols),
			static_cast<std::size_t>(dst.channels()),
			0.0f, 1.0f);
		
		

		angle = _model.predict_class({ input });

		*/

		imageLoadingFormat->DestroyJpegData(src, compressMethod);
	}
	return angle;

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