#include <header.h>
#include "FaceDetector.h"
#include <SqlFacePhoto.h>
#include <SqlFaceLabel.h>
#include <RegardsBitmap.h>
#include <opencv2/face/facemark.hpp>
#include "base64.h"
#include <RegardsConfigParam.h>
#include <ParamInit.h>
#include <SqlFaceRecognition.h>
#include <FileUtility.h>
#include <opencv2/face/facerec.hpp>
#include "DetectFace.h"
#include "DetectFacePCN.h"
#include <fstream>
#include <OpenCLContext.h>
#include <opencv2/tracking/tracking_by_matching.hpp>
#include "eyedetect.h"
#include <FileUtility.h>
using namespace Regards::OpenCL;
#define WIDTH_THUMBNAIL 1920
#define HEIGHT_THUMBNAIL 1080

using namespace cv;
using namespace dnn;
using namespace Regards::OpenCV;
using namespace Regards::Sqlite;
using namespace std;
using namespace cv::face;
using namespace std;

extern COpenCLContext* openclContext;

std::map<int, cv::Mat> listScore;

struct FaceValueIntegration
{
	double pertinence;
	int nbValue;
};


static Net netRecognition;
static Net netPosition;
static cv::CascadeClassifier eye_cascade;
std::mutex CFaceDetector::muFaceMark;
static Ptr<face::Facemark> facemark;

const Scalar meanVal(104.0, 177.0, 123.0);
const float confidenceThreshold = 0.59;
bool CFaceDetector::isload = false;
std::mutex CFaceDetector::muLoading;
//std::mutex CFaceDetector::muFaceMark;
//static Ptr<face::Facemark> facemark;

void CFaceDetector::CleanBase()
{
	listScore.clear();
}

CFaceDetector::CFaceDetector(const bool& fastDetection)
{
	detectFacePCN = new CDetectFacePCN();
}

CFaceDetector::~CFaceDetector()
{
	delete detectFacePCN;
}

float CalculPictureRatio(const int& pictureWidth, const int& pictureHeight)
{
	if (pictureWidth == 0 && pictureHeight == 0)
		return 1.0f;

	float new_ratio;

	//int tailleAffichageWidth = 0, tailleAffichageHeight = 0;

	if (pictureWidth > pictureHeight)
		new_ratio = static_cast<float>(WIDTH_THUMBNAIL) / static_cast<float>(pictureWidth);
	else
		new_ratio = static_cast<float>(HEIGHT_THUMBNAIL) / static_cast<float>(pictureHeight);

	if ((pictureHeight * new_ratio) > HEIGHT_THUMBNAIL)
	{
		new_ratio = static_cast<float>(HEIGHT_THUMBNAIL) / static_cast<float>(pictureHeight);
	}
	if ((pictureWidth * new_ratio) > WIDTH_THUMBNAIL)
	{
		new_ratio = static_cast<float>(WIDTH_THUMBNAIL) / static_cast<float>(pictureWidth);
	}

	return new_ratio;
}

int CFaceDetector::DectectOrientationByFaceDetector(CRegardsBitmap* pBitmap)
{
	std::vector<Rect> pointOfFace;
	bool faceFound = false;
	bool isLoading = false;
	muLoading.lock();
	isLoading = isload;
	muLoading.unlock();
	int selectAngle = 0;
	Mat Source;
	pBitmap->GetMatrix().copyTo(Source);
	if (isLoading)
	{
		cv::flip(Source, Source, 0);
		std::vector<CFace> listOfFace;

		detectFacePCN->DetectFace(Source, listOfFace, pointOfFace);

		if (listOfFace.size() > 0)
		{
			for (int i = 0; i < listOfFace.size(); i++)
			{
				if (listOfFace[i].confidence > confidenceThreshold)
				{
					try
					{
						selectAngle = listOfFace[i].angle;
						break;

					}
					catch (Exception& e)
					{
						const char* err_msg = e.what();
						std::cout << "exception caught: " << err_msg << std::endl;
						std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
						faceFound = false;
					}
				}

			}
		}
	}

	return selectAngle;
}

