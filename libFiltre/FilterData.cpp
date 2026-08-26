#include <header.h>
//
//  FilterData.cpp
//  Regards.libFiltreInterface
//
//  Created by figuinha jacques on 07/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#include "FilterData.h"
#include <FiltreEffet.h>
#include <DecodeRaw.h>
#include <EffectParameter.h>
#include "VignetteFilter.h"
#include "LensFlareFilter.h"
#include "SwirlFilter.h"
#include "ColorBalanceFilter.h"
#include "CloudsFilter.h"
#include "RotateFreeFilter.h"
#include "SolarisationFilter.h"
#include "PosterisationFilter.h"
#include "PhotoFiltreFilter.h"
#include "BrightAndContrastFilter.h"
#include "MosaicFilter.h"
#include "SharpenMaskingFilter.h"
#include "MotionBlurFilter.h"
#include "BlurFilter.h"
#include "GaussianBlurFilter.h"
#include "NlmeansFilter.h"
#include "bilateralFilter.h"
#include "VideoFilter.h"
#include "AudioVideoFilter.h"
#include "WaveFilter.h"
#include "CropFilter.h"
#include <hqdn3dFilter.h>
#include <OilPaintingFilter.h>
#include "BokehFilter.h"
#include <CartoonFilter.h>
#include "FiltreImplementation.h"
#include "DetailEnhance.h"
#include "LensDistortion.h"
#include "EdgePreservingFilter.h"
#include "PencilSketch.h"
#include "InpaintFilter.h"
#include "Stylization.h"
#include <effect_id.h>
using namespace Regards::Filter;

// Macro utilitaire pour déclarer une entrée de la table de fabrication :
// associe un identifiant de filtre à une factory créant le type donné.
#define FILTER_ENTRY(id, type) { id, [] { return std::make_unique<type>(); } }

std::unordered_map<int, std::unique_ptr<CFilterWindowParam>> CFiltreData::filterList;

