#pragma once
#include "TreeElement.h"
#include <WindowMain.h>
#include <Metadata.h>
#include "TreeElementSlideInterface.h"
using namespace Regards::Window;

#ifndef WX_PRECOMP
	#include <wx/combobox.h>
#endif

class CTreeElementListBox : public CTreeElement
{
public:
	CTreeElementListBox(CTreeElementSlideInterface * eventInterface);
	~CTreeElementListBox();

	void DrawElement(wxDC * deviceContext, const int &x, const int &y);
	void ClickElement(wxWindow * window, const int &x, const int &y);
	void SetTheme(CThemeTreeListBox * theme);

	CTreeElementListBox& operator=(const CTreeElementListBox &other);

	void SetZoneSize(const int &width, const int &height)
	{
		themeTreeListBox.SetWidth(width);
		themeTreeListBox.SetHeight(height);
	}

	void SetElementPos(const int &x, const int &y);
	
	void SetTabValue(const vector<CMetadata> & value, const int &index);
	void SetExifKey(const wxString &exifKey);

	void SetBackgroundColor(const wxColour &color)
	{
		themeTreeListBox.color = color;
	}

	int GetWidth()
	{
		return themeTreeListBox.GetWidth();
	}

	int GetHeight()
	{
		return themeTreeListBox.GetHeight();
	}


private:  
    
	wxBitmap CreateTriangle(const int &width, const int &height, const wxColor & color, const wxColor & colorBack);
	CTreeElementSlideInterface * eventInterface;
	wxString GetPositionValue();
	wxImage CreateFromSVG(const int & buttonWidth, const int & buttonHeight, const wxString &vectorCode);
	vector<CMetadata> tabValue;
	void TestMaxMinValue();
	CThemeTreeListBox themeTreeListBox;

	wxString buttonPlusVector;
	wxString buttonMoinsVector;
	wxString exifKey;
	wxImage buttonPlus;
	wxImage buttonMoins;
	wxRect plusPos;
	wxRect moinsPos;
	int position;
};

