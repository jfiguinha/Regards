#include <header.h>
//
//  BrightAndContrastFilter.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#include "ClaheFilter.h"
#include "ClaheEffectParameter.h"
#include <RegardsBitmap.h>
#include <LibResource.h>
#include <FilterData.h>

using namespace Regards::Viewer;

CClaheFilter::CClaheFilter()
{
    libelleEffectnBins = CLibResource::LoadStringFromResource(L"LBLEFFECTNBINS",1);//"Effect.nBins";
	libelleEffectclipLevel= CLibResource::LoadStringFromResource(L"LBLEFFECTCLIPLEVEL",1);//"Effect.Clip Level";
	libelleEffectwindowSize= CLibResource::LoadStringFromResource(L"LBLEFFECTWINDOWSIZE",1);//"Effect.Window Size";
}

CClaheFilter::~CClaheFilter()
{
    
}

int CClaheFilter::GetTypeFilter()
{
    return IDM_FILTRE_CLAHE;
}

void CClaheFilter::Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface)
{
    CClaheEffectParameter * claheEffectParameter = (CClaheEffectParameter *)effectParameter;
    
    vector<int> elementColor;
    for (auto i = 1; i < 300; i++)
        elementColor.push_back(i);

	vector<int> elementSigma;
	for (auto i = 0; i < 100; i++)
		elementSigma.push_back(i);
    
    filtreInterface->AddTreeInfos(libelleEffectnBins,new CTreeElementValueInt(claheEffectParameter->nBins), &elementColor);
	filtreInterface->AddTreeInfos(libelleEffectclipLevel,new CTreeElementValueInt(claheEffectParameter->clipLevel), &elementSigma);
	filtreInterface->AddTreeInfos(libelleEffectwindowSize,new CTreeElementValueInt(claheEffectParameter->windowSize), &elementColor);
}

void CClaheFilter::FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key)
{
    CClaheEffectParameter * claheEffectParameter = (CClaheEffectParameter *)effectParameter;
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

    if (key == libelleEffectnBins)
    {
        claheEffectParameter->nBins = value;
    }
	else if (key == libelleEffectclipLevel)
    {
        claheEffectParameter->clipLevel = value;
    }
	else if (key == libelleEffectwindowSize)
    {
        claheEffectParameter->windowSize = value;
    }
}