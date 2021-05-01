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

			int GetExifOrientation(CRegardsBitmap* pBitmap);
			//int GetAngleOrientation(CPictureData * pictureDat);

		private:
			int RotateToExifOrientation(const int &angle);
			int RotateToOrientation(const int &angle);
			int DectectOrientationByFaceDetector(CRegardsBitmap* pBitmap);
			void RotateOpenCV(const float& angle, int& maxFace, float& confidence, int& selectAngle, const cv::Mat& image);
			static bool isload;
			static mutex muLoading;
		};
	}
}
