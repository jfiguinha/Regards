//
//  FiltreEffectWnd.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 02/10/2015.
//  Copyright © 2015 figuinha jacques. All rights reserved.
//
#include <header.h>
#include "FiltreEffectWnd.h"
#include "InfoEffectWnd.h"
#include "CloudsEffectParameter.h"
#include "BitmapWndViewer.h"
#include "BitmapWnd3d.h"
#include <window_id.h>
#include <FilterData.h>
#include <TitleBar.h>
#include <ImageLoadingFormat.h>
#include <TreeWindow.h>
#include <SqlPhotos.h>
#include "ShowElement.h"
#include "effect_id.h"
using namespace Regards::Window;
using namespace Regards::Control;
using namespace Regards::Sqlite;

CFiltreEffectScrollWnd::CFiltreEffectScrollWnd(wxWindow* parent, wxWindowID id, const CThemeScrollBar& themeScroll,
                                               const CThemeTree& themeTree, int bitmapWindowId)
	: CTreeWithScrollbar("CFiltreEffectScrollWnd", parent, id, themeScroll, themeTree, "Filtre", true)
{
	this->bitmapWindowId = bitmapWindowId;
	numFiltre = 0;
	effectParameter = nullptr;
	filtreEffectOld = nullptr;
	Connect(wxEVENT_UPDATEFILTER, wxCommandEventHandler(CFiltreEffectScrollWnd::OnUpdateFilter));
}

CFiltreEffectScrollWnd::~CFiltreEffectScrollWnd(void)
{
	if (filtreEffectOld != nullptr)
		delete(filtreEffectOld);

	if (effectParameter != nullptr)
		delete(effectParameter);

}

void CFiltreEffectScrollWnd::OnFiltreCancel()
{
	CBitmapWndViewer* bitmapViewer = nullptr;
	auto bitmapWindow = dynamic_cast<IBitmapWnd*>(wxWindow::FindWindowById(bitmapWindowId));
	if (bitmapWindow != nullptr)
	{
		bitmapViewer = (CBitmapWndViewer*)bitmapWindow->GetWndPt();
	}

	//auto bitmapViewer = static_cast<CBitmapWndViewer*>(this->FindWindowById(bitmapWindowId));
	if (bitmapViewer != nullptr)
	{
		bitmapViewer->SetBitmap(bitmap);
	}
	if (bitmapViewer != nullptr && CFiltreData::NeedPreview(numFiltre))
		bitmapViewer->RemoveListener();

	if (filtreEffectOld != nullptr)
		delete(filtreEffectOld);
	filtreEffectOld = nullptr;
}

void CFiltreEffectScrollWnd::OnFiltreOk(const int& numFiltre, CInfoEffectWnd* historyEffectWnd)
{
	//auto bitmapViewer = static_cast<CBitmapWndViewer*>(this->FindWindowById(bitmapWindowId));
	CBitmapWndViewer* bitmapViewer = nullptr;
	auto bitmapWindow = dynamic_cast<IBitmapWnd*>(wxWindow::FindWindowById(bitmapWindowId));
	if (bitmapWindow != nullptr)
	{
		bitmapViewer = (CBitmapWndViewer*)bitmapWindow->GetWndPt();
	}
	if (filtreEffectOld != nullptr && bitmapViewer != nullptr)
	{
		CImageLoadingFormat* imageLoad = filtreEffectOld->ApplyEffect();
		if (imageLoad != nullptr)
		{
			historyEffectWnd->AddModification(imageLoad, CFiltreData::GetFilterLabel(numFiltre));
		}

		if (imageLoad != nullptr)
			bitmapViewer->SetBitmap(imageLoad, true);
	}

	if (bitmapViewer != nullptr && CFiltreData::NeedPreview(numFiltre))
		bitmapViewer->RemoveListener();
}

void CFiltreEffectScrollWnd::OnUpdateFilter(wxCommandEvent& event)
{
	if (filtreEffectOld != nullptr)
	{
		filtreEffectOld->UpdateMousePosition();
	}
}

int CFiltreEffectScrollWnd::GetNumFiltre()
{
	return numFiltre;
}

