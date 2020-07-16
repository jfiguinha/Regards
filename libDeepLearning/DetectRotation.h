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
			int GetExifOrientation(CImageLoadingFormat * imageLoadingFormat);
			int GetAngleOrientation(CImageLoadingFormat * imageLoadingFormat);

		private:
			int RotateToExifOrientation(const int &angle);
			int RotateToOrientation(const int &angle);
			int DectectOrientation(CImageLoadingFormat * imageLoadingFormat);
			static bool isload;
			static mutex muLoading;
		};
	}
}

