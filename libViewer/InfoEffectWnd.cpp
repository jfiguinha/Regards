//
//  InfoEffectWnd.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 02/10/2015.
//  Copyright © 2015 figuinha jacques. All rights reserved.
//
#include <header.h>
#include "InfoEffectWnd.h"
using namespace Regards::Viewer;

CInfoEffectWnd::CInfoEffectWnd(wxWindow* parent, wxWindowID id, const CThemeScrollBar & themeScroll, const CThemeTree & themeTree)
	: CTreeWithScrollbar("CInfoEffectWnd", parent, id, themeScroll, themeTree)
{
	historyEffectOld = nullptr;
}

CInfoEffectWnd::~CInfoEffectWnd(void)
{
    if(historyEffectOld != nullptr)
        delete(historyEffectOld);
 
}

void CInfoEffectWnd::AddModification(CRegardsBitmap * bitmap,const wxString & libelle)
{
    if(historyEffectOld != nullptr)
        historyEffectOld->AddModification(bitmap, libelle);
}

void CInfoEffectWnd::HistoryUpdate(CRegardsBitmap * bitmap,const wxString & filename, const wxString & historyLibelle, CBitmapWndViewer * bitmapViewer, CModificationManager * modificationManager)
{
    if (historyEffectOld == nullptr || historyEffectOld->GetFilename() != filename)
    {
        CInfoEffect * historyEffect = new CInfoEffect(bitmapViewer, treeWindow, modificationManager);
        historyEffect->Init(bitmap, filename, historyLibelle);
		treeWindow->SetTreeControl(historyEffect);
        delete(historyEffectOld);
        historyEffectOld = historyEffect;
    }
}