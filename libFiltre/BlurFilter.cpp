#include <header.h>
//
//  BrightAndContrastFilter.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//
#include <FiltreEffet.h>
#include "BlurFilter.h"
#include "BlurEffectParameter.h"
#include <RegardsBitmap.h>
#include <LibResource.h>
#include <FilterData.h>
#include <effect_id.h>
#include <ImageLoadingFormat.h>
#include "TreeElementValue.h"
#include <BitmapDisplay.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>

#include "bilateralFilter.h"
using namespace Regards::Filter;

CBlurFilter::CBlurFilter()
{
    libelleEffectSize =  CLibResource::LoadStringFromResource(L"LBLEFFECTRADIUSSIZE",1);//"Effect.Radius.Size";
}

CBlurFilter::~CBlurFilter()
{
    
}

wxString CBlurFilter::GetFilterLabel()
{
    return CLibResource::LoadStringFromResource("LBLfilterBlur", 1);
}


int CBlurFilter::GetNameFilter()
{
    return IDM_FILTRE_FLOU;
}

int CBlurFilter::GetTypeFilter()
{
    return CONVOLUTION_EFFECT;
}

void CBlurFilter::Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface)
{
    CBlurEffectParameter * blurEffectParameter = (CBlurEffectParameter *)effectParameter;
    
	this->source = source;

    vector<int> elementColor;
    for (int i = 1; i < 100; i++)
    {
        if (i % 2 == 1)
            elementColor.push_back(i);
    }

    
    filtreInterface->AddTreeInfos(libelleEffectSize,new CTreeElementValueInt(blurEffectParameter->size), &elementColor);
}

void CBlurFilter::FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key)
{
    CBlurEffectParameter * blurEffectParameter = (CBlurEffectParameter *)effectParameter;
    //Video Parameter
	CTreeElementValueInt * value = (CTreeElementValueInt *)valueData;

    if (key == libelleEffectSize)
    {
        blurEffectParameter->size = value->GetValue();
    }

}

void CBlurFilter::RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
{
    CBlurEffectParameter* blurEffectParameter = (CBlurEffectParameter*)effectParameter;
    if (blurEffectParameter != nullptr && filtreEffet != nullptr)
        filtreEffet->Blur(blurEffectParameter->size);   
}

bool CBlurFilter::NeedPreview()
{
    return true;
}

CEffectParameter* CBlurFilter::GetEffectPointer()
{
    return new CBlurEffectParameter();
}

int CBlurFilter::TypeApplyFilter()
{
    return 2;
}

CEffectParameter* CBlurFilter::GetDefaultEffectParameter()
{
    CBlurEffectParameter* blurEffect = new CBlurEffectParameter();
    blurEffect->size = 3;
    return blurEffect;
}


bool CBlurFilter::IsSourcePreview()
{
	return true;
}


void CBlurFilter::ApplyPreviewEffectSource(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer, CFiltreEffet* filtreEffet, CDraw* dessing)
{
	if (effectParameter != nullptr && source != nullptr)
	{
		CBlurEffectParameter* blurEffect = (CBlurEffectParameter*)effectParameter;
		filtreEffet->Blur(blurEffect->size);
	}

}


void CBlurFilter::ApplyPreviewEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer, CFiltreEffet* filtreEffet, CDraw* m_cDessin, int& widthOutput, int& heightOutput)
{

}

CImageLoadingFormat* CBlurFilter::ApplyEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer)
{
    CImageLoadingFormat* imageLoad = nullptr;
    if (effectParameter != nullptr && source != nullptr && bitmapViewer != nullptr)
    {  	
        CFiltreEffet * filter = bitmapViewer->GetFiltreEffet();
    	if(filter != nullptr)
    	{
            source->RotateExif(source->GetOrientation());
            CImageLoadingFormat image(false);
            image.SetPicture(source);
            filter->SetBitmap(&image);
            CBlurEffectParameter* blurEffect = (CBlurEffectParameter*)effectParameter;
            filter->Blur(blurEffect->size);
            imageLoad = new CImageLoadingFormat();
            CRegardsBitmap* bitmapOut = filter->GetBitmap(true);
            imageLoad->SetPicture(bitmapOut);
    	}
    }

    return imageLoad;
}
