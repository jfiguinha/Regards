#include <header.h>
#include "MyFrameIntro.h"
using namespace Regards::Introduction;

#ifndef wxHAS_IMAGES_IN_RESOURCES
#ifdef __WXGTK__
#include "../Resource/sample.xpm"
#else
#include "../../Resource/sample.xpm"
#endif
#endif

MyFrameIntro::MyFrameIntro(const wxString& title, const wxPoint& pos, const wxSize& size, IMainInterface * mainInterface)
	: wxFrame(nullptr, wxID_ANY, title, pos, size,   wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN)
{
	SetIcon(wxICON(sample));
	this->mainInterface = mainInterface;
	myCentralWindow = new CMyCentralWindowIntro(this, wxID_ANY);
	Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(MyFrameIntro::OnClose));
	//CreateStatusBar();
}

MyFrameIntro::~MyFrameIntro()
{
	delete(myCentralWindow);
}

void MyFrameIntro::OnClose(wxCloseEvent& event)
{
	mainInterface->HideAbout();
	return;
}

