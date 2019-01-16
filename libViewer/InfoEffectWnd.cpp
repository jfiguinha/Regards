//
//  InfoEffectWnd.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 02/10/2015.
//  Copyright © 2015 figuinha jacques. All rights reserved.
//

#include "InfoEffectWnd.h"
using namespace Regards::Viewer;

CInfoEffectWnd::CInfoEffectWnd(wxWindow* parent, wxWindowID id, const CThemeScrollBar & themeScroll, const CThemeTree & themeTree)
: CWindowMain("CInfoEffectWnd",parent, id)
{
	historyEffectScroll = nullptr;
	treeHistoryEffect = nullptr;
	historyEffectOld = nullptr;
    historyEffectScroll = new CScrollbarWnd(this, wxID_ANY);
    treeHistoryEffect = new CTreeWindow(historyEffectScroll, wxID_ANY, themeTree);
    historyEffectScroll->SetCentralWindow(treeHistoryEffect, themeScroll);

}

CInfoEffectWnd::~CInfoEffectWnd(void)
{
    if(treeHistoryEffect != nullptr)
        delete(treeHistoryEffect);
    
    if(historyEffectScroll != nullptr)
        delete(historyEffectScroll);
    
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
        CInfoEffect * historyEffect = new CInfoEffect(bitmapViewer, treeHistoryEffect->GetTheme(), treeHistoryEffect, modificationManager);
        historyEffect->Init(bitmap, filename, historyLibelle);
        treeHistoryEffect->SetTreeControl(historyEffect);
        delete(historyEffectOld);
        historyEffectOld = historyEffect;
    }
}

void CInfoEffectWnd::UpdateScreenRatio()
{
    if(historyEffectScroll != nullptr)
        historyEffectScroll->UpdateScreenRatio();
    
    if(treeHistoryEffect != nullptr)
        treeHistoryEffect->UpdateScreenRatio();
    
    if(historyEffectOld != nullptr)
        historyEffectOld->UpdateScreenRatio();

}

void CInfoEffectWnd::Resize()
{
	if(historyEffectScroll != nullptr)
	{
		historyEffectScroll->SetSize(0,0,GetWindowWidth(),GetWindowHeight());
		//historyEffectScroll->SendSizeEvent();
		treeHistoryEffect->SetSize(0,0,GetWindowWidth(),GetWindowHeight());
		//treeHistoryEffect->SendSizeEvent();
	}
}