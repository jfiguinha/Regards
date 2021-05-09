#include "header.h"
#include "FaceDetector.h"
#include <SqlFaceDescriptor.h>
#include <SqlFacePhoto.h>
#include <dlib/dnn.h>
#include <dlib/clustering.h>
#include <dlib/image_io.h>

#include <dlib/opencv.h>
#include <RegardsBitmap.h>
#include "base64.h"
#include <RegardsConfigParam.h>
#include <ParamInit.h>

#define WIDTH_THUMBNAIL 1920
#define HEIGHT_THUMBNAIL 1080

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
static shape_predictor sp;

//const size_t inWidth = 300;
//const size_t inHeight = 300;
//const double inScaleFactor = 1.0;
const float confidence = 0.5;
const float confidenceThreshold = 0.7;
const cv::Scalar meanVal(104.0, 177.0, 123.0);
static cv::CascadeClassifier eye_cascade;

bool CFaceDetector::isload = false;
std::mutex CFaceDetector::muLoading;
std::mutex CFaceDetector::muDnnAccess;
std::mutex CFaceDetector::muEyeAccess;
std::mutex CFaceDetector::muDlibAccess;
std::mutex CFaceDetector::muDlibLandmarkAccess;

class CFaceDetectPriv
{
public:

	static cv::Rect dlibRectangleToOpenCV(dlib::rectangle r)
	{
		return cv::Rect(cv::Point2i(r.left(), r.top()), cv::Point2i(r.right() + 1, r.bottom() + 1));
	}

	static dlib::rectangle openCVRectToDlib(cv::Rect r)
	{
		return dlib::rectangle((long)r.tl().x, (long)r.tl().y, (long)r.br().x - 1, (long)r.br().y - 1);
	}

	/** This function converts dlib::point to cv::Point and stores in a vector of landmarks
		This function is needed because in this implementation I have used opencv and dlib bothand they
		both have their own image processing library so when using a dlib method, its arguments should be
		as expected */
	static void point2cv_Point(full_object_detection& S, std::vector<Point>& L)
	{
		for (unsigned int i = 0; i < S.num_parts(); ++i)
		{
			L.push_back(Point(S.part(i).x(), S.part(i).y()));
		}
	}
};

bool CFaceDetector::LockOpenCLDnn()
{
#ifndef __WXGTK__
	muDnnAccess.lock();
#endif
    return true;
}

bool CFaceDetector::UnlockOpenCLDnn()
{
#ifndef __WXGTK__
	muDnnAccess.unlock();
#endif
    return true;
}

CFaceDetector::CFaceDetector(const bool& fastDetection)
{
	detector = get_frontal_face_detector();
	this->fastDetection = fastDetection;
}

CFaceDetector::~CFaceDetector()
{
}

void CFaceDetector::RotateOpenCV(const float& angle, int& maxFace, float& confidence, int& selectAngle, const cv::Mat& image)
{
	float bestConfidence = 0;
	cv::Mat dst;
	RotateCorrectly(image, dst, angle);
	int nbFace = FindNbFace(dst, bestConfidence, 0.7);
	if (nbFace > 0 && (nbFace > maxFace || bestConfidence > confidence))
	{
		maxFace = nbFace;
		selectAngle = angle;
		confidence = bestConfidence;
		bestConfidence = 0.0;
	}
}


int CFaceDetector::DectectOrientationByFaceDetector(CRegardsBitmap* pBitmap)
{
	cv::Mat image(pBitmap->GetBitmapHeight(), pBitmap->GetBitmapWidth(), CV_8UC4, pBitmap->GetPtBitmap());
	cv::cvtColor(image, image, cv::COLOR_BGRA2BGR);
	return DectectOrientationByFaceDetector(image);
}


float CalculPictureRatio(const int& pictureWidth, const int& pictureHeight)
{
	if (pictureWidth == 0 && pictureHeight == 0)
		return 1.0f;

	float newRatio = 1;

	//int tailleAffichageWidth = 0, tailleAffichageHeight = 0;

	if (pictureWidth > pictureHeight)
		newRatio = (float)WIDTH_THUMBNAIL / (float)(pictureWidth);
	else
		newRatio = (float)HEIGHT_THUMBNAIL / (float)(pictureHeight);

	if ((pictureHeight * newRatio) > HEIGHT_THUMBNAIL)
	{
		newRatio = (float)HEIGHT_THUMBNAIL / (float)(pictureHeight);
	}
	if ((pictureWidth * newRatio) > WIDTH_THUMBNAIL)
	{
		newRatio = (float)WIDTH_THUMBNAIL / (float)(pictureWidth);
	}

	return newRatio;
}

