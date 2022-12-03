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

void CVignetteFilter::Filter(CEffectParameter* effectParameter, cv::Mat & source, const wxString& filename, IFiltreEffectInterface* filtreInterface)
{
	CVignetteEffectParameter* vignetteEffectParameter = (CVignetteEffectParameter*)effectParameter;
	this->filename = filename;
	this->source = source;

	vignetteEffectParameter->bitmapWidth = source.size().width;
	vignetteEffectParameter->bitmapHeight = source.size().height;

	vector<int> elementRadius;
	for (auto i = 0; i < 100; i++)
		elementRadius.push_back(i);


	filtreInterface->AddTreeInfos(libelleEffectRadius, new CTreeElementValueInt(vignetteEffectParameter->radius), &elementRadius);
	filtreInterface->AddTreeInfos(libelleEffectPower, new CTreeElementValueInt(vignetteEffectParameter->power), &elementRadius);
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
	if (key == libelleEffectPower)
	{
		vignetteEffectParameter->power = (float)value;
	}
}

void CVignetteFilter::ApplyPreviewEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer, CFiltreEffet* filtreEffet, CDraw* m_cDessin, int& widthOutput, int& heightOutput)
{


}

bool CVignetteFilter::IsSourcePreview()
{
	return true;
}


void CVignetteFilter::ApplyPreviewEffectSource(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer, CFiltreEffet* filtreEffet, CDraw* dessing)
{
	CImageLoadingFormat* imageLoad = nullptr;
	if (effectParameter != nullptr && !source.empty())
	{
		CImageLoadingFormat image;
		image.SetPicture(source);
		CFiltreEffet* filtre = new CFiltreEffet(bitmapViewer->GetBackColor(), false, &image);


		CVignetteEffectParameter* vignetteEffectParameter = (CVignetteEffectParameter*)effectParameter;
		filtre->VignetteEffect(vignetteEffectParameter->radius, vignetteEffectParameter->power);

		imageLoad = new CImageLoadingFormat();
		cv::Mat mat = filtre->GetBitmap(true);
		imageLoad->SetPicture(mat);
		delete filtre;

		filtreEffet->SetBitmap(imageLoad);

		delete imageLoad;
	}

}

CImageLoadingFormat* CVignetteFilter::ApplyEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer)
{
	CImageLoadingFormat* imageLoad = nullptr;
	if (effectParameter != nullptr && !source.empty() && bitmapViewer != nullptr)
	{
		CVignetteEffectParameter* vignetteEffectParameter = (CVignetteEffectParameter*)effectParameter;
		CFiltreEffet* filter = bitmapViewer->GetFiltreEffet();
		if (filter != nullptr)
		{
			CImageLoadingFormat image;
			image.SetPicture(source);
			image.RotateExif(orientation);
			filter->SetBitmap(&image);
			
			filter->VignetteEffect(vignetteEffectParameter->radius, vignetteEffectParameter->power);
			imageLoad = new CImageLoadingFormat();
			cv::Mat bitmapOut = filter->GetBitmap(true);
			imageLoad->SetPicture(bitmapOut);
		}
	}

	return imageLoad;
}

void CVignetteFilter::RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
{
	if (effectParameter != nullptr && filtreEffet != nullptr)
	{
		CVignetteEffectParameter* vignetteEffectParameter = (CVignetteEffectParameter*)effectParameter;
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
