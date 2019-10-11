#include <header.h>
#include "BestExposureFilter.h"
#include "BestExposureEffectParameter.h"
#include <RegardsBitmap.h>
#include <LibResource.h>
#include <FilterData.h>

using namespace Regards::Viewer;

CBestExposureFilter::CBestExposureFilter()
{
    libelleEffectTmo = CLibResource::LoadStringFromResource(L"LBLEFFECTTMOVALUE",1);//L"effect.Tmo Value";
}

CBestExposureFilter::~CBestExposureFilter()
{
}


int CBestExposureFilter::GetTypeFilter()
{
    return IDM_BEST_EXPOSURE;
}

void CBestExposureFilter::Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface)
{
    CBestExposureEffectParameter * bestExposureEffectParameter = (CBestExposureEffectParameter *)effectParameter;
    
	this->source = source;

    vector<float> elementColor;
    for (float i = 0.0; i < 10.0; i+=0.1)
        elementColor.push_back(i);
    
    filtreInterface->AddTreeInfos(libelleEffectTmo, new CTreeElementValueFloat(bestExposureEffectParameter->tmo,1), &elementColor,4);
}

void CBestExposureFilter::FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key)
{
    CBestExposureEffectParameter * bestExposureEffectParameter = (CBestExposureEffectParameter *)effectParameter;
    //Video Parameter
	CTreeElementValueFloat * value = (CTreeElementValueFloat *)valueData;

    if (key == libelleEffectTmo)
    {
        bestExposureEffectParameter->tmo = value->GetValue();
    }
}

