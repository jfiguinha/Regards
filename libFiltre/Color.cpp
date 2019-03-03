#include <header.h>
// Color.cpp: implementation of the CColor class.
//
//////////////////////////////////////////////////////////////////////
#include "Color.h"
#include "RegardsBitmap.h"
using namespace Regards::FiltreEffet;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CColor::CColor()
{

}

CColor::~CColor()
{

}

CRgbaquad CColor::YUVtoRGB(const CRgbaquad & lYUVColor)
{
	int32_t U, V, R, G, B;
	float Y = lYUVColor.GetFRed();
	U = lYUVColor.GetGreen() - 128;
	V = lYUVColor.GetBlue() - 128;

	//	R = (int32_t)(1.164 * Y + 2.018 * U);
	//	G = (int32_t)(1.164 * Y - 0.813 * V - 0.391 * U);
	//	B = (int32_t)(1.164 * Y + 1.596 * V);
	R = (int32_t)(Y + 1.403f * V);
	G = (int32_t)(Y - 0.344f * U - 0.714f * V);
	B = (int32_t)(Y + 1.770f * U);

	R = min(255, max(0, R));
	G = min(255, max(0, G));
	B = min(255, max(0, B));
	CRgbaquad rgb = CRgbaquad((uint8_t)R, (uint8_t)G, (uint8_t)B, 0);
	return rgb;
}

CRgbaquad CColor::RGBtoYUV(const CRgbaquad &color)
{
	int32_t Y, U, V, R, G, B;
	R = color.GetRed();
	G = color.GetGreen();
	B = color.GetBlue();

	//	Y = (int32_t)( 0.257 * R + 0.504 * G + 0.098 * B);
	//	U = (int32_t)( 0.439 * R - 0.368 * G - 0.071 * B + 128);
	//	V = (int32_t)(-0.148 * R - 0.291 * G + 0.439 * B + 128);
	Y = (int32_t)(0.299f * R + 0.587f * G + 0.114f * B);
	U = (int32_t)((B - Y) * 0.565f + 128);
	V = (int32_t)((R - Y) * 0.713f + 128);

	Y = min(255, max(0, Y));
	U = min(255, max(0, U));
	V = min(255, max(0, V));
	CRgbaquad yuv = CRgbaquad((uint8_t)Y, (uint8_t)U, (uint8_t)V, 0);

	return yuv;
}

std::vector<wxString> CColor::split(const wxString& s, char seperator)
{
	std::vector<wxString> output;

	std::string::size_type prev_pos = 0, pos = 0;

	while ((pos = s.find(seperator, pos)) != std::string::npos)
	{
		wxString substring(s.substr(prev_pos, pos - prev_pos));

		output.push_back(substring);

		prev_pos = ++pos;
	}

	output.push_back(s.substr(prev_pos, pos - prev_pos)); // Last word

	return output;
}


/* in and out must be 16-byte aligned */
void CColor::rgb_to_bgrx(const int & w, const int & h, const uint8_t * diskmem, uint8_t * buff)
{
    int i,j;
    int picptr = 0, srcptr = 0;

	for (j=0; j<h; j++) {
		for (i=0; i<w; i++, picptr+=4, srcptr+=3) {
			memcpy(buff+picptr, diskmem+srcptr, 3 * sizeof(uint8_t));
		}
	}
}


void CColor::YCbCr2RGB(const int &Y,const int &Cb, const int &Cr,int &r, int &g, int &b)
{
	int C=256;
	double c11 = 0.0054980*C;
	double c12 = 0;
	double c13 = 0.0051681*C;
	double c21 = 0.0054980*C;
	double c22 =-0.0015446*C;
	double c23 =-0.0026325*C;
	double c31 = 0.0054980*C;
	double c32 = 0.0079533*C;
	double c33 = 0;

	r=(int)(c11*Y +c12*(Cb-156) +c13*(Cr-137));
	g=(int)(c21*Y +c22*(Cb-156) +c23*(Cr-137));
	b=(int)(c31*Y +c32*(Cb-156) +c33*(Cr-137));

	if(r<0)
		r=0;

	if(g<0)
		g=0;

	if(b<0)
		b=0;

	if(r>255)
		r=255;

	if(g>255)
		g=255;

	if(b>255)
		b=255;

}

