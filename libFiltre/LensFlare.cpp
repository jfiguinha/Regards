#include "LensFlare.h"
#include "RegardsBitmap.h"
#include "Color.h"
#include <math.h>
#include <cstdlib>
#define CONVRADIAN 0.0174532925 
const double pi = 3.14159265358979323846264338327950288419716939937510;
using namespace Regards::FiltreEffet;

CLensFlare::CLensFlare(void)
{
	m_lSize = 0;
	iTypeTransfert = 0;
	iBilinearFiltering = 0;
	iWidthMax = 0;
	iHeightMax = 0;
	iColorIntensity = 100;

}


CLensFlare::~CLensFlare(void)
{
}




long CLensFlare::CalculPos(long x, long y, long width)
{
	return ((y * width * 3) + x * 3);
}

void CLensFlare::AlphaBlending(uint8_t * pBitsSrc,const int &iPosSrc, uint8_t * pBitSrc2, const int &iPosSrc2, uint8_t * pBitDest, const int &iPosDest, const int &alpha)
{
	if(*(pBitSrc2 + iPosSrc2) == 0 && *(pBitSrc2 + iPosSrc2 + 1) == 0 && *(pBitSrc2 + iPosSrc2 + 2) == 0)
		return;

	*(pBitDest + iPosDest) = *(pBitsSrc + iPosSrc) + ((*(pBitSrc2 + iPosSrc2) * alpha) >> 8);
	*(pBitDest + iPosDest + 1) = *(pBitsSrc + iPosSrc + 1) + ((*(pBitSrc2 + iPosSrc2 + 1) * alpha) >> 8);
	*(pBitDest + iPosDest + 2) = *(pBitsSrc + iPosSrc + 2) + ((*(pBitSrc2 + iPosSrc2 + 2) * alpha) >> 8);

	if(*(pBitDest + iPosDest) > 255)
		*(pBitDest + iPosDest) = 255;

	if(*(pBitDest + iPosDest + 1) > 255)
		*(pBitDest + iPosDest + 1) = 255;

	if(*(pBitDest + iPosDest + 2) > 255)
		*(pBitDest + iPosDest + 2) = 255;

}

void CLensFlare::AdditiveBlending(uint8_t * pBitsSrc,const int &iPosSrc, uint8_t * pBitSrc2, const int &iPosSrc2, uint8_t * pBitDest, const int &iPosDest, const int &alpha)
{
	unsigned char alut [512];

	for (int i = 0; i < 512; i++)
	{
	  alut [i] = i;
	  if (i > 255) alut [i] = 255;
	}

	*(pBitDest + iPosDest) = alut[*(pBitsSrc + iPosSrc) + *(pBitSrc2 + iPosSrc2)];
	*(pBitDest + iPosDest + 1) = alut[*(pBitsSrc + iPosSrc + 1) + *(pBitSrc2 + iPosSrc2 + 1)];
	*(pBitDest + iPosDest + 2) = alut[*(pBitsSrc + iPosSrc + 2) + *(pBitSrc2 + iPosSrc2 + 2)];
}


///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CLensFlare::Halo(const int &x, const int &y,const int &iColor, const int &iTaille, const int &iWidth, const float &fAlpha2, const int &iCentre)
{
	iTypeTransfert = 1;

	float y1 = 0.3f;
	float x1 = 0.5f;
	float a = (y1 - 1.0f) / (x1 * (x1 - 1.0f));
	float b = 1.0f - a;
	int i = 0;

	if(iCentre)
	{				
		for(i = 0;i < iTaille - iWidth;i++)
		{
			float fAlpha = ((float)i / (float)(iTaille));
			fAlpha = (a * (fAlpha * fAlpha) + b * fAlpha);

			if(fAlpha < 0.0f)
				fAlpha = 0.0f;

			HSB m_value = {iColor,static_cast<long>((fAlpha * iColorIntensity)),100};
			CRgbaquad m_rgbValue;
			CColor::HSBToRGB(m_value,m_rgbValue);		

			MidpointCircle(x,y,i,m_rgbValue,fAlpha2);
		}
	}

	const double m_dCent = 100;

	for(i = iTaille - iWidth;i < iTaille;i++)
	{
		
		float fAlpha;
		int j = 100 - i;
		if(j < 0)
			fAlpha = 1.0f;
		else
		{		
			double m_iIntensity = ((double)((double)(i) / m_dCent));

			m_iIntensity = asin(m_iIntensity);

			double m = (m_iIntensity * 90) / pi;
				
			m_iIntensity = exp(-m*m*0.006)*0.50 + exp(-m*0.03)*(1-0.50);

			fAlpha = 1.0f - m_iIntensity;

			if(fAlpha > 1.0f)
				fAlpha = 1.0f;
		}

		HSB m_value = {iColor,static_cast<long>((fAlpha * iColorIntensity)),100};
		CRgbaquad m_rgbValue;
		CColor::HSBToRGB(m_value,m_rgbValue);

		if(i == iTaille - 1)
			MidpointCircle(x,y,i,m_rgbValue,fAlpha2,true);
		else
			MidpointCircle(x,y,i,m_rgbValue,fAlpha2);
	}

	iTypeTransfert = 0;

	if(iBilinearFiltering)
		RadialBlur(x,y, iTaille);
}

