#include "header.h"
#include "FaceDetector.h"
#include <SqlFaceDescriptor.h>
#include <SqlFacePhoto.h>
#include <dlib/dnn.h>
#include <dlib/clustering.h>
#include <dlib/image_io.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/opencv.h>
#include <RegardsBitmap.h>
#include "base64.h"
using namespace cv;
using namespace cv::dnn;
using namespace Regards::OpenCV;
using namespace Regards::Sqlite;
using namespace dlib;
using namespace std;

#define CAFFE
//#define WRITE_OUTPUT_SAMPLE

// ----------------------------------------------------------------------------------------

// The next bit of code defines a ResNet network.  It's basically copied
// and pasted from the dnn_imagenet_ex.cpp example, except we replaced the loss
// layer with loss_metric and made the network somewhat smaller.  Go read the introductory
// dlib DNN examples to learn what all this stuff means.
//
// Also, the dnn_metric_learning_on_images_ex.cpp example shows how to train this network.
// The dlib_face_recognition_resnet_model_v1 model used by this example was trained using
// essentially the code shown in dnn_metric_learning_on_images_ex.cpp except the
// mini-batches were made larger (35x15 instead of 5x5), the iterations without progress
// was set to 10000, and the training dataset consisted of about 3 million images instead of
// 55.  Also, the input layer was locked to images of size 150.
template <template <int, template<typename>class, int, typename> class block, int N, template<typename>class BN, typename SUBNET>
using residual = add_prev1<block<N, BN, 1, tag1<SUBNET>>>;

template <template <int, template<typename>class, int, typename> class block, int N, template<typename>class BN, typename SUBNET>
using residual_down = add_prev2<avg_pool<2, 2, 2, 2, skip1<tag2<block<N, BN, 2, tag1<SUBNET>>>>>>;

template <int N, template <typename> class BN, int stride, typename SUBNET>
using block = BN<con<N, 3, 3, 1, 1, relu<BN<con<N, 3, 3, stride, stride, SUBNET>>>>>;

template <int N, typename SUBNET> using ares = relu<residual<block, N, affine, SUBNET>>;
template <int N, typename SUBNET> using ares_down = relu<residual_down<block, N, affine, SUBNET>>;

template <typename SUBNET> using alevel0 = ares_down<256, SUBNET>;
template <typename SUBNET> using alevel1 = ares<256, ares<256, ares_down<256, SUBNET>>>;
template <typename SUBNET> using alevel2 = ares<128, ares<128, ares_down<128, SUBNET>>>;
template <typename SUBNET> using alevel3 = ares<64, ares<64, ares<64, ares_down<64, SUBNET>>>>;
template <typename SUBNET> using alevel4 = ares<32, ares<32, ares<32, SUBNET>>>;

using anet_type = loss_metric<fc_no_bias<128, avg_pool_everything<
	alevel0<
	alevel1<
	alevel2<
	alevel3<
	alevel4<
	max_pool<3, 3, 2, 2, relu<affine<con<32, 7, 7, 2, 2,
	input_rgb_image_sized<150>
	>>>>>>>>>>>>;

static Net net;    // And finally we load the DNN responsible for face recognition.
static anet_type anet;

const size_t inWidth = 300;
const size_t inHeight = 300;
const double inScaleFactor = 1.0;
const float confidenceThreshold = 0.7;
const cv::Scalar meanVal(104.0, 177.0, 123.0);
cv::CascadeClassifier eye_cascade;
bool CFaceDetector::isload = false;
std::mutex CFaceDetector::muLoading;

CFaceDetector::CFaceDetector()
{
}

CFaceDetector::~CFaceDetector()
{
}

