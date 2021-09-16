#include <header.h>
#include "GaussianBlurFilter.h"
#include "GaussianBlurEffectParameter.h"
#include <RegardsBitmap.h>
#include <LibResource.h>
#include <FilterData.h>
#include <FiltreEffet.h>
using namespace Regards::Filter;

CGaussianBlurFilter::CGaussianBlurFilter()
{
	libelleEffectRadius = CLibResource::LoadStringFromResource(L"LBLEFFECTRADIUS", 1);
	libelleEffectBoxSize = CLibResource::LoadStringFromResource(L"LBLEFFECTBOXSIZE", 1);
}

CGaussianBlurFilter::~CGaussianBlurFilter()
{

}

int CGaussianBlurFilter::TypeApplyFilter()
{
	return 2;
}

wxString CGaussianBlurFilter::GetFilterLabel()
{
	return CLibResource::LoadStringFromResource("LBLfilterGaussian", 1);
}


int CGaussianBlurFilter::GetNameFilter()
{
	return IDM_FILTRE_FLOUGAUSSIEN;
}

int CGaussianBlurFilter::GetTypeFilter()
{
	return CONVOLUTION_EFFECT; //
}

void CGaussianBlurFilter::Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface)
{
	CGaussianBlurEffectParameter * gaussianBlurEffectParameter = (CGaussianBlurEffectParameter *)effectParameter;

	this->source = source;

	vector<int> elementColor;
	for (auto i = 0; i < 255; i++)
	{
		if(i % 2 == 1)
			elementColor.push_back(i);
	}
		

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


void CGaussianBlurFilter::RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
{
	if (effectParameter != nullptr && filtreEffet != nullptr)
	{
		CGaussianBlurEffectParameter* gaussianBlur = (CGaussianBlurEffectParameter*)effectParameter;
		filtreEffet->GaussianBlur(gaussianBlur->radius, gaussianBlur->boxSize);
	}
}

bool CGaussianBlurFilter::NeedPreview()
{
	return true;
}

CEffectParameter* CGaussianBlurFilter::GetEffectPointer()
{
	return new CGaussianBlurEffectParameter();
}

CEffectParameter* CGaussianBlurFilter::GetDefaultEffectParameter()
{
	CGaussianBlurEffectParameter* gaussianBlur = new CGaussianBlurEffectParameter();
	gaussianBlur->radius = 5;
	gaussianBlur->boxSize = 3;
	return gaussianBlur;
}