///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CLensFlare::HaloGradient(const int &x, const int &y, const int &iTaille, const int &iWidth, const float &fAlpha2)
{
	iTypeTransfert = 1;

	int iNb = (360 / iWidth);

	int i = 0;

	for(i = iTaille - iWidth;i < iTaille;i++)
	{
		int j = iTaille - i;
		if(!(j < 0))
		{		
			double m_iIntensity = ((double)((double)(i) / (double)(100)));

			m_iIntensity = asin(m_iIntensity);

			double m = (m_iIntensity * 90) / pi;
				
			m_iIntensity = exp(-m*m*0.006)*0.50 + exp(-m*0.03)*(1-0.50);

		}

		HSB m_value = {iNb * j,50,100};
		CRgbaquad m_rgbValue;
		CColor::HSBToRGB(m_value,m_rgbValue);

		if(i == iTaille - 1)
			MidpointCircle(x,y,i,m_rgbValue,fAlpha2,true);
		else
			MidpointCircle(x,y,i,m_rgbValue,fAlpha2);
	}

	iTypeTransfert = 0;


	if(iBilinearFiltering)
	{	
		for(i = iTaille - iWidth;i < iTaille - 1;i++)
			RadialBlur(x,y,i,1);

		RadialBlur(x,y,i);
	}
}

