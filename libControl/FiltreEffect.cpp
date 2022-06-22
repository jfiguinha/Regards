#include <header.h>
#include "FiltreEffect.h"
#include <TreeDataEffect.h>
#include <FilterData.h>
#include <RegardsBitmap.h>
#include "MainTheme.h"
#include "MainThemeInit.h"
#include "BitmapWndViewer.h"
#include "BitmapWnd3d.h"
#include <TreeElementTexte.h>
#include <TreeElementSlide.h>
#include <TreeElementTriangle.h>
#include <TreeElementCheckBox.h>
#include <TreeElementListBox.h>
#include <PositionElement.h>
#include <TreeElementControlInterface.h>
#define TAILLEMAX 1024
using namespace Regards::Control;
using namespace Regards::Window;

CFiltreEffect::CFiltreEffect(IFiltreUpdate* bitmapViewer, CTreeElementControlInterface* interfaceControl, bool isVideo,
                             int bitmapWindowId): yPos(0), cxMax(0), cyMax(0), xMaxPos(0), xMaxPosValue(0), xMinPos(0),
                                                  effectParameter(nullptr)
{
	this->bitmapWindowId = bitmapWindowId;
	filtre = 0;
	widthPosition = 0;
	filterEffect = nullptr;
	source = nullptr;
	index = 0;
	this->isVideo = isVideo;

	CMainTheme* viewerTheme = CMainThemeInit::getInstance();
	if (viewerTheme)
		viewerTheme->GetTreeTheme(&themeTree);
	this->bitmapViewer = bitmapViewer;

	themeTree.themeTriangle.SetHeight(themeTree.GetRowHeight());
	themeTree.themeCheckbox.SetHeight(themeTree.GetRowHeight());
	themeTree.themeDelete.SetHeight(themeTree.GetRowHeight());
	themeTree.themeSlide.SetHeight(themeTree.GetRowHeight());
	themeTree.themeTexte.SetHeight(themeTree.GetRowHeight());
	eventControl = interfaceControl;
	rowWidth.push_back(0);
	rowWidth.push_back(0);
}

CFiltreEffect::~CFiltreEffect(void)
{
	if (filterEffect != nullptr)
		delete filterEffect;
}

void CFiltreEffect::AddTreeInfos(const wxString& exifKey, CTreeElementValue* position, void* value,
                                 const int& typeValue, const int& type)
{
	AddTreeInfos(exifKey, position, value, typeValue, index++, top, child, type);
}

void CFiltreEffect::Init(CEffectParameter* effectParameter, CRegardsBitmap* source, const wxString& filename,
                         const int& filtre)
{
	CBitmapWndViewer* bitmapViewer = nullptr;
	auto bitmapWindow = dynamic_cast<IBitmapWnd*>(wxWindow::FindWindowById(bitmapWindowId));
	if (!isVideo)
	{
		if (bitmapWindow != nullptr)
			bitmapViewer = (CBitmapWndViewer*)bitmapWindow->GetWndPt();
	}

	this->filtre = filtre;
	this->effectParameter = effectParameter;
	this->source = source;

	index = 0;
	top = tr.begin();
	child = top;

	if (filterEffect != nullptr)
		delete filterEffect;

	filterEffect = CFiltreData::CreateEffectPointer(filtre);
	if (filterEffect != nullptr)
	{
		if (source != nullptr)
		{
			source->SetFilename(filename);
			filterEffect->Filter(effectParameter, source, this);
		}
		else
			filterEffect->Filter(effectParameter, filename, this);
	}

	if (bitmapViewer != nullptr && CFiltreData::NeedPreview(filtre))
		bitmapViewer->SetListener(filterEffect);

	if (bitmapViewer != nullptr)
		bitmapViewer->UpdateFiltre(effectParameter);

	CreateElement();
}


void CFiltreEffect::UpdateScreenRatio()
{
	UpdateElement();
}


