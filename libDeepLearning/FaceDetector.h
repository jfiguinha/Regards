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
			int FindNbFace(cv::Mat & image, int & angle);
			void DetectEyes(CRegardsBitmap * pBitmap);

		private:
			void RemoveRedEye(cv::Mat & image, const cv::Rect & rSelectionBox);
			void RotateCorrectly(cv::Mat const &src, cv::Mat &dst, int angle);
			void ImageToJpegBuffer(cv::Mat & image, std::vector<uchar> & buff);
			void detectFaceOpenCVDNN(cv::Mat &frameOpenCVDNN, std::vector<CFace> & listOfFace, std::vector<cv::Rect> & pointOfFace);
			int FindFace(cv::Mat & image, int & angle, std::vector<cv::Rect> & pointOfFace, std::vector<CFace> & listOfFace, int typeRotate = 0);
			static bool isload;
			static std::mutex muLoading;
			void Rotate(const cv::Mat& image, cv::Mat& dst, int degrees);
		};
	}
}
