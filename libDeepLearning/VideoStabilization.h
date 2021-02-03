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
			void BufferFrame(CRegardsBitmap * pBitmap);
			void CorrectFrame(CRegardsBitmap * pictureData);
			void Init();
			int GetNbFrame();
			int GetNbFrameBuffer();
			void SetNbFrameBuffer(const int &nbFrame);
		private:

			COpenCVStabilizationPimpl_ * pimpl;
			int nbFrame = 0;
			int nbFrameBuffer = 0;
			
		};
	}
}
