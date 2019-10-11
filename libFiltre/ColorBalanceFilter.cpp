#include <header.h>
//
//  CColorBalanceFilter.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#include "ColorBalanceFilter.h"
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
    return IDM_COLOR_BALANCE;
}

void CColorBalanceFilter::Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface)
{
    CRgbEffectParameter * rgbEffectParameter = (CRgbEffectParameter *)effectParameter;
    
	this->source = source;

    vector<int> elementColor;
    for (auto i = -255; i < 256; i++)
        elementColor.push_back(i);
    
    filtreInterface->AddTreeInfos(libelleEffectColorRed, new CTreeElementValueInt(rgbEffectParameter->red), &elementColor);
    filtreInterface->AddTreeInfos(libelleEffectColorGreen, new CTreeElementValueInt(rgbEffectParameter->green), &elementColor);
    filtreInterface->AddTreeInfos(libelleEffectColorBlue, new CTreeElementValueInt(rgbEffectParameter->blue), &elementColor);
}

void CColorBalanceFilter::FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key)
{
    CRgbEffectParameter * rgbEffectParameter = (CRgbEffectParameter *)effectParameter;
    
	CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
    int value = valueInt->GetValue();
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
