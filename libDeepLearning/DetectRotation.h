#pragma once

class CImageLoadingFormat;

namespace Regards
{
	namespace OpenCV
	{
		class CDetectRotation
		{
		public:
			CDetectRotation();
			~CDetectRotation();

			static void LoadModel(const string &model_path);
			int GetExifOrientation(CPictureData * pictureDat);
			int GetAngleOrientation(CPictureData * pictureDat);

		private:
			int RotateToExifOrientation(const int &angle);
			int RotateToOrientation(const int &angle);
			int DectectOrientation(CPictureData * pictureDat);
			int DectectOrientationByFaceDetector(CPictureData * pictureData);
			static bool isload;
			static mutex muLoading;
		};
	}
}

