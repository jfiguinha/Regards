#include <header.h>
//
//  Chqdn3dFilter.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#include "hqdn3dFilter.h"
#include "hqdn3dEffectParameter.h"

#include <LibResource.h>
#include <FilterData.h>
#include <ImageLoadingFormat.h>
#include <FiltreEffet.h>
#include <Draw.h>
#include <BitmapDisplay.h>

#include "TreeElementValue.h"
using namespace Regards::Filter;

Chqdn3dFilter::Chqdn3dFilter()
{
	/*
	libelleLumSpac = CLibResource::LoadStringFromResource(L"LBLEFFECTLUMSPAC",1);
    libelleChromSpac = CLibResource::LoadStringFromResource(L"LBLEFFECTCHROMSPAC",1);
    libelleLumTmp = CLibResource::LoadStringFromResource(L"LBLEFFECTLUMTMP",1);
    libelleChromTmp = CLibResource::LoadStringFromResource(L"LBLEFFECTCHROMTMP",1);
	*/
	libelleLumSpac = CLibResource::LoadStringFromResource(L"LBLEFFECTSIZE", 1);
}

Chqdn3dFilter::~Chqdn3dFilter()
{
}

int Chqdn3dFilter::TypeApplyFilter()
{
	return 2;
}

wxString Chqdn3dFilter::GetFilterLabel()
{
	return CLibResource::LoadStringFromResource("LBLFILTREHQDN3D", 1);
}


int Chqdn3dFilter::GetNameFilter()
{
	return IDM_FILTREHQDN3D;
}

int Chqdn3dFilter::GetTypeFilter()
{
	return CONVOLUTION_EFFECT; //
}

void Chqdn3dFilter::Filter(CEffectParameter* effectParameter, cv::Mat& source, const wxString& filename,
                           IFiltreEffectInterface* filtreInterface)
{
	auto hqdn3dParameter = static_cast<Chqdn3dEffectParameter*>(effectParameter);
	this->filename = filename;
	this->source = source;

	vector<int> elementIntensity;
	for (auto i = 0; i < 26; i++)
	{
		if (i % 2 == 1)
			elementIntensity.push_back(i);
	}

	filtreInterface->AddTreeInfos(libelleLumSpac, new CTreeElementValueInt(hqdn3dParameter->LumSpac),
	                              &elementIntensity);
}

void Chqdn3dFilter::FilterChangeParam(CEffectParameter* effectParameter, CTreeElementValue* valueData,
                                      const wxString& key)
{
	auto hqdn3dParameter = static_cast<Chqdn3dEffectParameter*>(effectParameter);

	auto valueInt = static_cast<CTreeElementValueInt*>(valueData);
	int value = valueInt->GetValue();
	//Video Parameter
	if (key == libelleLumSpac)
	{
		hqdn3dParameter->LumSpac = value;
	}
	else if (key == libelleChromSpac)
	{
		hqdn3dParameter->ChromSpac = value;
	}
	else if (key == libelleLumTmp)
	{
		hqdn3dParameter->LumTmp = value;
	}
	else if (key == libelleChromTmp)
	{
		hqdn3dParameter->ChromTmp = value;
	}
}


void Chqdn3dFilter::ApplyPreviewEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer,
                                       CFiltreEffet* filtreEffet, CDraw* m_cDessin, int& widthOutput, int& heightOutput)
{
}

CImageLoadingFormat* Chqdn3dFilter::ApplyEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer)
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

			auto hqdn3dParameter = static_cast<Chqdn3dEffectParameter*>(effectParameter);
			filter->HQDn3D(hqdn3dParameter->LumSpac, hqdn3dParameter->ChromSpac, hqdn3dParameter->LumTmp,
			               hqdn3dParameter->ChromTmp);
			imageLoad = new CImageLoadingFormat();
			cv::Mat bitmapOut = filter->GetBitmap(true);
			imageLoad->SetPicture(bitmapOut);
		}
	}

	return imageLoad;
}

void Chqdn3dFilter::RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
{
	if (effectParameter != nullptr && filtreEffet != nullptr)
	{
		auto hq3dn = static_cast<Chqdn3dEffectParameter*>(effectParameter);
		filtreEffet->HQDn3D(hq3dn->LumSpac, hq3dn->ChromSpac, hq3dn->LumTmp, hq3dn->ChromTmp);
	}
}

bool Chqdn3dFilter::NeedPreview()
{
	return true;
}

CEffectParameter* Chqdn3dFilter::GetEffectPointer()
{
	return new Chqdn3dEffectParameter();
}

CEffectParameter* Chqdn3dFilter::GetDefaultEffectParameter()
{
	auto hq3deffect = new Chqdn3dEffectParameter();
	hq3deffect->LumSpac = 4;
	hq3deffect->ChromSpac = 4;
	hq3deffect->LumTmp = 3;
	hq3deffect->ChromTmp = 3;
	return hq3deffect;
}


bool Chqdn3dFilter::IsSourcePreview()
{
	return true;
}


void Chqdn3dFilter::ApplyPreviewEffectSource(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer,
                                             CFiltreEffet* filtreEffet, CDraw* dessing)
{
	if (effectParameter != nullptr && !source.empty())
	{
		auto hq3dn = static_cast<Chqdn3dEffectParameter*>(effectParameter);
		filtreEffet->HQDn3D(hq3dn->LumSpac, hq3dn->ChromSpac, hq3dn->LumTmp, hq3dn->ChromTmp);
	}
}
