//
//  InfoEffectWnd.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 02/10/2015.
//  Copyright © 2015 figuinha jacques. All rights reserved.
//
#include <header.h>
#include "OcrLabelWnd.h"
#include <TreeWindow.h>
using namespace Regards::Scanner;
using namespace Regards::Window;

COcrLabelWnd::COcrLabelWnd(wxWindow* parent, wxWindowID id, const CThemeScrollBar& themeScroll,
                           const CThemeTree& themeTree, int idWindow)
	: CTreeWithScrollbar("COcrLabelWnd", parent, id, themeScroll, themeTree)
{
	this->idWindow = idWindow;
	ocrLabelOld = new COcrLabel(treeWindow, idWindow);
	ocrLabelOld->Init();
	treeWindow->SetTreeControl(ocrLabelOld);
}

COcrLabelWnd::~COcrLabelWnd(void)
{
	if (ocrLabelOld != nullptr)
		delete(ocrLabelOld);
}

void COcrLabelWnd::init()
{
	if (ocrLabelOld != nullptr)
	{
		auto ocrLabel = new COcrLabel(treeWindow, idWindow);
		ocrLabel->Init();
		treeWindow->SetTreeControl(ocrLabel);
		delete(ocrLabelOld);
		ocrLabelOld = ocrLabel;
	}
}

void COcrLabelWnd::Update(vector<ChOcrElement*>& labelList)
{
	if (ocrLabelOld != nullptr)
	{
		auto ocrLabel = new COcrLabel(treeWindow, idWindow);
		ocrLabel->Init(labelList);
		treeWindow->SetTreeControl(ocrLabel);
		delete(ocrLabelOld);
		ocrLabelOld = ocrLabel;
	}
}
