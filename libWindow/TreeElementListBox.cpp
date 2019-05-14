#include "header.h"
#include "TreeElementListBox.h"
#include "LoadingResource.h"
#include <wxSVG/SVGDocument.h>
#include <wx/sstream.h>
#include <LibResource.h>


CTreeElementListBox::CTreeElementListBox(CTreeElementSlideInterface * eventInterface)
{
	this->eventInterface = eventInterface;
	position = 0;
}

wxBitmap CTreeElementListBox::CreateTriangle(const int &width, const int &height, const wxColor & color, const wxColor & colorBack)
{
	wxBitmap bitmapBuffer = wxBitmap(width, height);
	wxMemoryDC memDC(bitmapBuffer);
	wxBrush brushHatch(color);

	wxRect rc;
	rc.x = 0;
	rc.width = width;
	rc.y = 0;
	rc.height = height;
	CWindowMain::FillRect(&memDC, rc, colorBack);

	memDC.SetBrush(brushHatch);
	wxPoint star[3];
	star[0] = wxPoint(0, 0);
	star[1] = wxPoint(width, 0);
	star[2] = wxPoint(width / 2,height);
	memDC.DrawPolygon(WXSIZEOF(star), star, 0, 0);
	memDC.SetBrush(wxNullBrush);
    memDC.SelectObject(wxNullBitmap);
	return bitmapBuffer;
}

CTreeElementListBox& CTreeElementListBox::operator=(const CTreeElementListBox &other)
{
	visible = other.visible;
	xPos = other.xPos;
	yPos = other.yPos;
	numRow = other.numRow;
	numColumn = other.numColumn;
	themeTreeListBox = other.themeTreeListBox;
	position = other.position;
	tabValue = other.tabValue;
	exifKey = other.exifKey;
	eventInterface = other.eventInterface;
	return *this;
}

CTreeElementListBox::~CTreeElementListBox()
{

}

void CTreeElementListBox::SetElementPos(const int &x, const int &y)
{
	xPos = x;
	yPos = y;
}

wxString CTreeElementListBox::GetPositionValue()
{
	CMetadata metadata;
	if (tabValue.size() > position)
		metadata = tabValue.at(position);

	return metadata.value;
}

void CTreeElementListBox::SetTabValue(const vector<CMetadata> & value, const int &index)
{
	tabValue = value;
	position = index;
}

void CTreeElementListBox::SetExifKey(const wxString &exifKey)
{
	this->exifKey = exifKey;
}

void CTreeElementListBox::SetTheme(CThemeTreeListBox * theme)
{
	themeTreeListBox = *theme;

}
void CTreeElementListBox::TestMaxMinValue()
{
	if (position >= tabValue.size())
		position = int(tabValue.size()) - 1;

	if (position < 0)
		position = 0;
}

void CTreeElementListBox::ClickElement(wxWindow * window, const int &x, const int &y)
{
	if (x >= moinsPos.x && x < (moinsPos.width + moinsPos.x))
	{
		//Click button moins
		position--;
		TestMaxMinValue();
		CMetadata data = tabValue[position];
		CTreeElementValueInt elementValue(data.depth);
		eventInterface->SlidePosChange(this, position, &elementValue, exifKey);
	}
	else if (x >= plusPos.x && x < (plusPos.width + plusPos.x))
	{
		//Click button plus
		position++;
		TestMaxMinValue();
		CMetadata data = tabValue[position];
		CTreeElementValueInt elementValue(data.depth);
		eventInterface->SlidePosChange(this, position, &elementValue, exifKey);
	}
}


void CTreeElementListBox::DrawElement(wxDC * deviceContext, const int &x, const int &y)
{
	//bool oldRender = true;

	wxBitmap bitmapBuffer = wxBitmap(themeTreeListBox.GetWidth(), themeTreeListBox.GetHeight());
	wxMemoryDC memDC(bitmapBuffer);

	wxRect rc;
	rc.x = 0;
	rc.width = themeTreeListBox.GetWidth();
	rc.y = 0;
	rc.height = themeTreeListBox.GetHeight();
	CWindowMain::FillRect(&memDC, rc, themeTreeListBox.color);

	wxSize renderElement = CWindowMain::GetSizeTexte(deviceContext, GetPositionValue(), themeTreeListBox.font);

	int yMedium = (themeTreeListBox.GetHeight() - renderElement.y) / 2;
	CWindowMain::DrawTexte(&memDC, GetPositionValue(), 0, yMedium, themeTreeListBox.font);

	if (!buttonMoins.IsOk() || (buttonMoins.GetWidth() != themeTreeListBox.GetButtonWidth() || buttonMoins.GetHeight() != themeTreeListBox.GetButtonHeight()))
	{
		buttonMoins = CLibResource::CreatePictureFromSVG("IDB_MINUS", themeTreeListBox.GetButtonWidth(), themeTreeListBox.GetButtonHeight()); 
	}
	

	if (!buttonPlus.IsOk() || (buttonPlus.GetWidth() != themeTreeListBox.GetButtonWidth() || buttonPlus.GetHeight() != themeTreeListBox.GetButtonHeight()))
	{
		buttonPlus = CLibResource::CreatePictureFromSVG("IDB_PLUS", themeTreeListBox.GetButtonWidth(), themeTreeListBox.GetButtonHeight());
	}



	moinsPos.x = themeTreeListBox.GetWidth() - (buttonMoins.GetWidth() + themeTreeListBox.GetMarge() + buttonPlus.GetWidth() + themeTreeListBox.GetMarge());
	moinsPos.y =  (themeTreeListBox.GetHeight()  - themeTreeListBox.GetButtonHeight()) / 2;
	moinsPos.width = themeTreeListBox.GetButtonWidth();
	moinsPos.height = themeTreeListBox.GetButtonHeight();
	memDC.DrawBitmap(buttonMoins.ConvertToDisabled(), moinsPos.x, moinsPos.y);

	plusPos.x = moinsPos.x + buttonMoins.GetWidth() + 2 * themeTreeListBox.GetMarge();
	plusPos.y =(themeTreeListBox.GetHeight()  - themeTreeListBox.GetButtonHeight()) / 2;
	plusPos.width = themeTreeListBox.GetButtonWidth();
	plusPos.height = themeTreeListBox.GetButtonHeight();
	memDC.DrawBitmap(buttonPlus.ConvertToDisabled(), plusPos.x, plusPos.y);


	
	//memDC.DrawBitmap(buttonPlus, plusPos.x, plusPos.y);

	memDC.SelectObject(wxNullBitmap);

	deviceContext->DrawBitmap(bitmapBuffer, x, y);
}