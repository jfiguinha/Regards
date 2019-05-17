#include <header.h>
//
//  ThumbnailViewerEffectWnd.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 02/10/2015.
//  Copyright © 2015 figuinha jacques. All rights reserved.
//
#include "ViewerParam.h"
#include "ViewerParamInit.h"
#include "ThumbnailViewerEffectWnd.h"
using namespace Regards::Control;
using namespace Regards::Viewer;

CThumbnailViewerEffectWnd::CThumbnailViewerEffectWnd(wxWindow* parent, wxWindowID id, 
	const CThemeScrollBar & themeScroll, const CThemeThumbnail & themeThumbnail)
: CWindowMain("CThumbnailViewerEffectWnd",parent, id)
{
    thumbnailEffectScroll = nullptr;
    thumbnailEffect = nullptr;
	bool checkValidity = false;
    thumbnailEffectScroll = new CScrollbarWnd(this, wxID_ANY);
	CViewerParam * config = CViewerParamInit::getInstance();
	if (config != nullptr)
		checkValidity = config->GetCheckThumbnailValidity();

	thumbnailEffect = new CThumbnailViewerEffect(thumbnailEffectScroll, wxID_ANY,  themeThumbnail, checkValidity);
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
        thumbnailEffectScroll->SetSize(0, 0, GetWindowWidth(), GetWindowHeight());
}

wxString CThumbnailViewerEffectWnd::GetFilename()
{
    if(thumbnailEffect != nullptr)
        return thumbnailEffect->GetFilename();
    return "";
}

void CThumbnailViewerEffectWnd::SetFile(const wxString & filename)
{
    if(thumbnailEffect != nullptr)
        return thumbnailEffect->SetFile(filename);
}