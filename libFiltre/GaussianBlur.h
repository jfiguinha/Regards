#pragma once
class CRegardsBitmap;

namespace Regards
{
	namespace FiltreEffet
	{
		class CGaussianBlur
		{
		public:
			CGaussianBlur(){
			width =0;
			height = 0;
			bitmapSize =0;
			temp = nullptr;
			};
			~CGaussianBlur(){};

			int GaussianBlur(CRegardsBitmap * pBitmap, const int &r, const int &boxSize = 3);
			wxImage GaussianBlurwxImage(CRegardsBitmap * pBitmap, const int &r, const int &boxSize = 3);

		private:
			void BoxBlur(uint8_t * & pBitsSrc, uint8_t * & pBitsDest, const int &coeff);
			void BoxBlurH_3(uint8_t * & scl, uint8_t * & tcl, const int &coeff);
			void BoxBlurV_3(uint8_t * & scl, uint8_t * & tcl, const int &coeff);
			vector<int> BoxesForGauss(const int & r, const int & boxSize);

			int width;
			int height;
			int bitmapSize;
			vector<int> gaussCoeff;
			uint8_t * temp;
		};
	}
}