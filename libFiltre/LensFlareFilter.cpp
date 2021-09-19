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
#include <BitmapDisplay.h>
#include <Selection.h>

#include "TreeElementValue.h"
using namespace Regards::Filter;

int CLensFlareFilter::TypeApplyFilter()
{
	return 2;
}

CDraw* CLensFlareFilter::GetDrawingPt()
{
	return new CSelection();
}

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

bool CLensFlareFilter::IsOpenCLCompatible()
{
	return false;
}

bool CLensFlareFilter::SupportMouseClick()
{
	return true;
}

wxString CLensFlareFilter::GetFilterLabel()
{
	return CLibResource::LoadStringFromResource("LBLfilterLensFlare", 1);
}


int CLensFlareFilter::GetNameFilter()
{
	return IDM_FILTRELENSFLARE;
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



CImageLoadingFormat * CLensFlareFilter::ApplyEffect(CEffectParameter * effectParameter, IBitmapDisplay * bitmapViewer)
{
	CImageLoadingFormat * imageLoad = nullptr;
	if (effectParameter != nullptr && source != nullptr && bitmapViewer != nullptr)
	{
		source->RotateExif(source->GetOrientation());
		CImageLoadingFormat image(false);
		image.SetPicture(source);
		CFiltreEffet * filtre = new CFiltreEffet(bitmapViewer->GetBackColor(), nullptr, &image);

		wxPoint pt;
		bitmapViewer->GetDessinPt()->GetPoint(pt);
		CLensFlareEffectParameter * lensFlareParameter = (CLensFlareEffectParameter *)effectParameter;
		float puissance = (float)lensFlareParameter->size;
		float brightness = (float)lensFlareParameter->brightness;
		float colorIntensity = (float)lensFlareParameter->colorIntensity;
		puissance = ((float)(source->GetBitmapWidth() / 4) * (puissance / 100.0f));
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


void CLensFlareFilter::Drawing(wxMemoryDC * dc, IBitmapDisplay * bitmapViewer, CDraw * m_cDessin)
{
	int hpos = bitmapViewer->GetHPos();
	int vpos = bitmapViewer->GetVPos();

	if (m_cDessin != nullptr)
		m_cDessin->Dessiner(dc, hpos, vpos, bitmapViewer->GetRatio(), wxColour(0, 0, 0), wxColour(0, 0, 0), wxColour(0, 0, 0), 2);
}

void CLensFlareFilter::RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
{

}
bool CLensFlareFilter::NeedPreview()
{
	return true;
}

CEffectParameter* CLensFlareFilter::GetEffectPointer()
{
	return new CLensFlareEffectParameter();
}

CEffectParameter* CLensFlareFilter::GetDefaultEffectParameter()
{
	return new CLensFlareEffectParameter();
}


bool CLensFlareFilter::IsSourcePreview()
{
    return true;
}


void CLensFlareFilter::ApplyPreviewEffectSource(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer, CFiltreEffet* filtreEffet, CDraw* dessing)
{
	CImageLoadingFormat* imageLoad = nullptr;
	if (effectParameter != nullptr && source != nullptr)
	{
		CLensFlareEffectParameter* lensFlareParameter = (CLensFlareEffectParameter*)effectParameter;
		float puissance = (float)lensFlareParameter->size;
		puissance = ((float)(source->GetBitmapWidth() / 4) * (puissance / 100.0f));
		float brightness = (float)lensFlareParameter->brightness;
		float colorIntensity = (float)lensFlareParameter->colorIntensity;

		wxPoint pt;
		bitmapViewer->GetDessinPt()->GetPoint(pt);


		source->RotateExif(source->GetOrientation());
		CImageLoadingFormat image(false);
		image.SetPicture(source);
		CFiltreEffet* filtre = new CFiltreEffet(bitmapViewer->GetBackColor(), nullptr, &image);

		filtre->LensFlare(pt.x, pt.y, puissance, 0, brightness, lensFlareParameter->color, colorIntensity);

		imageLoad = new CImageLoadingFormat();
		imageLoad->SetPicture(filtre->GetBitmap(true));
		delete filtre;

		filtreEffet->SetBitmap(imageLoad);

	

	}
}


void CLensFlareFilter::ApplyPreviewEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer, CFiltreEffet* filtreEffet, CDraw* m_cDessin, int& widthOutput, int& heightOutput)
{

}
