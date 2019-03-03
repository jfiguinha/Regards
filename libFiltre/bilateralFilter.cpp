#include <header.h>
//
//  BrightAndContrastFilter.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#include "bilateralFilter.h"
#include "BilateralEffectParameter.h"
#include <RegardsBitmap.h>
#include <LibResource.h>
#include <FilterData.h>

using namespace Regards::Viewer;

CBilateralFilter::CBilateralFilter()
{
    libelleEffectSize = CLibResource::LoadStringFromResource(L"LBLEFFECTSIZE",1);//"Effect.Size";
	libelleEffectsigmaX= CLibResource::LoadStringFromResource(L"LBLEFFECTSIGMAX",1);//"Effect.Sigma.X";
	libelleEffectsigmaP= CLibResource::LoadStringFromResource(L"LBLEFFECTSIGMAP",1);//"Effect.Sigma.P";
}

CBilateralFilter::~CBilateralFilter()
{
    
}

int CBilateralFilter::GetTypeFilter()
{
    return IDM_FILTRE_BILATERAL;
}

void CBilateralFilter::Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface)
{
    CBilateralEffectParameter * bilateralEffectParameter = (CBilateralEffectParameter *)effectParameter;
    
    vector<int> elementColor;
    for (auto i = 1; i < 100; i++)
        elementColor.push_back(i);

	vector<float> elementSigma;
	for (float i = 0; i < 100; i+=0.1)
		elementSigma.push_back(i);
    
    filtreInterface->AddTreeInfos(libelleEffectSize,new CTreeElementValueInt(bilateralEffectParameter->fSize), &elementColor);
	filtreInterface->AddTreeInfos(libelleEffectsigmaX,new CTreeElementValueFloat(bilateralEffectParameter->sigmaX,1), &elementSigma,4);
	filtreInterface->AddTreeInfos(libelleEffectsigmaP,new CTreeElementValueFloat(bilateralEffectParameter->sigmaP,1), &elementSigma,4);
}

void CBilateralFilter::FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key)
{
    CBilateralEffectParameter * bilateralEffectParameter = (CBilateralEffectParameter *)effectParameter;
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

    if (key == libelleEffectSize)
    {
        bilateralEffectParameter->fSize = value;
    }
	else if (key == libelleEffectsigmaX)
    {
        bilateralEffectParameter->sigmaX = value;
    }
	else if (key == libelleEffectsigmaP)
    {
        bilateralEffectParameter->sigmaP = value;
    }
}