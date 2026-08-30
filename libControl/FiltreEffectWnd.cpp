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
#include "BitmapWndViewer.h"
#include "BitmapWnd3d.h"
#include <window_id.h>
#include <FilterData.h>
#include <TitleBar.h>
#include <ImageLoadingFormat.h>
#include <TreeWindow.h>
#include "ShowElement.h"
#include "effect_id.h"
using namespace Regards::Window;
using namespace Regards::Control;


CFiltreEffectScrollWnd::CFiltreEffectScrollWnd(wxWindow* parent, wxWindowID id, const CThemeScrollBar& themeScroll,
                                               const CThemeTree& themeTree, int bitmapWindowId)
	: CTreeWithScrollbar("CFiltreEffectScrollWnd", parent, id, themeScroll, themeTree, "Filtre", true)
{
	this->bitmapWindowId = bitmapWindowId;
	numFiltre = 0;
	bitmap = nullptr;
	effectParameter = nullptr;
	filtreEffectOld = nullptr;
	bitmap = std::make_unique<CImageLoadingFormat>();
	Connect(wxEVENT_UPDATEFILTER, wxCommandEventHandler(CFiltreEffectScrollWnd::OnUpdateFilter));
}



void CFiltreEffectScrollWnd::SetBitmapToViewer(CImageLoadingFormat* bitmap)
{
	auto bitmapWindow = wxWindow::FindWindowById(bitmapWindowId);
	auto event = new wxCommandEvent(wxEVENT_SETBITMAP);
	event->SetClientData(bitmap);
	wxQueueEvent(bitmapWindow, event);
}

void CFiltreEffectScrollWnd::OnFiltreCancel()
{
	CBitmapWndViewer* bitmapViewer = nullptr;
	auto bitmapWindow = dynamic_cast<IBitmapWnd*>(FindWindowById(bitmapWindowId));
	if (bitmapWindow != nullptr)
	{
		bitmapViewer = static_cast<CBitmapWndViewer*>(bitmapWindow->GetWndPt());
	}

	SetBitmapToViewer(bitmap.release());

	if (bitmapViewer != nullptr && CFiltreData::NeedPreview(numFiltre))
		bitmapViewer->RemoveListener();

	filtreEffectOld.reset();

}

void CFiltreEffectScrollWnd::OnFiltreOk(const int& numFiltre, CInfoEffectWnd* historyEffectWnd)
{
	//auto bitmapViewer = static_cast<CBitmapWndViewer*>(this->FindWindowById(bitmapWindowId));
	CBitmapWndViewer* bitmapViewer = nullptr;
	auto bitmapWindow = dynamic_cast<IBitmapWnd*>(FindWindowById(bitmapWindowId));
	if (bitmapWindow != nullptr)
	{
		bitmapViewer = static_cast<CBitmapWndViewer*>(bitmapWindow->GetWndPt());
	}
	if (filtreEffectOld != nullptr && bitmapViewer != nullptr)
	{
		CImageLoadingFormat* imageLoad = filtreEffectOld->ApplyEffect();
		if (imageLoad != nullptr)
		{
			historyEffectWnd->AddModification(imageLoad, CFiltreData::GetFilterLabel(numFiltre));
		}

		if (imageLoad != nullptr)
			SetBitmapToViewer(imageLoad);
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

	filtreEffectOld.reset();

	wxString filterName = CFiltreData::GetFilterLabel(numItem);
	if (filterName != "")
	{
		if (titleBar != nullptr)
			titleBar->SetTitle(filterName);

		showTitle = true;
	}
	else
	{
		showTitle = false;
	}

	numFiltre = numItem;
	if (!isVideo)
	{
		CFiltreEffect* filtreEffect = nullptr;
		CBitmapWndViewer* bitmapViewer = nullptr;
		auto bitmapWindow = dynamic_cast<IBitmapWnd*>(FindWindowById(bitmapWindowId));
		if (bitmapWindow != nullptr)
		{
			bitmapViewer = static_cast<CBitmapWndViewer*>(bitmapWindow->GetWndPt());
		}

		if (bitmapViewer != nullptr)
		{
			if (bitmapViewer != nullptr)
			{
				//auto filtreEffect = new CFiltreEffect(bitmapViewer, treeWindow, isVideo, bitmapWindowId);
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
						effectParameter = CFiltreData::GetEffectParameter(numItem);


						bitmapViewer->SetBitmapPreviewEffect(numItem);

						bitmap.reset(bitmapViewer->GetBitmap(true));

						if(filtreEffect == nullptr)
							filtreEffect = new CFiltreEffect(bitmapViewer, treeWindow, isVideo, bitmapWindowId);

						filtreEffect->Init(effectParameter, bitmap->GetMatrix().getMat(), filename, numItem);

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

						filtreEffectOld.reset(filtreEffect);


						break;
					}

				default:
					{
						CImageLoadingFormat* imageLoad = CFilterWindowParam::RenderEffect(
							effectParameter, bitmapViewer, numItem);
						historyEffectWnd->AddModification(imageLoad, CFiltreData::GetFilterLabel(numItem));
						if (imageLoad != nullptr)
							SetBitmapToViewer(imageLoad);
					}
					break;
				}
			}
		}
	}
	else
	{
		auto showVideo = static_cast<CShowElement*>(this->FindWindowById(SHOWBITMAPVIEWERID));
		
		switch (numItem)
		{
		case IDM_FILTRE_VIDEO:
			{
				auto filtreEffect = new CFiltreEffect(showVideo, treeWindow, isVideo, bitmapWindowId);

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
				filtreEffectOld.reset(filtreEffect);
				break;
			}

		case IDM_FILTRE_AUDIOVIDEO:
			{
				auto filtreEffect = new CFiltreEffect(showVideo, treeWindow, isVideo, bitmapWindowId);

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
				filtreEffectOld.reset(filtreEffect);
				break;
			}
		default: ;
		}
	}
	this->Resize();
}


CFiltreEffect* CFiltreEffectScrollWnd::GetFiltreEffect()
{
	return filtreEffectOld.get();
}
