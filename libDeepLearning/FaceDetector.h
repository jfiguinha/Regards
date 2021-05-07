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
			CFaceDetector();
			~CFaceDetector();


			static void LoadModel(const string &config_file, const string &weight_file, const string &face_recognition, const string &eye_detection, const string& landmarkPath);
			std::vector<int> FindFace(CPictureData * pictureData);
			std::vector<int> FindFace(CRegardsBitmap * pBitmap);
            void DetectEyes(CRegardsBitmap * pBitmap);

			int FindNbFace(cv::Mat & image, float& bestConfidence, const float& confidence = 0.5);
			int DetectAngleOrientation(const cv::Mat & image);

			void RotateCorrectly(cv::Mat const& src, cv::Mat& dst, int angle);
		private:
			void RemoveRedEye(cv::Mat & image, const cv::Rect & rSelectionBox);
			void ImageToJpegBuffer(cv::Mat & image, std::vector<uchar> & buff);  
			void detectFaceOpenCVDNN(cv::Mat &frameOpenCVDNN, std::vector<CFace> & listOfFace, std::vector<cv::Rect> & pointOfFace);
			cv::Mat face_alignement(const cv::Mat& image);

            static bool isload;
			static std::mutex muLoading;
			static std::mutex muDnnAccess;
			static std::mutex muEyeAccess;
			static std::mutex muDlibLandmarkAccess;
			static std::mutex muDlibAccess;
		};
	}
}
