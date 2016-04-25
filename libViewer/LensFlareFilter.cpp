//
//  CLensFlareFilter.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#include "LensFlareFilter.h"
#include <vector>
#include "LensFlareEffectParameter.h"
#include <RegardsBitmap.h>
#include <libResource.h>
#include <FilterData.h>

using namespace Regards::Viewer;

CLensFlareFilter::CLensFlareFilter()
{
    libelleEffectRadius = CLibResource::LoadStringFromResource(L"LBLEFFECTRADIUS",1);
    libelleEffectIntensity = CLibResource::LoadStringFromResource(L"LBLEFFECTINTENSITY",1);
    libelleEffectLightness = CLibResource::LoadStringFromResource(L"LBLEFFECTBRIGHTNESS",1);
    libelleColor = CLibResource::LoadStringFromResource(L"LBLCOLOREFFECT",1);
}

CLensFlareFilter::~CLensFlareFilter()
{
    
}

int CLensFlareFilter::GetTypeFilter()
{
    return IDM_FILTRE_SWIRL;
}

void CLensFlareFilter::Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface)
{
    CLensFlareEffectParameter * lensFlareParameter = (CLensFlareEffectParameter *)effectParameter;
    
    vector<int> elementIntensity;
    for (int i = 0; i < 101; i++)
        elementIntensity.push_back(i);
    
    vector<int> elementColor;
    for (int i = 0; i < 361; i++)
        elementColor.push_back(i);
    
    vector<int> elementBitmapHeight;
    for (int i = 0; i < 101; i++)
        elementBitmapHeight.push_back(i);
    
    filtreInterface->AddTreeInfos(libelleEffectLightness, lensFlareParameter->brightness, elementIntensity);
    filtreInterface->AddTreeInfos(libelleEffectIntensity, lensFlareParameter->colorIntensity, elementIntensity);
    filtreInterface->AddTreeInfos(libelleEffectRadius, lensFlareParameter->size, elementBitmapHeight);
    filtreInterface->AddTreeInfos(libelleColor, lensFlareParameter->color, elementColor);
}

void CLensFlareFilter::FilterChangeParam(CEffectParameter * effectParameter, const int &valueData, const wxString &key)
{
    CLensFlareEffectParameter * lensFlareParameter = (CLensFlareEffectParameter *)effectParameter;
    
    float value = valueData;
    //Video Parameter
    if (key == libelleEffectLightness)
    {
        lensFlareParameter->brightness = value;
    }
    else if (key == libelleEffectIntensity)
    {
        lensFlareParameter->colorIntensity = value;
    }
    else if (key == libelleEffectRadius)
    {
        lensFlareParameter->size = value;
    }
    else if (key == libelleColor)
    {
        lensFlareParameter->color = value;
    }
}