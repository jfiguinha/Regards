#include <header.h>
//
//  BrightAndContrastFilter.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#include "Stylization.h"
#include "StylizationParameter.h"

#include <LibResource.h>
#include <FilterData.h>
#include <BitmapDisplay.h>
#include <ImageLoadingFormat.h>
#include <FiltreEffet.h>
using namespace Regards::Filter;

CStylizationEffect::CStylizationEffect()
{
	libelleEffectsigmaX = CLibResource::LoadStringFromResource(L"LBLEFFECTSIGMAX", 1); //"Effect.Sigma.X";
	libelleEffectsigmaP = CLibResource::LoadStringFromResource(L"LBLEFFECTSIGMAP", 1); //"Effect.Sigma.P";
}

int CStylizationEffect::TypeApplyFilter()
{
	return 2;
}

CStylizationEffect::~CStylizationEffect()
{
}

wxString CStylizationEffect::GetFilterLabel()
{
	return "Stylisation";// CLibResource::LoadStringFromResource("LBLBILATERALDENOISING", 1);
}


int CStylizationEffect::GetNameFilter()
{
	return IDM_FILTRE_STYLISATION;
}

int CStylizationEffect::GetTypeFilter()
{
	return SPECIAL_EFFECT; //
}

void CStylizationEffect::Filter(CEffectParameter* effectParameter, cv::Mat& source, const wxString& filename,
                              IFiltreEffectInterface* filtreInterface)
{
	auto StylizationEffectParameter = static_cast<CStylizationEffectParameter*>(effectParameter);
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

	filtreInterface->AddTreeInfos(libelleEffectsigmaX, new CTreeElementValueInt(StylizationEffectParameter->sigmaX),
	                              &elementSigmaS);
	filtreInterface->AddTreeInfos(libelleEffectsigmaP, new CTreeElementValueInt(StylizationEffectParameter->sigmaP),
	                              &elementSigmaR);
}

void CStylizationEffect::FilterChangeParam(CEffectParameter* effectParameter, CTreeElementValue* valueData,
                                         const wxString& key)
{
	auto StylizationEffectParameter = static_cast<CStylizationEffectParameter*>(effectParameter);
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
		StylizationEffectParameter->sigmaX = value;
	}
	else if (key == libelleEffectsigmaP)
	{
		StylizationEffectParameter->sigmaP = value;
	}
}

void CStylizationEffect::RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
{
	if (effectParameter != nullptr && filtreEffet != nullptr)
	{
		auto StylizationEffectParameter = static_cast<CStylizationEffectParameter*>(effectParameter);
		filtreEffet->Stylization(StylizationEffectParameter->sigmaX, StylizationEffectParameter->sigmaP);
	}
}

bool CStylizationEffect::NeedPreview()
{
	return true;
}

CEffectParameter* CStylizationEffect::GetEffectPointer()
{
	return new CStylizationEffectParameter();
}

CEffectParameter* CStylizationEffect::GetDefaultEffectParameter()
{
	return new CStylizationEffectParameter();
}


bool CStylizationEffect::IsSourcePreview()
{
	return true;
}


void CStylizationEffect::ApplyPreviewEffectSource(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer,
                                                CFiltreEffet* filtreEffet, CDraw* dessing)
{
	if (effectParameter != nullptr && !source.empty())
	{
		auto StylizationEffectParameter = static_cast<CStylizationEffectParameter*>(effectParameter);
		filtreEffet->Stylization(StylizationEffectParameter->sigmaX, StylizationEffectParameter->sigmaP);
	}
}


void CStylizationEffect::ApplyPreviewEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer,
                                          CFiltreEffet* filtreEffet, CDraw* m_cDessin, int& widthOutput,
                                          int& heightOutput)
{
}

CImageLoadingFormat* CStylizationEffect::ApplyEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer)
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

			auto StylizationEffectParameter = static_cast<CStylizationEffectParameter*>(effectParameter);
			filter->Stylization(StylizationEffectParameter->sigmaX, StylizationEffectParameter->sigmaP);
			imageLoad = new CImageLoadingFormat();
			cv::Mat bitmapOut = filter->GetBitmap(true);
			imageLoad->SetPicture(bitmapOut);
		}
	}

	return imageLoad;
}
