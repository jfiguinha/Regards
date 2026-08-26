#include <header.h>
#include "CentralWindow.h"
#include "AnimationToolbar.h"
#include <LibResource.h>
#include <ToolbarButton.h>
#if defined(__WXMSW__)
#include "../include/window_id.h"
#else
#include <window_id.h>
#endif
#include <ToolbarInterface.h>
using namespace Regards::Window;
using namespace Regards::Viewer;


CAnimationToolbar::CAnimationToolbar(wxWindow* parent, wxWindowID id, const CThemeToolbar& theme,
                                     CToolbarInterface* toolbarInterface, const bool& vertical)
	: CToolbarWindow(parent, id, theme, vertical)
{
	imagePlayDiaporama = nullptr;
	imageStopDiaporama = nullptr;

	imagePrec = CreateButton(L"IDB_ARROWLPNG", L"LBLPREVIOUS", WM_IMAGES_PRCDENTE);
	imagePrec->SetRepeatable(true);
	imagePlayDiaporama = CreateButton(L"IDB_PLAY", L"LBLPLAY", WM_DIAPORAMA_PLAY);
	imageStopDiaporama = CreateButton(L"IDB_STOP", L"LBLSTOP", WM_DIAPORAMA_STOP);
	imageStopDiaporama->SetVisible(false);
	imageSuiv = CreateButton(L"IDB_ARROWRPNG", L"LBLNEXT", WM_IMAGES_SUIVANTE);
	imageSuiv->SetRepeatable(true);


	this->toolbarInterface = toolbarInterface;
}

void CAnimationToolbar::NextPicture()
{
	auto mainWindow = static_cast<CCentralWindow*>(this->FindWindowById(CENTRALVIEWERWINDOWID));
	if (mainWindow != nullptr)
		mainWindow->AnimationPictureNext();
}

void CAnimationToolbar::PreviousPicture()
{
	auto mainWindow = static_cast<CCentralWindow*>(this->FindWindowById(CENTRALVIEWERWINDOWID));
	if (mainWindow != nullptr)
		mainWindow->AnimationPicturePrevious();
}

void CAnimationToolbar::AnimationStart()
{
	auto mainWindow = static_cast<CCentralWindow*>(this->FindWindowById(CENTRALVIEWERWINDOWID));
	if (mainWindow != nullptr)
	{
		imagePlayDiaporama->SetVisible(false);
		imageStopDiaporama->SetVisible(true);
		auto event = new wxCommandEvent(wxEVENT_ANIMATIONSTART);
		wxQueueEvent(mainWindow, event);
		needToRefresh = true;
	}
}

void CAnimationToolbar::AnimationStop()
{
	auto mainWindow = static_cast<CCentralWindow*>(this->FindWindowById(CENTRALVIEWERWINDOWID));
	if (mainWindow != nullptr)
	{
		imageStopDiaporama->SetVisible(false);
		imagePlayDiaporama->SetVisible(true);

		auto event = new wxCommandEvent(wxEVENT_ANIMATIONSTOP);
		wxQueueEvent(mainWindow, event);

		needToRefresh = true;
	}
}


void CAnimationToolbar::EventManager(const int& id)
{
	switch (id)
	{
	case WM_DIAPORAMA_PLAY:
		AnimationStart();
		break;

	case WM_DIAPORAMA_STOP:
		AnimationStop();
		break;

	case WM_IMAGES_PRCDENTE:
		PreviousPicture();
		break;

	case WM_IMAGES_SUIVANTE:
		NextPicture();
		break;
	default: ;
	}
}
