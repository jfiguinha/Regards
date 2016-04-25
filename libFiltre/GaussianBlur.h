#pragma once
#include <stdint.h>
#include <vector>
#include "RegardsBitmap.h"
using namespace std;

namespace Regards
{
	namespace FiltreEffet
	{
		class CGaussianBlur
		{
		public:
			CGaussianBlur(){};
			~CGaussianBlur(){};

			int GaussianBlur(CRegardsBitmap * pBitmap, const int &r);

		private:
			void BoxBlur(uint8_t * & pBitsSrc, uint8_t * & pBitsDest, const int &coeff);
			void BoxBlurH_3(uint8_t * & scl, uint8_t * & tcl, const int &coeff);
			void BoxBlurV_3(uint8_t * & scl, uint8_t * & tcl, const int &coeff);
			void BoxBlurH_4(uint8_t * & pBitsSrc, uint8_t * & pBitsDest, const int &coeff);
			void BoxBlurV_4(uint8_t * & pBitsSrc, uint8_t * & pBitsDest, const int &coeff);
			vector<int> BoxesForGauss(const int & r, const int & boxSize);

			int width =0;
			int height = 0;
			int bitmapSize =0;
			vector<int> gaussCoeff;
		};
	}
}