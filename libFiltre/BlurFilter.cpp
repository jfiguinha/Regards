#include <header.h>
//
//  BrightAndContrastFilter.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//
#include <FiltreEffet.h>
#include "BlurFilter.h"
#include "BlurEffectParameter.h"
#include <LibResource.h>
#include <FilterData.h>
#include <effect_id.h>
#include <ImageLoadingFormat.h>
#include "TreeElementValue.h"
#include <BitmapDisplay.h>
using namespace Regards::Filter;

CBlurFilter::CBlurFilter()
{
	libelleEffectSize = CLibResource::LoadStringFromResource(L"LBLEFFECTRADIUSSIZE", 1); //"Effect.Radius.Size";
}

CBlurFilter::~CBlurFilter()
{
}

wxString CBlurFilter::GetFilterLabel()
{
	return CLibResource::LoadStringFromResource("LBLfilterBlur", 1);
}


int CBlurFilter::GetNameFilter()
{
	return IDM_FILTRE_FLOU;
}

int CBlurFilter::GetTypeFilter()
{
	return CONVOLUTION_EFFECT;
}

void CBlurFilter::Filter(CEffectParameter* effectParameter, cv::Mat& source, const wxString& filename,
                         IFiltreEffectInterface* filtreInterface)
{
	auto blurEffectParameter = static_cast<CBlurEffectParameter*>(effectParameter);

	this->source = source;
	this->filename = filename;
	vector<int> elementColor;
	for (int i = 1; i < 100; i++)
	{
		if (i % 2 == 1)
			elementColor.push_back(i);
	}


	filtreInterface->AddTreeInfos(libelleEffectSize, new CTreeElementValueInt(blurEffectParameter->size),
	                              &elementColor);
}

void CBlurFilter::FilterChangeParam(CEffectParameter* effectParameter, CTreeElementValue* valueData,
                                    const wxString& key)
{
	auto blurEffectParameter = static_cast<CBlurEffectParameter*>(effectParameter);
	//Video Parameter
	auto value = static_cast<CTreeElementValueInt*>(valueData);

	if (key == libelleEffectSize)
	{
		blurEffectParameter->size = value->GetValue();
	}
}

void CBlurFilter::RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
{
	auto blurEffectParameter = static_cast<CBlurEffectParameter*>(effectParameter);
	if (blurEffectParameter != nullptr && filtreEffet != nullptr)
		filtreEffet->Blur(blurEffectParameter->size);
}

bool CBlurFilter::NeedPreview()
{
	return true;
}

CEffectParameter* CBlurFilter::GetEffectPointer()
{
	return new CBlurEffectParameter();
}

int CBlurFilter::TypeApplyFilter()
{
	return 2;
}

CEffectParameter* CBlurFilter::GetDefaultEffectParameter()
{
	auto blurEffect = new CBlurEffectParameter();
	blurEffect->size = 3;
	return blurEffect;
}


bool CBlurFilter::IsSourcePreview()
{
	return true;
}


void CBlurFilter::ApplyPreviewEffectSource(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer,
                                           CFiltreEffet* filtreEffet, CDraw* dessing)
{
	if (effectParameter != nullptr && !source.empty())
	{
		auto blurEffect = static_cast<CBlurEffectParameter*>(effectParameter);
		filtreEffet->Blur(blurEffect->size);
	}
}


void CBlurFilter::ApplyPreviewEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer,
                                     CFiltreEffet* filtreEffet, CDraw* m_cDessin, int& widthOutput, int& heightOutput)
{
}

CImageLoadingFormat* CBlurFilter::ApplyEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer)
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
			auto blurEffect = static_cast<CBlurEffectParameter*>(effectParameter);
			filter->Blur(blurEffect->size);
			imageLoad = new CImageLoadingFormat();
			cv::Mat bitmapOut = filter->GetBitmap(true);
			imageLoad->SetPicture(bitmapOut);
		}
	}

	return imageLoad;
}