void CFiltreEffect::UpdateMousePosition()
{
	CBitmapWndViewer* bitmapViewer = nullptr;
	auto bitmapWindow = dynamic_cast<IBitmapWnd*>(wxWindow::FindWindowById(bitmapWindowId));
	if (bitmapWindow != nullptr)
	{
		bitmapViewer = (CBitmapWndViewer*)bitmapWindow->GetWndPt();
	}

	if (bitmapViewer != nullptr)
	{
		CImageLoadingFormat* imageLoad = filterEffect->ApplyEffect(effectParameter, bitmapViewer);
		bitmapViewer->UpdateBitmap(imageLoad, false);
	}
}

CImageLoadingFormat* CFiltreEffect::ApplyEffect()
{
	CBitmapWndViewer* bitmapViewer = nullptr;
	auto bitmapWindow = dynamic_cast<IBitmapWnd*>(wxWindow::FindWindowById(bitmapWindowId));
	if (bitmapWindow != nullptr)
	{
		bitmapViewer = (CBitmapWndViewer*)bitmapWindow->GetWndPt();
	}

	if (filterEffect != nullptr)
	{
		return filterEffect->ApplyEffect(effectParameter, bitmapViewer);
	}
	return nullptr;
}

void CFiltreEffect::SlidePosChange(CTreeElement* treeElement, const int& position, CTreeElementValue* value,
                                   const wxString& key)
{
	if (filterEffect != nullptr)
		filterEffect->FilterChangeParam(effectParameter, value, key);

	if (bitmapViewer != nullptr)
	{
		bitmapViewer->UpdateFiltre(effectParameter);
	}
	//eventControl->UpdateElement(treeElement);
}