//config.ToStdString(), weight.ToStdString(), eye.ToStdString(), mouth.ToStdString(), recognition.ToStdString()
void CFaceDetector::LoadModel(const string& config_file, const string& weight_file, const string& recognition, const string& face_landmark)
{

	const std::string tensorflowConfigFile = config_file;
	const std::string tensorflowWeightFile = weight_file;

	try
	{
		CDetectFacePCN detectFacePCN;
		CDetectFace detectFace;
#ifndef __WXGTK__
		
		bool openCLCompatible = false;
		CRegardsConfigParam* config = CParamInit::getInstance();
		if (config != nullptr)
		{
			if (config->GetIsOpenCLSupport())
				openCLCompatible = true;
		}

		netRecognition = readNetFromTorch(recognition);
		netRecognition.setPreferableBackend(DNN_BACKEND_DEFAULT);

		if (openCLCompatible)
			netRecognition.setPreferableTarget(DNN_TARGET_OPENCL);
		else
			netRecognition.setPreferableTarget(DNN_TARGET_CPU);
#else

		netRecognition = cv::dnn::readNetFromTorch(recognition);
		netRecognition.setPreferableBackend(DNN_BACKEND_DEFAULT);
		netRecognition.setPreferableTarget(DNN_TARGET_CPU);
#endif

		facemark = face::createFacemarkKazemi();
		facemark->loadModel(face_landmark);

		wxString fileEye = CFileUtility::GetResourcesFolderPath() + "\\model\\haarcascade_eye.xml";
		eye_cascade.load(fileEye.ToStdString());
		detectFace.LoadModel();
		detectFacePCN.LoadModel();
		cout << "Loaded model" << endl;
	}
	catch (Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}

	isload = true;
}


cv::Point2f rotatePointUsingTransformationMat(const cv::Point2f& inPoint, const cv::Point2f& center, const double& rotAngle)
{
	double angRad = rotAngle * (CV_PI / 180);
	cv::Point2f outPoint;
	//CW rotation
	outPoint.x = std::cos(angRad) * inPoint.x - std::sin(angRad) * inPoint.y;
	outPoint.y = std::sin(angRad) * inPoint.x + std::cos(angRad) * inPoint.y;
	return outPoint;
}

Mat CFaceDetector::RotateAndExtractFace(const double& theta_deg_eye, const Rect& faceLocation, Mat image)
{
	//Mat image = pBitmap->GetMatrix();
	Mat dst;
	// Rotate around the center
	auto center = Point2d(faceLocation.x + faceLocation.width / 2, faceLocation.y + faceLocation.height / 2);


	// determine bounding rectangle
	Rect bbox = RotatedRect(center, Size(faceLocation.width, faceLocation.height), theta_deg_eye).boundingRect();

	bbox.x = max(bbox.x, 0);
	bbox.y = max(bbox.y, 0);
	bbox.width = max(bbox.width, 0);
	bbox.height = max(bbox.height, 0);
	dst = image(bbox);


	center = Point2d(dst.cols / 2, dst.rows / 2);
	Mat r = getRotationMatrix2D(center, theta_deg_eye, 1.0);
	// adjust transformation matrix
	r.at<double>(0, 2) += bbox.width / 2.0 - center.x;
	r.at<double>(1, 2) += bbox.height / 2.0 - center.y;

	// Apply affine transform
	warpAffine(dst, dst, r, bbox.size());

	Rect rect;
	rect.x = max((bbox.width - faceLocation.width) / 2, 0);
	rect.y = max((bbox.height - faceLocation.height) / 2, 0);
	rect.width = max(faceLocation.width, 0);
	rect.height = max(faceLocation.height, 0);
	dst = dst(rect);

	r.release();

	return dst;
}

std::vector<cv::Rect> CFaceDetector::GetRectFace(CRegardsBitmap * picture)
{
	std::vector<cv::Rect> listFace;
	bool isLoading = false;
	muLoading.lock();
	isLoading = isload;
	muLoading.unlock();

	if (isLoading)
	{
		std::vector<CFace> listOfFace;
		picture->VertFlipBuf();
		detectFacePCN->DetectFace(picture->GetMatrix(), listOfFace, listFace);
	}

	return listFace;
}



