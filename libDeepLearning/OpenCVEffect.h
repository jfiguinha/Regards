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
		class COpenCVEffect
		{
		public:
			COpenCVEffect()
			{
			};

			~COpenCVEffect()
			{
			};
			static void BrightnessAndContrastAuto(CRegardsBitmap* pictureData, float clipHistPercent = 0);
			static void EqualizeHistogram(CRegardsBitmap* pictureData);
			static void BrightnessAndContrastAuto(cv::Mat& image, float clipHistPercent = 0);
			static void BrightnessAndContrastAuto(cv::UMat& pictureData, float clipHistPercent = 0);
			static void LoadAndRotate(const wxString& filePath, const int& rotate);
			static void CalculateHistogram(CRegardsBitmap* pictureData, CRegardsBitmap* histogram,
			                               const int& colorChoice, const wxColour& colorBgnd,
			                               const wxColour& colorFont);
			static void NormalizeHistogram(CRegardsBitmap* pictureData, const int& colorChoice, const int& minValue,
			                               const int& maxValue);
		};
	}
}