void CFaceDetector::LoadModel(const string &config_file, const string &weight_file, const string &face_recognition, const string &eye_detection)
{
#ifdef CAFFE
	const std::string caffeConfigFile = config_file;//"C:\\developpement\\git_gcc\\Rotnet\\Rotnetcpp\\model\\deploy.prototxt";
	const std::string caffeWeightFile = weight_file;// "C:\\developpement\\git_gcc\\Rotnet\\Rotnetcpp\\model\\res10_300x300_ssd_iter_140000_fp16.caffemodel";
#else
	const std::string tensorflowConfigFile = config_file;// "C:\\developpement\\git_gcc\\Rotnet\\Rotnetcpp\\model\\opencv_face_detector.pbtxt";
	const std::string tensorflowWeightFile = weight_file;// "C:\\developpement\\git_gcc\\Rotnet\\Rotnetcpp\\model\\opencv_face_detector_uint8.pb";
#endif

	try
	{
#ifdef CAFFE
		net = cv::dnn::readNetFromCaffe(caffeConfigFile, caffeWeightFile);
#else
		net = cv::dnn::readNetFromTensorflow(tensorflowWeightFile, tensorflowConfigFile);
#endif

		deserialize(face_recognition) >> anet;

		eye_cascade.load(eye_detection);
	}
	catch (cv::Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}
	muLoading.lock();
	isload = true;
	muLoading.unlock();
}


void CFaceDetector::ImageToJpegBuffer(cv::Mat & image, std::vector<uchar> & buff)
{
	//std::vector<uchar> buff;//buffer for coding
	std::vector<int> param(2);
	param[0] = cv::IMWRITE_JPEG_QUALITY;
	param[1] = 50;//default(95) 0-100
	cv::imencode(".jpg", image, buff, param);
}

int CFaceDetector::FindFace(cv::Mat & image, int & angle, std::vector<cv::Rect> & pointOfFace, std::vector<CFace> & listOfFace, int typeRotate)
{
	cv::Mat dst;      //Mat object for output image file
	int tab[] = { 0, 270, 90, 180 };
	for (int type = 0; type < 4; type++)
	{
		int angle_detect = tab[type];
		//if(typeRotate == 0)
		//	Rotate(image, dst, angle_detect);
		//else
		RotateCorrectly(image, dst, angle_detect);


#ifdef WRITE_OUTPUT_SAMPLE
		wxString file = "d:\\test" + to_string(type) + ".jpg";
		cv::imwrite(file.ToStdString(), dst);
#endif

		//imshow("Display window", dst);                   // Show our image inside it.     
		//cv::waitKey(0);



		detectFaceOpenCVDNN(dst, listOfFace, pointOfFace);
		if (listOfFace.size() > 0)
		{
			angle = angle_detect;
			break;
		}

	}
	return listOfFace.size();
}

void CFaceDetector::Rotate(const cv::Mat& image, cv::Mat& dst, int degrees)
{
	cv::Point2f pt(image.cols / 2., image.rows / 2.);          //point from where to rotate
	cv::Mat r;       //Mat object for storing after rotation
	///applie an affine transforation to image.
	switch (degrees)
	{
	case 0:
		r = cv::getRotationMatrix2D(pt, 0, 1.0);      //Mat object for storing after rotation
		warpAffine(image, dst, r, cv::Size(image.cols, image.rows));
		break;
	case 180:
		r = cv::getRotationMatrix2D(pt, degrees, 1.0);      //Mat object for storing after rotation
		warpAffine(image, dst, r, cv::Size(image.rows, image.cols));
		break;
	case 270:
		r = cv::getRotationMatrix2D(pt, degrees, 1.0);      //Mat object for storing after rotation
		warpAffine(image, dst, r, cv::Size(image.cols, image.rows));
		break;
	case 90:
		r = cv::getRotationMatrix2D(pt, degrees, 1.0);      //Mat object for storing after rotation
		warpAffine(image, dst, r, cv::Size(image.rows, image.cols));
		break;
	}
}


int CFaceDetector::FindNbFace(cv::Mat & image, int & angle)
{
	std::vector<cv::Rect> pointOfFace;
	std::vector<CFace> listOfFace;
	return FindFace(image, angle, pointOfFace, listOfFace);
}


