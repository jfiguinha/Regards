#pragma once
#include <vector>

class FaceRect;
class CDetectFace;
class CDetectFacePCN;
class CFace;

namespace Regards
{
	namespace OpenCV
	{
		class CSexeAndAge
		{
		public:
			wxString age = "";
			wxString sexe = "";
		};

		class CFaceDetector
		{
		public:
			CFaceDetector(const bool& fastDetection);
			~CFaceDetector();
			static void CleanBase();
			static void LoadModel(const bool& openCLCompatible, const bool& cudaCompatible);
			std::vector<int> FindFace(const cv::Mat& pBitmap, const wxString& filename);
			void RemoveRedEyes(cv::Mat& pBitmap);
			std::vector<cv::Rect> GetRectFace(const cv::Mat& picture);
			int DectectOrientationByFaceDetector(const cv::Mat& pBitmap);
			int FaceRecognition(const int& numFace);
			CSexeAndAge DetermineSexeAndAge(const cv::Mat& Face);
			static cv::Mat SuperResolution(const cv::Mat& Face);
			static cv::Mat Colorisation(const cv::Mat& Face);
		private:


			cv::Mat AlignFace(
				cv::Mat& face,
				std::vector<cv::Point2f> & landmarks);
			void RemoveRedEye(const cv::Mat& image, const cv::Rect& rSelectionBox, const cv::Rect& radius);
			void ImageToJpegBuffer(const cv::Mat& image, std::vector<uchar>& buff);
			cv::Mat RotateAndExtractFace(const double& angle, const cv::Rect& faceLocation, const cv::Mat& image);
			cv::Mat FaceDesriptor(const cv::Mat& face);
			void RotateAndCrop(CFace* face, cv::Mat& Source);
			std::unique_ptr<CDetectFacePCN> detectFacePCN;
			std::unique_ptr<CDetectFace> detectFace;
			static bool isload;
			
			static std::mutex muFaceMark;
			static std::mutex muLoading;
		};
	}
}
