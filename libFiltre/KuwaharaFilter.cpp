#include <header.h>
#include "KuwaharaFilter.h"
#include "KuwaharaEffectParameter.h"
#include <RegardsBitmap.h>
#include <LibResource.h>
#include <FilterData.h>
using namespace Regards::Filter;

CKuwaharaFilter::CKuwaharaFilter()
{
    libelleEffectKSize = CLibResource::LoadStringFromResource(L"LBLEFFECTKERNELSIZE", 1);//L"effect.Kernel Size";
}

CKuwaharaFilter::~CKuwaharaFilter()
{
}

int CKuwaharaFilter::GetTypeFilter()
{
    return IDM_FILTER_KUWAHARA;
}

void CKuwaharaFilter::Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface)
{
    CKuwaharaEffectParameter * bestExposureEffectParameter = (CKuwaharaEffectParameter *)effectParameter;
    
	this->source = source;

    vector<float> elementColor;
    for (float i = 0.0; i < 10.0; i+=0.1)
        elementColor.push_back(i);
    
    filtreInterface->AddTreeInfos(libelleEffectKSize, new CTreeElementValueFloat(bestExposureEffectParameter->fSize,1), &elementColor,4);
}

void CKuwaharaFilter::FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key)
{
    CKuwaharaEffectParameter * kuwaharaEffectParameter = (CKuwaharaEffectParameter *)effectParameter;
    //Video Parameter
	CTreeElementValueFloat * value = (CTreeElementValueFloat *)valueData;

    if (key == libelleEffectKSize)
    {
        kuwaharaEffectParameter->fSize = value->GetValue();
    }
}



