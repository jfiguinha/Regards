#include <header.h>
//
//  SwirlFilter.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright � 2016 figuinha jacques. All rights reserved.
//

#include "VignetteFilter.h"
#include "VignetteEffectParameter.h"
#include <RegardsBitmap.h>
#include <LibResource.h>
#include <FilterData.h>
#include <ImageLoadingFormat.h>
#include <FiltreEffet.h>
#include <BitmapDisplay.h>
using namespace Regards::Filter;

CVignetteFilter::CVignetteFilter()
{
	libelleEffectRadius = CLibResource::LoadStringFromResource(L"LBLEFFECTRADIUS", 1);
	libelleEffectPower = CLibResource::LoadStringFromResource(L"LBLEFFECTPOWER", 1);
}

CVignetteFilter::~CVignetteFilter()
{

}

int CVignetteFilter::GetTypeFilter()
{
	return IDM_FILTRE_VIGNETTE;
}

void CVignetteFilter::Filter(CEffectParameter* effectParameter, CRegardsBitmap* source, IFiltreEffectInterface* filtreInterface)
{
	this->source = source;

	CVignetteEffectParameter* vignetteEffectParameter = (CVignetteEffectParameter*)effectParameter;

	vignetteEffectParameter->bitmapWidth = source->GetBitmapWidth();
	vignetteEffectParameter->bitmapHeight = source->GetBitmapHeight();

	vector<int> elementPower;
	for (auto i = 0; i < 100; i++)
		elementPower.push_back(i);

	vector<int> elementRadius;
	for (auto i = 0; i < 300; i++)
		elementRadius.push_back(i);

	filtreInterface->AddTreeInfos(libelleEffectRadius, new CTreeElementValueInt(vignetteEffectParameter->radius * 10), &elementRadius);
	filtreInterface->AddTreeInfos(libelleEffectPower, new CTreeElementValueInt(vignetteEffectParameter->power * 10), &elementPower);
}

void CVignetteFilter::FilterChangeParam(CEffectParameter* effectParameter, CTreeElementValue* valueData, const wxString& key)
{
	CVignetteEffectParameter* vignetteEffectParameter = (CVignetteEffectParameter*)effectParameter;

	this->source = source;

	CTreeElementValueInt* valueInt = (CTreeElementValueInt*)valueData;
	int value = valueInt->GetValue();
	//Video Parameter
	if (key == libelleEffectRadius)
	{
		vignetteEffectParameter->radius = value / 10;
	}
	if (key == libelleEffectPower)
	{
		vignetteEffectParameter->power = value / 10;
	}
}

void CVignetteFilter::ApplyPreviewEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer, CFiltreEffet* filtreEffet, CDraw* m_cDessin, int& widthOutput, int& heightOutput)
{
	CRegardsBitmap* bitmapOut = filtreEffet->GetBitmap(false);
	CVignetteEffectParameter* vignetteEffectParameter = (CVignetteEffectParameter*)effectParameter;
	CImageLoadingFormat image;
	image.SetPicture(bitmapOut);
	CFiltreEffet* filtre = new CFiltreEffet(bitmapViewer->GetBackColor(), nullptr, &image);
	filtre->VignetteEffect(vignetteEffectParameter->radius, vignetteEffectParameter->power);

	filtreEffet->SetPreview(true);

	CImageLoadingFormat* imageLoad = new CImageLoadingFormat();
	imageLoad->SetPicture(filtre->GetBitmap(true));
	filtreEffet->SetBitmap(imageLoad);

	delete filtre;

}

CImageLoadingFormat* CVignetteFilter::ApplyEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer)
{
	CImageLoadingFormat* imageLoad = nullptr;
	if (effectParameter != nullptr && source != nullptr)
	{
		CVignetteEffectParameter* vignetteEffectParameter = (CVignetteEffectParameter*)effectParameter;
		source->RotateExif(source->GetOrientation());
		CImageLoadingFormat image;
		image.SetPicture(source);
		CFiltreEffet* filtre = new CFiltreEffet(bitmapViewer->GetBackColor(), nullptr, &image);
		filtre->VignetteEffect(vignetteEffectParameter->radius, vignetteEffectParameter->power);
		imageLoad = new CImageLoadingFormat();
		imageLoad->SetPicture(filtre->GetBitmap(true));
		delete filtre;
	}

	return imageLoad;
}