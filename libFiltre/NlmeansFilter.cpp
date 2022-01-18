#include <header.h>
//
//  BrightAndContrastFilter.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#include "NlmeansFilter.h"
#include "NlmeansEffectParameter.h"
#include <RegardsBitmap.h>
#include <LibResource.h>
#include <FilterData.h>
#include <FiltreEffet.h>
#include <ImageLoadingFormat.h>
#include <BitmapDisplay.h>
using namespace Regards::Filter;

CNlmeansFilter::CNlmeansFilter()
{
	libelleEffectH = "Effect.Filter Strengh";//CLibResource::LoadStringFromResource(L"LBLEFFECTFSIZE",1);//
	libelleEffectHColor = "Effect.Filter Color Strengh";//CLibResource::LoadStringFromResource(L"LBLEFFECTFSIZE",1);//
	libelleEffectTemplateWindowSize = "Effect.Template Size";//CLibResource::LoadStringFromResource(L"LBLEFFECTBSIZE",1);//"Effect.BSize";
	libelleEffectSearchWindowSize = "Effect.Search Size";//CLibResource::LoadStringFromResource(L"LBLEFFECTSIGMA",1);//LBLEFFECTSIGMA;//"Effect.Sigma";
}

int CNlmeansFilter::TypeApplyFilter()
{
	return 2;
}

CNlmeansFilter::~CNlmeansFilter()
{
    
}

wxString CNlmeansFilter::GetFilterLabel()
{
	return CLibResource::LoadStringFromResource("LBLNLMEANS", 1);
}

int CNlmeansFilter::GetNameFilter()
{
	return IDM_FILTRE_NLMEAN;
}

int CNlmeansFilter::GetTypeFilter()
{
	return CONVOLUTION_EFFECT; //return IDM_FILTRE_NLMEAN;
}

void CNlmeansFilter::Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface)
{
    CNlmeansEffectParameter * nlmeansEffectParameter = (CNlmeansEffectParameter *)effectParameter;
    
	this->source = source;

    vector<int> elementColor;
	for (auto i = 1; i < 26; i++)
	{
		if(i%2 == 1)
			elementColor.push_back(i);
	}


	vector<int> elementH;
	for (auto i = 1; i < 26; i++)
	{
		elementH.push_back(i);
	}
    
    filtreInterface->AddTreeInfos(libelleEffectH,new CTreeElementValueInt(nlmeansEffectParameter->h), &elementH);
	filtreInterface->AddTreeInfos(libelleEffectHColor, new CTreeElementValueInt(nlmeansEffectParameter->hColor), &elementH);
	filtreInterface->AddTreeInfos(libelleEffectTemplateWindowSize,new CTreeElementValueInt(nlmeansEffectParameter->templateWindowSize), &elementColor);
	filtreInterface->AddTreeInfos(libelleEffectSearchWindowSize,new CTreeElementValueInt(nlmeansEffectParameter->searchWindowSize), &elementColor);
}

void CNlmeansFilter::FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key)
{
    CNlmeansEffectParameter * nlmeansEffectParameter = (CNlmeansEffectParameter *)effectParameter;
    //Video Parameter
	float value = 0.0;
	switch(valueData->GetType())
	{
		case TYPE_ELEMENT_INT:
			{
				CTreeElementValueInt * intValue = (CTreeElementValueInt*)valueData;
				value = intValue->GetValue();
			}
			break;
		case TYPE_ELEMENT_FLOAT:
			{
				CTreeElementValueFloat * intValue = (CTreeElementValueFloat*)valueData;
				value = intValue->GetValue();
			}
			break;
		case TYPE_ELEMENT_BOOL:
			{
				CTreeElementValueBool * intValue = (CTreeElementValueBool*)valueData;
				value = intValue->GetValue();
			}
			break;
		default: ;
	}

    if (key == libelleEffectH)
    {
        nlmeansEffectParameter->h = value;
    }
	else if (key == libelleEffectHColor)
	{
		nlmeansEffectParameter->hColor = value;
	}
	else if (key == libelleEffectTemplateWindowSize)
    {
        nlmeansEffectParameter->templateWindowSize = value;
    }
	else if (key == libelleEffectSearchWindowSize)
    {
        nlmeansEffectParameter->searchWindowSize = value;
    }
}

void CNlmeansFilter::RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
{
	if (effectParameter != nullptr && filtreEffet != nullptr)
	{
		CNlmeansEffectParameter* nlmeansEffectParameter = (CNlmeansEffectParameter*)effectParameter;
		filtreEffet->NlmeansFilter(nlmeansEffectParameter->h, nlmeansEffectParameter->hColor, nlmeansEffectParameter->templateWindowSize, nlmeansEffectParameter->searchWindowSize);
	}
}

bool CNlmeansFilter::NeedPreview()
{
	return true;
}

CEffectParameter* CNlmeansFilter::GetEffectPointer()
{
	return new CNlmeansEffectParameter();
}

CEffectParameter* CNlmeansFilter::GetDefaultEffectParameter()
{
	return new CNlmeansEffectParameter();
}


bool CNlmeansFilter::IsSourcePreview()
{
	return true;
}


void CNlmeansFilter::ApplyPreviewEffectSource(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer, CFiltreEffet* filtreEffet, CDraw* dessing)
{
	if (effectParameter != nullptr && source != nullptr)
	{
		CNlmeansEffectParameter* nlmeansEffectParameter = (CNlmeansEffectParameter*)effectParameter;
		filtreEffet->NlmeansFilter(nlmeansEffectParameter->h, nlmeansEffectParameter->hColor, nlmeansEffectParameter->templateWindowSize, nlmeansEffectParameter->searchWindowSize);
	}

}


void CNlmeansFilter::ApplyPreviewEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer, CFiltreEffet* filtreEffet, CDraw* m_cDessin, int& widthOutput, int& heightOutput)
{

}


CImageLoadingFormat* CNlmeansFilter::ApplyEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer)
{
	CImageLoadingFormat* imageLoad = nullptr;
	if (effectParameter != nullptr && source != nullptr && bitmapViewer != nullptr)
	{
		CNlmeansEffectParameter* nlmeansEffectParameter = (CNlmeansEffectParameter*)effectParameter;
		CFiltreEffet* filter = bitmapViewer->GetFiltreEffet();
		if (filter != nullptr)
		{
			source->RotateExif(source->GetOrientation());
			CImageLoadingFormat image(false);
			image.SetPicture(source);
			filter->SetBitmap(&image);
			
			filter->NlmeansFilter(nlmeansEffectParameter->h, nlmeansEffectParameter->hColor, nlmeansEffectParameter->templateWindowSize, nlmeansEffectParameter->searchWindowSize);
			imageLoad = new CImageLoadingFormat();
			CRegardsBitmap* bitmapOut = filter->GetBitmap(true);
			imageLoad->SetPicture(bitmapOut);
		}
	}

	return imageLoad;
}
