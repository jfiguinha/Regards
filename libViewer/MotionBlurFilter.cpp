//
//  MotionBlurFilter.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#include "MotionBlurFilter.h"
#include <vector>
#include "MotionBlurEffectParameter.h"
#include <RegardsBitmap.h>
#include <LibResource.h>
#include <FilterData.h>

using namespace Regards::Viewer;

CMotionBlurFilter::CMotionBlurFilter()
{
    libelleEffectRadius = CLibResource::LoadStringFromResource(L"LBLEFFECTRADIUS",1);
    libelleEffectSigma = CLibResource::LoadStringFromResource(L"LBLEFFECTSIGMA",1);
    libelleEffectAngle = CLibResource::LoadStringFromResource(L"LBLEFFECTANGLE",1);
}

CMotionBlurFilter::~CMotionBlurFilter()
{
    
}

int CMotionBlurFilter::GetTypeFilter()
{
    return IDM_FILTRE_SWIRL;
}

void CMotionBlurFilter::Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface)
{
    CMotionBlurEffectParameter * motionBlurParameter = (CMotionBlurEffectParameter *)effectParameter;
    
    vector<int> elementAngle;
    for (int i = 0; i < 361; i++)
        elementAngle.push_back(i);
    
    
    vector<int> elementSample;
    for (int i = 0; i < 11; i++)
        elementSample.push_back(i);
    
    vector<int> velocity;
    for (int i = 0; i < 101; i++)
        velocity.push_back(i);
    
    filtreInterface->AddTreeInfos(libelleEffectRadius, motionBlurParameter->radius, elementSample);
    filtreInterface->AddTreeInfos(libelleEffectSigma, motionBlurParameter->sigma, velocity);
    filtreInterface->AddTreeInfos(libelleEffectAngle, motionBlurParameter->angle, elementAngle);
}

void CMotionBlurFilter::FilterChangeParam(CEffectParameter * effectParameter, const int &valueData, const wxString &key)
{
    CMotionBlurEffectParameter * motionBlurParameter = (CMotionBlurEffectParameter *)effectParameter;
    
    float value = valueData;
    //Video Parameter
    if (key == libelleEffectRadius)
    {
        motionBlurParameter->radius = value;
    }
    else if (key == libelleEffectSigma)
    {
        motionBlurParameter->sigma = value;
    }
    else if (key == libelleEffectAngle)
    {
        motionBlurParameter->angle = value;
    }
}