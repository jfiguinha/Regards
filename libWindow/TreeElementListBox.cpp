#include "header.h"
#include "TreeElementListBox.h"
#include <wx/sstream.h>
#include <LibResource.h>
#include <membitmap.h>
using namespace Regards::Window;

CTreeElementListBox::CTreeElementListBox(CTreeElementSlideInterface* eventInterface)
{
	this->eventInterface = eventInterface;
	position = 0;
	pimpl = new CMemBitmap(0, 0);
}

wxBitmap CTreeElementListBox::CreateTriangle(const int& width, const int& height, const wxColor& color,
                                             const wxColor& colorBack)
{
	pimpl->SetWindowSize(width, height);
	wxBrush brushHatch(color);

	wxRect rc;
	rc.x = 0;
	rc.width = width;
	rc.y = 0;
	rc.height = height;
	CWindowMain::FillRect(&pimpl->sourceDCContext, rc, colorBack);

	pimpl->sourceDCContext.SetBrush(brushHatch);
	wxPoint star[3];
	star[0] = wxPoint(0, 0);
	star[1] = wxPoint(width, 0);
	star[2] = wxPoint(width / 2, height);
	pimpl->sourceDCContext.DrawPolygon(WXSIZEOF(star), star, 0, 0);
	pimpl->sourceDCContext.SetBrush(wxNullBrush);
	pimpl->sourceDCContext.SelectObject(wxNullBitmap);
	return pimpl->memBitmap;
}

CTreeElementListBox& CTreeElementListBox::operator=(const CTreeElementListBox& other)
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
	delete pimpl;
}

void CTreeElementListBox::SetElementPos(const int& x, const int& y)
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

void CTreeElementListBox::SetTabValue(const vector<CMetadata>& value, const int& index)
{
	tabValue = value;
	position = index;
}

void CTreeElementListBox::SetExifKey(const wxString& exifKey)
{
	this->exifKey = exifKey;
}

void CTreeElementListBox::SetTheme(CThemeTreeListBox* theme)
{
	themeTreeListBox = *theme;
}

void CTreeElementListBox::TestMaxMinValue()
{
	if (position >= tabValue.size())
		position = static_cast<int>(tabValue.size()) - 1;

	if (position < 0)
		position = 0;
}

void CTreeElementListBox::ClickElement(wxWindow* window, const int& x, const int& y)
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


void CTreeElementListBox::DrawElement(wxDC* deviceContext, const int& x, const int& y)
{
	//bool oldRender = true;
	pimpl->SetWindowSize(themeTreeListBox.GetWidth(), themeTreeListBox.GetHeight());

	wxRect rc;
	rc.x = 0;
	rc.width = themeTreeListBox.GetWidth();
	rc.y = 0;
	rc.height = themeTreeListBox.GetHeight();
	CWindowMain::FillRect(&pimpl->sourceDCContext, rc, themeTreeListBox.color);

	wxSize renderElement = CWindowMain::GetSizeTexte(GetPositionValue(), themeTreeListBox.font);

	int yMedium = (themeTreeListBox.GetHeight() - renderElement.y) / 2;
	CWindowMain::DrawTexte(&pimpl->sourceDCContext, GetPositionValue(), 0, yMedium, themeTreeListBox.font);

	if (!buttonMoins.IsOk() || (buttonMoins.GetWidth() != themeTreeListBox.GetButtonWidth() || buttonMoins.GetHeight()
		!= themeTreeListBox.GetButtonHeight()))
	{
		buttonMoins = CLibResource::CreatePictureFromSVG("IDB_MINUS", themeTreeListBox.GetButtonWidth(),
		                                                 themeTreeListBox.GetButtonHeight());
	}


	if (!buttonPlus.IsOk() || (buttonPlus.GetWidth() != themeTreeListBox.GetButtonWidth() || buttonPlus.GetHeight() !=
		themeTreeListBox.GetButtonHeight()))
	{
		buttonPlus = CLibResource::CreatePictureFromSVG("IDB_PLUS", themeTreeListBox.GetButtonWidth(),
		                                                themeTreeListBox.GetButtonHeight());
	}


	moinsPos.x = themeTreeListBox.GetWidth() - (buttonMoins.GetWidth() + themeTreeListBox.GetMarge() + buttonPlus.
		GetWidth() + themeTreeListBox.GetMarge());
	moinsPos.y = (themeTreeListBox.GetHeight() - themeTreeListBox.GetButtonHeight()) / 2;
	moinsPos.width = themeTreeListBox.GetButtonWidth();
	moinsPos.height = themeTreeListBox.GetButtonHeight();
	pimpl->sourceDCContext.DrawBitmap(buttonMoins.ConvertToDisabled(), moinsPos.x, moinsPos.y);

	plusPos.x = moinsPos.x + buttonMoins.GetWidth() + 2 * themeTreeListBox.GetMarge();
	plusPos.y = (themeTreeListBox.GetHeight() - themeTreeListBox.GetButtonHeight()) / 2;
	plusPos.width = themeTreeListBox.GetButtonWidth();
	plusPos.height = themeTreeListBox.GetButtonHeight();
	pimpl->sourceDCContext.DrawBitmap(buttonPlus.ConvertToDisabled(), plusPos.x, plusPos.y);


	//memDC.DrawBitmap(buttonPlus, plusPos.x, plusPos.y);

	pimpl->sourceDCContext.SelectObject(wxNullBitmap);

	deviceContext->DrawBitmap(pimpl->memBitmap, x, y);
}
