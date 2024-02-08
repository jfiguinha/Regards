#include <header.h>
#include "MyFrameIntro.h"
#include "IntroTheme.h"
#include "TitleIntro.h"
using namespace Regards::Introduction;

#ifndef wxHAS_IMAGES_IN_RESOURCES
#include "../Resource/sample.xpm"
#endif

MyFrameIntro::MyFrameIntro(const wxString& title, const wxString& appTitle, const wxPoint& pos, const wxSize& size,
	ISCannerInterface * mainInterface)
	: wxFrame(nullptr, wxID_ANY, title, pos, size, wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN)
{
	CThemeScrollBar themeScroll;
	CThemeTree theme;
	CThemeIntroLogo themeIntro;


	SetIcon(wxICON(sample));
	this->mainInterface = mainInterface;
	std::unique_ptr<CIntroTheme> introTheme = std::unique_ptr<CIntroTheme>(new CIntroTheme());


	titleIntro = nullptr;

	if (introTheme != nullptr)
	{
		introTheme->GetScrollTheme(&themeScroll);
		introTheme->GetTreeTheme(&theme);

		myCentralWindow = std::unique_ptr<CMyCentralWindowIntro>(new CMyCentralWindowIntro(this, wxID_ANY, themeScroll, theme));
		introTheme->GetAboutTexteTheme(&themeIntro);

		titleIntro = std::unique_ptr<CTitleIntro>(new CTitleIntro(this, wxID_ANY, themeIntro));
		titleIntro->SetTitle(appTitle);

	}

	diaporamaTimer = std::unique_ptr<wxTimer>(new wxTimer(this, wxTIMER_DIAPORAMA));

	Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(MyFrameIntro::OnClose));
	Connect(wxEVT_SIZE, wxSizeEventHandler(MyFrameIntro::on_size));
	Connect(wxTIMER_DIAPORAMA, wxEVT_TIMER, wxTimerEventHandler(MyFrameIntro::OnTimeShowViewer), nullptr, this);
	//CreateStatusBar();

	diaporamaTimer->Start(200, wxTIMER_ONE_SHOT);
}

void MyFrameIntro::on_size(wxSizeEvent& event)
{
	wxSize size = event.GetSize();
	titleIntro->SetSize(0, 0, size.GetWidth(), titleIntro->GetHeight());
	myCentralWindow->SetSize(0, titleIntro->GetHeight(), size.GetWidth(), size.GetHeight() - titleIntro->GetHeight());
}

void MyFrameIntro::OnTimeShowViewer(wxTimerEvent& event)
{
	if (mainInterface != nullptr)
		mainInterface->ShowViewer();
}

MyFrameIntro::~MyFrameIntro()
{
	if (diaporamaTimer->IsRunning())
		diaporamaTimer->Stop();

}

void MyFrameIntro::OnClose(wxCloseEvent& event)
{
	if (mainInterface != nullptr)
		mainInterface->HideAbout();
}
