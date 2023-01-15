#include <header.h>
//
//  CColorBalanceFilter.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#include "ColorBalanceFilter.h"
#include "RgbEffectParameter.h"

#include <LibResource.h>
#include <FilterData.h>
#include <FiltreEffet.h>
#include <ImageLoadingFormat.h>
#include <BitmapDisplay.h>
#include "TreeElementValue.h"
using namespace Regards::OpenGL;
using namespace Regards::Filter;

CColorBalanceFilter::CColorBalanceFilter()
{
	libelleEffectColorRed = CLibResource::LoadStringFromResource(L"LBLEFFECTCOLORRED", 1);
	libelleEffectColorGreen = CLibResource::LoadStringFromResource(L"LBLEFFECTCOLORGREEN", 1);
	libelleEffectColorBlue = CLibResource::LoadStringFromResource(L"LBLEFFECTCOLORBLUE", 1);
}

int CColorBalanceFilter::TypeApplyFilter()
{
	return 2;
}

CColorBalanceFilter::~CColorBalanceFilter()
{
}

wxString CColorBalanceFilter::GetFilterLabel()
{
	return CLibResource::LoadStringFromResource("LBLfilterColor", 1);
}


int CColorBalanceFilter::GetNameFilter()
{
	return IDM_COLOR_BALANCE;
}

int CColorBalanceFilter::GetTypeFilter()
{
	return COLOR_EFFECT; // 
}

void CColorBalanceFilter::Filter(CEffectParameter* effectParameter, cv::Mat& source, const wxString& filename,
                                 IFiltreEffectInterface* filtreInterface)
{
	auto rgbEffectParameter = static_cast<CRgbEffectParameter*>(effectParameter);

	this->source = source;
	this->filename = filename;
	vector<int> elementColor;
	for (auto i = -255; i < 256; i++)
		elementColor.push_back(i);

	filtreInterface->AddTreeInfos(libelleEffectColorRed, new CTreeElementValueInt(rgbEffectParameter->red),
	                              &elementColor);
	filtreInterface->AddTreeInfos(libelleEffectColorGreen, new CTreeElementValueInt(rgbEffectParameter->green),
	                              &elementColor);
	filtreInterface->AddTreeInfos(libelleEffectColorBlue, new CTreeElementValueInt(rgbEffectParameter->blue),
	                              &elementColor);
}

void CColorBalanceFilter::FilterChangeParam(CEffectParameter* effectParameter, CTreeElementValue* valueData,
                                            const wxString& key)
{
	auto rgbEffectParameter = static_cast<CRgbEffectParameter*>(effectParameter);

	auto valueInt = static_cast<CTreeElementValueInt*>(valueData);

	if (rgbEffectParameter != nullptr && valueInt != nullptr)
	{
		int value = valueInt->GetValue();
		//Video Parameter
		if (key == libelleEffectColorRed)
		{
			rgbEffectParameter->red = value;
		}
		else if (key == libelleEffectColorGreen)
		{
			rgbEffectParameter->green = value;
		}
		else if (key == libelleEffectColorBlue)
		{
			rgbEffectParameter->blue = value;
		}
	}
}

void CColorBalanceFilter::RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter,
                                       const bool& preview)
{
	if (effectParameter != nullptr && filtreEffet != nullptr)
	{
		auto rgbParameter = static_cast<CRgbEffectParameter*>(effectParameter);
		filtreEffet->RGBFilter(rgbParameter->red, rgbParameter->green, rgbParameter->blue);
	}
}

bool CColorBalanceFilter::NeedPreview()
{
	return true;
}

CEffectParameter* CColorBalanceFilter::GetEffectPointer()
{
	return new CRgbEffectParameter();
}

CEffectParameter* CColorBalanceFilter::GetDefaultEffectParameter()
{
	auto rgbFilter = new CRgbEffectParameter();
	rgbFilter->red = 120;
	rgbFilter->green = 120;
	rgbFilter->blue = 120;
	return rgbFilter;
}

bool CColorBalanceFilter::IsSourcePreview()
{
	return true;
}


void CColorBalanceFilter::ApplyPreviewEffectSource(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer,
                                                   CFiltreEffet* filtreEffet, CDraw* dessing)
{
	if (effectParameter != nullptr && !source.empty())
	{
		auto rgbParameter = static_cast<CRgbEffectParameter*>(effectParameter);
		filtreEffet->RGBFilter(rgbParameter->red, rgbParameter->green, rgbParameter->blue);
	}
}


void CColorBalanceFilter::ApplyPreviewEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer,
                                             CFiltreEffet* filtreEffet, CDraw* m_cDessin, int& widthOutput,
                                             int& heightOutput)
{
}


CImageLoadingFormat* CColorBalanceFilter::ApplyEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer)
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

			auto rgbParameter = static_cast<CRgbEffectParameter*>(effectParameter);
			filtre->RGBFilter(rgbParameter->red, rgbParameter->green, rgbParameter->blue);
			imageLoad = new CImageLoadingFormat();
			cv::Mat bitmapOut = filtre->GetBitmap(true);
			imageLoad->SetPicture(bitmapOut);
		}
	}

	return imageLoad;
}
