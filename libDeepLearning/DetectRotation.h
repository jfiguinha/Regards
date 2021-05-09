#pragma once

class CImageLoadingFormat;
class CPictureData;

namespace Regards
{
	namespace OpenCV
	{
		class CDetectRotation
		{
		public:
			CDetectRotation();
			~CDetectRotation();

			int GetExifOrientation(CRegardsBitmap* pBitmap, const bool& fastDetection);
		private:
			int RotateToExifOrientation(const int &angle);
			int RotateToOrientation(const int &angle);
		};
	}
}
