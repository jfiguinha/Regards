//
//  SolarisationFilter.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#include "SolarisationFilter.h"
#include <vector>
#include "SolarisationEffectParameter.h"
#include <RegardsBitmap.h>
#include <libResource.h>
#include <FilterData.h>

using namespace Regards::Viewer;

CSolarisationFilter::CSolarisationFilter()
{
    libelleEffectThreshold = CLibResource::LoadStringFromResource(L"LBLEFFECTTHRESHOLD",1);
}

CSolarisationFilter::~CSolarisationFilter()
{
    
}

int CSolarisationFilter::GetTypeFilter()
{
    return IDM_FILTRE_SWIRL;
}

void CSolarisationFilter::Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface)
{
    CSolarisationEffectParameter * solarisationEffectParameter = (CSolarisationEffectParameter *)effectParameter;
    
    vector<int> elementColor;
    for (int i = 0; i < 256; i++)
        elementColor.push_back(i);
    
    filtreInterface->AddTreeInfos(libelleEffectThreshold, (int)solarisationEffectParameter->threshold, elementColor);
}

void CSolarisationFilter::FilterChangeParam(CEffectParameter * effectParameter, const int &valueData, const wxString &key)
{
    CSolarisationEffectParameter * solarisationEffectParameter = (CSolarisationEffectParameter *)effectParameter;
    
    float value = valueData;
    //Video Parameter
    if (key == libelleEffectThreshold)
    {
        solarisationEffectParameter->threshold = value;
    }
}
