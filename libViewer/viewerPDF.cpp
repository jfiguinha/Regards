#include <header.h>
#include "ViewerPDF.h"
#include "ThumbnailViewerVideo.h"
#include "ViewerParam.h"
#include "ViewerParamInit.h"
#include <ImageLoadingFormat.h>
#include <ImageVideoThumbnail.h>
#include <FileUtility.h>
#include <LibResource.h>
#include "ViewerTheme.h"
#include <libPicture.h>
#include <ShowVideo.h>
#include "ViewerThemeInit.h"
#include <window_id.h>
#include <ParamInit.h>
#include <StatusText.h>
#include <picture_id.h>
#include "PictureElement.h"
#include "ThumbnailMessage.h"
#include <SqlThumbnailVideo.h>

using namespace Regards::Window;
using namespace Regards::Viewer;
#define PANE_PICTURETHUMBNAIL 1
#define PANE_VIDEOTHUMBNAIL 2
#define DELAY_ANIMATION 20

CViewerPDF::CViewerPDF(wxWindow* parent, wxWindowID id)
	: CWindowMain("PDFWindow", parent, id)
{
	showBitmapWindow = nullptr;
	viewerParam = nullptr;
	isFullscreen = false;
	filename = L"";
	width = 0;
	height = 0;

	showToolbar = true;
	checkValidity = false;

	CViewerTheme * viewerTheme = CViewerThemeInit::getInstance();
	CViewerParam * config = CViewerParamInit::getInstance();
	if (config != nullptr)
		checkValidity = config->GetCheckThumbnailValidity();

	if (viewerTheme != nullptr)
	{
		CThemeToolbar theme;
		viewerTheme->GetMainToolbarTheme(&theme);
		toolbarPDF = new CToolbarPDF(this, wxID_ANY, theme, false);
		toolbarPDF->Show(true);
	}


	//----------------------------------------------------------------------------------------
	//Panel Thumbnail Video
	//----------------------------------------------------------------------------------------
	if (viewerTheme != nullptr)
	{
		bool checkValidity = true;
		bool isPanelVisible = true;
		CViewerTheme* viewerTheme = CViewerThemeInit::getInstance();
		CViewerParam* config = CViewerParamInit::getInstance();
		if (config != nullptr)
		{
			config->GetShowVideoThumbnail(isPanelVisible);
			checkValidity = config->GetCheckThumbnailValidity();
		}

		wxString libelle = "Image List";// CLibResource::LoadStringFromResource(L"LBLTHUMBNAILVIDEO", 1);
		CThemePane theme;
		CThemeToolbar themetoolbar;
		CThemeScrollBar themeScroll;
		CThemeThumbnail themeVideo;
		viewerTheme->GetPaneTheme(&theme);
		viewerTheme->GetClickToolbarTheme(&themetoolbar);
		viewerTheme->GetScrollTheme(&themeScroll);
		viewerTheme->GetThumbnailTheme(&themeVideo);

		panelVideo = new CPanelWithClickToolbar(this, "CThumbnailVideoPanel", THUMBNAILPDFPANEL, theme, themetoolbar, libelle, isPanelVisible, false, false);
		scrollVideoWindow = new CScrollbarWnd(panelVideo->GetPaneWindow(), wxID_ANY);
		thumbnailVideo = new CThumbnailMultiPage(scrollVideoWindow, wxID_ANY, themeVideo, checkValidity);
		scrollVideoWindow->SetCentralWindow(thumbnailVideo, themeScroll);
		scrollVideoWindow->HideVerticalScroll();
		scrollVideoWindow->SetPageSize(1000);
		scrollVideoWindow->SetLineSize(200);
		panelVideo->SetWindow(scrollVideoWindow);
	}

	//----------------------------------------------------------------------------------------
	//Panel Picture Infos
	//----------------------------------------------------------------------------------------
	if (viewerTheme != nullptr)
	{
		CViewerTheme * viewerTheme = CViewerThemeInit::getInstance();
		showBitmapWindow = new CShowBitmap(this, SHOWBITMAPVIEWERIDPDF, BITMAPWINDOWVIEWERIDPDF, MAINVIEWERWINDOWID, this, viewerTheme);
	}


	Connect(wxEVT_ANIMATIONPOSITION, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CViewerPDF::AnimationSetPosition));
	Connect(wxEVT_SIZE, wxSizeEventHandler(CViewerPDF::OnSize));
	Connect(wxEVENT_RESIZE, wxCommandEventHandler(CViewerPDF::OnResize));
}

