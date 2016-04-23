#include "CentralWnd.h"
#include "ExplorerTheme.h"
#include "ExplorerThemeInit.h"
#include "eventid.h"
#if defined(__WXMSW__)
#include "../include/window_id.h"
#else
#include <window_id.h>
#endif
#include "PreviewThumbnail.h"
#include "CategoryFolderWindow.h"
#include "ExplorerParam.h"
#include "ExplorerParamInit.h"
using namespace Regards::Explorer;

CCentralWnd::CCentralWnd(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusbar, const CThemeSplitter & theme, CVideoEffectParameter * videoEffectParameter)
	: CSplitter(parent, id, theme)
{
	CExplorerTheme * explorerTheme = CExplorerThemeInit::getInstance();
	numCatalog = 1;
	numElement = 0;


	if (explorerTheme != nullptr)
	{
		CThemeSplitter themeSplitter;
		explorerTheme->GetListImageViewTheme(themeSplitter);
		previewThumbnail = new CPreviewThumbnail(this, PREVIEWTHUMBNAILWINDOWID, statusbar, themeSplitter, videoEffectParameter);
	}

	if (explorerTheme != nullptr)
	{
		CThemeSplitter theme;
		explorerTheme->GetCategoryFolderSplitterTheme(theme);
		categoryFolder = new CCategoryFolderWindow(this, CATEGORYFOLDERWINDOWID);
	}

	
	this->SetWindow(categoryFolder, previewThumbnail);

	CConfigParam * configParam = CExplorerParamInit::getInstance();
	if (configParam != nullptr)
	{
		explorerconfig = (CExplorerParam *)configParam;
		this->posBar = explorerconfig->GetPositionFolderCriteriaPreviewThumbnail();
	}

	Connect(EVENT_HIDDENPANE, wxCommandEventHandler(CCentralWnd::OnHidePane));
}


CCentralWnd::~CCentralWnd()
{
	if (explorerconfig != nullptr)
		explorerconfig->SetPositionFolderCriteriaPreviewThumbnail(this->posBar);

	if (previewThumbnail != nullptr)
		delete(previewThumbnail);

	if (categoryFolder != nullptr)
		delete(categoryFolder);
}


void CCentralWnd::HideToolbar()
{
	if (previewThumbnail != nullptr)
		previewThumbnail->HideToolbar();
}

void CCentralWnd::ShowToolbar()
{
	if (previewThumbnail != nullptr)
		previewThumbnail->ShowToolbar();
}

void CCentralWnd::StopLoadingPicture()
{
    CListPicture * listPicture = (CListPicture *)this->FindWindowById(LISTPICTUREID);
    if (listPicture != nullptr)
        listPicture->StopLoadingPicture();
}

//---------------------------------------------------------
//
//---------------------------------------------------------
void CCentralWnd::ImageSuivante()
{
	CListPicture * listPicture = (CListPicture *)this->FindWindowById(LISTPICTUREID);
	if (listPicture != nullptr)
	{
		numElement++;
		if (numElement == listPicture->GetNbElement())
			numElement = 0;

		listPicture->SetNumElement(numElement);

	}
}

//---------------------------------------------------------
//
//---------------------------------------------------------
void CCentralWnd::ImagePrecedente()
{
	CListPicture * listPicture = (CListPicture *)this->FindWindowById(LISTPICTUREID);
	if (listPicture != nullptr)
	{
		numElement--;
		if (numElement < 0)
			numElement = listPicture->GetNbElement() - 1;

		listPicture->SetNumElement(numElement);

	}
}

void CCentralWnd::UpdateScreenRatio()
{
    previewThumbnail->UpdateScreenRatio();
    categoryFolder->UpdateScreenRatio();
    Resize();
}

void CCentralWnd::ImageDebut()
{
	CListPicture * listPicture = (CListPicture *)this->FindWindowById(LISTPICTUREID);
	if (listPicture != nullptr)
	{
		numElement = 0;
		listPicture->SetNumElement(numElement);

	}
}

void CCentralWnd::ImageFin()
{
	CListPicture * listPicture = (CListPicture *)this->FindWindowById(LISTPICTUREID);
	if (listPicture != nullptr)
	{
		numElement = listPicture->GetNbElement() - 1;
		listPicture->SetNumElement(numElement);

	}
}

void CCentralWnd::SetNumImage(const int &numElement, const wxString &filepath)
{
	CListPicture * listPicture = (CListPicture *)this->FindWindowById(LISTPICTUREID);
	if (listPicture != nullptr)
	{
		this->numElement = numElement;
		listPicture->PictureClick(filepath, numElement);
		//listPicture->SetNumElement(numElement);
	}
}

void CCentralWnd::OnHidePane(wxCommandEvent& event)
{
	this->Resize(this);
}