const std::unordered_map<int, CFiltreData::FilterFactory>& CFiltreData::GetFactoryMap()
{
	static const std::unordered_map<int, FilterFactory> factoryMap = {
		FILTER_ENTRY(IDM_FILTRE_SOFTEN, CSoftenFilter),
		FILTER_ENTRY(IDM_BRIGHTNESSCONTRAST_AUTO, CBrightnessAutoFilter),
		FILTER_ENTRY(IDM_HISTOGRAMNORMALIZE, CHistogramNormalizeFilter),
		FILTER_ENTRY(IDM_HISTOGRAMEQUALIZE, CHistogramEqualizeFilter),
		FILTER_ENTRY(IDM_ROTATE90, CRotate90Filter),
		FILTER_ENTRY(IDM_ROTATE270, CRotate270Filter),
		FILTER_ENTRY(IDM_FLIPVERTICAL, CFlipVerticalFilter),
		FILTER_ENTRY(IDM_FLIPHORIZONTAL, CFlipHorizontalFilter),
		FILTER_ENTRY(IDM_FILTRE_FLOU, CBlurFilter),
		FILTER_ENTRY(IDM_AJUSTEMENT_SOLARISATION, CSolarisationFilter),
		FILTER_ENTRY(IDM_FILTRE_FLOUGAUSSIEN, CGaussianBlurFilter),
		FILTER_ENTRY(IDM_FILTREANTIBRUIT, CMedianFilter),
		FILTER_ENTRY(IDM_DECODE_RAW, CDecodeRaw),
		FILTER_ENTRY(IDM_FILTRE_MOTIONBLUR, CMotionBlurFilter),
		FILTER_ENTRY(IDM_ROTATE_FREE, CRotateFreeFilter),
		FILTER_ENTRY(IDM_IMAGE_LIGHTCONTRAST, CBrightAndContrastFilter),
		FILTER_ENTRY(IDM_FILTREHQDN3D, Chqdn3dFilter),
		FILTER_ENTRY(IDM_FILTRE_VIGNETTE, CVignetteFilter),
		FILTER_ENTRY(IDM_FILTER_CARTOON, CCartoonFilter),
		FILTER_ENTRY(ID_AJUSTEMENT_PHOTOFILTRE, CPhotoFiltreFilter),
		FILTER_ENTRY(ID_AJUSTEMENT_POSTERISATION, CPosterisationFilter),
		FILTER_ENTRY(IDM_COLOR_BALANCE, CColorBalanceFilter),
		FILTER_ENTRY(IDM_FILTRE_SWIRL, CSwirlFilter),
		FILTER_ENTRY(IDM_FILTRE_CLOUDS, CCloudsFilter),
		FILTER_ENTRY(IDM_FILTER_OILPAINTING, COilPaintingFilter),
		FILTER_ENTRY(IDM_SHARPENMASKING, CSharpenMaskingFilter),
		FILTER_ENTRY(IDM_FILTRE_BOKEH, CBokehFilter),
		FILTER_ENTRY(IDM_FILTRE_BILATERAL, CBilateralFilter),
		FILTER_ENTRY(IDM_FILTRE_STYLISATION, CStylizationEffect),
		FILTER_ENTRY(IDM_FILTRE_PENCILSKETCH, CPencilSketchFilter),
		FILTER_ENTRY(IDM_FILTRE_EDGEPRESERVING, CEdgePreservingFilter),
		FILTER_ENTRY(IDM_FILTRE_DETAILENHANCE, CDetailEnhance),
		FILTER_ENTRY(IDM_FILTRE_NLMEAN, CNlmeansFilter),
		FILTER_ENTRY(IDM_FILTRE_ERODE, CErodeFilter),
		FILTER_ENTRY(IDM_FILTRE_DILATE, CDilateFilter),
		FILTER_ENTRY(IDM_FILTRE_SHARPEN, CSharpenFilter),
		FILTER_ENTRY(IDM_FILTRE_SHARPENSTRONG, CSharpenStrongFilter),
		FILTER_ENTRY(IDM_FILTRENOISE, CNoiseFilter),
		FILTER_ENTRY(IDM_FILTRE_MOSAIQUE, CMosaicFilter),
		FILTER_ENTRY(IDM_FILTRE_EMBOSS, CEmbossFilter),
		FILTER_ENTRY(IDM_GREY_LEVEL, CGrayLevelFilter),
		FILTER_ENTRY(IDM_IMAGE_SEPIA, CSepiaFilter),
		FILTER_ENTRY(IDM_BLACKANDWHITE, CNoirEtBlancFilter),
		FILTER_ENTRY(IDM_FILTRE_EDGE, CEdgeFilter),
		FILTER_ENTRY(IDM_NEGATIF, CNegatifFilter),
		FILTER_ENTRY(IDM_REDEYE, CRedEyeFilter),
		FILTER_ENTRY(IDM_FILTRE_RESTORE, CRestoreFilter),
		FILTER_ENTRY(IDM_FILTRE_COLORISATION, CColorisationFilter),
		FILTER_ENTRY(IDM_WAVE_EFFECT, CWaveFilter),
		FILTER_ENTRY(IDM_FILTRELENSFLARE, CLensFlareFilter),
		FILTER_ENTRY(IDM_FILTRELENSCORRECTION, CLensDistortion),
		FILTER_ENTRY(IDM_FILTRE_VIDEO, CVideoFilter),
		FILTER_ENTRY(IDM_FILTRE_AUDIOVIDEO, CAudioVideoFilter),
		FILTER_ENTRY(IDM_CROP, CCropFilter),
		FILTER_ENTRY(IDM_INPAINT, CInpaintFilter),
	};
	return factoryMap;
}

std::unique_ptr<CFilterWindowParam> CFiltreData::CreateEffectPointer(const int& numFilter)
{
	const auto& factoryMap = GetFactoryMap();
	auto it = factoryMap.find(numFilter);
	if (it != factoryMap.end())
		return it->second();
	return nullptr;
}

