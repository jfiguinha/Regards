#pragma once

class CPictureData;

namespace Regards
{
	namespace OpenCV
	{
		class CFaceDetector
		{
		public:
			CFaceDetector();
			~CFaceDetector();

			static void LoadModel(const string &config_file, const string &weight_file, const string &face_recognition, const string &eye_detection);
			std::vector<int> FindFace(CPictureData * pictureData);
			int FindNbFace(cv::Mat & image);
			std::vector<wxRect> DetectEyes(CPictureData * pictureData);

		private:

			void ImageToJpegBuffer(cv::Mat & image, std::vector<uchar> & buff);
			void detectFaceOpenCVDNN(cv::Mat &frameOpenCVDNN, std::vector<cv::Mat> & listOfFace, std::vector<cv::Rect> & pointOfFace);
			static bool isload;
			static std::mutex muLoading;
			
		};
	}
}
