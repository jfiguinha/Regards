#include <header.h>
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
#include <FiltreEffet.h>
using namespace Regards::Filter;

CBm3dWindowFilter::CBm3dWindowFilter()
{
    libelleEffectSigma = CLibResource::LoadStringFromResource(L"LBLEFFECTSIZE",1);//"Effect.Size";
}

CBm3dWindowFilter::~CBm3dWindowFilter()
{
    
}

int CBm3dWindowFilter::TypeApplyFilter()
{
	return 2;
}

int CBm3dWindowFilter::GetTypeFilter()
{
	return CONVOLUTION_EFFECT;// return IDM_FILTER_BM3D;
}

void CBm3dWindowFilter::Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface)
{
    CBm3dEffectParameter * bm3dEffectParameter = (CBm3dEffectParameter *)effectParameter;
    
	this->source = source;
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
		case TYPE_ELEMENT_INT:
			{
				CTreeElementValueInt * intValue = (CTreeElementValueInt*)valueData;
				value = intValue->GetValue();
			}
			break;
		case TYPE_ELEMENT_FLOAT:
			{
				CTreeElementValueFloat * intValue = (CTreeElementValueFloat*)valueData;
				value = intValue->GetValue();
			}
			break;
		case TYPE_ELEMENT_BOOL:
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

void CBm3dWindowFilter::RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
{
	if (effectParameter != nullptr && filtreEffet != nullptr)
	{
		CBm3dEffectParameter* bm3dParameter = (CBm3dEffectParameter*)effectParameter;
		filtreEffet->Bm3d(bm3dParameter->fSize);
	}
}

bool CBm3dWindowFilter::NeedPreview()
{
	return true;
}

CEffectParameter* CBm3dWindowFilter::GetEffectPointer()
{
	return new CBm3dEffectParameter();
}

CEffectParameter* CBm3dWindowFilter::GetDefaultEffectParameter()
{
	CBm3dEffectParameter* bm3dParameter = new CBm3dEffectParameter();
	bm3dParameter->fSize = 25;
	return bm3dParameter;
}