int CFaceDetector::DectectOrientationByFaceDetector(const cv::Mat & image)
{
	cv::Mat gray, resized;

	if (!fastDetection)
	{
		double scale = CalculPictureRatio(image.cols, image.rows);
		//converts original image to gray scale and stores it in "gray".
		cvtColor(image, gray, COLOR_BGR2GRAY);

		//resize the gray scale image for speeding the face detection.
		cv::resize(gray, resized, Size(), scale, scale);

		//cout<<"Resized Image"<<" Rows "<< resized.rows<<" Cols "<<resized.cols<<endl;
		// cout<<"Original Image"<<" Rows "<< image.rows<<" Cols "<<image.cols<<endl;
		//Histogram equalization is performed on the resized image to improve the contrast of the image which can help in detection.
		equalizeHist(resized, resized);

		cvtColor(resized, gray, COLOR_GRAY2BGR);
	}
	else
		gray = image;

	int angle = 0;
	int selectAngle = 0;
	int maxFace = 0;
	float confidence = 0;
	RotateOpenCV(angle, maxFace, confidence, selectAngle, gray);
	angle += 90;
	RotateOpenCV(angle, maxFace, confidence, selectAngle, gray);
	angle += 90;
	RotateOpenCV(angle, maxFace, confidence, selectAngle, gray);
	angle += 90;
	RotateOpenCV(angle, maxFace, confidence, selectAngle, gray);
	return selectAngle;
}

void CFaceDetector::LoadModel(const string &config_file, const string &weight_file, const string &face_recognition, const string &eye_detection, const string& landmarkPath)
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

		/*
			"{ backend     |  0 | Choose one of computation backends: "
						 "0: automatically (by default), "
						 "1: Halide language (http://halide-lang.org/), "
						 "2: Intel's Deep Learning Inference Engine (https://software.intel.com/openvino-toolkit), "
						 "3: OpenCV implementation }"
			"{ target      | 0 | Choose one of target computation devices: "
						 "0: CPU target (by default), "
						 "1: OpenCL, "
						 "2: OpenCL fp16 (half-float precision), "
						 "3: VPU }"

		*/

		/*
		DNN_TARGET_CPU = 0,
		DNN_TARGET_OPENCL,
		DNN_TARGET_OPENCL_FP16,
		DNN_TARGET_MYRIAD,
		DNN_TARGET_VULKAN,
		DNN_TARGET_FPGA,  //!< FPGA device with CPU fallbacks using Inference Engine's Heterogeneous plugin.
		DNN_TARGET_CUDA,
		DNN_TARGET_CUDA_FP16
		*/

		bool openCLCompatible = false;
		CRegardsConfigParam * config = CParamInit::getInstance();
		if (config != nullptr)
		{
			if (config->GetIsOpenCLSupport())
				openCLCompatible = config->GetFaceOpenCLProcess();
		}


		net = cv::dnn::readNetFromCaffe(caffeConfigFile, caffeWeightFile);
		net.setPreferableBackend(DNN_BACKEND_DEFAULT);
#ifndef __WXGTK__
		if (openCLCompatible)
			net.setPreferableTarget(DNN_TARGET_OPENCL);
		else
			net.setPreferableTarget(DNN_TARGET_CPU);
#else
        net.setPreferableTarget(DNN_TARGET_CPU);
#endif
#else
		net = cv::dnn::readNetFromTensorflow(tensorflowWeightFile, tensorflowConfigFile);
#endif

		deserialize(face_recognition) >> anet;
		deserialize(landmarkPath) >> sp;
		eye_cascade.load(eye_detection);
	}
	catch (cv::Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}

	isload = true;

}


