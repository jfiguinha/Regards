#include <header.h>
//
//  BrightAndContrastFilter.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#include "BrightAndContrastFilter.h"
#include "BrightAndContrastEffectParameter.h"
#include <RegardsBitmap.h>
#include <LibResource.h>
#include <FilterData.h>

using namespace Regards::Viewer;

CBrightAndContrastFilter::CBrightAndContrastFilter()
{
    libelleEffectContrast = CLibResource::LoadStringFromResource(L"LBLEFFECTCONTRAST",1);
    libelleEffectLightness = CLibResource::LoadStringFromResource(L"LBLEFFECTBRIGHTNESS",1);
}

CBrightAndContrastFilter::~CBrightAndContrastFilter()
{
    
}

int CBrightAndContrastFilter::GetTypeFilter()
{
    return IDM_IMAGE_LIGHTCONTRAST;
}

void CBrightAndContrastFilter::Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface)
{
    CBrightAndContrastEffectParameter * BrightAndContrastEffectParameter = (CBrightAndContrastEffectParameter *)effectParameter;
	
	this->source = source;

    vector<int> elementColor;
    for (int i = -100; i < 101; i++)
        elementColor.push_back(i);
    
    filtreInterface->AddTreeInfos(libelleEffectContrast,new CTreeElementValueInt(BrightAndContrastEffectParameter->contrast), &elementColor);
    filtreInterface->AddTreeInfos(libelleEffectLightness,new CTreeElementValueInt(BrightAndContrastEffectParameter->brightness), &elementColor);
}

void CBrightAndContrastFilter::FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key)
{
    CBrightAndContrastEffectParameter * BrightAndContrastEffectParameter = (CBrightAndContrastEffectParameter *)effectParameter;
    //Video Parameter
	CTreeElementValueInt * value = (CTreeElementValueInt *)valueData;

    if (key == libelleEffectContrast)
    {
        BrightAndContrastEffectParameter->contrast = value->GetValue();
    }
    else if (key == libelleEffectLightness)
    {
        BrightAndContrastEffectParameter->brightness = value->GetValue();
    }
}