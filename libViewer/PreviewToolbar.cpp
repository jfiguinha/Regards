#include <header.h>
#include "PreviewToolbar.h"
#include <LibResource.h>
#include <ToolbarInterface.h>
#include "MainWindow.h"
#include <window_id.h>
using namespace Regards::Window;
using namespace Regards::Viewer;


#define IDM_WINDOWSEARCH 152
#define IDM_THUMBNAILFACE 153
#define IDM_VIEWERMODE 154
#define IDM_EXPLORERMODE 155
#define IDM_SHOWINFOS 156
#define IDM_QUITTER 157
#define IDM_PRINT 158
#define IDM_SCANNER 159
#define IDM_PICTUREMODE 160
#define IDM_EDIT 161
#define IDM_EXPORT 162
#define IDM_NEWVERSION 163
#define IDM_EXPORT_DIAPORAMA 164

CPreviewToolbar::CPreviewToolbar(wxWindow* parent, wxWindowID id, const CThemeToolbar& theme,
                                 CToolbarInterface* toolbarInterface, const bool& vertical)
	: CToolbarWindow(parent, id, theme, vertical)
{
	fullscreen = CreateButton(L"IDB_SCREENPNG", L"LBLFULLSCREEN", IDM_SETFULLSCREEN);
	save = CreateButton("IDB_SAVE", "LBLSAVE", WM_SAVE);
	editor = CreateButton("IDB_OPEN", "LBLEDITORMODE", IDM_EDIT);
	exportFile = CreateButton("IDB_EXPORT", "LBLEXPORT", IDM_EXPORT);
	imageFirst = CreateButton(L"IDB_ARROWTRACKLPNG", L"LBLFIRST", WM_IMAGES_FIRST);
	imagePrec = CreateButton(L"IDB_ARROWLPNG", L"LBLPREVIOUS", WM_IMAGES_PRCDENTE);
	imagePrec->SetRepeatable(true);
	rotate90 = CreateButton(L"IDB_ROTATION90", L"LBLROTATE90", WM_ROTATE90);
	rotate270 = CreateButton(L"IDB_ROTATION270", L"LBLROTATE270", WM_ROTATE270);
	imagePlayDiaporama = CreateButton(L"IDB_PLAY", L"LBLPLAY", WM_DIAPORAMA_PLAY);
	imageStopDiaporama = CreateButton(L"IDB_STOP", L"LBLSTOP", WM_DIAPORAMA_STOP);
	imageStopDiaporama->SetVisible(false);
	flipVertical = CreateButton(L"IDB_FLIPVERT", L"LBLFLIPV", WM_FLIPVERTICAL);
	flipHorizontal = CreateButton(L"IDB_FLIPHORZ", L"LBLFLIPH", WM_FLIPHORIZONTAL);
	imageSuiv = CreateButton(L"IDB_ARROWRPNG", L"LBLNEXT", WM_IMAGES_SUIVANTE);
	imageSuiv->SetRepeatable(true);
	imageEnd = CreateButton(L"IDB_ARROWTRACKRPNG", L"LBLEND", WM_IMAGES_END);

	this->toolbarInterface = toolbarInterface;
}

void CPreviewToolbar::EnableScreenButton()
{
	fullscreen->SetVisible(true);
	needToRefresh = true;
}

void CPreviewToolbar::DisableScreenButton()
{
	fullscreen->SetVisible(false);
	needToRefresh = true;
}

void CPreviewToolbar::EnableExportButton()
{
	exportFile->SetVisible(true);
	needToRefresh = true;
}


void CPreviewToolbar::DisableExportButton()
{
	exportFile->SetVisible(false);
	needToRefresh = true;
}

void CPreviewToolbar::EnableSaveButton()
{
	save->SetVisible(true);
	needToRefresh = true;
}


void CPreviewToolbar::DisableSaveButton()
{
	save->SetVisible(false);
	needToRefresh = true;
}

void CPreviewToolbar::DisableNavigationButton()
{
	if (navigationButtonEnable)
	{
		navigationButtonEnable = false;
		imagePlayDiaporama->SetVisible(false);
		imageStopDiaporama->SetVisible(false);
		fullscreen->SetVisible(false);
		imageEnd->SetVisible(false);
		imageSuiv->SetVisible(false);
		imageFirst->SetVisible(false);
		imagePrec->SetVisible(false);
		needToRefresh = true;
	}
}

