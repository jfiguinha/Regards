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
			COpenCVStabilization(const int &nbFrame);
			~COpenCVStabilization();
			void AddFrame(CRegardsBitmap * pictureData);
			void BufferFrame(CRegardsBitmap * pBitmap, const int & pos);
			void CorrectFrame(CRegardsBitmap * pictureData);
			void CalculTransformation();
			int GetNbFrame();
		private:

			COpenCVStabilizationPimpl_ * pimpl;
			int nbFrame = 0;
			
		};
	}
}
