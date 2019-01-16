//
//  PhotoFiltreFilter.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#include "PhotoFiltreFilter.h"
#include "PhotoFiltreEffectParameter.h"
#include <RegardsBitmap.h>
#include <LibResource.h>
#include <FilterData.h>

using namespace Regards::Viewer;

CPhotoFiltreFilter::CPhotoFiltreFilter()
{
    libelleEffectIntensity = CLibResource::LoadStringFromResource(L"LBLEFFECTINTENSITY",1);
    libelleEffectColorRed = CLibResource::LoadStringFromResource(L"LBLEFFECTCOLORRED",1);
    libelleEffectColorGreen = CLibResource::LoadStringFromResource(L"LBLEFFECTCOLORGREEN",1);
    libelleEffectColorBlue = CLibResource::LoadStringFromResource(L"LBLEFFECTCOLORBLUE",1);
}

CPhotoFiltreFilter::~CPhotoFiltreFilter()
{
    
}

int CPhotoFiltreFilter::GetTypeFilter()
{
    return IDM_FILTRE_SWIRL;
}

void CPhotoFiltreFilter::Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface)
{
    CPhotoFiltreEffectParameter * photoEffectParameter = (CPhotoFiltreEffectParameter *)effectParameter;
    
    vector<int> elementColor;
    for (auto i = 0; i < 256; i++)
        elementColor.push_back(i);
    
    vector<int> intensity;
    for (auto i = 0; i < 101; i++)
        intensity.push_back(i);
    
    filtreInterface->AddTreeInfos(libelleEffectIntensity, new CTreeElementValueInt(photoEffectParameter->intensity), &intensity);
    filtreInterface->AddTreeInfos(libelleEffectColorRed, new CTreeElementValueInt(photoEffectParameter->red), &elementColor);
    filtreInterface->AddTreeInfos(libelleEffectColorGreen, new CTreeElementValueInt(photoEffectParameter->green), &elementColor);
    filtreInterface->AddTreeInfos(libelleEffectColorBlue, new CTreeElementValueInt(photoEffectParameter->blue), &elementColor);
}

void CPhotoFiltreFilter::FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key)
{
    CPhotoFiltreEffectParameter * photoEffectParameter = (CPhotoFiltreEffectParameter *)effectParameter;
    
	CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
    int value = valueInt->GetValue();
    //Video Parameter
    if (key == libelleEffectColorRed)
    {
        photoEffectParameter->red = value;
    }
    else if (key == libelleEffectColorGreen)
    {
        photoEffectParameter->green = value;
    }
    else if (key == libelleEffectColorBlue)
    {
        photoEffectParameter->blue = value;
    }
    else if (key == libelleEffectIntensity)
    {
        photoEffectParameter->intensity = value;
    }
}