#include <header.h>
#include "OcrLabel.h"
#include "TreeElementTexte.h"
#include "TreeElementTriangle.h"
#include "MainTheme.h"
#include "MainThemeInit.h"
#include <TreeData.h>
#include "TreeElementCheckBox.h"
#include "TreeElementControlInterface.h"
#include <window_id.h>
using namespace Regards::Window;
using namespace Regards::Scanner;


COcrLabel::COcrLabel(CTreeElementControlInterface* interfaceControl, int idWindow)
{
	this->idWindow = idWindow;
	widthPosition = 0;
	CMainTheme* viewerTheme = CMainThemeInit::getInstance();

	if (viewerTheme != nullptr)
		viewerTheme->GetTreeTheme(&themeTree);

	themeTree.themeTriangle.SetHeight(themeTree.GetRowHeight());
	themeTree.themeCheckbox.SetHeight(themeTree.GetRowHeight());
	themeTree.themeDelete.SetHeight(themeTree.GetRowHeight());
	themeTree.themeSlide.SetHeight(themeTree.GetRowHeight());
	themeTree.themeTexte.SetHeight(themeTree.GetRowHeight());
	eventControl = interfaceControl;
	themeTree.SetMargeX(5);
	rowWidth.push_back(0);
};


COcrLabel::~COcrLabel()
{
}


void COcrLabel::ClickOnElement(CPositionElement* element, wxWindow* window, const int& x, const int& y,
                               const int& posLargeur, const int& posHauteur)
{
	CTreeElement* treeElement = element->GetTreeElement();
	auto treeData = element->GetTreeData();
	if (element->GetType() == ELEMENT_TEXTE)
	{
		wxWindow* central = wxWindow::FindWindowById(idWindow);
		{
			auto event = new wxCommandEvent(wxEVENT_CHECKTREE_READ);
			wxString id = treeData->GetKey();
			event->SetInt(atoi(id));
			wxQueueEvent(central, event);
		}
		eventControl->UpdateTreeControl();
	}
	if (element->GetType() == ELEMENT_CHECKBOX)
	{
		auto checkBox = static_cast<CTreeElementCheckBox*>(element->GetTreeElement());
		bool check = !checkBox->GetCheckState();
		checkBox->SetCheckState(check);
		SaveState();

		wxWindow* central = wxWindow::FindWindowById(idWindow);
		{
			auto event = new wxCommandEvent(wxEVENT_CHECKTREE_CHOICE);
			wxString id = treeData->GetKey();
			event->SetInt(atoi(id));
			event->SetExtraLong(check);
			wxQueueEvent(central, event);
		}
		eventControl->UpdateTreeControl();
	}
	else if (element->GetType() == ELEMENT_TRIANGLE)
	{
		auto treeElementTriangle = static_cast<CTreeElementTriangle*>(treeElement);
		treeElementTriangle->ClickElement(window, (x + posLargeur) - element->GetX(),
		                                  (y + posHauteur) - element->GetY());
		RenderElement(RenderMode::Update);
		eventControl->UpdateTreeControl();
	}
}

void COcrLabel::MouseOver(wxDC* deviceContext, CPositionElement* element, const int& x, const int& y,
                          const int& posLargeur, const int& posHauteur, bool& update)
{
	int xPos = 0;
	if (element->GetRow() > 0)
		xPos = GetWidthRow(element->GetRow() - 1);

	element->GetTreeElement()->MouseOver(deviceContext, element->GetX() + xPos, element->GetY(), update);
}

void COcrLabel::Init()
{
	item = 0;
	top = tr.begin();

	auto treeDataPicture = new CTreeData();
	treeDataPicture->SetKey("Ocr");
	tr.insert(top, treeDataPicture);

	RenderElement(RenderMode::Create);
	RenderElement(RenderMode::Update);

	SaveState();
}

void COcrLabel::Init(vector<ChOcrElement*>& labelList)
{
	item = 0;

	top = tr.begin();

	auto treeDataPicture = new CTreeData();
	treeDataPicture->SetKey("Ocr");
	const auto child = tr.insert(top, treeDataPicture);

	for (ChOcrElement* text : labelList)
	{
		if (text->itemClass == "ocr_line")
		{
			auto bbox = static_cast<ChOcrElementLine*>(text);
			auto data = new CTreeData();
			data->SetIsParent(false);
			data->SetKey(to_string(bbox->id));
			data->SetValue(bbox->label);
			tr.append_child(child, data);
		}
	}

	RenderElement(RenderMode::Create);
	RenderElement(RenderMode::Update);

	SaveState();
}

void COcrLabel::SetActifElement(const wxString& key)
{
	for (auto fit = vectorPosElement.begin(); fit != vectorPosElement.end(); ++fit)
	{
		CPositionElement* posElement = *fit;
		if (posElement != nullptr)
		{
			CTreeData* data = posElement->GetTreeData();
			if (data->GetExifKey() == key)
				data->SetActif(true);
			else
				data->SetActif(false);
		}
	}
	eventControl->UpdateTreeControl();
}

