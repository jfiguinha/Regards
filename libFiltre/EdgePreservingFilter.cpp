#include <header.h>
//
//  BrightAndContrastFilter.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#include "EdgePreservingFilter.h"
#include "EdgePreservingFilterParameter.h"

#include <LibResource.h>
#include <FilterData.h>
#include <Metadata.h>
#include <BitmapDisplay.h>
#include <ImageLoadingFormat.h>
#include <FiltreEffet.h>
using namespace Regards::Filter;

CEdgePreservingFilter::CEdgePreservingFilter()
{
	libelleEffectSize = "flags";//CLibResource::LoadStringFromResource(L"LBLEFFECTSIZE", 1); //"Effect.Size";
	libelleEffectsigmaX = CLibResource::LoadStringFromResource(L"LBLEFFECTSIGMAX", 1); //"Effect.Sigma.X";
	libelleEffectsigmaP = CLibResource::LoadStringFromResource(L"LBLEFFECTSIGMAP", 1); //"Effect.Sigma.P";
}

int CEdgePreservingFilter::TypeApplyFilter()
{
	return 2;
}

CEdgePreservingFilter::~CEdgePreservingFilter()
{
}

wxString CEdgePreservingFilter::GetFilterLabel()
{
	return "Edge Preserving Filter";//CLibResource::LoadStringFromResource("LBLBILATERALDENOISING", 1);
}


int CEdgePreservingFilter::GetNameFilter()
{
	return IDM_FILTRE_EDGEPRESERVING;
}

int CEdgePreservingFilter::GetTypeFilter()
{
	return SPECIAL_EFFECT; //
}

void CEdgePreservingFilter::Filter(CEffectParameter* effectParameter, cv::Mat& source, const wxString& filename,
                              IFiltreEffectInterface* filtreInterface)
{
	auto edgePreservingFilterParameter = static_cast<CEdgePreservingFilterParameter*>(effectParameter);
	this->filename = filename;
	this->source = source;

	vector<CMetadata> flags;
	AddMetadataElement(flags, "RECURS_FILTER ", 1);
	AddMetadataElement(flags, "NORMCONV_FILTER", 2);

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


	filtreInterface->AddTreeInfos(libelleEffectSize, new CTreeElementValueInt(edgePreservingFilterParameter->flags),
		&flags, 3, 3);

	filtreInterface->AddTreeInfos(libelleEffectsigmaX, new CTreeElementValueInt(edgePreservingFilterParameter->sigmaX),
	                              &elementSigmaS);
	filtreInterface->AddTreeInfos(libelleEffectsigmaP, new CTreeElementValueInt(edgePreservingFilterParameter->sigmaP),
	                              &elementSigmaR);
}

void CEdgePreservingFilter::AddMetadataElement(vector<CMetadata>& element, wxString value, int key)
{
	CMetadata linear;
	linear.value = value;
	linear.depth = key;
	element.push_back(linear);
}

void CEdgePreservingFilter::FilterChangeParam(CEffectParameter* effectParameter, CTreeElementValue* valueData,
                                         const wxString& key)
{
	auto edgePreservingFilterParameter = static_cast<CEdgePreservingFilterParameter*>(effectParameter);
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
		edgePreservingFilterParameter->flags = value;
	}
	else if (key == libelleEffectsigmaX)
	{
		edgePreservingFilterParameter->sigmaX = value;
	}
	else if (key == libelleEffectsigmaP)
	{
		edgePreservingFilterParameter->sigmaP = value;
	}
}

void CEdgePreservingFilter::RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
{
	if (effectParameter != nullptr && filtreEffet != nullptr)
	{
		auto edgePreservingFilterParameter = static_cast<CEdgePreservingFilterParameter*>(effectParameter);
		filtreEffet->EdgePreservingFilter(edgePreservingFilterParameter->flags, edgePreservingFilterParameter->sigmaX,
			edgePreservingFilterParameter->sigmaP);
	}
}

bool CEdgePreservingFilter::NeedPreview()
{
	return true;
}

CEffectParameter* CEdgePreservingFilter::GetEffectPointer()
{
	return new CEdgePreservingFilterParameter();
}

CEffectParameter* CEdgePreservingFilter::GetDefaultEffectParameter()
{
	return new CEdgePreservingFilterParameter();
}


bool CEdgePreservingFilter::IsSourcePreview()
{
	return true;
}


void CEdgePreservingFilter::ApplyPreviewEffectSource(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer,
                                                CFiltreEffet* filtreEffet, CDraw* dessing)
{
	if (effectParameter != nullptr && !source.empty())
	{
		auto edgePreservingFilterParameter = static_cast<CEdgePreservingFilterParameter*>(effectParameter);
		filtreEffet->EdgePreservingFilter(edgePreservingFilterParameter->flags, edgePreservingFilterParameter->sigmaX,
			edgePreservingFilterParameter->sigmaP);
	}
}


void CEdgePreservingFilter::ApplyPreviewEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer,
                                          CFiltreEffet* filtreEffet, CDraw* m_cDessin, int& widthOutput,
                                          int& heightOutput)
{
}

CImageLoadingFormat* CEdgePreservingFilter::ApplyEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer)
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

			auto edgePreservingFilterParameter = static_cast<CEdgePreservingFilterParameter*>(effectParameter);
			filter->EdgePreservingFilter(edgePreservingFilterParameter->flags, edgePreservingFilterParameter->sigmaX,
				edgePreservingFilterParameter->sigmaP);
			imageLoad = new CImageLoadingFormat();
			cv::Mat bitmapOut = filter->GetBitmap(true);
			imageLoad->SetPicture(bitmapOut);
		}
	}

	return imageLoad;
}
