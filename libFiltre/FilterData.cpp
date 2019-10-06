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
#include <LibResource.h>
#include <RgbEffectParameter.h>
#include <BrightAndContrastEffectParameter.h>
#include <bm3dEffectParameter.h>
#include <CloudsEffectParameter.h>
#include <FreeRotateEffectParameter.h>
#include <MotionBlurEffectParameter.h>
#include <PhotoFiltreEffectParameter.h>
#include <PosterisationEffectParameter.h>
#include <RgbEffectParameter.h>
#include <SolarisationEffectParameter.h>
#include <LensFlareEffectParameter.h>
#include <SharpenMaskingParameter.h>
#include <SwirlEffectParameter.h>
#include <GaussianBlurEffectParameter.h>
#include <BlurEffectParameter.h>
#include <EffectVideoParameter.h>
#include "BilateralEffectParameter.h"
#include "DecodeRawParameter.h"
#include "NlmeansEffectParameter.h"
#include "bm3dWindowFilter.h"
#include "ClaheEffectParameter.h"
#include "KuwaharaEffectParameter.h"
#include "BestExposureEffectParameter.h"
#include "WaveEffectParameter.h"
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
#include "ClaheFilter.h"
#include "bilateralFilter.h"
#include "DecodeRaw.h"
#include "VideoFilter.h"
#include "AudioVideoFilter.h"
#include "WaveFilter.h"
#include "BestExposureFilter.h"
#include "CropFilter.h"
using namespace Regards::Viewer;
vector<CFiltreData::CLabelFilter> CFiltreData::labelFilterList;

CFiltreData::CFiltreData(){}
CFiltreData::~CFiltreData(){}


bool CFiltreData::NeedOriginalPreview(const int &numFilter)
{
    /*
    switch (numFilter)
    {
        case IDM_FILTER_BM3D:
            return true;
    }
    */
    return false;
}

