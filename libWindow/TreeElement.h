#pragma once
#include <Theme.h>
#include <vector>
using namespace std;


class CTreeElement
{
public:
	CTreeElement();
	virtual ~CTreeElement();

	virtual int GetWidth() = 0;
	virtual int GetHeight() = 0;

	int GetPosX(){
		return xPos;
	};
	int GetPosY(){
		return yPos;
	};

	void SetVisible(const bool &visible);
	bool IsVisible();
    

	virtual void MouseOver(wxDC * context, const int &x, const int &y)
	{
		::wxSetCursor(wxCursor(wxCURSOR_HAND));
	};

	virtual void MouseOut(wxDC * context, const int &x, const int &y)
	{
		::wxSetCursor(wxCursor(wxCURSOR_HAND));
	};

	virtual void SetZoneSize(const int &width, const int &height) = 0;
	virtual void SetElementPos(const int &x, const int &y);
	virtual void UnclickElement(wxWindow * window, const int &x, const int &y){};
	virtual void ClickElement(wxWindow * window, const int &x, const int &y){};
	virtual void DrawElement(wxDC * context, const int &x, const int &y){};

	virtual void SetBackgroundColor(const wxColour &color);

	
	void SetColumn(const int &numColumn)
	{
		this->numColumn = numColumn;
	}

	int GetColumn()
	{
		return numColumn;
	}

	void SetRow(const int &numRow)
	{
		this->numRow = numRow;
	}

	int GetRow()
	{
		return numRow;
	}

protected:

	bool visible;
	int xPos = 0;
	int yPos = 0;
	int numRow = 0;
	int numColumn = 0;
	wxColour backcolor;
};

typedef vector<CTreeElement *> VecTreeElement;
