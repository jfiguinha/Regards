#pragma once
#include <vector>

class CRegardsBitmap;
class CPictureData;
class FaceRect;
class CDetectFace;

namespace Regards
{
	namespace OpenCV
	{

		class CFaceDetector
		{
		public:
			CFaceDetector(const bool& fastDetection);
			~CFaceDetector();
			static bool LockOpenCLDnn();
			static bool UnlockOpenCLDnn();
			static void LoadModel(const string& config_file, const string& weight_file, const string& recognition,
			                      const string& face_landmark, const string& protoPosition, const string& weightPosition);
			std::vector<int> FindFace(CRegardsBitmap* pBitmap);
			void DetectEyes(CRegardsBitmap* pBitmap);
			std::vector<cv::Rect> GetRectFace(CRegardsBitmap * picture);
			int DectectOrientationByFaceDetector(CRegardsBitmap* pBitmap);
			int FaceRecognition(const int& numFace);

		private:

			void RotateCorrectly(CRegardsBitmap* pBitmap, int angle);
			int FindNbFace(CRegardsBitmap * image, float& bestConfidence, const float& confidence = 0.5);
			double face_opencv_alignement(cv::Mat& image, bool& findEye);
			void RotateOpenCV(const float& angle, int& maxFace, float& confidence, int& selectAngle, CRegardsBitmap* pBitmap);
			void RemoveRedEye(cv::Mat& image, const cv::Rect& rSelectionBox);
			void ImageToJpegBuffer(cv::Mat& image, std::vector<uchar>& buff);
			cv::Mat RotateAndExtractFace(const double& angle, const cv::Rect& faceLocation, CRegardsBitmap* pBitmap);

			CDetectFace * detectFace;

			static bool isload;

			static std::mutex muFaceMark;
			static std::mutex muLoading;
			
		};
	}
}