int CFiltreData::RenderEffect(const int &numEffect, CFiltreEffet * filtreEffet, CEffectParameter * effectParameter, const bool &preview)
{
    switch (numEffect)
    {      
        case IDM_FILTRE_SOFTEN:
            filtreEffet->Soften();
            break;
            
        case IDM_HISTOGRAMNORMALIZE:
            filtreEffet->HistogramNormalize();
            break;
            
        case IDM_HISTOGRAMEQUALIZE:
            filtreEffet->HistogramEqualize();
            break;
            
        case IDM_HISTOGRAMLOG:
            filtreEffet->HistogramLog();
            break;

		case IDM_ROTATE90:
			filtreEffet->Rotate90();
			break;

		case IDM_ROTATE270:
			filtreEffet->Rotate270();
			break;

		case IDM_FLIPVERTICAL:
			filtreEffet->FlipVertical();
			break;

		case IDM_FLIPHORIZONTAL:
			filtreEffet->FlipHorizontal();
			break;
            
        case IDM_FILTRE_FLOU:
            if (effectParameter != nullptr)
            {
                CBlurEffectParameter * blurEffect = (CBlurEffectParameter *)effectParameter;
                filtreEffet->Blur(blurEffect->size);
                return 1;
            }
            break;
            
        case IDM_AJUSTEMENT_SOLARISATION:
        {
            if (effectParameter != nullptr)
            {
                CSolarisationEffectParameter * solarisationEffectParameter = (CSolarisationEffectParameter *)effectParameter;
                filtreEffet->Solarize(solarisationEffectParameter->threshold);
                return 1;
            }
        }
            break;
            
        case IDM_FILTRE_FLOUGAUSSIEN:
			if (effectParameter != nullptr)
			{
				CGaussianBlurEffectParameter  * gaussianBlur = (CGaussianBlurEffectParameter *)effectParameter;
				filtreEffet->GaussianBlur(gaussianBlur->radius, gaussianBlur->boxSize);
                return 1;
			}
            break;
            
        case IDM_FILTREANTIBRUIT:
            filtreEffet->Median();
            break;
            
        case IDM_FILTRE_MOTIONBLUR:
        {
            if (effectParameter != nullptr)
            {
                CMotionBlurEffectParameter * motionblurEffectParameter = (CMotionBlurEffectParameter *)effectParameter;
                filtreEffet->MotionBlur(motionblurEffectParameter->radius, motionblurEffectParameter->sigma, motionblurEffectParameter->angle);
                return 1;
            }
            break;
        }
            break;
        case IDM_ROTATE_FREE:
        {
            if (effectParameter != nullptr)
            {
                CFreeRotateEffectParameter * freeRotate = (CFreeRotateEffectParameter *)effectParameter;
                filtreEffet->RotateFree(freeRotate->angle);
                return 1;
            }
        }
            break;
            
        case IDM_IMAGE_LIGHTCONTRAST:
        {
            if (effectParameter != nullptr)
            {
                CBrightAndContrastEffectParameter  * brightAndContrast = (CBrightAndContrastEffectParameter *)effectParameter;
                filtreEffet->BrightnessAndContrast(brightAndContrast->brightness, brightAndContrast->contrast);
                return 1;
            }
        }
            break;
            
        case ID_AJUSTEMENT_PHOTOFILTRE:
        {
            if (effectParameter != nullptr)
            {
                CPhotoFiltreEffectParameter * photoFiltreParameter = (CPhotoFiltreEffectParameter *)effectParameter;
                filtreEffet->PhotoFiltre(CRgbaquad(photoFiltreParameter->red, photoFiltreParameter->green, photoFiltreParameter->blue), photoFiltreParameter->intensity);
                return 1;
            }
        }
            break;
        case ID_AJUSTEMENT_POSTERISATION:
        {
            if (effectParameter != nullptr)
            {
                CPosterisationEffectParameter * posterisationFiltreParameter = (CPosterisationEffectParameter *)effectParameter;
                filtreEffet->Posterize(posterisationFiltreParameter->level, posterisationFiltreParameter->gamma);
                return 1;
            }
        }
            break;
        case IDM_COLOR_BALANCE:
        {
            if (effectParameter != nullptr)
            {
                CRgbEffectParameter * rgbParameter = (CRgbEffectParameter *)effectParameter;
                filtreEffet->RGBFilter(rgbParameter->red, rgbParameter->green, rgbParameter->blue);
                return 1;
            }
        }
            break;
        case IDM_FILTRE_SWIRL:
        {
            if (effectParameter != nullptr)
            {
                CSwirlEffectParameter * swirlParameter = (CSwirlEffectParameter *)effectParameter;
                if(preview)
                {
                    float ratio = (float)filtreEffet->GetWidth() / (float)swirlParameter->bitmapWidth;
                    filtreEffet->Swirl(swirlParameter->radius * ratio, swirlParameter->angle);
                }
                else
                    filtreEffet->Swirl(swirlParameter->radius, swirlParameter->angle);
                    
                return 1;
            }
        }
            break;
            
        case IDM_FILTRE_CLOUDS:
        {
            if (effectParameter != nullptr)
            {
                CCloudsEffectParameter * cloudsParameter = (CCloudsEffectParameter *)effectParameter;
				filtreEffet->CloudsFilter(cloudsParameter->colorFront, cloudsParameter->colorBack, cloudsParameter->amplitude, cloudsParameter->frequence, cloudsParameter->octave, cloudsParameter->transparency);
                return 1;
            }
        }
            break;

		case IDM_SHARPENMASKING:
		{
			if (effectParameter != nullptr)
			{
				CSharpenMaskingEffectParameter * sharpenParameter = (CSharpenMaskingEffectParameter *)effectParameter;
				filtreEffet->SharpenMasking(sharpenParameter->sharpness);
                return 1;
			}
		}
		break;

		case IDM_FILTRE_BILATERAL:
		{
			if (effectParameter != nullptr)
			{
				CBilateralEffectParameter * bilateralEffectParameter = (CBilateralEffectParameter *)effectParameter;
				filtreEffet->BilateralFilter(bilateralEffectParameter->fSize, bilateralEffectParameter->sigmaX, bilateralEffectParameter->sigmaP);
                return 1;
			}
		}
		break;

		case IDM_FILTRE_NLMEAN:
		{
			if (effectParameter != nullptr)
			{
				CNlmeansEffectParameter * nlmeansEffectParameter = (CNlmeansEffectParameter *)effectParameter;
				filtreEffet->NlmeansFilter(nlmeansEffectParameter->fSize, nlmeansEffectParameter->bSize, nlmeansEffectParameter->sigma);
                return 1;
			}
		}
		break;

		case IDM_FILTRE_CLAHE:
		{
			if (effectParameter != nullptr)
			{
				CClaheEffectParameter * claheEffectParameter = (CClaheEffectParameter *)effectParameter;
				filtreEffet->ClaheFilter(claheEffectParameter->nBins, claheEffectParameter->clipLevel, claheEffectParameter->windowSize);
                return 1;
			}
		}
		break;
        
        case IDM_FILTER_BM3D:
			if (effectParameter != nullptr)
			{
				CBm3dEffectParameter * bm3dParameter = (CBm3dEffectParameter *)effectParameter;
				filtreEffet->Bm3d(bm3dParameter->fSize);
                return 1;
			}
            break;

        case IDM_FILTRE_ERODE:
            filtreEffet->Erode();
            break;
            
        case IDM_BEST_EXPOSURE:
            //filtreEffet->BestExposure();
			if (effectParameter != nullptr)
			{
				CBestExposureEffectParameter * bestExposureParameter = (CBestExposureEffectParameter *)effectParameter;
				filtreEffet->BestExposure(bestExposureParameter->tmo);
                return 1;
			}
            break;     
            
        case IDM_FILTER_BILATERAL2DS:
            //filtreEffet->FilterBilateral2DS();
            break;              

        case IDM_FILTER_KUWAHARA:
			if (effectParameter != nullptr)
			{
				CKuwaharaEffectParameter * kuwaharaEffectParameter = (CKuwaharaEffectParameter *)effectParameter;
				filtreEffet->FilterKuwahara(kuwaharaEffectParameter->fSize);
                return 1;
			}
            break;        
            
        case IDM_FILTRE_DILATE:
            filtreEffet->Dilate();
            break;
            
        case IDM_FILTRE_SHARPEN:
            filtreEffet->Sharpen();
            break;
            
        case IDM_FILTRE_SHARPENSTRONG:
            filtreEffet->SharpenStrong();
            break;
            
        case IDM_FILTRENOISE:
            filtreEffet->Noise();
            break;
            
        case IDM_FILTRE_MOSAIQUE:
            filtreEffet->FiltreMosaic();
            break;
            
        case IDM_FILTRE_EMBOSS:
            filtreEffet->Emboss();
            break;
            
        case IDM_GREY_LEVEL:
            filtreEffet->NiveauDeGris();
            break;
            
        case IDM_IMAGE_SEPIA:
            filtreEffet->Sepia();
            break;
            
        case IDM_BLACKANDWHITE:
            filtreEffet->NoirEtBlanc();
            break;
            
        case IDM_FILTRE_EDGE:
            filtreEffet->FiltreEdge();
            break;
            
        case IDM_NEGATIF:
            filtreEffet->Negatif();
            break;

    }
    return 0;
}


