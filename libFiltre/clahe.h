#pragma once
class CRegardsBitmap;

namespace Regards
{
	namespace FiltreEffet
	{
		class CClahe
		{
		public:
			CClahe();
			~CClahe();
			CRegardsBitmap * ClaheFilter(CRegardsBitmap * image_in, int nBins, float clipLevel, int windowSize);

		private:
			CRegardsBitmap * Clahe(CRegardsBitmap * image_in);
			void histogram(CRegardsBitmap * bitmap, int origX,  int origY, int channel, int & count, int * bin, float* binFreq);
			double round(double d);
			inline void ClaheChannel(int * count, int channel, int pos, uint8_t * colorPt, uint8_t * colorPtOut, int * * bin, CRegardsBitmap * image_in, int i, int j);
			int windowSize;
			int nBins;
			float clipLevel;
			int w;
			int h;
			float range;
			float binSize;
		};
	}
}