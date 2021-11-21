#include "header.h"
#include "TreeElementStar.h"
#include "LibResource.h"
#include <wx/sstream.h>
#include <SqlCriteria.h>
#include <SqlPhotoCriteria.h>
#include <WindowMain.h>
using namespace Regards::Sqlite;
using namespace Regards::Window;

CTreeElementStar::CTreeElementStar()
{
	value = 0;
	CreateStar();
}


CTreeElementStar::~CTreeElementStar()
{
}

void CTreeElementStar::CreateStar()
{
	starEmpty = CLibResource::CreatePictureFromSVG(L"IDB_STAREMPTY", themeTriangle.GetWidth() * 2, themeTriangle.GetHeight() * 2);
	starYellow = CLibResource::CreatePictureFromSVG(L"IDB_STARYELLOW", themeTriangle.GetWidth() * 2, themeTriangle.GetHeight() * 2);
}

void CTreeElementStar::SetNumPhoto(const int& numPhotoId)
{
	this->numPhotoId = numPhotoId;
}

void CTreeElementStar::SetTheme(CThemeTreeTriangle* theme)
{
	themeTriangle = *theme;
}

void CTreeElementStar::SetValue(const int& value)
{
	this->value = value;
}

int CTreeElementStar::GetValue()
{
	return value;
}

void CTreeElementStar::DrawElement(wxDC* deviceContext, const int& x, const int& y)
{
	localx = x;
	localy = y;
	DrawStar(deviceContext, x, y);
}

void CTreeElementStar::DrawStar(wxDC* dc, const int& x, const int& y)
{
	int xPos = x;
	for (int i = 0; i < 5; i++)
	{
		if (value > i)
			dc->DrawBitmap(starYellow, xPos, y);
		else
			dc->DrawBitmap(starEmpty, xPos, y);
		xPos += starEmpty.GetWidth();
	}
}

void CTreeElementStar::ClickElement(wxWindow* window, const int& x, const int& y)
{
	CSqlPhotoCriteria sqlPhotoCriteria;
	CSqlCriteria sqlCriteria;
	bool isNew = false;
	if (value > 0 && value <= 5)
	{
		wxString libelle = to_string(value) + " Star";
		int oldCriteria = sqlCriteria.GetOrInsertCriteriaId(1, 6, libelle, isNew);
		sqlPhotoCriteria.DeletePhotoCriteria(numPhotoId, oldCriteria);
	}


	int maxX = 5 * starYellow.GetWidth() + localx;
	//int maxY = localy + starYellow.GetHeight();
	if (x > localx && x < maxX)
	{
		value = (x - localx) / starYellow.GetWidth();
		if ((x - localx) < starYellow.GetWidth() / 2)
			value = 0;
		else
			value++;
	}

	if (value >= 0 && value <= 5)
	{
		wxString libelle = to_string(value) + " Star";
		int newCriteria = sqlCriteria.GetOrInsertCriteriaId(1, 6, libelle, isNew);
		sqlPhotoCriteria.InsertPhotoCriteria(numPhotoId, newCriteria);
	}
}