CFilterWindowParam * CFiltreData::CreateEffectPointer(const int &numFilter)
{
    CFilterWindowParam * filterEffect = nullptr;
	switch (numFilter)
	{
	case IDM_FILTRE_MOTIONBLUR:
        filterEffect = new CMotionBlurFilter();
       // filterEffect->Filter(effectParameter, source, this);
		break;

	case IDM_FILTRE_CLOUDS:
        filterEffect = new CCloudsFilter();
       // filterEffect->Filter(effectParameter, source, this);
		break;

	case IDM_DECODE_RAW:
        filterEffect = new Regards::Viewer::CDecodeRaw();
        //filterEffect->Filter(effectParameter, source, this);
		break;
        
    case IDM_BEST_EXPOSURE:
        filterEffect = new CBestExposureFilter();
        break;
        
    case IDM_FILTER_BM3D:
        filterEffect = new CBm3dWindowFilter();
        break;

	case IDM_SHARPENMASKING:
		filterEffect = new CSharpenMaskingFilter();
		//filterEffect->Filter(effectParameter, source, this);
		break;

	case IDM_FILTRE_SWIRL:
        filterEffect = new CSwirlFilter();
       // filterEffect->Filter(effectParameter, source, this);
		break;

	case IDM_COLOR_BALANCE:
        filterEffect = new CColorBalanceFilter();
       // filterEffect->Filter(effectParameter, source, this);
		break;

	case ID_AJUSTEMENT_POSTERISATION:
        filterEffect = new CPosterisationFilter();
       // filterEffect->Filter(effectParameter, source, this);
		break;

	case ID_AJUSTEMENT_PHOTOFILTRE:
        filterEffect = new CPhotoFiltreFilter();
       // filterEffect->Filter(effectParameter, source, this);
		break;

	case IDM_IMAGE_LIGHTCONTRAST:
        filterEffect = new CBrightAndContrastFilter();
       // filterEffect->Filter(effectParameter, source, this);
		break;

	case IDM_AJUSTEMENT_SOLARISATION:
        filterEffect = new CSolarisationFilter();
       // filterEffect->Filter(effectParameter, source, this);
		break;

	case IDM_FILTRE_FLOU:
        filterEffect = new CBlurFilter();
       // filterEffect->Filter(effectParameter, source, this);
		break;

	case IDM_WAVE_EFFECT:
		filterEffect = new CWaveFilter();
		//filterEffect->Filter(effectParameter, source, this);
		break;

	case IDM_FILTRELENSFLARE:
        filterEffect = new CLensFlareFilter();
       /// filterEffect->Filter(effectParameter, source, this);
		break;

	case IDM_ROTATE_FREE:
        filterEffect = new CRotateFreeFilter();
        //filterEffect->Filter(effectParameter, source, this);
		break;

	case IDM_FILTRE_FLOUGAUSSIEN:
		filterEffect = new CGaussianBlurFilter();
		//filterEffect->Filter(effectParameter, source, this);
		break;

	case IDM_FILTRE_VIDEO:
		filterEffect = new CVideoFilter();
		//filterEffect->Filter(effectParameter, filename, this);
		break;   

	case IDM_FILTRE_BILATERAL:
		filterEffect = new CBilateralFilter();
		//filterEffect->Filter(effectParameter, source, this);
		break;

	case IDM_FILTRE_NLMEAN:
		filterEffect = new CNlmeansFilter();
		//filterEffect->Filter(effectParameter, source, this);
		break;

	case IDM_FILTRE_CLAHE:
		filterEffect = new CClaheFilter();
		//filterEffect->Filter(effectParameter, source, this);
		break;

	case IDM_FILTRE_AUDIOVIDEO:
		filterEffect = new CAudioVideoFilter();
		//filterEffect->Filter(effectParameter, filename, this);
		break;

	case IDM_CROP:
		filterEffect = new CCropFilter();
		//filterEffect->Filter(effectParameter, filename, this);
		break;
	}  
    return filterEffect;
}

