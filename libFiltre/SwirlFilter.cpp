#include <header.h>
//
//  SwirlFilter.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#include "SwirlFilter.h"
#include "SwirlEffectParameter.h"
#include <RegardsBitmap.h>
#include <LibResource.h>
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
    for (auto i = 0; i < 360; i++)
        elementAngle.push_back(i);
    
    vector<int> elementRadius;
    for (auto i = 0; i < swirlEffectParameter->bitmapWidth; i++)
        elementRadius.push_back(i);
    
    filtreInterface->AddTreeInfos(libelleEffectRadius, new CTreeElementValueInt(swirlEffectParameter->radius), &elementRadius);
    filtreInterface->AddTreeInfos(libelleEffectAngle, new CTreeElementValueInt(swirlEffectParameter->angle), &elementAngle);
}

void CSwirlFilter::FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key)
{
    CSwirlEffectParameter * swirlEffectParameter = (CSwirlEffectParameter *)effectParameter;
    
	this->source = source;

	CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
    int value = valueInt->GetValue();
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
