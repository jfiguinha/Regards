#include "header.h"
#include "FaceDetector.h"
#include <SqlFaceDescriptor.h>
#include <SqlFacePhoto.h>
#include <PictureData.h>
#include <dlib/dnn.h>
#include <dlib/clustering.h>
#include <dlib/image_io.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/opencv.h>
#include "base64.h"
using namespace cv;
using namespace cv::dnn;
using namespace Regards::OpenCV;
using namespace Regards::Sqlite;
using namespace dlib;
using namespace std;

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

bool CFaceDetector::isload = false;
std::mutex CFaceDetector::muLoading;

CFaceDetector::CFaceDetector()
{
}

CFaceDetector::~CFaceDetector()
{
}

void CFaceDetector::LoadModel(const string &config_file, const string &weight_file, const string &face_recognition)
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
	param[1] = 80;//default(95) 0-100
	cv::imencode(".jpg", image, buff, param);
}

int CFaceDetector::FindNbFace(cv::Mat & image)
{
	std::vector<cv::Mat> listOfFace;

	//imshow("Display window", image);                   // Show our image inside it.     
	//cv::waitKey(0);

	detectFaceOpenCVDNN(image, listOfFace);


	return listOfFace.size();
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
		
		CSqlFacePhoto facePhoto;
		std::vector<cv::Mat> listOfFace;
		//std::vector<char> data = pictureData->CopyData();
		cv::Mat image = cv::imdecode(cv::Mat(1, pictureData->GetSize(), CV_8UC1, pictureData->GetData()), IMREAD_UNCHANGED);

		detectFaceOpenCVDNN(image, listOfFace);

		std::vector<cv_image<rgb_pixel>> faces;

		for (Mat face : listOfFace)
		{
			std::vector<uchar> buff;
			ImageToJpegBuffer(face, buff);
			int numFace = facePhoto.InsertFace(pictureData->GetFilename(), i++, image.rows, image.cols, 1.0, reinterpret_cast<uchar*>(buff.data()), buff.size());
			listFace.push_back(numFace);

			cv::Size size(150, 150);
			cv::Mat dst;//dst image
			cv::resize(face, dst, size);
			//IplImage image2 = cvIplImage(face);
			cv_image<rgb_pixel> cimg(cvIplImage(dst));
			faces.push_back(cimg);
		}
		
		if (faces.size() == 0)
		{
			cout << "No faces found in image!" << endl;
			return listFace;
		}

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

	return listFace;
}

//--------------------------------------------------
//Code From https://github.com/spmallick/learnopencv
//--------------------------------------------------
void CFaceDetector::detectFaceOpenCVDNN(Mat &frameOpenCVDNN, std::vector<Mat> & listOfFace)
{
	int frameHeight = frameOpenCVDNN.rows;
	int frameWidth = frameOpenCVDNN.cols;
#ifdef CAFFE
	cv::Mat inputBlob = cv::dnn::blobFromImage(frameOpenCVDNN, inScaleFactor, cv::Size(inWidth, inHeight), meanVal, false, false);
#else
	cv::Mat inputBlob = cv::dnn::blobFromImage(frameOpenCVDNN, inScaleFactor, cv::Size(inWidth, inHeight), meanVal, true, false);
#endif

	net.setInput(inputBlob, "data");
	cv::Mat detection = net.forward("detection_out");
	cv::Mat detectionMat(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());

	try
	{
		for (int i = 0; i < detectionMat.rows; i++)
		{
			float confidence = detectionMat.at<float>(i, 2);

			if (confidence > confidenceThreshold)
			{
				int x1 = static_cast<int>(detectionMat.at<float>(i, 3) * frameWidth);
				int y1 = static_cast<int>(detectionMat.at<float>(i, 4) * frameHeight);
				int x2 = static_cast<int>(detectionMat.at<float>(i, 5) * frameWidth);
				int y2 = static_cast<int>(detectionMat.at<float>(i, 6) * frameHeight);

				//cv::rectangle(frameOpenCVDNN, cv::Point(x1, y1), cv::Point(x2, y2), cv::Scalar(0, 255, 0), 22, 4);

				// Setup a rectangle to define your region of interest
				cv::Rect myROI(cv::Point(x1, y1), cv::Point(x2, y2));

				// Crop the full image to that image contained by the rectangle myROI
				// Note that this doesn't copy the data
				cv::Mat croppedImage = frameOpenCVDNN(myROI);

				listOfFace.push_back(croppedImage);
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
