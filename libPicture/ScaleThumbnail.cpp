#include <header.h>
#include "ScaleThumbnail.h"
#include <RegardsBitmap.h>
#include <InterpolationBicubic.h>
#include <opencv2/opencv.hpp>
using namespace Regards::Picture;

CScaleThumbnail::CScaleThumbnail(void)
{
}


CScaleThumbnail::~CScaleThumbnail(void)
{
}


//-----------------------------------------------------------------
//Calcul du ratio pour l'image plein écran
//-----------------------------------------------------------------
float CScaleThumbnail::CalculRatio(CRegardsBitmap* pBitmap, const int& xMax, const int& yMax)
{
	float newRatio;

	//int tailleAffichageWidth = 0, tailleAffichageHeight = 0;

	if (pBitmap->GetBitmapWidth() > pBitmap->GetBitmapHeight())
		newRatio = static_cast<float>(xMax) / static_cast<float>(pBitmap->GetBitmapWidth());
	else
		newRatio = static_cast<float>(yMax) / static_cast<float>(pBitmap->GetBitmapHeight());

	if ((pBitmap->GetBitmapHeight() * newRatio) > yMax)
	{
		newRatio = static_cast<float>(yMax) / static_cast<float>(pBitmap->GetBitmapHeight());
	}
	else
	{
		if ((pBitmap->GetBitmapWidth() * newRatio) > xMax)
		{
			newRatio = static_cast<float>(xMax) / static_cast<float>(pBitmap->GetBitmapWidth());
		}
	}

	return newRatio;
}

//-----------------------------------------------------------------
//Calcul du ratio pour l'image plein écran
//-----------------------------------------------------------------
float CScaleThumbnail::CalculRatio(const int& width, const int& height, const int& xMax, const int& yMax)
{
	float newRatio;

	//int tailleAffichageWidth = 0, tailleAffichageHeight = 0;

	if (width > height)
		newRatio = static_cast<float>(xMax) / static_cast<float>(width);
	else
		newRatio = static_cast<float>(yMax) / static_cast<float>(height);

	if ((height * newRatio) > yMax)
	{
		newRatio = static_cast<float>(yMax) / static_cast<float>(height);
	}
	else
	{
		if ((width * newRatio) > xMax)
		{
			newRatio = static_cast<float>(xMax) / static_cast<float>(width);
		}
	}

	return newRatio;
}

void CScaleThumbnail::CreateScaleBitmap(CRegardsBitmap* pBitmap, const int& width, const int& height)
{
	float newRatio = CalculRatio(pBitmap, width, height);
	if (newRatio == 0.0)
		return;

	int nTailleAffichageWidth = pBitmap->GetBitmapWidth() * newRatio;
	int nTailleAffichageHeight = pBitmap->GetBitmapHeight() * newRatio;


	//RedrawPicture
	//Création d'un nouveau bitmap à la bonne échelle
	if (nTailleAffichageWidth != pBitmap->GetBitmapWidth() && nTailleAffichageHeight != pBitmap->GetBitmapHeight())
	{
		//auto scaleBitmap = new CRegardsBitmap(nTailleAffichageWidth, nTailleAffichageHeight);
		/*
		CInterpolationBicubic imageScale;
		imageScale.Execute(pBitmap, scaleBitmap);
		pBitmap->SetBitmap(scaleBitmap->GetPtBitmap(), nTailleAffichageWidth, nTailleAffichageHeight);
		*/

		cv::Mat resized_down;
		cv::resize(pBitmap->GetMatrix(), resized_down, cv::Size(nTailleAffichageWidth, nTailleAffichageHeight), cv::INTER_CUBIC);

		//delete scaleBitmap;
	}
}