void CFiltreEffect::AddTreeInfos(const wxString& exifKey, CTreeElementValue* position, void* value, int typeValue,
                                 const int& index, tree<CTreeData*>::iterator& top, tree<CTreeData*>::iterator& child,
                                 const int& type)
{
	wchar_t seps[] = L".";
	int item = 0;
	wchar_t informations[TAILLEMAX];
	wchar_t* next_token1 = nullptr;

	wcscpy(informations, exifKey.c_str());

	// Establish string and get the first token:
#if defined(WIN32) && _MSC_VER < 1900
	wchar_t * token = wcstok(informations, seps ); // C4996
#else
	wchar_t* token = wcstok(informations, seps, &next_token1); // C4996
#endif

	// Note: strtok is deprecated; consider using strtok_s instead
	while (token != nullptr)
	{
		auto treeData = new CTreeDataEffect();
		treeData->SetKey(token);
#if defined(WIN32) && _MSC_VER < 1900
		token = wcstok(nullptr, seps); // C4996
#else
		token = wcstok(nullptr, seps, &next_token1); // C4996
#endif

		if (token != nullptr)
		{
			treeData->SetIsParent(true);

			if (index > 0)
			{
				if (item == 0)
				{
					tree<CTreeData*>::iterator it;
					//Recherche de la clé
					it = FindKey(treeData->GetKey());
					if (it != nullptr)
					{
						child = it;
						item++;
						delete(treeData);
						continue;
					}
				}
				else
				{
					tree<CTreeData*>::iterator it;
					//Recherche de la clé
					it = FindKey(treeData->GetKey(), child);
					if (it != nullptr)
					{
						child = it;
						item++;
						delete(treeData);
						continue;
					}
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
			treeData->SetInitValue(position);
			treeData->SetValue(value, typeValue);
			treeData->SetExifKey(exifKey);
			treeData->SetType(type);
			tr.append_child(child, treeData);
		}
		item++;
	}
}

void CFiltreEffect::CreateChildTree(tree<CTreeData*>::sibling_iterator& parent)
{
	CPositionElement* pos_element;
	tree<CTreeData*>::sibling_iterator it = tr.begin(parent);
	//tree<CTreeData *>::iterator itend = tr.end(parent);
	bool isVisible = true;
	//int i =

	for (int i = 0; i < parent.number_of_children(); i++)
	{
		int profondeur = tr.depth(it);
		auto data = static_cast<CTreeDataEffect*>(*it);

		if (data->HasValue() || it.number_of_children() == 0)
		{
			int xPos = widthPosition * profondeur;
			int widthElementColumn2 = 0;

			CTreeElementTexte* treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(),
			                                                         data->GetKey());
			treeElementTexte->SetVisible(isVisible);
			pos_element = CreatePositionElement(xPos, yPos, nbRow, 0, themeTree.GetRowWidth(), themeTree.GetRowHeight(),
			                                   ELEMENT_TEXTE, treeElementTexte, data, false);

			int widthElementColumn1 = xPos + pos_element->GetWidth() + themeTree.GetMargeX();

			if (data->HasValue())
			{
				xPos = themeTree.GetMargeX();
				if (data->GetType() == TYPE_SLIDE)
				{
					CTreeElementSlide* treeElementSlide = CreateSlideElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(),
					                                                         data->GetInitValue(), data->GetVectorValue(),
					                                                         data->GetExifKey());
					treeElementSlide->SetVisible(isVisible);
					pos_element = CreatePositionElement(xPos, yPos, nbRow, 1, themeTree.GetRowWidth(),
					                                   themeTree.GetRowHeight(), ELEMENT_SLIDE, treeElementSlide, data);
					widthElementColumn2 = xPos + pos_element->GetWidth() + themeTree.GetMargeX();
				}
				else if (data->GetType() == TYPE_CHECKBOX)
				{
					CTreeElementCheckBox* treeElementCheckbox = CreateCheckBoxElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(),
						data->GetBoolValue());
					treeElementCheckbox->SetVisible(isVisible);
					pos_element = CreatePositionElement(xPos, yPos, nbRow, 1, themeTree.GetRowWidth(),
					                                   themeTree.GetRowHeight(), ELEMENT_CHECKBOX, treeElementCheckbox,
					                                   data);
					widthElementColumn2 = xPos + pos_element->GetWidth() + themeTree.GetMargeX();
				}
				else if (data->GetType() == TYPE_LISTBOX)
				{
					CTreeElementListBox* treeElementListbox = CreateListBoxElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(),
						data->GetMetadataValue(), data->GetIndex(),
						data->GetExifKey());
					treeElementListbox->SetVisible(isVisible);
					pos_element = CreatePositionElement(xPos, yPos, nbRow, 1, themeTree.GetRowWidth(),
					                                   themeTree.GetRowHeight(), ELEMENT_LISTBOX, treeElementListbox,
					                                   data);
					widthElementColumn2 = xPos + pos_element->GetWidth() + themeTree.GetMargeX();
				}
			}

			if (isVisible)
			{
				yPos += themeTree.GetRowHeight();

				nbRow++;
				if (rowWidth[0] < widthElementColumn1)
					rowWidth[0] = widthElementColumn1;

				if (rowWidth[1] < widthElementColumn2)
					rowWidth[1] = widthElementColumn2;
			}
		}
		else
		{
			int xPos = widthPosition * profondeur;

			CTreeElementTriangle* treeElementTriangle = CreateTriangleElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(),
			                                                                  true);
			treeElementTriangle->SetVisible(isVisible);
			pos_element = CreatePositionElement(xPos, yPos, nbRow, 0, themeTree.GetRowWidth(), themeTree.GetRowHeight(),
			                                   ELEMENT_TRIANGLE, treeElementTriangle, data);

			xPos += pos_element->GetWidth() + themeTree.GetMargeX();
			CTreeElementTexte* treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(),
			                                                         data->GetKey());
			treeElementTexte->SetVisible(isVisible);
			pos_element = CreatePositionElement(xPos, yPos, nbRow, 0, themeTree.GetRowWidth(), themeTree.GetRowHeight(),
			                                   ELEMENT_TEXTE, treeElementTexte, data, false);


			int widthElement = xPos + pos_element->GetWidth() + themeTree.GetMargeX();
			yPos += themeTree.GetRowHeight();

			nbRow++;
			if (rowWidth[0] < widthElement)
				rowWidth[0] = widthElement;

			CreateChildTree(it);
		}
		++it;
	}
}


