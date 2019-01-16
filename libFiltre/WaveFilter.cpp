//
//  CWaveFilter.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#include "WaveFilter.h"
#include "WaveEffectParameter.h"
#include <RegardsBitmap.h>
#include <LibResource.h>
#include <FilterData.h>

using namespace Regards::Viewer;

CWaveFilter::CWaveFilter()
{
    libelleEffectRadius = CLibResource::LoadStringFromResource(L"LBLEFFECTRADIUS",1);
	libelleEffectScale = CLibResource::LoadStringFromResource(L"LBLEFFECTSCALE", 1);
    libelleEffectHeight = CLibResource::LoadStringFromResource(L"LBLEFFECTHEIGHT", 1);

}

CWaveFilter::~CWaveFilter()
{
    
}

int CWaveFilter::GetTypeFilter()
{
    return IDM_WAVE_EFFECT;
}

void CWaveFilter::Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface)
{
    CWaveEffectParameter * waveParameter = (CWaveEffectParameter *)effectParameter;
    
    vector<int> elementIntensity;
    for (int i = 0; i < 40; i++)
        elementIntensity.push_back(i);
    
    vector<int> elementColor;
    for (int i = 0; i < 30; i++)
        elementColor.push_back(i);
    
    vector<int> elementBitmapHeight;
    for (int i = 0; i < source->GetBitmapHeight() / 2; i++)
        elementBitmapHeight.push_back(i);
    
    filtreInterface->AddTreeInfos(libelleEffectHeight, new CTreeElementValueInt(waveParameter->height), &elementBitmapHeight);
    filtreInterface->AddTreeInfos(libelleEffectScale, new CTreeElementValueInt(waveParameter->scale), &elementIntensity);
    filtreInterface->AddTreeInfos(libelleEffectRadius, new CTreeElementValueInt(waveParameter->radius), &elementColor);
}

void CWaveFilter::FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key)
{
	CWaveEffectParameter * waveParameter = (CWaveEffectParameter *)effectParameter;
    
	CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
    int value = valueInt->GetValue();
    //Video Parameter
    if (key == libelleEffectHeight)
    {
		waveParameter->height = value;
    }
    else if (key == libelleEffectRadius)
    {
		waveParameter->radius = value;
    }
    else if (key == libelleEffectScale)
    {
		waveParameter->scale = value;
    }
}