std::vector<int> CFaceDetector::FindFace(CRegardsBitmap * pBitmap)
{
	CSqlFaceDescriptor sqlfaceDescritor;
	std::vector<int> listFace;
	int i = 0;
	bool isLoading = false;
	muLoading.lock();
	isLoading = isload;
	muLoading.unlock();

	if (isLoading)
	{

		int angle = 0;
		CSqlFacePhoto facePhoto;
		cv::Mat dest;
		std::vector<CFace> listOfFace;
		std::vector<cv::Rect> pointOfFace;
		//std::vector<char> data = pictureData->CopyData();
		cv::Mat image(pBitmap->GetBitmapHeight(), pBitmap->GetBitmapWidth(), CV_8UC4, pBitmap->GetPtBitmap());
		cv::cvtColor(image, image, cv::COLOR_BGRA2BGR);

		FindFace(image, angle, pointOfFace, listOfFace);
		listOfFace.clear();
		pointOfFace.clear();
		RotateCorrectly(image, dest, angle);
		detectFaceOpenCVDNN(dest, listOfFace, pointOfFace);

		std::vector<cv_image<rgb_pixel>> faces;

		for (CFace face : listOfFace)
		{
			std::vector<uchar> buff;
			RotateCorrectly(face.croppedImage, image, (360 - angle) % 360);
			ImageToJpegBuffer(image, buff);
			int numFace = facePhoto.InsertFace(pBitmap->GetFilename(), ++i, face.croppedImage.rows, face.croppedImage.cols, face.confidence, reinterpret_cast<uchar*>(buff.data()), buff.size());
			listFace.push_back(numFace);
			cv::Size size(150, 150);
			cv::Mat dst;//dst image
			cv::resize(face.croppedImage, dst, size);
			//IplImage image2 = cvIplImage(face);
			cv_image<rgb_pixel> cimg(cvIplImage(dst));
			faces.push_back(cimg);
		}

		if (faces.size() == 0)
		{
			cout << "No faces found in image!" << endl;
			return listFace;
		}

		try
		{
			// This call asks the DNN to convert each face image in faces into a 128D vector.
			// In this 128D vector space, images from the same person will be close to each other
			// but vectors from different people will be far apart.  So we can use these vectors to
			// identify if a pair of images are from the same person or from different people.  
			std::vector<matrix<float, 0, 1>> face_descriptors = anet(faces);

			for (int i = 0; i < faces.size(); i++)
			{
				matrix<float, 0, 1> face = face_descriptors[i];
				ostringstream sout;
				serialize(face, sout);

				string base64_data = base64_encode(reinterpret_cast<const unsigned char*>(sout.str().c_str()), sout.str().size());
				sqlfaceDescritor.InsertFaceDescriptor(listFace[i], base64_data.c_str(), base64_data.size());
				//Write into database


			}
		}
		catch (cv::Exception& e)
		{
			const char* err_msg = e.what();
			std::cout << "exception caught: " << err_msg << std::endl;
			std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
		}
	}

	return listFace;
}

std::vector<int> CFaceDetector::FindFace(CPictureData * pictureData)
{
	CSqlFaceDescriptor sqlfaceDescritor;
	std::vector<int> listFace;
	int i = 0;
	bool isLoading = false;
	muLoading.lock();
	isLoading = isload;
	muLoading.unlock();

	if (isLoading)
	{
		
		int angle = 0;
		CSqlFacePhoto facePhoto;
		cv::Mat dest;
		std::vector<CFace> listOfFace;
		std::vector<cv::Rect> pointOfFace;
		//std::vector<char> data = pictureData->CopyData();
		cv::Mat image = cv::imdecode(cv::Mat(1, pictureData->GetSize(), CV_8UC1, pictureData->GetData()), IMREAD_UNCHANGED);


		FindFace(image, angle, pointOfFace, listOfFace);
		listOfFace.clear();
		pointOfFace.clear();
		RotateCorrectly(image, dest, angle);
		detectFaceOpenCVDNN(dest, listOfFace, pointOfFace);
							

		std::vector<cv_image<rgb_pixel>> faces;

		for (CFace face : listOfFace)
		{
			if (face.confidence > confidenceThreshold)
			{
				std::vector<uchar> buff;
				ImageToJpegBuffer(face.croppedImage, buff);
				int numFace = facePhoto.InsertFace(pictureData->GetFilename(), ++i, face.croppedImage.rows, face.croppedImage.cols, face.confidence, reinterpret_cast<uchar*>(buff.data()), buff.size());
				listFace.push_back(numFace);

				cv::Size size(150, 150);
				cv::Mat dst;//dst image
				cv::resize(face.croppedImage, dst, size);
				//IplImage image2 = cvIplImage(face);
				cv_image<rgb_pixel> cimg(cvIplImage(dst));
				faces.push_back(cimg);
			}

		}
		
		if (faces.size() == 0)
		{
			cout << "No faces found in image!" << endl;
			return listFace;
		}

		try
		{

			// This call asks the DNN to convert each face image in faces into a 128D vector.
			// In this 128D vector space, images from the same person will be close to each other
			// but vectors from different people will be far apart.  So we can use these vectors to
			// identify if a pair of images are from the same person or from different people.  
			std::vector<matrix<float, 0, 1>> face_descriptors = anet(faces);

			for (int i = 0; i < faces.size(); i++)
			{
				matrix<float, 0, 1> face = face_descriptors[i];
				ostringstream sout;
				serialize(face, sout);

				string base64_data = base64_encode(reinterpret_cast<const unsigned char*>(sout.str().c_str()), sout.str().size());
				sqlfaceDescritor.InsertFaceDescriptor(listFace[i], base64_data.c_str(), base64_data.size());
				//Write into database


			}
		}
		catch (cv::Exception& e)
		{
			const char* err_msg = e.what();
			std::cout << "exception caught: " << err_msg << std::endl;
			std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
		}


	}

	return listFace;
}

