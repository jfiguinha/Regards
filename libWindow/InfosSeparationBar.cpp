#include "header.h"
#include "InfosSeparationBar.h"
#include <ConvertUtility.h>
#include "WindowMain.h"
#include <membitmap.h>
using namespace Regards::Window;
//const unsigned int cuStackSize = 128 * 1024;

#define WM_PICTURELOAD 1


void CInfosSeparationBar::OnClick(const int& x, const int& y)
{
}

int CInfosSeparationBar::GetXPos()
{
	return _xPos;
}

int CInfosSeparationBar::GetYPos()
{
	return _yPos;
}

const int& CInfosSeparationBar::GetWidth()
{
	return width;
}

const int& CInfosSeparationBar::GetHeight()
{
	return theme.GetHeight();
}

void CInfosSeparationBar::Clear()
{
	listElement.clear();
	listElement.reserve(0);
}

void CInfosSeparationBar::SetTitle(const wxString& title)
{
	this->title = title;
}

bool CInfosSeparationBar::operator ==(const CInfosSeparationBar& n2)
{
	int left = _xPos;
	int right = _xPos + width;
	int top = _yPos;
	int bottom = _yPos + GetHeight();

	if ((left < n2._xPos && n2._xPos < right) && (top < n2._yPos && n2._yPos < bottom))
	{
		return true;
	}
	return false;
}

void CInfosSeparationBar::SetWindowPos(const int& x, const int& y)
{
	this->_xPos = x;
	this->_yPos = y;
}

wxRect CInfosSeparationBar::GetPos()
{
	wxRect rc;
	rc.x = _xPos;
	rc.width = width;
	rc.y = _yPos;
	rc.height = theme.GetHeight();
	return rc;
}

void CInfosSeparationBar::SetWidth(const int& width)
{
	if (width != this->width)
	{
		this->width = width;
	}
}


CInfosSeparationBar::CInfosSeparationBar(const CThemeInfosSeparationBar& theme): _xPos(0), _yPos(0), width(0)
{
	this->theme = theme;
	pimpl = new CMemBitmap(0, 0);
}


CInfosSeparationBar::~CInfosSeparationBar(void)
{
	listElement.clear();
	listElement.reserve(0);
	delete pimpl;
}

void CInfosSeparationBar::RenderTitle(wxDC* dc)
{
	wxRect rc;
	int x = 0;
	int y = 0;
	rc.x = 0;
	rc.y = 0;
	rc.width = width;
	rc.height = GetHeight();
	CWindowMain::FillRect(dc, rc, theme.colorBack);

	int posY = 0;
	//int i = 0;

	vector<wxString> listOfTexte = CConvertUtility::split(title, '@');
	if (listOfTexte.size() > 0)
	{
		int sizeOfY = GetHeight() / listOfTexte.size();

		for (wxString title : listOfTexte)
		{
			if (title != L"")
			{
				wxSize size = CWindowMain::GetSizeTexte( title, theme.themeFont);
				int localy = y + posY + (sizeOfY - size.y) / 2;
				int localx = x + (width - size.x) / 2;
				CWindowMain::DrawTexte(dc, title, localx, localy, theme.themeFont);
				posY += sizeOfY;

				titleRectPos.x = localx;
				titleRectPos.y = localy;
				titleRectPos.width = size.x;
				titleRectPos.height = size.y;
			}
		}
	}
}

void  CInfosSeparationBar::SetNbElementX(const int& nbElementX)
{
	this->nbElementX = nbElementX;
}

void CInfosSeparationBar::SetNbElementY(const int& nbElementY)
{
	this->nbElementY = nbElementY;
}

int  CInfosSeparationBar::GetNbElementX()
{
	return nbElementX;
}

int  CInfosSeparationBar::GetNbElementY()
{
	return nbElementY;
}

void CInfosSeparationBar::Render(wxDC* dc, const int& posLargeur, const int& posHauteur)
{
	pimpl->SetWindowSize(GetWidth(), GetHeight());
	RenderIcone(&pimpl->sourceDCContext, posLargeur, posHauteur);
	dc->DrawBitmap(pimpl->memBitmap, _xPos + posLargeur, _yPos + posHauteur);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void CInfosSeparationBar::RenderIcone(wxDC* dc, const int& posLargeur, const int& posHauteur)
{
	RenderTitle(dc);
}
