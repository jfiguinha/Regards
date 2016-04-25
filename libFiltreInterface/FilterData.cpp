//
//  FilterData.cpp
//  Regards.libFiltreInterface
//
//  Created by figuinha jacques on 07/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#include "FilterData.h"
#include <EffectParameter.h>
#include <LibResource.h>
#include <effect.h>
#include <RgbEffectParameter.h>
#include <BrightAndContrastEffectParameter.h>
#include <CloudsEffectParameter.h>
#include <FreeRotateEffectParameter.h>
#include <MotionBlurEffectParameter.h>
#include <PhotoFiltreEffectParameter.h>
#include <PosterisationEffectParameter.h>
#include <RgbEffectParameter.h>
#include <SolarisationEffectParameter.h>
#include <LensFlareEffectParameter.h>
#include <SwirlEffectParameter.h>

vector<CFiltreData::CLabelFilter> CFiltreData::labelFilterList;

CFiltreData::CFiltreData(){}
CFiltreData::~CFiltreData(){}

CFiltreData::CLabelFilter CFiltreData::CLabelFilter::CreateLabelFilter(const int &filter, const wxString &numResource)
{
    CLabelFilter labelFilter;
    labelFilter.label = CLibResource::LoadStringFromResource(numResource,1);
    labelFilter.filter = filter;
    return labelFilter;
}

bool CFiltreData::NeedPreview(const int &numFilter)
{
    switch(numFilter)
    {
        case IDM_FILTRELENSFLARE:
        case IDM_FILTRE_MOTIONBLUR:
        case IDM_ROTATE_FREE:
        case IDM_IMAGE_LIGHTCONTRAST:
        case ID_AJUSTEMENT_PHOTOFILTRE:
        case ID_AJUSTEMENT_POSTERISATION:
        case IDM_COLOR_BALANCE:
        case IDM_FILTRE_SWIRL:
        case IDM_FILTRE_CLOUDS:
        case IDM_AJUSTEMENT_SOLARISATION:
            return true;
    }
    
    return false;
}

int CFiltreData::GetTypeEffect(const int &numFilter)
{

    
    switch(numFilter)
    {
            //Convolution
        case IDM_FILTRE_MOTIONBLUR:
        case IDM_FILTRE_SHARPEN:
        case IDM_FILTRE_SHARPENSTRONG:
        case IDM_FILTREANTIBRUIT:
        case IDM_FILTRE_FLOUGAUSSIEN:
        case IDM_FILTRE_FLOU:
        case IDM_FILTRE_SOFTEN:
            return CONVOLUTION_EFFECT;
            break;
            
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
        case IDM_ROTATE_FREE:
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
        }
    return 0;
}

CEffectParameter * CFiltreData::GetDefaultEffectParameter(const int &numFilter)
{
    switch (numFilter)
    {
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
            

        case IDM_FILTRE_CLOUDS:
        {
            CCloudsEffectParameter * clouds = new CCloudsEffectParameter();
            clouds->colorFront = CRgbaquad(0, 0, 0);
            clouds->colorBack = CRgbaquad(255, 255, 255);
            clouds->amplitude = 3;
            clouds->frequence = 5;
            clouds->octave = 8;
            return clouds;
            break;
        }
            

    }
    return nullptr;
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
}