void CFaceDetector::RemoveRedEye(cv::Mat & image, const cv::Rect & rSelectionBox)
{
	int xmin, xmax, ymin, ymax;
	xmin = rSelectionBox.x;
	xmax = rSelectionBox.x + rSelectionBox.width;
	ymax = rSelectionBox.y + rSelectionBox.height;
	ymin = rSelectionBox.y;

	if (xmin > xmax)
	{
		int old = xmax;
		xmax = xmin;
		xmin = old;
	}

	if (ymin > xmax)
	{
		int old = ymax;
		ymax = ymin;
		ymin = old;
	}


	for (int32_t y = ymin; y < ymax; y++) {
		for (int32_t x = xmin; x < xmax; x++)
		{
			float a = 1.0f - 5.0f*((float)((x - 0.5f*(xmax + xmin))*(x - 0.5f*(xmax + xmin)) + (y - 0.5f*(ymax + ymin))*(y - 0.5f*(ymax + ymin)))) / ((float)((xmax - xmin)*(ymax - ymin)));
			if (a < 0)
				a = 0;
			image.at<Vec4b>(Point(x, y))[2] = (uint8_t)(a*min(image.at<Vec4b>(Point(x, y))[1], image.at<Vec4b>(Point(x, y))[0]) + (1.0f - a)*image.at<Vec4b>(Point(x, y))[2]);
		}
	}
}

