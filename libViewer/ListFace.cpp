#include <header.h>
#ifndef __NOFACE_DETECTION__
#include "ListFace.h"
#include <ConfigParam.h>
#include <ParamInit.h>
#include "ViewerParam.h"
#include "ViewerParamInit.h"
#include "MainTheme.h"
#include "MainThemeInit.h"
#include <MoveFaceDialog.h>
using namespace Regards::Sqlite;
using namespace Regards::Window;
using namespace Regards::Viewer;



CListFace::CListFace(wxWindow* parent, wxWindowID id)
	: CWindowMain("CListFace",parent, id)
{
	wxRect rect;
	thumbscrollbar = nullptr;
	thumbFaceToolbar = nullptr;
	thumbnailFace = nullptr;
	bool checkValidity = false;
	CMainParam * config = CMainParamInit::getInstance();
	if (config != nullptr)
		checkValidity = config->GetCheckThumbnailValidity();

	
	CMainTheme * viewerTheme = CMainThemeInit::getInstance();

	if (viewerTheme != nullptr)
	{
		CThemeSplitter theme;
		viewerTheme->GetSplitterTheme(&theme);
		windowManager = new CWindowManager(this, wxID_ANY, theme);
	}


	if (viewerTheme != nullptr)
	{
		CThemeThumbnail themeThumbnail;
		CThemeScrollBar theme;
		viewerTheme->GetScrollTheme(&theme);
		viewerTheme->GetThumbnailTheme(&themeThumbnail);
		thumbnailFace = new CThumbnailFace(windowManager, THUMBNAILFACE, themeThumbnail, checkValidity);
		thumbscrollbar = new CScrollbarWnd(windowManager, thumbnailFace, wxID_ANY);
		thumbscrollbar->ShowVerticalScroll();
		thumbnailFace->SetNoVScroll(false);
		thumbnailFace->SetCheck(true);
		thumbnailFace->Init();

		windowManager->AddWindow(thumbnailFace, Pos::wxCENTRAL, false, 0, rect, wxID_ANY, false);
	}

	if (viewerTheme != nullptr)
	{
		std::vector<int> value = { 50, 100, 150, 200, 250, 300, 350, 400, 450, 500, 550, 600, 650, 700 };
		std::vector<int> valuePertinence = { 0, 10, 20, 50, 70, 100, 125, 150, 175, 200, 225, 250, 275, 300 };

		CThemeToolbar theme;
		viewerTheme->GetThumbnailToolbarTheme(theme);
		thumbFaceToolbar = new CThumbnailFaceToolBar(windowManager, wxID_ANY, theme, false);
		thumbFaceToolbar->SetTabValue(value);
		thumbFaceToolbar->SetTrackBarPosition(4);

		windowManager->AddWindow(thumbFaceToolbar, Pos::wxBOTTOM, true, thumbFaceToolbar->GetHeight(), rect, wxID_ANY, false);

		int position = 2;
		if(config != nullptr)
		{
			double pertinence = config->GetPertinenceValue();
			int pertinenceValue = pertinence * 100;
			for(int i = 0;i < valuePertinence.size();i++)
			{
				if(pertinenceValue == valuePertinence[i])
					position = i;
			}
		}

		thumbFacePertinenceToolbar = new CThumbnailFacePertinenceToolBar(windowManager, wxID_ANY, theme, false);//new CThumbnailFacePertinenceToolBar(this, wxID_ANY, theme);
		thumbFacePertinenceToolbar->SetTabValue(valuePertinence);
		thumbFacePertinenceToolbar->SetTrackBarPosition(2);

		windowManager->AddWindow(thumbFacePertinenceToolbar, Pos::wxTOP, true, thumbFacePertinenceToolbar->GetHeight(), rect, wxID_ANY, false);
	}

	


	Connect(wxEVENT_THUMBNAILZOOMON, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CListFace::ThumbnailZoomOn));
	Connect(wxEVENT_THUMBNAILZOOMOFF, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CListFace::ThumbnailZoomOff));
	Connect(wxEVENT_THUMBNAILZOOMPOSITION, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CListFace::ThumbnailZoomPosition));
	Connect(wxEVENT_THUMBNAILREFRESH, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CListFace::ThumbnailRefresh));
	Connect(wxEVENT_THUMBNAILMOVE, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CListFace::ThumbnailMove));

}

void CListFace::ThumbnailRefresh(wxCommandEvent& event)
{
	thumbnailFace->Init();
}

void CListFace::ThumbnailMove(wxCommandEvent& event)
{
	vector<CThumbnailData *> listItem;
	thumbnailFace->GetSelectItem(listItem);
	if (listItem.size() > 0)
	{
		//Choix de la Face
		MoveFaceDialog moveFaceDialog(this);
		moveFaceDialog.ShowModal();
		if(moveFaceDialog.IsOk())
		{
			thumbnailFace->MoveFace(moveFaceDialog.GetFaceNameSelected());
		}
	}
}


CListFace::~CListFace()
{
	delete(windowManager);

}

int CListFace::GetThumbnailHeight()
{
	return thumbnailFace->GetIconeHeight() + thumbscrollbar->GetBarHeight();
}

void CListFace::SetActifItem(const int &numItem, const bool &move)
{
	if (thumbnailFace != nullptr)
		thumbnailFace->SetActifItem(numItem, move);
}

void CListFace::ThumbnailZoomOn(wxCommandEvent& event)
{
	thumbnailFace->ZoomOn();
}

void CListFace::ThumbnailZoomOff(wxCommandEvent& event)
{
	thumbnailFace->ZoomOff();
}

void CListFace::ThumbnailZoomPosition(wxCommandEvent& event)
{
	thumbnailFace->ZoomPosition(event.GetExtraLong());
}

void CListFace::UpdateScreenRatio()
{
	if (windowManager != nullptr)
		windowManager->UpdateScreenRatio();
}


void CListFace::Resize()
{
	if (windowManager != nullptr)
		windowManager->SetSize(GetWindowWidth(), GetWindowHeight());
}

#endif