void CPreviewToolbar::EnableNavigationButton()
{
	if (!navigationButtonEnable)
	{
		navigationButtonEnable = true;
		imagePlayDiaporama->SetVisible(true);
		imageStopDiaporama->SetVisible(true);
		fullscreen->SetVisible(true);
		imageEnd->SetVisible(true);
		imageSuiv->SetVisible(true);
		imageFirst->SetVisible(true);
		imagePrec->SetVisible(true);
		needToRefresh = true;
	}
}

void CPreviewToolbar::SetFullscreen()
{
	wxString libelleFullscreen = CLibResource::LoadStringFromResource(L"LBLSCREEN", 1);
	fullscreen->SetLibelle(libelleFullscreen);
	needToRefresh = true;
}

void CPreviewToolbar::SetScreen()
{
	wxString libelleFullscreen = CLibResource::LoadStringFromResource(L"LBLFULLSCREEN", 1);
	fullscreen->SetLibelle(libelleFullscreen);
	needToRefresh = true;
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

void CPreviewToolbar::Save()
{
	if (toolbarInterface != nullptr)
		toolbarInterface->ClickShowButton(WM_SAVE);
}

void CPreviewToolbar::Fullscreen()
{
	auto mainWindow = static_cast<CMainWindow*>(this->FindWindowById(MAINVIEWERWINDOWID));
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
	wxWindow* mainWindow = this->FindWindowById(CENTRALVIEWERWINDOWID);
	if (mainWindow != nullptr)
	{
		wxCommandEvent evt(wxEVENT_PICTURENEXT);
		mainWindow->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CPreviewToolbar::PreviousPicture()
{
	wxWindow* mainWindow = this->FindWindowById(CENTRALVIEWERWINDOWID);
	if (mainWindow != nullptr)
	{
		wxCommandEvent evt(wxEVENT_PICTUREPREVIOUS);
		mainWindow->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CPreviewToolbar::LastPicture()
{
	wxWindow* mainWindow = this->FindWindowById(CENTRALVIEWERWINDOWID);
	if (mainWindow != nullptr)
	{
		wxCommandEvent evt(wxEVENT_PICTURELAST);
		mainWindow->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CPreviewToolbar::FirstPicture()
{
	wxWindow* mainWindow = this->FindWindowById(CENTRALVIEWERWINDOWID);
	if (mainWindow != nullptr)
	{
		wxCommandEvent evt(wxEVENT_PICTUREFIRST);
		mainWindow->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CPreviewToolbar::DiaporamaStart()
{
	auto mainWindow = static_cast<CMainWindow*>(this->FindWindowById(MAINVIEWERWINDOWID));
	if (mainWindow != nullptr)
	{
		imagePlayDiaporama->SetVisible(false);
		imageStopDiaporama->SetVisible(true);

		wxWindow* window = this->FindWindowById(CENTRALVIEWERWINDOWID);
		if (window != nullptr)
		{
			wxCommandEvent evt(wxEVENT_STARTDIAPORAMA);
			window->GetEventHandler()->AddPendingEvent(evt);
		}

		this->Resize();
	}
}

void CPreviewToolbar::DiaporamaStop()
{
	auto mainWindow = static_cast<CMainWindow*>(this->FindWindowById(MAINVIEWERWINDOWID));
	if (mainWindow != nullptr)
	{
		imageStopDiaporama->SetVisible(false);
		imagePlayDiaporama->SetVisible(true);

		wxWindow* window = this->FindWindowById(CENTRALVIEWERWINDOWID);
		if (window != nullptr)
		{
			wxCommandEvent evt(wxEVENT_STOPDIAPORAMA);
			window->GetEventHandler()->AddPendingEvent(evt);
		}

		this->Resize();
	}
}


void CPreviewToolbar::EventManager(const int& id)
{
	switch (id)
	{
	case WM_SAVE:
		Save();
		break;

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
	case IDM_EDIT:
		{
			wxWindow* central = this->FindWindowById(MAINVIEWERWINDOWID);
			auto event = new wxCommandEvent(wxEVENT_EDITFILE);
			wxQueueEvent(central, event);
			break;
		}
	case IDM_EXPORT:
		{
			wxWindow* central = this->FindWindowById(MAINVIEWERWINDOWID);
			auto event = new wxCommandEvent(wxEVENT_EXPORTFILE);
			wxQueueEvent(central, event);
			break;
		}
    
	default: ;
	}
}
