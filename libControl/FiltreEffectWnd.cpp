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
#include "ShowBitmap.h"
#include "ShowVideo.h"
#include <window_id.h>
#include <FilterData.h>
#include <ImageLoadingFormat.h>
#include <TreeWindow.h>
#include <SqlPhotos.h>

#include "effect_id.h"
using namespace Regards::Window;
using namespace Regards::Control;
using namespace Regards::Sqlite;

CFiltreEffectScrollWnd::CFiltreEffectScrollWnd(wxWindow* parent, wxWindowID id, const CThemeScrollBar& themeScroll,
                                               const CThemeTree& themeTree, int bitmapWindowId)
	: CTreeWithScrollbar("CFiltreEffectScrollWnd", parent, id, themeScroll, themeTree)
{
	this->bitmapWindowId = bitmapWindowId;
	bitmap = nullptr;
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

	if (bitmap != nullptr)
		delete bitmap;
}

void CFiltreEffectScrollWnd::OnFiltreCancel()
{
	auto imageLoad = new CImageLoadingFormat(false);
	imageLoad->SetPicture(bitmap);
	auto bitmapViewer = static_cast<CBitmapWndViewer*>(this->FindWindowById(bitmapWindowId));
	if (bitmapViewer != nullptr)
	{
		bitmapViewer->SetBitmap(imageLoad);
	}
	if (bitmapViewer != nullptr && CFiltreData::NeedPreview(numFiltre))
		bitmapViewer->RemoveListener();
}

void CFiltreEffectScrollWnd::OnFiltreOk(const int& numFiltre, CInfoEffectWnd* historyEffectWnd)
{
	auto bitmapViewer = static_cast<CBitmapWndViewer*>(this->FindWindowById(bitmapWindowId));

	if (filtreEffectOld != nullptr && bitmapViewer != nullptr)
	{
		CImageLoadingFormat* imageLoad = filtreEffectOld->ApplyEffect();
		CRegardsBitmap* bitmapOut = imageLoad->GetRegardsBitmap(true);
		historyEffectWnd->AddModification(bitmapOut, CFiltreData::GetFilterLabel(numFiltre));
		delete bitmapOut;

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

	numFiltre = numItem;
	if (!isVideo)
	{
		auto bitmapViewer = static_cast<CBitmapWndViewer*>(this->FindWindowById(bitmapWindowId));
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
							wxCommandEvent evt(wxEVENT_SHOWTOOLBARFILTRE);
							evt.SetInt(numItem);
							previewWindow->GetEventHandler()->AddPendingEvent(evt);
						}
						bitmapViewer->SetTool(numItem);
						bitmapViewer->SetBitmapPreviewEffect(numItem);
					}
					break;

				case 2:
					{
						if (effectParameter != nullptr)
							delete(effectParameter);

						if (bitmap != nullptr)
							delete bitmap;
						bitmap = nullptr;

						effectParameter = CFiltreData::GetEffectParameter(numItem);


						bitmapViewer->SetBitmapPreviewEffect(numItem);

						bitmap = bitmapViewer->GetBitmap(true);

						filtreEffect->Init(effectParameter, bitmap, filename, numItem);

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
						if (filtreEffectOld != nullptr)
							delete(filtreEffectOld);
						filtreEffectOld = filtreEffect;


						break;
					}

				default:
					{
						CImageLoadingFormat* imageLoad = CFilterWindowParam::RenderEffect(
							effectParameter, bitmapViewer, numItem);
						CRegardsBitmap* bitmapOut = imageLoad->GetRegardsBitmap(true);
						historyEffectWnd->AddModification(bitmapOut, CFiltreData::GetFilterLabel(numItem));
						delete bitmapOut;

						if (imageLoad != nullptr)
							bitmapViewer->SetBitmap(imageLoad, true);

						if (numItem == IDM_ROTATE90 || numItem == IDM_ROTATE270)
						{
							CSqlPhotos sqlPhotos;
							int exif = sqlPhotos.GetPhotoExif(filename);
							if (exif != -1)
							{
								sqlPhotos.UpdatePhotoExif(filename, 0);
							}
						}
					}
					break;
				}
			}
		}
	}
	else
	{
		auto showVideo = static_cast<CShowVideo*>(this->FindWindowById(SHOWVIDEOVIEWERID));
		auto filtreEffect = new CFiltreEffect(showVideo, treeWindow, isVideo, bitmapWindowId);
		switch (numItem)
		{
		case IDM_FILTRE_VIDEO:
			{
				if (effectParameter != nullptr)
					delete(effectParameter);
				effectParameter = showVideo->GetParameter();
				showVideo->SetVideoPreviewEffect(effectParameter);
				filtreEffect->Init(effectParameter, nullptr, filename, numItem);

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

		case IDM_FILTRE_AUDIOVIDEO:
			{
				if (effectParameter != nullptr)
					delete(effectParameter);
				effectParameter = showVideo->GetParameter();
				showVideo->SetVideoPreviewEffect(effectParameter);
				filtreEffect->Init(effectParameter, nullptr, filename, numItem);

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
		}
	}
}


CFiltreEffect* CFiltreEffectScrollWnd::GetFiltreEffect()
{
	return filtreEffectOld;
}
