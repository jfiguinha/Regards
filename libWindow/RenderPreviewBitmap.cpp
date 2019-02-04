// stdafx.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets qui sont utilisés fréquemment,
// et sont rarement modifiés
//
#include "RenderPreviewBitmap.h"
#include <FiltreEffet.h>
#include <ImageLoadingFormat.h>
#include <LoadingResource.h>
#include <RegardsConfigParam.h>
#include <ParamInit.h>
using namespace Regards::Window;


CRenderPreviewBitmap::CRenderPreviewBitmap()
{
    pictureWidth = 0;
    pictureHeight = 0;
	filtre = nullptr;
}

void CRenderPreviewBitmap::SetBackgroundColor(const CRgbaquad &backColor)
{
	colorBackground = backColor;
}

CRenderPreviewBitmap::~CRenderPreviewBitmap()
{   
	if(filtre != nullptr)
		delete filtre;

    /*
	if(bitmapScale != nullptr)
		delete bitmapScale;

	bitmapScale = nullptr;
     * */
}

void CRenderPreviewBitmap::SetNewBitmap(CImageLoadingFormat * newbitmap, wxWindow * parent, COpenCLContext * openclContext)
{


    /*
	if(bitmapScale != nullptr)
		delete bitmapScale;

	bitmapScale = nullptr;
    
	bitmapScale = newbitmap->GetRegardsBitmap();
    */
    if(newbitmap != nullptr)
    {
        if(filtre != nullptr)
            delete filtre;
        pictureWidth = newbitmap->GetWidth();
        pictureHeight =newbitmap->GetHeight();
        filtre = new CFiltreEffet(colorBackground, parent, openclContext, newbitmap);       
    }

	//filtre->SetBitmap(newbitmap, 0);
}

wxImage CRenderPreviewBitmap::GetRender()
{
	lastRender = filtre->GetwxImage();
    return lastRender;
}

wxImage CRenderPreviewBitmap::GetLastRender()
{
    return lastRender;
}

void CRenderPreviewBitmap::RenderEffect(const int &numEffect, CEffectParameter * effectParameter)
{
	filtre->RenderEffectPreview(numEffect, effectParameter);
}

void CRenderPreviewBitmap::RotateFree(const int &angle)
{
	filtre->RotateFree(angle);
}

CFiltreEffet * CRenderPreviewBitmap::GetFiltre()
{
    return filtre;
}

void CRenderPreviewBitmap::MotionBlur(const double &radius, const double &sigma, const double &angle)
{
	//Copie de l'image
	int puissance = (int)((float)(pictureWidth / 2) * ((float)sigma / 100.0f));
	filtre->MotionBlur(radius, puissance, angle);

}

void CRenderPreviewBitmap::WaveFilter(const int &iPosX, const int &iPosY, short height, int radius, int scale, const int &posLeft, const int &posTop)
{
	filtre->WaveFilter(iPosX - posLeft, pictureHeight - (iPosY - posTop), height, radius, scale);
}

void CRenderPreviewBitmap::LensFlare(const int &iPosX, const int &iPosY, const int &iPuissance, const int &iType, const int &iIntensity, const int &iColor, const int &iColorIntensity, const int &posLeft, const int &posTop)
{
	int puissance = (int)((float)(pictureWidth / 4) * ((float)iPuissance / 100.0f));
	filtre->LensFlare(iPosX - posLeft, iPosY - posTop, puissance, iType, iIntensity, iColor, iColorIntensity);
}
