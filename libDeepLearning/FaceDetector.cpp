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
	//detectFace = new CDetectFace();
	detectFacePCN = new CDetectFacePCN();
}

CFaceDetector::~CFaceDetector()
{
	//delete detectFace;
	delete detectFacePCN;
}

void CFaceDetector::RotateOpenCV(const float& angle, int& maxFace, float& confidence, int& selectAngle, CRegardsBitmap* pBitmap)
{
	float bestConfidence = 0;
	RotateCorrectly(pBitmap, angle);
	int nbFace = FindNbFace(pBitmap, bestConfidence, confidenceThreshold);
	if (nbFace > 0 && (nbFace > maxFace || bestConfidence > confidence))
	{
		maxFace = nbFace;
		selectAngle = angle;
		confidence = bestConfidence;
		bestConfidence = 0.0;
	}

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

	int angle = 0;
	int selectAngle = 0;
	int maxFace = 0;
	float confidence = 0;
	RotateOpenCV(angle, maxFace, confidence, selectAngle, pBitmap);
	angle += 90;
	RotateOpenCV(angle, maxFace, confidence, selectAngle, pBitmap);
	angle += 90;
	RotateOpenCV(angle, maxFace, confidence, selectAngle, pBitmap);
	angle += 90;
	RotateOpenCV(angle, maxFace, confidence, selectAngle, pBitmap);

	return selectAngle;
}