///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CLensFlare::DefineZoneBilinear(const int &x, const int &y, const int &iTaille)
{
	int i = 0;
	int j = 0;

	for(i = y - iTaille;i < y + iTaille;i++)
	{
		for(j = x - iTaille;j < x + iTaille;j++)
		{
			CRgbaquad m_color;
			if(i > 1 && j > 1 && j < pBitmap->GetBitmapWidth() - 1 && i < pBitmap->GetBitmapHeight() - 1)
			{
				BilinearFiltering(j,i,m_color);
				pBitmap->SetColorValue(j, i, m_color);
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CLensFlare::Circle(const int &x, const int &y,const CRgbaquad &m_color, const int &iTaille, const float &fAlpha)
{
	iTypeTransfert = 1;

	MidpointCircle(x,y,iTaille-1,m_color,fAlpha,true,true);

	iTypeTransfert = 0;

	if(iBilinearFiltering)
		RadialBlur(x,y,iTaille);
}

///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CLensFlare::CircleGradient(const int &x, const int &y,const CRgbaquad &m_color, const int &iTaille,const float &fAlpha)
{
	iTypeTransfert = 1;

	int i = 0;

	for(i = 0;i < iTaille;i++)
	{
		if(i == iTaille - 1)
			MidpointCircle(x,y,i,m_color,((float)(i) / (float)(iTaille)),true);
		else
			MidpointCircle(x,y,i,m_color,((float)(i) / (float)(iTaille)));
	}

	iTypeTransfert = 0;
	
	if(iBilinearFiltering)
		RadialBlur(x,y,iTaille);
}

///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CLensFlare::RadialBlur(const int &xFrom, const int &yFrom, const int &iRayon,const int &iSize)
{
	//int iDiff = 4;
	int iTaille = iRayon;// + iDiff;
	int r2 = iTaille * iTaille;
	int y = 0,x=0;
			
	//int i = 0;
	//int j = 0;

	do
	{
		
		float fValue = -sqrt((double)r2 - y*y);

		/////////////////////////////////////////////////////////////////////
		//Antialiasing avant
		/////////////////////////////////////////////////////////////////////

		x = (int)fValue;

		int xLocal = xFrom - x;
		int xLocal2 = xFrom + x;
		int yLocal = yFrom + y;
		int yLocal2 = yFrom - y;

		DefineZoneBilinear(xLocal,yLocal,iSize);
		DefineZoneBilinear(xLocal2,yLocal,iSize);
		DefineZoneBilinear(xLocal,yLocal2,iSize);
		DefineZoneBilinear(xLocal2,yLocal2,iSize);

		xLocal = xFrom - y;
		xLocal2 = xFrom + y;
		yLocal = yFrom + x;
		yLocal2 = yFrom - x;

		DefineZoneBilinear(xLocal,yLocal,iSize);
		DefineZoneBilinear(xLocal2,yLocal,iSize);
		DefineZoneBilinear(xLocal,yLocal2,iSize);
		DefineZoneBilinear(xLocal2,yLocal2,iSize);

		y++;

	}while(y < -x);
}

///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CLensFlare::RadialBlurCercleVide(const int &xFrom, const int &yFrom, const int &iRayon,const int &iCercleTaille)
{
	int iDiff = 4;
	int iTaille = 50 + iDiff;
	int r2 = iTaille * iTaille;
	int x=0;
			
	//int i = 0;
	//int j = 0;

	for(int i = yFrom - iTaille;i < yFrom + iTaille;i++)
	{
		int yLocalValue = i - yFrom;
		//Détermination pour ce y du x Max + 1;
        float fValue = sqrt((double)r2 - std::abs(yLocalValue*yLocalValue));
		x = (int)fValue;

		if(i >= 0)
		{
			pBitmap->SetColorValue(xFrom + iTaille - x,i ,CRgbaquad(0,0,255));
			pBitmap->SetColorValue(xFrom + iTaille + x, i, CRgbaquad(0, 0, 255));
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CLensFlare::Burst(const int &x, const int &y,const int &iTaille,const int &iColor, const int &iIntensity, const int &iColorIntensity)
{
	iTypeTransfert = 1;

	//float m_Alpha;

	//float y1 = 1.0f - (iIntensity/100.0f);
	float y1 = 0.6f;
	float x1 = 0.5f;

	float a = (y1 - 1.0f) / (x1 * (x1 - 1.0f));
	float b = 1.0f - a;


	int i = 0;

	for(i = 0;i < iTaille;i++)
	{
		float fAlpha;
		//double m_iIntensity;

		//int j = iTaille - i;

		//Test nouvelle version de l'intensité

		float k = (float)i / (float)iTaille;

		fAlpha = a * (k * k) + b * k;

		if(fAlpha < 0.0f)
			fAlpha = 0.0f;

		HSB m_value = {iColor,static_cast<long>(iColorIntensity * fAlpha),100};
		CRgbaquad m_rgbValue;
		CColor::HSBToRGB(m_value,m_rgbValue);

		if(i == iTaille - 1)
			MidpointCircle(x,y,i,m_rgbValue,fAlpha,true);
		else
			MidpointCircle(x,y,i,m_rgbValue,fAlpha);
	}

	iTypeTransfert = 0;

	if(iBilinearFiltering)
		RadialBlur(x,y,iTaille);
}

///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CLensFlare::SetAlphaColorValue(const int &xFrom, const int &yFrom, const int &x, const int &y,const float &fAlpha,const CRgbaquad &m_color)
{
	CRgbaquad m_color2;

	float fDifference = 1.0f - fAlpha;

	int iXMin = pBitmap->GetBitmapWidth() - iWidthMax;
	int iYMin =  pBitmap->GetBitmapHeight() - iHeightMax;


	if(iTypeTransfert == 0)
	{
		if(xFrom >= iWidthMax || xFrom < iXMin || x < iXMin || x >= iWidthMax )
			return;

		if(yFrom >= iHeightMax || yFrom < iYMin || y < iYMin || y >= iHeightMax)
			return;

		if(iYMin < 0)
		{
			if(y >= iHeightMax + iYMin)
				return;
		}

		if(iXMin < 0)
		{
			if(x >= iWidthMax + iXMin)
				return;
		}
	
		m_color2 = pBitmap->GetColorValue(xFrom, yFrom);
	}
	else
	{

		if(x < iXMin || x >= iWidthMax)
			return;

		if(y < iYMin || y >= iHeightMax)
			return;

		if(iYMin < 0)
		{
			if(y >= iHeightMax + iYMin)
				return;
		}

		if(iXMin < 0)
		{
			if(x >= iWidthMax + iXMin)
				return;
		}

		int iPos = y * (int)pBitmap->GetWidthSize() + x * 4;
		uint8_t * pBitsSrc = pBitmap->GetPtBitmap();
		m_color2 = CRgbaquad(*(pBitsSrc + iPos + 2), *(pBitsSrc + iPos + 1), *(pBitsSrc + iPos));
	}

	m_color2 = CRgbaquad((m_color.GetFRed()) * fDifference + m_color2.GetFRed() * fAlpha
		, m_color.GetFGreen() * fDifference + m_color2.GetFGreen() * fAlpha
		, m_color.GetFBlue() * fDifference + m_color2.GetFBlue() * fAlpha);

	pBitmap->SetColorValue(x, y, m_color2);
}


///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CLensFlare::MidpointCircle(const int &xFrom, const int &yFrom,const int &Rayon, const CRgbaquad &m_color, const float &fAlpha, const bool &bAntialiasing, const bool &bFullCircle)
{

	int r2;
	int iOldy = 0;
	int iOldx = 0;

	int x,y;

	r2 = Rayon * Rayon;
	int iDemiRayon = -(Rayon>>1) + 1;

	int iFin = (int)sqrt((double)r2 - iDemiRayon*iDemiRayon) - 1;

		for (x = -Rayon; x < iDemiRayon; x++) 
		{
			
			float fValue = sqrt((double)r2 - x*x);

			//int y = (int)fValue;

			int y = int(fValue);
			
			
			if(bFullCircle)
			{
				for(int i = 0;i < iOldy;i++)
				{
					WriteCirclePoint(xFrom,yFrom,i,x,fAlpha,m_color);
				}
			}

			for(int i = iOldy;i < Rayon;i++)
			{
				float fValue2 = sqrt((double)i*i + (-x*-x));
				//int iValue = (int)fValue2;

				int iValue = int(fValue2);

				if(iValue < Rayon)
				{
					WriteCirclePoint(xFrom,yFrom,i,x,fAlpha,m_color);
				}
				else if(bAntialiasing)
				{
					float fDifference = (fValue - Rayon);

					if(fDifference > 1.0f || fDifference < 0.0f)
						break;

					fDifference = fAlpha + ((1.0f - fAlpha) * fDifference);

					WriteCirclePoint(xFrom,yFrom,i,x,fDifference,m_color);
				}
			}
		
			iOldy = y;	
		}
		for (y = Rayon; y >= iFin; y--) 
		{
			
			float fValue = -sqrt((double)r2 - y*y);

			//int x = (int)fValue;

			int x = int(fValue);
			
			
			if(bFullCircle)
			{
				for(int i = iOldx;i <= 0;i++)
				{
					WriteCirclePoint(xFrom,yFrom,y,i,fAlpha,m_color);
				}
			}

			for(int i = iOldx;i > -Rayon;i--)
			{
				float fValue2 = sqrt((double)i*i + (y*y));
				//int iValue = (int)fValue2;
				int iValue = int(fValue2);

				if(iValue < Rayon)
				{
					WriteCirclePoint(xFrom,yFrom,y,i,fAlpha,m_color);
				}
				else if(bAntialiasing)
				{
					
					float fDifference = fValue2 - Rayon;

					if(fDifference > 1.0f)
						break;

					fDifference = fAlpha + ((1.0f - fAlpha) * fDifference);

					WriteCirclePoint(xFrom,yFrom,y,i,fDifference,m_color);


				}
			}
			iOldx = x;	
		}

	if(bFullCircle)
	{
		for(y = 0; y <= iFin+1; y++)
		{
			for(x = iDemiRayon;x <= 0;x++)
			{
				WriteCirclePoint(xFrom,yFrom,y,x,fAlpha,m_color);
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CLensFlare::WriteCirclePoint(const int &xFrom, const int &yFrom, const int &i, const int &x, const float &fAlpha, const CRgbaquad &m_color)
{
	float fAlpha2 = fAlpha;
	if(fAlpha > 1.0f)
		fAlpha2 = 1.0f;

	SetAlphaColorValue(xFrom + x,yFrom + i + 1,xFrom + x, yFrom + i,fAlpha2,m_color);
	SetAlphaColorValue(xFrom + x,yFrom - i - 1,xFrom + x, yFrom - i,fAlpha2,m_color);
	SetAlphaColorValue(xFrom - x,yFrom + i + 1,xFrom - x, yFrom + i,fAlpha2,m_color);
	SetAlphaColorValue(xFrom - x,yFrom - i - 1,xFrom - x, yFrom - i,fAlpha2,m_color);
}


///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
int CLensFlare::BilinearFiltering(const int &iSrcX,const int &iSrcY, CRgbaquad &m_colorValue)
{

	CRgbaquad pTopLeft, pTopRight, pBottomLeft, pBottomRight;

	int iXMin = pBitmap->GetBitmapWidth() - iWidthMax;
	int iYMin =  pBitmap->GetBitmapHeight() - iHeightMax;


	if(iSrcX >= iWidthMax || iSrcX < iXMin)
		return 0;

	if(iSrcY >= iHeightMax || iSrcY < iYMin)
		return 0;


	CRgbaquad color = pBitmap->GetColorValue(iSrcX - 1, iSrcY + 1);

	pTopLeft = color;// RGB(color.GetRed(), color.GetGreen(), color.GetBlue());

	color = pBitmap->GetColorValue(iSrcX + 1, iSrcY + 1);
	pTopRight = color;// RGB(color.GetRed(), color.GetGreen(), color.GetBlue());

	color = pBitmap->GetColorValue(iSrcX - 1, iSrcY - 1);
	pBottomLeft = color;// RGB(color.GetRed(), color.GetGreen(), color.GetBlue());

	color = pBitmap->GetColorValue(iSrcX + 1, iSrcY - 1);
	pBottomRight = color;// RGB(color.GetRed(), color.GetGreen(), color.GetBlue());
	
	
	/*
	// red interpolation

	int redFP =	(GetRValue(pTopLeft) +
					GetRValue(pTopRight)	+
					GetRValue(pBottomLeft) +
					GetRValue(pBottomRight)
					) >> 2;
	
	// green interpolation
	
	int greenFP =	(GetGValue(pTopLeft) +
					GetGValue(pTopRight)	+
					GetGValue(pBottomLeft) +
					GetGValue(pBottomRight)
					) >> 2;


	// blue interpolation
	int blueFP =	(GetBValue(pTopLeft) +
					GetBValue(pTopRight)	+
					GetBValue(pBottomLeft) +
					GetBValue(pBottomRight)
					) >> 2;
					*/

	int redFP = (pTopLeft.GetRed() +
		pTopRight.GetRed() +
		(pBottomLeft.GetRed()) +
		(pBottomRight.GetRed())
		) >> 2;

	// green interpolation

	int greenFP = (pTopLeft.GetGreen() +
		(pTopRight.GetGreen()) +
		(pBottomLeft.GetGreen()) +
		(pBottomRight.GetGreen())
		) >> 2;


	// blue interpolation
	int blueFP = ((pTopLeft.GetBlue()) +
		(pTopRight.GetBlue()) +
		(pBottomLeft.GetBlue()) +
		(pBottomRight.GetBlue())
		) >> 2;
	m_colorValue = CRgbaquad(redFP,greenFP,blueFP);

	return 1;

}

///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
void CLensFlare::MidpointLine(const int &xFrom, const int &yFrom,const int &xTo, const int &yTo, const CRgbaquad &m_color, const float &fAlpha, const bool &m_bAntialiasing)
{
 	int x, y;
 	float a = 0.0f, b = 0.0f, fValue = 0.0f;

 	a = ((float)(yTo - yFrom) / (float)(xTo - xFrom));
 	b = yFrom - (a * xFrom);

 	x = xFrom;
 	y = yFrom;

 	SetAlphaColorValue(x,y,x, y,fAlpha,m_color);

 	while(x < xTo)
 	{

 		x++;

 		fValue = a * x + b;

 		//Définition de l'antialiasing

 		y = (int)fValue;

 		SetAlphaColorValue(x,y,x, y,fAlpha,m_color);
 	}
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void CLensFlare::LensFlare(CRegardsBitmap * pBitmap, const int &iPosX, const int &iPosY, const int &iPuissance, const int &iType, const int &iIntensity, const int &iColor, const int &iColorIntensity)
{
	this->pBitmap = pBitmap;

	int iWidth = pBitmap->GetBitmapWidth();
	int iHeight = pBitmap->GetBitmapHeight();


	//Chargement du lensflare 1
	if (pBitmap->GetPtBitmap() != NULL)
	{
		//long Moyenne;
		//uint8_t * pBitsSrc = NULL;
		//unsigned int bmWidth, bmHeight;//,bmWidth2, bmHeight2;
		//unsigned long m_lSize;
	//	HBITMAP hBitmap;
		int i = 0;

		int x = iPosX;
		int y = iPosY;

		int iRayon = iPuissance;

		iBilinearFiltering = iType;

		int iMaxX = iWidth >> 1;
		int iMaxY = iHeight >> 1;
	
		iHeightMax = iHeight;
		iWidthMax = iWidth;
		
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
		
		for (i = 0; i <= 360; i++) 
		{
			
			float fxValue = cos(i * CONVRADIAN) * rand();
			float fyValue = sin(i * CONVRADIAN) * rand();

			fxValue = (int)fxValue % (iIntRayon);
			fyValue = (int)fyValue % (iIntRayon);
			
			MidpointLine(x,y,x+fxValue,y+fyValue,CRgbaquad(255,255,255),0.9f,true);
			MidpointLine(x - fxValue, y - fyValue, x, y, CRgbaquad(255, 255, 255), 0.9f, true);
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
