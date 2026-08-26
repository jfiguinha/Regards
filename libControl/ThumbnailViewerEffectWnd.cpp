#include <header.h>
//
//  ThumbnailViewerEffectWnd.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 02/10/2015.
//  Copyright © 2015 figuinha jacques. All rights reserved.

#include "ThumbnailViewerEffectWnd.h"
#include <libPicture.h>
using namespace Regards::Control;
using namespace Regards::Picture;

CThumbnailViewerEffectWnd::CThumbnailViewerEffectWnd(wxWindow* parent, wxWindowID id,
                                                     const CThemeScrollBar& themeScroll,
                                                     const CThemeThumbnail& themeThumbnail, int panelInfosId,
                                                     bool checkValidity)
	: CWindowMain("CThumbnailViewerEffectWnd", parent, id)
{
	thumbnailEffectScroll = nullptr;
	thumbnailEffect = nullptr;

	thumbnailEffect = std::make_unique<CThumbnailViewerEffect>(this, wxID_ANY, themeThumbnail, checkValidity, panelInfosId);
	thumbnailEffectScroll = std::make_unique<CScrollbarWnd>(this, thumbnailEffect.get(), wxID_ANY);
}

void CThumbnailViewerEffectWnd::UpdateScreenRatio()
{
	if (thumbnailEffectScroll != nullptr)
		thumbnailEffectScroll->UpdateScreenRatio();

	if (thumbnailEffect != nullptr)
		thumbnailEffect->UpdateScreenRatio();
}

void CThumbnailViewerEffectWnd::Resize()
{
	if (thumbnailEffectScroll != nullptr)
		thumbnailEffectScroll->SetSize(0, 0, GetWindowWidth(), GetWindowHeight());
	thumbnailEffect->UpdateScroll();
}

wxString CThumbnailViewerEffectWnd::GetFilename()
{
	if (thumbnailEffect != nullptr)
		return thumbnailEffect->GetFilename();
	return "";
}

void CThumbnailViewerEffectWnd::SetFile(const wxString& filename)
{
	if (thumbnailEffect != nullptr)
	{
		CLibPicture libPicture;
		CImageLoadingFormat* load = libPicture.LoadThumbnail(filename);
		thumbnailEffect->SetFile(filename, load);
		//thumbnailEffect->UpdateScroll();
	}
}
