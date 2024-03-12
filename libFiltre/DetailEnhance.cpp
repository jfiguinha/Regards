#include <header.h>
//
//  BrightAndContrastFilter.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#include "DetailEnhance.h"
#include "DetailEnhanceParameter.h"
#include <LibResource.h>
#include <BitmapDisplay.h>
#include <ImageLoadingFormat.h>
#include <FiltreEffet.h>
using namespace Regards::Filter;

CDetailEnhance::CDetailEnhance()
{
	libelleEffectsigmaX = CLibResource::LoadStringFromResource(L"LBLEFFECTSIGMAX", 1); //"Effect.Sigma.X";
	libelleEffectsigmaP = CLibResource::LoadStringFromResource(L"LBLEFFECTSIGMAP", 1); //"Effect.Sigma.P";
}

int CDetailEnhance::TypeApplyFilter()
{
	return 2;
}

CDetailEnhance::~CDetailEnhance()
{
}

wxString CDetailEnhance::GetFilterLabel()
{
	return "Detail Enhance";// CLibResource::LoadStringFromResource("LBLBILATERALDENOISING", 1);
}


int CDetailEnhance::GetNameFilter()
{
	return IDM_FILTRE_DETAILENHANCE;
}

int CDetailEnhance::GetTypeFilter()
{
	return SPECIAL_EFFECT; //
}

void CDetailEnhance::Filter(CEffectParameter* effectParameter, cv::Mat& source, const wxString& filename,
                              IFiltreEffectInterface* filtreInterface)
{
	auto detailEnhanceParameter = static_cast<CDetailEnhanceParameter*>(effectParameter);
	this->filename = filename;
	this->source = source;

	vector<int> elementSigmaS;
	for (auto i = 0; i < 200; i++)
	{
		elementSigmaS.push_back(i);
	}
	
	vector<int> elementSigmaR;
	for (auto i = 0; i < 100; i++)
	{
		elementSigmaR.push_back(i);
	}

	filtreInterface->AddTreeInfos(libelleEffectsigmaX, new CTreeElementValueInt(detailEnhanceParameter->sigmaX),
	                              &elementSigmaS);
	filtreInterface->AddTreeInfos(libelleEffectsigmaP, new CTreeElementValueInt(detailEnhanceParameter->sigmaP),
	                              &elementSigmaR);
}

void CDetailEnhance::FilterChangeParam(CEffectParameter* effectParameter, CTreeElementValue* valueData,
                                         const wxString& key)
{
	auto detailEnhanceParameter = static_cast<CDetailEnhanceParameter*>(effectParameter);
	//Video Parameter
	float value = 0.0;
	switch (valueData->GetType())
	{
	case TYPE_ELEMENT_INT:
		{
			auto intValue = static_cast<CTreeElementValueInt*>(valueData);
			value = intValue->GetValue();
		}
		break;
	case TYPE_ELEMENT_FLOAT:
		{
			auto intValue = static_cast<CTreeElementValueFloat*>(valueData);
			value = intValue->GetValue();
		}
		break;
	case TYPE_ELEMENT_BOOL:
		{
			auto intValue = static_cast<CTreeElementValueBool*>(valueData);
			value = intValue->GetValue();
		}
		break;
	}

	if (key == libelleEffectsigmaX)
	{
		detailEnhanceParameter->sigmaX = value;
	}
	else if (key == libelleEffectsigmaP)
	{
		detailEnhanceParameter->sigmaP = value;
	}
}

void CDetailEnhance::RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
{
	if (effectParameter != nullptr && filtreEffet != nullptr)
	{
		auto detailEnhanceParameter = static_cast<CDetailEnhanceParameter*>(effectParameter);
		filtreEffet->DetailEnhance(detailEnhanceParameter->sigmaX, detailEnhanceParameter->sigmaP);
	}
}

bool CDetailEnhance::NeedPreview()
{
	return true;
}

CEffectParameter* CDetailEnhance::GetEffectPointer()
{
	return new CDetailEnhanceParameter();
}

CEffectParameter* CDetailEnhance::GetDefaultEffectParameter()
{
	return new CDetailEnhanceParameter();
}


bool CDetailEnhance::IsSourcePreview()
{
	return true;
}


void CDetailEnhance::ApplyPreviewEffectSource(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer,
                                                CFiltreEffet* filtreEffet, CDraw* dessing)
{
	if (effectParameter != nullptr && !source.empty())
	{
		auto detailEnhanceParameter = static_cast<CDetailEnhanceParameter*>(effectParameter);
		filtreEffet->DetailEnhance(detailEnhanceParameter->sigmaX,
		                             detailEnhanceParameter->sigmaP);
	}
}


void CDetailEnhance::ApplyPreviewEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer,
                                          CFiltreEffet* filtreEffet, CDraw* m_cDessin, int& widthOutput,
                                          int& heightOutput)
{
}

CImageLoadingFormat* CDetailEnhance::ApplyEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer)
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

			auto detailEnhanceParameter = static_cast<CDetailEnhanceParameter*>(effectParameter);
			filter->DetailEnhance(detailEnhanceParameter->sigmaX,
			                        detailEnhanceParameter->sigmaP);
			imageLoad = new CImageLoadingFormat();
			cv::Mat bitmapOut = filter->GetBitmap(true);
			imageLoad->SetPicture(bitmapOut);
		}
	}

	return imageLoad;
}
