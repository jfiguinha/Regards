#include "MyCentralWindowIntro.h"
#include <RegardsBitmap.h>
#include <libResource.h>
#include <LoadingResource.h>
#include <wx/dcbuffer.h>
#include "IntroTheme.h"
using namespace Regards::Introduction;

CMyCentralWindowIntro::CMyCentralWindowIntro(wxWindow* parent, wxWindowID id) :
CWindowMain(parent, id)
{
	CThemeTree theme;
	CThemeIntroLogo themeIntro;
	CIntroTheme * config = new CIntroTheme();
	if (config != nullptr)
	{
		config->GetAboutTexteTheme(&themeIntro);

		titleIntro = new CTitleIntro(this, wxID_ANY, themeIntro);
		titleIntro->SetTitle(L"REGARDS V2");

		CThemeScrollBar themeScroll;
		config->GetScrollAboutTheme(&themeScroll);
		aboutWndScroll = new CScrollbarWnd(this, wxID_ANY);
			

		config->GetAboutTreeControlTheme(&theme);
		treeAbout = new CTreeWindow(aboutWndScroll, wxID_ANY, theme);
		aboutWndScroll->SetCentralWindow(treeAbout, themeScroll);

		infoAbout = new CInfoAbout(&theme);

		treeAbout->SetTreeControl(infoAbout);
		infoAbout->Init();

		Connect(wxEVT_SIZE, wxSizeEventHandler(CMyCentralWindowIntro::OnSize));

		delete config;
	}
}


CMyCentralWindowIntro::~CMyCentralWindowIntro()
{
	if (infoAbout != nullptr)
		delete(infoAbout);

	if (treeAbout != nullptr)
		delete(treeAbout);

	if (aboutWndScroll != nullptr)
		delete(aboutWndScroll);

	if (titleIntro != nullptr)
		delete(titleIntro);
}


void CMyCentralWindowIntro::Resize()
{
	titleIntro->SetSize(0, 0, width, titleIntro->GetHeight());
	aboutWndScroll->SetSize(0, titleIntro->GetHeight(), width, height - titleIntro->GetHeight());
	
}
