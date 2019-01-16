
#include "TreeControl.h"
#include <wx/dcbuffer.h>
using namespace Regards::Window;

CTreeControl::CTreeControl(CThemeTree * theme, CTreeElementControlInterface * interfaceControl)
{
	themeTree = *theme;
	themeTree.themeTriangle.SetHeight(themeTree.GetRowHeight());
	themeTree.themeCheckbox.SetHeight(themeTree.GetRowHeight());
	themeTree.themeDelete.SetHeight(themeTree.GetRowHeight());
	themeTree.themeSlide.SetHeight(themeTree.GetRowHeight());
	themeTree.themeTexte.SetHeight(themeTree.GetRowHeight());
	eventControl = interfaceControl;
}

CTreeControl& CTreeControl::operator=(const CTreeControl &other)
{
	return *this;
}

wxColour CTreeControl::GetBackgroundColour(const int &yPos)
{
    int moduloValue = yPos % (themeTree.GetRowHeight() * 2);
    if(moduloValue < themeTree.GetRowHeight())
        return themeTree.bgColorOne;
    
    return themeTree.bgColorTwo;
}

CTreeControl::~CTreeControl()
{
	ClearData();
	rowWidth.clear();
}

int CTreeControl::GetNbRow()
{
	return nbRow;
}

int CTreeControl::GetWidthRow(const int &numRow)
{
	if (numRow < rowWidth.size())
		return rowWidth[numRow];
	

	return 0;
}

int CTreeControl::GetWidth()
{
	int controlWidth = 0;
	if(rowWidth.size() == 0)
		return controlWidth;

	for (auto size : rowWidth)
	{
		controlWidth += size;
	}
	return controlWidth;
}


void CTreeControl::GenerateWindowBitmap(wxDC * deviceContext, const int &width, const int &height, const int &posLargeur, const int &posHauteur)
{
	std::sort(vectorPosElement.begin(), vectorPosElement.end(),
		[](CPositionElement * i, CPositionElement * j)
	{
		return (i->GetY() < j->GetY());
	});

	int diffHauteur = posHauteur % 20;
	int startHauteur = (diffHauteur != 0) ? posHauteur - (20 - diffHauteur) : posHauteur;


	for (CPositionElement * value : vectorPosElement)
	{
		if (value->GetY() > (posHauteur + height))
			break;

		if (value->GetY() < startHauteur)
			continue;

		if (value != nullptr)
		{
			//int x = 0;
			int y = 0;
			//CTreeData * data = value->GetTreeData();
			CTreeElement * treeElement = value->GetTreeElement();

			if (treeElement->IsVisible())
			{
				//if (value->GetRow() > 0)
				//	x = value->GetX() + treeElement->GetWidth() + rowWidth.at(value->GetRow() - 1);
				//else
				//	x = value->GetX() + treeElement->GetWidth();

				y = value->GetY();

				int xStartElement = 0;
				if (value->GetRow() > 0)
				{
					xStartElement = rowWidth.at(value->GetRow() - 1);
				}

				treeElement->SetBackgroundColor(GetBackgroundColour(y));
				value->DrawElement(deviceContext, xStartElement, posLargeur, posHauteur);
			}
		}
	}
}

CTreeElementCheckBox * CTreeControl::CreateCheckBoxElement(const int &width, const int &height, const bool &check)
{
	CTreeElementCheckBox * treeElementCheckBox = new CTreeElementCheckBox();
	treeElementCheckBox->SetZoneSize(width, height);
	treeElementCheckBox->SetCheckState(check);
	treeElementCheckBox->SetTheme(&themeTree.themeCheckbox);
	return treeElementCheckBox;
}

CTreeElementSlide * CTreeControl::CreateSlideElement(const int &width, const int &height, CTreeElementValue * position, vector<CTreeElementValue *> * value, const wxString &exifKey)
{
	CTreeElementSlide * treeElementSlide = new CTreeElementSlide(this);
	treeElementSlide->SetTheme(&themeTree.themeSlide);
	treeElementSlide->SetZoneSize(themeTree.themeSlide.GetElementWidth(), height);
	treeElementSlide->SetExifKey(exifKey);
	treeElementSlide->SetTabValue(value);
	treeElementSlide->SetInitValue(position);
	return treeElementSlide;
}

CTreeElementListBox * CTreeControl::CreateListBoxElement(const int &width, const int &height, const vector<CMetadata> & value, const int &index, const wxString &exifKey)
{
	CTreeElementListBox * treeElementListBox = new CTreeElementListBox(this);
	treeElementListBox->SetTheme(&themeTree.themeListbox);
	treeElementListBox->SetZoneSize(themeTree.themeSlide.GetElementWidth(), height);
	treeElementListBox->SetExifKey(exifKey);
	treeElementListBox->SetTabValue(value, index);
	return treeElementListBox;
}

CTreeElementTriangle * CTreeControl::CreateTriangleElement(const int &width, const int &height, const bool &open)
{
	CTreeElementTriangle * treeElementTriangle = new CTreeElementTriangle();
	treeElementTriangle->SetTheme(&themeTree.themeTriangle);
	treeElementTriangle->SetZoneSize(width, height);
	treeElementTriangle->SetOpen(open);

	return treeElementTriangle;
}

