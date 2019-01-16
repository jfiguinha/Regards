#include "SharpenMaskingFilter.h"
#include "SharpenMaskingParameter.h"
#include <FilterData.h>
#include <LibResource.h>
using namespace Regards::Viewer;

CSharpenMaskingFilter::CSharpenMaskingFilter()
{
	libelleSharpness = CLibResource::LoadStringFromResource(L"LBLSHARPNESS", 1);
}


CSharpenMaskingFilter::~CSharpenMaskingFilter()
{
}

int CSharpenMaskingFilter::GetTypeFilter()
{
	return IDM_SHARPENMASKING;
}

void CSharpenMaskingFilter::Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface)
{
	CSharpenMaskingEffectParameter * sharpenMaskingParameter = (CSharpenMaskingEffectParameter *)effectParameter;

	vector<int> elementFreq;
	for (auto i = 0; i < 100; i++)
		elementFreq.push_back(i);

	filtreInterface->AddTreeInfos(libelleSharpness, new CTreeElementValueInt(sharpenMaskingParameter->sharpness), &elementFreq);
}

void CSharpenMaskingFilter::FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key)
{
	CSharpenMaskingEffectParameter * sharpenMaskingParameter = (CSharpenMaskingEffectParameter *)effectParameter;

	CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
    int value = valueInt->GetValue();

	//Video Parameter
	if (key == libelleSharpness)
	{
		sharpenMaskingParameter->sharpness = (float)value / 10.0f;
	}
}
