// ReSharper disable All
#include "header.h"
#include "TreeControl.h"
#include "TreeElementTriangle.h"
#include "TreeElementSlide.h"
#include "TreeElementDelete.h"
#include "TreeElementTexte.h"
#include "TreeElementTexteClick.h"
#include "TreeElementCheckBox.h"
#include "TreeElementListBox.h"
#include "TreeElementStar.h"
#include "TreeDataStars.h"
#include "TreeDataLink.h"
#include "TreeData.h"
using namespace Regards::Window;


struct treeElementPos
{
	treeElementPos(int x, int y, CTreeControl* parent) : treeControl(parent), _x(x), _y(y)
	{
	}

	bool operator()(CPositionElement* value)
	{
		if (value != nullptr && treeControl != nullptr)
		{
			CTreeElement* treeElement = value->GetTreeElement();
			if (treeElement != nullptr)
			{
				if (treeElement->IsVisible())
				{
					int xPos = 0;
					if (value->GetRow() > 0)
						xPos = treeControl->GetWidthRow(value->GetRow() - 1);

					int x1 = value->GetX() + xPos;
					int x2 = value->GetX() + xPos + treeElement->GetWidth();
					int y1 = value->GetY();
					int y2 = value->GetY() + treeElement->GetHeight();
					if ((x1 <= _x && _x <= x2) && (y1 <= _y && _y <= y2))
					{
						return true;
					}
				}
			}
		}
		return false;
	}

	CTreeControl* treeControl;
	int _x;
	int _y;
};

CTreeControl::CTreeControl(CThemeTree* theme, CTreeElementControlInterface* interfaceControl): nbRow(0)
{
	themeTree = *theme;
	themeTree.themeTriangle.SetHeight(themeTree.GetRowHeight());
	themeTree.themeCheckbox.SetHeight(themeTree.GetRowHeight());
	themeTree.themeDelete.SetHeight(themeTree.GetRowHeight());
	themeTree.themeSlide.SetHeight(themeTree.GetRowHeight());
	themeTree.themeTexte.SetHeight(themeTree.GetRowHeight());
	eventControl = interfaceControl;
}

CTreeControl& CTreeControl::operator=(const CTreeControl& other)
{
	return *this;
}