CFiltreData::CLabelFilter CFiltreData::CLabelFilter::CreateLabelFilter(const int &filter, const wxString &numResource)
{
    CLabelFilter labelFilter;
    labelFilter.label = CLibResource::LoadStringFromResource(numResource,1);
    labelFilter.filter = filter;
    return labelFilter;
}

bool CFiltreData::IsPiccanteCompatible(const int &numFilter)
{
 	switch(numFilter)
	{
        case IDM_FILTER_KUWAHARA:
        case IDM_HDR_DEBLURRING:
        case IDM_BEST_EXPOSURE:
        case IDM_FILTER_BILATERAL2DS:
            return true;
    }   
    return false;
}


bool CFiltreData::IsOpenCLCompatible(const int &numFilter)
{
	switch(numFilter)
	{   
		case IDM_FILTER_KUWAHARA:
		case IDM_HDR_DEBLURRING:
		case IDM_BEST_EXPOSURE:
		case IDM_FILTER_BILATERAL2DS:
		case IDM_FILTRE_CLAHE:
        case IDM_HISTOGRAMLOG:
        case IDM_HISTOGRAMEQUALIZE:
        case IDM_HISTOGRAMNORMALIZE:
		case IDM_FILTRE_CLOUDS:
		case IDM_CROP:
		case IDM_REDEYE:
		case IDM_DECODE_RAW:
		case IDM_WAVE_EFFECT:
		case IDM_FILTRELENSFLARE:
		case IDM_FILTRE_MOTIONBLUR:
			return false;
			break;
	}
	return true;
}

bool CFiltreData::IsOpenCLPreviewCompatible(const int &numFilter)
{
	switch(numFilter)
	{
        case IDM_FILTER_KUWAHARA:
        case IDM_HDR_DEBLURRING:
        case IDM_FILTER_BILATERAL2DS:
        case IDM_BEST_EXPOSURE:
		case IDM_FILTRE_CLOUDS:
		case IDM_CROP:
		case IDM_REDEYE:
		case IDM_DECODE_RAW:
		case IDM_FILTRELENSFLARE:
		case IDM_WAVE_EFFECT:
		case IDM_FILTRE_MOTIONBLUR:
		case IDM_ROTATE_FREE:
		case IDM_FILTRE_CLAHE:
			return false;
			break;
	}
	return true;
}

bool CFiltreData::NeedPreview(const int &numFilter)
{
    switch(numFilter)
    {
		
		case IDM_CROP:
		case IDM_REDEYE:
        case IDM_FILTER_BM3D:
		case IDM_FILTRE_BILATERAL:
		case IDM_FILTRE_NLMEAN:
		case IDM_FILTRE_CLAHE:
		case IDM_DECODE_RAW:
		case IDM_WAVE_EFFECT:
        case IDM_FILTRELENSFLARE:
        case IDM_FILTRE_MOTIONBLUR:
        case IDM_ROTATE_FREE:
        case IDM_IMAGE_LIGHTCONTRAST:
        case ID_AJUSTEMENT_PHOTOFILTRE:
        case ID_AJUSTEMENT_POSTERISATION:
        case IDM_COLOR_BALANCE:
        case IDM_FILTRE_SWIRL:
		case IDM_SHARPENMASKING:
        case IDM_BEST_EXPOSURE:
        case IDM_FILTRE_CLOUDS:
		case IDM_FILTRE_FLOUGAUSSIEN:
		case IDM_FILTRE_FLOU:
        case IDM_AJUSTEMENT_SOLARISATION:
            return true;
		/*
		case IDM_FILTRELENSFLARE:
		case IDM_FILTRE_SWIRL:
		case IDM_WAVE_EFFECT:
		case IDM_CROP:
		case IDM_REDEYE:
			return true;
		*/
    }
    
    return false;
}

