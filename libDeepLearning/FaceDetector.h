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
		};

		class CFaceDetector
		{
		public:
			CFaceDetector();
			~CFaceDetector();

#ifdef __APPLE__
            std::vector<int> FindFace(CRegardsBitmap * pBitmap, std::vector<FaceRect> & listFaceRect);
            void DetectEyes(CRegardsBitmap * pBitmap, std::vector<FaceRect> & listFaceRect);
            static void LoadModel(const string &eye_detection);
#else
			static void LoadModel(const string &config_file, const string &weight_file, const string &face_recognition, const string &eye_detection);
			std::vector<int> FindFace(CPictureData * pictureData);
			std::vector<int> FindFace(CRegardsBitmap * pBitmap);
            void DetectEyes(CRegardsBitmap * pBitmap);

			int FindNbFace(cv::Mat & image);
			int DetectAngleOrientation(const cv::Mat & image);
#endif

		private:
			void RemoveRedEye(cv::Mat & image, const cv::Rect & rSelectionBox);
			void ImageToJpegBuffer(cv::Mat & image, std::vector<uchar> & buff);
			
            
#ifndef __APPLE
            void RotateCorrectly(cv::Mat const &src, cv::Mat &dst, int angle);
			void detectFaceOpenCVDNN(cv::Mat &frameOpenCVDNN, std::vector<CFace> & listOfFace, std::vector<cv::Rect> & pointOfFace);
#endif
            
            static bool isload;
			static std::mutex muLoading;
			static std::mutex muDnnAccess;
			static std::mutex muEyeAccess;

		};
	}
}
