#pragma once

class CPictureData;
class FaceRect;
class CRegardsBitmap;

namespace Regards
{
	namespace DeepLearning
	{
		class CDeepLearning
		{
		public:
			static bool LockOpenCLDnn();
			static bool UnlockOpenCLDnn();

			static void LoadRessource(const string& config_file, const string& weight_file, const string& recognition,
			                          const string& face_landmark, const string& protoPosition, const string& weightPosition);
			static vector<int> FindFace(CRegardsBitmap* pictureData);
			static void DetectEyes(CRegardsBitmap* pBitmap);

			static int GetExifOrientation(CRegardsBitmap* pBitmap);
			static bool IsResourceReady();
			static bool FindFaceCompatible(const int& numFace);
		};
	}
}