void CFiltreEffectScrollWnd::ApplyEffect(const int& numItem, CInfoEffectWnd* historyEffectWnd, const wxString& filename,
                                         const int& isVideo, int panelId, int previewId)
{
	auto panelInfos = this->FindWindowById(panelId);
	auto previewWindow = this->FindWindowById(previewId);

	if (filtreEffectOld != nullptr)
		delete(filtreEffectOld);
	filtreEffectOld = nullptr;

	wxString filterName = CFiltreData::GetFilterLabel(numItem);
	if (titleBar != nullptr)
		titleBar->SetTitle(filterName);

	numFiltre = numItem;
	if (!isVideo)
	{
		CBitmapWndViewer* bitmapViewer = nullptr;
		auto bitmapWindow = dynamic_cast<IBitmapWnd*>(wxWindow::FindWindowById(bitmapWindowId));
		if (bitmapWindow != nullptr)
		{
			bitmapViewer = (CBitmapWndViewer*)bitmapWindow->GetWndPt();
		}

		if (bitmapViewer != nullptr)
		{
			if (bitmapViewer != nullptr)
			{
				auto filtreEffect = new CFiltreEffect(bitmapViewer, treeWindow, isVideo, bitmapWindowId);
				int typeData = CFiltreData::TypeApplyFilter(numItem);

				switch (typeData)
				{
				case 1:
					{
						wxCommandEvent evt(wxEVENT_APPLYEFFECT);
						evt.SetInt(numItem);
						previewWindow->GetEventHandler()->AddPendingEvent(evt);


						if (previewWindow != nullptr)
						{
							wxCommandEvent wx_command_event(wxEVENT_SHOWTOOLBARFILTRE);
							wx_command_event.SetInt(numItem);
							previewWindow->GetEventHandler()->AddPendingEvent(wx_command_event);
						}
						bitmapViewer->SetTool(numItem);
						bitmapViewer->SetBitmapPreviewEffect(numItem);
					}
					break;

				case 2:
					{
						if (effectParameter != nullptr)
							delete(effectParameter);

	
						effectParameter = CFiltreData::GetEffectParameter(numItem);


						bitmapViewer->SetBitmapPreviewEffect(numItem);

						bitmap = bitmapViewer->GetBitmap(true);
						
						filtreEffect->Init(effectParameter, bitmap->GetOpenCVPicture(), filename, numItem);

						if (previewWindow != nullptr)
						{
							wxCommandEvent evt(wxEVENT_SHOWTOOLBARFILTRE);
							evt.SetInt(numItem);
							previewWindow->GetEventHandler()->AddPendingEvent(evt);
						}

						//if (previewWindow != nullptr)
						//	previewWindow->ShowValidationToolbar(true, numItem);

						if (panelInfos != nullptr)
						{
							wxCommandEvent evt(wxEVENT_SHOWFILTRE);
							evt.SetInt(numItem);
							panelInfos->GetEventHandler()->AddPendingEvent(evt);
						}
						//panelInfos->ShowFiltre(CFiltreData::GetFilterLabel(numItem));
						treeWindow->SetTreeControl(filtreEffect);

						filtreEffectOld = filtreEffect;


						break;
					}

				default:
					{
						CImageLoadingFormat* imageLoad = CFilterWindowParam::RenderEffect(
							effectParameter, bitmapViewer, numItem);
						historyEffectWnd->AddModification(imageLoad, CFiltreData::GetFilterLabel(numItem));
						if (imageLoad != nullptr)
							bitmapViewer->SetBitmap(imageLoad, true);

					}
					break;
				}
			}
		}
	}
	else
	{
		auto showVideo = static_cast<CShowElement*>(this->FindWindowById(SHOWBITMAPVIEWERID));
		auto filtreEffect = new CFiltreEffect(showVideo, treeWindow, isVideo, bitmapWindowId);
		switch (numItem)
		{
		case IDM_FILTRE_VIDEO:
			{
				if (effectParameter != nullptr)
					delete(effectParameter);
				effectParameter = showVideo->GetParameter();
				showVideo->SetVideoPreviewEffect(effectParameter);
				cv::Mat bitmap;
				filtreEffect->Init(effectParameter, bitmap, filename, numItem);

				if (panelInfos != nullptr)
				{
					wxCommandEvent evt(wxEVENT_SHOWFILTRE);
					evt.SetInt(numItem);
					panelInfos->GetEventHandler()->AddPendingEvent(evt);
				}

				treeWindow->SetTreeControl(filtreEffect);
				delete(filtreEffectOld);
				filtreEffectOld = filtreEffect;
				break;
			}

		case IDM_FILTRE_AUDIOVIDEO:
			{
				if (effectParameter != nullptr)
					delete(effectParameter);
				effectParameter = showVideo->GetParameter();
				showVideo->SetVideoPreviewEffect(effectParameter);
				cv::Mat bitmap;
				filtreEffect->Init(effectParameter, bitmap, filename, numItem);

				if (panelInfos != nullptr)
				{
					wxCommandEvent evt(wxEVENT_SHOWFILTRE);
					evt.SetInt(numItem);
					panelInfos->GetEventHandler()->AddPendingEvent(evt);
				}
				//panelInfos->ShowFiltre(CFiltreData::GetFilterLabel(numItem));


				treeWindow->SetTreeControl(filtreEffect);
				delete(filtreEffectOld);
				filtreEffectOld = filtreEffect;
				break;
			}
		default: ;
		}
	}
}


CFiltreEffect* CFiltreEffectScrollWnd::GetFiltreEffect()
{
	return filtreEffectOld;
}