void CFiltreData::CreateFilterList()
{
	for (int numEffect = FILTER_START; numEffect < FILTER_END; numEffect++)
	{
		filterList[numEffect] = CreateEffectPointer(numEffect);
	}

	//Add Raw Filter
	filterList[IDM_DECODE_RAW] = CreateEffectPointer(IDM_DECODE_RAW);
	filterList[IDM_FILTRE_VIDEO] = CreateEffectPointer(IDM_FILTRE_VIDEO);
	filterList[IDM_FILTRE_AUDIOVIDEO] = CreateEffectPointer(IDM_FILTRE_AUDIOVIDEO);
}

// Fonction utilitaire centralisant l'accès au filtre par son identifiant.
// Retourne nullptr si l'identifiant n'existe pas dans la table.
CFilterWindowParam* CFiltreData::GetFilter(const int& numFilter)
{
	auto it = filterList.find(numFilter);
	if (it != filterList.end())
		return it->second.get();
	return nullptr;
}

int CFiltreData::RenderEffect(const int& numEffect, CFiltreEffet* filtreEffet, CEffectParameter* effectParameter,
	const bool& preview)
{
	CFilterWindowParam* filterEffect = GetFilter(numEffect);
	if (filterEffect != nullptr)
		filterEffect->RenderEffect(filtreEffet, effectParameter, preview);
	return 0;
}

CDraw* CFiltreData::GetDrawingPt(const int& numFilter)
{
	CFilterWindowParam* filterEffect = GetFilter(numFilter);
	if (filterEffect != nullptr)
		return filterEffect->GetDrawingPt();
	return nullptr;
}


bool CFiltreData::IsOpenCLCompatible(const int& numFilter)
{
	CFilterWindowParam* filterEffect = GetFilter(numFilter);
	if (filterEffect != nullptr)
		return filterEffect->IsOpenCLCompatible();

	return false;
}

bool CFiltreData::SupportMouseSelection(const int& numFilter)
{
	CFilterWindowParam* filterEffect = GetFilter(numFilter);
	if (filterEffect != nullptr)
		return filterEffect->SupportMouseSelection();

	return false;
}

void CFiltreData::SetCursor(const int& numFilter)
{
	CFilterWindowParam* filterEffect = GetFilter(numFilter);
	if (filterEffect != nullptr)
		filterEffect->SetCursor();
}

bool CFiltreData::SupportMouseClick(const int& numFilter)
{
	CFilterWindowParam* filterEffect = GetFilter(numFilter);
	if (filterEffect != nullptr)
		return filterEffect->SupportMouseClick();

	return false;
}

bool CFiltreData::NeedPreview(const int& numFilter)
{
	CFilterWindowParam* filterEffect = GetFilter(numFilter);
	if (filterEffect != nullptr)
		return filterEffect->NeedPreview();
	return false;
}

int CFiltreData::GetTypeEffect(const int& numFilter)
{
	CFilterWindowParam* filterEffect = GetFilter(numFilter);
	if (filterEffect != nullptr)
		return filterEffect->GetTypeFilter();

	return 0;
}

CEffectParameter* CFiltreData::GetEffectParameter(const int& numFilter)
{
	CFilterWindowParam* filterEffect = GetFilter(numFilter);
	if (filterEffect != nullptr)
		return filterEffect->GetEffectPointer();

	return nullptr;
}

CEffectParameter* CFiltreData::GetDefaultEffectParameter(const int& numFilter)
{
	CFilterWindowParam* filterEffect = GetFilter(numFilter);
	if (filterEffect != nullptr)
		return filterEffect->GetDefaultEffectParameter();

	return nullptr;
}


int CFiltreData::TypeApplyFilter(const int& numItem)
{
	CFilterWindowParam* filterEffect = GetFilter(numItem);
	if (filterEffect != nullptr)
		return filterEffect->TypeApplyFilter();
	return 3;
}

wxString CFiltreData::GetFilterLabel(const int& numFilter)
{
	CFilterWindowParam* filterEffect = GetFilter(numFilter);
	if (filterEffect != nullptr)
		return filterEffect->GetFilterLabel();
	return "";
}