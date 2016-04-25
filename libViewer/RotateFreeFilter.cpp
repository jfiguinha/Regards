#include "RotateFreeFilter.h"
#include <vector>
#include "FreeRotateEffectParameter.h"
#include <RegardsBitmap.h>
#include <libResource.h>
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
    
    vector<int> elementColor;
    for (int i = 0; i < 361; i++)
        elementColor.push_back(i);
    
    filtreInterface->AddTreeInfos(libelleRotationAngle, freeRotateEffectParameter->angle, elementColor);
}

void CRotateFreeFilter::FilterChangeParam(CEffectParameter * effectParameter, const int &valueData, const wxString &key)
{
    CFreeRotateEffectParameter * freeRotateEffectParameter = (CFreeRotateEffectParameter *)effectParameter;
    
    float value = valueData;
    //Video Parameter
    if (key == libelleRotationAngle)
    {
        freeRotateEffectParameter->angle = value;
    }
}
