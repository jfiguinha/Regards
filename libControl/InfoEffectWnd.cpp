//
//  InfoEffectWnd.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 02/10/2015.
//  Copyright © 2015 figuinha jacques. All rights reserved.
//
#include <header.h>
#include "InfoEffectWnd.h"
#include <TreeWindow.h>
#include <ImageLoadingFormat.h>
using namespace Regards::Control;
using namespace Regards::Window;

CInfoEffectWnd::CInfoEffectWnd(wxWindow* parent, const wxWindowID id, const CThemeScrollBar& themeScroll,
                               const CThemeTree& themeTree, int bitmap_window_id)
	: CTreeWithScrollbar("CInfoEffectWnd", parent, id, themeScroll, themeTree)
{
	this->bitmapWindowId = bitmap_window_id;
	historyEffectOld = nullptr;
}

CInfoEffectWnd::~CInfoEffectWnd(void)
{
	if (historyEffectOld != nullptr)
		delete(historyEffectOld);
}

void CInfoEffectWnd::AddModification(CImageLoadingFormat* bitmap, const wxString& libelle)
{
	if (historyEffectOld != nullptr)
		historyEffectOld->AddModification(bitmap, libelle);
}

void CInfoEffectWnd::HistoryUpdate(CImageLoadingFormat* bitmap, const wxString& filename, const wxString& historyLibelle,
                                   CModificationManager* modificationManager)
{
	if (historyEffectOld == nullptr || historyEffectOld->GetFilename() != filename)
	{
		auto historyEffect = new CInfoEffect(treeWindow, modificationManager, bitmapWindowId);
		historyEffect->Init(bitmap, filename, historyLibelle);
		treeWindow->SetTreeControl(historyEffect);
		delete(historyEffectOld);
		historyEffectOld = historyEffect;
	}
}
