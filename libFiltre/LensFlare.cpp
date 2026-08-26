#include <header.h>
#include "LensFlare.h"

#include "Color.h"
#include <math.h>
#include <cstdlib>
#include <random>
#include "circle.h"
#include "Line.h"
#define CONVRADIAN 0.0174532925
using namespace Regards::FiltreEffet;

CLensFlare::CLensFlare(void)
{
	iColorIntensity = 100;
}


CLensFlare::~CLensFlare(void)
{}

int CLensFlare::InsertwxImage(const wxImage& bitmap, int xPos, int yPos)
{
	if (pBitmap == nullptr || pBitmap->empty() || !bitmap.IsOk())
		return 0;

	const int srcWidth = bitmap.GetWidth();
	const int srcHeight = bitmap.GetHeight();

	// Décalage à appliquer côté source si xPos/yPos sont négatifs
	// (flare positionné partiellement hors du cadre de l'image).
	int srcStartX = 0;
	int srcStartY = 0;

	if (xPos < 0)
	{
		srcStartX = -xPos;
		xPos = 0;
	}
	if (yPos < 0)
	{
		srcStartY = -yPos;
		yPos = 0;
	}

	int xEnd = xPos + (srcWidth - srcStartX);
	int yEnd = yPos + (srcHeight - srcStartY);

	if (xEnd > pBitmap->cols)
		xEnd = pBitmap->cols;
	if (yEnd > pBitmap->rows)
		yEnd = pBitmap->rows;

	if (xEnd <= xPos || yEnd <= yPos)
		return 0;

	const uint8_t* data = bitmap.GetData();
	const uint8_t* alpha = bitmap.HasAlpha() ? bitmap.GetAlpha() : nullptr;

	tbb::parallel_for(yPos, yEnd, 1, [=](int y)
		{
			const int srcY = srcStartY + (y - yPos);
			for (int x = xPos; x < xEnd; x++)
			{
				const int srcX = srcStartX + (x - xPos);
				const int i = srcY * srcWidth + srcX;

				CRgbaquad* colorSrc = CRgbaquad::GetPtColorValue(pBitmap, x, y);
				if (colorSrc == nullptr)
					continue;

				const uint8_t a = (alpha != nullptr) ? alpha[i] : 255;
				auto color = CRgbaquad(data[i * 3], data[i * 3 + 1], data[i * 3 + 2], a);
				const float value = color.GetFAlpha() / 255.0f;
				const float alphaDiff = 1.0f - value;
				if (alphaDiff < 1.0f)
				{
					colorSrc->Mul(alphaDiff);
					color.Mul(value);
					colorSrc->Add(color);
				}
			}
		});

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CLensFlare::Halo(int x, int y, int iColor, int iTaille, int iWidth,
	float fAlpha2, int iCentre)
{
	int rayon = iTaille;

	if (iTaille > 0)
		InsertwxImage(CCircle::Halo(iColor, iColorIntensity, iTaille * 2, iWidth, fAlpha2, iCentre), x - rayon,
			y - rayon);
}

///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CLensFlare::HaloGradient(int x, int y, int iTaille, int iWidth, float fAlpha2)
{
	int rayon = iTaille;
	if (iTaille > 0)
		InsertwxImage(CCircle::HaloGradient(iTaille * 2, iWidth, fAlpha2), x - rayon, y - rayon);
}

///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CLensFlare::Circle(int x, int y, const CRgbaquad& m_color, int iTaille, float fAlpha)
{
	int rayon = iTaille / 2;
	if (rayon > 0)
		InsertwxImage(CCircle::GenerateCircle(m_color, iTaille, fAlpha), x - rayon, y - rayon);
}

///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CLensFlare::CircleGradient(int x, int y, const CRgbaquad& m_color, int iTaille, float fAlpha)
{
	int rayon = iTaille;
	if (rayon > 0)
		InsertwxImage(CCircle::GradientTransparent(m_color, iTaille * 2, fAlpha), x - rayon, y - rayon);
}


///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CLensFlare::Burst(int x, int y, int iTaille, int iColor, int iIntensity, int iColorIntensity)
{
	int rayon = iTaille;
	if (rayon > 0)
		InsertwxImage(CCircle::Burst(iTaille * 2, iColor, iIntensity, iColorIntensity), x - rayon, y - rayon);
}


//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void CLensFlare::LensFlare(cv::Mat* pBitmap, int iPosX, int iPosY, int iPuissance,
	int iType, int iIntensity, int iColor, int iColorIntensity)
{
	this->pBitmap = pBitmap;

	if (pBitmap == nullptr || pBitmap->empty())
		return;

	int iWidth = pBitmap->size().width;
	int iHeight = pBitmap->size().height;

	int i = 0;

	int x = iPosX;
	int y = iPosY;

	int iRayon = iPuissance;

	int iMaxX = iWidth >> 1;
	int iMaxY = iHeight >> 1;

	this->iColorIntensity = iColorIntensity;

	int iLargeur = (x - (iMaxX)) << 1;
	iLargeur = -iLargeur;

	int iHauteur = (y - (iMaxY)) << 1;
	iHauteur = -iHauteur;

	int x2 = iLargeur + x;
	int y2 = iHauteur + y;

	// Cas dégénéré : flare positionné exactement au centre de l'image (x2 == x).
	// On évite la division par zéro qui produirait des coordonnées NaN propagées
	// dans tous les Circle(...) suivants via y = a * x + b.
	float a = 0.0f;
	float b = static_cast<float>(y);
	if (x2 != x)
	{
		a = (static_cast<float>(y2 - y) / static_cast<float>(x2 - x));
		b = y - (a * x);
	}

	int iTaille = iRayon;

	int iCouleur1 = iColor + 50;
	if (iCouleur1 > 360)
		iCouleur1 -= 360;

	int iCouleur2 = iColor + 100;
	if (iCouleur2 > 360)
		iCouleur2 -= 360;

	int iCouleur3 = iColor + 200;
	if (iCouleur3 > 360)
		iCouleur3 -= 360;


	HSB m_value2 = { iCouleur3, 100, 100 };
	CRgbaquad m_rgbValue3;
	CColor::HSBToRGB(m_value2, m_rgbValue3);

	HSB m_value = { iCouleur1, 100, 100 };
	CRgbaquad m_rgbValue1;
	CColor::HSBToRGB(m_value, m_rgbValue1);

	HSB m_value3 = { iCouleur2, 100, 100 };
	CRgbaquad m_rgbValue2;
	CColor::HSBToRGB(m_value3, m_rgbValue2);

	HSB m_value4 = { iColor, iColorIntensity, 100 };
	CRgbaquad m_rgbValue4;
	CColor::HSBToRGB(m_value4, m_rgbValue4);

	float fRayon = iRayon * (iIntensity / 10.0f);

	iRayon = iRayon * 0.75f;

	////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////
	//1er Halo
	//////////////////////////////////////////////////////////////////

	x = iPosX + (iLargeur * 0.875f);
	y = iPosY + (iHauteur * 0.875f);

	iTaille = iRayon;

	Halo(x, y, iCouleur1, iTaille, 8, 0.7f);

	//////////////////////////////////////////////////////////////////
	//2eme Halo
	//////////////////////////////////////////////////////////////////

	x = iPosX - (iLargeur * 0.125f);
	y = iPosY - (iHauteur * 0.125f);

	iTaille = iRayon >> 1;

	Halo(x, y, iCouleur2, iTaille, 5, 0.7f);


	////////////////////////////////////////////////////////////////
	//Small Burst
	////////////////////////////////////////////////////////////////

	x = iPosX + (iLargeur >> 1);
	y = iPosY + (iHauteur >> 1);

	iTaille = iRayon >> 3;

	Burst(x, y, iTaille, iColor, 25, 100);


	////////////////////////////////////////////////////////////////
	//Deuxieme Burst
	////////////////////////////////////////////////////////////////

	x = iPosX + (iLargeur * 0.625f);
	y = iPosY + (iHauteur * 0.625f);

	iTaille = iRayon / 10;

	Burst(x, y, iTaille, iCouleur2);

	x = (iPosX + (iLargeur * 0.75f));
	y = (iPosY + (iHauteur * 0.75f));

	iTaille = iRayon >> 3;

	Burst(x, y, iTaille, iCouleur3, 25, 100);


	////////////////////////////////////////////////////////////////

	iTaille = iRayon << 2;

	HaloGradient(x2, y2, iTaille, iWidth / 20, 0.7f);

	////////////////////////////////////////////////////////////////

	x = (iPosX + (iLargeur * 0.625f));

	if (iLargeur > 0)
		x += iRayon >> 1;
	else
		x -= iRayon >> 1;

	y = a * x + b;

	Circle(x, y, m_rgbValue1, iRayon * 0.75, 0.8f);

	x = (iPosX + (iLargeur * 0.625f));

	if (iLargeur > 0)
		x += iRayon >> 1;
	else
		x -= iRayon >> 1;

	y = a * x + b;

	Circle(x, y, m_rgbValue1, iRayon / 2, 0.8f);

	x = (iPosX + (iLargeur * 0.625f));

	if (iLargeur > 0)
		x += iRayon * 0.6f;
	else
		x -= iRayon * 0.6f;

	y = a * x + b;

	Circle(x, y, m_rgbValue1, iRayon / 4, 0.8f);


	x = (iPosX + (iLargeur * 0.4f));

	if (iLargeur > 0)
		x -= (iRayon >> 3);
	else
		x += (iRayon >> 3);

	y = a * x + b;

	Circle(x, y, m_rgbValue1, iRayon / 4, 0.8f);


	x = (iPosX + (iLargeur / 5));
	y = (iPosY + (iHauteur / 5));

	Circle(x, y, m_rgbValue3, iRayon * 0.75, 0.8f);

	if (iLargeur > 0)
		x += iRayon * 0.2f;
	else
		x -= iRayon * 0.2f;

	y = a * x + b;

	Circle(x, y, m_rgbValue3, iRayon * 0.4, 0.8f);

	x = (iPosX + (iLargeur / 5));

	if (iLargeur > 0)
		x -= iRayon * 0.2f;
	else
		x += iRayon * 0.2f;

	y = a * x + b;

	Circle(x, y, m_rgbValue3, iRayon * 0.2, 0.8f);

	x = (iPosX + (iLargeur * 0.75f));
	y = (iPosY + (iHauteur * 0.75f));

	Circle(x, y, m_rgbValue2, iRayon * 0.2, 0.8f);

	x = iPosX;
	y = iPosY;

	iTaille = fRayon * 0.5f;

	CircleGradient(x, y, m_rgbValue4, fRayon);

	Halo(x, y, iColor, iTaille, 8, 0.8f, 0);

	Burst(x, y, iTaille * 0.9f, iColor, iIntensity, iColorIntensity);

	//Trait lumineux

	int iIntRayon = static_cast<int>(fRayon) / 2;
	if (iIntRayon == 0)
		iIntRayon = 1;

	CLine line(iHeight, iWidth);

	static thread_local std::mt19937 s_rng{ std::random_device{}() };
	std::uniform_int_distribution<int> dist(0, RAND_MAX);

	for (i = 0; i <= 360; i++)
	{
		float fxValue = cos(i * CONVRADIAN) * dist(s_rng);
		float fyValue = sin(i * CONVRADIAN) * dist(s_rng);

		fxValue = static_cast<int>(fxValue) % (iIntRayon);
		fyValue = static_cast<int>(fyValue) % (iIntRayon);

		line.MidpointLine(pBitmap, x, y, x + fxValue, y + fyValue, CRgbaquad(255, 255, 255), 0.9f, true);
		line.MidpointLine(pBitmap, x - fxValue, y - fyValue, x, y, CRgbaquad(255, 255, 255), 0.9f, true);
	}

	if (iLargeur > 0)
		x = iPosX - iRayon;
	else
		x = iPosX + iRayon;

	y = a * x + b;

	Circle(x, y, m_rgbValue1, iRayon * 4, 0.95f);
}