wxColour CTreeControl::GetBackgroundColour(const int& yPos)
{
	int moduloValue = yPos % (themeTree.GetRowHeight() * 2);
	if (moduloValue < themeTree.GetRowHeight())
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

int CTreeControl::GetWidthRow(const int& numRow)
{
	if (numRow < rowWidth.size())
		return rowWidth[numRow];


	return 0;
}

int CTreeControl::GetWidth()
{
	int controlWidth = 0;
	if (rowWidth.size() == 0)
		return controlWidth;

	for (auto size : rowWidth)
	{
		controlWidth += size;
	}
	return controlWidth;
}


void CTreeControl::GenerateWindowBitmap(wxDC* deviceContext, const int& width, const int& height, const int& posLargeur,
                                        const int& posHauteur)
{
	std::sort(vectorPosElement.begin(), vectorPosElement.end(),
	          [](CPositionElement* i, CPositionElement* j)
	          {
		          return (i->GetY() < j->GetY());
	          });

	int diffHauteur = posHauteur % 20;
	int startHauteur = (diffHauteur != 0) ? posHauteur - (20 - diffHauteur) : posHauteur;

	for (CPositionElement* value : vectorPosElement)
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
			CTreeElement* treeElement = value->GetTreeElement();

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
void CTreeControl::HideAll()
{
	for (auto value: vectorPosElement)
	{
		if (value != nullptr)
		{
			CTreeElement* treeElement = value->GetTreeElement();
			treeElement->SetVisible(false);
		}
	}
}

CTreeElementCheckBox* CTreeControl::CreateCheckBoxElement(const int& width, const int& height, const bool& check)
{
	auto treeElementCheckBox = new CTreeElementCheckBox();
	treeElementCheckBox->SetZoneSize(width, height);
	treeElementCheckBox->SetCheckState(check);
	treeElementCheckBox->SetTheme(&themeTree.themeCheckbox);
	return treeElementCheckBox;
}

CTreeElementSlide* CTreeControl::CreateSlideElement(const int& width, const int& height, CTreeElementValue* position,
                                                    vector<CTreeElementValue*>* value, const wxString& exifKey)
{
	auto treeElementSlide = new CTreeElementSlide(this);
	treeElementSlide->SetTheme(&themeTree.themeSlide);
	treeElementSlide->SetZoneSize(themeTree.themeSlide.GetElementWidth(), height);
	treeElementSlide->SetExifKey(exifKey);
	treeElementSlide->SetTabValue(value);
	treeElementSlide->SetInitValue(position);
	return treeElementSlide;
}

CTreeElementListBox* CTreeControl::CreateListBoxElement(const int& width, const int& height,
                                                        const vector<CMetadata>& value, const int& index,
                                                        const wxString& exifKey)
{
	auto treeElementListBox = new CTreeElementListBox(this);
	treeElementListBox->SetTheme(&themeTree.themeListbox);
	treeElementListBox->SetZoneSize(themeTree.themeSlide.GetElementWidth(), height);
	treeElementListBox->SetExifKey(exifKey);
	treeElementListBox->SetTabValue(value, index);
	return treeElementListBox;
}

CTreeElementTriangle* CTreeControl::CreateTriangleElement(const int& width, const int& height, const bool& open)
{
	auto treeElementTriangle = new CTreeElementTriangle();
	treeElementTriangle->SetTheme(&themeTree.themeTriangle);
	treeElementTriangle->SetZoneSize(width, height);
	treeElementTriangle->SetOpen(open);

	return treeElementTriangle;
}



CTreeElementDelete* CTreeControl::CreateDeleteElement(const int& width, const int& height)
{
	auto treeElementDelete = new CTreeElementDelete();
	treeElementDelete->SetTheme(&themeTree.themeDelete);
	treeElementDelete->SetZoneSize(width, height);
	return treeElementDelete;
}


CPositionElement* CTreeControl::RenderStar(
	CTreeData* data,
	int& xPos,
	int& yPos,
	bool visible,
	RenderMode mode)
{
	CPositionElement* posElement = nullptr;

	if (mode == RenderMode::Update)
		posElement = GetElement(data, ELEMENT_STAR);

	if (posElement == nullptr)
	{
		auto treedataStar = static_cast<CTreeDataStars*>(data);
		CTreeElementStar* treeElementStar = CreateStarElement(
			themeTree.GetRowWidth(), themeTree.GetRowHeight(),
			data->GetKey(), data->GetValue(),
			treedataStar->GetNumPhotoId());

		treeElementStar->SetVisible(visible);
		posElement = CreatePositionElement(
			xPos, yPos, nbRow, 1, treeElementStar->GetWidth(),
			themeTree.GetRowHeight(), ELEMENT_STAR, treeElementStar,
			data);
	}
	else
	{
		auto* text =
			static_cast<CTreeElementTexte*>(posElement->GetTreeElement());

		text->SetVisible(visible);
		text->SetElementPos(xPos, yPos);

		posElement->SetX(xPos);
		posElement->SetY(yPos);
	}

	return posElement;

}

CPositionElement* CTreeControl::RenderText(
	CTreeData* data,
	int& xPos,
	int& yPos,
	bool visible,
	RenderMode mode,
	bool addDynamic)
{
	CPositionElement* posElement = nullptr;

	if (mode == RenderMode::Update)
		posElement = GetElement(data, ELEMENT_TEXTE);

	if (posElement == nullptr)
	{
		auto* text = CreateTexteElement(
			themeTree.GetRowWidth(),
			themeTree.GetRowHeight(),
			data->GetKey());

		text->SetVisible(visible);

		posElement = CreatePositionElement(
			xPos,
			yPos,
			nbRow,
			0,
			text->GetWidth(),
			text->GetHeight(),
			ELEMENT_TEXTE,
			text,
			data,
			addDynamic);
	}
	else
	{
		auto* text =
			static_cast<CTreeElementTexte*>(posElement->GetTreeElement());

		text->SetVisible(visible);
		text->SetElementPos(xPos, yPos);

		posElement->SetX(xPos);
		posElement->SetY(yPos);
	}

	return posElement;
}


CPositionElement* CTreeControl::RenderTextValue(
	CTreeData* data,
	int& xPos,
	int& yPos,
	bool visible,
	RenderMode mode)
{
	CPositionElement* posElement = nullptr;

	if (mode == RenderMode::Update)
		posElement = GetElement(data, ELEMENT_TEXTEVALUE);

	if (posElement != nullptr)
	{
		auto element_texte = static_cast<CTreeElementTexte*>(posElement->GetTreeElement());
		element_texte->SetVisible(visible);
		posElement->SetX(xPos);
		posElement->SetY(yPos);
	}
	else
	{
		auto tree_element_texte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(),
			data->GetValue());
		tree_element_texte->SetVisible(visible);
		posElement = CreatePositionElement(xPos, yPos, nbRow, 1, tree_element_texte->GetWidth(),
			tree_element_texte->GetHeight(), ELEMENT_TEXTEVALUE,
			tree_element_texte, data, false);
	}
	return posElement;
}


CPositionElement* CTreeControl::RenderTextLink(
	CTreeData* data,
	int& xPos,
	int& yPos,
	bool visible,
	RenderMode mode)
{
	CPositionElement* posElement = nullptr;

	if (mode == RenderMode::Update)
		posElement = GetElement(data, ELEMENT_TEXTE);

	if (posElement != nullptr)
	{
		auto element_texte = static_cast<CTreeElementTexte*>(posElement->GetTreeElement());
		element_texte->SetVisible(visible);
		posElement->SetX(xPos);
		posElement->SetY(yPos);
	}
	else
	{
		auto dataLink = static_cast<CTreeDataLink*>(data);
		auto tree_element_texte = CreateTexteLinkElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(),
				data->GetKey(), dataLink->GetLinkPath(),
				dataLink->GetLinkType());
		tree_element_texte->SetVisible(visible);
		posElement = CreatePositionElement(xPos, yPos, nbRow, 0, tree_element_texte->GetWidth(),
			tree_element_texte->GetHeight(), ELEMENT_TEXTE, tree_element_texte, data,
			false);
	}
	return posElement;
}


