//
//  PosterisationFilter.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#include "PosterisationFilter.h"
#include <vector>
#include "PosterisationEffectParameter.h"
#include <RegardsBitmap.h>
#include <LibResource.h>
#include <FilterData.h>

using namespace Regards::Viewer;

CPosterisationFilter::CPosterisationFilter()
{
    libelleEffectLevel = CLibResource::LoadStringFromResource(L"LBLEFFECTLEVEL",1);
    libelleEffectGamma = CLibResource::LoadStringFromResource(L"LBLEFFECTGAMMA",1);
}

CPosterisationFilter::~CPosterisationFilter()
{
    
}

int CPosterisationFilter::GetTypeFilter()
{
    return IDM_FILTRE_SWIRL;
}

void CPosterisationFilter::Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface)
{
    CPosterisationEffectParameter * posterisationEffectParameter = (CPosterisationEffectParameter *)effectParameter;
    
    vector<int> elementColor;
    for (int i = 0; i < 256; i++)
        elementColor.push_back(i);
    
    vector<int> elementGamma;
    for (int i = 0; i < 11; i++)
        elementGamma.push_back(i);
    
    filtreInterface->AddTreeInfos(libelleEffectLevel, (int)posterisationEffectParameter->level, elementColor);
    filtreInterface->AddTreeInfos(libelleEffectGamma, (int)posterisationEffectParameter->gamma * 10, elementGamma);
}

void CPosterisationFilter::FilterChangeParam(CEffectParameter * effectParameter, const int &valueData, const wxString &key)
{
    CPosterisationEffectParameter * posterisationEffectParameter = (CPosterisationEffectParameter *)effectParameter;
    
    float value = valueData;
    //Video Parameter
    if (key == libelleEffectLevel)
    {
        posterisationEffectParameter->level = value;
    }
    if (key == libelleEffectGamma)
    {
        posterisationEffectParameter->gamma = value / 10.0;
    }
}

