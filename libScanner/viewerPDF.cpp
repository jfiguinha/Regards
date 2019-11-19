#include <header.h>
#include "viewerPDF.h"
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <wx/choicdlg.h> 
#include <ImageLoadingFormat.h>
#include <ImageVideoThumbnail.h>

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
	width = 0;
	height = 0;

	showToolbar = true;
	checkValidity = false;

	CMainTheme * viewerTheme = CMainThemeInit::getInstance();
	CMainParam * config = CMainParamInit::getInstance();
	if (config != nullptr)
		checkValidity = config->GetCheckThumbnailValidity();

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

	if (viewerTheme != nullptr)
	{
		CThemeToolbar theme;
		viewerTheme->GetFiltreToolbarTheme(&theme);
		filtreToolbar = new CFiltreToolbar(this, wxID_ANY, theme, false);
	}

	//----------------------------------------------------------------------------------------
	//Panel Picture Infos
	//----------------------------------------------------------------------------------------
	if (viewerTheme != nullptr)
	{
		showBitmapWindow = new CShowBitmap(this, SHOWBITMAPVIEWERIDPDF, BITMAPWINDOWVIEWERIDPDF, SCANNER_MAINVIEWERWINDOWID, this, viewerTheme);
	}

	filtreToolbar->Show(false);

	Connect(wxEVT_ANIMATIONPOSITION, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CViewerPDF::AnimationSetPosition));
	Connect(wxEVT_SIZE, wxSizeEventHandler(CViewerPDF::OnSize));
	Connect(wxEVENT_RESIZE, wxCommandEventHandler(CViewerPDF::OnResize));
	Connect(wxEVENT_FILTREOK, wxCommandEventHandler(CViewerPDF::OnFiltreOK));
	Connect(wxEVENT_FILTRECANCEL, wxCommandEventHandler(CViewerPDF::OnFiltreCancel));
	Connect(wxEVENT_SHOWTOOLBARFILTRE, wxCommandEventHandler(CViewerPDF::OnShowToolbar));
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
	filtreToolbar->SetNumFiltre(filtre);
	if (filtreToolbar != nullptr)
	{
		//if (!filtreToolbar->IsShown())
		filtreToolbar->Show(true);
	}
	Resize();
}

void CViewerPDF::HideValidationToolbar()
{
	isEffect = false;
	if (filtreToolbar != nullptr)
	{
		if (filtreToolbar->IsShown())
			filtreToolbar->Show(false);
	}
	showBitmapWindow->SetBitmapPreviewEffect(0);
	Resize();
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

	
	delete(thumbnailVideo);
	delete(scrollVideoWindow);
	delete(panelVideo);
	delete(showBitmapWindow);
	delete(filtreToolbar);
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

	if (!isFullscreen && !panelVideo->IsShown())
	{
		panelVideo->Show();
		//if(!thumbnailVideoPanel->IsPanelVideoThumbnailVisible())
		panelVideo->ShowPanel();
	}

	if (panelVideo->IsShown())
	{
		int iconeHeight = panelVideo->GetHeight();
		panelVideo->SetSize(0, 0, width, iconeHeight);
		panelVideo->Refresh();
		topHeight += iconeHeight;
	}



	if (!showToolbar && isFullscreen)
	{
		panelVideo->HidePanel(false);
		bottomHeight = 0;
		topHeight = 0;
	}

	if (showBitmapWindow != nullptr)
	{
		if (filtreToolbar != nullptr)
		{
			if (filtreToolbar->IsShown())
			{
				wxRect rcAffichageBitmap;
				int toolbarHeightSize = filtreToolbar->GetHeight();	    
				rcAffichageBitmap.x = 0;
				rcAffichageBitmap.y = 0;
				rcAffichageBitmap.width = width;
				rcAffichageBitmap.height = height - (topHeight + bottomHeight);
				filtreToolbar->SetSize(rcAffichageBitmap.x, (topHeight + rcAffichageBitmap.height) - toolbarHeightSize, rcAffichageBitmap.width, toolbarHeightSize);
				showBitmapWindow->SetSize(rcAffichageBitmap.x, topHeight, rcAffichageBitmap.width, rcAffichageBitmap.height - toolbarHeightSize);
			}
			else
				showBitmapWindow->SetSize(0, topHeight, width, height - (topHeight + bottomHeight));
		}
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

		COcrWnd * ocrWnd = (COcrWnd *)wxWindow::FindWindowById(SCANNER_OCRPAGE);
		if (ocrWnd != nullptr)
		{
			ocrWnd->Init();
		}

		showBitmapWindow->SetBitmap(image, false);
	}
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

int CViewerPDF::GetAnimationPosition()
{
	return oldAnimationPosition;
}


bool CViewerPDF::GetProcessEnd()
{
	TRACE();
	return true;
}