int CColor::HSBToRGB(HSB &HSBValue, CRgbaquad &rgb)
{
	if(HSBValue.Saturation > 0)
	{
		double nH = (double)(HSBValue.Hue % 360) / (double)60;
		double nL = (double)HSBValue.Brightness / (double)100;
		double nS = (double)HSBValue.Saturation / (double)100;

		int lH = (int)nH;
		double nF = nH - lH;

	    double nP = nL * (1 - nS);
	    double nQ = nL * (1 - nS * nF);
		double nT = nL * (1 - nS * (1 - nF));

		switch(lH)
		{
			case 0:
				{
					rgb = CRgbaquad(nL * 255,nT * 255,nP * 255);
				}
				break;

			case 1:
				{
					rgb = CRgbaquad(nQ * 255,nL * 255,nP * 255);
				}
				break;

			case 2:
				{
					rgb = CRgbaquad(nP * 255,nL * 255,nT * 255);
				}
				break;

			case 3:
				{
					rgb = CRgbaquad(nP * 255,nQ * 255,nL * 255);
				}
				break;

			case 4:
				{
					rgb = CRgbaquad(nT * 255,nP * 255,nL * 255);
				}
				break;

			case 5:
				{
					rgb = CRgbaquad(nL * 255,nP * 255,nQ * 255);
				}
				break;
		}

	}
	else
	{
		int iRed = (((int)HSBValue.Brightness * 255) / 100);
		rgb = CRgbaquad(iRed,iRed,iRed);
	}

	return 0;
}

int CColor::RGBToHSB(HSB &HSBValue, CRgbaquad &rgb)
{
	float nTemp = 0;
	float lMin = 0, lMax = 0;
	float R = 0, G = 0, B = 0, lDelta = 0;

	R = rgb.GetFRed();
	G = rgb.GetFGreen();
	B = rgb.GetFBlue();
  
	lMax = (R > G) ? ((R > B) ? R : B) : ((G > B) ? G : B);
	lMin = (R < G) ? ((R < B) ? R : B) : ((G < B) ? G : B);

	lDelta = lMax - lMin;

	HSBValue.Brightness = (lMax * 100) / 255;

	if(lMax > 0)
	{
		HSBValue.Saturation = (lDelta / lMax) * 100;
		if(lDelta > 0)
		{
			if(lMax == R)
			{
				nTemp = (G - B) / lDelta;
			}
			else if(lMax == G)
			{
				nTemp = 2 + (B - R) / lDelta;
			}
			else
			{
				nTemp = 4 + (R - G) / lDelta;
			}
		}	
		HSBValue.Hue = nTemp * 60;
		if(HSBValue.Hue < 0)
		{
			HSBValue.Hue = HSBValue.Hue + 360;
		}
	}

	return 0;
}

