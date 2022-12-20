#include <header.h>
//
//  SwirlFilter.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#include "SwirlFilter.h"
#include "SwirlEffectParameter.h"

#include <LibResource.h>
#include <FilterData.h>
#include <FiltreEffet.h>
#include <BitmapDisplay.h>
#include <ImageLoadingFormat.h>
using namespace Regards::Filter;

CSwirlFilter::CSwirlFilter()
{
    libelleEffectRadius = CLibResource::LoadStringFromResource(L"LBLEFFECTRADIUS",1);
    libelleEffectAngle = CLibResource::LoadStringFromResource(L"LBLEFFECTANGLE",1);
}

CSwirlFilter::~CSwirlFilter()
{
    
}

int CSwirlFilter::TypeApplyFilter()
{
	return 2;
}

wxString CSwirlFilter::GetFilterLabel()
{
	return CLibResource::LoadStringFromResource("LBLfilterSwirl", 1);
}


int CSwirlFilter::GetNameFilter()
{
	return IDM_FILTRE_SWIRL;
}

int CSwirlFilter::GetTypeFilter()
{
	return SPECIAL_EFFECT; //
}

void CSwirlFilter::Filter(CEffectParameter * effectParameter, cv::Mat & source, const wxString& filename, IFiltreEffectInterface * filtreInterface)
{   
   	CSwirlEffectParameter * swirlEffectParameter = (CSwirlEffectParameter *)effectParameter;
	this->source = source;
    swirlEffectParameter->bitmapWidth = source.size().width;
    swirlEffectParameter->bitmapHeight = source.size().height;
	this->filename = filename;
    vector<int> elementAngle;
    for (auto i = 0; i < 360; i++)
        elementAngle.push_back(i);
    
    vector<int> elementRadius;
    for (auto i = 0; i < swirlEffectParameter->bitmapWidth; i++)
        elementRadius.push_back(i);
    
    filtreInterface->AddTreeInfos(libelleEffectRadius, new CTreeElementValueInt(swirlEffectParameter->radius), &elementRadius);
    filtreInterface->AddTreeInfos(libelleEffectAngle, new CTreeElementValueInt(swirlEffectParameter->angle), &elementAngle);
}

void CSwirlFilter::FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key)
{
    CSwirlEffectParameter * swirlEffectParameter = (CSwirlEffectParameter *)effectParameter;
    
	this->source = source;

	CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
    int value = valueInt->GetValue();
    //Video Parameter
    if (key == libelleEffectRadius)
    {
        swirlEffectParameter->radius = value;
    }
    if (key == libelleEffectAngle)
    {
        swirlEffectParameter->angle = value;
    }
}


void CSwirlFilter::RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
{
	if (effectParameter != nullptr && filtreEffet != nullptr)
	{
		CSwirlEffectParameter* swirlParameter = (CSwirlEffectParameter*)effectParameter;
		if (preview)
		{
			float ratio = (float)filtreEffet->GetWidth() / (float)swirlParameter->bitmapWidth;
			filtreEffet->Swirl(swirlParameter->radius * ratio, swirlParameter->angle);
		}
		else
			filtreEffet->Swirl(swirlParameter->radius, swirlParameter->angle);
	}
}

bool CSwirlFilter::NeedPreview()
{
	return true;
}

CEffectParameter* CSwirlFilter::GetEffectPointer()
{
	return new CSwirlEffectParameter();
}


CEffectParameter* CSwirlFilter::GetDefaultEffectParameter()
{
	CSwirlEffectParameter* swirl = new CSwirlEffectParameter();
	swirl->angle = 20;
	swirl->radius = 20;
	return swirl;
}

bool CSwirlFilter::IsSourcePreview()
{
	return true;
}


void CSwirlFilter::ApplyPreviewEffectSource(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer, CFiltreEffet* filtreEffet, CDraw* dessing)
{
	if (effectParameter != nullptr && !source.empty())
	{
		CSwirlEffectParameter* swirlParameter = (CSwirlEffectParameter*)effectParameter;
		filtreEffet->Swirl(swirlParameter->radius, swirlParameter->angle);
	}

}


void CSwirlFilter::ApplyPreviewEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer, CFiltreEffet* filtreEffet, CDraw* m_cDessin, int& widthOutput, int& heightOutput)
{

}

CImageLoadingFormat* CSwirlFilter::ApplyEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer)
{
	CImageLoadingFormat* imageLoad = nullptr;
	if (effectParameter != nullptr && !source.empty() && bitmapViewer != nullptr)
	{
		CFiltreEffet* filter = bitmapViewer->GetFiltreEffet();
		if (filter != nullptr)
		{
			
			CImageLoadingFormat image;
			image.SetPicture(source);
			image.RotateExif(orientation);
			filter->SetBitmap(&image);
			
			CSwirlEffectParameter* swirlParameter = (CSwirlEffectParameter*)effectParameter;
			filter->Swirl(swirlParameter->radius, swirlParameter->angle);
			imageLoad = new CImageLoadingFormat();
			cv::Mat bitmapOut = filter->GetBitmap(true);
			imageLoad->SetPicture(bitmapOut);
		}
	}

	return imageLoad;
}
