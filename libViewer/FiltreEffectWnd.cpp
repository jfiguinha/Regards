//
//  FiltreEffectWnd.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 02/10/2015.
//  Copyright © 2015 figuinha jacques. All rights reserved.
//

#include "FiltreEffectWnd.h"
#include <FiltreEffet.h>
#include "InfoEffectWnd.h"
#include "CloudsEffectParameter.h"
#include "BrightAndContrastEffectParameter.h"
#include "FreeRotateEffectParameter.h"
#include "MotionBlurEffectParameter.h"
#include "LensFlareEffectParameter.h"
#include "SolarisationEffectParameter.h"
#include "PosterisationEffectParameter.h"
#include "PhotoFiltreEffectParameter.h"
#include "RgbEffectParameter.h"
#include "SwirlEffectParameter.h"
#include <libResource.h>
#include <BitmapWndViewer.h>
#include <ShowBitmap.h>
#include "PreviewWnd.h"
#if defined(__WXMSW__)
#include "../include/window_id.h"
#include "../include/config_id.h"
#else
#include <window_id.h>
#include <config_id.h>
#endif
#include <effect.h>
#include "PanelInfosWnd.h"
#include <FilterData.h>
using namespace Regards::Viewer;

CFiltreEffectScrollWnd::CFiltreEffectScrollWnd(wxWindow* parent, wxWindowID id, const CThemeScrollBar & themeScroll, const CThemeTree & themeTree)
: CWindowMain(parent, id)
{
    filtreEffectScroll = new CScrollbarWnd(this, wxID_ANY);
    treeFiltreEffect = new CTreeWindow(filtreEffectScroll, wxID_ANY, themeTree);
    filtreEffectScroll->SetCentralWindow(treeFiltreEffect, themeScroll);
}

CFiltreEffectScrollWnd::~CFiltreEffectScrollWnd(void)
{
    if(treeFiltreEffect != nullptr)
        delete(treeFiltreEffect);
    
    if(filtreEffectScroll != nullptr)
        delete(filtreEffectScroll);
    
    if(filtreEffectOld != nullptr)
        delete(filtreEffectOld);
    
    if(effectParameter != nullptr)
       delete(effectParameter);
}
void CFiltreEffectScrollWnd::UpdateScreenRatio()
{
    if(filtreEffectScroll != nullptr)
        filtreEffectScroll->UpdateScreenRatio();
    
    if(treeFiltreEffect != nullptr)
        treeFiltreEffect->UpdateScreenRatio();
    
    if(filtreEffectOld != nullptr)
        filtreEffectOld->UpdateScreenRatio();
    
}

void CFiltreEffectScrollWnd::Resize()
{
    filtreEffectScroll->SetSize(0,0,width,height);
}

void CFiltreEffectScrollWnd::OnFiltreOk(const int &numFiltre, CInfoEffectWnd * historyEffectWnd)
{
    CBitmapWndViewer * bitmapViewer = (CBitmapWndViewer *)this->FindWindowById(BITMAPWINDOWVIEWERID);
    if (bitmapViewer != nullptr)
    {
        switch (numFiltre)
        {
            case IDM_AJUSTEMENT_SOLARISATION:
            case IDM_ROTATE_FREE:
            case IDM_IMAGE_LIGHTCONTRAST:
            case ID_AJUSTEMENT_PHOTOFILTRE:
            case ID_AJUSTEMENT_POSTERISATION:
            case IDM_COLOR_BALANCE:
            case IDM_FILTRE_SWIRL:
            case IDM_FILTRE_CLOUDS:
            case IDM_REDEYE:
            case IDM_CROP:
            case IDM_FILTRE_MOTIONBLUR:
            case IDM_FILTRELENSFLARE:
                bitmapViewer->SetBitmapEffect(numFiltre, effectParameter);
                historyEffectWnd->AddModification(bitmapViewer->GetBitmap(), CFiltreData::GetFilterLabel(numFiltre));
                break;
        }
    }
}

CEffectParameter * CFiltreEffectScrollWnd::GetEffectPointer(const int &numItem)
{
    switch (numItem)
    {
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
        case IDM_FILTRELENSFLARE:
            return new CLensFlareEffectParameter();
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
    }
    return nullptr;
}

void CFiltreEffectScrollWnd::ApplyEffect(const int &numItem, CInfoEffectWnd * historyEffectWnd, CPanelInfosWnd * panelInfos)
{

    CPreviewWnd * previewWindow = (CPreviewWnd *)this->FindWindowById(PREVIEWVIEWERID);
    CShowBitmap * showBitmap = (CShowBitmap *)this->FindWindowById(SHOWBITMAPVIEWERID);
    
    if (showBitmap != nullptr)
    {
        CBitmapWndViewer * bitmapViewer = showBitmap->GetBitmapViewer();
        
        CFiltreEffect * filtreEffect = new CFiltreEffect(bitmapViewer, treeFiltreEffect->GetTheme(), treeFiltreEffect);
        
        if (bitmapViewer != nullptr)
        {
            switch (numItem)
            {
                case IDM_CROP:
                case IDM_REDEYE:
                {
                    if (previewWindow != nullptr)
                        previewWindow->ShowValidationToolbar(true, numItem);
                    
                    bitmapViewer->SetTool(numItem);
                    bitmapViewer->SetBitmapPreviewEffect(numItem);

                }
                break;
                   
                case IDM_FILTRE_MOTIONBLUR:
                case IDM_ROTATE_FREE:
                case IDM_IMAGE_LIGHTCONTRAST:
                case ID_AJUSTEMENT_PHOTOFILTRE:
                case ID_AJUSTEMENT_POSTERISATION:
                case IDM_FILTRELENSFLARE:
                case IDM_COLOR_BALANCE:
                case IDM_FILTRE_SWIRL:
                case IDM_FILTRE_CLOUDS:
                case IDM_AJUSTEMENT_SOLARISATION:
                {
                    if(effectParameter != nullptr)
                        delete(effectParameter);
                    effectParameter = GetEffectPointer(numItem);
                    bitmapViewer->SetBitmapPreviewEffect(numItem, effectParameter);
                    filtreEffect->Init(effectParameter, bitmapViewer->GetBitmap(), numItem);
                    if (previewWindow != nullptr)
                        previewWindow->ShowValidationToolbar(true, numItem);
                    panelInfos->ShowFiltre(CFiltreData::GetFilterLabel(numItem));
                    treeFiltreEffect->SetTreeControl(filtreEffect);
                    delete(filtreEffectOld);
                    filtreEffectOld = filtreEffect;
                    break;
                }

                default:
                    bitmapViewer->SetBitmapEffect(numItem, nullptr);
                    historyEffectWnd->AddModification(bitmapViewer->GetBitmap(), CFiltreData::GetFilterLabel(numItem));
                    break;



            }
        }
    }
}