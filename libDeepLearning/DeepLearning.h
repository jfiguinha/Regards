#pragma once

class CPictureData;
class FaceRect;

namespace Regards
{
	namespace DeepLearning
	{
		class CDeepLearning
		{
		public:
			static void LoadRessource(const string& config_file, const string& weight_file, const string& eye_detection, const string& recognition, const string& face_landmark);
			static vector<int> FindFace(CRegardsBitmap * pictureData);
			static void DetectEyes(CRegardsBitmap * pBitmap);

            static int GetExifOrientation(CRegardsBitmap* pBitmap);
            static bool IsResourceReady();
			static bool LockOpenCLDnn();
			static bool UnlockOpenCLDnn();
			static bool FindFaceCompatible(const int &numFace);
		};
	}
}