//config.ToStdString(), weight.ToStdString(), eye.ToStdString(), mouth.ToStdString(), recognition.ToStdString()
void CFaceDetector::LoadModel(const string& config_file, const string& weight_file, const string& recognition,
                              const string& face_landmark, const string &protoPosition, const string& weightPosition)
{

#ifdef CAFFE
	const std::string caffeConfigFile = config_file;//"C:\\developpement\\git_gcc\\Rotnet\\Rotnetcpp\\model\\deploy.prototxt";
	const std::string caffeWeightFile = weight_file;// "C:\\developpement\\git_gcc\\Rotnet\\Rotnetcpp\\model\\res10_300x300_ssd_iter_140000_fp16.caffemodel";
#else
	const std::string tensorflowConfigFile = config_file;
	// "C:\\developpement\\git_gcc\\Rotnet\\Rotnetcpp\\model\\opencv_face_detector.pbtxt";
	const std::string tensorflowWeightFile = weight_file;
	// "C:\\developpement\\git_gcc\\Rotnet\\Rotnetcpp\\model\\opencv_face_detector_uint8.pb";
#endif
	try
	{
		CDetectFacePCN detectFacePCN;
       // CDetectFace detectFace;
        
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

		//detectFace.LoadModel(config_file, weight_file);
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

#ifdef OLD
double CFaceDetector::face_opencv_alignement(Mat& image, bool& findEye)
{
	double theta_deg_eye = 0;
	vector<Rect> faces;
	int angle_add = 0;
	//Declaring a variable "image" to store input image given.
	//Mat detected_edges, Laugh_L, Laugh_R;
	Mat gray;
	image.copyTo(gray);
	//cvtColor(image, gray, COLOR_BGR2GRAY);

	Rect rc;
	rc.x = 0;
	rc.y = 0;
	rc.width = image.cols;
	rc.height = image.rows;

	faces.push_back(rc);

	bool faceFound = false;
	std::vector<Point2f> landmarks, R_Eyebrow, L_Eyebrow, L_Eye, R_Eye, Mouth, Jaw_Line, Nose;
	vector<vector<Point2f>> shapes;

	int i = 0;
	do
	{
		try
		{
			muFaceMark.lock();
			if (facemark->fit(gray, faces, shapes))
			{
				faceFound = true;
			}
			muFaceMark.unlock();
		}
		catch (Exception& e)
		{
			const char* err_msg = e.what();
			std::cout << "exception caught: " << err_msg << std::endl;
			std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
			faceFound = false;
		}

		if (faceFound)
			break;

		cv::rotate(gray, gray, cv::ROTATE_90_CLOCKWISE);

		i++;

	} while (i < 4);

	if (faceFound)
	{
		landmarks = shapes[0];
		//face::drawFacemarks(gray, landmarks, Scalar(0, 0, 255));
		/*
		for (size_t s = 0; s < landmarks.size(); s++) {

			circle(gray,landmarks[s], 2.0, Scalar( 255,0,0 ), 1, 8 );
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
		for (int i = 0; i < 6; i++)
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

		int posMouthY = 0;
		for (int i = 48; i < 68; i++)
			posMouthY += landmarks[i].y;

		posMouthY /= 20;

		double rot_eye = atan2(posRightEyeY - posLeftEyeY, posRightEyeX - posLeftEyeX);
		//double rot_eye = atan2(landmarks[45].y - landmarks[36].y, landmarks[45].x - landmarks[36].x);

		if (posLeftEyeY > posMouthY && posRightEyeY > posMouthY)
			angle_add += 180;

		theta_deg_eye = rot_eye / M_PI * 180 + angle_add;

		//imwrite("d:\\test.jpg", gray);
	}
	return theta_deg_eye;
}
#endif

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
		detectFacePCN->DetectFace(picture, listOfFace, listFace);
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
		pBitmap->VertFlipBuf();
		CSqlFacePhoto facePhoto;
		Mat dest;
		std::vector<CFace> listOfFace;
		std::vector<Rect> pointOfFace;

		//detectFace->DetectFace(pBitmap, confidenceThreshold, listOfFace, pointOfFace);
		detectFacePCN->DetectFace(pBitmap, listOfFace, pointOfFace);

		Mat Source;
		pBitmap->GetMatrix().copyTo(Source);
		
		for (CFace face : listOfFace)
		{
			if (face.confidence > confidenceThreshold)
			{

				Mat resizedImage;
				Size size(150, 150);

				try
				{

					face.croppedImage = RotateAndExtractFace(face.angle, face.myROI, Source);
 					std::vector<uchar> buff;
					resize(face.croppedImage, face.croppedImage, size);
					cv::Mat localFace = face.croppedImage;
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
	pBitmap->VertFlipBuf();
	pBitmap->GetMatrix().copyTo(Source);
	Mat & dest = pBitmap->GetMatrix();
	if (isLoading)
	{
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
						vector<Rect> faces;
						
						Mat face = listOfFace[i].croppedImage;
						int angle = listOfFace[i].angle;

						Mat faceColor;
						Point2f center22(face.cols / 2.0, face.rows / 2.0);
						Mat rot = getRotationMatrix2D(center22, angle, 1.0);
						// determine bounding rectangle
						Rect bbox = RotatedRect(center22, face.size(), angle).boundingRect();
						// adjust transformation matrix
						rot.at<double>(0, 2) += bbox.width / 2.0 - center22.x;
						rot.at<double>(1, 2) += bbox.height / 2.0 - center22.y;

						warpAffine(face, faceColor, rot, bbox.size());
						Rect rc = { 0,0, faceColor.size().width, faceColor.size().height};
						Mat gray;
						cvtColor(faceColor, gray, COLOR_BGR2GRAY);

						faces.push_back(rc);

						bool faceFound = true;
						std::vector<Point2f> landmarks, R_Eyebrow, L_Eyebrow, L_Eye, R_Eye, Mouth, Jaw_Line, Nose;
						vector<vector<Point2f>> shapes;

						int i = 0;

						try
						{
							muFaceMark.lock();
							facemark->fit(gray, faces, shapes);
							muFaceMark.unlock();
						}
						catch (Exception& e)
						{
							const char* err_msg = e.what();
							std::cout << "exception caught: " << err_msg << std::endl;
							std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
						}

						landmarks = shapes[0];
						int radiusEyeL = 0;
						int radiusEyeR = 0;
						/*
						for (size_t s = 0; s < landmarks.size(); s++)
						{
							
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
								circle(gray, landmarks[s], 2.0, Scalar(255, 0, 0), 1, 8);
								L_Eye.push_back(landmarks[s]);
							}
							// Right Eye indicies
							else if (s >= 42 && s <= 47) {
								circle(gray, landmarks[s], 2.0, Scalar(255, 0, 0), 1, 8);
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

						int radiusL = landmarks[i + 41].y - landmarks[i + 37].y;
						int radiusR = landmarks[i + 47].y - landmarks[i + 43].y;
						/*
						for (int i = 0; i < 6; i++)
						{
							posLeftEyeY += landmarks[i + 36].y;
							posRightEyeY += landmarks[i + 42].y;
							posLeftEyeX += landmarks[i + 36].x;
							posRightEyeX += landmarks[i + 42].x;
						}
						*/

						//cv::cvtColor(faceColor, gray, COLOR_BGR2GRAY);
						cv::CascadeClassifier eye_cascade;
						std::vector<cv::Rect> eyes;
						eye_cascade.load("d:\\haarcascade_eye.xml");
						eye_cascade.detectMultiScale(gray, eyes, 1.1, 5);// , 0 | CASCADE_SCALE_IMAGE, cv::Size(20, 20));
						//mouth.load("d:\\haarcascade_mcs_mouth.xml");

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

						for (int k = 0; k < eyes.size(); k++)
						{
							//rectangle(faceColor, eyes[k], Scalar(0, 255, 0), 10);
							RemoveRedEye(faceColor, eyes[k], radiusL);
						}

						{
							Point2f center22(faceColor.cols / 2.0, faceColor.rows / 2.0);
							Mat rot = getRotationMatrix2D(center22, 360 - angle, 1.0);
							// determine bounding rectangle
							Rect bbox = RotatedRect(center22, faceColor.size(), 360 - angle).boundingRect();
							// adjust transformation matrix
							rot.at<double>(0, 2) += bbox.width / 2.0 - center22.x;
							rot.at<double>(1, 2) += bbox.height / 2.0 - center22.y;

							warpAffine(faceColor, faceColor, rot, bbox.size());

							rc = { (bbox.width - listOfFace[i].myROI.width) / 2, (bbox.height - listOfFace[i].myROI.height) / 2,listOfFace[i].myROI.width, listOfFace[i].myROI.height };

							faceColor = faceColor(rc);
						}

						cv::cvtColor(faceColor, faceColor, COLOR_BGR2BGRA);
						faceColor.copyTo(dest(listOfFace[i].myROI));

						imshow("test", dest);
						waitKey(0);
					}
					catch (Exception& e)
					{
						const char* err_msg = e.what();
						std::cout << "exception caught: " << err_msg << std::endl;
						std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
						faceFound = false;
					}

#ifdef TEST

						vector<Rect> faces;
						Rect rc = { 0,0,listOfFace[i].myROI.width, listOfFace[i].myROI.height };
						Mat face = listOfFace[i].croppedImage;
						int angle = listOfFace[i].angle;
						Mat gray;

						Mat faceColor;
						Point2f center22(face.cols / 2.0, face.rows / 2.0);
						Mat rot = getRotationMatrix2D(center22, angle, 1.0);
						// determine bounding rectangle
						Rect bbox = RotatedRect(center22, face.size(), angle).boundingRect();
						// adjust transformation matrix
						rot.at<double>(0, 2) += bbox.width / 2.0 - center22.x;
						rot.at<double>(1, 2) += bbox.height / 2.0 - center22.y;

						warpAffine(face, faceColor, rot, bbox.size());

						cv::cvtColor(faceColor, gray, COLOR_BGR2GRAY);
						cv::CascadeClassifier eye_cascade;
						std::vector<cv::Rect> eyes;
						eye_cascade.load("d:\\haarcascade_eye.xml");
						eye_cascade.detectMultiScale(gray, eyes, 1.1, 5);// , 0 | CASCADE_SCALE_IMAGE, cv::Size(20, 20));
						//mouth.load("d:\\haarcascade_mcs_mouth.xml");

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

						for (int k = 0; k < eyes.size(); k++)
						{
							//rectangle(faceColor, eyes[k], Scalar(0, 255, 0), 10);
							RemoveRedEye(faceColor, eyes[k]);
						}

						{
							Point2f center22(faceColor.cols / 2.0, faceColor.rows / 2.0);
							Mat rot = getRotationMatrix2D(center22, 360 - angle, 1.0);
							// determine bounding rectangle
							Rect bbox = RotatedRect(center22, faceColor.size(), 360 - angle).boundingRect();
							// adjust transformation matrix
							rot.at<double>(0, 2) += bbox.width / 2.0 - center22.x;
							rot.at<double>(1, 2) += bbox.height / 2.0 - center22.y;

							warpAffine(faceColor, faceColor, rot, bbox.size());

							rc = { (bbox.width - listOfFace[i].myROI.width) / 2, (bbox.height - listOfFace[i].myROI.height) / 2,listOfFace[i].myROI.width, listOfFace[i].myROI.height };

							faceColor = faceColor(rc);
						}

						cv::cvtColor(faceColor, faceColor, COLOR_BGR2BGRA);
						faceColor.copyTo(dest(listOfFace[i].myROI));
					}
					catch (Exception& e)
					{
						const char* err_msg = e.what();
						std::cout << "exception caught: " << err_msg << std::endl;
						std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
					}
			
#endif
				}
				
			}
		}
		
		pBitmap->VertFlipBuf();
	}
}



