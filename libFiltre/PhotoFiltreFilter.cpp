#include <header.h>
//
//  PhotoFiltreFilter.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#include "PhotoFiltreFilter.h"
#include "PhotoFiltreEffectParameter.h"

#include <LibResource.h>
#include <FilterData.h>
#include <FiltreEffet.h>
#include <BitmapDisplay.h>
#include <ImageLoadingFormat.h>
using namespace Regards::Filter;

CPhotoFiltreFilter::CPhotoFiltreFilter()
{
	libelleEffectIntensity = CLibResource::LoadStringFromResource(L"LBLEFFECTINTENSITY", 1);
	libelleEffectColorRed = CLibResource::LoadStringFromResource(L"LBLEFFECTCOLORRED", 1);
	libelleEffectColorGreen = CLibResource::LoadStringFromResource(L"LBLEFFECTCOLORGREEN", 1);
	libelleEffectColorBlue = CLibResource::LoadStringFromResource(L"LBLEFFECTCOLORBLUE", 1);
}

CPhotoFiltreFilter::~CPhotoFiltreFilter()
{
}

int CPhotoFiltreFilter::TypeApplyFilter()
{
	return 2;
}

wxString CPhotoFiltreFilter::GetFilterLabel()
{
	return CLibResource::LoadStringFromResource("LBLfilterPhoto", 1);
}

int CPhotoFiltreFilter::GetNameFilter()
{
	return ID_AJUSTEMENT_PHOTOFILTRE;
}

int CPhotoFiltreFilter::GetTypeFilter()
{
	return COLOR_EFFECT; //
}

void CPhotoFiltreFilter::Filter(CEffectParameter* effectParameter, cv::Mat& source, const wxString& filename,
                                IFiltreEffectInterface* filtreInterface)
{
	auto photoEffectParameter = static_cast<CPhotoFiltreEffectParameter*>(effectParameter);

	this->source = source;
	this->filename = filename;
	vector<int> elementColor;
	for (auto i = 0; i < 256; i++)
		elementColor.push_back(i);

	vector<int> intensity;
	for (auto i = 0; i < 101; i++)
		intensity.push_back(i);

	filtreInterface->AddTreeInfos(libelleEffectIntensity, new CTreeElementValueInt(photoEffectParameter->intensity),
	                              &intensity);
	filtreInterface->AddTreeInfos(libelleEffectColorRed, new CTreeElementValueInt(photoEffectParameter->red),
	                              &elementColor);
	filtreInterface->AddTreeInfos(libelleEffectColorGreen, new CTreeElementValueInt(photoEffectParameter->green),
	                              &elementColor);
	filtreInterface->AddTreeInfos(libelleEffectColorBlue, new CTreeElementValueInt(photoEffectParameter->blue),
	                              &elementColor);
}

void CPhotoFiltreFilter::FilterChangeParam(CEffectParameter* effectParameter, CTreeElementValue* valueData,
                                           const wxString& key)
{
	auto photoEffectParameter = static_cast<CPhotoFiltreEffectParameter*>(effectParameter);

	auto valueInt = static_cast<CTreeElementValueInt*>(valueData);
	int value = valueInt->GetValue();
	//Video Parameter
	if (key == libelleEffectColorRed)
	{
		photoEffectParameter->red = value;
	}
	else if (key == libelleEffectColorGreen)
	{
		photoEffectParameter->green = value;
	}
	else if (key == libelleEffectColorBlue)
	{
		photoEffectParameter->blue = value;
	}
	else if (key == libelleEffectIntensity)
	{
		photoEffectParameter->intensity = value;
	}
}

void CPhotoFiltreFilter::RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
{
	if (effectParameter != nullptr && filtreEffet != nullptr)
	{
		auto photoFiltreParameter = static_cast<CPhotoFiltreEffectParameter*>(effectParameter);
		filtreEffet->PhotoFiltre(
			CRgbaquad(photoFiltreParameter->red, photoFiltreParameter->green, photoFiltreParameter->blue),
			photoFiltreParameter->intensity);
	}
}

bool CPhotoFiltreFilter::NeedPreview()
{
	return true;
}

CEffectParameter* CPhotoFiltreFilter::GetEffectPointer()
{
	return new CPhotoFiltreEffectParameter();
}

CEffectParameter* CPhotoFiltreFilter::GetDefaultEffectParameter()
{
	auto photoFiltre = new CPhotoFiltreEffectParameter();
	photoFiltre->red = 255;
	photoFiltre->green = 0;
	photoFiltre->blue = 0;
	photoFiltre->intensity = 30;
	return photoFiltre;
}


bool CPhotoFiltreFilter::IsSourcePreview()
{
	return true;
}


void CPhotoFiltreFilter::ApplyPreviewEffectSource(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer,
                                                  CFiltreEffet* filtreEffet, CDraw* dessing)
{
	if (effectParameter != nullptr && !source.empty())
	{
		auto photoFiltreParameter = static_cast<CPhotoFiltreEffectParameter*>(effectParameter);
		filtreEffet->PhotoFiltre(
			CRgbaquad(photoFiltreParameter->red, photoFiltreParameter->green, photoFiltreParameter->blue),
			photoFiltreParameter->intensity);
	}
}


void CPhotoFiltreFilter::ApplyPreviewEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer,
                                            CFiltreEffet* filtreEffet, CDraw* m_cDessin, int& widthOutput,
                                            int& heightOutput)
{
}

CImageLoadingFormat* CPhotoFiltreFilter::ApplyEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer)
{
	CImageLoadingFormat* imageLoad = nullptr;

	if (effectParameter != nullptr && bitmapViewer != nullptr)
	{
		CFiltreEffet* filtre = bitmapViewer->GetFiltreEffet();

		if (!source.empty() && filtre != nullptr)
		{
			CImageLoadingFormat image;
			image.SetPicture(source);
			image.RotateExif(orientation);
			filtre->SetBitmap(&image);

			imageLoad = new CImageLoadingFormat();
			auto photoFiltreParameter = static_cast<CPhotoFiltreEffectParameter*>(effectParameter);
			filtre->PhotoFiltre(
				CRgbaquad(photoFiltreParameter->red, photoFiltreParameter->green, photoFiltreParameter->blue),
				photoFiltreParameter->intensity);
			cv::Mat bitmapOut = filtre->GetBitmap(true);
			imageLoad->SetPicture(bitmapOut);
		}
	}


	return imageLoad;
}
