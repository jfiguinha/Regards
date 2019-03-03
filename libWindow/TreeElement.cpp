#include "header.h"
#include "TreeElement.h"


CTreeElement::CTreeElement()
{
	visible = true;
	xPos = 0;
	yPos = 0;
	numRow = 0;
	numColumn = 0;
}


CTreeElement::~CTreeElement()
{
}

void CTreeElement::SetElementPos(const int &x, const int &y)
{
	xPos = x;
	yPos = y;
}

void CTreeElement::SetVisible(const bool &visible)
{
	this->visible = visible;
}

bool CTreeElement::IsVisible()
{
	return visible;
}


void CTreeElement::SetBackgroundColor(const wxColour &color)
{
	backcolor = color;
}