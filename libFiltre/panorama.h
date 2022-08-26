#pragma once

namespace cv
{
	class UMat;
	class Mat;
}

namespace Regards
{
	namespace OpenCV
	{
		class CImagePanorama
		{
		public:
			CImagePanorama()
			{

			}

			~CImagePanorama()
			{

			}

			static cv::Mat CreatePanorama(const cv::Mat & image_left, const cv::Mat & image_right);
		};
	}
}