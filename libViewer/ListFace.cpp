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
		CThemeThumbnail themeThumbnail;
		CThemeScrollBar theme;
		viewerTheme->GetScrollTheme(&theme);
		viewerTheme->GetThumbnailTheme(&themeThumbnail);
		thumbnailFace = new CThumbnailFace(this, THUMBNAILFACE, themeThumbnail, checkValidity);
		thumbscrollbar = new CScrollbarWnd(this, thumbnailFace, wxID_ANY);
		thumbscrollbar->ShowVerticalScroll();
		thumbnailFace->SetNoVScroll(false);
		thumbnailFace->SetCheck(true);
		thumbnailFace->Init();
	}

	if (viewerTheme != nullptr)
	{
		std::vector<int> value = { 50, 100, 150, 200, 250, 300, 350, 400, 450, 500, 550, 600, 650, 700 };
		std::vector<int> valuePertinence = { 0, 10, 20, 50, 70, 100, 125, 150, 175, 200, 225, 250, 275, 300 };

		CThemeToolbar theme;
		viewerTheme->GetThumbnailToolbarTheme(theme);
		thumbFaceToolbar = new CThumbnailFaceToolBar(this, wxID_ANY, theme, false);
		thumbFaceToolbar->SetTabValue(value);
		thumbFaceToolbar->SetTrackBarPosition(4);

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

		thumbFacePertinenceToolbar = new CThumbnailFacePertinenceToolBar(this, wxID_ANY, theme, false);//new CThumbnailFacePertinenceToolBar(this, wxID_ANY, theme);
		thumbFacePertinenceToolbar->SetTabValue(valuePertinence);
		thumbFacePertinenceToolbar->SetTrackBarPosition(2);
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
	delete(thumbnailFace);
	delete(thumbFacePertinenceToolbar);
	delete(thumbFaceToolbar);
	delete(thumbscrollbar);
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
    thumbscrollbar->UpdateScreenRatio();
    thumbFaceToolbar->UpdateScreenRatio();
	thumbnailFace->UpdateScreenRatio();
    this->Resize();
}


void CListFace::Resize()
{
	int pictureWidth = GetWindowWidth();
	int pictureHeight = GetWindowHeight() - (thumbFaceToolbar->GetHeight() + thumbFacePertinenceToolbar->GetHeight());
	thumbFacePertinenceToolbar->SetSize(0, 0, GetWindowWidth(), thumbFacePertinenceToolbar->GetHeight());
	thumbscrollbar->SetSize(0, thumbFacePertinenceToolbar->GetHeight(), pictureWidth, pictureHeight);
	thumbFaceToolbar->SetSize(0, thumbFacePertinenceToolbar->GetHeight() + pictureHeight, GetWindowWidth(), thumbFaceToolbar->GetHeight());
}

#endif