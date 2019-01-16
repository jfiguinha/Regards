//
//  FilterData.hpp
//  Regards.libFiltreInterface
//
//  Created by figuinha jacques on 07/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#pragma once
#include <FilterWindowParam.h>
class CFiltreEffet;
class CEffectParameter;

#define SPECIAL_EFFECT 1
#define COLOR_EFFECT 2
#define CONVOLUTION_EFFECT 3
#define HISTOGRAM_EFFECT 4
#define ROTATE_EFFECT 5
#define HDR_EFFECT 6

#define FILTER_START 1
#define IDM_FILTRE_SOFTEN 1
#define IDM_FILTRE_FLOU 2
#define IDM_AJUSTEMENT_SOLARISATION 3
#define IDM_FILTRE_FLOUGAUSSIEN 4
#define IDM_FILTREANTIBRUIT 5
#define IDM_FILTRE_MOTIONBLUR 6
#define IDM_ROTATE_FREE 7
#define IDM_IMAGE_LIGHTCONTRAST 8
#define ID_AJUSTEMENT_PHOTOFILTRE 9
#define ID_AJUSTEMENT_POSTERISATION 10
#define IDM_FILTRELENSFLARE 11
#define IDM_COLOR_BALANCE 12
#define IDM_FILTRE_SWIRL 13
#define IDM_SHARPENMASKING 14
#define IDM_FILTRE_ERODE 15
#define IDM_FILTRE_DILATE 16
#define IDM_FILTRE_SHARPEN 17
#define IDM_FILTRE_SHARPENSTRONG 18
#define IDM_FILTRENOISE 19
#define IDM_FILTRE_MOSAIQUE 20
#define IDM_FILTRE_EMBOSS 21
#define IDM_GREY_LEVEL 22
#define IDM_IMAGE_SEPIA 23
#define IDM_BLACKANDWHITE 24
#define IDM_FILTRE_EDGE 25
#define IDM_NEGATIF 26
#define IDM_REDEYE 27
#define IDM_HISTOGRAMNORMALIZE 28
#define IDM_HISTOGRAMEQUALIZE 29
#define IDM_HISTOGRAMLOG 30
#define IDM_CROP 31
#define IDM_FILTRE_CLOUDS 32
#define IDM_FILTRE_BILATERAL 33
#define IDM_FILTRE_NLMEAN 34
#define IDM_FILTRE_CLAHE 35
#define IDM_ROTATE90  36
#define IDM_ROTATE270  37
#define IDM_FLIPVERTICAL  38
#define IDM_FLIPHORIZONTAL  39
#define IDM_WAVE_EFFECT 40
#define IDM_BEST_EXPOSURE 41
#define IDM_FILTER_KUWAHARA 42
#define IDM_FILTER_BILATERAL2DS 43
#define IDM_HDR_DEBLURRING 44
#define FILTER_END 43

#define IDM_DECODE_RAW 100
#define IDM_FILTRE_VIDEO 200
#define IDM_FILTRE_AUDIOVIDEO 201


#define BOXFILTER 1001
#define BILINEARFILTER 1002
#define GAUSSIANFILTER 1003
#define HAMMINGFILTER 1004
#define CUBICFILTER 1005
#define BLACKMANFILTER 1006
#define QUADRATICFILTER 1007
#define MITCHELLFILTER 1008
#define TRIANGLEFILTER 1009
#define SINCFILTER 1010
#define BESSELFILTER 1011
#define BLACKMANBESSELFILTER 1012
#define BLACKMANSINCFILTER 1013
#define LANCZOSFILTER 1014
#define HERMITEFILTER 1015
#define HANNINGFILTER 1016
#define CATROMFILTER 1017


class CFiltreData
{
public:
    CFiltreData();
    ~CFiltreData();
    
    static wxString GetFilterLabel(const int &numFilter);
    static CEffectParameter * GetDefaultEffectParameter(const int &numFilter);
    static CFilterWindowParam * CreateEffectPointer(const int &numFilter);
    static CEffectParameter * GetEffectPointer(const int &numItem);
    static int GetTypeEffect(const int &numFilter);
    static bool NeedPreview(const int &numFilter);
    static bool IsOpenCLPreviewCompatible(const int &numFilter);
	static bool IsOpenCLCompatible(const int &numFilter);
    static bool IsPiccanteCompatible(const int &numFilter);
    static bool OnFiltreOk(const int &numFiltre);
    static int TypeApplyFilter(const int &numFiltre);
    static int RenderEffect(const int &numEffect, CFiltreEffet * filtreEffet, CEffectParameter * effectParameter, const bool &preview = false);
private:
    
    class CLabelFilter
    {
    public:
        int filter;
        wxString label;
        static CLabelFilter CreateLabelFilter(const int &filter, const wxString &numResource);
    };
    
    
    static void InitFilterListLabel();
    static vector<CLabelFilter> labelFilterList;
};

