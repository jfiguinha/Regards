#include <header.h>
#include "CentralWindow.h"
#include "AnimationToolbar.h"
#include "ToolbarSlide.h"
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

	wxString libellePrevious = CLibResource::LoadStringFromResource(L"LBLPREVIOUS", 1);
	wxString libellePlay = CLibResource::LoadStringFromResource(L"LBLPLAY", 1);
	wxString libelleStop = CLibResource::LoadStringFromResource(L"LBLSTOP", 1);
	wxString libelleNext = CLibResource::LoadStringFromResource(L"LBLNEXT", 1);


	auto imagePrec = new CToolbarButton(themeToolbar.button);
	imagePrec->SetButtonResourceId(L"IDB_ARROWLPNG");
	imagePrec->SetLibelle(libellePrevious);
	imagePrec->SetCommandId(WM_IMAGES_PRCDENTE);
	imagePrec->SetRepeatable(true);
	navElement.push_back(imagePrec);

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

	auto imageSuiv = new CToolbarButton(themeToolbar.button);
	imageSuiv->SetButtonResourceId(L"IDB_ARROWRPNG");
	imageSuiv->SetCommandId(WM_IMAGES_SUIVANTE);
	imageSuiv->SetRepeatable(true);
	imageSuiv->SetLibelle(libelleNext);
	navElement.push_back(imageSuiv);

	this->toolbarInterface = toolbarInterface;
}


CAnimationToolbar::~CAnimationToolbar()
{
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