int CFiltreData::GetTypeEffect(const int &numFilter)
{

    
    switch(numFilter)
    {
        case IDM_FILTER_KUWAHARA:
        case IDM_HDR_DEBLURRING:
        case IDM_FILTER_BILATERAL2DS:
        case IDM_BEST_EXPOSURE:
            return HDR_EFFECT;
            //Convolution
        case IDM_FILTER_BM3D:
		case IDM_FILTRE_BILATERAL:
		case IDM_FILTRE_NLMEAN:	
        case IDM_FILTRE_MOTIONBLUR:
        case IDM_FILTRE_SHARPEN:
        case IDM_FILTRE_SHARPENSTRONG:
        case IDM_FILTREANTIBRUIT:
		case IDM_SHARPENMASKING:
        case IDM_FILTRE_FLOUGAUSSIEN:
        case IDM_FILTRE_FLOU:
        case IDM_FILTRE_SOFTEN:
            return CONVOLUTION_EFFECT;
            break;
            
		case IDM_FILTRE_CLAHE:
        case IDM_HISTOGRAMLOG:
        case IDM_HISTOGRAMEQUALIZE:
        case IDM_HISTOGRAMNORMALIZE:
            return HISTOGRAM_EFFECT;
            break;
            
            //Color Effect
        case IDM_COLOR_BALANCE:
        case ID_AJUSTEMENT_PHOTOFILTRE:
        case IDM_IMAGE_LIGHTCONTRAST:
        case IDM_NEGATIF:
        case IDM_BLACKANDWHITE:
        case IDM_IMAGE_SEPIA:
        case IDM_GREY_LEVEL:
            return COLOR_EFFECT;
            break;
            
            //Special Effect
        case IDM_FILTRE_CLOUDS:
        case ID_AJUSTEMENT_POSTERISATION:
        case IDM_FILTRE_SWIRL:
        case IDM_FILTRE_EDGE:
        case IDM_FILTRE_EMBOSS:
        case IDM_FILTRE_MOSAIQUE:
        case IDM_FILTRENOISE:
        case IDM_FILTRE_DILATE:
        case IDM_FILTRE_ERODE:
        case IDM_AJUSTEMENT_SOLARISATION:
            return SPECIAL_EFFECT;
            break;

		case IDM_ROTATE90:
		case IDM_ROTATE270:
		case IDM_FLIPVERTICAL:
		case IDM_FLIPHORIZONTAL:
		case IDM_ROTATE_FREE:
			return ROTATE_EFFECT;
			break;
        }
    return 0;
}

CEffectParameter * CFiltreData::GetEffectPointer(const int &numItem)
{
    switch (numItem)
    {
		case IDM_FILTRE_BILATERAL:
			return new CBilateralEffectParameter();
			break;
		case IDM_FILTRE_NLMEAN:
			return new CNlmeansEffectParameter();
			break;
		case IDM_FILTRE_CLAHE:
			return new CClaheEffectParameter();
			break;

		case IDM_FILTRE_FLOU:
			return new CBlurEffectParameter();
			break;

		case IDM_FILTRE_VIDEO:
			return new CVideoEffectParameter();
			break;

		case IDM_DECODE_RAW:
            return new CDecodeRawParameter();
            break;
        case IDM_AJUSTEMENT_SOLARISATION:
            return new CSolarisationEffectParameter();
            break;
        case IDM_FILTRE_MOTIONBLUR:
            return new CMotionBlurEffectParameter();
            break;
        case IDM_ROTATE_FREE:
            return new CFreeRotateEffectParameter();
            break;
        case IDM_IMAGE_LIGHTCONTRAST:
            return new CBrightAndContrastEffectParameter();
            break;
        case ID_AJUSTEMENT_PHOTOFILTRE:
            return new CPhotoFiltreEffectParameter();
            break;
        case ID_AJUSTEMENT_POSTERISATION:
            return new CPosterisationEffectParameter();
            break;
		case IDM_SHARPENMASKING:
			return new CSharpenMaskingEffectParameter();
			break;
            
        case IDM_FILTER_BM3D:
            return new CBm3dEffectParameter();
            break;
            
        case IDM_BEST_EXPOSURE:
            return new CBestExposureEffectParameter();
            break;
            
        case IDM_FILTRELENSFLARE:
            return new CLensFlareEffectParameter();
            break;
		case IDM_WAVE_EFFECT:
			return new CWaveEffectParameter();
			break;
        case IDM_COLOR_BALANCE:
            return new CRgbEffectParameter();
            break;
        case IDM_FILTRE_SWIRL:
            return new CSwirlEffectParameter();
            break;
        case IDM_FILTRE_CLOUDS:
            return new CCloudsEffectParameter();
            break;
		case IDM_FILTRE_FLOUGAUSSIEN:
			return new CGaussianBlurEffectParameter();
			break;

		default:
			return new CEffectParameter();
    }
    return nullptr;
}

