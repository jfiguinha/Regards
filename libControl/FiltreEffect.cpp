#include <header.h>
#include "FiltreEffect.h"
#include <TreeDataEffect.h>
#include <FilterData.h>
#include <wx/tokenzr.h>
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
#include <ImageLoadingFormat.h>
#include <appcontext.h>
extern AppContext application_context;
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
		bitmapViewer->SetListener(filterEffect.get());

	if (bitmapViewer != nullptr)
		bitmapViewer->UpdateFiltre(effectParameter);

	RenderElement(RenderMode::Create);
}


void CFiltreEffect::UpdateScreenRatio()
{
	RenderElement(RenderMode::Update);
}


void CFiltreEffect::UpdateBitmapToViewer(CImageLoadingFormat* bitmap)
{
	auto bitmapWindow = wxWindow::FindWindowById(bitmapWindowId);
	auto event = new wxCommandEvent(wxEVENT_SETBITMAP);
	event->SetClientData(bitmap);
	wxQueueEvent(bitmapWindow, event);
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
		if(imageLoad != nullptr)
			UpdateBitmapToViewer(imageLoad);
		else
		{
			//Default picture if no effect is applied
			CImageLoadingFormat* imageLoad = new CImageLoadingFormat();
			cv::Mat mat = application_context.GetDefaultPictureThumbnail();
			imageLoad->SetPicture(mat);
			UpdateBitmapToViewer(imageLoad);
		}
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
void CFiltreEffect::AddTreeInfos(const wxString& exifKey,
	CTreeElementValue* position,
	void* value,
	int typeValue,
	const int& index,
	tree<CTreeData*>::iterator& top,
	tree<CTreeData*>::iterator& child,
	const int& type)
{
	AddTreeInfosImpl<CTreeDataEffect>(
		exifKey,
		index,
		top,
		child,
		[&](CTreeDataEffect* treeData)
		{
			treeData->SetInitValue(position);
			treeData->SetValue(value, typeValue);
			treeData->SetExifKey(exifKey);
			treeData->SetType(type);
		});
}

CPositionElement* CFiltreEffect::RenderSlide(
	CTreeData* dataEffect,
	int& xPos,
	int& yPos,
	bool visible,
	RenderMode mode)
{
	CPositionElement* posElement = nullptr;
	auto data = static_cast<CTreeDataEffect*>(dataEffect);

	if (mode == RenderMode::Update)
		posElement = GetElement(data, ELEMENT_SLIDE);

	if (posElement == nullptr)
	{
		CTreeElementSlide* treeElementSlide = CreateSlideElement(
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
		auto* slide = static_cast<CTreeElementSlide*>(posElement->GetTreeElement());

		slide->SetVisible(visible);
		slide->SetElementPos(xPos, yPos);

		posElement->SetX(xPos);
		posElement->SetY(yPos);
	}

	return posElement;
}

CPositionElement* CFiltreEffect::RenderCheckbox(
	CTreeData* dataEffect,
	int& xPos,
	int& yPos,
	bool visible,
	RenderMode mode)
{
	CPositionElement* posElement = nullptr;
	auto data = static_cast<CTreeDataEffect*>(dataEffect);

	if (mode == RenderMode::Update)
		posElement = GetElement(data, ELEMENT_CHECKBOX);

	if (posElement == nullptr)
	{
		CTreeElementCheckBox* treeElementCheckbox = CreateCheckBoxElement(
			themeTree.GetRowWidth(), themeTree.GetRowHeight(),
			data->GetBoolValue());
		treeElementCheckbox->SetVisible(visible);
		posElement = CreatePositionElement(xPos, yPos, nbRow, 1, themeTree.GetRowWidth(),
			themeTree.GetRowHeight(), ELEMENT_CHECKBOX, treeElementCheckbox,
			data);
	}
	else
	{
		auto* checkbox = static_cast<CTreeElementCheckBox*>(posElement->GetTreeElement());

		checkbox->SetVisible(visible);
		checkbox->SetElementPos(xPos, yPos);

		posElement->SetX(xPos);
		posElement->SetY(yPos);
	}

	return posElement;
}


CPositionElement* CFiltreEffect::RenderList(
	CTreeData* dataEffect,
	int& xPos,
	int& yPos,
	bool visible,
	RenderMode mode)
{
	CPositionElement* posElement = nullptr;
	auto data = static_cast<CTreeDataEffect*>(dataEffect);

	if (mode == RenderMode::Update)
		posElement = GetElement(data, ELEMENT_LISTBOX);

	if (posElement == nullptr)
	{
		CTreeElementListBox* treeElementListBox = CreateListBoxElement(
			themeTree.GetRowWidth(), themeTree.GetRowHeight(),
			data->GetMetadataValue(), data->GetIndex(),
			data->GetExifKey());
		treeElementListBox->SetVisible(visible);
		posElement = CreatePositionElement(xPos, yPos, nbRow, 1, themeTree.GetRowWidth(),
			themeTree.GetRowHeight(), ELEMENT_LISTBOX, treeElementListBox,
			data); 
	}
	else
	{
		auto* checkbox = static_cast<CTreeElementListBox*>(posElement->GetTreeElement());

		checkbox->SetVisible(visible);
		checkbox->SetElementPos(xPos, yPos);

		posElement->SetX(xPos);
		posElement->SetY(yPos);
	}

	return posElement;
}

void CFiltreEffect::CreateElementChild(tree<CTreeData*>::sibling_iterator& it, CTreeDataEffect* data, int widthPosition, int profondeur, bool isVisible, RenderMode mode)
{
	int xPos = widthPosition * profondeur;
	int widthElementColumn2 = 0;
	int widthElementColumn1 = 0;

	CPositionElement* pos_element = RenderText(data, xPos, yPos, isVisible, mode, false);
	widthElementColumn1 = xPos + pos_element->GetWidth() + themeTree.GetMargeX();

	if (data->HasValue())
	{
		xPos = themeTree.GetMargeX();
		if (data->GetType() == TYPE_SLIDE)
		{
			pos_element = RenderSlide(
				data,
				xPos, yPos,
				isVisible,
				mode);
		}
		else if (data->GetType() == TYPE_CHECKBOX)
		{
			pos_element = RenderCheckbox(
				data,
				xPos, yPos,
				isVisible,
				mode);
		}
		else if (data->GetType() == TYPE_LISTBOX)
		{
			pos_element = RenderList(
				data,
				xPos, yPos,
				isVisible,
				mode);

		}

		if (pos_element != nullptr)
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


void CFiltreEffect::UpdateElementChild(tree<CTreeData*>::sibling_iterator& it, CTreeDataEffect * data, int widthPosition, int profondeur, bool isVisible, RenderMode mode)
{
	int xPos = widthPosition * profondeur;
	CTreeElementTriangle* tree_element_triangle = nullptr;
	CPositionElement* pos_element = RenderTriangle(
		data,
		xPos, yPos,
		isVisible,
		mode);

	tree_element_triangle = static_cast<CTreeElementTriangle*>(pos_element->GetTreeElement());
	xPos += pos_element->GetWidth() + themeTree.GetMargeX();

	pos_element = RenderText(
		data,
		xPos, yPos,
		isVisible,
		mode);


	int widthElement = xPos + pos_element->GetWidth() + themeTree.GetMargeX();
	yPos += themeTree.GetRowHeight();

	nbRow++;
	if (rowWidth[0] < widthElement)
		rowWidth[0] = widthElement;

	if(mode == RenderMode::Create)
		UpdateElement(it, mode);
	else
	{
		bool isOpen = tree_element_triangle->GetOpen();
		if (isOpen)
			UpdateElement(it, mode);
	}
}

void CFiltreEffect::UpdateElement(tree<CTreeData*>::sibling_iterator& parent, RenderMode mode)
{
	tree<CTreeData*>::sibling_iterator it = tr.begin(parent);
	bool isVisible = true;

	for (int i = 0; i < parent.number_of_children(); i++)
	{
		int profondeur = tr.depth(it);
		auto data = static_cast<CTreeDataEffect*>(*it);

		if (data->HasValue() || it.number_of_children() == 0)
		{
			CreateElementChild(it, data, widthPosition, profondeur, isVisible, mode);
		}
		else
		{
			UpdateElementChild(it, data, widthPosition, profondeur, isVisible, mode);
		}
		++it;
	}
}

void CFiltreEffect::RenderElement(RenderMode mode)
{
	tree<CTreeData*>::sibling_iterator it = tr.begin();
	auto itend = tr.end();
	bool isVisible = true;
	yPos = 0;
	nbRow = 0;

	if (mode == RenderMode::Create)
	{
		vectorPosElement.clear();
		vectorPosElementDynamic.clear();
	}
	else if (mode == RenderMode::Update)
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
	}

	while (it != itend)
	{
		CTreeData* data = *it;
		int profondeur = tr.depth(it);
		if (profondeur == 0)
		{
			int xPos = themeTree.GetMargeX();
			int widthElement = 0;

			CTreeElementTriangle* tree_element_triangle = nullptr;

			CPositionElement* posElement = RenderTriangle(
				data,
				xPos, yPos,
				isVisible,
				mode);

			tree_element_triangle = static_cast<CTreeElementTriangle*>(posElement->GetTreeElement());
			widthPosition = xPos + posElement->GetWidth() + themeTree.GetMargeX();
			xPos += posElement->GetWidth() + themeTree.GetMargeX();

			posElement = RenderText(
				data,
				xPos, yPos,
				isVisible,
				mode);

			widthElement += xPos + posElement->GetWidth() + themeTree.GetMargeX();
			yPos += themeTree.GetRowHeight();
			nbRow++;
			if (rowWidth[0] < widthElement)
				rowWidth[0] = widthElement;

			if (mode == RenderMode::Update)
			{
				bool isOpen = tree_element_triangle->GetOpen();
				if (isOpen)
					UpdateElement(it, mode);
			}
			else
				UpdateElement(it, mode);
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
