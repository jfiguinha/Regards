#include "header.h"
#include "FaceDetector.h"
#include <SqlFaceDescriptor.h>
#include <SqlFacePhoto.h>
#include <SqlFaceLabel.h>
#include <RegardsBitmap.h>
#include <opencv2/face/facemark.hpp>
#include "base64.h"
#include <RegardsConfigParam.h>
#include <ParamInit.h>
#include <SqlFaceRecognition.h>
#include <FileUtility.h>
#define WIDTH_THUMBNAIL 1920
#define HEIGHT_THUMBNAIL 1080

using namespace cv;
using namespace cv::dnn;
using namespace Regards::OpenCV;
using namespace Regards::Sqlite;
using namespace std;

#define CAFFE
//#define WRITE_OUTPUT_SAMPLE


static Net net;    // And finally we load the DNN responsible for face recognition.
static Net netRecognition;

const float confidence = 0.5;
const float confidenceThreshold = 0.7;
const cv::Scalar meanVal(104.0, 177.0, 123.0);

bool CFaceDetector::isload = false;
std::mutex CFaceDetector::muLoading;
std::mutex CFaceDetector::muDnnAccess;
std::mutex CFaceDetector::muFaceMark;
string CFaceDetector::eyeCascadeFile;
static Ptr<cv::face::Facemark> facemark;

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
	//this->fastDetection = fastDetection;
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
//config.ToStdString(), weight.ToStdString(), eye.ToStdString(), mouth.ToStdString(), recognition.ToStdString()
void CFaceDetector::LoadModel(const string &config_file, const string &weight_file, const string & eye_detection, const string& recognition, const string& face_landmark)
{
#ifdef CAFFE
	const std::string caffeConfigFile = config_file;//"C:\\developpement\\git_gcc\\Rotnet\\Rotnetcpp\\model\\deploy.prototxt";
	const std::string caffeWeightFile = weight_file;// "C:\\developpement\\git_gcc\\Rotnet\\Rotnetcpp\\model\\res10_300x300_ssd_iter_140000_fp16.caffemodel";
#else
	const std::string tensorflowConfigFile = config_file;// "C:\\developpement\\git_gcc\\Rotnet\\Rotnetcpp\\model\\opencv_face_detector.pbtxt";
	const std::string tensorflowWeightFile = weight_file;// "C:\\developpement\\git_gcc\\Rotnet\\Rotnetcpp\\model\\opencv_face_detector_uint8.pb";
#endif

	eyeCascadeFile = eye_detection;

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

#ifndef __WXGTK__
		bool openCLCompatible = false;
		CRegardsConfigParam * config = CParamInit::getInstance();
		if (config != nullptr)
		{
			if (config->GetIsOpenCLSupport())
				openCLCompatible = config->GetFaceOpenCLProcess();
		}

		net = cv::dnn::readNetFromCaffe(caffeConfigFile, caffeWeightFile);
		net.setPreferableBackend(DNN_BACKEND_DEFAULT);
		if (openCLCompatible)
			net.setPreferableTarget(DNN_TARGET_OPENCL);
		else
			net.setPreferableTarget(DNN_TARGET_CPU);

		netRecognition = cv::dnn::readNetFromTorch(recognition);
		netRecognition.setPreferableBackend(DNN_BACKEND_DEFAULT);
		if (openCLCompatible)
			netRecognition.setPreferableTarget(DNN_TARGET_OPENCL);
		else
			netRecognition.setPreferableTarget(DNN_TARGET_CPU);


		facemark = cv::face::createFacemarkKazemi();
		facemark->loadModel(face_landmark);
		cout << "Loaded model" << endl;
#else
   		net = cv::dnn::readNetFromCaffe(caffeConfigFile, caffeWeightFile);
		net.setPreferableBackend(DNN_BACKEND_DEFAULT);
		net.setPreferableTarget(DNN_TARGET_CPU);

		netRecognition = cv::dnn::readNetFromTorch(recognition);
		netRecognition.setPreferableBackend(DNN_BACKEND_DEFAULT);
		netRecognition.setPreferableTarget(DNN_TARGET_CPU);
#endif

#else
		net = cv::dnn::readNetFromTensorflow(tensorflowWeightFile, tensorflowConfigFile);
#endif


	}
	catch (cv::Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}

	isload = true;

}

