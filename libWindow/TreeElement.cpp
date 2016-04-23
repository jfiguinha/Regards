
#include "TreeElement.h"


CTreeElement::CTreeElement()
{
	visible = true;
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