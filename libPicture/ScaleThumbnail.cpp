#include "ScaleThumbnail.h"
#include <RegardsBitmap.h>
#include <InterpolationBicubic.h>


CScaleThumbnail::CScaleThumbnail(void)
{
}


CScaleThumbnail::~CScaleThumbnail(void)
{
}


//-----------------------------------------------------------------
//Calcul du ratio pour l'image plein écran
//-----------------------------------------------------------------
float CScaleThumbnail::CalculRatio(CRegardsBitmap * pBitmap, const int &xMax, const int &yMax)
{
	float newRatio;

	//int tailleAffichageWidth = 0, tailleAffichageHeight = 0;

	if(pBitmap->GetBitmapWidth() > pBitmap->GetBitmapHeight())
		newRatio = (float)xMax / (float)(pBitmap->GetBitmapWidth());
	else
		newRatio = (float)yMax / (float)(pBitmap->GetBitmapHeight());

	if((pBitmap->GetBitmapHeight() * newRatio) > yMax)
	{
		newRatio = (float)yMax / (float)(pBitmap->GetBitmapHeight());
	}
	else
	{
		if((pBitmap->GetBitmapWidth() * newRatio) > xMax)
		{
			newRatio = (float)xMax / (float)(pBitmap->GetBitmapWidth());
		}
	}

	return newRatio;
}

//-----------------------------------------------------------------
//Calcul du ratio pour l'image plein écran
//-----------------------------------------------------------------
float CScaleThumbnail::CalculRatio(const int &width, const int &height, const int &xMax, const int &yMax)
{
	float newRatio;

	//int tailleAffichageWidth = 0, tailleAffichageHeight = 0;

	if (width > height)
		newRatio = (float)xMax / (float)(width);
	else
		newRatio = (float)yMax / (float)(height);

	if ((height * newRatio) > yMax)
	{
		newRatio = (float)yMax / (float)(height);
	}
	else
	{
		if ((width * newRatio) > xMax)
		{
			newRatio = (float)xMax / (float)(width);
		}
	}

	return newRatio;
}

void CScaleThumbnail::CreateScaleBitmap(CRegardsBitmap * pBitmap, const int &width, const int &height)
{


	float newRatio = CalculRatio(pBitmap, width, height);
	if(newRatio == 0.0)
		return;

	int nTailleAffichageWidth = pBitmap->GetBitmapWidth() * newRatio;
	int nTailleAffichageHeight = pBitmap->GetBitmapHeight() * newRatio;

	

	//RedrawPicture
	//Création d'un nouveau bitmap à la bonne échelle
	if(nTailleAffichageWidth != pBitmap->GetBitmapWidth() && nTailleAffichageHeight != pBitmap->GetBitmapHeight())
	{	
		CRegardsBitmap * scaleBitmap = new CRegardsBitmap(nTailleAffichageWidth, nTailleAffichageHeight);
		CInterpolationBicubic imageScale;
		imageScale.Execute(pBitmap, scaleBitmap);
		pBitmap->SetBitmap(scaleBitmap->GetPtBitmap(), nTailleAffichageWidth, nTailleAffichageHeight);
		delete scaleBitmap;

	}

}