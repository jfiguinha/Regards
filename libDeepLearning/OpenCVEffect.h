#pragma once

namespace cv
{
	class UMat;
}

namespace Regards
{
	namespace OpenCV
	{
		class COpenCVEffect
		{
		public:
			COpenCVEffect() {};
			~COpenCVEffect() {};
			static void BrightnessAndContrastAuto(CRegardsBitmap * pictureData, float clipHistPercent = 0);
			static void EqualizeHistogram(CRegardsBitmap * pictureData);

			static void BrightnessAndContrastAuto(cv::UMat & pictureData, float clipHistPercent = 0);

		};
	}
}
