#include <header.h>
#include "RotateFreeFilter.h"
#include "FreeRotateEffectParameter.h"
#include <RegardsBitmap.h>
#include <LibResource.h>
#include <FilterData.h>

using namespace Regards::Viewer;

CRotateFreeFilter::CRotateFreeFilter()
{
    libelleRotationAngle = CLibResource::LoadStringFromResource(L"LBLROTATIONANGLE",1);
}

CRotateFreeFilter::~CRotateFreeFilter()
{
    
}

int CRotateFreeFilter::GetTypeFilter()
{
    return IDM_FILTRE_SWIRL;
}

void CRotateFreeFilter::Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface)
{   
    CFreeRotateEffectParameter * freeRotateEffectParameter = (CFreeRotateEffectParameter *)effectParameter;
    
	this->source = source;

    vector<int> elementColor;
    for (auto i = 0; i < 361; i++)
        elementColor.push_back(i);
    
    filtreInterface->AddTreeInfos(libelleRotationAngle, new CTreeElementValueInt(freeRotateEffectParameter->angle), &elementColor);
}

void CRotateFreeFilter::FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key)
{
    CFreeRotateEffectParameter * freeRotateEffectParameter = (CFreeRotateEffectParameter *)effectParameter;
    
	CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
    int value = valueInt->GetValue();
    //Video Parameter
    if (key == libelleRotationAngle)
    {
        freeRotateEffectParameter->angle = value;
    }
}
