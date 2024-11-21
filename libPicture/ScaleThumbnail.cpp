#include <header.h>
#include "ScaleThumbnail.h"
#include "ImageLoadingFormat.h"
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
float CScaleThumbnail::CalculRatio(CImageLoadingFormat* pBitmap, const int& xMax, const int& yMax)
{
	float newRatio;

	//int tailleAffichageWidth = 0, tailleAffichageHeight = 0;

	if (pBitmap->GetWidth() > pBitmap->GetHeight())
		newRatio = static_cast<float>(xMax) / static_cast<float>(pBitmap->GetWidth());
	else
		newRatio = static_cast<float>(yMax) / static_cast<float>(pBitmap->GetHeight());

	if ((pBitmap->GetHeight() * newRatio) > yMax)
	{
		newRatio = static_cast<float>(yMax) / static_cast<float>(pBitmap->GetHeight());
	}
	else
	{
		if ((pBitmap->GetWidth() * newRatio) > xMax)
		{
			newRatio = static_cast<float>(xMax) / static_cast<float>(pBitmap->GetWidth());
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

void CScaleThumbnail::CreateScaleBitmap(CImageLoadingFormat* pBitmap, const int& width, const int& height)
{
	float newRatio = CalculRatio(pBitmap, width, height);
	if (newRatio == 0.0)
		return;

	int nTailleAffichageWidth = pBitmap->GetWidth() * newRatio;
	int nTailleAffichageHeight = pBitmap->GetHeight() * newRatio;


	//RedrawPicture
	//Création d'un nouveau bitmap à la bonne échelle
	if (nTailleAffichageWidth != pBitmap->GetWidth() && nTailleAffichageHeight != pBitmap->GetHeight())
	{
		//auto scaleBitmap = new CRegardsBitmap(nTailleAffichageWidth, nTailleAffichageHeight);
		/*
		CInterpolationBicubic imageScale;
		imageScale.Execute(pBitmap, scaleBitmap);
		pBitmap->SetBitmap(scaleBitmap->GetPtBitmap(), nTailleAffichageWidth, nTailleAffichageHeight);
		*/

		cv::Mat resized_down;
		resize(pBitmap->GetMatrix().getMat(), resized_down, cv::Size(nTailleAffichageWidth, nTailleAffichageHeight),
		       cv::INTER_CUBIC);

		//delete scaleBitmap;
	}
}
