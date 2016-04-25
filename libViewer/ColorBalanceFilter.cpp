//
//  CColorBalanceFilter.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#include "ColorBalanceFilter.h"
#include <vector>
#include "RgbEffectParameter.h"
#include <RegardsBitmap.h>
#include <LibResource.h>
#include <FilterData.h>

using namespace Regards::Viewer;

CColorBalanceFilter::CColorBalanceFilter()
{
    libelleEffectColorRed = CLibResource::LoadStringFromResource(L"LBLEFFECTCOLORRED",1);
    libelleEffectColorGreen = CLibResource::LoadStringFromResource(L"LBLEFFECTCOLORGREEN",1);
    libelleEffectColorBlue = CLibResource::LoadStringFromResource(L"LBLEFFECTCOLORBLUE",1);
}

CColorBalanceFilter::~CColorBalanceFilter()
{
    
}

int CColorBalanceFilter::GetTypeFilter()
{
    return IDM_FILTRE_SWIRL;
}

void CColorBalanceFilter::Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface)
{
    CRgbEffectParameter * rgbEffectParameter = (CRgbEffectParameter *)effectParameter;
    
    vector<int> elementColor;
    for (int i = -255; i < 256; i++)
        elementColor.push_back(i);
    
    filtreInterface->AddTreeInfos(libelleEffectColorRed, rgbEffectParameter->red + 255, elementColor);
    filtreInterface->AddTreeInfos(libelleEffectColorGreen, rgbEffectParameter->green + 255, elementColor);
    filtreInterface->AddTreeInfos(libelleEffectColorBlue, rgbEffectParameter->blue + 255, elementColor);
}

void CColorBalanceFilter::FilterChangeParam(CEffectParameter * effectParameter, const int &valueData, const wxString &key)
{
    CRgbEffectParameter * rgbEffectParameter = (CRgbEffectParameter *)effectParameter;
    
    float value = valueData;
    //Video Parameter
    if (key == libelleEffectColorRed)
    {
        rgbEffectParameter->red = value;
    }
    else if (key == libelleEffectColorGreen)
    {
        rgbEffectParameter->green = value;
    }
    else if (key == libelleEffectColorBlue)
    {
        rgbEffectParameter->blue = value;
    }
}
