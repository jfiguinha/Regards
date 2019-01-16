
#include "PositionElement.h"
#include "TreeData.h"
#include "TreeElement.h"

CPositionElement::~CPositionElement()
{
	delete(treeElement);
}

int CPositionElement::GetRow()
{
	return numRow;
}
void CPositionElement::SetRow(const int &row)
{
	numRow = row;
}

int CPositionElement::GetColumn()
{
	return numColumn;
}

void CPositionElement::SetColumn(const int &column)
{
	numColumn = column;
}

int CPositionElement::GetType()
{
	return type;
}

void CPositionElement::SetType(const int &type)
{
	this->type = type;
}

int CPositionElement::GetX()
{
	return x;
}

void CPositionElement::SetX(const int &x)
{
	this->x = x;
}

int CPositionElement::GetY()
{
	return y;
}

void CPositionElement::SetY(const int &y)
{
	this->y = y;
}

int CPositionElement::GetWidth()
{
	return treeElement->GetWidth();
}

void CPositionElement::SetWidth(const int &width)
{
	this->width = width;
}

int CPositionElement::GetHeight()
{
	return treeElement->GetHeight();
}

void CPositionElement::SetHeight(const int &height)
{
	this->height = height;
}

CTreeData * CPositionElement::GetTreeData()
{
	return treeData;
}

void CPositionElement::SetTreeData(CTreeData * treeData)
{
	this->treeData = treeData;
}

CTreeElement * CPositionElement::GetTreeElement()
{
	return treeElement;
}

void CPositionElement::SetTreeElement(CTreeElement * treeElement)
{
	this->treeElement = treeElement;
}

void CPositionElement::SetColorBackground(const wxColour &colorBackground)
{
	this->colorBackground = colorBackground;
}

wxColour CPositionElement::GetColorBackground()
{
	return colorBackground;
}

void CPositionElement::DrawElement(wxDC * deviceContext, const int &xStartElement, const int &xposLargeur, const int &posHauteur)
{
	if (treeElement != nullptr)
		treeElement->DrawElement(deviceContext, (x + xStartElement) - xposLargeur, y - posHauteur);
}

void CPositionElement::DrawElement(wxDC * deviceContext)
{
	if (treeElement != nullptr)
		treeElement->DrawElement(deviceContext, x, y);
}

void CPositionElement::SetValue(const int &x, const int &y, const int &numColumn, const int &numRow, const int &width, const int &height, const int &type, CTreeElement * treeElement, CTreeData * treeData)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->treeData = treeData;
	this->numColumn = numColumn;
	this->numRow = numRow;
	this->type = type;
	this->treeElement = treeElement;
}