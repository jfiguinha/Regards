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
#include "bm3dWindowFilter.h"
#include "LensFlareFilter.h"
#include "SwirlFilter.h"
#include "ColorBalanceFilter.h"
#include "CloudsFilter.h"
#include "RotateFreeFilter.h"
#include "SolarisationFilter.h"
#include "PosterisationFilter.h"
#include "PhotoFiltreFilter.h"
#include "BrightAndContrastFilter.h"
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
#include <BokehFilter.h>
#include <CartoonFilter.h>
#include "FiltreImplementation.h"
#include <effect_id.h>
using namespace Regards::Filter;

map<int, CFilterWindowParam*> CFiltreData::filterList;

void CFiltreData::CreateFilterList()
{
    for (int numEffect = FILTER_START; numEffect < FILTER_END; numEffect++)
    {
        filterList.insert(std::make_pair(numEffect, CreateEffectPointer(numEffect)));
    }

    //Add Raw Filter
    filterList.insert(std::make_pair(IDM_FILTRE_VIDEO, CreateEffectPointer(IDM_FILTRE_VIDEO)));
    filterList.insert(std::make_pair(IDM_FILTRE_AUDIOVIDEO, CreateEffectPointer(IDM_FILTRE_AUDIOVIDEO)));
}

CFiltreData::CFiltreData(){}
CFiltreData::~CFiltreData(){}


int CFiltreData::RenderEffect(const int &numEffect, CFiltreEffet * filtreEffet, CEffectParameter * effectParameter, const bool &preview)
{
    CFilterWindowParam* filterEffect = filterList[numEffect];
    if (filterEffect != nullptr)
        filterEffect->RenderEffect(filtreEffet, effectParameter, preview);
    return 0;
}


CFilterWindowParam * CFiltreData::CreateEffectPointer(const int &numFilter)
{
    CFilterWindowParam * filterEffect = nullptr;
	switch (numFilter)
	{
    case IDM_FILTRE_SOFTEN:
        filterEffect = new CSoftenFilter();
        break;

    case IDM_BRIGHTNESSCONTRAST_AUTO:
        filterEffect = new CBrightnessAutoFilter();
        break;

    case IDM_HISTOGRAMNORMALIZE:
        filterEffect = new CHistogramNormalizeFilter();
        break;

    case IDM_HISTOGRAMEQUALIZE:
        filterEffect = new CHistogramEqualizeFilter();
        break;

    case IDM_ROTATE90:
        filterEffect = new CRotate90Filter();
        break;

    case IDM_ROTATE270:
        filterEffect = new CRotate270Filter();
        break;

    case IDM_FLIPVERTICAL:
        filterEffect = new CFlipVerticalFilter();
        break;

    case IDM_FLIPHORIZONTAL:
        filterEffect = new CFlipHorizontalFilter();
        break;

    case IDM_FILTRE_FLOU:
        filterEffect = new CBlurFilter();
        break;

    case IDM_AJUSTEMENT_SOLARISATION:
        filterEffect = new CSolarisationFilter();
        break;

    case IDM_FILTRE_FLOUGAUSSIEN:
        filterEffect = new CGaussianBlurFilter();
        break;

    case IDM_FILTREANTIBRUIT:
        filterEffect = new CMedianFilter();
        break;

    case IDM_FILTRE_MOTIONBLUR:
    {
        filterEffect = new  CMotionBlurFilter();
        break;
    }

    case IDM_ROTATE_FREE:
    {
        filterEffect = new  CRotateFreeFilter();
        break;
    }

    case IDM_IMAGE_LIGHTCONTRAST:
    {
        filterEffect = new  CBrightAndContrastFilter();
        break;
    }

    case IDM_FILTREHQDN3D:
    {
        filterEffect = new  Chqdn3dFilter();
        break;
    }

    case IDM_FILTRE_VIGNETTE:
    {
        filterEffect = new  CVignetteFilter();
        break;
    }

    case IDM_FILTER_CARTOON:
    {
        filterEffect = new  CCartoonFilter();
        break;
    }

    case ID_AJUSTEMENT_PHOTOFILTRE:
    {
        filterEffect = new  CPhotoFiltreFilter();
        break;
    }

    case ID_AJUSTEMENT_POSTERISATION:
    {
        filterEffect = new  CPosterisationFilter();
        break;
    }

    case IDM_COLOR_BALANCE:
    {
        filterEffect = new  CColorBalanceFilter();
        break;
    }

    case IDM_FILTRE_SWIRL:
    {
        filterEffect = new  CSwirlFilter();
        break;
    }

    case IDM_FILTRE_CLOUDS:
    {
        filterEffect = new  CCloudsFilter();
        break;
    }

    case IDM_FILTER_OILPAINTING:
    {
        filterEffect = new  COilPaintingFilter();
        break;
    }

    case IDM_SHARPENMASKING:
    {
        filterEffect = new  CSharpenMaskingFilter();
        break;
    }

    case IDM_FILTRE_BOKEH:
    {
        filterEffect = new  CBokehFilter();
        break;
    }

    case IDM_FILTRE_BILATERAL:
    {
        filterEffect = new  CBilateralFilter();
        break;
    }

    case IDM_FILTRE_NLMEAN:
    {
        filterEffect = new  CNlmeansFilter();
        break;
    }


    case IDM_FILTER_BM3D:
        filterEffect = new  CBm3dWindowFilter();
        break;

    case IDM_FILTRE_ERODE:
        filterEffect = new  CErodeFilter();
        break;

    case IDM_FILTRE_DILATE:
        filterEffect = new  CDilateFilter();
        break;

    case IDM_FILTRE_SHARPEN:
        filterEffect = new  CSharpenFilter();
        break;

    case IDM_FILTRE_SHARPENSTRONG:
        filterEffect = new CSharpenStrongFilter();
        break;

    case IDM_FILTRENOISE:
        filterEffect = new CNoiseFilter();
        break;

    case IDM_FILTRE_MOSAIQUE:
        filterEffect = new CMosaicFilter();
        break;

    case IDM_FILTRE_EMBOSS:
        filterEffect = new CEmbossFilter();
        break;

    case IDM_GREY_LEVEL:
        filterEffect = new CGrayLevelFilter();
        break;

    case IDM_IMAGE_SEPIA:
        filterEffect = new CSepiaFilter();
        break;

    case IDM_BLACKANDWHITE:
        filterEffect = new CNoirEtBlancFilter();
        break;

    case IDM_FILTRE_EDGE:
        filterEffect = new CEdgeFilter();
        break;

    case IDM_NEGATIF:
        filterEffect = new CNegatifFilter();
        break;

    case IDM_REDEYE:
        filterEffect = new CRedEyeFilter();
        break;

	case IDM_WAVE_EFFECT:
		filterEffect = new CWaveFilter();
		break;

	case IDM_FILTRELENSFLARE:
        filterEffect = new CLensFlareFilter();
		break;


	case IDM_FILTRE_VIDEO:
		filterEffect = new CVideoFilter();
		break;   


	case IDM_FILTRE_AUDIOVIDEO:
		filterEffect = new CAudioVideoFilter();
		break;

	case IDM_CROP:
		filterEffect = new CCropFilter();
		break;
    default: ;
	}  
    return filterEffect;
}



