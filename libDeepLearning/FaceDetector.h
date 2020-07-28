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

			static void LoadModel(const string &config_file, const string &weight_file, const string &face_recognition);
			std::vector<int> FindFace(CPictureData * pictureData);
			int FindNbFace(cv::Mat & image);
		private:

			void ImageToJpegBuffer(cv::Mat & image, std::vector<uchar> & buff);
			void detectFaceOpenCVDNN(cv::Mat &frameOpenCVDNN, std::vector<cv::Mat> & listOfFace);
			static bool isload;
			static std::mutex muLoading;
			
		};
	}
}
