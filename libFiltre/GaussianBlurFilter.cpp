#include <header.h>
#include "GaussianBlurFilter.h"
#include "GaussianBlurEffectParameter.h"

#include <LibResource.h>
#include <FilterData.h>
#include <FiltreEffet.h>
#include <BitmapDisplay.h>
#include <ImageLoadingFormat.h>
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

void CGaussianBlurFilter::Filter(CEffectParameter* effectParameter, cv::Mat& source, const wxString& filename,
                                 IFiltreEffectInterface* filtreInterface)
{
	auto gaussianBlurEffectParameter = static_cast<CGaussianBlurEffectParameter*>(effectParameter);
	this->filename = filename;
	this->source = source;

	vector<int> elementColor;
	for (auto i = 0; i < 26; i++)
	{
		if (i % 2 == 1)
			elementColor.push_back(i);
	}


	filtreInterface->AddTreeInfos(libelleEffectRadius, new CTreeElementValueInt(gaussianBlurEffectParameter->radius),
	                              &elementColor);
	filtreInterface->AddTreeInfos(libelleEffectBoxSize, new CTreeElementValueInt(gaussianBlurEffectParameter->boxSize),
	                              &elementColor);
}

void CGaussianBlurFilter::FilterChangeParam(CEffectParameter* effectParameter, CTreeElementValue* valueData,
                                            const wxString& key)
{
	auto gaussianBlurEffectParameter = static_cast<CGaussianBlurEffectParameter*>(effectParameter);

	auto valueInt = static_cast<CTreeElementValueInt*>(valueData);
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


void CGaussianBlurFilter::RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter,
                                       const bool& preview)
{
	if (effectParameter != nullptr && filtreEffet != nullptr)
	{
		auto gaussianBlur = static_cast<CGaussianBlurEffectParameter*>(effectParameter);
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
	auto gaussianBlur = new CGaussianBlurEffectParameter();
	gaussianBlur->radius = 5;
	gaussianBlur->boxSize = 3;
	return gaussianBlur;
}

bool CGaussianBlurFilter::IsSourcePreview()
{
	return true;
}


void CGaussianBlurFilter::ApplyPreviewEffectSource(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer,
                                                   CFiltreEffet* filtreEffet, CDraw* dessing)
{
	if (effectParameter != nullptr && !source.empty())
	{
		auto gaussianBlur = static_cast<CGaussianBlurEffectParameter*>(effectParameter);
		filtreEffet->GaussianBlur(gaussianBlur->radius, gaussianBlur->boxSize);
	}
}


void CGaussianBlurFilter::ApplyPreviewEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer,
                                             CFiltreEffet* filtreEffet, CDraw* m_cDessin, int& widthOutput,
                                             int& heightOutput)
{
}

CImageLoadingFormat* CGaussianBlurFilter::ApplyEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer)
{
	CImageLoadingFormat* imageLoad = nullptr;
	if (effectParameter != nullptr && !source.empty() && bitmapViewer != nullptr)
	{
		CFiltreEffet* filtre = bitmapViewer->GetFiltreEffet();
		if (filtre != nullptr)
		{
			CImageLoadingFormat image;
			image.SetPicture(source);
			image.RotateExif(orientation);
			filtre->SetBitmap(&image);

			auto gaussianBlur = static_cast<CGaussianBlurEffectParameter*>(effectParameter);
			filtre->GaussianBlur(gaussianBlur->radius, gaussianBlur->boxSize);
			imageLoad = new CImageLoadingFormat();
			cv::Mat bitmapOut = filtre->GetBitmap(true);
			imageLoad->SetPicture(bitmapOut);
		}
	}

	return imageLoad;
}
