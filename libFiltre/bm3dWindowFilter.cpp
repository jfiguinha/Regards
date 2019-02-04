//
//  BrightAndContrastFilter.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#include "bm3dWindowFilter.h"
#include "bm3dEffectParameter.h"
#include <RegardsBitmap.h>
#include <LibResource.h>
#include <FilterData.h>

using namespace Regards::Viewer;

CBm3dWindowFilter::CBm3dWindowFilter()
{
    libelleEffectSigma = CLibResource::LoadStringFromResource(L"LBLEFFECTSIZE",1);//"Effect.Size";
}

CBm3dWindowFilter::~CBm3dWindowFilter()
{
    
}

int CBm3dWindowFilter::GetTypeFilter()
{
    return IDM_FILTER_BM3D;
}

void CBm3dWindowFilter::Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface)
{
    CBm3dEffectParameter * bm3dEffectParameter = (CBm3dEffectParameter *)effectParameter;
    
    vector<int> elementSigma;
    for (auto i = 1; i < 100; i++)
        elementSigma.push_back(i);
    
    filtreInterface->AddTreeInfos(libelleEffectSigma,new CTreeElementValueInt(bm3dEffectParameter->fSize), &elementSigma);
}

void CBm3dWindowFilter::FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key)
{
    CBm3dEffectParameter * bm3dEffectParameter = (CBm3dEffectParameter *)effectParameter;
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

    if (key == libelleEffectSigma)
    {
        bm3dEffectParameter->fSize = value;
    }

}