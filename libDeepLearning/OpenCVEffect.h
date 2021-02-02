#pragma once

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
		};
	}
}