bool CFiltreData::OnFiltreOk(const int &numFiltre)
{
    switch (numFiltre)
    {
        case IDM_FILTER_BM3D:
        case IDM_FILTRE_BILATERAL:
        case IDM_FILTRE_NLMEAN:
        case IDM_FILTRE_CLAHE:
        case IDM_FILTRE_FLOU:
        case IDM_DECODE_RAW:
        case IDM_AJUSTEMENT_SOLARISATION:
        case IDM_ROTATE_FREE:
        case IDM_IMAGE_LIGHTCONTRAST:
        case ID_AJUSTEMENT_PHOTOFILTRE:
        case ID_AJUSTEMENT_POSTERISATION:
        case IDM_COLOR_BALANCE:
        case IDM_FILTRE_SWIRL:
        case IDM_FILTRE_CLOUDS:
        case IDM_BEST_EXPOSURE:
        case IDM_SHARPENMASKING:
        case IDM_REDEYE:
        case IDM_CROP:
        case IDM_FILTRE_MOTIONBLUR:
        case IDM_WAVE_EFFECT:
        case IDM_FILTRELENSFLARE:
            {
                return true;
            }
            break;
    }    
    return false;
}

CEffectParameter * CFiltreData::GetDefaultEffectParameter(const int &numFilter)
{
    switch (numFilter)
    {
	case IDM_FILTRE_FLOUGAUSSIEN:
		{
			CGaussianBlurEffectParameter * gaussianBlur = new CGaussianBlurEffectParameter();
			gaussianBlur->radius = 5;
			gaussianBlur->boxSize = 3;
			return gaussianBlur;
			break;
		}

        case IDM_FILTRE_MOTIONBLUR:
        {
            CMotionBlurEffectParameter * motionBlur = new CMotionBlurEffectParameter();
            motionBlur->radius = 20;
            motionBlur->sigma = 5;
            motionBlur->angle = 40;
            return motionBlur;
            break;
        }
            //Color Effect
        case IDM_IMAGE_LIGHTCONTRAST:
        {
            CBrightAndContrastEffectParameter * brightness = new CBrightAndContrastEffectParameter();
            brightness->brightness = 20;
            brightness->contrast = 20;
            return brightness;
            break;
        }
            
        case ID_AJUSTEMENT_PHOTOFILTRE:
        {
            CPhotoFiltreEffectParameter * photoFiltre = new CPhotoFiltreEffectParameter();
            photoFiltre->red = 255;
            photoFiltre->green = 0;
            photoFiltre->blue = 0;
            photoFiltre->intensity = 30;
            return photoFiltre;
            break;
        }
            
        case IDM_COLOR_BALANCE:
        {
            CRgbEffectParameter * rgbFilter = new CRgbEffectParameter();
            rgbFilter->red = 120;
            rgbFilter->green = 120;
            rgbFilter->blue = 120;
            return rgbFilter;
            break;
        }

		case IDM_FILTRE_FLOU:
			{
				CBlurEffectParameter * blurEffect = new CBlurEffectParameter();
				blurEffect->size = 3;
				return blurEffect;
			}
			break;
            
        case IDM_AJUSTEMENT_SOLARISATION:
        {
            CSolarisationEffectParameter * solarisation = new CSolarisationEffectParameter();
            solarisation->threshold = 50;
            return solarisation;
            break;
        }
            
        case IDM_ROTATE_FREE:
        {
            CFreeRotateEffectParameter * rotate = new CFreeRotateEffectParameter();
            rotate->angle = 50;
            return rotate;
            break;
        }
            
        case ID_AJUSTEMENT_POSTERISATION:
        {
            CPosterisationEffectParameter * posterization = new CPosterisationEffectParameter();
            posterization->gamma = 20;
            posterization->level = 20;
            return posterization;
            break;
        }
            
        case IDM_FILTRE_SWIRL:
        {
            CSwirlEffectParameter * swirl = new CSwirlEffectParameter();
            swirl->angle = 20;
            swirl->radius = 20;
            return swirl;
            break;
        }

		case IDM_SHARPENMASKING:
		{
			CSharpenMaskingEffectParameter * sharpen = new CSharpenMaskingEffectParameter();
			sharpen->sharpness = 1.0f;
			return sharpen;
			break;
		}
        
        case IDM_FILTER_BM3D:
        {
            CBm3dEffectParameter * bm3dParameter = new CBm3dEffectParameter();
            bm3dParameter->fSize = 25;
            return bm3dParameter;
            break;
        }
        
        case IDM_BEST_EXPOSURE:
        {
            CBestExposureEffectParameter * exposure = new CBestExposureEffectParameter();
            exposure->tmo = 2.2f;
            return exposure;
            break;
        }
            

        case IDM_FILTRE_CLOUDS:
        {
            CCloudsEffectParameter * clouds = new CCloudsEffectParameter();
            clouds->colorFront = CRgbaquad(0, 0, 0);
            clouds->colorBack = CRgbaquad(255, 255, 255);
            clouds->amplitude = 1;
            clouds->frequence = 65;
            clouds->octave = 8;
			clouds->transparency = 50;
            return clouds;
            break;
        }

		case IDM_FILTRE_BILATERAL:
		{
			return new CBilateralEffectParameter();
		}
		break;

		case IDM_FILTRE_NLMEAN:
		{
			return new CNlmeansEffectParameter();
		}
		break;

		case IDM_FILTRE_CLAHE:
		{
			return new CClaheEffectParameter();
		}
		break;

		default:
			return new CEffectParameter();

    }
    return nullptr;
}

