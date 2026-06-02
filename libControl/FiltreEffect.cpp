#include <header.h>
#include "FiltreEffect.h"
#include <TreeDataEffect.h>
#include <FilterData.h>
#include "MainTheme.h"
#include "MainThemeInit.h"
#include "BitmapWndViewer.h"
#include "BitmapWnd3d.h"
#include <TreeElementSlide.h>
#include <TreeElementTriangle.h>
#include <TreeElementListBox.h>
#include <TreeElementCheckBox.h>
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

void CFiltreEffect::Init(CEffectParameter* effectParameter, cv::Mat source, const wxString& filename,
                         const int& filtre)
{
	CBitmapWndViewer* bitmapViewer = nullptr;
	auto bitmapWindow = dynamic_cast<IBitmapWnd*>(wxWindow::FindWindowById(bitmapWindowId));
	if (!isVideo)
	{
		if (bitmapWindow != nullptr)
			bitmapViewer = static_cast<CBitmapWndViewer*>(bitmapWindow->GetWndPt());
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
		if (!source.empty())
		{
			filterEffect->Filter(effectParameter, source, filename, this);
		}
		else
			filterEffect->Filter(effectParameter, filename, this);
	}

	if (bitmapViewer != nullptr && CFiltreData::NeedPreview(filtre))
		bitmapViewer->SetListener(filterEffect);

	if (bitmapViewer != nullptr)
		bitmapViewer->UpdateFiltre(effectParameter);

	RenderElement(RenderMode::Create);
}


void CFiltreEffect::UpdateScreenRatio()
{
	RenderElement(RenderMode::Update);
}


