#include <header.h>
#include "Line.h"
#include <RGBAQuad.h>

CLine::CLine(const int& heightMax, const int& widthMax)
{
	this->heightMax = heightMax;
	this->widthMax = widthMax;
}


CLine::~CLine()
{
}


///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CLine::MidpointLine(cv::Mat* bitmap, const int& xFrom, const int& yFrom, const int& xTo, const int& yTo,
                         const CRgbaquad& m_color, const float& fAlpha, const bool& m_bAntialiasing)
{
	int x, y;

	auto a = (static_cast<float>(yTo - yFrom) / static_cast<float>(xTo - xFrom));
	auto b = yFrom - (a * xFrom);

	x = xFrom;
	y = yFrom;

	SetAlphaColorValue(x, y, x, y, fAlpha, m_color, bitmap);

	while (x < xTo)
	{
		x++;

		const float fValue = a * x + b;

		//Définition de l'antialiasing

		y = static_cast<int>(fValue);

		SetAlphaColorValue(x, y, x, y, fAlpha, m_color, bitmap);
	}
}


///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CLine::SetAlphaColorValue(const int& xFrom, const int& yFrom, const int& x, const int& y, const float& fAlpha,
                               const CRgbaquad& m_color, cv::Mat* pBitmap)
{
	float fDifference = 1.0f - fAlpha;

	int iXMin = pBitmap->size().width - widthMax;
	int iYMin = pBitmap->size().height - heightMax;

	if (xFrom >= widthMax || xFrom < iXMin || x < iXMin || x >= widthMax)
		return;

	if (yFrom >= heightMax || yFrom < iYMin || y < iYMin || y >= heightMax)
		return;

	if (iYMin < 0)
	{
		if (y >= heightMax + iYMin)
			return;
	}

	if (iXMin < 0)
	{
		if (x >= widthMax + iXMin)
			return;
	}

	CRgbaquad* m_color2 = CRgbaquad::GetPtColorValue(pBitmap, xFrom, yFrom);
	m_color2->SetRed(m_color.GetFRed() * fDifference + m_color2->GetFRed() * fAlpha);
	m_color2->SetGreen(m_color.GetFGreen() * fDifference + m_color2->GetFGreen() * fAlpha);
	m_color2->SetBlue(m_color.GetFBlue() * fDifference + m_color2->GetFBlue() * fAlpha);
}
