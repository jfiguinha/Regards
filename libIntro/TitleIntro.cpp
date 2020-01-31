#include <header.h>
#include "TitleIntro.h"
using namespace Regards::Introduction;


CTitleIntro::CTitleIntro(wxWindow* parent, wxWindowID id, const CThemeIntroLogo & theme)
	: CWindowMain("CTitleIntro",parent, id)
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

void CTitleIntro::UpdateScreenRatio()
{
	this->Resize();
}

int CTitleIntro::GetHeight()
{
	wxClientDC dc(this);
	wxSize size = GetSizeTexte(&dc, title, themeFont.font);
	return size.y;
}

void CTitleIntro::OnPaint(wxPaintEvent& event)
{

    int width = GetWindowWidth();
    int height = GetWindowHeight();
    if(width == 0 || height == 0)
        return;  
	
    wxPaintDC dc(this);
	FillRect(&dc, GetWindowRect(), themeFont.colorBack);
	wxSize size = GetSizeTexte(&dc, title, themeFont.font);
	int posX = (GetWindowWidth() - size.x) / 2;
	DrawTexte(&dc, title, posX, 0, themeFont.font);
	
}