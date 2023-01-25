#include <header.h>
//
//  BrightAndContrastFilter.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#include "BrightAndContrastFilter.h"
#include "BrightAndContrastEffectParameter.h"
#include <LibResource.h>
#include <FilterData.h>
#include <FiltreEffet.h>
#include <BitmapDisplay.h>
#include <ImageLoadingFormat.h>
using namespace Regards::Filter;

CBrightAndContrastFilter::CBrightAndContrastFilter()
{
	libelleEffectContrast = CLibResource::LoadStringFromResource(L"LBLEFFECTCONTRAST", 1);
	libelleEffectLightness = CLibResource::LoadStringFromResource(L"LBLEFFECTBRIGHTNESS", 1);
}

CBrightAndContrastFilter::~CBrightAndContrastFilter()
{
}

int CBrightAndContrastFilter::TypeApplyFilter()
{
	return 2;
}

wxString CBrightAndContrastFilter::GetFilterLabel()
{
	return CLibResource::LoadStringFromResource("LBLfilterLight", 1);
}

int CBrightAndContrastFilter::GetNameFilter()
{
	return IDM_IMAGE_LIGHTCONTRAST;
}

int CBrightAndContrastFilter::GetTypeFilter()
{
	return COLOR_EFFECT; //return IDM_IMAGE_LIGHTCONTRAST;
}

void CBrightAndContrastFilter::Filter(CEffectParameter* effectParameter, cv::Mat& source, const wxString& filename,
                                      IFiltreEffectInterface* filtreInterface)
{
	auto BrightAndContrastEffectParameter = static_cast<CBrightAndContrastEffectParameter*>(effectParameter);
	this->filename = filename;
	this->source = source;

	vector<int> elementContrast;
	for (int i = 0; i < 200; i++)
		elementContrast.push_back(i);

	vector<int> elementColor;
	for (int i = -100; i < 101; i++)
		elementColor.push_back(i);

	filtreInterface->AddTreeInfos(libelleEffectContrast,
	                              new CTreeElementValueInt(BrightAndContrastEffectParameter->contrast),
	                              &elementContrast);
	filtreInterface->AddTreeInfos(libelleEffectLightness,
	                              new CTreeElementValueInt(BrightAndContrastEffectParameter->brightness),
	                              &elementColor);
}

void CBrightAndContrastFilter::FilterChangeParam(CEffectParameter* effectParameter, CTreeElementValue* valueData,
                                                 const wxString& key)
{
	auto BrightAndContrastEffectParameter = static_cast<CBrightAndContrastEffectParameter*>(effectParameter);
	//Video Parameter
	auto value = static_cast<CTreeElementValueInt*>(valueData);

	if (key == libelleEffectContrast)
	{
		BrightAndContrastEffectParameter->contrast = value->GetValue();
	}
	else if (key == libelleEffectLightness)
	{
		BrightAndContrastEffectParameter->brightness = value->GetValue();
	}
}

void CBrightAndContrastFilter::RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter,
                                            const bool& preview)
{
	if (effectParameter != nullptr && filtreEffet != nullptr)
	{
		auto brightAndContrast = static_cast<CBrightAndContrastEffectParameter*>(effectParameter);
		filtreEffet->BrightnessAndContrast(brightAndContrast->brightness, brightAndContrast->contrast);
	}
}

bool CBrightAndContrastFilter::NeedPreview()
{
	return true;
}

CEffectParameter* CBrightAndContrastFilter::GetEffectPointer()
{
	return new CBrightAndContrastEffectParameter();
}

CEffectParameter* CBrightAndContrastFilter::GetDefaultEffectParameter()
{
	auto brightness = new CBrightAndContrastEffectParameter();
	brightness->brightness = 20;
	brightness->contrast = 20;
	return brightness;
}


bool CBrightAndContrastFilter::IsSourcePreview()
{
	return true;
}


void CBrightAndContrastFilter::ApplyPreviewEffectSource(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer,
                                                        CFiltreEffet* filtreEffet, CDraw* dessing)
{
	if (effectParameter != nullptr && !source.empty())
	{
		auto brightAndContrast = static_cast<CBrightAndContrastEffectParameter*>(effectParameter);
		filtreEffet->BrightnessAndContrast(brightAndContrast->brightness, brightAndContrast->contrast);
	}
}


void CBrightAndContrastFilter::ApplyPreviewEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer,
                                                  CFiltreEffet* filtreEffet, CDraw* m_cDessin, int& widthOutput,
                                                  int& heightOutput)
{
}

CImageLoadingFormat* CBrightAndContrastFilter::ApplyEffect(CEffectParameter* effectParameter,
                                                           IBitmapDisplay* bitmapViewer)
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

			auto brightAndContrast = static_cast<CBrightAndContrastEffectParameter*>(effectParameter);
			filter->BrightnessAndContrast(brightAndContrast->brightness, brightAndContrast->contrast);
			imageLoad = new CImageLoadingFormat();
			cv::Mat bitmapOut = filter->GetBitmap(true);
			imageLoad->SetPicture(bitmapOut);
		}
	}

	return imageLoad;
}