void CTreeControl::AddTreeInfos(const wxString& exifKey, const wxString& exifValue, const int& index,
	tree<CTreeData*>::iterator& top, tree<CTreeData*>::iterator& child)
{
	wchar_t seps[] = L".";
	int item = 0;
	vector<wchar_t> informations;
	informations.resize(exifKey.size() + 1);
	wcscpy(&informations[0], exifKey.c_str());
	wchar_t* token1;
	wchar_t* token = wcstok(&informations[0], seps, &token1); // C4996

	// Note: strtok is deprecated; consider using strtok_s instead
	while (token != nullptr)
	{
		auto treeData = new CTreeData();
		treeData->SetKey(token);
		wchar_t* token2 = wcstok(&informations[0], seps, &token1); // C4996


		if (token2 != nullptr)
		{
			treeData->SetIsParent(true);

			if (index > 0)
			{
				tree<CTreeData*>::iterator it;
				//Recherche de la clé
				if (item == 0)
				{

					it = FindKey(treeData->GetKey());
				}
				else
				{
					it = FindKey(treeData->GetKey(), child);
				}

				if (it != nullptr)
				{
					child = it;
					item++;
					delete(treeData);
					continue;
				}
			}

			if (item > 0)
			{
				child = tr.append_child(child, treeData);
			}
			else
			{
				child = tr.insert(top, treeData);
			}
		}
		else
		{
			treeData->SetIsParent(false);
			treeData->SetValue(exifValue);
			treeData->SetExifKey(exifKey);
			tr.append_child(child, treeData);
		}
		item++;
	}
}

