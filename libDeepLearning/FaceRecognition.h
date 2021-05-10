#pragma once
class CPictureData;

namespace Regards
{
	namespace OpenCV
	{
		class CFaceRecognition
		{
		public:
			CFaceRecognition();
			~CFaceRecognition();
			
			int FaceRecognition(const int &numFace);
			bool FindCompatibility(const int &numFace);
			int FaceRecognitionEigen(const int& numFace);
		private:

			double GetSimilarity(const cv::Mat& A, const cv::Mat& B);
			float IsCompatibleFace(std::string const& dataface1, std::string const& dataface2);
		};
	}
}