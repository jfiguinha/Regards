//
//  BrightAndContrastFilter.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#include "BrightAndContrastFilter.h"
#include <vector>
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
    return IDM_FILTRE_SWIRL;
}

void CBrightAndContrastFilter::Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface)
{
    CBrightAndContrastEffectParameter * BrightAndContrastEffectParameter = (CBrightAndContrastEffectParameter *)effectParameter;
    
    vector<int> elementColor;
    for (int i = -100; i < 101; i++)
        elementColor.push_back(i);
    
    filtreInterface->AddTreeInfos(libelleEffectContrast, (int)BrightAndContrastEffectParameter->contrast + 100, elementColor);
    filtreInterface->AddTreeInfos(libelleEffectLightness, (int)BrightAndContrastEffectParameter->brightness + 100, elementColor);
}

void CBrightAndContrastFilter::FilterChangeParam(CEffectParameter * effectParameter, const int &valueData, const wxString &key)
{
    CBrightAndContrastEffectParameter * BrightAndContrastEffectParameter = (CBrightAndContrastEffectParameter *)effectParameter;
    
    float value = valueData;
    //Video Parameter
    if (key == libelleEffectContrast)
    {
        BrightAndContrastEffectParameter->contrast = value;
    }
    if (key == libelleEffectLightness)
    {
        BrightAndContrastEffectParameter->brightness = value;
    }
}