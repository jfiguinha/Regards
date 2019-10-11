#include <header.h>
//
//  BrightAndContrastFilter.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#include "NlmeansFilter.h"
#include "NlmeansEffectParameter.h"
#include <RegardsBitmap.h>
#include <LibResource.h>
#include <FilterData.h>

using namespace Regards::Viewer;

CNlmeansFilter::CNlmeansFilter()
{
    libelleEffectFSize = CLibResource::LoadStringFromResource(L"LBLEFFECTFSIZE",1);//"Effect.FSize";
	libelleEffectBSize= CLibResource::LoadStringFromResource(L"LBLEFFECTBSIZE",1);//"Effect.BSize";
	libelleEffectsigma= CLibResource::LoadStringFromResource(L"LBLEFFECTSIGMA",1);//LBLEFFECTSIGMA;//"Effect.Sigma";
}

CNlmeansFilter::~CNlmeansFilter()
{
    
}

int CNlmeansFilter::GetTypeFilter()
{
    return IDM_FILTRE_NLMEAN;
}

void CNlmeansFilter::Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface)
{
    CNlmeansEffectParameter * nlmeansEffectParameter = (CNlmeansEffectParameter *)effectParameter;
    
	this->source = source;

    vector<int> elementColor;
    for (auto i = 1; i < 100; i++)
        elementColor.push_back(i);

	vector<float> elementSigma;
	for (float i = 0; i < 100; i+=0.1)
		elementSigma.push_back(i);
    
    filtreInterface->AddTreeInfos(libelleEffectFSize,new CTreeElementValueInt(nlmeansEffectParameter->fSize), &elementColor);
	filtreInterface->AddTreeInfos(libelleEffectBSize,new CTreeElementValueInt(nlmeansEffectParameter->bSize), &elementColor);
	filtreInterface->AddTreeInfos(libelleEffectsigma,new CTreeElementValueFloat(nlmeansEffectParameter->sigma, 1), &elementSigma,4);
}

void CNlmeansFilter::FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key)
{
    CNlmeansEffectParameter * nlmeansEffectParameter = (CNlmeansEffectParameter *)effectParameter;
    //Video Parameter
	float value = 0.0;
	switch(valueData->GetType())
	{
		case TYPE_INT:
			{
				CTreeElementValueInt * intValue = (CTreeElementValueInt*)valueData;
				value = intValue->GetValue();
			}
			break;
		case TYPE_FLOAT:
			{
				CTreeElementValueFloat * intValue = (CTreeElementValueFloat*)valueData;
				value = intValue->GetValue();
			}
			break;
		case TYPE_BOOL:
			{
				CTreeElementValueBool * intValue = (CTreeElementValueBool*)valueData;
				value = intValue->GetValue();
			}
			break;
	}

    if (key == libelleEffectFSize)
    {
        nlmeansEffectParameter->fSize = value;
    }
	else if (key == libelleEffectBSize)
    {
        nlmeansEffectParameter->bSize = value;
    }
	else if (key == libelleEffectsigma)
    {
        nlmeansEffectParameter->sigma = value;
    }
}