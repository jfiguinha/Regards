//
//  FilterData.hpp
//  Regards.libFiltreInterface
//
//  Created by figuinha jacques on 07/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <vector>
using namespace std;

class CEffectParameter;

#define SPECIAL_EFFECT 1
#define COLOR_EFFECT 2
#define CONVOLUTION_EFFECT 3
#define HISTOGRAM_EFFECT 4

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
#define IDM_FILTRE_CLOUDS 14
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


class CFiltreData
{
public:
    CFiltreData();
    ~CFiltreData();
    
    static wxString GetFilterLabel(const int &numFilter);
    static CEffectParameter * GetDefaultEffectParameter(const int &numFilter);
    static int GetTypeEffect(const int &numFilter);
    static bool NeedPreview(const int &numFilter);
    
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

