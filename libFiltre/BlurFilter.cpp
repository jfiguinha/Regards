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
#include <FiltreEffet.h>
using namespace Regards::Filter;

CBlurFilter::CBlurFilter()
{
    libelleEffectSize =  CLibResource::LoadStringFromResource(L"LBLEFFECTRADIUSSIZE",1);//"Effect.Radius.Size";
}

CBlurFilter::~CBlurFilter()
{
    
}

int CBlurFilter::GetNameFilter()
{
    return IDM_FILTRE_FLOU;
}

int CBlurFilter::GetTypeFilter()
{
    return CONVOLUTION_EFFECT; //
}

void CBlurFilter::Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface)
{
    CBlurEffectParameter * blurEffectParameter = (CBlurEffectParameter *)effectParameter;
    
	this->source = source;

    vector<int> elementColor;
    for (int i = 1; i < 100; i++)
        elementColor.push_back(i);
    
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
    if(blurEffectParameter != nullptr && filtreEffet != nullptr)
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

CEffectParameter* CBlurFilter::GetDefaultEffectParameter()
{
    CBlurEffectParameter* blurEffect = new CBlurEffectParameter();
    blurEffect->size = 3;
    return blurEffect;
}