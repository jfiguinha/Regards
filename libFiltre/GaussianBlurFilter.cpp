#include <header.h>
#include "GaussianBlurFilter.h"
#include "GaussianBlurEffectParameter.h"
#include <RegardsBitmap.h>
#include <LibResource.h>
#include <FilterData.h>

using namespace Regards::Filter;

CGaussianBlurFilter::CGaussianBlurFilter()
{
	libelleEffectRadius = CLibResource::LoadStringFromResource(L"LBLEFFECTRADIUS", 1);
	libelleEffectBoxSize = CLibResource::LoadStringFromResource(L"LBLEFFECTBOXSIZE", 1);
}

CGaussianBlurFilter::~CGaussianBlurFilter()
{

}

int CGaussianBlurFilter::GetTypeFilter()
{
	return IDM_FILTRE_FLOUGAUSSIEN;
}

void CGaussianBlurFilter::Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface)
{
	CGaussianBlurEffectParameter * gaussianBlurEffectParameter = (CGaussianBlurEffectParameter *)effectParameter;

	this->source = source;

	vector<int> elementColor;
	for (auto i = 0; i < 255; i++)
		elementColor.push_back(i);

	filtreInterface->AddTreeInfos(libelleEffectRadius, new CTreeElementValueInt(gaussianBlurEffectParameter->radius), &elementColor);
	filtreInterface->AddTreeInfos(libelleEffectBoxSize, new CTreeElementValueInt(gaussianBlurEffectParameter->boxSize), &elementColor);
}

void CGaussianBlurFilter::FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key)
{
	CGaussianBlurEffectParameter * gaussianBlurEffectParameter = (CGaussianBlurEffectParameter *)effectParameter;

	CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
    int value = valueInt->GetValue();
	//Video Parameter
	if (key == libelleEffectRadius)
	{
		gaussianBlurEffectParameter->radius = value;
	}
	else if (key == libelleEffectBoxSize)
	{
		gaussianBlurEffectParameter->boxSize = value;
	}

}