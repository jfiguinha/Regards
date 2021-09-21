#include <header.h>
#include "OilPaintingFilter.h"
#include "OilPaintingEffectParameter.h"
#include <RegardsBitmap.h>
#include <LibResource.h>
#include <FilterData.h>
#include <FiltreEffet.h>
#include <ImageLoadingFormat.h>
#include <BitmapDisplay.h>
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

int COilPaintingFilter::TypeApplyFilter()
{
	return 2;
}

wxString COilPaintingFilter::GetFilterLabel()
{
	return CLibResource::LoadStringFromResource("LBLfilterOilPainting", 1);
}

int COilPaintingFilter::GetNameFilter()
{
	return IDM_FILTER_OILPAINTING;
}

int COilPaintingFilter::GetTypeFilter()
{
	return SPECIAL_EFFECT; //return IDM_FILTER_OILPAINTING;
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
default: ;
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

void COilPaintingFilter::RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
{
	if (effectParameter != nullptr)
	{
		COilPaintingEffectParameter* oilPaintingParam = (COilPaintingEffectParameter*)effectParameter;
		filtreEffet->OilPaintingEffect(oilPaintingParam->size, oilPaintingParam->dynRatio);
	}
}

bool COilPaintingFilter::NeedPreview()
{
	return true;
}

CEffectParameter* COilPaintingFilter::GetEffectPointer()
{
	return new COilPaintingEffectParameter();
}

CEffectParameter* COilPaintingFilter::GetDefaultEffectParameter()
{
	COilPaintingEffectParameter* oilpainteffect = new COilPaintingEffectParameter();
	oilpainteffect->size = 10;
	oilpainteffect->dynRatio = 1;
	return oilpainteffect;
}


bool COilPaintingFilter::IsSourcePreview()
{
	return true;
}


void COilPaintingFilter::ApplyPreviewEffectSource(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer, CFiltreEffet* filtreEffet, CDraw* dessing)
{
	if (effectParameter != nullptr && source != nullptr)
	{
		COilPaintingEffectParameter* oilPaintingParam = (COilPaintingEffectParameter*)effectParameter;
		filtreEffet->OilPaintingEffect(oilPaintingParam->size, oilPaintingParam->dynRatio);
	}

}


void COilPaintingFilter::ApplyPreviewEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer, CFiltreEffet* filtreEffet, CDraw* m_cDessin, int& widthOutput, int& heightOutput)
{

}


CImageLoadingFormat* COilPaintingFilter::ApplyEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer)
{
	CImageLoadingFormat* imageLoad = nullptr;
	if (effectParameter != nullptr && source != nullptr && bitmapViewer != nullptr)
	{
		COilPaintingEffectParameter* oilPaintingParam = (COilPaintingEffectParameter*)effectParameter;
		CFiltreEffet* filter = bitmapViewer->GetFiltreEffet();
		if (filter != nullptr)
		{
			source->RotateExif(source->GetOrientation());
			CImageLoadingFormat image(false);
			image.SetPicture(source);
			filter->SetBitmap(&image);
			
			imageLoad = new CImageLoadingFormat();
			filter->OilPaintingEffect(oilPaintingParam->size, oilPaintingParam->dynRatio);
			CRegardsBitmap* bitmapOut = filter->GetBitmap(true);
			imageLoad->SetPicture(bitmapOut);
		}
	}

	return imageLoad;
}