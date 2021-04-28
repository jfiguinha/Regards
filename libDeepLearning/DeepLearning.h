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
#ifdef __APPLE__
            static vector<int> FindFace(CRegardsBitmap * pictureData, vector<FaceRect> & listFace);
            static void DetectEyes(CRegardsBitmap * pBitmap, vector<FaceRect> & listFace);
            static void LoadRessource(const string &eye_detection);
#else
			static void LoadRessource(const string &config_file, const string &weight_file, const string &face_recognition, const string &rotation_json, const string &eye_detection);
			static vector<int> FindFace(CRegardsBitmap * pictureData);
			static void DetectEyes(CRegardsBitmap * pBitmap);
            static int GetExifOrientation(CRegardsBitmap* pBitmap);
            static bool IsResourceReady();
#endif
            
			static bool FindFaceCompatible(const int &numFace);
		};
	}
}