void CFiltreEffect::UpdateChildTree(tree<CTreeData*>::sibling_iterator& parent)
{
	CPositionElement* pos_element;
	tree<CTreeData*>::sibling_iterator it = tr.begin(parent);
	//tree<CTreeData *>::iterator itend = tr.end(parent);
	bool isVisible = true;
	//int i =

	for (int i = 0; i < parent.number_of_children(); i++)
	{
		int profondeur = tr.depth(it);
		auto data = static_cast<CTreeDataEffect*>(*it);

		if (data->HasValue() || it.number_of_children() == 0)
		{
			int xPos = widthPosition * profondeur;
			int widthElementColumn2 = 0;

			pos_element = GetElement(data, ELEMENT_TEXTE);
			if (pos_element != nullptr)
			{
				auto treeElementTexte = static_cast<CTreeElementTexte*>(pos_element->GetTreeElement());
				treeElementTexte->SetVisible(isVisible);
				pos_element->SetX(xPos);
				pos_element->SetY(yPos);
			}
			else
			{
				CTreeElementTexte* treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(),
				                                                         data->GetKey());
				treeElementTexte->SetVisible(isVisible);
				pos_element = CreatePositionElement(xPos, yPos, nbRow, 0, themeTree.GetRowWidth(),
				                                   themeTree.GetRowHeight(), ELEMENT_TEXTE, treeElementTexte, data,
				                                   false);
			}
			const int widthElementColumn1 = xPos + pos_element->GetWidth() + themeTree.GetMargeX();

			if (data->HasValue() > 0)
			{
				CTreeElementListBox * tree_element_list_box;
				CTreeElementCheckBox * tree_element_check;
				CTreeElementSlide * tree_element_slide;
				xPos = themeTree.GetMargeX();
				if (data->GetType() == TYPE_SLIDE)
					pos_element = GetElement(data, ELEMENT_SLIDE);
				else if (data->GetType() == TYPE_CHECKBOX)
					pos_element = GetElement(data, ELEMENT_CHECKBOX);
				else if (data->GetType() == TYPE_LISTBOX)
					pos_element = GetElement(data, ELEMENT_LISTBOX);

				if (pos_element != nullptr)
				{
					if (data->GetType() == TYPE_SLIDE)
					{
						tree_element_slide = static_cast<CTreeElementSlide*>(pos_element->GetTreeElement());
						tree_element_slide->SetVisible(isVisible);
						tree_element_slide->SetElementPos(xPos, yPos);
					}
					else if (data->GetType() == TYPE_CHECKBOX)
					{
						tree_element_check = static_cast<CTreeElementCheckBox*>(pos_element->GetTreeElement());
						tree_element_check->SetVisible(isVisible);
						tree_element_check->SetElementPos(xPos, yPos);
					}
					else if (data->GetType() == TYPE_LISTBOX)
					{
						tree_element_list_box = static_cast<CTreeElementListBox*>(pos_element->GetTreeElement());
						tree_element_list_box->SetVisible(isVisible);
						tree_element_list_box->SetElementPos(xPos, yPos);
					}

					pos_element->SetX(xPos);
					pos_element->SetY(yPos);
				}
				else if (data->GetType() == TYPE_SLIDE)
				{
					tree_element_slide = CreateSlideElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(),
					                                      data->GetInitValue(), data->GetVectorValue(),
					                                      data->GetExifKey());
					tree_element_slide->SetVisible(isVisible);
					pos_element = CreatePositionElement(xPos, yPos, nbRow, 1, themeTree.GetRowWidth(),
					                                   themeTree.GetRowHeight(), ELEMENT_SLIDE, tree_element_slide, data);
				}
				else if (data->GetType() == TYPE_CHECKBOX)
				{
					tree_element_check = CreateCheckBoxElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(),
					                                         data->GetBoolValue());
					tree_element_check->SetVisible(isVisible);
					pos_element = CreatePositionElement(xPos, yPos, nbRow, 1, themeTree.GetRowWidth(),
					                                   themeTree.GetRowHeight(), ELEMENT_CHECKBOX, tree_element_check,
					                                   data);
				}
				else if (data->GetType() == TYPE_LISTBOX)
				{
					tree_element_list_box = CreateListBoxElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(),
					                                          data->GetMetadataValue(), data->GetIndex(),
					                                          data->GetExifKey());
					tree_element_list_box->SetVisible(isVisible);
					pos_element = CreatePositionElement(xPos, yPos, nbRow, 1, themeTree.GetRowWidth(),
					                                   themeTree.GetRowHeight(), ELEMENT_LISTBOX, tree_element_list_box,
					                                   data);
				}

				widthElementColumn2 = xPos + pos_element->GetWidth() + themeTree.GetMargeX();
			}

			if (isVisible)
			{
				yPos += themeTree.GetRowHeight();

				nbRow++;
				if (rowWidth[0] < widthElementColumn1)
					rowWidth[0] = widthElementColumn1;

				if (rowWidth[1] < widthElementColumn2)
					rowWidth[1] = widthElementColumn2;
			}
		}
		else
		{
			int xPos = widthPosition * profondeur;
			CTreeElementTriangle* tree_element_triangle;

			pos_element = GetElement(data, ELEMENT_TRIANGLE);
			if (pos_element != nullptr)
			{
				tree_element_triangle = static_cast<CTreeElementTriangle*>(pos_element->GetTreeElement());
				tree_element_triangle->SetVisible(isVisible);
				pos_element->SetX(xPos);
				pos_element->SetY(yPos);
			}
			else
			{
				tree_element_triangle = CreateTriangleElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), true);
				tree_element_triangle->SetVisible(isVisible);
				pos_element = CreatePositionElement(xPos, yPos, nbRow, 0, themeTree.GetRowWidth(),
				                                   themeTree.GetRowHeight(), ELEMENT_TRIANGLE, tree_element_triangle,
				                                   data);
			}

			xPos += pos_element->GetWidth() + themeTree.GetMargeX();
			pos_element = GetElement(data, ELEMENT_TEXTE);
			if (pos_element != nullptr)
			{
				auto treeElementTexte = static_cast<CTreeElementTexte*>(pos_element->GetTreeElement());
				treeElementTexte->SetVisible(isVisible);
				pos_element->SetX(xPos);
				pos_element->SetY(yPos);
			}
			else
			{
				auto treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(),
				                                           data->GetKey());
				treeElementTexte->SetVisible(isVisible);
				pos_element = CreatePositionElement(xPos, yPos, nbRow, 0, themeTree.GetRowWidth(),
				                                   themeTree.GetRowHeight(), ELEMENT_TEXTE, treeElementTexte, data,
				                                   false);
			}

			const int widthElement = xPos + pos_element->GetWidth() + themeTree.GetMargeX();
			yPos += themeTree.GetRowHeight();

			nbRow++;
			if (rowWidth[0] < widthElement)
				rowWidth[0] = widthElement;

			bool isOpen = tree_element_triangle->GetOpen();
			if (isOpen)
				UpdateChildTree(it);
		}
		++it;
	}
}

