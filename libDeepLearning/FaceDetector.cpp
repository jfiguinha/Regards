#include <header.h>
#include "FaceDetector.h"
#include <SqlFacePhoto.h>
#include <SqlFaceLabel.h>
#include <opencv2/face/facemark.hpp>
#include <SqlFaceRecognition.h>
#include <FileUtility.h>
#include "DetectFace.h"
#include "DetectFacePCN.h"
#include <opencv2/objdetect/face.hpp>
#include "realesrgan.h"
#include "gfpgan.h"
#include <ConvertUtility.h>
#include <picture_utility.h>
#include "colornet.h"
#include <appcontext.h>
#include <wx/filename.h>
extern AppContext application_context;

#define WIDTH_THUMBNAIL 1920
#define HEIGHT_THUMBNAIL 1080

using namespace cv;
using namespace dnn;
using namespace Regards::OpenCV;
using namespace Regards::Sqlite;
using namespace face;
using namespace std;

const double cosine_similar_thresh = 0.363;
std::map<int, Mat> listScore;

struct FaceValueIntegration
{
	double pertinence;
	int nbValue;
};


static Net netPosition;
static CascadeClassifier eye_cascade;
std::mutex CFaceDetector::muFaceMark;
static Ptr<Facemark> facemark;
static Ptr<FaceRecognizerSF> faceRecognizer;
static Net ageNet;
static Net genderNet;
static std::unique_ptr<RealESRGAN>  real_net = nullptr;
static std::unique_ptr<GFPGAN> gfpgan = nullptr;
static std::unique_ptr<CColorisationNCNN> colorreal_net = nullptr;
static std::unique_ptr<CColorization> color_net = nullptr;

static bool isRealESRGAN_load = false;
static bool isGFPGAN_load = false;
static bool isColorisation_load = false;

const Scalar meanVal(104.0, 177.0, 123.0);
const float confidenceThreshold = 0.59;
bool CFaceDetector::isload = false;
std::mutex CFaceDetector::muLoading;

void CFaceDetector::CleanBase()
{
	listScore.clear();
}

CFaceDetector::CFaceDetector(const bool& fastDetection)
{
	detectFace = std::make_unique<CDetectFace>();
	detectFacePCN = std::make_unique<CDetectFacePCN>();
}

CFaceDetector::~CFaceDetector()
{
}



static void LoadRealESRGAN()
{
	if (!isRealESRGAN_load)
	{
		real_net = std::make_unique<RealESRGAN>();
		real_net->load(CFileUtility::GetFullpathModel("real_esrgan.param"), CFileUtility::GetFullpathModel("real_esrgan.bin"));
	}
	isRealESRGAN_load = true;
}

static void LoadGFPGAN()
{
	if (!isGFPGAN_load)
	{
		gfpgan = std::make_unique<GFPGAN>();
		gfpgan->load(CFileUtility::GetFullpathModel("encoder.param"), CFileUtility::GetFullpathModel("encoder.bin"), CFileUtility::GetFullpathModel("style.bin"));
		ageNet = readNet(CFileUtility::GetFullpathModel("age_net.caffemodel"), CFileUtility::GetFullpathModel("age_deploy.prototxt"));
		genderNet = readNet(CFileUtility::GetFullpathModel("gender_net.caffemodel"), CFileUtility::GetFullpathModel("gender_deploy.prototxt"));
	}
	isGFPGAN_load = true;
}

static void LoadColorisationNCNN()
{
	if (!isColorisation_load)
	{
		colorreal_net = std::make_unique<CColorisationNCNN>();
		colorreal_net->load(CFileUtility::GetFullpathModel("siggraph17_color_sim.param"), CFileUtility::GetFullpathModel("siggraph17_color_sim.bin"));
	}
	isColorisation_load = true;
}

static void LoadColorisation()
{
	if (!isColorisation_load)
	{
		color_net = std::make_unique<CColorization>();
		color_net->load(CFileUtility::GetFullpathModel("ncnn_v1.param"), CFileUtility::GetFullpathModel("ncnn_v1.bin"));
	}
	isColorisation_load = true;
}