double CFaceDetector::face_alignement(const cv::Mat & image, bool &findEye) {

	int angle_add = 0;
	//Declaring a variable "image" to store input image given.
	Mat gray, detected_edges, Laugh_L, Laugh_R;

	//converts original image to gray scale and stores it in "gray".
	cvtColor(image, gray, COLOR_BGR2GRAY);

	//Histogram equalization is performed on the resized image to improve the contrast of the image which can help in detection.
	equalizeHist(gray, gray);

	dlib::rectangle rectDlib;
	cv::Rect R;
	R.x = 0;
	R.y = 0;
	R.width = image.cols;
	R.height = image.rows;

	rectDlib = CFaceDetectPriv::openCVRectToDlib(R);

	//landmarks vector is declared to store the 68 landmark points. The rest are for individual face components
	std::vector<cv::Point> landmarks , R_Eyebrow, L_Eyebrow, L_Eye, R_Eye, Mouth, Jaw_Line, Nose;

	/**at each index of "shapes" vector there is an object of full_object_detection class which stores the 68 landmark
	points in dlib::point from, which needs to be converted back to cv::Point for displaying.*/
	//std::vector<full_object_detection> shape;
	muDlibLandmarkAccess.lock();
	full_object_detection shape = sp(dlib::cv_image<unsigned char>(gray), rectDlib);
	muDlibLandmarkAccess.unlock();
	CFaceDetectPriv::point2cv_Point(shape, landmarks);

	if (landmarks.size() < 45)
		return 0;

	findEye = true;


	for (size_t s = 0; s < landmarks.size(); s++) {
		//circle(image,landmarks[s], 2.0, Scalar( 255,0,0 ), 1, 8 );
		//putText(image,to_string(s),landmarks[s],FONT_HERSHEY_PLAIN,0.8,Scalar(0,0,0));

		// Right Eyebrow indicies
		if (s >= 22 && s <= 26) {
			R_Eyebrow.push_back(landmarks[s]);
			//circle( image,landmarks[s], 2.0, Scalar( 0, 0, 255 ), 1, 8 );
		}
		// Left Eyebrow indicies
		else if (s >= 17 && s <= 21) {
			L_Eyebrow.push_back(landmarks[s]);
		}
		// Left Eye indicies
		else if (s >= 36 && s <= 41) {
			L_Eye.push_back(landmarks[s]);
		}
		// Right Eye indicies
		else if (s >= 42 && s <= 47) {
			R_Eye.push_back(landmarks[s]);
		}
		// Mouth indicies
		else if (s >= 48 && s <= 67) {
			Mouth.push_back(landmarks[s]);
		}
		// Jawline Indicies
		else if (s >= 0 && s <= 16) {
			Jaw_Line.push_back(landmarks[s]);
		}
		// Nose Indicies
		else if (s >= 27 && s <= 35) {
			Nose.push_back(landmarks[s]);
		}
	}
	// Extract each part using the pre fixed indicies
#ifdef FACE_DESCRIPTOR
	// 2D image points. If you change the image, you need to change vector
	std::vector<cv::Point2d> image_points;
	image_points.push_back(landmarks[30]);    // Nose tip
	image_points.push_back(landmarks[8]);    // Chin
	image_points.push_back(landmarks[45]);     // Left eye left corner
	image_points.push_back(landmarks[36]);    // Right eye right corner
	image_points.push_back(landmarks[54]);    // Left Mouth corner
	image_points.push_back(landmarks[48]);    // Right mouth corner

	// 3D model points.
	std::vector<cv::Point3d> model_points;
	model_points.push_back(cv::Point3d(0.0f, 0.0f, 0.0f));               // Nose tip
	model_points.push_back(cv::Point3d(0.0f, -330.0f, -65.0f));          // Chin
	model_points.push_back(cv::Point3d(-225.0f, 170.0f, -135.0f));       // Left eye left corner
	model_points.push_back(cv::Point3d(225.0f, 170.0f, -135.0f));        // Right eye right corner
	model_points.push_back(cv::Point3d(-150.0f, -150.0f, -125.0f));      // Left Mouth corner
	model_points.push_back(cv::Point3d(150.0f, -150.0f, -125.0f));       // Right mouth corner

	// Camera internals
	double focal_length = image.cols; // Approximate focal length.
	Point2d center = cv::Point2d(image.cols / 2, image.rows / 2);
	cv::Mat camera_matrix = (cv::Mat_<double>(3, 3) << focal_length, 0, center.x, 0, focal_length, center.y, 0, 0, 1);
	cv::Mat dist_coeffs = cv::Mat::zeros(4, 1, cv::DataType<double>::type); // Assuming no lens distortion

	// cout << "Camera Matrix " << endl << camera_matrix << endl ;
	// Output rotation and translation
	cv::Mat rotation_vector; // Rotation in axis-angle form
	cv::Mat translation_vector;

	// Solve for pose
	cv::solvePnP(model_points, image_points, camera_matrix, dist_coeffs, rotation_vector, translation_vector);

	// Draw line between two eyes
	cv::line(image,landmarks[45],landmarks[36],Scalar(0,255,0),4);

	// Access the last element in the Rotation Vector
	double rot = rotation_vector.at<double>(0, 2);
#endif
	double rot_eye = atan2(landmarks[45].y - landmarks[36].y, landmarks[45].x - landmarks[36].x);

	if (Mouth[0].y < landmarks[45].y)
		angle_add = 180;

/*

	std::vector<cv::Rect> eyes;

	muEyeAccess.lock();
	eye_cascade.detectMultiScale(gray, eyes, 1.3, 6, CASCADE_FIND_BIGGEST_OBJECT, cv::Size(gray.rows * 0.1, gray.cols * 0.1));
	muEyeAccess.unlock();

	if (eyes.size() != 2)
	{
		return 0;
	}

	findEye = true;

	cv::Point2d left_eye;
	left_eye.x = eyes[0].x;
	left_eye.y = eyes[0].y;

	cv::Point2d right_eye;
	right_eye.x = eyes[1].x;
	right_eye.y = eyes[1].y;

	cv::line(image, left_eye, right_eye, Scalar(0, 255, 0), 4);
	Point2d center = cv::Point2d(image.cols / 2, image.rows / 2);
	double rot_eye = atan2(right_eye.y - left_eye.y, right_eye.x - left_eye.x);
*/

	// Conver to degrees
	//double theta_deg = rot / M_PI * 180;
	double theta_deg_eye = rot_eye / M_PI * 180 + angle_add;
#if defined(WIN32)
	wchar_t message[255];
	//wsprintf(message, L"rot : x : %d \n", (int)theta_deg);
	//OutputDebugString(message);
	wsprintf(message, L"rot_eye : x : %d \n", (int)theta_deg_eye);
	OutputDebugString(message);
#endif

	return theta_deg_eye;
}