void CFaceDetector::DetectEyes(CRegardsBitmap * pBitmap)
{
	std::vector<wxRect> listEye;
	
	std::vector<cv::Rect> pointOfFace;
	int i = 0;
	bool isLoading = false;
	muLoading.lock();
	isLoading = isload;
	muLoading.unlock();
	int angle = 0;

	if (isLoading)
	{
		cv::Mat dest;
		std::vector<CFace> listOfFace;

		cv::Mat image(pBitmap->GetBitmapHeight(), pBitmap->GetBitmapWidth(), CV_8UC4, pBitmap->GetPtBitmap());
		cv::cvtColor(image, dest, cv::COLOR_BGRA2BGR);
		FindFace(dest, angle, pointOfFace, listOfFace);
		RotateCorrectly(dest, dest, angle);
		listOfFace.clear();
		pointOfFace.clear();
		detectFaceOpenCVDNN(dest, listOfFace, pointOfFace);
			   
		if (listOfFace.size() > 0)
		{
			RotateCorrectly(image, image, angle);

			for (int i = 0; i < listOfFace.size(); i++)
			{
				if (listOfFace[i].confidence > confidenceThreshold)
				{
					cv::Mat gray;
					std::vector<cv::Rect> eyes;

					cv::cvtColor(listOfFace[i].croppedImage, gray, COLOR_BGR2GRAY);
					eye_cascade.detectMultiScale(gray, eyes, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
					for (cv::Rect rect : eyes)
					{
						cv::Rect rectEye;

						rectEye.x = rect.x + pointOfFace[i].x;
						rectEye.y = rect.y + pointOfFace[i].y;
						rectEye.width = rect.width;
						rectEye.height = rect.height;
						RemoveRedEye(image, rectEye);
					}
				}
			}
		}

		cv::Mat cloneImage;
		RotateCorrectly(image, cloneImage, 360 - angle);
		pBitmap->SetBitmap(cloneImage.data, pBitmap->GetBitmapWidth(), pBitmap->GetBitmapHeight());
	}
}

void CFaceDetector::RotateCorrectly(cv::Mat const &src, cv::Mat &dst, int angle)
{
	CV_Assert(angle % 90 == 0 && angle <= 360 && angle >= -360);
	if (angle == 90) {
		// Rotate clockwise 270 degrees
		cv::transpose(src, dst);
		cv::flip(dst, dst, 0);
	}
	else if (angle == 180) {
		// Rotate clockwise 180 degrees
		cv::flip(src, dst, -1);
	}
	else if (angle == 270) {
		// Rotate clockwise 90 degrees
		cv::transpose(src, dst);
		cv::flip(dst, dst, 1);
	}
	else if (angle == 0) {
		if (src.data != dst.data) {
			src.copyTo(dst);
		}
	}
}

//--------------------------------------------------
//Code From https://github.com/spmallick/learnopencv
//--------------------------------------------------
void CFaceDetector::detectFaceOpenCVDNN(Mat &frameOpenCVDNN, std::vector<CFace> & listOfFace, std::vector<cv::Rect> & pointOfFace)
{
	int frameHeight = frameOpenCVDNN.rows;
	int frameWidth = frameOpenCVDNN.cols;
#ifdef CAFFE

	//frameOpenCVDNN.resize(300, 300);
	cv::Mat inputBlob = cv::dnn::blobFromImage(frameOpenCVDNN, 1.0, cv::Size(300, 300), (104.0, 177.0, 123.0));
	//cv::Mat inputBlob = cv::dnn::blobFromImage(frameOpenCVDNN, inScaleFactor, cv::Size(inWidth, inHeight), meanVal, false, false);
	net.setInput(inputBlob);
	auto detection = net.forward();

#else
	cv::Mat inputBlob = cv::dnn::blobFromImage(frameOpenCVDNN, inScaleFactor, cv::Size(inWidth, inHeight), meanVal, true, false);
	net.setInput(inputBlob, "data");
	cv::Mat detection = net.forward("detection_out");

#endif

	cv::Mat detectionMat(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());


	try
	{
		for (int i = 0; i < detectionMat.rows; i++)
		{
			float confidence = detectionMat.at<float>(i, 2);

			CFace face;
			face.confidence = confidence;

			int x1 = static_cast<int>(detectionMat.at<float>(i, 3) * frameWidth);
			int y1 = static_cast<int>(detectionMat.at<float>(i, 4) * frameHeight);
			int x2 = static_cast<int>(detectionMat.at<float>(i, 5) * frameWidth);
			int y2 = static_cast<int>(detectionMat.at<float>(i, 6) * frameHeight);

			//cv::rectangle(frameOpenCVDNN, cv::Point(x1, y1), cv::Point(x2, y2), cv::Scalar(0, 255, 0), 22, 4);

			// Setup a rectangle to define your region of interest
			cv::Rect myROI(cv::Point(x1, y1), cv::Point(x2, y2));

			// Crop the full image to that image contained by the rectangle myROI
			// Note that this doesn't copy the data
			face.croppedImage = frameOpenCVDNN(myROI);


			cv::Mat gray;
			std::vector<cv::Rect> eyes;

			cv::cvtColor(face.croppedImage, gray, COLOR_BGR2GRAY);
			eye_cascade.detectMultiScale(gray, eyes, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
			if (eyes.size() > 0)
			{
				listOfFace.push_back(face);
				pointOfFace.push_back(myROI);
			}

		}
	}
	catch (cv::Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}
}
