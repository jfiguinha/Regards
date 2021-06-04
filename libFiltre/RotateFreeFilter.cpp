#include <header.h>
#include "RotateFreeFilter.h"
#include "FreeRotateEffectParameter.h"
#include <RegardsBitmap.h>
#include <LibResource.h>
#include <FilterData.h>
#include <FiltreEffet.h>
using namespace Regards::Filter;

CRotateFreeFilter::CRotateFreeFilter()
{
    libelleRotationAngle = CLibResource::LoadStringFromResource(L"LBLROTATIONANGLE",1);
}

CRotateFreeFilter::~CRotateFreeFilter()
{
    
}

int CRotateFreeFilter::TypeApplyFilter()
{
    return 2;
}

int CRotateFreeFilter::GetTypeFilter()
{
    return ROTATE_EFFECT; //return IDM_ROTATE_FREE;
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

void CRotateFreeFilter::RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
{
    if (effectParameter != nullptr && filtreEffet != nullptr)
    {
        CFreeRotateEffectParameter* freeRotate = (CFreeRotateEffectParameter*)effectParameter;
        filtreEffet->RotateFree(freeRotate->angle);
    }
}

bool CRotateFreeFilter::NeedPreview()
{
    return true;
}

CEffectParameter* CRotateFreeFilter::GetEffectPointer()
{
    return new CFreeRotateEffectParameter();
}

CEffectParameter* CRotateFreeFilter::GetDefaultEffectParameter()
{
    CFreeRotateEffectParameter* rotate = new CFreeRotateEffectParameter();
    rotate->angle = 50;
    return rotate;
}