#pragma once
#include <vector>

class CRegardsBitmap;
class CPictureData;
class FaceRect;
class CDetectFace;
class CDetectFacePCN;
class CFace;

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
			static void LoadModel();
			std::vector<int> FindFace(CRegardsBitmap* pBitmap);
			void DetectEyes(CRegardsBitmap* pBitmap);
			std::vector<cv::Rect> GetRectFace(CRegardsBitmap * picture);
			int DectectOrientationByFaceDetector(CRegardsBitmap* pBitmap);
			int FaceRecognition(const int& numFace);

		private:

			void RemoveRedEye(cv::Mat& image, const cv::Rect& rSelectionBox, const cv::Rect & radius);
			void ImageToJpegBuffer(cv::Mat& image, std::vector<uchar>& buff);
			cv::Mat RotateAndExtractFace(const double& angle, const cv::Rect& faceLocation, cv::Mat image);
			cv::Mat FaceDesriptor(cv::Mat face);
			void RotateAndCrop(CFace * face, cv::Mat& Source);
			CDetectFacePCN * detectFacePCN;
			CDetectFace * detectFace;
			static bool isload;

			static std::mutex muFaceMark;
			static std::mutex muLoading;
			
		};
	}
}