void CFiltreEffect::UpdateMousePosition()
{
	CBitmapWndViewer* bitmapViewer = nullptr;
	auto bitmapWindow = dynamic_cast<IBitmapWnd*>(wxWindow::FindWindowById(bitmapWindowId));
	if (bitmapWindow != nullptr)
	{
		bitmapViewer = static_cast<CBitmapWndViewer*>(bitmapWindow->GetWndPt());
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
		bitmapViewer = static_cast<CBitmapWndViewer*>(bitmapWindow->GetWndPt());
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
				tree<CTreeData*>::iterator it;
				if (item == 0)
					it = FindKey(treeData->GetKey());
				else
					it = FindKey(treeData->GetKey(), child);

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
			treeData->SetInitValue(position);
			treeData->SetValue(value, typeValue);
			treeData->SetExifKey(exifKey);
			treeData->SetType(type);
			tr.append_child(child, treeData);
		}
		item++;
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
	switch (element->GetType())
	{
		case ELEMENT_SLIDE:
			{
				auto treeElementSlide = static_cast<CTreeElementSlide*>(treeElement);
				int xPos = 0;
				if (treeElementSlide->GetRow() > 0)
					xPos = GetWidthRow(treeElementSlide->GetRow() - 1);
				treeElementSlide->ClickElement(window, (x + posLargeur) - (element->GetX() + xPos),
					(y + posHauteur) - element->GetY());
				update = true;
				break;
			}
		case ELEMENT_TRIANGLE:
			{
				auto treeElementTriangle = static_cast<CTreeElementTriangle*>(treeElement);
				treeElementTriangle->ClickElement(window, (x + posLargeur) - element->GetX(),
					(y + posHauteur) - element->GetY());
				update = true;
				break;
			}
		case ELEMENT_CHECKBOX:
			{
				CTreeElementCheckBox* treeElementCheckBox = static_cast<CTreeElementCheckBox*>(treeElement);
				treeElementCheckBox->ClickElement(window, (x + posLargeur) - element->GetX(),
					(y + posHauteur) - element->GetY());

				if (filterEffect != nullptr)
				{
					auto data = static_cast<CTreeDataEffect*>(element->GetTreeData());
					CTreeElementValueBool tree_element_value_bool(treeElementCheckBox->GetCheckState());
					filterEffect->FilterChangeParam(effectParameter, &tree_element_value_bool, data->GetExifKey());
				}

				update = true;
				break;
			}

	}

	if (update)
	{
		RenderElement(RenderMode::Update);
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

CPositionElement* CFiltreEffect::RenderListBox(
	CTreeDataEffect * data,
	int& xPos,
	int& yPos,
	bool visible,
	RenderMode mode)
{
	CPositionElement* posElement = nullptr;

	if (mode == RenderMode::Update)
		posElement = GetElement(data, ELEMENT_LISTBOX);

	if (posElement == nullptr)
	{
		CTreeElementListBox * treeElementListbox = CreateListBoxElement(
			themeTree.GetRowWidth(), themeTree.GetRowHeight(),
			data->GetMetadataValue(), data->GetIndex(),
			data->GetExifKey());
		treeElementListbox->SetVisible(visible);
		posElement = CreatePositionElement(xPos, yPos, nbRow, 1, themeTree.GetRowWidth(),
			themeTree.GetRowHeight(), ELEMENT_LISTBOX, treeElementListbox,
			data);

	}
	else
	{
		auto* text =
			static_cast<CTreeElementListBox*>(posElement->GetTreeElement());

		text->SetVisible(visible);
		text->SetElementPos(xPos, yPos);

		posElement->SetX(xPos);
		posElement->SetY(yPos);
	}

	return posElement;
}


CPositionElement* CFiltreEffect::RenderSlide(
	CTreeDataEffect* data,
	int& xPos,
	int& yPos,
	bool visible,
	RenderMode mode)
{
	CPositionElement* posElement = nullptr;

	if (mode == RenderMode::Update)
		posElement = GetElement(data, ELEMENT_SLIDE);

	if (posElement == nullptr)
	{
		CTreeElementSlide * treeElementSlide = CreateSlideElement(
			themeTree.GetRowWidth(), themeTree.GetRowHeight(),
			data->GetInitValue(), data->GetVectorValue(),
			data->GetExifKey());
		treeElementSlide->SetVisible(visible);
		posElement = CreatePositionElement(xPos, yPos, nbRow, 1, themeTree.GetRowWidth(),
			themeTree.GetRowHeight(), ELEMENT_SLIDE, treeElementSlide,
			data);

	}
	else
	{
		auto* text =
			static_cast<CTreeElementSlide*>(posElement->GetTreeElement());

		text->SetVisible(visible);
		text->SetElementPos(xPos, yPos);

		posElement->SetX(xPos);
		posElement->SetY(yPos);
	}

	return posElement;
}

void CFiltreEffect::RenderChildTree(tree<CTreeData*>::sibling_iterator& parent, RenderMode mode)
{
	CPositionElement* pos_element;
	tree<CTreeData*>::sibling_iterator it = tr.begin(parent);
	bool isVisible = true;

	for (int i = 0; i < parent.number_of_children(); i++)
	{
		int profondeur = tr.depth(it);
		auto data = static_cast<CTreeDataEffect*>(*it);

		if (data->HasValue() || it.number_of_children() == 0)
		{
			int xPos = widthPosition * profondeur;
			int widthElementColumn2 = 0;

			pos_element = RenderText(data,
				xPos,
				yPos,
				isVisible,
				mode);

			int widthElementColumn1 = xPos + pos_element->GetWidth() + themeTree.GetMargeX();

			if (data->HasValue())
			{
				xPos = themeTree.GetMargeX();
				switch (data->GetType())
				{
				case TYPE_SLIDE:
					pos_element = RenderSlide(data,
						xPos,
						yPos,
						isVisible,
						mode);
					break;

				case TYPE_CHECKBOX:
					pos_element = RenderCheckBox(data,
						xPos,
						yPos,
						data->GetBoolValue(),
						isVisible,
						mode);
					break;

				case TYPE_LISTBOX:
					pos_element = RenderListBox(data,
						xPos,
						yPos,
						isVisible,
						mode);
					break;
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

			pos_element = RenderTriangle(data,
				xPos,
				yPos,
				isVisible,
				mode);

			xPos += pos_element->GetWidth() + themeTree.GetMargeX();

			pos_element = RenderText(data,
				xPos,
				yPos,
				isVisible,
				mode);

			int widthElement = xPos + pos_element->GetWidth() + themeTree.GetMargeX();
			yPos += themeTree.GetRowHeight();

			nbRow++;
			if (rowWidth[0] < widthElement)
				rowWidth[0] = widthElement;

			RenderChildTree(it, mode);
		}
		++it;
	}
}

void CFiltreEffect::RenderElement(RenderMode mode)
{
	CPositionElement* posElement = nullptr;
	CTreeElementTriangle* treeElementTriangle = nullptr;

	for (auto value : vectorPosElement)
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

			posElement = RenderTriangle(data,
				xPos,
				yPos,
				isVisible,
				mode);

			treeElementTriangle = static_cast<CTreeElementTriangle*>(posElement->GetTreeElement());

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
				const bool isOpen = treeElementTriangle->GetOpen();
				if (isOpen)
					RenderChildTree(it, mode);
			}
		}
		++it;
	}
}