void CViewerPDF::AnimationPictureNext()
{
	animationPosition++;
	if (animationPosition >= nbThumbnail)
		animationPosition = nbThumbnail - 1;
	LoadAnimationBitmap(filename, animationPosition);
}

void CViewerPDF::AnimationPicturePrevious()
{
	animationPosition--;
	if (animationPosition < 0)
		animationPosition = 0;

	LoadAnimationBitmap(filename, animationPosition);
}

wxString CViewerPDF::GetFilename()
{
	return filename;
}

CImageLoadingFormat * CViewerPDF::GetImage()
{
	CLibPicture libPicture;
	CImageLoadingFormat * image = libPicture.LoadPicture(filename, 0);
	return image;
}

CViewerPDF::~CViewerPDF()
{
	pageThumbnail.clear();

	delete(toolbarPDF);
	delete(thumbnailVideo);
	delete(scrollVideoWindow);
	delete(panelVideo);
	delete(showBitmapWindow);
}

void CViewerPDF::HideToolbar()
{
	showToolbar = false;
	if (showBitmapWindow != nullptr)
	{
		if (isFullscreen)
		{
			panelVideo->HidePanel(false);
		}
		//previewInfosWnd->HideToolbar();

		wxWindow * window = this->FindWindowById(PREVIEWVIEWERID);
		if (window != nullptr)
		{
			wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_HIDETOOLBAR);
			window->GetEventHandler()->AddPendingEvent(evt);
		}

		this->RedrawBarPos();
	}
}

void CViewerPDF::ShowToolbar()
{
	showToolbar = true;
	if (showBitmapWindow != nullptr)
	{
		if (isFullscreen)
		{
			panelVideo->ShowPanel();
		}

		//previewThumbnailSplitter->ShowToolbar();

		wxWindow * window = this->FindWindowById(PREVIEWVIEWERID);
		if (window != nullptr)
		{
			wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_SHOWTOOLBAR);
			window->GetEventHandler()->AddPendingEvent(evt);
		}

		this->RedrawBarPos();
	}
}

void CViewerPDF::HidePanel()
{
	if (panelVideo != nullptr)
		panelVideo->HidePanel();

	this->RedrawBarPos();
}

void CViewerPDF::ShowPanelVideoThumbnail()
{
	if (panelVideo != nullptr)
	{
		panelVideo->ShowPanel();
	}

	this->RedrawBarPos();
}


void CViewerPDF::SetPosition(const long &timePosition)
{
	animationPosition = timePosition;
	LoadAnimationBitmap(filename, timePosition);
}

void CViewerPDF::UpdateScreenRatio()
{
	printf("CViewerPDF::UpdateScreenRatio() \n");
	panelVideo->UpdateScreenRatio();
	showBitmapWindow->UpdateScreenRatio();
	this->Resize();
}


void CViewerPDF::OnResize(wxCommandEvent& event)
{
	RedrawBarPos();
}

void CViewerPDF::Resize()
{
	RedrawBarPos();
}

wxRect CViewerPDF::GetWindowRect()
{
	wxRect rc;
	rc.x = 0;
	rc.y = 0;
	rc.width = width;
	rc.height = height;
	return rc;
}


