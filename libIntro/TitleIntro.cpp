#include "TitleIntro.h"
using namespace Regards::Introduction;


CTitleIntro::CTitleIntro(wxWindow* parent, wxWindowID id, const CThemeIntroLogo & theme)
	: CWindowMain(parent, id)
{
	this->themeFont = theme;
	Connect(wxEVT_PAINT, wxPaintEventHandler(CTitleIntro::OnPaint));

}

void CTitleIntro::SetTitle(const wxString &title)
{
	this->title = title;
}


CTitleIntro::~CTitleIntro()
{

}

int CTitleIntro::GetHeight()
{
	wxWindowDC dc(this);
	wxSize size = GetSizeTexte(&dc, title, themeFont.font);
	return size.y;
}

void CTitleIntro::Resize()
{
	this->Refresh();
}

void CTitleIntro::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	FillRect(&dc, GetWindowRect(), themeFont.colorBack);
	wxSize size = GetSizeTexte(&dc, title, themeFont.font);
	int posX = (width - size.x) / 2;
	DrawTexte(&dc, title, posX, 0, themeFont.font);
	
}