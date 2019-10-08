#include <header.h>
//
//  CLensFlareFilter.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#include "LensFlareFilter.h"
#include "LensFlareEffectParameter.h"
#include <RegardsBitmap.h>
#include <LibResource.h>
#include <FilterData.h>
#include <ImageLoadingFormat.h>
#include <FiltreEffet.h>
#include <Draw.h>
#include <BitmapWndViewer.h>
using namespace Regards::Viewer;

CLensFlareFilter::CLensFlareFilter()
{
    libelleEffectRadius = CLibResource::LoadStringFromResource(L"LBLEFFECTRADIUS",1);
    libelleEffectIntensity = CLibResource::LoadStringFromResource(L"LBLEFFECTINTENSITY",1);
    libelleEffectLightness = CLibResource::LoadStringFromResource(L"LBLEFFECTBRIGHTNESS",1);
    libelleColor = CLibResource::LoadStringFromResource(L"LBLCOLOREFFECT",1);
}

CLensFlareFilter::~CLensFlareFilter()
{
    
}

int CLensFlareFilter::GetTypeFilter()
{
    return IDM_FILTRELENSFLARE;
}

void CLensFlareFilter::Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface)
{
    CLensFlareEffectParameter * lensFlareParameter = (CLensFlareEffectParameter *)effectParameter;
 
	this->source = source;

    vector<int> elementIntensity;
    for (auto i = 0; i < 101; i++)
        elementIntensity.push_back(i);
    
    vector<int> elementColor;
    for (auto i = 0; i < 361; i++)
        elementColor.push_back(i);
    
    vector<int> elementBitmapHeight;
    for (auto i = 0; i < 101; i++)
        elementBitmapHeight.push_back(i);
    
    filtreInterface->AddTreeInfos(libelleEffectLightness, new CTreeElementValueInt(lensFlareParameter->brightness), &elementIntensity);
    filtreInterface->AddTreeInfos(libelleEffectIntensity, new CTreeElementValueInt(lensFlareParameter->colorIntensity), &elementIntensity);
    filtreInterface->AddTreeInfos(libelleEffectRadius, new CTreeElementValueInt(lensFlareParameter->size), &elementBitmapHeight);
    filtreInterface->AddTreeInfos(libelleColor, new CTreeElementValueInt(lensFlareParameter->color), &elementColor);
}

void CLensFlareFilter::FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key)
{
    CLensFlareEffectParameter * lensFlareParameter = (CLensFlareEffectParameter *)effectParameter;
    
	CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
    int value = valueInt->GetValue();
    //Video Parameter
    if (key == libelleEffectLightness)
    {
        lensFlareParameter->brightness = value;
    }
    else if (key == libelleEffectIntensity)
    {
        lensFlareParameter->colorIntensity = value;
    }
    else if (key == libelleEffectRadius)
    {
        lensFlareParameter->size = value;
    }
    else if (key == libelleColor)
    {
        lensFlareParameter->color = value;
    }
}


void CLensFlareFilter::ApplyPreviewEffect(CEffectParameter * effectParameter, Regards::Control::CBitmapWndViewer * bitmapViewer, CFiltreEffet * filtreEffet, CDraw * m_cDessin)
{
	CRegardsBitmap * bitmapOut = filtreEffet->GetBitmap(false);

	int widthOutput = bitmapOut->GetBitmapWidth();
	int heightOutput = bitmapOut->GetBitmapHeight();
	wxPoint pt;
	m_cDessin->GetScreenPoint(pt);
	if (pt.x == 0 && pt.y == 0)
	{
		pt.x = widthOutput / 4;
		pt.y = bitmapOut->GetBitmapHeight() / 4;
	}

	CLensFlareEffectParameter * lensFlareParameter = (CLensFlareEffectParameter *)effectParameter;
	if (pt.x != 0 && pt.y != 0)
	{
		CImageLoadingFormat image;
		image.SetPicture(bitmapOut);
		CFiltreEffet * filtre = new CFiltreEffet(bitmapViewer->GetBackColor(), nullptr, &image);
		int puissance = (float)lensFlareParameter->size;
		int brightness = (float)lensFlareParameter->brightness;
		int radius = (float)lensFlareParameter->colorIntensity;
		LensFlare(filtre, pt.x, pt.y, puissance, 0, brightness, lensFlareParameter->color, radius, 0, 0);

		DrawingToPicture(effectParameter, bitmapViewer, filtre, m_cDessin);

		filtreEffet->SetPreview(true);

		CImageLoadingFormat * imageLoad = new CImageLoadingFormat();
		imageLoad->SetPicture(filtre->GetBitmap(true));
		filtreEffet->SetBitmap(imageLoad);

		delete filtre;
	}
}

CImageLoadingFormat * CLensFlareFilter::ApplyEffect(CEffectParameter * effectParameter, Regards::Control::CBitmapWndViewer * bitmapViewer)
{
	CImageLoadingFormat * imageLoad = nullptr;
	if (effectParameter != nullptr && source != nullptr)
	{
		CImageLoadingFormat image;
		image.SetPicture(source);
		CFiltreEffet * filtre = new CFiltreEffet(bitmapViewer->GetBackColor(), nullptr, &image);

		wxPoint pt;
		bitmapViewer->GetDessinPt()->GetPoint(pt);
		CLensFlareEffectParameter * lensFlareParameter = (CLensFlareEffectParameter *)effectParameter;
		float puissance = (float)lensFlareParameter->size;
		float brightness = (float)lensFlareParameter->brightness;
		float colorIntensity = (float)lensFlareParameter->colorIntensity;
		puissance = ((float)(source->GetBitmapWidth() / 4) * ((float)puissance / 100.0f));
		filtre->LensFlare(pt.x, pt.y, puissance, 0, brightness, lensFlareParameter->color, colorIntensity);

		imageLoad = new CImageLoadingFormat();
		imageLoad->SetPicture(filtre->GetBitmap(true));
		delete filtre;
	}

	return imageLoad;
}

void CLensFlareFilter::LensFlare(CFiltreEffet * filtreEffet, const int &iPosX, const int &iPosY, const int &iPuissance, const int &iType, const int &iIntensity, const int &iColor, const int &iColorIntensity, const int &posLeft, const int &posTop)
{
	int puissance = (int)((float)(source->GetBitmapWidth() / 4) * ((float)iPuissance / 100.0f));
	filtreEffet->LensFlare(iPosX - posLeft, iPosY - posTop, puissance, iType, iIntensity, iColor, iColorIntensity);
}


void CLensFlareFilter::Drawing(wxMemoryDC * dc, Regards::Control::CBitmapWndViewer * bitmapViewer, CDraw * m_cDessin)
{
	if (m_cDessin != nullptr)
		m_cDessin->Dessiner(dc, 0, 0, 1, wxColour(0, 0, 0), wxColour(0, 0, 0), wxColour(0, 0, 0), 2);
}