void CFiltreEffect::UpdateElement()
{
	for (CPositionElement* value : vectorPosElement)
	{
		if (value != nullptr)
		{
			value->SetX(0);
			value->SetY(0);
			CTreeElement* treeElement = value->GetTreeElement();
			if (treeElement != nullptr)
			{
				treeElement->SetVisible(false);
				treeElement->SetElementPos(0, 0);
			}
		}
	}

	tree<CTreeData*>::sibling_iterator it = tr.begin();
	auto itend = tr.end();
	yPos = 0;
	nbRow = 0;
	bool isVisible = true;

	while (it != itend)
	{
		CTreeData* data = *it;
		int profondeur = tr.depth(it);
		if (profondeur == 0)
		{
			int xPos = themeTree.GetMargeX();
			int widthElement = 0;
			CTreeElementTriangle* tree_element_triangle;
			CPositionElement* posElement = GetElement(data, ELEMENT_TRIANGLE);
			if (posElement != nullptr)
			{
				tree_element_triangle = static_cast<CTreeElementTriangle*>(posElement->GetTreeElement());
				tree_element_triangle->SetVisible(isVisible);
				posElement->SetX(xPos);
				posElement->SetY(yPos);
			}
			else
			{
				tree_element_triangle = CreateTriangleElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), true);
				tree_element_triangle->SetVisible(isVisible);
				posElement = CreatePositionElement(xPos, yPos, nbRow, 0, themeTree.GetRowWidth(),
				                                   themeTree.GetRowHeight(), ELEMENT_TRIANGLE, tree_element_triangle,
				                                   data);
			}
			widthPosition = xPos + posElement->GetWidth() + themeTree.GetMargeX();
			xPos += posElement->GetWidth() + themeTree.GetMargeX();

			posElement = GetElement(data, ELEMENT_TEXTE);
			if (posElement != nullptr)
			{
				auto treeElementTexte = static_cast<CTreeElementTexte*>(posElement->GetTreeElement());
				treeElementTexte->SetVisible(isVisible);
				posElement->SetX(xPos);
				posElement->SetY(yPos);
			}
			else
			{
				CTreeElementTexte* treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(),
				                                                         data->GetKey());
				treeElementTexte->SetVisible(isVisible);
				posElement = CreatePositionElement(xPos, yPos, nbRow, 0, themeTree.GetRowWidth(),
				                                   themeTree.GetRowHeight(), ELEMENT_TEXTE, treeElementTexte, data,
				                                   false);
			}

			widthElement += xPos + posElement->GetWidth() + themeTree.GetMargeX();
			yPos += themeTree.GetRowHeight();
			nbRow++;
			if (rowWidth[0] < widthElement)
				rowWidth[0] = widthElement;

			bool isOpen = tree_element_triangle->GetOpen();
			if (isOpen)
				UpdateChildTree(it);
		}
		++it;
	}
}


