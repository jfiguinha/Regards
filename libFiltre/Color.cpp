#include <header.h>
// Color.cpp: implementation of the CColor class.
//
//////////////////////////////////////////////////////////////////////
#include "Color.h"
#include <RGBAQuad.h>
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

CRgbaquad CColor::YUVtoRGB(const CRgbaquad& lYUVColor)
{
	int32_t U, V, R, G, B;
	float Y = lYUVColor.GetFRed();
	U = lYUVColor.GetGreen() - 128;
	V = lYUVColor.GetBlue() - 128;

	//	R = (int32_t)(1.164 * Y + 2.018 * U);
	//	G = (int32_t)(1.164 * Y - 0.813 * V - 0.391 * U);
	//	B = (int32_t)(1.164 * Y + 1.596 * V);
	R = static_cast<int32_t>(Y + 1.403f * V);
	G = static_cast<int32_t>(Y - 0.344f * U - 0.714f * V);
	B = static_cast<int32_t>(Y + 1.770f * U);

	R = min(255, max(0, R));
	G = min(255, max(0, G));
	B = min(255, max(0, B));
	auto rgb = CRgbaquad(static_cast<uint8_t>(R), static_cast<uint8_t>(G), static_cast<uint8_t>(B), 0);
	return rgb;
}

CRgbaquad CColor::RGBtoYUV(const CRgbaquad& color)
{
	int32_t Y, U, V, R, G, B;
	R = color.GetRed();
	G = color.GetGreen();
	B = color.GetBlue();

	//	Y = (int32_t)( 0.257 * R + 0.504 * G + 0.098 * B);
	//	U = (int32_t)( 0.439 * R - 0.368 * G - 0.071 * B + 128);
	//	V = (int32_t)(-0.148 * R - 0.291 * G + 0.439 * B + 128);
	Y = static_cast<int32_t>(0.299f * R + 0.587f * G + 0.114f * B);
	U = static_cast<int32_t>((B - Y) * 0.565f + 128);
	V = static_cast<int32_t>((R - Y) * 0.713f + 128);

	Y = min(255, max(0, Y));
	U = min(255, max(0, U));
	V = min(255, max(0, V));
	auto yuv = CRgbaquad(static_cast<uint8_t>(Y), static_cast<uint8_t>(U), static_cast<uint8_t>(V), 0);

	return yuv;
}

int CColor::HSBToRGB(HSB& HSBValue, CRgbaquad& rgb)
{
	if (HSBValue.Saturation > 0)
	{
		double nH = static_cast<double>(HSBValue.Hue % 360) / static_cast<double>(60);
		double nL = static_cast<double>(HSBValue.Brightness) / static_cast<double>(100);
		double nS = static_cast<double>(HSBValue.Saturation) / static_cast<double>(100);

		int lH = static_cast<int>(nH);
		double nF = nH - lH;

		double nP = nL * (1 - nS);
		double nQ = nL * (1 - nS * nF);
		double nT = nL * (1 - nS * (1 - nF));

		switch (lH)
		{
		case 0:
			{
				rgb = CRgbaquad(nL * 255, nT * 255, nP * 255);
			}
			break;

		case 1:
			{
				rgb = CRgbaquad(nQ * 255, nL * 255, nP * 255);
			}
			break;

		case 2:
			{
				rgb = CRgbaquad(nP * 255, nL * 255, nT * 255);
			}
			break;

		case 3:
			{
				rgb = CRgbaquad(nP * 255, nQ * 255, nL * 255);
			}
			break;

		case 4:
			{
				rgb = CRgbaquad(nT * 255, nP * 255, nL * 255);
			}
			break;

		case 5:
			{
				rgb = CRgbaquad(nL * 255, nP * 255, nQ * 255);
			}
			break;
		default: ;
		}
	}
	else
	{
		int iRed = ((static_cast<int>(HSBValue.Brightness) * 255) / 100);
		rgb = CRgbaquad(iRed, iRed, iRed);
	}

	return 0;
}

int CColor::RGBToHSB(HSB& HSBValue, CRgbaquad& rgb)
{
	float r = rgb.GetFRed();
	float G = rgb.GetFGreen();
	float B = rgb.GetFBlue();

	const float lMax = (r > G) ? ((r > B) ? r : B) : ((G > B) ? G : B);
	const float lMin = (r < G) ? ((r < B) ? r : B) : ((G < B) ? G : B);

	const float lDelta = lMax - lMin;

	HSBValue.Brightness = (lMax * 100) / 255;

	if (lMax > 0)
	{
		float n_temp = 0;
		HSBValue.Saturation = (lDelta / lMax) * 100;
		if (lDelta > 0)
		{
			if (lMax == r)
			{
				n_temp = (G - B) / lDelta;
			}
			else if (lMax == G)
			{
				n_temp = 2 + (B - r) / lDelta;
			}
			else
			{
				n_temp = 4 + (r - G) / lDelta;
			}
		}
		HSBValue.Hue = n_temp * 60;
		if (HSBValue.Hue < 0)
		{
			HSBValue.Hue = HSBValue.Hue + 360;
		}
	}

	return 0;
}
