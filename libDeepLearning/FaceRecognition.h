#pragma once

namespace Regards
{
	namespace OpenCV
	{
		class CFaceRecognition
		{
		public:
			CFaceRecognition();
			~CFaceRecognition();
			
			int FaceRecognition(CPictureData * pictureData, const int &numFace);
			bool FindCompatibility(const int &numFace);

		private:
			float IsCompatibleFace(std::string const& dataface1, std::string const& dataface2);
		};
	}
}