void CFiltreEffect::CreateElement()
{
	tree<CTreeData*>::sibling_iterator it = tr.begin();
	auto itend = tr.end();
	yPos = 0;
	nbRow = 0;
	vectorPosElement.clear();
	vectorPosElementDynamic.clear();
	bool isVisible = true;

	while (it != itend)
	{
		CTreeData* data = *it;
		int profondeur = tr.depth(it);
		if (profondeur == 0)
		{
			int xPos = themeTree.GetMargeX();
			int widthElement = 0;
			auto treeElementTriangle = CreateTriangleElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(),
			                                                 true);
			treeElementTriangle->SetVisible(isVisible);
			auto posElement = CreatePositionElement(xPos, yPos, nbRow, 0, themeTree.GetRowWidth(),
			                                        themeTree.GetRowHeight(),
			                                        ELEMENT_TRIANGLE, treeElementTriangle, data);
			widthPosition = xPos + posElement->GetWidth() + themeTree.GetMargeX();
			xPos += posElement->GetWidth() + themeTree.GetMargeX();

			CTreeElementTexte* treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(),
			                                                         data->GetKey());
			treeElementTexte->SetVisible(isVisible);
			posElement = CreatePositionElement(xPos, yPos, nbRow, 0, themeTree.GetRowWidth(), themeTree.GetRowHeight(),
			                                   ELEMENT_TEXTE, treeElementTexte, data, false);


			widthElement += xPos + posElement->GetWidth() + themeTree.GetMargeX();
			yPos += themeTree.GetRowHeight();
			nbRow++;
			if (rowWidth[0] < widthElement)
				rowWidth[0] = widthElement;

			CreateChildTree(it);
		}
		++it;
	}
}

