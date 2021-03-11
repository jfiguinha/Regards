#include <header.h>
#include "OilPaintingFilter.h"
#include "OilPaintingEffectParameter.h"
#include <RegardsBitmap.h>
#include <LibResource.h>
#include <FilterData.h>

using namespace Regards::Filter;

COilPaintingFilter::COilPaintingFilter()
{
	libelleSize = CLibResource::LoadStringFromResource(L"LBLEFFECTSIZE", 1);//"Effect.FSize";
	libelleDynRatio = CLibResource::LoadStringFromResource(L"LBLDYNAMICRATIO", 1);//"Effect.BSize";

//    libelleSize = L"effect.Size";
//	libelleDynRatio = L"effect.Dynamic Ratio";
}

COilPaintingFilter::~COilPaintingFilter()
{
}


int COilPaintingFilter::GetTypeFilter()
{
    return IDM_FILTER_OILPAINTING;
}

void COilPaintingFilter::Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface)
{
	COilPaintingEffectParameter * oilPaintingEffectParameter = (COilPaintingEffectParameter *)effectParameter;
    
	this->source = source;

    vector<int> elementColor;
    for (int i = 0; i < 50; i++)
        elementColor.push_back(i);

	vector<int> elementRatio;
	for (int i = 0; i < 10; i++)
		elementRatio.push_back(i);
    
    filtreInterface->AddTreeInfos(libelleSize, new CTreeElementValueInt(oilPaintingEffectParameter->size), &elementColor);
	filtreInterface->AddTreeInfos(libelleDynRatio, new CTreeElementValueInt(oilPaintingEffectParameter->dynRatio), &elementRatio);
}

void COilPaintingFilter::FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key)
{
	COilPaintingEffectParameter * oilPaintingEffectParameter = (COilPaintingEffectParameter *)effectParameter;
    //Video Parameter
	//Video Parameter
	float value = 0.0;
	switch (valueData->GetType())
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

    if (key == libelleSize)
    {
		oilPaintingEffectParameter->size = value;
    }
	else if (key == libelleDynRatio)
	{
		oilPaintingEffectParameter->dynRatio = value;
	}
}