std::vector<int> CFaceDetector::FindFace(CRegardsBitmap* pBitmap)
{
	std::vector<int> listFace;
	int i = 0;
	bool isLoading = false;
	muLoading.lock();
	isLoading = isload;
	muLoading.unlock();

	if (isLoading)
	{
		CSqlFacePhoto facePhoto;
		Mat dest, source;
		std::vector<CFace> listOfFace;
		std::vector<Rect> pointOfFace;

		cv::flip(pBitmap->GetMatrix(), source, 0);
		detectFace->DetectFace(source, confidenceThreshold, listOfFace, pointOfFace);
		cvtColor(pBitmap->GetMatrix(), source, COLOR_BGRA2BGR);
		for (CFace face : listOfFace)
		{
			if (face.confidence > confidenceThreshold)
			{
				Mat resizedImage;
				Size size(150, 150);
				int angle = 0;

				if(listOfFace.size() == 1)
					angle = detectFacePCN->DetectFaceAngle(source);
				else
					angle = detectFacePCN->DetectFaceAngle(face.croppedImage);
				
				try
				{
					cv::flip(pBitmap->GetMatrix(), source, 0);
					cv::Mat dst;
									
					try
					{
						dst = RotateAndExtractFace(angle, face.myROI, source);
					}
					catch (Exception& e)
					{
						dst = source(face.myROI);


					}

					
					Mat localFace;
 					std::vector<uchar> buff;
					resize(dst, localFace, size);
					ImageToJpegBuffer(localFace, buff);
					int numFace = facePhoto.InsertFace(pBitmap->GetFilename(), ++i, face.croppedImage.rows,
						                                face.croppedImage.cols, face.confidence, buff.data(),
						                                buff.size());

					listFace.push_back(numFace);
					face.croppedImage.release();
				}
				catch (Exception& e)
				{
					const char* err_msg = e.what();
					std::cout << "exception caught: " << err_msg << std::endl;
					std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
				}

				resizedImage.release();
			}

			if (listFace.size() == 0)
			{
				cout << "No faces found in image!" << endl;
				return listFace;
			}
		}
		dest.release();
	}

	return listFace;
}