int CFaceDetector::FindNbFace(CRegardsBitmap * image, float& bestConfidence, const float& confidence)
{
	std::vector<Rect> pointOfFace;
	std::vector<CFace> listOfFace;
	int nbFace = 0;
	detectFacePCN->DetectFace(image, listOfFace, pointOfFace);

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


void CFaceDetector::RotateCorrectly(CRegardsBitmap * pBitmap, int angle)
{
	CV_Assert(angle % 90 == 0 && angle <= 360 && angle >= -360);
	if (angle == 90)
	{
		pBitmap->Rotate90();
	}
	else if (angle == 180)
	{
		pBitmap->Rotate180();
	}
	else if (angle == 270)
	{
		pBitmap->Rotate270();
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

void fillHoles(Mat & mask)
{

	Mat mask_floodfill = mask.clone();
	floodFill(mask_floodfill, cv::Point(0, 0), Scalar(255));
	Mat mask2;
	bitwise_not(mask_floodfill, mask2);
	mask = (mask2 | mask);
}

void CFaceDetector::RemoveRedEye(Mat& image, const Rect& rSelectionBox, int radius)
{
	Mat eyeMat = image(rSelectionBox);
	Mat eye;
	int radiusw = rSelectionBox.width / 2;
	int radiush = rSelectionBox.height / 2;
	Mat iris = imread("d:\\iris.png");
	cvtColor(eyeMat, eye, COLOR_BGR2GRAY);
	GaussianBlur(eye, eye, Size(5, 5), 0);
	double min = 0;
	double max = 0;
	Point minLoc;
	double min_val, max_val;
	Point maxLoc;
	minMaxLoc(eye, &min_val, &max_val, &minLoc, &maxLoc);
	//cv::cvtColor(eye, eye, cv::COLOR_GRAY2BGR);
	//cv::circle(eye, maxLoc, radius / 2, cv::Scalar(0, 255, 0));
	Rect rc;
	rc.x = maxLoc.x - radius / 2;
	rc.y = maxLoc.y - radius / 2;
	rc.width = radius;
	rc.height = radius;

	resize(iris, iris,Size(radius, radius));
	iris.copyTo(eyeMat(rc));
	imshow("test", eyeMat);
	waitKey(0);
	eyeMat.copyTo(image(rSelectionBox));
	imshow("test", image);
	waitKey(0);


	// Copy the fixed eye to the output image.
//	eyeOut.copyTo(image(rSelectionBox));

}

#ifdef TEST


eye = eyeMat(select);

imshow("test", eye);
waitKey(0);


vector<Mat>bgr(3);
split(eye, bgr);
Mat b = bgr[0];
Mat g = bgr[1];
Mat r = bgr[2];
Mat bg = b + g;
Mat mask = (bgr[2] > 40) & (bgr[2] > (bgr[1] + bgr[0]));;
Mat dst = Mat::zeros(eye.rows, eye.cols, CV_8UC3);
vector<vector<Point> > contours;
vector<Point> maxCont;
vector<Vec4i> hierarchy;
findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
// iterate through all the top-level contours,
// draw each connected component with its own random color

int thresh = 100;
RNG rng(12345);
Mat drawing = Mat::zeros(mask.size(), CV_8UC3);
for (size_t i = 0; i < contours.size(); i++)
{
	Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
	drawContours(drawing, contours, (int)i, color, 2, LINE_8, hierarchy, 0);
}

imshow("test", drawing);
waitKey(0);

double  maxArea = 0;
for (int k = 0; k < contours.size(); k++)
{
	double area = contourArea(contours[k]);
	if (area > maxArea)
	{
		maxArea = area;
		maxCont = contours[k];
	}
}

imshow("test", mask);
waitKey(0);

mask = mask * 0;
for (int i = 0; i < maxCont.size(); i++)
{
	Point cv = maxCont[i];
	mask.data[cv.x + cv.y * mask.size().width] = 255;
}
//drawContours(mask, maxCont, 0, (255), -1);
morphologyEx(mask, mask, MORPH_CLOSE, getStructuringElement(MORPH_DILATE, Point(5, 5)));

imshow("test", mask);
waitKey(0);
//dilate(mask, mask, Mat(), Point(3, 3), 3);

Mat mean = bg / 2;
bitwise_and(mean, mask, mean);
cvtColor(mean, mean, COLOR_GRAY2BGR);
cvtColor(mask, mask, COLOR_GRAY2BGR);
bitwise_and(~mask, eye, eye);
eye = eye - mean;

imshow("test", eye);
waitKey(0);

/*
vector<Mat>bgr(3);
split(eye, bgr);
Mat mask = (bgr[2] > 40) & (bgr[2] > (bgr[1] + bgr[0]));
fillHoles(mask);
dilate(mask, mask, Mat(), Point(-1, -1), 3, 1, 1);

// Calculate the mean channel by averaging
// the green and blue channels
Mat mean = (bgr[0] + bgr[1]) / 2;

// Copy the mean image to blue channel with mask.
mean.copyTo(bgr[0], mask);

// Copy the mean image to green channel with mask.
mean.copyTo(bgr[1], mask);

// Copy the mean image to red channel with mask.
mean.copyTo(bgr[2], mask);

// Merge the three channels
Mat eyeOut;
merge(bgr, eyeOut);
*/

#endif

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
