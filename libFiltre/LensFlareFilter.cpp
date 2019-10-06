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
    return IDM_FILTRE_SWIRL;
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


void CLensFlareFilter::ApplyPreviewEffect(CEffectParameter * effectParameter, CFiltreEffet * filtreEffet, CDraw * m_cDessin)
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
		int left = 0, top = 0;
		if (source->GetBitmapWidth() > widthOutput)
			left = ((source->GetBitmapWidth() - widthOutput) / 2);
		else
			left = 0;

		if (source->GetBitmapHeight() > heightOutput)
			top = ((source->GetBitmapHeight() - heightOutput) / 2);
		else
			top = 0;
		int puissance = (float)lensFlareParameter->size;
		int brightness = (float)lensFlareParameter->brightness;
		int radius = (float)lensFlareParameter->colorIntensity;
		LensFlare(filtreEffet, pt.x, pt.y, puissance, 0, brightness, lensFlareParameter->color, radius, left, top);

		if (m_cDessin != nullptr)
		{
			wxImage image = filtreEffet->GetwxImage();
			wxBitmap bitmap = wxBitmap(image);
			wxMemoryDC dc;
			dc.SelectObject(bitmap);
			wxRect rc(0, 0, image.GetWidth(), image.GetHeight());
			wxImage render = filtreEffet->GetwxImage();
			m_cDessin->Dessiner(&dc, 0, 0, 0, wxColour(30, 30, 30), wxColour(30, 30, 30), wxColour(255, 255, 255), 2);
			dc.SelectObject(wxNullBitmap);
			CImageLoadingFormat * imageLoad = new CImageLoadingFormat();
			wxImage * local_image = new wxImage(bitmap.ConvertToImage());
			imageLoad->SetPicture(local_image);
			filtreEffet->SetBitmap(imageLoad);
		}
	}
}

void CLensFlareFilter::LensFlare(CFiltreEffet * filtreEffet, const int &iPosX, const int &iPosY, const int &iPuissance, const int &iType, const int &iIntensity, const int &iColor, const int &iColorIntensity, const int &posLeft, const int &posTop)
{
	int puissance = (int)((float)(source->GetBitmapWidth() / 4) * ((float)iPuissance / 100.0f));
	filtreEffet->LensFlare(iPosX - posLeft, iPosY - posTop, puissance, iType, iIntensity, iColor, iColorIntensity);
}