double CFaceDetector::face_opencv_alignement(cv::Mat& image, bool& findEye) {

	double theta_deg_eye = 0;
	vector<Rect> faces;
	int angle_add = 0;
	//Declaring a variable "image" to store input image given.
	Mat gray, detected_edges, Laugh_L, Laugh_R;

	//converts original image to gray scale and stores it in "gray".
	cvtColor(image, gray, COLOR_BGR2GRAY);

	Rect rc;
	rc.x = 0;
	rc.y = 0;
	rc.width = image.cols;
	rc.height = image.rows;

	faces.push_back(rc);

	//Histogram equalization is performed on the resized image to improve the contrast of the image which can help in detection.
	equalizeHist(gray, gray);
	bool faceFound = false;
	std::vector<cv::Point2f> landmarks, R_Eyebrow, L_Eyebrow, L_Eye, R_Eye, Mouth, Jaw_Line, Nose;
	vector<vector<Point2f>> shapes;

	muFaceMark.lock();
	if (facemark->fit(image, faces, shapes))
	{
		faceFound = true;
	}
	muFaceMark.unlock();

	if (faceFound)
	{
		landmarks = shapes[0];
		/*
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
		*/
		int posLeftEyeY = 0;
		int posRightEyeY = 0;
		int posLeftEyeX = 0;
		int posRightEyeX = 0;
		for (int i = 0; i < 7; i++)
		{
			posLeftEyeY += landmarks[i + 36].y;
			posRightEyeY += landmarks[i + 42].y;
			posLeftEyeX += landmarks[i + 36].x;
			posRightEyeX += landmarks[i + 42].x;
		}
		posLeftEyeY /= 8;
		posRightEyeY /= 8;
		posLeftEyeX /= 8;
		posRightEyeX /= 8;

		//double rot_eye = atan2(posRightEyeY - posLeftEyeY, posRightEyeX - posLeftEyeX);
		double rot_eye = atan2(landmarks[45].y - landmarks[36].y, landmarks[45].x - landmarks[36].x);
		
		if (posLeftEyeY > (rc.height / 2) && posRightEyeY > (rc.height / 2))
			angle_add += 180;
	
		theta_deg_eye = rot_eye / M_PI * 180 + angle_add;
	}
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

		detectFaceOpenCVDNN(image, listOfFace, pointOfFace);

		for (CFace face : listOfFace)
		{
			if (face.confidence > 0.69)
			{
				double angleRot = 0;
				cv::Mat resizedImage;
				bool findEye = true;
				cv::Size size(150, 150);
				cv::resize(face.croppedImage, resizedImage, size);

				angleRot = face_opencv_alignement(resizedImage, findEye);


				if (findEye)
				{
					try
					{
						face.croppedImage = RotateAndExtractFace(angleRot, face.myROI, image);
						std::vector<uchar> buff;
						cv::resize(face.croppedImage, face.croppedImage, size);
						ImageToJpegBuffer(face.croppedImage, buff);
						int numFace = facePhoto.InsertFace(pBitmap->GetFilename(), ++i, face.croppedImage.rows, face.croppedImage.cols, face.confidence, reinterpret_cast<uchar*>(buff.data()), buff.size());
						listFace.push_back(numFace);
					}
					catch (cv::Exception& e)
					{
						const char* err_msg = e.what();
						std::cout << "exception caught: " << err_msg << std::endl;
						std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
					}
				}
			}

			if (listFace.size() == 0)
			{
				cout << "No faces found in image!" << endl;
				return listFace;
			}
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
		cv::CascadeClassifier eye_cascade;
		cv::Mat dest;
		std::vector<CFace> listOfFace;
		pBitmap->VertFlipBuf();
		cv::Mat image(pBitmap->GetBitmapHeight(), pBitmap->GetBitmapWidth(), CV_8UC4, pBitmap->GetPtBitmap());
		cv::cvtColor(image, image, cv::COLOR_BGRA2BGR);
		int angle = DectectOrientationByFaceDetector(image);
		RotateCorrectly(image, dest, angle);

		eye_cascade.load(eyeCascadeFile);
		detectFaceOpenCVDNN(image, listOfFace, pointOfFace);
			   
		if (listOfFace.size() > 0)
		{
			for (int i = 0; i < listOfFace.size(); i++)
			{
				if (listOfFace[i].confidence > confidenceThreshold)
				{
					vector<Rect> faces;
					int angle_add = 0;
					//Declaring a variable "image" to store input image given.
					Mat gray, detected_edges, Laugh_L, Laugh_R;

					//converts original image to gray scale and stores it in "gray".
					cvtColor(listOfFace[i].croppedImage, gray, COLOR_BGR2GRAY);

					Rect rc;
					rc.x = 0;
					rc.y = 0;
					rc.width = image.cols;
					rc.height = image.rows;

					faces.push_back(rc);

					//Histogram equalization is performed on the resized image to improve the contrast of the image which can help in detection.
					equalizeHist(gray, gray);
					bool faceFound = false;
					std::vector<cv::Point2f> landmarks, R_Eyebrow, L_Eyebrow, L_Eye, R_Eye, Mouth, Jaw_Line, Nose;
					vector<vector<Point2f>> shapes;

					muFaceMark.lock();
					if (facemark->fit(image, faces, shapes))
					{
						faceFound = true;
					}
					muFaceMark.unlock();

					if (faceFound)
					{
						landmarks = shapes[0];

						int posMinLeftEyeY = 0;
						int posMaxLeftEyeY = 0;
						int posMaxRightEyeY = 0;
						int posMinRightEyeY = 0;

						int posMinLeftEyeX = 0;
						int posMaxLeftEyeX = 0;
						int posMinRightEyeX = 0;
						int posMaxRightEyeX = 0;
						for (int i = 0; i < 7; i++)
						{
							if (posMinLeftEyeY > landmarks[i + 36].y)
								posMinLeftEyeY = landmarks[i + 36].y;

							if (posMaxLeftEyeY < landmarks[i + 36].y)
								posMaxLeftEyeY = landmarks[i + 36].y;

							if (posMinRightEyeY > landmarks[i + 42].y)
								posMinRightEyeY = landmarks[i + 42].y;

							if (posMaxRightEyeY < landmarks[i + 42].y)
								posMaxRightEyeY = landmarks[i + 42].y;

							if (posMinLeftEyeX > landmarks[i + 36].x)
								posMinLeftEyeX = landmarks[i + 36].x;

							if (posMaxLeftEyeX < landmarks[i + 36].x)
								posMaxLeftEyeX = landmarks[i + 36].x;

							if (posMinRightEyeX > landmarks[i + 42].x)
								posMinRightEyeX = landmarks[i + 42].x;

							if (posMaxRightEyeX < landmarks[i + 42].x)
								posMaxRightEyeX = landmarks[i + 42].x;

						}

						//Left Eye
						{
							cv::Rect rectEye;
							rectEye.x = posMinLeftEyeX + pointOfFace[i].x;
							rectEye.y = posMinLeftEyeY + pointOfFace[i].y;
							rectEye.width = posMaxLeftEyeX - posMinLeftEyeX;
							rectEye.height = posMaxLeftEyeY - posMinLeftEyeY;
							RemoveRedEye(dest, rectEye);
						}

						//Right Eye
						{
							cv::Rect rectEye;
							rectEye.x = posMinRightEyeX + pointOfFace[i].x;
							rectEye.y = posMinRightEyeY + pointOfFace[i].y;
							rectEye.width = posMaxRightEyeX - posMinRightEyeX;
							rectEye.height = posMaxRightEyeY - posMinRightEyeY;
							RemoveRedEye(dest, rectEye);
						}
					}


					/*
					cv::Mat gray;
					std::vector<cv::Rect> eyes;
					cv::cvtColor(listOfFace[i].croppedImage, gray, COLOR_BGR2GRAY);
					eye_cascade.detectMultiScale(gray, eyes, 1.3, 5, CASCADE_FIND_BIGGEST_OBJECT, cv::Size(listOfFace[i].croppedImage.rows * 0.1, listOfFace[i].croppedImage.cols * 0.1));
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
					*/
				}
			}
		}

		RotateCorrectly(dest, image, (360 - angle) % 360);

		cv::cvtColor(image, image, cv::COLOR_BGR2BGRA);
		pBitmap->SetBitmap(image.data, pBitmap->GetBitmapWidth(), pBitmap->GetBitmapHeight());
		pBitmap->VertFlipBuf();
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
	muDnnAccess.lock();
	cv::Mat inputBlob = cv::dnn::blobFromImage(frameOpenCVDNN, 1.0, cv::Size(300, 300) , (104.0, 177.0, 123.0), false, false);
	net.setInput(inputBlob);
	auto detection = net.forward().clone();
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


Mat eval(Mat face) {
	Mat result;
	try
	{
		Mat blob = blobFromImage(face, 1.0 / 255, Size(96, 96), Scalar(0, 0, 0, 0), true, false);
		netRecognition.setInput(blob);
		result = netRecognition.forward().clone();
	}
	catch (cv::Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
		wxString error = err_msg;
		OutputDebugString(error.ToStdWstring().c_str());
	}
	return result;
}


int CFaceDetector::FaceRecognition(const int& numFace)
{
	int predictedLabel = -1;
	double maxConfidence = 0.0;

	bool findFaceCompatible = false;
	CSqlFacePhoto facePhoto;
	CSqlFaceRecognition sqlfaceRecognition;
	vector<CFaceRecognitionData> faceRecognitonVec = facePhoto.GetAllNumFaceRecognition();
	Mat face1 = imread(CFileUtility::GetFaceThumbnailPath(numFace).ToStdString());
	Mat face1Vec = eval(face1);

	if (faceRecognitonVec.size() > 0)
	{
		for (CFaceRecognitionData picture : faceRecognitonVec)
		{
			Mat face2 = imread(CFileUtility::GetFaceThumbnailPath(picture.numFace).ToStdString());
			Mat face2Vec = eval(face2);
			cout << "Face2Vec" << face2Vec << endl;
			double confidence = face1Vec.dot(face2Vec);
			//confidence = GetSimilarity(imageSrc, image);
			if (maxConfidence < confidence)
			{
				predictedLabel = picture.numFace;
				maxConfidence = confidence;
			}

		}


		if (maxConfidence > 0.6)
		{
			sqlfaceRecognition.InsertFaceRecognition(numFace, predictedLabel);
			findFaceCompatible = true;
		}

		if (!findFaceCompatible)
		{
			CSqlFaceLabel sqlfaceLabel;
			wxString label = "Face number " + to_string(numFace);
			sqlfaceRecognition.InsertFaceRecognition(numFace, numFace);
			sqlfaceLabel.InsertFaceLabel(numFace, label, true);
		}
	}
	else
	{
		CSqlFaceLabel sqlfaceLabel;
		wxString label = "Face number " + to_string(numFace);
		sqlfaceRecognition.InsertFaceRecognition(numFace, numFace);
		sqlfaceLabel.InsertFaceLabel(numFace, label, true);
	}

	return findFaceCompatible;
}
