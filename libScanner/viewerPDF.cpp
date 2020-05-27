#include <header.h>
#include "viewerPDF.h"
#include <tesseract/baseapi.h>
#undef fract1
#include <leptonica/allheaders.h>
#define fract1
#include <wx/choicdlg.h> 
#include <ImageLoadingFormat.h>
#include <ImageVideoThumbnail.h>
#include "PanelInfosWnd.h"
#include <LibResource.h>
#include "MainTheme.h"
#include "MainThemeInit.h"
#include "ScannerParam.h"
#include "ScannerParamInit.h"
#include <libPicture.h>
#include <ShowVideo.h>
#include <ShowBitmap.h>
#include "ThumbnailMultiPage.h"
#include <window_id.h>
#include <ParamInit.h>
#include <StatusText.h>
#include <picture_id.h>
#include "PanelInfosWnd.h"
//#include "PictureElement.h"
#include "ThumbnailMessage.h"
#include <SqlThumbnailVideo.h>
#include "ScannerFrame.h"
#include <FileUtility.h>
#include "CentralWindow.h"
#include "OcrWnd.h"
//#include <qpdf/QIntC.hh>

using namespace Regards::Window;
using namespace Regards::Scanner;
#define PANE_PICTURETHUMBNAIL 1
#define PANE_VIDEOTHUMBNAIL 2
#define DELAY_ANIMATION 20
#define DEMO

CViewerPDF::CViewerPDF(wxWindow* parent, CScannerFrame * frame, wxWindowID id)
	: CWindowMain("PDFWindow", parent, id)
{
	this->frame = frame;
	showBitmapWindow = nullptr;
	isFullscreen = false;
	filename = L"";
	wxRect rect;
	showToolbar = true;
	checkValidity = false;

	CMainTheme * viewerTheme = CMainThemeInit::getInstance();
	CMainParam * config = CMainParamInit::getInstance();
	if (config != nullptr)
		checkValidity = config->GetCheckThumbnailValidity();


	if (viewerTheme != nullptr)
	{
		CThemeSplitter theme;
		viewerTheme->GetSplitterTheme(&theme);
		windowManager = new CWindowManager(this, wxID_ANY, theme);
	}

	//----------------------------------------------------------------------------------------
	//Panel Thumbnail Video
	//----------------------------------------------------------------------------------------
	if (viewerTheme != nullptr)
	{
		bool checkValidity = true;
		bool isPanelVisible = true;
		if (config != nullptr)
		{
			config->GetShowVideoThumbnail(isPanelVisible);
			checkValidity = config->GetCheckThumbnailValidity();
		}

		wxString libelle = "Image List";
		CThemePane theme;
		CThemeToolbar themetoolbar;
		CThemeScrollBar themeScroll;
		CThemeThumbnail themeVideo;
		viewerTheme->GetPaneTheme(&theme);
		viewerTheme->GetClickToolbarTheme(&themetoolbar);
		viewerTheme->GetScrollTheme(&themeScroll);
		viewerTheme->GetThumbnailTheme(&themeVideo);

		thumbnailVideo = new CThumbnailMultiPage(windowManager, wxID_ANY, themeVideo, checkValidity);
		scrollVideoWindow = new CScrollbarWnd(windowManager, thumbnailVideo, wxID_ANY);
		scrollVideoWindow->HideVerticalScroll();
		scrollVideoWindow->SetPageSize(200);
		scrollVideoWindow->SetLineSize(200);

		windowManager->AddPanel(scrollVideoWindow, Pos::wxTOP, true, themeVideo.themeIcone.GetHeight() + theme.GetHeight() * 2, rect, libelle, "ThumbnailImageList", true, THUMBNAILPDFPANEL, true);

	}


	if (viewerTheme != nullptr)
	{
		wxString libelle = CLibResource::LoadStringFromResource(L"LBLINFORMATIONS", 1);
		CThemePane theme;
		viewerTheme->GetPaneTheme(&theme);
		CThemeToolbar themeClickInfosToolbar;
		viewerTheme->GetClickToolbarTheme(&themeClickInfosToolbar);
		panelInfosWindow = new CPanelInfosWnd(windowManager, PANELINFOSWNDSCANNERID);
		windowManager->AddPanel(panelInfosWindow, Pos::wxRIGHT, false, 0, rect, libelle, "PictureInfosPanel", true, PANELCLICKINFOSWNDID, false, true);

	}

	if (viewerTheme != nullptr)
	{
		CThemeToolbar theme;
		viewerTheme->GetFiltreToolbarTheme(&theme);
		filtreToolbar = new CFiltreToolbar(this, wxID_ANY, theme, false);
		windowManager->AddWindow(filtreToolbar, Pos::wxBOTTOM, true, filtreToolbar->GetHeight(), rect, wxID_ANY, false);
	}

	//----------------------------------------------------------------------------------------
	//Panel Picture Infos
	//----------------------------------------------------------------------------------------
	if (viewerTheme != nullptr)
	{
		showBitmapWindow = new CShowBitmap(windowManager, SHOWBITMAPVIEWERIDPDF, BITMAPWINDOWVIEWERIDPDF, SCANNER_MAINVIEWERWINDOWID, this, viewerTheme);
		windowManager->AddWindow(showBitmapWindow, Pos::wxCENTRAL, false, 0, rect, SCANNER_MAINVIEWERWINDOWID, false);
	}

	

	Connect(wxEVT_ANIMATIONPOSITION, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CViewerPDF::AnimationSetPosition));
	Connect(wxEVENT_FILTREOK, wxCommandEventHandler(CViewerPDF::OnFiltreOK));
	Connect(wxEVENT_FILTRECANCEL, wxCommandEventHandler(CViewerPDF::OnFiltreCancel));
	Connect(wxEVENT_SHOWTOOLBARFILTRE, wxCommandEventHandler(CViewerPDF::OnShowToolbar));

	//windowManager->HideWindow(Pos::wxBOTTOM);
}