CPositionElement* CTreeControl::RenderCheckBox(
	CTreeData* data,
	int& xPos,
	int& yPos,
	bool check,
	bool visible,
	RenderMode mode)
{
	CPositionElement* posElement = nullptr;

	if (mode == RenderMode::Update)
		posElement = GetElement(data, ELEMENT_CHECKBOX);

	if (posElement == nullptr)
	{
		//bool check = GetCheckState(data);
		CTreeElementCheckBox* tree_element_check = CreateCheckBoxElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), check);
		posElement = CreatePositionElement(xPos, yPos, nbRow, 0, tree_element_check->GetWidth(),
			tree_element_check->GetHeight(), ELEMENT_CHECKBOX, tree_element_check, data);


	}
	else
	{
		CTreeElementCheckBox* tree_element_check = static_cast<CTreeElementCheckBox*>(posElement->GetTreeElement());
		tree_element_check->SetVisible(visible);
		tree_element_check->SetElementPos(xPos, yPos);
		posElement->SetX(xPos);
		posElement->SetY(yPos);
	}

	return posElement;
}

CPositionElement* CTreeControl::RenderTriangle(
	CTreeData* data,
	int& xPos,
	int& yPos,
	bool visible,
	RenderMode mode)
{
	CPositionElement* posElement = nullptr;

	if (mode == RenderMode::Update)
		posElement = GetElement(data, ELEMENT_TRIANGLE);

	if (posElement == nullptr)
	{
		bool isOpen = GetTriangleState(data);
		CTreeElementTriangle* treeElementTriangle = CreateTriangleElement(
			themeTree.GetRowWidth(), themeTree.GetRowHeight(),
			isOpen);
		posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTriangle->GetWidth(),
			treeElementTriangle->GetHeight(), ELEMENT_TRIANGLE, treeElementTriangle,
			data);

	}
	else
	{
		CTreeElementTriangle* tree_element_triangle = static_cast<CTreeElementTriangle*>(posElement->GetTreeElement());
		tree_element_triangle->SetVisible(visible);
		tree_element_triangle->SetElementPos(xPos, yPos);
		posElement->SetX(xPos);
		posElement->SetY(yPos);
	}

	return posElement;
}

CTreeElementTexte* CTreeControl::CreateTexteElement(const int& width, const int& height, const wxString& libelle)
{
	auto treeElementTexte = new CTreeElementTexte();
	treeElementTexte->SetTheme(&themeTree.themeTexte);
	treeElementTexte->SetZoneSize(width, height);
	treeElementTexte->SetLibelle(libelle);
	return treeElementTexte;
}

CTreeElementStar* CTreeControl::CreateStarElement(const int& width, const int& height, const wxString& libelle,
                                                  const wxString& value, const int& numPhotoId)
{
	auto treeElementStar = new CTreeElementStar();
	treeElementStar->SetNumPhoto(numPhotoId);
	treeElementStar->SetTheme(&themeTree.themeTriangle);
	treeElementStar->SetZoneSize(width * 5, height);
	treeElementStar->SetValue(atoi(value));
	return treeElementStar;
}

CTreeElementTexteClick* CTreeControl::CreateTexteLinkElement(const int& width, const int& height,
                                                             const wxString& libelle, const wxString& link,
                                                             const int& linkType)
{
	auto treeElementTexte = new CTreeElementTexteClick();
	treeElementTexte->SetTheme(&themeTree.themeTexte);
	treeElementTexte->SetZoneSize(width, height);
	treeElementTexte->SetLibelle(libelle);
	treeElementTexte->SetTypeLink(linkType);
	treeElementTexte->SetLinkElement(link);
	return treeElementTexte;
}


