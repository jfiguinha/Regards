#pragma once
#ifndef __APPLE__
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

			int GetExifOrientation(CPictureData * pictureDat);
			//int GetAngleOrientation(CPictureData * pictureDat);

		private:
			int RotateToExifOrientation(const int &angle);
			int RotateToOrientation(const int &angle);
			int DectectOrientationByFaceDetector(CPictureData * pictureData);
			static bool isload;
			static mutex muLoading;
		};
	}
}

#endif