int CFiltreData::TypeApplyFilter(const int &numItem)
{
    switch (numItem)
    {
        case IDM_CROP:
        case IDM_REDEYE:
        case IDM_FILTER_BM3D:
        case IDM_FILTRE_BILATERAL:
        case IDM_FILTRE_NLMEAN:
        case IDM_FILTRE_CLAHE:
        case IDM_FILTRE_FLOU:
        case IDM_DECODE_RAW:
        case IDM_BEST_EXPOSURE:
        case IDM_SHARPENMASKING:
        case IDM_FILTRE_FLOUGAUSSIEN:
        case IDM_FILTRE_MOTIONBLUR:
        case IDM_ROTATE_FREE:
        case IDM_IMAGE_LIGHTCONTRAST:
        case ID_AJUSTEMENT_PHOTOFILTRE:
        case ID_AJUSTEMENT_POSTERISATION:
        case IDM_WAVE_EFFECT:
        case IDM_FILTRELENSFLARE:
        case IDM_COLOR_BALANCE:
        case IDM_FILTRE_SWIRL:
        case IDM_FILTRE_CLOUDS:
        case IDM_AJUSTEMENT_SOLARISATION:
            return 2;
    }
    return 3;
}

wxString CFiltreData::GetFilterLabel(const int &numFilter)
{
    if(labelFilterList.size() == 0)
        InitFilterListLabel();
    
    for(CLabelFilter labelFilter : labelFilterList)
    {
        if(labelFilter.filter == numFilter)
            return labelFilter.label;
    }
    return "";
}