void CViewerPDF::RedrawBarPos()
{
	//wxRect rc = GetWindowRect();
	int bottomHeight = 0;
	int topHeight = 0;

	toolbarPDF->SetSize(0, 0, width, toolbarPDF->GetNavigatorHeight());
	toolbarPDF->Refresh();

	if (!isFullscreen && !panelVideo->IsShown())
	{
		panelVideo->Show();
		//if(!thumbnailVideoPanel->IsPanelVideoThumbnailVisible())
		panelVideo->ShowPanel();
	}

	if (panelVideo->IsShown())
	{
		int iconeHeight = panelVideo->GetHeight();
		panelVideo->SetSize(0, toolbarPDF->GetNavigatorHeight(), width, iconeHeight);
		panelVideo->Refresh();
		topHeight += iconeHeight + toolbarPDF->GetNavigatorHeight();
	}



	if (!showToolbar && isFullscreen)
	{
		panelVideo->HidePanel(false);
		bottomHeight = 0;
		topHeight = 0;
	}

	if (showBitmapWindow != nullptr)
	{
		showBitmapWindow->SetSize(0, topHeight, width, height - (topHeight + bottomHeight));
	}

	printf("CViewerPDF::RedrawBarPos() \n");
}


void CViewerPDF::OnSize(wxSizeEvent& event)
{

	width = event.GetSize().GetWidth();
	height = event.GetSize().GetHeight();
	RedrawBarPos();
}

void CViewerPDF::AnimationSetPosition(wxCommandEvent& event)
{
	long position = event.GetExtraLong();
	LoadAnimationBitmap(filename, position);
}


void CViewerPDF::LoadAnimationBitmap(const wxString &filename, const int &numFrame)
{
	if (numFrame == oldAnimationPosition && filename == oldFilename)
		return;

	oldFilename = filename;
	oldAnimationPosition = numFrame;

	printf("CViewerPDF::LoadAnimationBitmap %d \n", numFrame);
	if (numFrame < nbThumbnail && numFrame >= 0)
	{
		CImageLoadingFormat * image = nullptr;
		CLibPicture libPicture;
		image = libPicture.LoadPicture(filename, false, numFrame);
		showBitmapWindow->SetBitmap(image, false);
	}
}

void CViewerPDF::SetImage(const wxImage &imageFile)
{
	wxString documentPath = CFileUtility::GetDocumentFolderPath();
#ifdef WIN32
	wxString tempFolder = documentPath + "\\temp";
#else
	wxString tempFolder = documentPath + "/temp";
#endif
	if (!wxMkDir(tempFolder)) {
		// handle the error here
	}
	else
	{
#ifdef WIN32
		wxString file = tempFolder + "\\temp.pdf";
#else
		wxString file = tempFolder + "/temp.pdf";
#endif
		wxRemoveFile(file);

		//wxImage * imageTemp = new wxImage(imageFile);
		imageFile.SaveFile(file);
		LoadFile(file);
	}
}

void CViewerPDF::LoadFile(const wxString &filename)
{
	CLibPicture libPicture;
	bool result = false;
	oldAnimationPosition = -1;
	oldFilename = L"";
	this->filename = filename;
	pageThumbnail.clear();

	libPicture.LoadAllVideoThumbnail(filename, &pageThumbnail, true, true);

	thumbnailVideo->SetFile(filename, pageThumbnail);

	nbThumbnail = libPicture.GetNbImage(filename);

	if (showBitmapWindow != nullptr)
	{
		animationPosition = 0;

	}

	//nbThumbnail = page.size();
	LoadAnimationBitmap(filename, 0);
}

void CViewerPDF::FullscreenMode()
{
	isFullscreen = true;
	//showBitmapWindow->FullscreenMode();
	RedrawBarPos();
}

void CViewerPDF::ScreenMode()
{
	isFullscreen = false;
	//showBitmapWindow->ScreenMode();
	RedrawBarPos();
}

bool CViewerPDF::IsPanelInfosVisible()
{

	return false;
}

bool CViewerPDF::GetProcessEnd()
{
	TRACE();
	return true;
}
