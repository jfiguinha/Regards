#include "PreviewThumbnail.h"
#include "ExplorerParam.h"
#include "ExplorerParamInit.h"
#include <libResource.h>
#include <libPicture.h>
#include "ExplorerTheme.h"
#include "ExplorerThemeInit.h"
#if defined(__WXMSW__)
#include "../include/window_id.h"
#else
#include <window_id.h>
#endif
#include <wx/filename.h>
using namespace Regards::Explorer;

#define LIST_PANE 1
#define PREVIEW_PANE 2
#define CLICK_PREVIEW 3

CPreviewThumbnail::CPreviewThumbnail(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusbar, const CThemeSplitter & theme, CVideoEffectParameter * videoEffectParameter)
	: CSplitter(parent, id, theme)
{
	CExplorerTheme * explorerTheme = CExplorerThemeInit::getInstance();

	this->statusBar = statusbar;
	this->videoEffectParameter = videoEffectParameter;

	if (explorerTheme != nullptr)
	{
		wxString libelle = CLibResource::LoadStringFromResource(L"LBLLISTPICTURE", 1);
		CThemePane themePane;
		explorerTheme->GetPaneThumbnail(themePane);
		paneListPicture = new CPane(this, wxID_ANY, this, LIST_PANE, themePane);
		paneListPicture->SetClosable(false);
		paneListPicture->SetTitle(libelle);
	}

	if (explorerTheme != nullptr)
	{
		wxString libelle = CLibResource::LoadStringFromResource(L"LBLPREVIEWPICTURE", 1);
		CThemePane themePane;
		explorerTheme->GetPanePreviewPictureTheme(themePane);
		panePreviewPicture = new CPane(this, wxID_ANY, this, PREVIEW_PANE, themePane);
		panePreviewPicture->SetTitle(libelle);
	}

    
	if (explorerTheme != nullptr)
	{
		wxString libelle = CLibResource::LoadStringFromResource(L"LBLPREVIEWPICTURE", 1);
		CThemePane themePane;
		explorerTheme->GetPanePreviewPictureTheme(themePane);
		paneEmpty = new CPane(this, wxID_ANY, this, PREVIEW_PANE, themePane);
		paneEmpty->SetTitle(libelle);
	}
    

	listPicture = new CListPicture(paneListPicture, LISTPICTUREID, statusbar);
    if(paneListPicture != nullptr)
        paneListPicture->SetOtherWindow(listPicture);

	
	if (previewImage == nullptr)
	{
		CExplorerTheme * explorerTheme = CExplorerThemeInit::getInstance();

		if (explorerTheme != nullptr)
		{
			CThemeSplitter themeSplitter;
			explorerTheme->GetPreviewSplitterTheme(themeSplitter);
			previewImage = new CPreviewWnd(panePreviewPicture, PREVIEWEXPLORERID, themeSplitter, videoEffectParameter, statusBar);
		}

        if(panePreviewPicture != nullptr)
        {
            panePreviewPicture->SetOtherWindow(previewImage);
            panePreviewPicture->Show(false);
        }
	}

	if (emptyWindow == nullptr && paneEmpty != nullptr)
	{
		emptyWindow = new CEmptyWindow(paneEmpty, wxID_ANY);
		paneEmpty->SetOtherWindow(emptyWindow);
	}
	

	if (explorerTheme != nullptr)
	{
		CThemeToolbar theme;
		explorerTheme->GetClickPreviewToolbarTheme(theme);
		clickPreview = new CClickToolbar(this, wxID_ANY, theme, this, CLICK_PREVIEW);
	}


	//this->SetWindow(panePreviewPicture, paneListPicture);
	this->SetWindow(paneEmpty, paneListPicture);

	explorerconfig = CExplorerParamInit::getInstance();
	if (explorerconfig != nullptr)
	{
		this->posBar = explorerconfig->GetPositionPreviewThumbnail();
		bool showPreview = explorerconfig->GetShowPreview();
		if (!showPreview)
		{
			ClosePane(PREVIEW_PANE);
		}
	}
}

void CPreviewThumbnail::UpdateScreenRatio()
{
    paneListPicture->UpdateScreenRatio();
    panePreviewPicture->UpdateScreenRatio();
    previewImage->UpdateScreenRatio();
    listPicture->UpdateScreenRatio();
    clickPreview->UpdateScreenRatio();

    this->Resize();
}


void CPreviewThumbnail::HideToolbar()
{
	if (previewImage != nullptr)
	{
		previewImage->HideToolbar();
	}
}

void CPreviewThumbnail::ShowToolbar()
{
	if (previewImage != nullptr)
	{
		previewImage->ShowToolbar();
	}
}


CPreviewThumbnail::~CPreviewThumbnail()
{
	CExplorerParam * explorerconfig = CExplorerParamInit::getInstance();
	if (explorerconfig != nullptr)
        explorerconfig->SetPositionPreviewThumbnail(this->posBar);

	if (previewImage != nullptr)
		delete(previewImage);

	if (listPicture != nullptr)
		delete(listPicture);

	if (paneListPicture != nullptr)
		delete(paneListPicture);

	if (panePreviewPicture != nullptr)
		delete(panePreviewPicture);

	if (clickPreview != nullptr)
		delete(clickPreview);

	if (emptyWindow != nullptr)
		delete(emptyWindow);

	if (paneEmpty != nullptr)
		delete(paneEmpty);
}

void CPreviewThumbnail::ShowPreview()
{
	if (!panePreviewPicture->IsShown())
		ClickShowButton(CLICK_PREVIEW);
}

void CPreviewThumbnail::ClickShowButton(const int &id)
{
	switch (id)
	{
		case CLICK_PREVIEW:
		{
			clickPreview->Show(false);
			panePreviewPicture->Show(true);
			this->SetFixPosition(false);
			this->SetPosition(oldPos);
			this->SetSeparationBarVisible(true);

			if (explorerconfig != nullptr)
				explorerconfig->SetShowPreview(true);

			this->SetWindow(panePreviewPicture, paneListPicture);
			this->Resize(this);
			break;
		}
	}
}

void CPreviewThumbnail::PhotoSelect(const wxString &photoPath)
{
	CLibPicture libPicture;
	if (!panePreviewPicture->IsShown())
	{
		paneEmpty->Show(false);
		panePreviewPicture->Show(true);
		this->SetWindow(panePreviewPicture, paneListPicture);
		this->Resize();
	}

	if (statusBar != nullptr)
	{
		wxFileName filename(photoPath);
		statusBar->SetText(1, photoPath);
	}

	if (libPicture.TestIsVideo(photoPath))
		previewImage->SetVideo(photoPath);
    else if (libPicture.TestIsAnimation(photoPath))
        previewImage->SetAnimation(photoPath);
	else 
		previewImage->SetPhoto(photoPath);

	
}


void CPreviewThumbnail::ClosePane(const int &id)
{
	switch (id)
	{
	case PREVIEW_PANE:
		clickPreview->Show(true);
		panePreviewPicture->Show(false);
		oldPos = this->GetPosition();
		//this->SetPosition(10);
		this->SetWindow1FixPosition(true, clickPreview->GetNavigatorWidth());
		this->SetSeparationBarVisible(false);
		//this->SetFixPosition(true);
		CExplorerParam * explorerconfig = CExplorerParamInit::getInstance();
		if (explorerconfig != nullptr)
			explorerconfig->SetShowPreview(false);

		this->SetWindow(clickPreview, paneListPicture);
		this->Resize(this);
		paneListPicture->Refresh();
		break;
	}
}