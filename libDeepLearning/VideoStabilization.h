#pragma once

class COpenCVStabilizationPimpl_;
class CRegardsBitmap;

namespace Regards
{
	namespace OpenCV
	{
		
		class COpenCVStabilization
		{
		public:
			COpenCVStabilization();
			~COpenCVStabilization();
			void AddFrame(CRegardsBitmap * pictureData);
			void CalculStabilization();
			void CorrectFrame(CRegardsBitmap * pictureData, int i);
			void Init(const int &framerate);
		private:
			COpenCVStabilizationPimpl_ * pimpl;
			
		};
	}
}
