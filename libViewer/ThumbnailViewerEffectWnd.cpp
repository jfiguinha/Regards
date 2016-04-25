//
//  ThumbnailViewerEffectWnd.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 02/10/2015.
//  Copyright © 2015 figuinha jacques. All rights reserved.
//

#include "ThumbnailViewerEffectWnd.h"
using namespace Regards::Control;
using namespace Regards::Viewer;

CThumbnailViewerEffectWnd::CThumbnailViewerEffectWnd(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusBarInterface, const CThemeScrollBar & themeScroll, const CThemeThumbnail & themeThumbnail)
: CWindowMain(parent, id)
{
    /*
    CThemeScrollBar themeScroll;
    viewerTheme->GetScrollThumbnailEffectTheme(&themeScroll);
    thumbnailEffectScroll = new CScrollbarWnd(this, wxID_ANY);
    
    CThemeThumbnail themeThumbnail;
    viewerTheme->GetTreeThumbnailViewerEffectTheme(&themeThumbnail);
    thumbnailEffect = new CThumbnailViewerEffect(thumbnailEffectScroll, wxID_ANY, statusBarInterface, themeThumbnail);
    
    thumbnailEffectScroll->SetCentralWindow(thumbnailEffect, themeScroll);
    
    thumbnailEffectScroll->Show(false);
     */
    
    thumbnailEffectScroll = new CScrollbarWnd(this, wxID_ANY);
    thumbnailEffect = new CThumbnailViewerEffect(thumbnailEffectScroll, wxID_ANY, statusBarInterface, themeThumbnail);
    thumbnailEffectScroll->SetCentralWindow(thumbnailEffect, themeScroll);
}

CThumbnailViewerEffectWnd::~CThumbnailViewerEffectWnd(void)
{
    delete(thumbnailEffect);
    delete(thumbnailEffectScroll);
}

void CThumbnailViewerEffectWnd::UpdateScreenRatio()
{
    if(thumbnailEffectScroll != nullptr)
        thumbnailEffectScroll->UpdateScreenRatio();
   
    if(thumbnailEffect != nullptr)
        thumbnailEffect->UpdateScreenRatio();
}

void CThumbnailViewerEffectWnd::Resize()
{
    if(thumbnailEffectScroll != nullptr)
        thumbnailEffectScroll->SetSize(0, 0, width, height);
}

wxString CThumbnailViewerEffectWnd::GetFilename()
{
    if(thumbnailEffect != nullptr)
        return thumbnailEffect->GetFilename();
    return "";
}

void CThumbnailViewerEffectWnd::SetFile(const wxString & filename, CRegardsBitmap * bitmap)
{
    if(thumbnailEffect != nullptr)
        return thumbnailEffect->SetFile(filename, bitmap);
}