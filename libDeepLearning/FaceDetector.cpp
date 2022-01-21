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
#include "DetectFace.h"
#define WIDTH_THUMBNAIL 1920
#define HEIGHT_THUMBNAIL 1080

using namespace cv;
using namespace dnn;
using namespace Regards::OpenCV;
using namespace Regards::Sqlite;
using namespace std;

struct FaceValueIntegration
{
	double pertinence;
	int nbValue;
};


static Net netRecognition;
static Net netPosition;


const Scalar meanVal(104.0, 177.0, 123.0);
const float confidenceThreshold = 0.59;
bool CFaceDetector::isload = false;
std::mutex CFaceDetector::muLoading;
std::mutex CFaceDetector::muFaceMark;
static Ptr<face::Facemark> facemark;

bool CFaceDetector::LockOpenCLDnn()
{
	return CDetectFace::LockOpenCLDnn();
}

bool CFaceDetector::UnlockOpenCLDnn()
{
	return CDetectFace::UnlockOpenCLDnn();;
}

CFaceDetector::CFaceDetector(const bool& fastDetection)
{
	detectFace = new CDetectFace();
}

CFaceDetector::~CFaceDetector()
{
	delete detectFace;
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

		detectFace.LoadModel(config_file, weight_file);

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

double CFaceDetector::face_opencv_alignement(Mat& image, bool& findEye)
{
	double theta_deg_eye = 0;
	vector<Rect> faces;
	int angle_add = 0;
	//Declaring a variable "image" to store input image given.
	//Mat detected_edges, Laugh_L, Laugh_R;

	Rect rc;
	rc.x = 0;
	rc.y = 0;
	rc.width = image.cols;
	rc.height = image.rows;

	faces.push_back(rc);

	bool faceFound = false;
	std::vector<Point2f> landmarks, R_Eyebrow, L_Eyebrow, L_Eye, R_Eye, Mouth, Jaw_Line, Nose;
	vector<vector<Point2f>> shapes;

	try
	{
		muFaceMark.lock();
		if (facemark->fit(image, faces, shapes))
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
	{
		landmarks = shapes[0];
		/*
		for (size_t s = 0; s < landmarks.size(); s++) {

			circle(image,landmarks[s], 2.0, Scalar( 255,0,0 ), 1, 8 );
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
	}
	return theta_deg_eye;
}


Mat CFaceDetector::RotateAndExtractFace(const double& theta_deg_eye, const Rect& faceLocation, CRegardsBitmap* pBitmap)
{
	Mat image = pBitmap->GetMatrix();
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
		detectFace->DetectFace(picture, confidenceThreshold, listOfFace, listFace);
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

		//int angle = DectectOrientationByFaceDetector(pBitmap);
		//if (angle != 0)
		//	RotateCorrectly(pBitmap, angle);

		detectFace->DetectFace(pBitmap, confidenceThreshold, listOfFace, pointOfFace);

		for (CFace face : listOfFace)
		{
			if (face.confidence > confidenceThreshold)
			{
				double angleRot = 0;
				Mat resizedImage;
				bool findEye = true;
				Size size(150, 150);
				resize(face.croppedImage, resizedImage, size);

				angleRot = face_opencv_alignement(resizedImage, findEye);

				//imwrite("d:\\test.jpg", resizedImage);

				if (findEye)
				{
					try
					{
						face.croppedImage = RotateAndExtractFace(angleRot, face.myROI, pBitmap);
						std::vector<uchar> buff;
						resize(face.croppedImage, face.croppedImage, size);
						ImageToJpegBuffer(face.croppedImage, buff);
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
	//int angle = 0;

	if (isLoading)
	{
		Mat dest;
		std::vector<CFace> listOfFace;
		pBitmap->VertFlipBuf();
		//int angle = DectectOrientationByFaceDetector(pBitmap);
		//if (angle != 0)
		//	RotateCorrectly(pBitmap, angle);

		detectFace->DetectFace(pBitmap, confidenceThreshold, listOfFace, pointOfFace);

		if (listOfFace.size() > 0)
		{
			for (int i = 0; i < listOfFace.size(); i++)
			{
				if (listOfFace[i].confidence > confidenceThreshold)
				{
					vector<Rect> faces;
					Mat gray, detected_edges, Laugh_L, Laugh_R;

					Rect rc = listOfFace[i].myROI;

					faces.push_back(rc);

					std::vector<Point2f> landmarks, R_Eyebrow, L_Eyebrow, L_Eye, R_Eye, Mouth, Jaw_Line, Nose;
					vector<vector<Point2f>> shapes;


					try
					{
						muFaceMark.lock();
						if (facemark->fit(dest, faces, shapes))
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
					{
						landmarks = shapes[0];

						/*
						for (size_t s = 0; s < landmarks.size(); s++) {

							circle(dest, landmarks[s], 2.0, Scalar(255, 0, 0), 30, 8);
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
						int posMinLeftEyeY = landmarks[i + 36].y;
						int posMaxLeftEyeY = 0;
						int posMaxRightEyeY = 0;
						int posMinRightEyeY = landmarks[i + 42].y;

						int posMinLeftEyeX = landmarks[i + 36].x;
						int posMaxLeftEyeX = 0;
						int posMinRightEyeX = landmarks[i + 42].x;
						int posMaxRightEyeX = 0;
						for (int i1 = 0; i1 < 6; i1++)
						{
							if (posMinLeftEyeY > landmarks[i1 + 36].y)
								posMinLeftEyeY = landmarks[i1 + 36].y;

							if (posMaxLeftEyeY < landmarks[i1 + 36].y)
								posMaxLeftEyeY = landmarks[i1 + 36].y;

							if (posMinRightEyeY > landmarks[i1 + 42].y)
								posMinRightEyeY = landmarks[i1 + 42].y;

							if (posMaxRightEyeY < landmarks[i1 + 42].y)
								posMaxRightEyeY = landmarks[i1 + 42].y;

							if (posMinLeftEyeX > landmarks[i1 + 36].x)
								posMinLeftEyeX = landmarks[i1 + 36].x;

							if (posMaxLeftEyeX < landmarks[i1 + 36].x)
								posMaxLeftEyeX = landmarks[i1 + 36].x;

							if (posMinRightEyeX > landmarks[i1 + 42].x)
								posMinRightEyeX = landmarks[i1 + 42].x;

							if (posMaxRightEyeX < landmarks[i1 + 42].x)
								posMaxRightEyeX = landmarks[i1 + 42].x;
						}

						//Left Eye
						{
							Rect rectEye;
							rectEye.x = posMinLeftEyeX; // +pointOfFace[i].x;
							rectEye.y = posMinLeftEyeY; // +pointOfFace[i].y;
							rectEye.width = posMaxLeftEyeX - posMinLeftEyeX;
							rectEye.height = posMaxLeftEyeY - posMinLeftEyeY;
							RemoveRedEye(dest, rectEye);

#ifndef NDEBUG
							cv::rectangle(dest, rectEye, cv::Scalar(255, 0, 0),10);
#endif
						}

						//Right Eye
						{
							Rect rectEye;
							rectEye.x = posMinRightEyeX; // +pointOfFace[i].x;
							rectEye.y = posMinRightEyeY; // +pointOfFace[i].y;
							rectEye.width = posMaxRightEyeX - posMinRightEyeX;
							rectEye.height = posMaxRightEyeY - posMinRightEyeY;
							RemoveRedEye(dest, rectEye);

#ifndef NDEBUG
							cv::rectangle(dest, rectEye, cv::Scalar(255, 0, 0), 10);
#endif
						}
					}

				}
			}
		}
        /*
		if (faceFound)
		{
			if (angle != 0)
			{
				RotateCorrectly(pBitmap, (360 - angle) % 360);
			}
		}
        */
		dest.release();
	}
}



int CFaceDetector::FindNbFace(CRegardsBitmap * image, float& bestConfidence, const float& confidence)
{
	std::vector<Rect> pointOfFace;
	std::vector<CFace> listOfFace;
	int nbFace = 0;
	detectFace->DetectFace(image, confidenceThreshold, listOfFace, pointOfFace);

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
		pBitmap->Rotation90();
	}
	else if (angle == 180)
	{
		pBitmap->HorzFlipBuf();
		pBitmap->VertFlipBuf();
	}
	else if (angle == 270)
	{
		pBitmap->Rotation90();
		pBitmap->HorzFlipBuf();
		pBitmap->VertFlipBuf();
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


void CFaceDetector::RemoveRedEye(Mat& image, const Rect& rSelectionBox)
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
	for (int32_t y = ymin; y < ymax; y++)
	{
		for (int32_t x = xmin; x < xmax; x++)
		{
			float a = 1.0f - 5.0f * ((x - 0.5f * (xmax + xmin)) * (x - 0.5f * (xmax + xmin)) + (y - 0.5f * (ymax +
				ymin)) * (y - 0.5f * (ymax + ymin))) / static_cast<float>((xmax - xmin) * (ymax - ymin));
			if (a < 0)
				a = 0;
			image.at<Vec4b>(Point(x, y))[2] = static_cast<uint8_t>(a * min(
				image.at<Vec4b>(Point(x, y))[1], image.at<Vec4b>(Point(x, y))[0]) + (1.0f - a) * image.at<
				Vec4b>(Point(x, y))[2]);
		}
	}
}

Mat Zscore(const Mat& fc) {
	Mat mean, std;
	meanStdDev(fc, mean, std);
	//    cout << mean << std << endl;
	Mat fc_norm = (fc - mean) / std;
	return fc_norm;

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


Mat eval(Mat face)
{
	Mat result;
	try
	{
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
	int predictedLabel = -1;
	double maxConfidence = 0.0;

	bool findFaceCompatible = false;
	CSqlFacePhoto facePhoto;
	CSqlFaceRecognition sqlfaceRecognition;
	vector<CFaceRecognitionData> faceRecognitonVec = facePhoto.GetAllNumFaceRecognition();

	if(!wxFileExists(CFileUtility::GetFaceThumbnailPath(numFace).ToStdString()))
	{
		return 0;
	}
	
	Mat face1 = imread(CFileUtility::GetFaceThumbnailPath(numFace).ToStdString());
	if(face1.empty())
		return 0;
	
	Mat face1Vec = eval(face1);
	Mat fc1 = Zscore(face1Vec);
	std::map<int, FaceValueIntegration>::iterator it;
	std::map<int, FaceValueIntegration> faceConfidence;

	if (faceRecognitonVec.size() > 0)
	{
		for (CFaceRecognitionData picture : faceRecognitonVec)
		{
			try
			{
				if (wxFileExists(CFileUtility::GetFaceThumbnailPath(picture.numFace).ToStdString()))
				{
					Mat face2 = imread(CFileUtility::GetFaceThumbnailPath(picture.numFace).ToStdString());
					if (face2.empty())
						continue;

					Mat face2Vec = eval(face2);
					Mat fc2 = Zscore(face2Vec);
					double confidence = CosineDistance(fc1, fc2);

					if (maxConfidence < confidence)
					{
						predictedLabel = picture.numFaceCompatible;
						maxConfidence = confidence;
					}

					face2.release();
					face2Vec.release();
				}


			}
			catch(...)
			{
				
			}
		}

		/*
		for (it = faceConfidence.begin(); it != faceConfidence.end(); it++)
		{
			int numFaceCompatible = it->first;
			FaceValueIntegration value = faceConfidence[numFaceCompatible];

			double confidence = value.pertinence / value.nbValue;

			if (maxConfidence < confidence)
			{
				predictedLabel = numFaceCompatible;
				maxConfidence = confidence;
			}
			
		}
		*/
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


	face1.release();
	face1Vec.release();
	return findFaceCompatible;
}
