#include <header.h>
#include "ListFace.h"
#include <MoveFaceDialog.h>
#include <FileUtility.h>
#include <SQLRemoveData.h>
#include <SqlFindCriteria.h>
#include <ThumbnailData.h>
#include "ViewerTheme.h"
#include "ViewerThemeInit.h"
#include <LibResource.h>
#include "MainWindow.h"
#include <window_id.h>
#include <CopyFileDlg.h>
#include <ExportFile.h>
#include <ConvertUtility.h>
#include <wx/dir.h>
#include <libPicture.h>
#include <MapSelect.h>
#include <CalendarSelect.h>
#include "SqlFindPhotos.h"
#include "ViewerParam.h"
#include "ViewerParamInit.h"
wxDEFINE_EVENT(EVENT_REFRESHVIEWER, wxCommandEvent);

using namespace Regards::Sqlite;
using namespace Regards::Window;
using namespace Regards::Viewer;

CListFace::CListFace(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusbar)
	: CWindowMain("CListFace",parent, id)
{
	thumbscrollbar = nullptr;
	thumbFaceToolbar = nullptr;
	thumbnailFace = nullptr;
	update = true;

	bool checkValidity = false;
	CViewerParam * config = CViewerParamInit::getInstance();
	if (config != nullptr)
		checkValidity = config->GetCheckThumbnailValidity();

	
	CViewerTheme * viewerTheme = CViewerThemeInit::getInstance();

	if (viewerTheme != nullptr)
	{
		CThemeThumbnail themeThumbnail;
		CThemeScrollBar theme;
		viewerTheme->GetThumbnailScrollTheme(theme);
		thumbscrollbar = new CScrollbarWnd(this, wxID_ANY);

		viewerTheme->GetThumbnailTheme(themeThumbnail);
		thumbnailFace = new CThumbnailFace(thumbscrollbar, THUMBNAILFOLDER, statusbar, themeThumbnail, checkValidity);
		//thumbnailWindow->Init(typeAffichage);
		thumbscrollbar->SetCentralWindow(thumbnailFace, theme);
	}

	if (viewerTheme != nullptr)
	{
		std::vector<int> value = { 50, 100, 150, 200, 250, 300, 350, 400, 450, 500, 550, 600, 650, 700 };
		std::vector<int> valuePertinence = { 0, 10, 20, 50, 70, 100, 125, 150, 175, 200, 225, 250, 275, 300 };

		CThemeToolbar theme;
		viewerTheme->GetThumbnailToolbarTheme(theme);
		thumbFaceToolbar = new CThumbnailFaceToolBar(this, wxID_ANY, theme);
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

		thumbFacePertinenceToolbar = new CThumbnailFacePertinenceToolBar(this, wxID_ANY, theme);//new CThumbnailFacePertinenceToolBar(this, wxID_ANY, theme);
		thumbFacePertinenceToolbar->SetTabValue(valuePertinence);
		thumbFacePertinenceToolbar->SetTrackBarPosition(2);
	}


    if(thumbscrollbar != nullptr)
        thumbscrollbar->Show(true);
    if(thumbFaceToolbar != nullptr)
        thumbFaceToolbar->Show(true);
    if(thumbFacePertinenceToolbar != nullptr)
        thumbFacePertinenceToolbar->Show(true);

	thumbFaceToolbar->Show(true);
	thumbscrollbar->ShowVerticalScroll();
	thumbnailFace->SetNoVScroll(false);
	thumbnailFace->SetCheck(true);
	thumbnailFace->Init();

	Connect(wxEVENT_THUMBNAILZOOMON, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CListFace::ThumbnailZoomOn));
	Connect(wxEVENT_THUMBNAILZOOMOFF, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CListFace::ThumbnailZoomOff));
	Connect(wxEVENT_THUMBNAILZOOMPOSITION, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CListFace::ThumbnailZoomPosition));
	Connect(wxEVENT_THUMBNAILUPDATE, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CListFace::ThumbnailUpdate));
	Connect(wxEVENT_THUMBNAILADD, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CListFace::ThumbnailAdd));
	Connect(wxEVENT_THUMBNAILREMOVE, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CListFace::ThumbnailRemove));
	Connect(wxEVENT_THUMBNAILMOVE, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CListFace::ThumbnailMove));

	update = false;
	processIdle = true;
}

void CListFace::ThumbnailAdd(wxCommandEvent& event)
{
	thumbnailFace->Init();//thumbnailFace->UpdatePhotoFace();
}

void CListFace::ThumbnailRemove(wxCommandEvent& event)
{
	thumbnailFace->Init();//thumbnailFace->DeletePhotoFace();
}

void CListFace::ThumbnailUpdate(wxCommandEvent& event)
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
	int pictureHeight = GetWindowHeight() - thumbFaceToolbar->GetHeight() - thumbFacePertinenceToolbar->GetHeight();
	thumbFacePertinenceToolbar->SetSize(0, 0, GetWindowWidth(), thumbFacePertinenceToolbar->GetHeight());
	thumbscrollbar->SetSize(0, thumbFacePertinenceToolbar->GetHeight(), pictureWidth, pictureHeight);
	thumbFaceToolbar->SetSize(0, thumbFacePertinenceToolbar->GetHeight() + pictureHeight, GetWindowWidth(), thumbFaceToolbar->GetHeight());
}

void CListFace::PictureClick(const wxString &filepath, const int &numElement)
{
	if (thumbnailFace != nullptr)
	{
		//thumbnailFolder->StopLoadingPicture();
		//thumbnailFolder->StartLoadingPicture(numElement);
		wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_ONSTOPLOADINGPICTURE);
		thumbnailFace->GetEventHandler()->AddPendingEvent(evt);

		wxCommandEvent evt2(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_ONSTARTLOADINGPICTURE);
		evt2.SetExtraLong(numElement);
		thumbnailFace->GetEventHandler()->AddPendingEvent(evt2);
	}
}
