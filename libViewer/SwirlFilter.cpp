//
//  SwirlFilter.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#include "SwirlFilter.h"
#include <vector>
#include "SwirlEffectParameter.h"
#include <RegardsBitmap.h>
#include <libResource.h>
#include <FilterData.h>

using namespace Regards::Viewer;

CSwirlFilter::CSwirlFilter()
{
    libelleEffectRadius = CLibResource::LoadStringFromResource(L"LBLEFFECTRADIUS",1);
    libelleEffectAngle = CLibResource::LoadStringFromResource(L"LBLEFFECTANGLE",1);
}

CSwirlFilter::~CSwirlFilter()
{
    
}

int CSwirlFilter::GetTypeFilter()
{
    return IDM_FILTRE_SWIRL;
}

void CSwirlFilter::Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface)
{   
   	CSwirlEffectParameter * swirlEffectParameter = (CSwirlEffectParameter *)effectParameter;
    
    swirlEffectParameter->bitmapWidth = source->GetBitmapWidth();
    swirlEffectParameter->bitmapHeight = source->GetBitmapHeight();
    
    vector<int> elementAngle;
    for (int i = 0; i < 360; i++)
        elementAngle.push_back(i);
    
    vector<int> elementRadius;
    for (int i = 0; i < swirlEffectParameter->bitmapWidth; i++)
        elementRadius.push_back(i);
    
    filtreInterface->AddTreeInfos(libelleEffectRadius, swirlEffectParameter->radius, elementRadius);
    filtreInterface->AddTreeInfos(libelleEffectAngle, swirlEffectParameter->angle, elementAngle);
}

void CSwirlFilter::FilterChangeParam(CEffectParameter * effectParameter, const int &valueData, const wxString &key)
{
    CSwirlEffectParameter * swirlEffectParameter = (CSwirlEffectParameter *)effectParameter;
    
    float value = valueData;
    //Video Parameter
    if (key == libelleEffectRadius)
    {
        swirlEffectParameter->radius = value;
    }
    if (key == libelleEffectAngle)
    {
        swirlEffectParameter->angle = value;
    }
}