int CColor::CreateColorGradient(CRegardsBitmap * m_CRegardsBitmap, const CRgbaquad colorFrom, const CRgbaquad colorTo)
{
	//uint8_t * pBitsSrc = m_CRegardsBitmap->GetPtBitmap();

	int bmWidth = m_CRegardsBitmap->GetBitmapWidth();
	int bmHeight = m_CRegardsBitmap->GetBitmapHeight();

	float m_fRedEtape = (float)(colorTo.GetFRed() - colorFrom.GetFRed()) / (float)bmWidth;
	float m_fGreenEtape = (float)(colorTo.GetFGreen() - colorFrom.GetFGreen()) / (float)bmWidth;
	float m_fBlueEtape = (float)(colorTo.GetFBlue() - colorFrom.GetFBlue()) / (float)bmWidth;

	float m_fRedValue = colorFrom.GetFRed();
	float m_fGreenValue = colorFrom.GetFGreen();
	float m_fBlueValue = colorFrom.GetFBlue();

	for(int x = 0;x < bmWidth;x++)
	{
		CRgbaquad m_color = CRgbaquad((int)m_fRedValue,(int)m_fGreenValue,(int)m_fBlueValue);
		for(int y = 0;y < bmHeight;y++)
		{
			m_CRegardsBitmap->SetColorValue(x, y, m_color);
		}

		m_fRedValue += m_fRedEtape;
		m_fGreenValue += m_fGreenEtape;
		m_fBlueValue += m_fBlueEtape;
	}

	//m_CRegardsBitmap->ReCreateHBitmap(false);
	return 0;
}

int CColor::CreateHSBMatrixColorGradient(CRegardsBitmap * m_CRegardsBitmap, const long &m_lHue)
{
	if(m_CRegardsBitmap->GetPtBitmap() != nullptr)
	{
		int bmWidth = m_CRegardsBitmap->GetBitmapWidth();
		int bmHeight = m_CRegardsBitmap->GetBitmapHeight();
		for(int k = 0;k < bmHeight;k++)
		{
			for(int j = 0;j < bmWidth;j++)
			{
				HSB m_value = {m_lHue,(j * 100) / bmHeight,k * 100 / bmWidth};
				CRgbaquad m_rgbValue;
				HSBToRGB(m_value, m_rgbValue);
				m_CRegardsBitmap->SetColorValue(k, j, m_rgbValue);
			}
		}
	}
	return 0;
}

int CColor::CreateColorBitmap(CRegardsBitmap * m_CRegardsBitmap, CRgbaquad m_rgbValue)
{
	if(m_CRegardsBitmap->GetPtBitmap() != nullptr)
	{
		int bmWidth = m_CRegardsBitmap->GetBitmapWidth();
		int bmHeight = m_CRegardsBitmap->GetBitmapHeight();
		//Définition du centre du lens flare

		for(int k = 0;k < bmHeight;k++)
		{
			for(int j = 0;j < bmWidth;j++)
			{
				m_CRegardsBitmap->SetColorValue(k, j, m_rgbValue);
			}
		}
	}
	return 0;
}


int CColor::OneColorBitmap(CRegardsBitmap * m_CRegardsBitmap, CRgbaquad m_rgbValue)
{
	if (m_CRegardsBitmap->GetPtBitmap() != nullptr)
	{
		int bmWidth = m_CRegardsBitmap->GetBitmapWidth();
		int bmHeight = m_CRegardsBitmap->GetBitmapHeight();

		for (auto k = 0; k < bmHeight; k++)
		{
			for (auto j = 0; j < bmWidth; j++)
			{
				CRgbaquad color = m_CRegardsBitmap->GetColorValue(k, j);
				if (color.GetAlpha() != 0)
				{
					m_CRegardsBitmap->SetColorValue(k, j, m_rgbValue);
				}
			}
		}
	}
	return 0;
}

int CColor::CreateHSBColorGradient(CRegardsBitmap * m_CRegardsBitmap)
{
	if(m_CRegardsBitmap->GetPtBitmap() != nullptr)
	{
		int bmWidth = m_CRegardsBitmap->GetBitmapWidth();
		int bmHeight = m_CRegardsBitmap->GetBitmapHeight();

		//Test du gradient de couleur
		for(int k = 0;k < bmHeight;k++)
		{
			HSB m_value = {(k * 360) / bmHeight,100,100};
			CRgbaquad m_rgbValue;
			CColor::HSBToRGB(m_value, m_rgbValue);

			for(int j = 0;j < bmWidth;j++)
			{
				m_CRegardsBitmap->SetColorValue(k, j, m_rgbValue);
			}
		}

	}
	return 0;
}