void CFiltreData::InitFilterListLabel()
{
    labelFilterList.push_back(CLabelFilter::CreateLabelFilter(IDM_FILTRE_SOFTEN, "LBLfilterSoften"));
    labelFilterList.push_back(CLabelFilter::CreateLabelFilter(IDM_FILTRE_FLOU, "LBLfilterBlur"));
    labelFilterList.push_back(CLabelFilter::CreateLabelFilter(IDM_AJUSTEMENT_SOLARISATION, "LBLfilterSolarize"));
    labelFilterList.push_back(CLabelFilter::CreateLabelFilter(IDM_FILTRE_FLOUGAUSSIEN, "LBLfilterGaussian"));
    labelFilterList.push_back(CLabelFilter::CreateLabelFilter(IDM_FILTREANTIBRUIT, "LBLfilterMedian"));
    labelFilterList.push_back(CLabelFilter::CreateLabelFilter(IDM_FILTRE_MOTIONBLUR, "LBLfilterMotion"));
    labelFilterList.push_back(CLabelFilter::CreateLabelFilter(IDM_ROTATE_FREE, "LBLfilterRotate"));
    labelFilterList.push_back(CLabelFilter::CreateLabelFilter(IDM_IMAGE_LIGHTCONTRAST, "LBLfilterLight"));
    labelFilterList.push_back(CLabelFilter::CreateLabelFilter(ID_AJUSTEMENT_PHOTOFILTRE, "LBLfilterPhoto"));
    labelFilterList.push_back(CLabelFilter::CreateLabelFilter(ID_AJUSTEMENT_POSTERISATION, "LBLfilterPosterisation"));
    labelFilterList.push_back(CLabelFilter::CreateLabelFilter(IDM_COLOR_BALANCE, "LBLfilterColor"));
    labelFilterList.push_back(CLabelFilter::CreateLabelFilter(IDM_FILTRE_SWIRL, "LBLfilterSwirl"));
    labelFilterList.push_back(CLabelFilter::CreateLabelFilter(IDM_FILTRE_CLOUDS, "LBLfilterClouds"));
    labelFilterList.push_back(CLabelFilter::CreateLabelFilter(IDM_FILTRE_ERODE, "LBLfilterErode"));
    labelFilterList.push_back(CLabelFilter::CreateLabelFilter(IDM_FILTRE_DILATE, "LBLfilterDilate"));
    labelFilterList.push_back(CLabelFilter::CreateLabelFilter(IDM_FILTRE_SHARPEN, "LBLfilterSharpen"));
    labelFilterList.push_back(CLabelFilter::CreateLabelFilter(IDM_FILTRE_SHARPENSTRONG, "LBLfilterSharpenStrong"));
    labelFilterList.push_back(CLabelFilter::CreateLabelFilter(IDM_FILTRENOISE, "LBLfilterNoise"));
    labelFilterList.push_back(CLabelFilter::CreateLabelFilter(IDM_FILTRE_MOSAIQUE, "LBLfilterMosaic"));
    labelFilterList.push_back(CLabelFilter::CreateLabelFilter(IDM_FILTRE_EMBOSS, "LBLfilterEmboss"));
    labelFilterList.push_back(CLabelFilter::CreateLabelFilter(IDM_GREY_LEVEL, "LBLfilterGrey"));
    labelFilterList.push_back(CLabelFilter::CreateLabelFilter(IDM_IMAGE_SEPIA, "LBLfilterSepia"));
    labelFilterList.push_back(CLabelFilter::CreateLabelFilter(IDM_BLACKANDWHITE, "LBLfilterBlack"));
    labelFilterList.push_back(CLabelFilter::CreateLabelFilter(IDM_FILTRE_EDGE, "LBLfilterEdge"));
    labelFilterList.push_back(CLabelFilter::CreateLabelFilter(IDM_NEGATIF, "LBLfilterNegatif"));
    labelFilterList.push_back(CLabelFilter::CreateLabelFilter(IDM_FILTRELENSFLARE, "LBLfilterLensFlare"));
    labelFilterList.push_back(CLabelFilter::CreateLabelFilter(IDM_REDEYE, "LBLfilterRedEye"));
    labelFilterList.push_back(CLabelFilter::CreateLabelFilter(IDM_CROP, "LBLCROP"));
    labelFilterList.push_back(CLabelFilter::CreateLabelFilter(IDM_HISTOGRAMNORMALIZE, "LBLHistogramNormalize"));
    labelFilterList.push_back(CLabelFilter::CreateLabelFilter(IDM_HISTOGRAMEQUALIZE, "LBLHistogramEqualize"));
    labelFilterList.push_back(CLabelFilter::CreateLabelFilter(IDM_HISTOGRAMLOG, "LBLHistogramLog"));
	labelFilterList.push_back(CLabelFilter::CreateLabelFilter(IDM_SHARPENMASKING, "LBLSharpenMasking"));
	labelFilterList.push_back(CLabelFilter::CreateLabelFilter(IDM_DECODE_RAW, "LBLBLACKROOM"));
	labelFilterList.push_back(CLabelFilter::CreateLabelFilter(IDM_FILTRE_VIDEO,"LBLVIDEOEFFECT"));
	labelFilterList.push_back(CLabelFilter::CreateLabelFilter(IDM_FILTRE_NLMEAN,"LBLNLMEANS"));
	labelFilterList.push_back(CLabelFilter::CreateLabelFilter(IDM_FILTRE_BILATERAL,"LBLBILATERALDENOISING"));
	labelFilterList.push_back(CLabelFilter::CreateLabelFilter(IDM_FILTRE_CLAHE,"LBLENHANCELOCALCONTRAST"));
	labelFilterList.push_back(CLabelFilter::CreateLabelFilter(IDM_ROTATE90, "LBLROTATE90"));
	labelFilterList.push_back(CLabelFilter::CreateLabelFilter(IDM_ROTATE270, "LBLROTATE270"));
	labelFilterList.push_back(CLabelFilter::CreateLabelFilter(IDM_FLIPVERTICAL, "LBLFLIPV"));
	labelFilterList.push_back(CLabelFilter::CreateLabelFilter(IDM_FLIPHORIZONTAL, "LBLFLIPH"));
	labelFilterList.push_back(CLabelFilter::CreateLabelFilter(IDM_WAVE_EFFECT, "LBLWAVEFILTER"));
    labelFilterList.push_back(CLabelFilter::CreateLabelFilter(IDM_BEST_EXPOSURE, "LBLHDRBESTEXPOSUREFILTER"));
    //labelFilterList.push_back(CLabelFilter::CreateLabelFilter(IDM_HDR_DEBLURRING, "LBLHDRDEBLURRINGFILTER"));
    labelFilterList.push_back(CLabelFilter::CreateLabelFilter(IDM_FILTER_KUWAHARA, "LBLFILTERKUWAHARA"));
    //labelFilterList.push_back(CLabelFilter::CreateLabelFilter(IDM_FILTER_BILATERAL2DS, "LBLFILTERBILATERAL"));
    labelFilterList.push_back(CLabelFilter::CreateLabelFilter(IDM_FILTER_BM3D, "LBLFILTREBM3D"));
    
}
