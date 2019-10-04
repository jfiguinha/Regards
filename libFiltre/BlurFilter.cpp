#include <header.h>
//
//  BrightAndContrastFilter.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#include "BlurFilter.h"
#include "BlurEffectParameter.h"
#include <RegardsBitmap.h>
#include <LibResource.h>
#include <FilterData.h>

using namespace Regards::Viewer;

CBlurFilter::CBlurFilter()
{
    libelleEffectSize =  CLibResource::LoadStringFromResource(L"LBLEFFECTRADIUSSIZE",1);//"Effect.Radius.Size";
}

CBlurFilter::~CBlurFilter()
{
    
}

int CBlurFilter::GetTypeFilter()
{
    return IDM_FILTRE_FLOU;
}

void CBlurFilter::Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface)
{
    CBlurEffectParameter * blurEffectParameter = (CBlurEffectParameter *)effectParameter;
    
	this->source = source;

    vector<int> elementColor;
    for (int i = 1; i < 100; i++)
        elementColor.push_back(i);
    
    filtreInterface->AddTreeInfos(libelleEffectSize,new CTreeElementValueInt(blurEffectParameter->size), &elementColor);
}

void CBlurFilter::FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key)
{
    CBlurEffectParameter * blurEffectParameter = (CBlurEffectParameter *)effectParameter;
    //Video Parameter
	CTreeElementValueInt * value = (CTreeElementValueInt *)valueData;

    if (key == libelleEffectSize)
    {
        blurEffectParameter->size = value->GetValue();
    }

}