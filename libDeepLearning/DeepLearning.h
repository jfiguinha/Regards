#pragma once

class CPictureData;

namespace Regards
{
	namespace DeepLearning
	{
		class CDeepLearning
		{
		public:
			static void LoadRessource(const string &config_file, const string &weight_file, const string &face_recognition, const string &rotation_json, const string &eye_detection);
			static vector<int> FindFace(CPictureData * pictureData);
			static bool IsResourceReady();
			static int GetExifOrientation(CPictureData * pictureData);
			static int GetAngleOrientation(CPictureData * pictureData);
			static bool FindFaceCompatible(const int &numFace);
			static std::vector<wxRect> DetectEyes(CPictureData * pictureData);
		};
	}
}