cv::Mat CFaceDetector::RotateAndExtractFace(const double& theta_deg_eye, const cv::Rect& faceLocation, const cv::Mat& image)
{
	Mat dst;
	// Rotate around the center
	Point2d center = cv::Point2d(faceLocation.x + faceLocation.width / 2, faceLocation.y + faceLocation.height / 2);
	Point2f pt(faceLocation.width / 2., faceLocation.height / 2.);

	// determine bounding rectangle
	cv::Rect bbox = cv::RotatedRect(center, cv::Size(faceLocation.width, faceLocation.height), theta_deg_eye).boundingRect();

	bbox.x = max(bbox.x, 0);
	bbox.y = max(bbox.y, 0);
	bbox.width = max(bbox.width, 0);
	bbox.height = max(bbox.height, 0);
	dst = image(bbox);


	center = cv::Point2d(dst.cols / 2, dst.rows / 2);
	Mat r = getRotationMatrix2D(center, theta_deg_eye, 1.0);
	// adjust transformation matrix
	r.at<double>(0, 2) += bbox.width / 2.0 - center.x;
	r.at<double>(1, 2) += bbox.height / 2.0 - center.y;

	// Apply affine transform
	warpAffine(dst, dst, r, bbox.size());

	cv::Rect rect = faceLocation;
	rect.x = max((bbox.width - faceLocation.width) / 2, 0);
	rect.y = max((bbox.height - faceLocation.height) / 2, 0);
	rect.width = max(faceLocation.width, 0);
	rect.height = max(faceLocation.height, 0);
	dst = dst(rect);
	//imwrite("d:\\test1.jpg", image);
	//imwrite("d:\\test2.jpg", dst);

	return dst;
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
		pBitmap->VertFlipBuf();
		

		CSqlFacePhoto facePhoto;
		cv::Mat dest;
		std::vector<CFace> listOfFace;
		std::vector<cv::Rect> pointOfFace;
		//std::vector<char> data = pictureData->CopyData();
		
		cv::Mat image(pBitmap->GetBitmapHeight(), pBitmap->GetBitmapWidth(), CV_8UC4, pBitmap->GetPtBitmap());
		cv::cvtColor(image, dest, cv::COLOR_BGRA2BGR);
		int angle = DectectOrientationByFaceDetector(dest);
		RotateCorrectly(dest, image, angle);

		if (!fastDetection)
			DetectFaceDlib(image, listOfFace, pointOfFace);
		else
			detectFaceOpenCVDNN(image, listOfFace, pointOfFace);

		std::vector<cv_image<rgb_pixel>> faces;

		for (CFace face : listOfFace)
		{
			//if (face.confidence > confidence)
			//
				bool findEye = false;
				//imwrite("d:\\test1.jpg", face.croppedImage);
				double angleRot = face_alignement(face.croppedImage, findEye);
				if (findEye)
				{
					try
					{
						face.croppedImage = RotateAndExtractFace(angleRot, face.myROI, image);
						//imwrite("d:\\test2.jpg", face.croppedImage);
						cv::Size size(150, 150);
						std::vector<uchar> buff;
						cv::resize(face.croppedImage, face.croppedImage, size);
						ImageToJpegBuffer(face.croppedImage, buff);
						int numFace = facePhoto.InsertFace(pBitmap->GetFilename(), ++i, face.croppedImage.rows, face.croppedImage.cols, face.confidence, reinterpret_cast<uchar*>(buff.data()), buff.size());
						listFace.push_back(numFace);
						cv_image<rgb_pixel> cimg(face.croppedImage);
						faces.push_back(cimg);
					}
					catch (cv::Exception& e)
					{
						const char* err_msg = e.what();
						std::cout << "exception caught: " << err_msg << std::endl;
						std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
					}
				}
			//}
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
			muDlibAccess.lock();
			std::vector<matrix<float, 0, 1>> face_descriptors = anet(faces);
			muDlibAccess.unlock();

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


void CFaceDetector::DetectEyes(CRegardsBitmap * pBitmap)
{
	std::vector<wxRect> listEye;
	
	std::vector<cv::Rect> pointOfFace;
	//int i = 0;
	bool isLoading = false;
	muLoading.lock();
	isLoading = isload;
	muLoading.unlock();
	//int angle = 0;

	if (isLoading)
	{
		cv::Mat dest;
		std::vector<CFace> listOfFace;
		pBitmap->VertFlipBuf();
		cv::Mat image(pBitmap->GetBitmapHeight(), pBitmap->GetBitmapWidth(), CV_8UC4, pBitmap->GetPtBitmap());
		cv::cvtColor(image, image, cv::COLOR_BGRA2BGR);
		int angle = DectectOrientationByFaceDetector(image);
		RotateCorrectly(image, dest, angle);

		if(!fastDetection)
			DetectFaceDlib(dest, listOfFace, pointOfFace);
		else
			detectFaceOpenCVDNN(image, listOfFace, pointOfFace);
			   
		if (listOfFace.size() > 0)
		{
			for (int i = 0; i < listOfFace.size(); i++)
			{
				if (listOfFace[i].confidence > confidenceThreshold)
				{


					cv::Mat gray;
					std::vector<cv::Rect> eyes;
					cv::cvtColor(listOfFace[i].croppedImage, gray, COLOR_BGR2GRAY);
					muEyeAccess.lock();
					eye_cascade.detectMultiScale(gray, eyes, 1.3, 6, CASCADE_FIND_BIGGEST_OBJECT, cv::Size(listOfFace[i].croppedImage.rows * 0.1, listOfFace[i].croppedImage.cols * 0.1));
					muEyeAccess.unlock();
#ifndef NDEBUG
					cv::rectangle(image, pointOfFace[i], cv::Scalar(0, 255, 0));
#endif
					for (cv::Rect rect : eyes)
					{
						cv::Rect rectEye;

						rectEye.x = rect.x + pointOfFace[i].x;
						rectEye.y = rect.y + pointOfFace[i].y;
						rectEye.width = rect.width;
						rectEye.height = rect.height;
#ifndef NDEBUG
						cv::rectangle(image, rectEye, cv::Scalar(255, 0, 0));
#endif
						RemoveRedEye(dest, rectEye);
					}
					
				}
			}
		}

		RotateCorrectly(dest, image, (360 - angle) % 360);

		cv::cvtColor(image, image, cv::COLOR_BGR2BGRA);
		pBitmap->SetBitmap(image.data, pBitmap->GetBitmapWidth(), pBitmap->GetBitmapHeight());
		pBitmap->VertFlipBuf();
	}
}

void CFaceDetector::DetectFaceDlib(const cv::Mat& frameOpenCVDNN, std::vector<CFace>& listOfFace, std::vector<cv::Rect>& pointOfFace)
{
	try
	{

		//cv::Mat resized;
		//cv::resize(frameOpenCVDNN, resized, Size(300, 300));
		// Conver OpenCV image Dlib image i.e. cimg
		cv_image<rgb_pixel> cimg(frameOpenCVDNN);

		
		std::vector<dlib::rect_detection> face_DLib = detector(cimg);
		//muDnnAccess.unlock();

		if (face_DLib.empty()) {
			cout << "No face is deteced by DLib" << endl;
		}

		for (dlib::rect_detection rect : face_DLib)
		{
			// Convert DLib Rect to OpenCV Rect

			CFace face;
			face.confidence = min(rect.detection_confidence,1.0);

			cv::Rect R;
			R.x = rect.rect.left();
			R.y = rect.rect.top();
			R.width = rect.rect.width();
			R.height = rect.rect.height();

			face.myROI = R;
			face.croppedImage = frameOpenCVDNN(face.myROI);

			listOfFace.push_back(face);
			pointOfFace.push_back(face.myROI);
		}
	}
	catch (cv::Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}
}

//--------------------------------------------------
//Code From https://github.com/spmallick/learnopencv
//--------------------------------------------------
void CFaceDetector::detectFaceOpenCVDNN(const cv::Mat& frameOpenCVDNN, std::vector<CFace> & listOfFace, std::vector<cv::Rect> & pointOfFace)
{
	int frameHeight = frameOpenCVDNN.rows;
	int frameWidth = frameOpenCVDNN.cols;
#ifdef CAFFE
	Mat resizedFrame;
	cv::resize(frameOpenCVDNN, resizedFrame, cv::Size(300, 300));
	muDnnAccess.lock();
	cv::Mat inputBlob = cv::dnn::blobFromImage(frameOpenCVDNN, 1.0, cv::Size(300, 300), (104.0, 177.0, 123.0), false, false);
	net.setInput(inputBlob);
	auto detection = net.forward();
	muDnnAccess.unlock();

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

			face.myROI = cv::Rect(cv::Point(x1, y1), cv::Point(x2, y2));
			face.croppedImage = frameOpenCVDNN(face.myROI);
			listOfFace.push_back(face);
			pointOfFace.push_back(face.myROI);
		}
	}
	catch (cv::Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}


}

