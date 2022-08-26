#include <header.h>
#include "MosaicFilter.h"
#include "MosaicFilterParameter.h"
#include <FilterData.h>
#include <RegardsBitmap.h>
#include <LibResource.h>
#include <RenderOpenGL.h>
#include <FiltreEffet.h>
#include <ImageLoadingFormat.h>
#include <BitmapDisplay.h>

using namespace Regards::Filter;

CMosaicFilter::CMosaicFilter()
{
	libelleSharpness = CLibResource::LoadStringFromResource(L"LBLEFFECTSIZE", 1);
}

int CMosaicFilter::TypeApplyFilter()
{
	return 2;
}

CMosaicFilter::~CMosaicFilter()
{
}

wxString CMosaicFilter::GetFilterLabel()
{
	return CLibResource::LoadStringFromResource("LBLfilterMosaic", 1);
}

int CMosaicFilter::GetNameFilter()
{
	return IDM_FILTRE_MOSAIQUE;
}

int CMosaicFilter::GetTypeFilter()
{
	return SPECIAL_EFFECT; //return IDM_SHARPENMASKING;
}

void CMosaicFilter::Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface)
{
	CMosaicEffectParameter* sharpenMaskingParameter = (CMosaicEffectParameter*)effectParameter;

	this->source = source;

	vector<int> elementFreq;
	for (auto i = 0; i < 100; i++)
		elementFreq.push_back(i);

	filtreInterface->AddTreeInfos(libelleSharpness, new CTreeElementValueInt(sharpenMaskingParameter->size), &elementFreq);
}

void CMosaicFilter::FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key)
{
	CMosaicEffectParameter* mosaicParameter = (CMosaicEffectParameter*)effectParameter;

	CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
    int value = valueInt->GetValue();

	//Video Parameter
	if (key == libelleSharpness)
	{
		mosaicParameter->size = value;
	}
}


void CMosaicFilter::RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
{
	if (effectParameter != nullptr && filtreEffet != nullptr)
	{
		CMosaicEffectParameter* mosaicParameter = (CMosaicEffectParameter*)effectParameter;
		filtreEffet->SharpenMasking(mosaicParameter->size);
	}
}

bool CMosaicFilter::NeedPreview()
{
	return true;
}

CEffectParameter* CMosaicFilter::GetEffectPointer()
{
	return new CMosaicEffectParameter();
}

CEffectParameter* CMosaicFilter::GetDefaultEffectParameter()
{
	CMosaicEffectParameter* mosaicParameter = new CMosaicEffectParameter();
	mosaicParameter->size = 5;
	return mosaicParameter;
}


bool CMosaicFilter::IsSourcePreview()
{
	return true;
}


void CMosaicFilter::ApplyPreviewEffectSource(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer, CFiltreEffet* filtreEffet, CDraw* dessing)
{
	if (effectParameter != nullptr && source != nullptr)
	{
		CMosaicEffectParameter* mosaicParameter = (CMosaicEffectParameter*)effectParameter;
		filtreEffet->FiltreMosaic(mosaicParameter->size);
	}

}


void CMosaicFilter::ApplyPreviewEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer, CFiltreEffet* filtreEffet, CDraw* m_cDessin, int& widthOutput, int& heightOutput)
{

}

CImageLoadingFormat* CMosaicFilter::ApplyEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer)
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
			CMosaicEffectParameter* mosaicParameter = (CMosaicEffectParameter*)effectParameter;
			filter->FiltreMosaic(mosaicParameter->size);
			CRegardsBitmap* bitmapOut = filter->GetBitmap(true);
			imageLoad->SetPicture(bitmapOut);
		}
	}

	return imageLoad;
}
