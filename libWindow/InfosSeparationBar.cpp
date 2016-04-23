#include "InfosSeparationBar.h"
#include <ConvertUtility.h>
#include "WindowMain.h"
using namespace Regards::Window;
//const unsigned int cuStackSize = 128 * 1024;

#define WM_PICTURELOAD 1


void CInfosSeparationBar::OnClick(const int &x, const int &y)
{

}

int CInfosSeparationBar::GetXPos()
{
	return x;
}
int CInfosSeparationBar::GetYPos()
{
	return y;
}

const int & CInfosSeparationBar::GetWidth()
{
	return width;
}
const int & CInfosSeparationBar::GetHeight()
{
    return theme.GetHeight();
}

void CInfosSeparationBar::Clear()
{
	listElement.clear();
	listElement.reserve(0);
}

void CInfosSeparationBar::SetTitle(const wxString &title)
{
	this->title = title;
}

bool CInfosSeparationBar::operator == (const CInfosSeparationBar &n2)
{
	int left = x;
	int right = x + width;
	int top = y;
	int bottom = y + theme.GetHeight();

	if ((left < n2.x && n2.x < right) && (top < n2.y && n2.y < bottom))
	{
		return true;
	}
	return false;
}

void CInfosSeparationBar::SetWindowPos(const int &x, const int &y)
{
	this->x = x;
	this->y = y;
}

wxRect CInfosSeparationBar::GetPos()
{
	wxRect rc;
	rc.x = x;
	rc.width = width;
	rc.y = y;
	rc.height = theme.GetHeight();
	return rc;
}

void CInfosSeparationBar::SetWidth(const int &width)
{
	if (width != this->width)
	{
		this->width = width;
	}
}


CInfosSeparationBar::CInfosSeparationBar(const CThemeInfosSeparationBar &theme)
{
	this->theme = theme;
}


CInfosSeparationBar::~CInfosSeparationBar(void)
{
	listElement.clear();
	listElement.reserve(0);
}

void CInfosSeparationBar::RenderTitle(wxDC * dc)
{
	wxRect rc;
	rc.x = x;
	rc.y = y;
	rc.width = width;
	rc.height = theme.GetHeight();
	CWindowMain::FillRect(dc, rc, theme.colorBack);

	int posY = 0;
	//int i = 0;

	vector<wxString> listOfTexte = CConvertUtility::split(title, '@');
	if (listOfTexte.size() > 0)
	{
		int sizeOfY = theme.GetHeight() / listOfTexte.size();

		for (wxString title : listOfTexte)
		{
			if (title != L"")
			{
				wxSize size = CWindowMain::GetSizeTexte(dc, title, theme.themeFont);
				int localy = y + posY + (sizeOfY - size.y) / 2;
				int localx = x + (width - size.x) / 2;
				CWindowMain::DrawTexte(dc, title, localx, localy, theme.themeFont);
				posY += sizeOfY;
			}
		}

	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void CInfosSeparationBar::RenderIcone(wxDC * dc)
{
	RenderTitle(dc);
}