int CFaceDetector::FindNbFace(cv::Mat & image, float &bestConfidence, const float& confidence)
{
	std::vector<cv::Rect> pointOfFace;
	std::vector<CFace> listOfFace;
	int nbFace = 0;
	if (!fastDetection)
		DetectFaceDlib(image, listOfFace, pointOfFace);
	else
		detectFaceOpenCVDNN(image, listOfFace, pointOfFace);

	for (int i = 0; i < listOfFace.size(); i++)
	{
		if (listOfFace[i].confidence > confidence)
		{
			if (listOfFace[i].confidence > bestConfidence)
				bestConfidence = listOfFace[i].confidence;
			nbFace++;
		}
	}

	return nbFace;
}


int CFaceDetector::DetectAngleOrientation(const cv::Mat & image)
{
	int angle = 0;
	cv::Mat dst;      //Mat object for output image file
	int tab[] = { 0, 270, 90, 180 };
	for (int type = 0; type < 4; type++)
	{
		std::vector<cv::Rect> pointOfFace;
		std::vector<CFace> listOfFace;
		int angle_detect = tab[type];
		RotateCorrectly(image, dst, angle_detect);


#ifdef WRITE_OUTPUT_SAMPLE
		wxString file = "d:\\test" + to_string(type) + ".jpg";
		cv::imwrite(file.ToStdString(), dst);
#endif

		if (!fastDetection)
			DetectFaceDlib(image, listOfFace, pointOfFace);
		else
			detectFaceOpenCVDNN(image, listOfFace, pointOfFace);

		if (listOfFace.size() > 0)
		{
			angle = angle_detect;
			break;
		}

	}
	return angle;
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


void CFaceDetector::ImageToJpegBuffer(cv::Mat & image, std::vector<uchar> & buff)
{
	//std::vector<uchar> buff;//buffer for coding
	std::vector<int> param(2);
	param[0] = cv::IMWRITE_JPEG_QUALITY;
	param[1] = 80;//default(95) 0-100
	cv::imencode(".jpg", image, buff, param);
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
#ifndef NDEBUG
	cv::rectangle(image, rSelectionBox, cv::Scalar(0, 255, 0));
#endif
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