void COcrLabel::RenderElement(RenderMode mode)
{
	CTreeElementTriangle* tree_element_triangle = nullptr;
	CPositionElement* posElement = nullptr;

	HideAll();

	bool isVisible = true;
	tree<CTreeData*>::sibling_iterator it = tr.begin();
	auto itend = tr.end();
	yPos = 0;
	nbRow = 0;

	while (it != itend)
	{
		CTreeData* data = *it;
		int profondeur = tr.depth(it);
		if (profondeur == 0)
		{
			int xPos = themeTree.GetMargeX();
			int widthElement = 0;
			CTreeElementTriangle* tree_element_triangle;

			posElement = RenderTriangle(data,
				xPos,
				yPos,
				isVisible,
				mode);

			tree_element_triangle = static_cast<CTreeElementTriangle*>(posElement->GetTreeElement());

			widthPosition = xPos + posElement->GetWidth() + themeTree.GetMargeX();
			xPos += posElement->GetWidth() + themeTree.GetMargeX();

			posElement = RenderText(data,
				xPos,
				yPos,
				isVisible,
				mode);

			widthElement += xPos + posElement->GetWidth() + themeTree.GetMargeX();
			yPos += themeTree.GetRowHeight();
			nbRow++;
			if (rowWidth[0] < widthElement)
				rowWidth[0] = widthElement;

			if (mode == RenderMode::Create)
			{
				RenderChildTree(it, mode);
			}
			else
			{
				const bool isOpen = tree_element_triangle->GetOpen();
				if (isOpen)
					RenderChildTree(it, mode);
			}
		}
		++it;
	}
}

void COcrLabel::UpdateScreenRatio()
{
	RenderElement(RenderMode::Update);
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void COcrLabel::SaveState()
{
	int nbElement = 0;
	stateValue = L"";

	for (auto value : vectorPosElementDynamic)
	{
		if (value != nullptr)
		{
			if (value->GetType() == ELEMENT_CHECKBOX)
			{
				//bool checkState = false;
				CTreeData* treeData = value->GetTreeData();
				auto checkBox = static_cast<CTreeElementCheckBox*>(value->GetTreeElement());
				if (checkBox != nullptr && treeData != nullptr)
				{
					bool checkState = checkBox->GetCheckState();
					wxString exifKey = treeData->GetExifKey();
					wxString key = treeData->GetKey();
					stateValue.append(exifKey);
					stateValue.append(L":");
					stateValue.append(key);
					if (checkState)
						stateValue.append(L":1");
					else
						stateValue.append(L":0");

					stateValue.append(L";");
					nbElement++;
				}
			}
		}
	}
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
bool COcrLabel::GetCheckState(const wxString& exifKey, const wxString& key)
{
	wxString localkey = exifKey + L":" + key + L":";
	size_t pos = stateValue.find(localkey);
	if (pos < stateValue.size())
	{
		pos += localkey.size();
		wchar_t check = stateValue[pos];
		if (check == '1')
			return true;
		return false;
	}
	return true;
}

void COcrLabel::RenderChildTree(tree<CTreeData*>::sibling_iterator& parent, RenderMode mode)
{
	CTreeElementTriangle* tree_element_triangle = nullptr;
	CPositionElement* posElement = nullptr;
	tree<CTreeData*>::sibling_iterator it = tr.begin(parent);
	//tree<CTreeData *>::iterator itend = tr.end(parent);
	bool isVisible = true;
	//int i = 

	for (auto i = 0; i < parent.number_of_children(); i++)
	{
		const int profondeur = tr.depth(it);
		CTreeData* data = *it;

		int xPos = widthPosition * (profondeur);

		CPositionElement* posElement = GetElement(data, ELEMENT_TEXTE);
		if (posElement == nullptr)
		{

			posElement = RenderCheckBox(data,
				xPos,
				yPos,
				GetCheckState(data->GetExifKey(), data->GetKey()),
				isVisible,
				mode);

			xPos += posElement->GetWidth() + themeTree.GetMargeX();

			posElement = RenderText(data,
				xPos,
				yPos,
				isVisible,
				mode);
		}
		else
		{

			posElement = RenderCheckBox(data,
				xPos,
				yPos,
				GetCheckState(data->GetExifKey(), data->GetKey()),
				isVisible,
				mode);

			xPos += posElement->GetWidth() + themeTree.GetMargeX();

			posElement = RenderText(data,
				xPos,
				yPos,
				isVisible,
				mode);
		}

		const int widthElement = xPos + posElement->GetWidth() + themeTree.GetMargeX();
		yPos += themeTree.GetRowHeight();

		nbRow++;
		if (rowWidth[0] < widthElement)
			rowWidth[0] = widthElement;

		++it;
	}
}
