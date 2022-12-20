// Color.h: interface for the CColor class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CRgbaquad;
class CRegardsBitmap;

struct HSB
{
	long Hue;
	long Saturation;
	long Brightness;
};

namespace Regards
{
	namespace FiltreEffet
	{
		class CColor  
		{
		public:
			CColor();
			virtual ~CColor();


			static CRgbaquad RGBtoYUV(const CRgbaquad &color);
			static CRgbaquad YUVtoRGB(const CRgbaquad & lYUVColor);
			static int HSBToRGB(HSB &HSBValue, CRgbaquad &rgb);
			static int RGBToHSB(HSB &HSBValue, CRgbaquad &rgb);

		};
	}
}


