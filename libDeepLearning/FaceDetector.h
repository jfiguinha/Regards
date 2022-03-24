#pragma once
#include <vector>

class CRegardsBitmap;
class CPictureData;
class FaceRect;
class CDetectFace;
class CDetectFacePCN;

namespace Regards
{
	namespace OpenCV
	{

		class CFaceDetector
		{
		public:
			CFaceDetector(const bool& fastDetection);
			~CFaceDetector();
			static void CleanBase();
			static void LoadModel(const string& config_file, const string& weight_file, const string& recognition, const string& face_landmark);
			std::vector<int> FindFace(CRegardsBitmap* pBitmap);
			void DetectEyes(CRegardsBitmap* pBitmap);
			std::vector<cv::Rect> GetRectFace(CRegardsBitmap * picture);
			int DectectOrientationByFaceDetector(CRegardsBitmap* pBitmap);
			int FaceRecognition(const int& numFace);

		private:

			void RotateCorrectly(CRegardsBitmap* pBitmap, int angle);
			int FindNbFace(CRegardsBitmap * image, float& bestConfidence, const float& confidence = 0.5);
			//double face_opencv_alignement(cv::Mat& image, bool& findEye);
			void RotateOpenCV(const float& angle, int& maxFace, float& confidence, int& selectAngle, CRegardsBitmap* pBitmap);
			void RemoveRedEye(cv::Mat& image, const cv::Rect& rSelectionBox, const cv::Rect & radius);
			void ImageToJpegBuffer(cv::Mat& image, std::vector<uchar>& buff);
			cv::Mat RotateAndExtractFace(const double& angle, const cv::Rect& faceLocation, cv::Mat image);
			cv::Mat FaceDesriptor(cv::Mat face);
			cv::Mat Zscore(const cv::Mat& fc);
			cv::Mat GetFaceScore(const int& numFace);
			//CDetectFace * detectFace;
			CDetectFacePCN * detectFacePCN;
			static bool isload;

			static std::mutex muFaceMark;
			static std::mutex muLoading;
			
		};
	}
}
