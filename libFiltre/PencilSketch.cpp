#include <header.h>
//
//  BrightAndContrastFilter.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#include "PencilSketch.h"
#include "PencilSketchParameter.h"

#include <LibResource.h>
#include <FilterData.h>
#include <BitmapDisplay.h>
#include <ImageLoadingFormat.h>
#include <FiltreEffet.h>
using namespace Regards::Filter;

CPencilSketchFilter::CPencilSketchFilter()
{
	libelleEffectSize = "Effect.shade_factor";// CLibResource::LoadStringFromResource(L"LBLEFFECTSIZE", 1); //"Effect.Size";
	libelleEffectsigmaX = CLibResource::LoadStringFromResource(L"LBLEFFECTSIGMAX", 1); //"Effect.Sigma.X";
	libelleEffectsigmaP = CLibResource::LoadStringFromResource(L"LBLEFFECTSIGMAP", 1); //"Effect.Sigma.P";
}

int CPencilSketchFilter::TypeApplyFilter()
{
	return 2;
}

CPencilSketchFilter::~CPencilSketchFilter()
{
}

wxString CPencilSketchFilter::GetFilterLabel()
{
	return "Pencil Sketch";// CLibResource::LoadStringFromResource("LBLBILATERALDENOISING", 1);
}


int CPencilSketchFilter::GetNameFilter()
{
	return IDM_FILTRE_PENCILSKETCH;
}

int CPencilSketchFilter::GetTypeFilter()
{
	return SPECIAL_EFFECT; //
}

void CPencilSketchFilter::Filter(CEffectParameter* effectParameter, cv::Mat& source, const wxString& filename,
                              IFiltreEffectInterface* filtreInterface)
{
	auto PencilSketchFilterParameter = static_cast<CPencilSketchFilterParameter*>(effectParameter);
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

	vector<int> elementShadefactor;
	for (auto i = 0; i < 10; i++)
	{
		elementShadefactor.push_back(i);
	}



	filtreInterface->AddTreeInfos(libelleEffectsigmaX, new CTreeElementValueInt(PencilSketchFilterParameter->sigmaX),
	                              &elementSigmaS);
	filtreInterface->AddTreeInfos(libelleEffectsigmaP, new CTreeElementValueInt(PencilSketchFilterParameter->sigmaP),
	                              &elementSigmaR);
	filtreInterface->AddTreeInfos(libelleEffectSize, new CTreeElementValueInt(PencilSketchFilterParameter->shade_factor),
		&elementShadefactor);
}

void CPencilSketchFilter::FilterChangeParam(CEffectParameter* effectParameter, CTreeElementValue* valueData,
                                         const wxString& key)
{
	auto PencilSketchFilterParameter = static_cast<CPencilSketchFilterParameter*>(effectParameter);
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

	if (key == libelleEffectSize)
	{
		PencilSketchFilterParameter->shade_factor = value;
	}
	else if (key == libelleEffectsigmaX)
	{
		PencilSketchFilterParameter->sigmaX = value;
	}
	else if (key == libelleEffectsigmaP)
	{
		PencilSketchFilterParameter->sigmaP = value;
	}
}

void CPencilSketchFilter::RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
{
	if (effectParameter != nullptr && filtreEffet != nullptr)
	{
		auto PencilSketchFilterParameter = static_cast<CPencilSketchFilterParameter*>(effectParameter);
		filtreEffet->PencilSketch(PencilSketchFilterParameter->sigmaX,
			PencilSketchFilterParameter->sigmaP, PencilSketchFilterParameter->shade_factor);
	}
}

bool CPencilSketchFilter::NeedPreview()
{
	return true;
}

CEffectParameter* CPencilSketchFilter::GetEffectPointer()
{
	return new CPencilSketchFilterParameter();
}

CEffectParameter* CPencilSketchFilter::GetDefaultEffectParameter()
{
	return new CPencilSketchFilterParameter();
}


bool CPencilSketchFilter::IsSourcePreview()
{
	return true;
}


void CPencilSketchFilter::ApplyPreviewEffectSource(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer,
                                                CFiltreEffet* filtreEffet, CDraw* dessing)
{
	if (effectParameter != nullptr && !source.empty())
	{
		auto PencilSketchFilterParameter = static_cast<CPencilSketchFilterParameter*>(effectParameter);
		filtreEffet->PencilSketch(PencilSketchFilterParameter->sigmaX,
			PencilSketchFilterParameter->sigmaP, PencilSketchFilterParameter->shade_factor);
	}
}


void CPencilSketchFilter::ApplyPreviewEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer,
                                          CFiltreEffet* filtreEffet, CDraw* m_cDessin, int& widthOutput,
                                          int& heightOutput)
{
}

CImageLoadingFormat* CPencilSketchFilter::ApplyEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer)
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

			auto PencilSketchFilterParameter = static_cast<CPencilSketchFilterParameter*>(effectParameter);
			filter->PencilSketch(PencilSketchFilterParameter->sigmaX,
				PencilSketchFilterParameter->sigmaP, PencilSketchFilterParameter->shade_factor);
			imageLoad = new CImageLoadingFormat();
			cv::Mat bitmapOut = filter->GetBitmap(true);
			imageLoad->SetPicture(bitmapOut);
		}
	}

	return imageLoad;
}
