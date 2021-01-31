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
			void Init();
		private:
			COpenCVStabilizationPimpl_ * pimpl;
		};
	}
}
