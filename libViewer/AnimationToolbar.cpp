#include <header.h>
#include "AnimationToolbar.h"
#include "ToolbarSlide.h"
#include "ViewerParamInit.h"
#include <ConfigParam.h>
#include <LibResource.h>
#include "ViewerParam.h"
#include "CentralWnd.h"
#if defined(__WXMSW__)
#include "../include/window_id.h"
#else
#include <window_id.h>
#endif
using namespace Regards::Window;
using namespace Regards::Viewer;


CAnimationToolbar::CAnimationToolbar(wxWindow* parent, wxWindowID id, const CThemeToolbar & theme, CToolbarInterface * toolbarInterface)
	: CToolbarWindow(parent, id, theme)
{
	imagePlayDiaporama = nullptr;
	imageStopDiaporama = nullptr;

	wxString libellePrevious = CLibResource::LoadStringFromResource(L"LBLPREVIOUS",1);
	wxString libellePlay = CLibResource::LoadStringFromResource(L"LBLPLAY",1);
	wxString libelleStop = CLibResource::LoadStringFromResource(L"LBLSTOP",1);
	wxString libelleNext = CLibResource::LoadStringFromResource(L"LBLNEXT",1);


	CToolbarButton * imagePrec = new CToolbarButton(themeToolbar.button);
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

	CToolbarButton * imageSuiv = new CToolbarButton(themeToolbar.button);
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
	CCentralWnd * mainWindow = (CCentralWnd *)this->FindWindowById(CENTRALVIEWERWINDOWID);
	if (mainWindow != nullptr)
		mainWindow->AnimationPictureNext();
}

void CAnimationToolbar::PreviousPicture()
{
	CCentralWnd * mainWindow = (CCentralWnd *)this->FindWindowById(CENTRALVIEWERWINDOWID);
	if (mainWindow != nullptr)
		mainWindow->AnimationPicturePrevious();
}

void CAnimationToolbar::AnimationStart()
{
	CCentralWnd * mainWindow = (CCentralWnd *)this->FindWindowById(CENTRALVIEWERWINDOWID);
	if (mainWindow != nullptr)
	{
		imagePlayDiaporama->SetVisible(false);
		imageStopDiaporama->SetVisible(true);
		mainWindow->StartAnimation();
		this->FastRefresh(this);
	}
}

void CAnimationToolbar::AnimationStop()
{
	CCentralWnd * mainWindow = (CCentralWnd *)this->FindWindowById(CENTRALVIEWERWINDOWID);
	if (mainWindow != nullptr)
	{
		imageStopDiaporama->SetVisible(false);
		imagePlayDiaporama->SetVisible(true);
		mainWindow->StopAnimation();
		this->FastRefresh(this);
	}
}


void CAnimationToolbar::EventManager(const int &id)
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
	}
}




