#pragma once

class CPictureData;

namespace Regards
{
	namespace DeepLearning
	{
		class CDeepLearning
		{
		public:
			static int GetExifOrientation(CPictureData * pictureData);
			static int GetAngleOrientation(CPictureData * pictureData);
			static bool FindFaceCompatible(const int &numFace);
		};
	}
}