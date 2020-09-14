#include "header.h"
#include "TreeElementStar.h"
#include "LibResource.h"
#include <wxSVG/SVGDocument.h>
#include <wx/sstream.h>
using namespace Regards::Window;

CTreeElementStar::CTreeElementStar()
{
	value = 0;
}


CTreeElementStar::~CTreeElementStar()
{
}


wxImage CTreeElementStar::CreateFromSVG(const int & buttonWidth, const int & buttonHeight, const wxString &vector)
{
	wxImage img;
	if (vector.size() > 0)
	{
		wxStringInputStream memBuffer(vector);
		wxSVGDocument svgDoc;
		svgDoc.Load(memBuffer);
		img = svgDoc.Render(buttonWidth, buttonHeight, NULL, true, true);

	}
	else
	{
		img.Create(buttonWidth, buttonHeight);
	}
	return img;
}

void CTreeElementStar::CreateStar()
{
	wxString star = CLibResource::GetVector(L"IDB_STAREMPTY");
	starEmpty = CreateFromSVG(themeTriangle.GetWidth(), themeTriangle.GetHeight(), star);
	star = CLibResource::GetVector(L"IDB_STARYELLOW");
	starYellow = CreateFromSVG(themeTriangle.GetWidth(), themeTriangle.GetHeight(), star);
}


void CTreeElementStar::SetTheme(CThemeTreeTriangle * theme)
{
	themeTriangle = *theme;
}

void CTreeElementStar::SetValue(const int &value)
{
	this->value = value;
}

int CTreeElementStar::GetValue()
{
	return value;
}

void CTreeElementStar::DrawElement(wxDC * deviceContext, const int &x, const int &y)
{
	DrawStar(deviceContext, x, y);
}

void CTreeElementStar::DrawStar(wxDC * dc, const int &x, const int &y)
{
	int xPos = x;
	for (int i = 0; i < 5; i++)
	{
		if(value > i)
		dc->DrawBitmap(starEmpty, xPos, y);
		xPos += starEmpty.GetWidth();
	}
}

void CTreeElementStar::ClickElement(wxWindow * window, const int &x, const int &y)
{

}
