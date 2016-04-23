// Color.h: interface for the CColor class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <stdint.h>
#include <string>
#include <vector>
using namespace std;

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

			static void YCbCr2RGB(const int &Y, const int &Cb, const int &Cr, int &r, int &g, int &b);
			static void rgb_to_bgrx(const int & w, const int & h, const uint8_t * diskmem, uint8_t * buff);
			static int HSBToRGB(HSB &HSBValue, CRgbaquad &rgb);
			static int RGBToHSB(HSB &HSBValue, CRgbaquad &rgb);
			static int CreateHSBMatrixColorGradient(CRegardsBitmap * m_CRegardsBitmap, const long &m_lHue);
			static int CreateHSBColorGradient(CRegardsBitmap * m_CRegardsBitmap);
			static int CreateColorBitmap(CRegardsBitmap * m_CRegardsBitmap, CRgbaquad m_rgbValue);
			static int CreateColorGradient(CRegardsBitmap * m_CRegardsBitmap, const CRgbaquad colorFrom, const CRgbaquad colorTo);
			static int OneColorBitmap(CRegardsBitmap * m_CRegardsBitmap, CRgbaquad m_rgbValue);

		private:
			static std::vector<wxString> split(const wxString & s, char seperator);
		};
	}
}


