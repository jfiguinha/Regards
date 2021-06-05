#include <header.h>
//
//  SwirlFilter.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
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

int CVignetteFilter::TypeApplyFilter()
{
	return 2;
}

wxString CVignetteFilter::GetFilterLabel()
{
	return CLibResource::LoadStringFromResource("LBLFILTREVIGNETTE", 1);
}


int CVignetteFilter::GetNameFilter()
{
	return IDM_FILTRE_VIGNETTE;
}

int CVignetteFilter::GetTypeFilter()
{
	return SPECIAL_EFFECT; //
}

void CVignetteFilter::Filter(CEffectParameter* effectParameter, CRegardsBitmap* source, IFiltreEffectInterface* filtreInterface)
{
	CVignetteEffectParameter* vignetteEffectParameter = (CVignetteEffectParameter*)effectParameter;

	this->source = source;

	vignetteEffectParameter->bitmapWidth = source->GetBitmapWidth();
	vignetteEffectParameter->bitmapHeight = source->GetBitmapHeight();

	vector<int> elementRadius;
	for (auto i = 0; i < 100; i++)
		elementRadius.push_back(i);


	filtreInterface->AddTreeInfos(libelleEffectRadius, new CTreeElementValueInt(vignetteEffectParameter->radius), &elementRadius);
	//filtreInterface->AddTreeInfos(libelleEffectPower, new CTreeElementValueInt(vignetteEffectParameter->power * 10), &elementPower);
}

void CVignetteFilter::FilterChangeParam(CEffectParameter* effectParameter, CTreeElementValue* valueData, const wxString& key)
{
	CVignetteEffectParameter* vignetteEffectParameter = (CVignetteEffectParameter*)effectParameter;

	CTreeElementValueInt* valueInt = (CTreeElementValueInt*)valueData;
	int value = valueInt->GetValue();
	//Video Parameter
	if (key == libelleEffectRadius)
	{
		vignetteEffectParameter->radius = (float)value;
	}

}

void CVignetteFilter::ApplyPreviewEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer, CFiltreEffet* filtreEffet, CDraw* m_cDessin, int& widthOutput, int& heightOutput)
{
	CRegardsBitmap* bitmapOut = filtreEffet->GetBitmap(false);
	CVignetteEffectParameter* vignetteEffectParameter = (CVignetteEffectParameter*)effectParameter;
	CImageLoadingFormat image;
	image.SetPicture(bitmapOut);
	CFiltreEffet* filtre = new CFiltreEffet(bitmapViewer->GetBackColor(), nullptr, &image);


	//float ratio = (float)bitmapOut->GetBitmapWidth() / (float)vignetteEffectParameter->bitmapWidth;
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
		CImageLoadingFormat image(false);
		image.SetPicture(source);
		CFiltreEffet* filtre = new CFiltreEffet(bitmapViewer->GetBackColor(), nullptr, &image);
		filtre->VignetteEffect(vignetteEffectParameter->radius, vignetteEffectParameter->power);
		imageLoad = new CImageLoadingFormat();
		imageLoad->SetPicture(filtre->GetBitmap(true));
		delete filtre;
	}

	return imageLoad;
}

void CVignetteFilter::RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
{
	if (effectParameter != nullptr && filtreEffet != nullptr)
	{
		CVignetteEffectParameter* vignetteEffectParameter = (CVignetteEffectParameter*)effectParameter;
		if (preview)
		{
			//float ratio = (float)filtreEffet->GetWidth() / (float)vignetteEffectParameter->bitmapWidth;
			filtreEffet->VignetteEffect(vignetteEffectParameter->radius, vignetteEffectParameter->power);
		}
		else
			filtreEffet->VignetteEffect(vignetteEffectParameter->radius, vignetteEffectParameter->power);
	}
}

bool CVignetteFilter::NeedPreview()
{
	return true;
}

CEffectParameter* CVignetteFilter::GetEffectPointer()
{
	return new CVignetteEffectParameter();
}

CEffectParameter* CVignetteFilter::GetDefaultEffectParameter()
{
	CVignetteEffectParameter* vignetteEffectParameter = new CVignetteEffectParameter();
	//vignetteEffectParameter->power = 0.8;
	vignetteEffectParameter->radius = 50;
	return vignetteEffectParameter;
}
