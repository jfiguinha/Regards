#include <header.h>
//
//  BrightAndContrastFilter.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#include "BokehFilter.h"
#include "BokehEffectParameter.h"
#include <RegardsBitmap.h>
#include <LibResource.h>
#include <FilterData.h>
#include <FiltreEffet.h>
using namespace Regards::Filter;

CBokehFilter::CBokehFilter()
{
	//libelleEffectSize = CLibResource::LoadStringFromResource(L"LBLEFFECTSIZE", 1);//"Effect.Size";
	//libelleEffectsigmaX = CLibResource::LoadStringFromResource(L"LBLEFFECTSIGMAX", 1);//"Effect.Sigma.X";
	//libelleEffectsigmaP = CLibResource::LoadStringFromResource(L"LBLEFFECTSIGMAP", 1);//"Effect.Sigma.P";
	libelleEffectBlur = "Effect.Blur";
	libelleEffectBokehThreshold = "Effect.Threshold";
	libelleEffectBokehThreshold2 = "Effect.Threshold 2";
	libelleEffectBokehDilatation = "Effect.Dilatation";
	libelleEffectBokehDilatation2 = "Effect.Dilatation 2";
}

int CBokehFilter::TypeApplyFilter()
{
	return 2;
}

CBokehFilter::~CBokehFilter()
{

}

wxString CBokehFilter::GetFilterLabel()
{
	return "Bokeh Effect";//CLibResource::LoadStringFromResource("LBLBokehDENOISING", 1);
}


int CBokehFilter::GetNameFilter()
{
	return IDM_FILTRE_BOKEH;
}

int CBokehFilter::GetTypeFilter()
{
	return SPECIAL_EFFECT; //
}

void CBokehFilter::Filter(CEffectParameter* effectParameter, CRegardsBitmap* source, IFiltreEffectInterface* filtreInterface)
{
	CBokehEffectParameter* BokehEffectParameter = (CBokehEffectParameter*)effectParameter;

	this->source = source;

	vector<int> elementColor;
	for (auto i = 1; i < 100; i++)
		elementColor.push_back(i);

	vector<int> elementThreshold;
	for (auto i = 1; i < 255; i++)
		elementThreshold.push_back(i);

	filtreInterface->AddTreeInfos(libelleEffectBlur, new CTreeElementValueInt(BokehEffectParameter->blurvalue), &elementColor);
	filtreInterface->AddTreeInfos(libelleEffectBokehThreshold, new CTreeElementValueInt(BokehEffectParameter->bokehthreshold), &elementThreshold);
	filtreInterface->AddTreeInfos(libelleEffectBokehThreshold2, new CTreeElementValueInt(BokehEffectParameter->bokehthreshold2), &elementThreshold);
	filtreInterface->AddTreeInfos(libelleEffectBokehDilatation, new CTreeElementValueInt(BokehEffectParameter->dilation_size), &elementColor);
	filtreInterface->AddTreeInfos(libelleEffectBokehDilatation2, new CTreeElementValueInt(BokehEffectParameter->dilation_size2), &elementColor);

}

void CBokehFilter::FilterChangeParam(CEffectParameter* effectParameter, CTreeElementValue* valueData, const wxString& key)
{
	CBokehEffectParameter* BokehEffectParameter = (CBokehEffectParameter*)effectParameter;
	//Video Parameter
	float value = 0.0;
	switch (valueData->GetType())
	{
	case TYPE_ELEMENT_INT:
	{
		CTreeElementValueInt* intValue = (CTreeElementValueInt*)valueData;
		value = intValue->GetValue();
	}
	break;
	case TYPE_ELEMENT_FLOAT:
	{
		CTreeElementValueFloat* intValue = (CTreeElementValueFloat*)valueData;
		value = intValue->GetValue();
	}
	break;
	case TYPE_ELEMENT_BOOL:
	{
		CTreeElementValueBool* intValue = (CTreeElementValueBool*)valueData;
		value = intValue->GetValue();
	}
	break;
	}
	
	if (key == libelleEffectBlur)
	{
		BokehEffectParameter->blurvalue = value;
	}
	else if (key == libelleEffectBokehThreshold)
	{
		BokehEffectParameter->bokehthreshold = value;
	}
	else if (key == libelleEffectBokehThreshold2)
	{
		BokehEffectParameter->bokehthreshold2 = value;
	}
	else if (key == libelleEffectBokehDilatation)
	{
		BokehEffectParameter->dilation_size = value;
	}
	else if (key == libelleEffectBokehDilatation2)
	{
		BokehEffectParameter->dilation_size2 = value;
	}
}

void CBokehFilter::RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
{
	if (effectParameter != nullptr && filtreEffet != nullptr)
	{
		CBokehEffectParameter* BokehEffectParameter = (CBokehEffectParameter*)effectParameter;
		filtreEffet->BokehEffect(BokehEffectParameter->blurvalue, BokehEffectParameter->bokehthreshold, BokehEffectParameter->bokehthreshold2, BokehEffectParameter->dilation_size, BokehEffectParameter->dilation_size2);
	}
}

bool CBokehFilter::NeedPreview()
{
	return true;
}

CEffectParameter* CBokehFilter::GetEffectPointer()
{
	return new CBokehEffectParameter();
}

CEffectParameter* CBokehFilter::GetDefaultEffectParameter()
{
	return new CBokehEffectParameter();
}