void CFaceDetector::DetectEyes(CRegardsBitmap* pBitmap)
{
	std::vector<Rect> pointOfFace;
	bool faceFound = false;
	bool isLoading = false;
	muLoading.lock();
	isLoading = isload;
	muLoading.unlock();

	Mat Source;	
	pBitmap->GetMatrix().copyTo(Source);
	if (isLoading)
	{
		cvtColor(Source, Source, COLOR_BGRA2BGR);
		cv::flip(Source, Source, 0);
		std::vector<CFace> listOfFace;
		detectFacePCN->DetectFace(Source, listOfFace, pointOfFace);

		if (listOfFace.size() > 0)
		{
			for (int i = 0; i < listOfFace.size(); i++)
			{
				if (listOfFace[i].confidence > confidenceThreshold)
				{
					try
					{
						Mat face = listOfFace[i].croppedImage;
						vector<Rect> faces;
						Mat faceColor;
						Point2f center22(face.cols / 2.0, face.rows / 2.0);
						Mat rot = getRotationMatrix2D(center22, listOfFace[i].angle, 1.0);
						// determine bounding rectangle
						Rect bbox = RotatedRect(center22, face.size(), listOfFace[i].angle).boundingRect();
						// adjust transformation matrix
						rot.at<double>(0, 2) += bbox.width / 2.0 - center22.x;
						rot.at<double>(1, 2) += bbox.height / 2.0 - center22.y;

						warpAffine(face, faceColor, rot, bbox.size());
						Rect rc = { 0,0, faceColor.size().width, faceColor.size().height};
						Mat gray;
						cvtColor(faceColor, gray, COLOR_BGR2GRAY);

						int radiusL = 40;
						int radiusR = 40;

						faces.push_back(rc);
					
						std::vector<cv::Rect> eyes;
						eye_cascade.detectMultiScale(gray, eyes, 1.1, 5);// , 0 | CASCADE_SCALE_IMAGE, cv::Size(20, 20));

						if (eyes.size() == 2)
						{
							for (int p = 0; p < eyes.size(); p++)
							{
								Rect rect = eyes[p];
								if (rect.y > (face.size().height / 2))
								{
									eyes.erase(eyes.begin() + p);
									p--;
								}
							}

							for (int k = 0; k < eyes.size(); k++)
							{
								for (int j = 0; j < eyes.size(); j++)
								{
									if (k != j)
									{
										Rect interesect = eyes[k] & eyes[j];
										if (interesect.area() > 0)
										{
											eyes.erase(eyes.begin() + j);
											j--;
										}
									}
								}
							}

							vector<vector<Point2f>> shapes;

							try
							{
								muFaceMark.lock();
								facemark->fit(faceColor, faces, shapes);
								muFaceMark.unlock();
							}
							catch (Exception& e)
							{
								const char* err_msg = e.what();
								std::cout << "exception caught: " << err_msg << std::endl;
								std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
							}

							std::vector<Point2f> landmarks = shapes[0];
							
							std::vector<Point2f> _veyeLeft;
							for(int p = 37;p < 42;p++)
								_veyeLeft.push_back(landmarks[p]);

							Rect eyeLeft = boundingRect(_veyeLeft);

							std::vector<Point2f> _veyeRight;
							for (int p = 43; p < 48; p++)
								_veyeRight.push_back(landmarks[p]);

							Rect eyeRight = boundingRect(_veyeRight);

							RemoveRedEye(faceColor, eyes[0], eyeLeft);
							RemoveRedEye(faceColor, eyes[1], eyeRight);

							{
								Point2f center22(faceColor.cols / 2.0, faceColor.rows / 2.0);
								Mat rot = getRotationMatrix2D(center22, 360 - listOfFace[i].angle, 1.0);
								// determine bounding rectangle
								Rect bbox = RotatedRect(center22, faceColor.size(), 360 - listOfFace[i].angle).boundingRect();
								// adjust transformation matrix
								rot.at<double>(0, 2) += bbox.width / 2.0 - center22.x;
								rot.at<double>(1, 2) += bbox.height / 2.0 - center22.y;

								warpAffine(faceColor, faceColor, rot, bbox.size());

								rc = { (bbox.width - listOfFace[i].myROI.width) / 2, (bbox.height - listOfFace[i].myROI.height) / 2,listOfFace[i].myROI.width, listOfFace[i].myROI.height };
								rc.x += 1;
								rc.y += 1;
								rc.width -= 2;
								rc.height -= 2;
								faceColor = faceColor(rc);
							}

							listOfFace[i].myROI.x += 1;
							listOfFace[i].myROI.y += 1;
							listOfFace[i].myROI.width -= 2;
							listOfFace[i].myROI.height -= 2;
							cv::cvtColor(faceColor, faceColor, COLOR_BGR2BGRA);
							faceColor.copyTo(Source(listOfFace[i].myROI));
							faceFound = true;
						}

					}
					catch (Exception& e)
					{
						const char* err_msg = e.what();
						std::cout << "exception caught: " << err_msg << std::endl;
						std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
					}
				}
				
			}
		}

		if (faceFound)
		{
			cv::flip(Source, Source, 0);
			cvtColor(Source, pBitmap->GetMatrix(), COLOR_BGR2BGRA);
		}
	}
}

void CFaceDetector::ImageToJpegBuffer(Mat& image, std::vector<uchar>& buff)
{
	//std::vector<uchar> buff;//buffer for coding
	std::vector<int> param(2);
	param[0] = IMWRITE_JPEG_QUALITY;
	param[1] = 80; //default(95) 0-100
	imencode(".jpg", image, buff, param);
}

void CFaceDetector::RemoveRedEye(Mat& image, const Rect& rSelectionBox, const Rect& radius)
{
	Mat eyeMat = image(rSelectionBox);

	Mat eye;
	cvtColor(eyeMat, eye, COLOR_BGR2GRAY);
	GaussianBlur(eye, eye, Size(5, 5), 0);
	double min = 0;
	double max = 0;
	Point minLoc;
	double min_val, max_val;
	Point maxLoc;
	minMaxLoc(eye, &min_val, &max_val, &minLoc, &maxLoc);

	Rect rc;
	rc.x = maxLoc.x - radius.width / 2;
	rc.y = maxLoc.y - radius.height / 2;
	rc.width = radius.width;
	rc.height = radius.height;

	int xMax = eyeMat.size().width;
	int yMax = eyeMat.size().height;

	if (rc.x + rc.width > xMax)
		rc.width = xMax - rc.x;

	if (rc.y + rc.height > yMax)
		rc.height = yMax - rc.y;

	Mat iris2 = eyeMat(rc);
	Mat img_gray;
	cvtColor(iris2, img_gray, COLOR_BGR2GRAY);
	Mat thresh;
	threshold(img_gray, thresh, 100, 255, THRESH_BINARY);
	bitwise_not(thresh, thresh);
	cvtColor(img_gray, img_gray, COLOR_GRAY2BGR);
	iris2.copyTo(img_gray, thresh);
	img_gray.copyTo(eyeMat(rc));
}

