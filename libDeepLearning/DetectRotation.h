#pragma once

namespace Regards
{
	namespace OpenCV
	{
		class CDetectRotation
		{
		public:
			CDetectRotation();
			~CDetectRotation();

			int GetExifOrientation(const cv::Mat& pBitmap, const bool& fastDetection);

		private:
			int RotateToExifOrientation(const int& angle);
			int RotateToOrientation(const int& angle);
		};
	}
}
