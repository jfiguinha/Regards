#include <header.h>
//
//  BrightAndContrastFilter.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#include "bilateralFilter.h"
#include "BilateralEffectParameter.h"
#include <RegardsBitmap.h>
#include <LibResource.h>
#include <FilterData.h>
#include <BitmapDisplay.h>
#include <ImageLoadingFormat.h>
#include <FiltreEffet.h>
using namespace Regards::Filter;

CBilateralFilter::CBilateralFilter()
{
    libelleEffectSize = CLibResource::LoadStringFromResource(L"LBLEFFECTSIZE",1);//"Effect.Size";
	libelleEffectsigmaX= CLibResource::LoadStringFromResource(L"LBLEFFECTSIGMAX",1);//"Effect.Sigma.X";
	libelleEffectsigmaP= CLibResource::LoadStringFromResource(L"LBLEFFECTSIGMAP",1);//"Effect.Sigma.P";
}

int CBilateralFilter::TypeApplyFilter()
{
	return 2;
}

CBilateralFilter::~CBilateralFilter()
{
    
}

wxString CBilateralFilter::GetFilterLabel()
{
	return CLibResource::LoadStringFromResource("LBLBILATERALDENOISING", 1);
}


int CBilateralFilter::GetNameFilter()
{
	return IDM_FILTRE_BILATERAL;
}

int CBilateralFilter::GetTypeFilter()
{
	return CONVOLUTION_EFFECT; //
}

void CBilateralFilter::Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface)
{
    CBilateralEffectParameter * bilateralEffectParameter = (CBilateralEffectParameter *)effectParameter;
    
	this->source = source;

    vector<int> elementColor;
    for (auto i = 1; i < 100; i++)
	{
		if (i % 2 == 1)
			elementColor.push_back(i);
	}

    
    filtreInterface->AddTreeInfos(libelleEffectSize,new CTreeElementValueInt(bilateralEffectParameter->fSize), &elementColor);
	filtreInterface->AddTreeInfos(libelleEffectsigmaX,new CTreeElementValueInt(bilateralEffectParameter->sigmaX), &elementColor);
	filtreInterface->AddTreeInfos(libelleEffectsigmaP,new CTreeElementValueInt(bilateralEffectParameter->sigmaP), &elementColor);
}

void CBilateralFilter::FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key)
{
    CBilateralEffectParameter * bilateralEffectParameter = (CBilateralEffectParameter *)effectParameter;
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
	}

    if (key == libelleEffectSize)
    {
        bilateralEffectParameter->fSize = value;
    }
	else if (key == libelleEffectsigmaX)
    {
        bilateralEffectParameter->sigmaX = value;
    }
	else if (key == libelleEffectsigmaP)
    {
        bilateralEffectParameter->sigmaP = value;
    }
}

void CBilateralFilter::RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
{
	if (effectParameter != nullptr && filtreEffet != nullptr)
	{
		CBilateralEffectParameter* bilateralEffectParameter = (CBilateralEffectParameter*)effectParameter;
		filtreEffet->BilateralFilter(bilateralEffectParameter->fSize, bilateralEffectParameter->sigmaX, bilateralEffectParameter->sigmaP);
	}
}

bool CBilateralFilter::NeedPreview()
{
	return true;
}

CEffectParameter* CBilateralFilter::GetEffectPointer()
{
	return new CBilateralEffectParameter();
}

CEffectParameter* CBilateralFilter::GetDefaultEffectParameter()
{
	return new CBilateralEffectParameter();
}


bool CBilateralFilter::IsSourcePreview()
{
	return true;
}


void CBilateralFilter::ApplyPreviewEffectSource(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer, CFiltreEffet* filtreEffet, CDraw* dessing)
{
	if (effectParameter != nullptr && source != nullptr)
	{
		CBilateralEffectParameter* bilateralEffectParameter = (CBilateralEffectParameter*)effectParameter;
		filtreEffet->BilateralFilter(bilateralEffectParameter->fSize, bilateralEffectParameter->sigmaX, bilateralEffectParameter->sigmaP);
	}

}


void CBilateralFilter::ApplyPreviewEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer, CFiltreEffet* filtreEffet, CDraw* m_cDessin, int& widthOutput, int& heightOutput)
{

}

CImageLoadingFormat* CBilateralFilter::ApplyEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer)
{
	CImageLoadingFormat* imageLoad = nullptr;
	if (effectParameter != nullptr && source != nullptr && bitmapViewer != nullptr)
	{
		CFiltreEffet* filter = bitmapViewer->GetFiltreEffet();
		if (filter != nullptr)
		{	
			CBilateralEffectParameter* bilateralEffectParameter = (CBilateralEffectParameter*)effectParameter;
			filter->BilateralFilter(bilateralEffectParameter->fSize, bilateralEffectParameter->sigmaX, bilateralEffectParameter->sigmaP);
			imageLoad = new CImageLoadingFormat();
			CRegardsBitmap* bitmapOut = filter->GetBitmap(true);
			bitmapOut->RotateExif(source->GetOrientation());
			imageLoad->SetPicture(bitmapOut);
		}
	}

	return imageLoad;
}
