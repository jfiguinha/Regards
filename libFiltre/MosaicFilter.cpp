#include <header.h>
#include "MosaicFilter.h"
#include "MosaicFilterParameter.h"
#include <FilterData.h>

#include <LibResource.h>
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

void CMosaicFilter::Filter(CEffectParameter* effectParameter, cv::Mat& source, const wxString& filename,
                           IFiltreEffectInterface* filtreInterface)
{
	auto sharpenMaskingParameter = static_cast<CMosaicEffectParameter*>(effectParameter);

	this->source = source;
	this->filename = filename;
	vector<int> elementFreq;
	for (auto i = 0; i < 100; i++)
		elementFreq.push_back(i);

	filtreInterface->AddTreeInfos(libelleSharpness, new CTreeElementValueInt(sharpenMaskingParameter->size),
	                              &elementFreq);
}

void CMosaicFilter::FilterChangeParam(CEffectParameter* effectParameter, CTreeElementValue* valueData,
                                      const wxString& key)
{
	auto mosaicParameter = static_cast<CMosaicEffectParameter*>(effectParameter);

	auto valueInt = static_cast<CTreeElementValueInt*>(valueData);
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
		auto mosaicParameter = static_cast<CMosaicEffectParameter*>(effectParameter);
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
	auto mosaicParameter = new CMosaicEffectParameter();
	mosaicParameter->size = 5;
	return mosaicParameter;
}


bool CMosaicFilter::IsSourcePreview()
{
	return true;
}


void CMosaicFilter::ApplyPreviewEffectSource(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer,
                                             CFiltreEffet* filtreEffet, CDraw* dessing)
{
	if (effectParameter != nullptr && !source.empty())
	{
		auto mosaicParameter = static_cast<CMosaicEffectParameter*>(effectParameter);
		filtreEffet->FiltreMosaic(mosaicParameter->size);
	}
}


void CMosaicFilter::ApplyPreviewEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer,
                                       CFiltreEffet* filtreEffet, CDraw* m_cDessin, int& widthOutput, int& heightOutput)
{
}

CImageLoadingFormat* CMosaicFilter::ApplyEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer)
{
	CImageLoadingFormat* imageLoad = nullptr;
	if (effectParameter != nullptr && !source.empty() && bitmapViewer != nullptr)
	{
		CFiltreEffet* filter = bitmapViewer->GetFiltreEffet();
		if (filter != nullptr)
		{
			CImageLoadingFormat image;
			image.SetPicture(source);
			image.RotateExif(orientation);
			filter->SetBitmap(&image);

			imageLoad = new CImageLoadingFormat();
			auto mosaicParameter = static_cast<CMosaicEffectParameter*>(effectParameter);
			filter->FiltreMosaic(mosaicParameter->size);
			cv::Mat bitmapOut = filter->GetBitmap(true);
			imageLoad->SetPicture(bitmapOut);
		}
	}

	return imageLoad;
}
