#include "PreviewToolbar.h"
#include "ToolbarSlide.h"
#include "ViewerParamInit.h"
#include <ConfigParam.h>
#include <LibResource.h>
#include "ViewerParam.h"
#include "MainWindow.h"
#if defined(__WXMSW__)
#include "../include/window_id.h"
#else
#include <window_id.h>
#endif
using namespace Regards::Window;
using namespace Regards::Viewer;


CPreviewToolbar::CPreviewToolbar(wxWindow* parent, wxWindowID id, const CThemeToolbar & theme, CToolbarInterface * toolbarInterface)
	: CToolbarWindow(parent, id, theme)
{
	imagePlayDiaporama = nullptr;
	imageStopDiaporama = nullptr;
	fullscreen = nullptr;

	wxString libelleFullscreen = CLibResource::LoadStringFromResource(L"LBLFULLSCREEN",1);
	wxString libelleFirst = CLibResource::LoadStringFromResource(L"LBLFIRST",1);
	wxString libellePrevious = CLibResource::LoadStringFromResource(L"LBLPREVIOUS",1);
	wxString libelleRotate90 = CLibResource::LoadStringFromResource(L"LBLROTATE90",1);
	wxString libelleRotate270 = CLibResource::LoadStringFromResource(L"LBLROTATE270",1);
	wxString libellePlay = CLibResource::LoadStringFromResource(L"LBLPLAY",1);
	wxString libelleStop = CLibResource::LoadStringFromResource(L"LBLSTOP",1);
	wxString libelleFlipVertical = CLibResource::LoadStringFromResource(L"LBLFLIPV",1);
	wxString libelleFlipHorizontal = CLibResource::LoadStringFromResource(L"LBLFLIPH",1);
	wxString libelleNext = CLibResource::LoadStringFromResource(L"LBLNEXT",1);
	wxString libelleEnd = CLibResource::LoadStringFromResource(L"LBLEND",1);

	fullscreen = new CToolbarButton(themeToolbar.button);
	fullscreen->SetButtonResourceId(L"IDB_SCREENPNG");
	fullscreen->SetCommandId(IDM_SETFULLSCREEN);
	fullscreen->SetLibelle(libelleFullscreen);
	navElement.push_back(fullscreen);


	CToolbarButton * imageFirst = new CToolbarButton(themeToolbar.button);
	imageFirst->SetButtonResourceId(L"IDB_ARROWTRACKLPNG");
	imageFirst->SetLibelle(libelleFirst);
	imageFirst->SetCommandId(WM_IMAGES_FIRST);
	navElement.push_back(imageFirst);

	CToolbarButton * imagePrec = new CToolbarButton(themeToolbar.button);
	imagePrec->SetButtonResourceId(L"IDB_ARROWLPNG");
	imagePrec->SetLibelle(libellePrevious);
	imagePrec->SetCommandId(WM_IMAGES_PRCDENTE);
	imagePrec->SetRepeatable(true);
	navElement.push_back(imagePrec);

	CToolbarButton * rotate90 = new CToolbarButton(themeToolbar.button);
	rotate90->SetButtonResourceId(L"IDB_ROTATION90");
	rotate90->SetCommandId(WM_ROTATE90);
	rotate90->SetLibelle(libelleRotate90);
	navElement.push_back(rotate90);

	CToolbarButton * rotate180 = new CToolbarButton(themeToolbar.button);
	rotate180->SetButtonResourceId(L"IDB_ROTATION270");
	rotate180->SetCommandId(WM_ROTATE270);
	rotate180->SetLibelle(libelleRotate270);
	navElement.push_back(rotate180);

	imagePlayDiaporama = new CToolbarButton(themeToolbar.button);
	imagePlayDiaporama->SetButtonResourceId(L"IDB_PLAY");
	imagePlayDiaporama->SetCommandId(WM_DIAPORAMA_PLAY);
	imagePlayDiaporama->SetLibelle(libellePlay);
	navElement.push_back(imagePlayDiaporama);

	imageStopDiaporama = new CToolbarButton(themeToolbar.button);
	imageStopDiaporama->SetButtonResourceId(L"IDB_STOP");
	imageStopDiaporama->SetCommandId(WM_DIAPORAMA_STOP);
	imageStopDiaporama->SetLibelle(libelleStop);
	imageStopDiaporama->SetVisible(false);
	navElement.push_back(imageStopDiaporama);

	CToolbarButton * flipVertical = new CToolbarButton(themeToolbar.button);
	flipVertical->SetButtonResourceId(L"IDB_FLIPVERT");
	flipVertical->SetCommandId(WM_FLIPVERTICAL);
	flipVertical->SetLibelle(libelleFlipVertical);
	navElement.push_back(flipVertical);

	CToolbarButton * flipHorizontal = new CToolbarButton(themeToolbar.button);
	flipHorizontal->SetButtonResourceId(L"IDB_FLIPHORZ");
	flipHorizontal->SetCommandId(WM_FLIPHORIZONTAL);
	flipHorizontal->SetLibelle(libelleFlipHorizontal);
	navElement.push_back(flipHorizontal);

	CToolbarButton * imageSuiv = new CToolbarButton(themeToolbar.button);
	imageSuiv->SetButtonResourceId(L"IDB_ARROWRPNG");
	imageSuiv->SetCommandId(WM_IMAGES_SUIVANTE);
	imageSuiv->SetRepeatable(true);
	imageSuiv->SetLibelle(libelleNext);
	navElement.push_back(imageSuiv);

	CToolbarButton * imageEnd = new CToolbarButton(themeToolbar.button);
	imageEnd->SetButtonResourceId(L"IDB_ARROWTRACKRPNG");
	imageEnd->SetCommandId(WM_IMAGES_END);
	imageEnd->SetLibelle(libelleEnd);
	navElement.push_back(imageEnd);
	this->toolbarInterface = toolbarInterface;
}