CDraw * CFiltreData::GetDrawingPt(const int &numFilter)
{
    CFilterWindowParam* filterEffect = filterList[numFilter];
    if (filterEffect != nullptr)
        return filterEffect->GetDrawingPt();
	return nullptr;
}

bool CFiltreData::IsOpenGLCompatible(const int &numFilter)
{
    CFilterWindowParam* filterEffect = filterList[numFilter];
    if (filterEffect != nullptr)
        return filterEffect->IsOpenGLCompatible();

	return false;
}

bool CFiltreData::IsOpenCLCompatible(const int &numFilter)
{
    CFilterWindowParam* filterEffect = filterList[numFilter];
    if (filterEffect != nullptr)
        return filterEffect->IsOpenCLCompatible();

	return false;
}

bool CFiltreData::SupportMouseSelection(const int &numFilter)
{
    CFilterWindowParam* filterEffect = filterList[numFilter];
    if (filterEffect != nullptr)
        return filterEffect->SupportMouseSelection();

	return false;
}

void CFiltreData::SetCursor(const int &numFilter)
{
    CFilterWindowParam* filterEffect = filterList[numFilter];
    if (filterEffect != nullptr)
        return filterEffect->SetCursor();

}

bool CFiltreData::SupportMouseClick(const int &numFilter)
{
    CFilterWindowParam* filterEffect = filterList[numFilter];
    if (filterEffect != nullptr)
        return filterEffect->SupportMouseClick();

	return false;
}

bool CFiltreData::NeedPreview(const int &numFilter)
{
    CFilterWindowParam* filterEffect = filterList[numFilter];
    if (filterEffect != nullptr)
        return filterEffect->NeedPreview();
    return false;
}

int CFiltreData::GetTypeEffect(const int &numFilter)
{
    CFilterWindowParam* filterEffect = filterList[numFilter];
    if (filterEffect != nullptr)
        return filterEffect->GetTypeFilter();

    return 0;
}

CEffectParameter* CFiltreData::GetEffectParameter(const int& numFilter)
{
    CFilterWindowParam* filterEffect = filterList[numFilter];
    if (filterEffect != nullptr)
        return filterEffect->GetEffectPointer();

    return nullptr;
}

CEffectParameter * CFiltreData::GetDefaultEffectParameter(const int &numFilter)
{
    CFilterWindowParam* filterEffect = filterList[numFilter];
    if (filterEffect != nullptr)
        return filterEffect->GetDefaultEffectParameter();

    return nullptr;
}


int CFiltreData::TypeApplyFilter(const int &numItem)
{
    CFilterWindowParam* filterEffect = filterList[numItem];
    if (filterEffect != nullptr)
        return filterEffect->TypeApplyFilter();
    return 3;
}

wxString CFiltreData::GetFilterLabel(const int &numFilter)
{
    CFilterWindowParam* filterEffect = filterList[numFilter];
    if (filterEffect != nullptr)
        return filterEffect->GetFilterLabel();
    return "";
}
