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
			static void CleanRecognition();
			static void LoadRessource(const bool& openCLCompatible);
			static vector<int> FindFace(const cv::Mat& pictureData, const wxString& filename,
			                            const bool& fastDetection);
			static void DetectEyes(const cv::Mat& pBitmap, const bool& fastDetection);

			static int GetExifOrientation(const cv::Mat& pBitmap, const bool& fastDetection);
			static bool IsResourceReady();
			static bool FindFaceCompatible(const int& numFace, const bool& fastDetection);
		};
	}
}