void CViewerPDF::Resize()
{
	if (windowManager != nullptr)
		windowManager->SetSize(0, 0, GetWindowWidth(), GetWindowHeight());

	if (!showValidationToolbar && windowManager->IsWindowVisible(Pos::wxBOTTOM))
		windowManager->HideWindow(Pos::wxBOTTOM);
}


void CViewerPDF::OnShowToolbar(wxCommandEvent& event)
{
	int numItem = event.GetInt();
	ShowValidationToolbar(true, numItem);
}

void CViewerPDF::OnFiltreOK(wxCommandEvent& event)
{
	CPanelInfosWnd * panelInfos = (CPanelInfosWnd *)this->FindWindowById(PANELINFOSWNDSCANNERID);
	int numFiltre = event.GetInt();
	if (panelInfos != nullptr)
		panelInfos->OnFiltreOk(numFiltre);
	this->HideValidationToolbar();
}

void CViewerPDF::OnFiltreCancel(wxCommandEvent& event)
{
	CPanelInfosWnd * panelInfos = (CPanelInfosWnd *)this->FindWindowById(PANELINFOSWNDSCANNERID);
	int numFiltre = event.GetInt();
	if (panelInfos != nullptr)
		panelInfos->OnFiltreCancel();
	this->HideValidationToolbar();
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


void CViewerPDF::ShowValidationToolbar(const bool &visible, const int &filtre)
{
	isEffect = visible;
	showValidationToolbar = true;
	filtreToolbar->SetNumFiltre(filtre);
	windowManager->ShowWindow(Pos::wxBOTTOM);
	//Resize();
}

void CViewerPDF::HideValidationToolbar()
{
	isEffect = false;
	showValidationToolbar = false;
	windowManager->HideWindow(Pos::wxBOTTOM);
	showBitmapWindow->SetBitmapPreviewEffect(0);
	//Resize();
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
	delete(windowManager);
}



void CViewerPDF::HidePanel()
{
	windowManager->HidePaneWindow(Pos::wxTOP);
}

void CViewerPDF::ShowPanelVideoThumbnail()
{
	windowManager->ShowPaneWindow(Pos::wxTOP);
}


void CViewerPDF::SetPosition(const long &timePosition)
{
	animationPosition = timePosition;
	LoadAnimationBitmap(filename, timePosition);
}

void CViewerPDF::UpdateScreenRatio()
{
	printf("CViewerPDF::UpdateScreenRatio() \n");
	windowManager->UpdateScreenRatio();
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

		COcrWnd * ocrWnd = (COcrWnd *)wxWindow::FindWindowById(SCANNER_OCRPAGE);
		if (ocrWnd != nullptr)
		{
			ocrWnd->Init();
		}

		showBitmapWindow->SetBitmap(image, false);
	}

	if (panelInfosWindow != nullptr)
		panelInfosWindow->SetFile(filename);
}

void CViewerPDF::ImageSuivante()
{
	if (oldAnimationPosition < (nbThumbnail - 1))
	{
		thumbnailVideo->SetVideoPosition(oldAnimationPosition + 1);
		SetPosition(oldAnimationPosition + 1);
	}
}

void CViewerPDF::ImagePrecedente()
{
	if (oldAnimationPosition > 0)
	{
		thumbnailVideo->SetVideoPosition(oldAnimationPosition - 1);
		SetPosition(oldAnimationPosition - 1);
	}
}

wxString CViewerPDF::SetImage(wxImage imageFile)
{
	wxString file;
	wxString documentPath = CFileUtility::GetDocumentFolderPath();
#ifdef WIN32
	wxString tempFolder = documentPath + "\\temp";
    if (!wxMkDir(tempFolder)) {
#else
	wxString tempFolder = documentPath + "/temp";
    if (!wxMkDir(tempFolder, wxS_DIR_DEFAULT)) {
#endif
		// handle the error here
	}
	else
	{
#ifdef WIN32
		file = tempFolder + "\\temp.pdf";
#else
		file = tempFolder + "/temp.pdf";
#endif

		if(wxFileExists(file))
			wxRemoveFile(file);

		//wxImage * imageTemp = new wxImage(imageFile);
		imageFile.SaveFile(file);
	}
	return file;
}

void CViewerPDF::LoadFile(const wxString &filename)
{

	CLibPicture libPicture;
	bool result = false;
	oldAnimationPosition = -1;
	oldFilename = L"";

	this->filename = filename;

	if (libPicture.TestIsPDF(this->filename))
	{
		pageThumbnail.clear();

		libPicture.LoadAllVideoThumbnail(filename, &pageThumbnail, true, true);

		thumbnailVideo->SetFile(filename, pageThumbnail);

		nbThumbnail = libPicture.GetNbImage(filename);

		if (showBitmapWindow != nullptr)
		{
			animationPosition = 0;

		}
	}
	
	//nbThumbnail = page.size();
	LoadAnimationBitmap(this->filename, 0);
}

int CViewerPDF::GetAnimationPosition()
{
	return oldAnimationPosition;
}


bool CViewerPDF::GetProcessEnd()
{
	TRACE();
	return true;
}
