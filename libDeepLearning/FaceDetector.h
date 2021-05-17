#pragma once
#include <vector>

class CRegardsBitmap;
class CPictureData;
class FaceRect;
namespace Regards
{
	namespace OpenCV
	{
		class CFace
		{
		public:
			float confidence;
			cv::Mat croppedImage;
			cv::Rect myROI;
		};

		class CFaceDetector
		{
		public:
			CFaceDetector(const bool & fastDetection);
			~CFaceDetector();

			static bool LockOpenCLDnn();
			static bool UnlockOpenCLDnn();

			static void LoadModel(const string& config_file, const string& weight_file, const string& recognition, const string& face_landmark);
			std::vector<int> FindFace(CRegardsBitmap * pBitmap);
            void DetectEyes(CRegardsBitmap * pBitmap);

			int FindNbFace(cv::Mat & image, float& bestConfidence, const float& confidence = 0.5);
			int DetectAngleOrientation(const cv::Mat & image);
			int DectectOrientationByFaceDetector(CRegardsBitmap* pBitmap);
			void RotateCorrectly(cv::Mat const& src, cv::Mat& dst, int angle);
			int FaceRecognition(const int& numFace);
		private:

			double face_opencv_alignement(cv::Mat& image, bool& findEye);
			int DectectOrientationByFaceDetector(const cv::Mat& image);
			void RotateOpenCV(const float& angle, int& maxFace, float& confidence, int& selectAngle, const cv::Mat& image);
			void RemoveRedEye(cv::Mat & image, const cv::Rect & rSelectionBox);
			void ImageToJpegBuffer(cv::Mat & image, std::vector<uchar> & buff);  
			void detectFaceOpenCVDNN(const cv::Mat& frameOpenCVDNN, std::vector<CFace> & listOfFace, std::vector<cv::Rect> & pointOfFace);
			cv::Mat RotateAndExtractFace(const double &angle,const cv::Rect & faceLocation, const cv::Mat& image);

            static bool isload;
			
			static std::mutex muFaceMark;
			static std::mutex muLoading;
			static std::mutex muDnnAccess;
		};
	}
}
