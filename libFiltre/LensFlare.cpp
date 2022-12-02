#include <header.h>
#include "LensFlare.h"

#include "Color.h"
#include <math.h>
#include <cstdlib>
#include "circle.h"
#include "Line.h"
#define CONVRADIAN 0.0174532925 
//const double pi = 3.14159265358979323846264338327950288419716939937510;
using namespace Regards::FiltreEffet;

CLensFlare::CLensFlare(void)
{
	iColorIntensity = 100;
}


CLensFlare::~CLensFlare(void)
{

}

int CLensFlare::InsertwxImage(const wxImage& bitmap, int xPos, int yPos)
{
	if (!pBitmap->empty() && bitmap.IsOk())
	{
		int withwxImage = bitmap.GetWidth();
		int yEnd = yPos + bitmap.GetHeight();
		int xEnd = xPos + bitmap.GetWidth();

		if (yEnd > pBitmap->rows)
			yEnd = pBitmap->rows;

		if (xEnd > pBitmap->cols)
			xEnd = pBitmap->cols;

		uint8_t* data = bitmap.GetData();
		uint8_t* alpha = bitmap.GetAlpha();


		tbb::parallel_for(yPos, yEnd, 1, [=](int y)
			//for (auto y = yPos; y < yEnd; y++)
			{

				for (auto x = xPos; x < xEnd; x++)
				{
					int i = (y - yPos) * withwxImage + (x - xPos);
					CRgbaquad* colorSrc = CRgbaquad::GetPtColorValue(pBitmap, x, y);
					if (colorSrc != nullptr)
					{
						auto color = CRgbaquad(data[i * 3], data[i * 3 + 1], data[i * 3 + 2], alpha[i]);
						float value = color.GetFAlpha() / 255.0f;
						float alphaDiff = 1.0f - value;
						if (alphaDiff < 1.0f)
						{
							colorSrc->Mul(alphaDiff);
							color.Mul(value);
							colorSrc->Add(color);
						}
					}
				}
			});
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CLensFlare::Halo(const int &x, const int &y,const int &iColor, const int &iTaille, const int &iWidth, const float &fAlpha2, const int &iCentre)
{
	int rayon = iTaille;
    

    if(iTaille > 0)
        InsertwxImage(CCircle::Halo(iColor, iColorIntensity, iTaille * 2, iWidth, fAlpha2, iCentre), x - rayon, y - rayon);
}

///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CLensFlare::HaloGradient(const int &x, const int &y, const int &iTaille, const int &iWidth, const float &fAlpha2)
{
	int rayon = iTaille;
    if(iTaille > 0)
        InsertwxImage(CCircle::HaloGradient(iTaille * 2, iWidth, fAlpha2), x - rayon, y - rayon);
}

///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CLensFlare::Circle(const int &x, const int &y,const CRgbaquad &m_color, const int &iTaille, const float &fAlpha)
{
	int rayon = iTaille / 2;
    if(rayon > 0)
        InsertwxImage(CCircle::GenerateCircle(m_color, iTaille, fAlpha), x - rayon, y - rayon);
	
}

///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CLensFlare::CircleGradient(const int &x, const int &y,const CRgbaquad &m_color, const int &iTaille,const float &fAlpha)
{

	int rayon = iTaille;
    if(rayon > 0)
        InsertwxImage(CCircle::GradientTransparent(m_color, iTaille * 2, fAlpha), x - rayon, y - rayon);
}


///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CLensFlare::Burst(const int &x, const int &y,const int &iTaille,const int &iColor, const int &iIntensity, const int &iColorIntensity)
{
	int rayon = iTaille;
    if(rayon > 0)
        InsertwxImage(CCircle::Burst(iTaille * 2, iColor, iIntensity, iColorIntensity), x - rayon, y - rayon);
}



//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void CLensFlare::LensFlare(cv::Mat * pBitmap, const int &iPosX, const int &iPosY, const int &iPuissance, const int &iType, const int &iIntensity, const int &iColor, const int &iColorIntensity)
{
	this->pBitmap = pBitmap;

	int iWidth = pBitmap->size().width;
	int iHeight = pBitmap->size().height;


	//Chargement du lensflare 1
	if (!pBitmap->empty())
	{

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

 		float a = ((float)(y2 - y) / (float)(x2 - x));

 		float b = y - (a * x);

		int iTaille = iRayon;

		int iCouleur1 = iColor + 50;//(iColor + 50) % 360;
		if(iCouleur1 > 360)
			iCouleur1 -= 360;

		int iCouleur2 = iColor + 100;//(iColor + 100) % 360;
		if(iCouleur2 > 360)
			iCouleur2 -= 360;

		int iCouleur3 = iColor + 200;//(iColor + 200) % 360;
		if(iCouleur3 > 360)
			iCouleur3 -= 360;


		HSB m_value2 = {iCouleur3,100,100};
		CRgbaquad m_rgbValue3;
		CColor::HSBToRGB(m_value2,m_rgbValue3);
		
		HSB m_value = {iCouleur1,100,100};
		CRgbaquad m_rgbValue1;
		CColor::HSBToRGB(m_value,m_rgbValue1);

		HSB m_value3 = {iCouleur2,100,100};
		CRgbaquad m_rgbValue2;
		CColor::HSBToRGB(m_value3,m_rgbValue2);

		HSB m_value4 = {iColor,iColorIntensity,100};
		CRgbaquad m_rgbValue4;
		CColor::HSBToRGB(m_value4,m_rgbValue4);

		float fRayon = iRayon*(iIntensity / 10.0f);


		iRayon = iRayon * 0.75f;
		
		//return;

		////////////////////////////////////////////////////////////////
		
		

		//////////////////////////////////////////////////////////////////
		//1er Halo
		//////////////////////////////////////////////////////////////////

		x = iPosX + (iLargeur * 0.875f);
		y = iPosY + (iHauteur * 0.875f);

		iTaille = iRayon;
		
		//y = a * x + b;

		Halo(x,y,iCouleur1,iTaille,8,0.7f);

		//////////////////////////////////////////////////////////////////
		//1er Halo
		//////////////////////////////////////////////////////////////////

		x = iPosX - (iLargeur*0.125f);
		y = iPosY - (iHauteur*0.125f);

		iTaille = iRayon >> 1;
		
		//y = a * x + b;

		Halo(x,y,iCouleur2,iTaille,5,0.7f);


		////////////////////////////////////////////////////////////////
		//Small Burst
		////////////////////////////////////////////////////////////////
		
		x = iPosX + (iLargeur >>1);
		y = iPosY + (iHauteur >>1);

		iTaille = iRayon >> 3;

		Burst(x, y,iTaille,iColor,25,100);


		////////////////////////////////////////////////////////////////
		//Deuxieme Burst
		////////////////////////////////////////////////////////////////


		x = iPosX + (iLargeur * 0.625f);
		y = iPosY + (iHauteur * 0.625f);

		iTaille = iRayon / 10;

		Burst(x, y,iTaille,iCouleur2);

		x = (iPosX + (iLargeur * 0.75f));
		y = (iPosY + (iHauteur * 0.75f));

		iTaille = iRayon >> 3;

		Burst(x, y,iTaille,iCouleur3,25,100);
		

		////////////////////////////////////////////////////////////////

		//x = (iPosX + (iLargeur * 1.125f));
		//y = (iPosY + (iHauteur * 1.125f));

		iTaille = iRayon << 2;
		
		HaloGradient(x2,y2,iTaille,iWidth/20,0.7f);

		////////////////////////////////////////////////////////////////

		
		x = (iPosX + (iLargeur * 0.625f));
		//y = (iPosY + (iHauteur * 0.625f));

		if(iLargeur > 0)
			x += iRayon >> 1;
		else
			x -= iRayon >> 1;

		y = a * x + b;

		Circle(x, y, m_rgbValue1, iRayon*0.75, 0.8f);

		x = (iPosX + (iLargeur * 0.625f));
		//y = (iPosY + (iHauteur * 6 / 8));

		if(iLargeur > 0)
			x += iRayon >> 1;
		else
			x -= iRayon >> 1;

		y = a * x + b;

		Circle(x, y, m_rgbValue1, iRayon / 2, 0.8f);

		x = (iPosX + (iLargeur * 0.625f));
		//y = (iPosY + (iHauteur * 6 / 8));

		if(iLargeur > 0)
			x += iRayon*0.6f;
		else
			x -= iRayon*0.6f;

		y = a * x + b;

		Circle(x, y, m_rgbValue1, iRayon / 4, 0.8f);

		
		x = (iPosX + (iLargeur * 0.4f));
		//y = (iPosY + (iHauteur * 0.4f));

		if(iLargeur > 0)
			x -= (iRayon >> 3);
		else
			x += (iRayon >> 3);

		y = a * x + b;

		Circle(x, y, m_rgbValue1, iRayon / 4, 0.8f);


		x = (iPosX + (iLargeur / 5));
		y = (iPosY + (iHauteur / 5));

		Circle(x, y, m_rgbValue3, iRayon*0.75, 0.8f);

		if(iLargeur > 0)
			x += iRayon*0.2f;
		else
			x -= iRayon*0.2f;

		y = a * x + b;

		Circle(x, y, m_rgbValue3, iRayon*0.4, 0.8f);

		x = (iPosX + (iLargeur / 5));
		//y = (iPosY + (iHauteur / 5));

		if(iLargeur > 0)
			x -= iRayon*0.2f;
		else
			x += iRayon*0.2f;

		y = a * x + b;

		Circle(x, y, m_rgbValue3, iRayon*0.2, 0.8f);

		x = (iPosX + (iLargeur * 0.75f));
		y = (iPosY + (iHauteur * 0.75f));

		Circle(x, y, m_rgbValue2, iRayon*0.2, 0.8f);
		
		x = iPosX;
		y = iPosY;

		iTaille = fRayon * 0.5f;
		

		CircleGradient(x, y, m_rgbValue4, fRayon);

		//x = iPosX;
		//y = iPosY;

		Halo(x,y,iColor,iTaille,8,0.8f,0);
		

		Burst(x, y,iTaille*0.9f,iColor,iIntensity,iColorIntensity);

		//Trait lumineux
		
		int iIntRayon = (int)fRayon/2;
		if (iIntRayon == 0)
			iIntRayon = 1;

		CLine line(iHeight, iWidth);
		
		for (i = 0; i <= 360; i++) 
		{
			
			float fxValue = cos(i * CONVRADIAN) * rand();
			float fyValue = sin(i * CONVRADIAN) * rand();

			fxValue = (int)fxValue % (iIntRayon);
			fyValue = (int)fyValue % (iIntRayon);
			
			line.MidpointLine(pBitmap, x, y, x + fxValue, y + fyValue, CRgbaquad(255, 255, 255), 0.9f, true);
			line.MidpointLine(pBitmap, x - fxValue, y - fyValue, x, y, CRgbaquad(255, 255, 255), 0.9f, true);
		}

		
		if(iLargeur > 0)
			x = iPosX - iRayon;
		else
			x = iPosX + iRayon;

		y = a*x + b;
		

		Circle(x, y, m_rgbValue1, iRayon * 4, 0.95f);
		
		//pBitmap->ReCreateHBitmap(false);

	}
}
