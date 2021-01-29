#pragma once
class CRegardsBitmap;
class CPictureData;
namespace Regards
{
	namespace OpenCV
	{
		class CFace
		{
		public:
			float confidence;
			cv::Mat croppedImage;
		};

		class CFaceDetector
		{
		public:
			CFaceDetector();
			~CFaceDetector();

			static void LoadModel(const string &config_file, const string &weight_file, const string &face_recognition, const string &eye_detection);
			std::vector<int> FindFace(CPictureData * pictureData);
			std::vector<int> FindFace(CRegardsBitmap * pBitmap);
			int FindNbFace(cv::Mat & image);
			void DetectEyes(CRegardsBitmap * pBitmap);
			int DetectAngleOrientation(const cv::Mat & image);


		private:
			void RemoveRedEye(cv::Mat & image, const cv::Rect & rSelectionBox);
			void ImageToJpegBuffer(cv::Mat & image, std::vector<uchar> & buff);
			void RotateCorrectly(cv::Mat const &src, cv::Mat &dst, int angle);
			void detectFaceOpenCVDNN(cv::Mat &frameOpenCVDNN, std::vector<CFace> & listOfFace, std::vector<cv::Rect> & pointOfFace);
			static bool isload;
			static std::mutex muLoading;
			static std::mutex muDnnAccess;
			static std::mutex muEyeAccess;

		};
	}
}
