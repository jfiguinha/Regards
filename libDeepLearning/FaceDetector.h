#pragma once
class CRegardsBitmap;
class CPictureData;
class CFace;
class CFaceDetectorPimpl;

namespace Regards
{
	namespace OpenCV
	{


		class CFaceDetector
		{
		public:
			CFaceDetector();
			~CFaceDetector();

			int FindNbFace(CPictureData * pictureData, int & angle);
			void LoadModel(const string &config_file, const string &weight_file, const string &face_recognition, const string &eye_detection);
			std::vector<int> FindFace(CPictureData * pictureData);
			std::vector<int> FindFace(CRegardsBitmap * pBitmap);
			
			void DetectEyes(CRegardsBitmap * pBitmap);

		private:

			CFaceDetectorPimpl * pimpl_;

			bool isload = false;
		};
	}
}
