#include <header.h>
//
//  PosterisationFilter.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#include "PosterisationFilter.h"
#include "PosterisationEffectParameter.h"
#include <RegardsBitmap.h>
#include <LibResource.h>
#include <FilterData.h>
#include <RenderOpenGL.h>
#include <FiltreEffet.h>
#include <BitmapDisplay.h>
#include <ImageLoadingFormat.h>
using namespace Regards::Filter;

int CPosterisationFilter::TypeApplyFilter()
{
	return 2;
}

CPosterisationFilter::CPosterisationFilter()
{
    libelleEffectLevel = CLibResource::LoadStringFromResource(L"LBLEFFECTLEVEL",1);
    libelleEffectGamma = CLibResource::LoadStringFromResource(L"LBLEFFECTGAMMA",1);
}

CPosterisationFilter::~CPosterisationFilter()
{
    
}

wxString CPosterisationFilter::GetFilterLabel()
{
	return CLibResource::LoadStringFromResource("LBLfilterPosterisation", 1);
}

int CPosterisationFilter::GetNameFilter()
{
	return ID_AJUSTEMENT_POSTERISATION;
}

int CPosterisationFilter::GetTypeFilter()
{
	return SPECIAL_EFFECT; //
}

void CPosterisationFilter::Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface)
{
    CPosterisationEffectParameter * posterisationEffectParameter = (CPosterisationEffectParameter *)effectParameter;
    
	this->source = source;

    vector<int> elementColor;
    for (auto i = 0; i < 256; i++)
        elementColor.push_back(i);
    
    vector<int> elementGamma;
    for (auto i = 0; i < 11; i++)
        elementGamma.push_back(i);
    
    filtreInterface->AddTreeInfos(libelleEffectLevel, new CTreeElementValueInt(posterisationEffectParameter->level), &elementColor);
    filtreInterface->AddTreeInfos(libelleEffectGamma, new CTreeElementValueInt(posterisationEffectParameter->gamma * 10), &elementGamma);
}

void CPosterisationFilter::FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key)
{
    CPosterisationEffectParameter * posterisationEffectParameter = (CPosterisationEffectParameter *)effectParameter;
    
	CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;

	if (posterisationEffectParameter != nullptr && valueInt != nullptr)
	{
		int value = valueInt->GetValue();
		//Video Parameter
		if (key == libelleEffectLevel)
		{
			posterisationEffectParameter->level = value;
		}
		if (key == libelleEffectGamma)
		{
			posterisationEffectParameter->gamma = value / 10.0;
		}
	}
}


void CPosterisationFilter::RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
{
	if (effectParameter != nullptr && filtreEffet != nullptr)
	{
		CPosterisationEffectParameter* posterisationFiltreParameter = (CPosterisationEffectParameter*)effectParameter;
		filtreEffet->Posterize(posterisationFiltreParameter->level, posterisationFiltreParameter->gamma);
	}
}

bool CPosterisationFilter::NeedPreview()
{
	return true;
}

CEffectParameter* CPosterisationFilter::GetEffectPointer()
{
	return new CPosterisationEffectParameter();
}

CEffectParameter* CPosterisationFilter::GetDefaultEffectParameter()
{
	CPosterisationEffectParameter* posterization = new CPosterisationEffectParameter();
	posterization->gamma = 20;
	posterization->level = 20;
	return posterization;
}


bool CPosterisationFilter::IsSourcePreview()
{
	return true;
}


void CPosterisationFilter::ApplyPreviewEffectSource(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer, CFiltreEffet* filtreEffet, CDraw* dessing)
{

	if (effectParameter != nullptr && source != nullptr)
	{
		CPosterisationEffectParameter* posterisationFiltreParameter = (CPosterisationEffectParameter*)effectParameter;
		filtreEffet->Posterize(posterisationFiltreParameter->level, posterisationFiltreParameter->gamma);
	}
}


void CPosterisationFilter::ApplyPreviewEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer, CFiltreEffet* filtreEffet, CDraw* m_cDessin, int& widthOutput, int& heightOutput)
{

}

CImageLoadingFormat* CPosterisationFilter::ApplyEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer)
{
	CImageLoadingFormat* imageLoad = nullptr;
	if (effectParameter != nullptr && source != nullptr && bitmapViewer != nullptr)
	{
		CFiltreEffet* filtre = bitmapViewer->GetFiltreEffet();

		if (source != nullptr && filtre != nullptr)
		{
			source->RotateExif(source->GetOrientation());
			CImageLoadingFormat image(false);
			image.SetPicture(source);
			filtre->SetBitmap(&image);
			
			CPosterisationEffectParameter* posterisationFiltreParameter = (CPosterisationEffectParameter*)effectParameter;
			filtre->Posterize(posterisationFiltreParameter->level, posterisationFiltreParameter->gamma);
			imageLoad = new CImageLoadingFormat();
			CRegardsBitmap* bitmapOut = filtre->GetBitmap(true);
			imageLoad->SetPicture(bitmapOut);
		}
	}

	return imageLoad;
}