CPreviewToolbar::~CPreviewToolbar()
{
}

void CPreviewToolbar::SetFullscreen()
{
    wxString libelleFullscreen = CLibResource::LoadStringFromResource(L"LBLSCREEN",1);
    fullscreen->SetLibelle(libelleFullscreen);
    this->FastRefresh(this);
}

void CPreviewToolbar::SetScreen()
{
    wxString libelleFullscreen = CLibResource::LoadStringFromResource(L"LBLFULLSCREEN",1);
    fullscreen->SetLibelle(libelleFullscreen);
    this->FastRefresh(this);
}

void CPreviewToolbar::Rotate90()
{
	if (toolbarInterface != nullptr)
		toolbarInterface->ClickShowButton(WM_ROTATE90);
}

void CPreviewToolbar::Rotate270()
{
	if (toolbarInterface != nullptr)
		toolbarInterface->ClickShowButton(WM_ROTATE270);
}

void CPreviewToolbar::FlipVertical()
{
	if (toolbarInterface != nullptr)
		toolbarInterface->ClickShowButton(WM_FLIPVERTICAL);
}

void CPreviewToolbar::FlipHorizontal()
{
	if (toolbarInterface != nullptr)
		toolbarInterface->ClickShowButton(WM_FLIPHORIZONTAL);
}


void CPreviewToolbar::Fullscreen()
{
	CMainWindow * mainWindow = (CMainWindow *)this->FindWindowById(MAINVIEWERWINDOWID);
	if (mainWindow != nullptr)
	{
		if (mainWindow->IsFullscreen())
			mainWindow->SetScreen();
		else
			mainWindow->SetFullscreen();
	}
}

void CPreviewToolbar::NextPicture()
{
	CMainWindow * mainWindow = (CMainWindow *)this->FindWindowById(MAINVIEWERWINDOWID);
	if (mainWindow != nullptr)
		mainWindow->ImageSuivante();
}

void CPreviewToolbar::PreviousPicture()
{
	CMainWindow * mainWindow = (CMainWindow *)this->FindWindowById(MAINVIEWERWINDOWID);
	if (mainWindow != nullptr)
		mainWindow->ImagePrecedente();
}

void CPreviewToolbar::FirstPicture()
{
	CMainWindow * mainWindow = (CMainWindow *)this->FindWindowById(MAINVIEWERWINDOWID);
	if (mainWindow != nullptr)
		mainWindow->ImageDebut();
}

void CPreviewToolbar::DiaporamaStart()
{
	CMainWindow * mainWindow = (CMainWindow *)this->FindWindowById(MAINVIEWERWINDOWID);
	if (mainWindow != nullptr)
	{
		imagePlayDiaporama->SetVisible(false);
		imageStopDiaporama->SetVisible(true);
		mainWindow->StartDiaporama();
		this->FastRefresh(this);
	}
}

void CPreviewToolbar::DiaporamaStop()
{
	CMainWindow * mainWindow = (CMainWindow *)this->FindWindowById(MAINVIEWERWINDOWID);
	if (mainWindow != nullptr)
	{
		imageStopDiaporama->SetVisible(false);
		imagePlayDiaporama->SetVisible(true);
		mainWindow->StopDiaporama();
		this->FastRefresh(this);
	}
}

void CPreviewToolbar::LastPicture()
{
	CMainWindow * mainWindow = (CMainWindow *)this->FindWindowById(MAINVIEWERWINDOWID);
	if (mainWindow != nullptr)
		mainWindow->ImageFin();
}

void CPreviewToolbar::EventManager(const int &id)
{
	switch (id)
	{
	case WM_DIAPORAMA_PLAY:
		DiaporamaStart();
		break;

	case WM_DIAPORAMA_STOP:
		DiaporamaStop();
		break;

	case IDM_SETFULLSCREEN:
		Fullscreen();
		break;
	case WM_ROTATE90:
		Rotate90();
		break;
	case WM_ROTATE270:
		Rotate270();
		break;
	case WM_FLIPVERTICAL:
		FlipVertical();
		break;
	case WM_FLIPHORIZONTAL:
		FlipHorizontal();
		break;

	case WM_IMAGES_FIRST:
		FirstPicture();
		break;

	case WM_IMAGES_PRCDENTE:
		PreviousPicture();
		break;

	case WM_IMAGES_SUIVANTE:
		NextPicture();
		break;

	case WM_IMAGES_END:
		LastPicture();
		break;

	}
}




