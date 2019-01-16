#include "ToolbarElement.h"
using namespace Regards::Window;


CToolbarElement::CToolbarElement()
{
	isVisible = true;
	isRepeatable = false;
	isActif = false;
	isPush = false;
	isVertical = false;
	activePush = true;
}


CToolbarElement::~CToolbarElement()
{
}

void CToolbarElement::SetBackgroundBitmap(wxImage bmBackground)
{
    this->bmBackground = bmBackground;
}

void CToolbarElement::SetVertical(const bool &isVertical)
{
	this->isVertical = isVertical;
}

bool CToolbarElement::GetVertical()
{
	return isVertical;
}

void CToolbarElement::SetRepeatable(const bool &repeat)
{
	this->isRepeatable = repeat;
}

bool CToolbarElement::GetRepeatable()
{
	return isRepeatable;
}

bool CToolbarElement::IsVisible()
{
	return isVisible;
}

void CToolbarElement::SetVisible(const bool &value)
{
	isVisible = value;
}

void CToolbarElement::SetCommandId(const int &commandId)
{
	this->commandId = commandId;
}

int CToolbarElement::GetCommandId()
{
	return commandId;
}

void CToolbarElement::SetLibelleTooltip(const wxString &libelle)
{
	libelleTooltip = libelle;
}

wxString CToolbarElement::GetLibelleTooltip()
{
	return libelleTooltip;
}

void CToolbarElement::SetPosition(const int &x, const int &y)
{
	this->x = x;
	this->y = y;
}



int CToolbarElement::GetXPos()
{
	return x;
}

int CToolbarElement::GetYPos()
{
	return y;
}

void CToolbarElement::IsPushActif(const bool &activePush)
{
	this->activePush = activePush;
}

bool CToolbarElement::IsActif()
{
    return isActif;
}

bool CToolbarElement::SetInactif()
{
	if (isActif && !isPush)
	{
		isActif = false;
		return true;
	}
	return false;
}

bool CToolbarElement::SetActif()
{
	if (!isActif && !isPush)
	{
		isActif = true;
		return true;
	}
	return false;
}

bool CToolbarElement::FindElement(const int &xPos, const int &yPos)
{
	int left = x;
	int right = x + GetWidth();
	int top = y;
	int bottom = y + GetHeight();
	bool test;

	if ((left < xPos && xPos < right) && (top < yPos && yPos < bottom))
	{
		test = true;
	}
	else
	{
		test = false;
	}
	return test;
}

void CToolbarElement::SetPush(const bool &push)
{
	isPush = push;
}