//------------------------------------------------------------------------------------
static void to_ocv(const ncnn::Mat& result, cv::Mat& out)
{
	cv::Mat cv_result_32F = cv::Mat::zeros(cv::Size(512, 512), CV_32FC3);
	for (int i = 0; i < result.h; i++)
	{
		for (int j = 0; j < result.w; j++)
		{
			cv_result_32F.at<cv::Vec3f>(i, j)[2] = (result.channel(0)[i * result.w + j] + 1) / 2;
			cv_result_32F.at<cv::Vec3f>(i, j)[1] = (result.channel(1)[i * result.w + j] + 1) / 2;
			cv_result_32F.at<cv::Vec3f>(i, j)[0] = (result.channel(2)[i * result.w + j] + 1) / 2;
		}
	}

	cv::Mat cv_result_8U;
	cv_result_32F.convertTo(cv_result_8U, CV_8UC3, 255.0, 0);

	cv_result_8U.copyTo(out);

	cv::flip(out, out, 1);

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



int CFaceDetector::DectectOrientationByFaceDetector(const Mat& pBitmap)
{
	std::vector<Rect> pointOfFace;
	bool faceFound = false;
	bool isLoading = false;
	try
	{
		std::lock_guard<std::mutex> lock(muLoading);
		isLoading = isload;
	}
	catch(...)
	{

	}

	int selectAngle = 0;
	Mat Source;
	pBitmap.copyTo(Source);
	if (isLoading)
	{
		//cv::flip(Source, Source, 0);
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

void CFaceDetector::LoadModel(const bool& openCLCompatible, const bool& cudaCompatible)
{
	try
	{
		CDetectFacePCN detectFacePCN;
		CDetectFace detectFace;

		string model = CFileUtility::GetFullpathModel("face_landmark_model.dat");
		facemark = createFacemarkKazemi();
		facemark->loadModel(model);

		model = CFileUtility::GetFullpathModel("face_recognition_sface_2021dec.onnx");
		faceRecognizer = FaceRecognizerSF::create(model, "");
		eye_cascade.load(CFileUtility::GetFullpathModel("haarcascade_eye.xml"));

       // printf("CFaceDetector::LoadModel \n");
		detectFace.LoadModel(openCLCompatible, cudaCompatible);
		detectFacePCN.LoadModel(openCLCompatible, cudaCompatible);


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




Point2f rotatePointUsingTransformationMat(const Point2f& inPoint, const Point2f& center, const double& rotAngle)
{
	double angRad = rotAngle * (CV_PI / 180);
	Point2f outPoint;
	//CW rotation
	outPoint.x = std::cos(angRad) * inPoint.x - std::sin(angRad) * inPoint.y;
	outPoint.y = std::sin(angRad) * inPoint.x + std::cos(angRad) * inPoint.y;
	return outPoint;
}

cv::Mat CFaceDetector::SuperResolution(const cv::Mat& Face)
{
	LoadRealESRGAN();
	cv::Mat img_up;
	if (Face.channels() == 4)
	{
		cv::Mat FaceBGR;
		cv::cvtColor(Face, FaceBGR, cv::COLOR_BGRA2BGR);
		real_net->tile_process(FaceBGR, img_up);
		cv::resize(img_up, img_up, Face.size());
		cv::cvtColor(img_up, img_up, cv::COLOR_BGR2BGRA);
	}
	else
	{
		
		real_net->tile_process(Face, img_up);
		cv::resize(img_up, img_up, Face.size());
		
	}

	return img_up;
}

cv::Mat CFaceDetector::Colorisation(const cv::Mat& Face)
{
   // printf("CFaceDetector::Colorisation \n");
	LoadColorisationNCNN();
	//return colorreal_net->colorization(Face);
    return colorreal_net->Execute(Face);
}

CSexeAndAge CFaceDetector::DetermineSexeAndAge(const cv::Mat& Face)
{
	CSexeAndAge sexeAndAge;
	Scalar MODEL_MEAN_VALUES = Scalar(78.4263377603, 87.7689143744, 114.895847746);
	vector<string> ageList = { "(0-2)","(4-6)","(8-12)","(15-20)","(25-32)","(38-43)","(48-53)","(60-100)" };
	vector<string> genderList = { "Male","Female" };

	Mat blob = blobFromImage(Face, 1, Size(227, 227), MODEL_MEAN_VALUES, false);

	genderNet.setInput(blob);
	vector<float> genderPreds = genderNet.forward();
	// find max element index (distance function does the argmax() work in C++)
	int max_index_gender = std::distance(genderPreds.begin(), max_element(genderPreds.begin(), genderPreds.end()));
	string gender = genderList[max_index_gender];


	ageNet.setInput(blob);
	vector<float> agePreds = ageNet.forward();
	// finding maximum indicd in the age_preds vector
	int max_indice_age = std::distance(agePreds.begin(), max_element(agePreds.begin(), agePreds.end()));
	string age = ageList[max_indice_age];

	cout << "Gender: " << gender << "  Age: " << age << endl;

	sexeAndAge.age = age;
	sexeAndAge.sexe = gender;

	return sexeAndAge;

}

Mat CFaceDetector::RotateAndExtractFace(const double& theta_deg_eye, const Rect& faceLocation, const Mat& image)
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
	CPictureUtility::VerifRectSize(rect, dst);
	//bool is_inside = (rect & cv::Rect(0, 0, dst.cols, dst.rows)) == rect;

	dst = dst(rect);

	r.release();

	return dst;
}

std::vector<Rect> CFaceDetector::GetRectFace(const Mat& picture)
{
	std::vector<Rect> listFace;
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

float CalculPictureRatio(const int& pictureWidth, const int& pictureHeight, const int& pictureWidthOut,
                         const int& pictureHeightOut)
{
	float new_ratio = 1;

	//int tailleAffichageWidth = 0, tailleAffichageHeight = 0;

	if (pictureWidth > pictureHeight)
		new_ratio = static_cast<float>(pictureWidthOut) / static_cast<float>(pictureWidth);
	else
		new_ratio = static_cast<float>(pictureHeightOut) / static_cast<float>(pictureHeight);

	if ((pictureHeight * new_ratio) > pictureHeightOut)
	{
		new_ratio = static_cast<float>(pictureHeightOut) / static_cast<float>(pictureHeight);
	}
	if ((pictureWidth * new_ratio) > pictureWidthOut)
	{
		new_ratio = static_cast<float>(pictureWidthOut) / static_cast<float>(pictureWidth);
	}

	return new_ratio;
}

static std::vector<cv::Point2f> GetSFaceLandmarks(
	const std::vector<cv::Point2f>& landmarks)
{
	if (landmarks.size() < 68)
		return {};

	cv::Point2f leftEye(0.0f, 0.0f);
	cv::Point2f rightEye(0.0f, 0.0f);

	for (int i = 36; i <= 41; ++i)
		leftEye += landmarks[i];

	for (int i = 42; i <= 47; ++i)
		rightEye += landmarks[i];

	leftEye *= 1.0f / 6.0f;
	rightEye *= 1.0f / 6.0f;

	return
	{
		leftEye,
		rightEye,
		landmarks[30], // nez
		landmarks[48], // bouche gauche
		landmarks[54]  // bouche droite
	};
}

std::vector<int> CFaceDetector::FindFace(const Mat& pBitmap, const wxString& filename)
{
	std::vector<int> listFace;
	int i = 0;
	bool isLoading = false;
	muLoading.lock();
	isLoading = isload;
	muLoading.unlock();

	if (isLoading)
	{

		LoadGFPGAN();

		CSqlFacePhoto facePhoto;
		Mat dest, source;
		std::vector<CFace> listOfFace;
		std::vector<Rect> pointOfFace;
		Mat resizeSource;
		//cv::flip(pBitmap, source, 1);
		cvtColor(pBitmap, source, COLOR_BGRA2BGR);
		float dRatio = 1.0;
		float invertRatio = 1.0;
		if (source.size().width > 600 || source.size().height > 600)
		{
			dRatio = CalculPictureRatio(source.size().width, source.size().height, 600, 600);
			invertRatio = CalculPictureRatio(source.size().width * dRatio, source.size().height * dRatio,
			                                 source.size().width, source.size().height);
			resize(source, resizeSource, Size(source.size().width * dRatio, source.size().height * dRatio));
			detectFacePCN->DetectFace(resizeSource, listOfFace, pointOfFace);
		}
		else
			detectFacePCN->DetectFace(source, listOfFace, pointOfFace);


		for (CFace face : listOfFace)
		{
			if (face.confidence > confidenceThreshold)
			{
				CSexeAndAge sexeAndAge;
				Mat resizedImage;
				Size size(150, 150);

				try
				{
					face.myROI.x *= invertRatio;
					face.myROI.y *= invertRatio;
					face.myROI.width *= invertRatio;
					face.myROI.height *= invertRatio;

					resizedImage = RotateAndExtractFace(
						face.angle,
						face.myROI,
						source);

					if (resizedImage.empty())
						continue;

					/*
					try
					{


						std::vector<cv::Rect> faces;
						faces.emplace_back(
							0,
							0,
							resizedImage.cols,
							resizedImage.rows);

						std::vector<std::vector<cv::Point2f>> landmarkList;

						{
							std::lock_guard<std::mutex> lock(muFaceMark);

							if (!facemark->fit(
								resizedImage,
								faces,
								landmarkList))
							{
								continue;
							}
						}

						if (landmarkList.empty() ||
							landmarkList[0].size() != 68)
						{
							continue;
						}

						const auto sfaceLandmarks =
							GetSFaceLandmarks(landmarkList[0]);

						if (sfaceLandmarks.size() != 5)
							continue;

						cv::Mat alignedFace;

						faceRecognizer->alignCrop(
							resizedImage,
							sfaceLandmarks,
							alignedFace);

						if (alignedFace.empty())
							continue;

						resizedImage = alignedFace;

					}
					catch (cv::Exception& e)
					{
						printf("CFaceDetector::FindFace exception: %s\n", e.what());
						source(face.myROI).copyTo(resizedImage);
					}
					*/

					float bestConfidence = 0;
					Mat resizedBgra;
					cvtColor(resizedImage, resizedBgra, COLOR_BGR2BGRA);
					int nbFace = detectFace->FindNbFace(resizedBgra, confidenceThreshold, bestConfidence);

					if (nbFace > 0)
					{
						Mat localFace;
						std::vector<uchar> buff;
						cv::Mat bg_upsample;

						sexeAndAge = DetermineSexeAndAge(resizedImage);
                        

                        if(resizedImage.size().width < 200 && resizedImage.size().height < 200)
                        {
                            bg_upsample = CFaceDetector::SuperResolution(resizedImage);
                            ImageToJpegBuffer(bg_upsample, buff);
                        }
                        else
                            ImageToJpegBuffer(resizedImage, buff);
						
						int numFace = facePhoto.InsertFace(filename, sexeAndAge.sexe, sexeAndAge.age, ++i, face.croppedImage.rows,
						                                   face.croppedImage.cols, face.confidence, buff.data(),
						                                   buff.size());

						listFace.push_back(numFace);
						face.croppedImage.release();
					}
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


void CFaceDetector::RemoveRedEyes(cv::Mat& pBitmap)
{
	if (pBitmap.empty())
		return;

	{
		std::lock_guard<std::mutex> lock(muLoading);

		if (!isload)
			return;
	}

	if (facemark.empty() || detectFacePCN == nullptr)
		return;

	cv::Mat source;

	// On travaille sur une copie afin de ne pas modifier directement
	// l'image fournie par l'appelant.
	pBitmap.copyTo(source);

	std::vector<CFace> listOfFace;
	std::vector<cv::Rect> pointOfFace;

	try
	{
		detectFacePCN->DetectFace(
			source,
			listOfFace,
			pointOfFace);
	}
	catch (const cv::Exception& e)
	{
		std::cout << "PCN face detection error: "
			<< e.what()
			<< std::endl;
		return;
	}

	for (auto& detectedFace : listOfFace)
	{
		if (detectedFace.confidence <= confidenceThreshold)
			continue;

		cv::Mat face = detectedFace.croppedImage;

		if (face.empty())
			continue;

		try
		{
			/*
			 * ---------------------------------------------------------
			 * 1. Rotation du visage suivant l'orientation détectée
			 *    par PCN.
			 * ---------------------------------------------------------
			 */
			cv::Mat faceColor;

			const cv::Point2f center(
				face.cols * 0.5f,
				face.rows * 0.5f);

			const double angle =
				detectedFace.angle;

			cv::Mat rotation =
				cv::getRotationMatrix2D(
					center,
					angle,
					1.0);

			const cv::Rect bbox =
				cv::RotatedRect(
					center,
					face.size(),
					angle).boundingRect();

			rotation.at<double>(0, 2) +=
				bbox.width * 0.5 -
				center.x;

			rotation.at<double>(1, 2) +=
				bbox.height * 0.5 -
				center.y;

			cv::warpAffine(
				face,
				faceColor,
				rotation,
				bbox.size(),
				cv::INTER_LINEAR,
				cv::BORDER_REPLICATE);

			if (faceColor.empty())
				continue;

			/*
			 * ---------------------------------------------------------
			 * 2. Détection des 68 landmarks Kazemi.
			 * ---------------------------------------------------------
			 */
			std::vector<cv::Rect> faceRects;
			faceRects.emplace_back(
				0,
				0,
				faceColor.cols,
				faceColor.rows);

			std::vector<std::vector<cv::Point2f>> shapes;

			{
				std::lock_guard<std::mutex> lock(muFaceMark);

				if (!facemark->fit(
					faceColor,
					faceRects,
					shapes))
				{
					continue;
				}
			}

			if (shapes.empty() ||
				shapes[0].size() < 68)
			{
				continue;
			}

			const std::vector<cv::Point2f>& landmarks =
				shapes[0];

			/*
			 * ---------------------------------------------------------
			 * 3. Récupération des landmarks des yeux.
			 *
			 * Kazemi 68 points :
			 *
			 * œil gauche : 36 -> 41
			 * œil droit  : 42 -> 47
			 * ---------------------------------------------------------
			 */
			std::vector<cv::Point2f> leftEyePoints;
			std::vector<cv::Point2f> rightEyePoints;

			leftEyePoints.reserve(6);
			rightEyePoints.reserve(6);

			for (int i = 36; i <= 41; ++i)
				leftEyePoints.push_back(landmarks[i]);

			for (int i = 42; i <= 47; ++i)
				rightEyePoints.push_back(landmarks[i]);

			cv::Rect leftEye =
				cv::boundingRect(leftEyePoints);

			cv::Rect rightEye =
				cv::boundingRect(rightEyePoints);

			/*
			 * On agrandit légèrement les rectangles afin de donner
			 * suffisamment de matière à RemoveRedEye().
			 */
			constexpr int EYE_MARGIN = 5;

			leftEye.x -= EYE_MARGIN;
			leftEye.y -= EYE_MARGIN;
			leftEye.width += EYE_MARGIN * 2;
			leftEye.height += EYE_MARGIN * 2;

			rightEye.x -= EYE_MARGIN;
			rightEye.y -= EYE_MARGIN;
			rightEye.width += EYE_MARGIN * 2;
			rightEye.height += EYE_MARGIN * 2;

			CPictureUtility::VerifRectSize(
				leftEye,
				faceColor);

			CPictureUtility::VerifRectSize(
				rightEye,
				faceColor);

			if (leftEye.empty() || rightEye.empty())
				continue;

			/*
			 * ---------------------------------------------------------
			 * 4. Suppression des yeux rouges.
			 *
			 * Le rectangle des landmarks sert également de zone
			 * de correction.
			 * ---------------------------------------------------------
			 */
			RemoveRedEye(
				faceColor,
				leftEye,
				leftEye);

			RemoveRedEye(
				faceColor,
				rightEye,
				rightEye);

			/*
			 * ---------------------------------------------------------
			 * 5. Remise du visage dans son orientation originale.
			 * ---------------------------------------------------------
			 */
			cv::Mat restoredFace;

			const cv::Point2f restoreCenter(
				faceColor.cols * 0.5f,
				faceColor.rows * 0.5f);

			const double restoreAngle =
				360.0 - detectedFace.angle;

			cv::Mat restoreRotation =
				cv::getRotationMatrix2D(
					restoreCenter,
					restoreAngle,
					1.0);

			const cv::Rect restoreBBox =
				cv::RotatedRect(
					restoreCenter,
					faceColor.size(),
					restoreAngle).boundingRect();

			restoreRotation.at<double>(0, 2) +=
				restoreBBox.width * 0.5 -
				restoreCenter.x;

			restoreRotation.at<double>(1, 2) +=
				restoreBBox.height * 0.5 -
				restoreCenter.y;

			cv::warpAffine(
				faceColor,
				restoredFace,
				restoreRotation,
				restoreBBox.size(),
				cv::INTER_LINEAR,
				cv::BORDER_REPLICATE);

			if (restoredFace.empty())
				continue;

			/*
			 * ---------------------------------------------------------
			 * 6. Repositionnement du visage dans l'image source.
			 * ---------------------------------------------------------
			 */
			cv::Rect targetRect(
				(restoreBBox.width -
					detectedFace.myROI.width) / 2,

				(restoreBBox.height -
					detectedFace.myROI.height) / 2,

				detectedFace.myROI.width,
				detectedFace.myROI.height);

			targetRect.x += 1;
			targetRect.y += 1;
			targetRect.width -= 2;
			targetRect.height -= 2;

			CPictureUtility::VerifRectSize(
				targetRect,
				restoredFace);

			if (targetRect.empty())
				continue;

			cv::Mat finalFace =
				restoredFace(targetRect);

			/*
			 * ---------------------------------------------------------
			 * 7. Vérification de la ROI dans l'image originale.
			 * ---------------------------------------------------------
			 */
			cv::Rect destinationRect =
				detectedFace.myROI;

			destinationRect.x += 1;
			destinationRect.y += 1;
			destinationRect.width -= 2;
			destinationRect.height -= 2;

			CPictureUtility::VerifRectSize(
				destinationRect,
				source);

			if (destinationRect.empty())
				continue;

			/*
			 * On évite de copier une image dont les dimensions
			 * ne correspondent pas exactement à la destination.
			 */
			cv::Mat resizedFace;

			if (finalFace.size() != destinationRect.size())
			{
				cv::resize(
					finalFace,
					resizedFace,
					destinationRect.size(),
					0.0,
					0.0,
					cv::INTER_LINEAR);
			}
			else
			{
				resizedFace = finalFace;
			}

			resizedFace.copyTo(
				source(destinationRect));
		}
		catch (const cv::Exception& e)
		{
			std::cout << "DetectEyes error: "
				<< e.what()
				<< std::endl;
		}
	}

	/*
	 * Si pBitmap est destiné à être modifié par référence,
	 * il faut recopier source dans le Mat original.
	 *
	 * Attention : cette ligne ne modifie pas les données du Mat
	 * si pBitmap est passé en const.
	 */
	source.copyTo(pBitmap);
}



void CFaceDetector::ImageToJpegBuffer(const Mat& image, std::vector<uchar>& buff)
{
	//std::vector<uchar> buff;//buffer for coding
	std::vector<int> param(2);
	param[0] = IMWRITE_JPEG_QUALITY;
	param[1] = 100;
	imencode(".jpg", image, buff, param);
}

void CFaceDetector::RemoveRedEye(
	const cv::Mat& image,
	const cv::Rect& eyeRect,
	const cv::Rect& radius)
{
	if (image.empty() ||
		image.channels() != 3 ||
		eyeRect.empty() ||
		radius.empty())
	{
		return;
	}

	const cv::Rect imageBounds(
		0, 0,
		image.cols,
		image.rows);

	const cv::Rect selection =
		eyeRect & imageBounds;

	if (selection.empty())
		return;

	cv::Mat eye = image(selection);

	/*
	 * Recherche des pixels rouges.
	 *
	 * OpenCV utilise BGR :
	 *   [0] = Bleu
	 *   [1] = Vert
	 *   [2] = Rouge
	 */
	std::vector<cv::Mat> channels;
	cv::split(eye, channels);

	cv::Mat maskRed;
	cv::Mat maskGreen;
	cv::Mat maskBlue;

	// Rouge suffisamment intense
	cv::threshold(
		channels[2],
		maskRed,
		80,
		255,
		cv::THRESH_BINARY);

	/*
	 * R > G * 1.35
	 * R > B * 1.35
	 */
	cv::Mat greenFloat;
	cv::Mat redFloat;
	cv::Mat blueFloat;

	channels[1].convertTo(
		greenFloat,
		CV_32F);

	channels[2].convertTo(
		redFloat,
		CV_32F);

	channels[0].convertTo(
		blueFloat,
		CV_32F);

	cv::Mat maskRG;
	cv::Mat maskRB;

	cv::compare(
		redFloat,
		greenFloat * 1.35,
		maskRG,
		cv::CMP_GT);

	cv::compare(
		redFloat,
		blueFloat * 1.35,
		maskRB,
		cv::CMP_GT);

	/*
	 * Combinaison des trois conditions.
	 */
	cv::Mat mask;

	cv::bitwise_and(
		maskRed,
		maskRG,
		mask);

	cv::bitwise_and(
		mask,
		maskRB,
		mask);

	/*
	 * Nettoyage du masque.
	 */
	const cv::Mat kernel =
		cv::getStructuringElement(
			cv::MORPH_ELLIPSE,
			cv::Size(3, 3));

	cv::morphologyEx(
		mask,
		mask,
		cv::MORPH_OPEN,
		kernel);

	cv::morphologyEx(
		mask,
		mask,
		cv::MORPH_CLOSE,
		kernel);

	/*
	 * Limite la correction à la zone radius
	 * autour du centre de l'œil.
	 */
	cv::Point center(
		eye.cols / 2,
		eye.rows / 2);

	cv::Rect correction(
		center.x - radius.width / 2,
		center.y - radius.height / 2,
		radius.width,
		radius.height);

	correction &= cv::Rect(
		0,
		0,
		eye.cols,
		eye.rows);

	if (correction.empty())
		return;

	cv::Mat localMask =
		cv::Mat::zeros(
			mask.size(),
			CV_8UC1);

	mask(correction).copyTo(
		localMask(correction));

	/*
	 * Remplacement du rouge par une luminance neutre.
	 */
	cv::Mat gray;

	cv::cvtColor(
		eye,
		gray,
		cv::COLOR_BGR2GRAY);

	cv::Mat corrected;

	cv::cvtColor(
		gray,
		corrected,
		cv::COLOR_GRAY2BGR);

	corrected.copyTo(
		eye,
		localMask);
}

double GetNumFaceCompatibleScore(
	int numFace,
	const std::vector<CFaceRecognitionData>& faceRecognitionVec,
	const cv::Mat& feature1)
{
	std::vector<double> scores;

	for (const auto& picture : faceRecognitionVec)
	{
		if (picture.numFaceCompatible != numFace)
			continue;

		if (picture.feature.empty())
		{
			const wxString fileSource =
				CFileUtility::GetFaceThumbnailPath(picture.numFace);

			if (!wxFileExists(fileSource))
				continue;

			cv::Mat alignedFace2 =
				cv::imread(CConvertUtility::ConvertToStdString(fileSource));

			if (alignedFace2.empty())
				continue;

			faceRecognizer->feature(alignedFace2, picture.feature);
		}

		if (picture.feature.empty())
			continue;

		const double localScore =
			faceRecognizer->match(
				feature1,
				picture.feature,
				cv::FaceRecognizerSF::DisType::FR_COSINE);

		scores.push_back(localScore);
	}

	if (scores.empty())
		return 0.0;

	constexpr size_t MAX_SCORES = 5;

	const size_t count = std::min(scores.size(), MAX_SCORES);

	std::partial_sort(
		scores.begin(),
		scores.begin() + count,
		scores.end(),
		std::greater<double>());

	double score = 0.0;

	for (size_t i = 0; i < count; ++i)
		score += scores[i];

	return score / static_cast<double>(count);
}

int CFaceDetector::FaceRecognition(const int& numFace)
{
	double maxConfidence = 0.0;
	Mat fc1;
	bool findFaceCompatible = false;
	CSqlFaceLabel faceLabel;
	CSqlFacePhoto facePhoto;
	CSqlFaceRecognition sqlfaceRecognition;
	vector<CFaceRecognitionData> faceRecognitonVec = facePhoto.GetAllNumFaceRecognition();
	wxString fileSource = CFileUtility::GetFaceThumbnailPath(numFace);
	vector<int> listLabel = faceLabel.GetAllFace();
	if (!wxFileExists(fileSource))
	{
		return 0;
	}


	Mat aligned_face1 = imread(CConvertUtility::ConvertToStdString(fileSource));
	if (aligned_face1.empty())
		return 0;

	Mat feature1;
	faceRecognizer->feature(aligned_face1, feature1);
	feature1 = feature1.clone();


	if (faceRecognitonVec.size() > 0)
	{
		int predictedLabel = -1;
		double confidence = 0.0;

		for (int i : listLabel)
		{
			double score = GetNumFaceCompatibleScore(i, faceRecognitonVec, feature1);
			if (score >= cosine_similar_thresh && score > confidence)
			{
				confidence = score;
				predictedLabel = i;
			}
		}

		if (predictedLabel != -1 && confidence > cosine_similar_thresh)
		{
			sqlfaceRecognition.InsertFaceRecognition(numFace, predictedLabel);
			findFaceCompatible = true;
		}

	}
	if (faceRecognitonVec.size() == 0 || !findFaceCompatible)
	{
		CSqlFaceLabel sqlfaceLabel;
		wxString label = "Face number " + to_string(numFace);
		sqlfaceRecognition.InsertFaceRecognition(numFace, numFace);
		sqlfaceLabel.InsertFaceLabel(numFace, label, true);
	}

	return findFaceCompatible;

}