/**
 * This module is using to computing the cosine distance between input feature and ground truth feature
 */
inline float CosineDistance(const cv::Mat& v1, const cv::Mat& v2) {
	double dot = v1.dot(v2);
	double denom_v1 = norm(v1);
	double denom_v2 = norm(v2);

	return dot / (denom_v1 * denom_v2);

}


Mat CFaceDetector::Zscore(const Mat& fc) {
	Mat mean, std;
	meanStdDev(fc, mean, std);
	//    cout << mean << std << endl;
	Mat fc_norm = (fc - mean) / std;
	return fc_norm;

}

Mat CFaceDetector::GetFaceScore(const int& numFace)
{
	Mat fc1;
	try
	{
		Mat face1 = imread(CFileUtility::GetFaceThumbnailPath(numFace).ToStdString());
		Mat face1Vec = FaceDesriptor(face1);
		fc1 = Zscore(face1Vec);
		return fc1;
	
	}
	catch (Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
#ifdef WIN32
		wxString error = err_msg;
		OutputDebugString(error.ToStdWstring().c_str());
#endif
	}

	return fc1;
}

Mat CFaceDetector::FaceDesriptor(Mat face)
{
	Mat result;
	try
	{
		if (openclContext != nullptr)
			openclContext->GetContextForOpenCV().bind();

		Mat blob = blobFromImage(face, 1.0 / 255, Size(96, 96), Scalar(0, 0, 0, 0), true, false);
		netRecognition.setInput(blob);
		result = netRecognition.forward().clone();
		blob.release();
	}
	catch (Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
#ifdef WIN32
		wxString error = err_msg;
		OutputDebugString(error.ToStdWstring().c_str());
#endif
	}
	return result;
}

int CFaceDetector::FaceRecognition(const int& numFace)
{
	//int predictedLabel = -1;
	double maxConfidence = 0.0;
	Mat fc1;
	bool findFaceCompatible = false;
	CSqlFacePhoto facePhoto;
	CSqlFaceRecognition sqlfaceRecognition;
	vector<CFaceRecognitionData> faceRecognitonVec = facePhoto.GetAllNumFaceRecognition();

	if(!wxFileExists(CFileUtility::GetFaceThumbnailPath(numFace).ToStdString()))
	{
		return 0;
	}

	fc1 = GetFaceScore(numFace);
	if (fc1.rows == 0 || fc1.cols == 0)
		return false;
	//fc1 = imread(CFileUtility::GetFaceThumbnailPath(numFace).ToStdString(),0);

	if (faceRecognitonVec.size() > 0)
	{
		
		int predictedLabel = -1;
		double confidence = 0.0; 

		for (CFaceRecognitionData picture : faceRecognitonVec)
		{
			Mat face1;
			if (listScore.find(picture.numFace) == listScore.end())
			{
				face1 = GetFaceScore(picture.numFace);
				listScore.insert(std::pair<int,Mat>(picture.numFace, face1));
			}
			else
			{
				face1 = listScore[picture.numFace];
			}
			
			if (face1.rows == 0 || face1.cols == 0)
				return false;

			cv::detail::tracking::tbm::CosDistance cosDistance(fc1.size());
			float score = 1.0 - cosDistance.compute(fc1, face1);
			//float score = CosineDistance(fc1, face1);
			if (score > confidence)
			{
				confidence = score;
				predictedLabel = picture.numFaceCompatible;
			}
		}

		if (predictedLabel != -1 && confidence > 0.90)
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
