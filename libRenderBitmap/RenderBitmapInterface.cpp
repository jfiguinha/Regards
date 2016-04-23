// stdafx.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets qui sont utilisés fréquemment,
// et sont rarement modifiés
//
#include "RenderBitmapInterface.h"
#include <FiltreEffet.h>
#include <RegardsConfigParam.h>
#include <ParamInit.h>
using namespace Regards::Render;


void CRenderBitmapInterface::MotionBlur(const double &radius, const double &sigma, const double &angle)
{
	
	if (config != nullptr)
	{
		//CFiltreEffet * filtre = new CFiltreEffet(nullptr, renderBitmap, colorBackground, config->GetEffectLibrary());
		//filtre->MotionBlur(radius, sigma, angle);
		//delete filtre;
		int puissance = (int)((float)(bitmapScale->GetBitmapWidth() / 2) * ((float)sigma / 100.0f));
		CFiltreEffet * filtre = new CFiltreEffet(bitmapScale, colorBackground, config->GetEffectLibrary());
		filtre->MotionBlur(radius, puissance, angle);
		delete filtre;

		renderBitmap->InsertBitmapWithoutAlpha(bitmapScale, bitmapZone.x, bitmapZone.y);
	}
}

void CRenderBitmapInterface::LensFlare(const int &iPosX, const int &iPosY, const int &iPuissance, const int &iType, const int &iIntensity, const int &iColor, const int &iColorIntensity)
{
	
	if (config != nullptr)
	{
		CFiltreEffet * filtre = new CFiltreEffet(bitmapScale, colorBackground, config->GetEffectLibrary());
		int puissance = (int)((float)(bitmapScale->GetBitmapWidth() / 2) * ((float)iPuissance / 100.0f));
		int intensity = (int)((float)(bitmapScale->GetBitmapWidth() / 2) * ((float)iIntensity / 100.0f));

		filtre->LensFlare(iPosX - bitmapZone.x, (bitmapScale->GetBitmapHeight() - iPosY) - bitmapZone.y, puissance, iType, intensity, iColor, iColorIntensity);
		delete filtre;

		renderBitmap->InsertBitmapWithoutAlpha(bitmapScale, bitmapZone.x, bitmapZone.y);
	}
}