CTreeElementDelete * CTreeControl::CreateDeleteElement(const int &width, const int &height)
{
	CTreeElementDelete * treeElementDelete = new CTreeElementDelete();
	treeElementDelete->SetTheme(&themeTree.themeDelete);
	treeElementDelete->SetZoneSize(width, height);
	return treeElementDelete;
}

CTreeElementTexte * CTreeControl::CreateTexteElement(const int &width, const int &height, const wxString &libelle)
{
	CTreeElementTexte * treeElementTexte = new CTreeElementTexte();
	treeElementTexte->SetTheme(&themeTree.themeTexte);
	treeElementTexte->SetZoneSize(width, height);
	treeElementTexte->SetLibelle(libelle);
	return treeElementTexte;
}

CTreeElementTexteClick * CTreeControl::CreateTexteLinkElement(const int &width, const int &height, const wxString &libelle, const wxString &link, const int &linkType)
{
    CTreeElementTexteClick * treeElementTexte = new CTreeElementTexteClick();
    treeElementTexte->SetTheme(&themeTree.themeTexte);
    treeElementTexte->SetZoneSize(width, height);
    treeElementTexte->SetLibelle(libelle);
    treeElementTexte->SetTypeLink(linkType);
    treeElementTexte->SetLinkElement(link);
    return treeElementTexte;
}



void CTreeControl::ClearData()
{

	for (CPositionElement * posElement : vectorPosElement)
	{
		if (posElement != nullptr)
		{
			delete posElement;
			posElement = nullptr;
		}
		
	}

	vectorPosElement.clear();
	vectorPosElementDynamic.clear();

	tree<CTreeData *>::sibling_iterator it = tr.begin();
	tree<CTreeData *>::iterator itend = tr.end();

	while (it != itend) {
		CTreeData * data = *it;
		if (it.number_of_children() > 0)
			EraseChildTree(it);
		if (data != nullptr)
		{
			delete data;
			data = nullptr;
		}
		it++;
	}

	tr.clear();
	tr.empty();
}

void CTreeControl::EraseChildTree(tree<CTreeData *>::sibling_iterator &parent)
{
	tree<CTreeData *>::sibling_iterator it = tr.begin(parent);
	//tree<CTreeData *>::iterator itend = tr.end(parent);

	//int i = 

	for (auto i = 0; i < parent.number_of_children(); i++)
	{
		CTreeData * data = *it;

		if (it.number_of_children() > 0)
		{
			EraseChildTree(it);
		}

		if (data != nullptr)
		{
			delete data;
			data = nullptr;
		}

		it++;
	}
}

CPositionElement * CTreeControl::GetElement(CTreeData * data, const int &typeElement)
{
	for (CPositionElement * value : vectorPosElement)
	{
		if (value != nullptr)
		{
			CTreeData * dataElement = value->GetTreeData();
			if (dataElement == data && value->GetType() == typeElement)
			{
				return value;
			}
		}
	}

	return nullptr;
}


CPositionElement * CTreeControl::FindElement(const int &x, const int &y)
{
	if(vectorPosElementDynamic.size() == 0)
		return nullptr;

	for (CPositionElement * value : vectorPosElementDynamic)
	{
		if (value != nullptr)
		{
			try
			{
				CTreeElement * treeElement = value->GetTreeElement();
				if (treeElement != nullptr)
				{
					if (treeElement->IsVisible())
					{
						int xPos = 0;
						if (value->GetRow() > 0)
							xPos = GetWidthRow(value->GetRow() - 1);

						int x1 = value->GetX() + xPos;
						int x2 = value->GetX() + xPos + treeElement->GetWidth();
						int y1 = value->GetY();
						int y2 = value->GetY() + treeElement->GetHeight();
						if ((x1 <= x && x <= x2) && (y1 <= y && y <= y2))
						{
							return value;
						}
					}
				}
			}
			catch (...)
			{

			}
		}
	}

	return nullptr;
}

CPositionElement * CTreeControl::CreatePositionElement(const int &x, const int &y, const int &numColumn, const int &numRow, const int &width, const int &height, const int &type, CTreeElement * treeElement, CTreeData * treeData, const bool &dynamic)
{
	CPositionElement * positionElement = new CPositionElement();
	treeElement->SetColumn(numColumn);
	treeElement->SetRow(numRow);
	treeElement->SetElementPos(x, y);
	positionElement->SetValue(x, y, numColumn, numRow, width, height, type, treeElement, treeData);
	vectorPosElement.push_back(positionElement);
	if (dynamic)
	{
		vectorPosElementDynamic.push_back(positionElement);
	}
	return positionElement;
}

tree<CTreeData *>::iterator CTreeControl::FindKey(const wxString & key, tree<CTreeData *>::iterator &parent)
{
	tree<CTreeData *>::sibling_iterator it = tr.begin(parent);
	//tree<CTreeData *>::iterator itend = tr.end(parent);

	//int i = 

	for (auto i = 0; i < parent.number_of_children(); i++)
	{
		CTreeData * data = *it;
		if (data->GetKey() == key)
		{
			return it;
		}
		it++;
	}

	return nullptr;
}

tree<CTreeData *>::iterator CTreeControl::FindKey(const wxString & key)
{
	tree<CTreeData *>::iterator it = tr.begin();
	tree<CTreeData *>::iterator itend = tr.end();
	while (it != itend) {
		CTreeData * data = *it;
		if (data->GetKey() == key)
			return it;
		it++;
	}

	return nullptr;
}