void CFiltreEffect::UnclickOnElement(CPositionElement* element, wxWindow* window, const int& x, const int& y,
                                     const int& posLargeur, const int& posHauteur)
{
	CTreeElement* treeElement = element->GetTreeElement();
	if (element->GetType() == ELEMENT_SLIDE)
	{
		auto treeElementSlide = static_cast<CTreeElementSlide*>(treeElement);

		int xPos = 0;
		if (treeElementSlide->GetRow() > 0)
			xPos = GetWidthRow(treeElementSlide->GetRow() - 1);

		treeElementSlide->UnclickElement(window, (x + posLargeur) - (element->GetX() + xPos),
		                                 (y + posHauteur) - element->GetY());
	}
}

void CFiltreEffect::ClickOnElement(CPositionElement* element, wxWindow* window, const int& x, const int& y,
                                   const int& posLargeur, const int& posHauteur)
{
	bool update = false;
	CTreeElement* treeElement = element->GetTreeElement();
	if (element->GetType() == ELEMENT_SLIDE)
	{
		auto treeElementSlide = static_cast<CTreeElementSlide*>(treeElement);

		int xPos = 0;
		if (treeElementSlide->GetRow() > 0)
			xPos = GetWidthRow(treeElementSlide->GetRow() - 1);

		//treeElementSlide->SetElementPos((x - xPos) - posLargeur, y - posHauteur);
		treeElementSlide->ClickElement(window, (x + posLargeur) - (element->GetX() + xPos),
		                               (y + posHauteur) - element->GetY());

		update = true;
	}
	else if (element->GetType() == ELEMENT_TRIANGLE)
	{
		auto treeElementTriangle = static_cast<CTreeElementTriangle*>(treeElement);
		treeElementTriangle->ClickElement(window, (x + posLargeur) - element->GetX(),
		                                  (y + posHauteur) - element->GetY());
		update = true;
	}
	else if (element->GetType() == ELEMENT_CHECKBOX)
	{
		auto treeElementCheckBox = static_cast<CTreeElementCheckBox*>(treeElement);
		treeElementCheckBox->ClickElement(window, (x + posLargeur) - element->GetX(),
		                                  (y + posHauteur) - element->GetY());

		if (filterEffect != nullptr)
		{
			auto data = static_cast<CTreeDataEffect*>(element->GetTreeData());
			CTreeElementValueBool tree_element_value_bool(treeElementCheckBox->GetCheckState());
			filterEffect->FilterChangeParam(effectParameter, &tree_element_value_bool, data->GetExifKey());
		}

		update = true;

		if (bitmapViewer != nullptr)
			bitmapViewer->UpdateFiltre(effectParameter);
	}
	else if (element->GetType() == ELEMENT_LISTBOX)
	{
		auto treeElementListbox = static_cast<CTreeElementListBox*>(treeElement);

		int xPos = 0;
		if (treeElementListbox->GetRow() > 0)
			xPos = GetWidthRow(treeElementListbox->GetRow() - 1);

		//treeElementSlide->SetElementPos((x - xPos) - posLargeur, y - posHauteur);
		treeElementListbox->ClickElement(window, (x + posLargeur) - (element->GetX() + xPos),
		                                 (y + posHauteur) - element->GetY());


		update = true;
	}

	if (update)
	{
		UpdateElement();
		eventControl->UpdateTreeControl();
	}
}

void CFiltreEffect::MouseOver(wxDC* deviceContext, CPositionElement* element, const int& x, const int& y,
                              const int& posLargeur, const int& posHauteur, bool& update)
{
	CTreeElement* treeElement = element->GetTreeElement();
	if (element->GetType() == ELEMENT_SLIDE)
	{
		auto treeElementSlide = static_cast<CTreeElementSlide*>(treeElement);

		int xPos = 0;
		if (treeElementSlide->GetRow() > 0)
			xPos = GetWidthRow(treeElementSlide->GetRow() - 1);

		//
		if (treeElementSlide->MouseBlock())
		{
			treeElementSlide->MouseOver(deviceContext, (x + posLargeur) - (element->GetX() + xPos),
			                            (y + posHauteur) - element->GetY(), update);
			treeElementSlide->DrawElement(deviceContext, (element->GetX() + xPos) - posLargeur,
			                              element->GetY() - posHauteur);
		}
	}
}