void CTreeControl::ClearData()
{
	for (CPositionElement* posElement : vectorPosElement)
	{
		if (posElement != nullptr)
		{
			delete posElement;
			posElement = nullptr;
		}
	}

	vectorPosElement.clear();
	vectorPosElementDynamic.clear();

	tree<CTreeData*>::sibling_iterator it = tr.begin();
	auto itend = tr.end();

	while (it != itend)
	{
		CTreeData* data = *it;
		if (it.number_of_children() > 0)
			EraseChildTree(it);
		if (data != nullptr)
		{
			delete data;
			data = nullptr;
		}
		++it;
	}

	tr.clear();
	tr.empty();
}

void CTreeControl::EraseChildTree(tree<CTreeData*>::sibling_iterator& parent)
{
	tree<CTreeData*>::sibling_iterator it = tr.begin(parent);

	for (auto i = 0; i < parent.number_of_children(); i++)
	{
		CTreeData* data = *it;

		if (it.number_of_children() > 0)
		{
			EraseChildTree(it);
		}

		if (data != nullptr)
		{
			delete data;
			data = nullptr;
		}

		++it;
	}
}

CPositionElement* CTreeControl::GetElement(CTreeData* data, const int& typeElement)
{
	auto p = std::find_if(
		vectorPosElementDynamic.begin(), vectorPosElementDynamic.end(),
		[&](const auto& val)
		{
			auto value = static_cast<CPositionElement*>(val);
			CTreeData* dataElement = value->GetTreeData();
			return (dataElement == data && value->GetType() == typeElement);
		}
	);

	if (p == vectorPosElementDynamic.end())
		return nullptr;

	return *p;

}


CPositionElement* CTreeControl::FindElement(const int& x, const int& y)
{
	if (vectorPosElementDynamic.size() == 0)
		return nullptr;

	auto p = std::find_if(
		vectorPosElementDynamic.begin(), vectorPosElementDynamic.end(),
		[&](const auto& data)
		{
			auto value = static_cast<CPositionElement*>(data);
			CTreeElement* treeElement = value->GetTreeElement();
			if (treeElement != nullptr)
			{
				if (treeElement->IsVisible())
				{
					int xPos = 0;
					if (value->GetRow() > 0)
						xPos = GetWidthRow(value->GetRow() - 1);

					int x1 = value->GetX() + xPos;
					int x2 = value->GetX() + xPos + value->GetWidth();
					int y1 = value->GetY();
					int y2 = value->GetY() + value->GetHeight();
					if ((x1 <= x && x <= x2) && (y1 <= y && y <= y2))
					{
						return true;
					}
				}
			}
			return false;
		}
	);

	if (p == vectorPosElementDynamic.end())
		return nullptr;

	return *p;
}

CPositionElement* CTreeControl::CreatePositionElement(const int& x, const int& y, const int& numColumn,
                                                      const int& numRow, const int& width, const int& height,
                                                      const int& type, CTreeElement* treeElement, CTreeData* treeData,
                                                      const bool& dynamic)
{
	auto positionElement = new CPositionElement();
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

tree<CTreeData*>::iterator CTreeControl::FindKey(const wxString& key, tree<CTreeData*>::iterator& parent)
{
	auto p = std::find_if(
		tr.begin(parent), tr.end(parent),
		[&](const auto& data)
		{
			if (data != nullptr)
				return data->GetKey() == key;
		}
	);

	if (p == tr.end(parent))
		return nullptr;

	return p;
}

tree<CTreeData*>::iterator CTreeControl::FindKey(const wxString& key)
{
	auto p = std::find_if(
		tr.begin(), tr.end(),
		[&](const auto& data)
		{
			//CPhotos photo = static_cast<CPhotos>(val);
			return data->GetKey() == key;
		}
	);

	if (p == tr.end())
		return nullptr;

	return p;
}
