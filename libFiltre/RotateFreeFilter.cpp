#include <header.h>
#include "RotateFreeFilter.h"
#include "FreeRotateEffectParameter.h"
#include <RegardsBitmap.h>
#include <LibResource.h>
#include <FilterData.h>
#include <FiltreEffet.h>
#include <ImageLoadingFormat.h>
#include <BitmapDisplay.h>
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

wxString CRotateFreeFilter::GetFilterLabel()
{
    return CLibResource::LoadStringFromResource("LBLfilterRotate", 1);
}

int CRotateFreeFilter::GetNameFilter()
{
    return IDM_ROTATE_FREE;
}

int CRotateFreeFilter::GetTypeFilter()
{
    return ROTATE_EFFECT; //
}

bool CRotateFreeFilter::NeedToShrink()
{
    return true;
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

    freeRotateEffectParameter->updateEffect = true;
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

bool CRotateFreeFilter::IsSourcePreview()
{
    return true;
}


void CRotateFreeFilter::ApplyPreviewEffectSource(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer, CFiltreEffet* filtreEffet, CDraw* dessing)
{
    if (effectParameter != nullptr && source != nullptr)
    {
        CFreeRotateEffectParameter* rotate = (CFreeRotateEffectParameter*)effectParameter;
        filtreEffet->RotateFree(rotate->angle);
    }

}


void CRotateFreeFilter::ApplyPreviewEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer, CFiltreEffet* filtreEffet, CDraw* m_cDessin, int& widthOutput, int& heightOutput)
{

}

CImageLoadingFormat* CRotateFreeFilter::ApplyEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer)
{
    CImageLoadingFormat* imageLoad = nullptr;
    if (effectParameter != nullptr && source != nullptr && bitmapViewer != nullptr)
    {
        CFiltreEffet* filter = bitmapViewer->GetFiltreEffet();
        if (filter != nullptr)
        {
            source->RotateExif(source->GetOrientation());
            CImageLoadingFormat image(false);
            image.SetPicture(source);
            filter->SetBitmap(&image);

            imageLoad = new CImageLoadingFormat();
            CFreeRotateEffectParameter* rotate = (CFreeRotateEffectParameter*)effectParameter;
            filter->RotateFree(rotate->angle);
            CRegardsBitmap* bitmapOut = filter->GetBitmap(true);
            imageLoad->SetPicture(bitmapOut);
        }
    }

    return imageLoad;
}
