#include <header.h>
#include "FiltreEffect.h"
#include <ConvertUtility.h>
#include <TreeDataEffect.h>
#include <FilterData.h>
#include <LibResource.h>
#include <RegardsBitmap.h>
#include <FilterData.h>
#include "MainTheme.h"
#include "MainThemeInit.h"
#include "BitmapWndViewer.h"
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
CFiltreEffect::CFiltreEffect(IFiltreUpdate * bitmapViewer, CTreeElementControlInterface * interfaceControl, bool isVideo, int bitmapWindowId)
{
	this->bitmapWindowId = bitmapWindowId;
	filtre = 0;
	widthPosition = 0;
	filterEffect = nullptr;
	source = nullptr;
	index = 0;
	this->isVideo = isVideo;

	CMainTheme * viewerTheme = CMainThemeInit::getInstance();
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
    if(filterEffect != nullptr)
        delete filterEffect;
}

void CFiltreEffect::AddTreeInfos(const wxString &exifKey, CTreeElementValue * position, void * value, const int & typeValue, const int &type)
{
    AddTreeInfos(exifKey, position, value, typeValue, index++, top, child, type);
}

void CFiltreEffect::Init(CEffectParameter * effectParameter, CRegardsBitmap * source, const wxString &filename, const int &filtre)
{
	CBitmapWndViewer * bitmapViewer = (CBitmapWndViewer*)wxWindow::FindWindowById(bitmapWindowId);

	this->filtre = filtre;
	this->effectParameter = effectParameter;
	this->source = source;
    
    index = 0;    
    top = tr.begin();
    child = top;
    
    if(filterEffect != nullptr)
        delete filterEffect;

    filterEffect = CFiltreData::CreateEffectPointer(filtre);
    if(filterEffect != nullptr)
    {
        if(source != nullptr)
            filterEffect->Filter(effectParameter, source, this);
        else
            filterEffect->Filter(effectParameter, filename, this);
    }

	if(bitmapViewer != nullptr && CFiltreData::NeedPreview(filtre))
		bitmapViewer->SetListener(filterEffect);

	if (bitmapViewer != nullptr)
	{
		bitmapViewer->UpdateFiltre(effectParameter);
	}

	CreateElement();
}


void CFiltreEffect::UpdateScreenRatio()
{
    UpdateElement();
}


void CFiltreEffect::UpdateMousePosition()
{
	CBitmapWndViewer * bitmapWindow = (CBitmapWndViewer *)wxWindow::FindWindowById(bitmapWindowId);
	if (bitmapWindow != nullptr)
	{
		CImageLoadingFormat * imageLoad = filterEffect->ApplyEffect(effectParameter, bitmapWindow);
		bitmapWindow->UpdateBitmap(imageLoad, false);
	}
}

CImageLoadingFormat * CFiltreEffect::ApplyEffect()
{
	CBitmapWndViewer * bitmapWindow = (CBitmapWndViewer *)wxWindow::FindWindowById(bitmapWindowId);

	if (filterEffect != nullptr)
	{
		return filterEffect->ApplyEffect(effectParameter, bitmapWindow);
	}
	return nullptr;
}

void CFiltreEffect::SlidePosChange(CTreeElement * treeElement, const int &position,  CTreeElementValue * value, const wxString &key)
{
    if(filterEffect != nullptr)
        filterEffect->FilterChangeParam(effectParameter, value, key);
    
	if (bitmapViewer != nullptr)
	{
		bitmapViewer->UpdateFiltre(effectParameter);
	}
	eventControl->UpdateElement(treeElement);
}


void CFiltreEffect::AddTreeInfos(const wxString &exifKey, CTreeElementValue * position, void * value, int typeValue, const int &index, tree<CTreeData *>::iterator &top, tree<CTreeData *>::iterator &child, const int &type)
{
	wchar_t seps[] = L".";
	int item = 0;
	wchar_t informations[TAILLEMAX];
	wchar_t *next_token1 = nullptr;

	wcscpy(informations, exifKey.c_str());

	// Establish string and get the first token:
#if defined(WIN32) && _MSC_VER < 1900
	wchar_t * token = wcstok(informations, seps ); // C4996
#else
	wchar_t * token = wcstok(informations, seps, &next_token1); // C4996
#endif

	// Note: strtok is deprecated; consider using strtok_s instead
	while (token != nullptr)
	{
		CTreeDataEffect * treeData = new CTreeDataEffect();
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
					tree<CTreeData *>::iterator it;
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
					tree<CTreeData *>::iterator it;
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
			treeData->SetValue(value,typeValue);
			treeData->SetExifKey(exifKey);
			treeData->SetType(type);
			tr.append_child(child, treeData);
		}
		item++;
	}
}

void CFiltreEffect::CreateChildTree(tree<CTreeData *>::sibling_iterator &parent)
{
	CPositionElement * posElement = nullptr;
	tree<CTreeData *>::sibling_iterator it = tr.begin(parent);
	//tree<CTreeData *>::iterator itend = tr.end(parent);
	bool isVisible = true;
	//int i = 

	for (int i = 0; i < parent.number_of_children(); i++)
	{
		int profondeur = tr.depth(it);
		CTreeDataEffect * data = (CTreeDataEffect *)*it;

		if (data->HasValue() || it.number_of_children() == 0)
		{
			int xPos = widthPosition * profondeur;
			int widthElementColumn1 = 0;
			int widthElementColumn2 = 0;
			
			CTreeElementTexte * treeElementTexte = nullptr;
			treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey());
			treeElementTexte->SetVisible(isVisible);
			posElement = CreatePositionElement(xPos, yPos, nbRow, 0, themeTree.GetRowWidth(), themeTree.GetRowHeight(), ELEMENT_TEXTE, treeElementTexte, data, false);

			widthElementColumn1 = xPos + posElement->GetWidth() + themeTree.GetMargeX();

			if (data->HasValue())
			{
				xPos = themeTree.GetMargeX();
				if(data->GetType() == TYPE_SLIDE)
				{
					CTreeElementSlide * treeElementSlide = nullptr;
					treeElementSlide = CreateSlideElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetInitValue(), data->GetVectorValue(), data->GetExifKey());
					treeElementSlide->SetVisible(isVisible);
					posElement = CreatePositionElement(xPos, yPos, nbRow, 1, themeTree.GetRowWidth(), themeTree.GetRowHeight(), ELEMENT_SLIDE, treeElementSlide, data);
					widthElementColumn2 = xPos + posElement->GetWidth() + themeTree.GetMargeX();
				}
				else if(data->GetType() == TYPE_CHECKBOX)
				{
					CTreeElementCheckBox * treeElementCheckbox = nullptr;
					treeElementCheckbox = CreateCheckBoxElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetBoolValue());
					treeElementCheckbox->SetVisible(isVisible);
					posElement = CreatePositionElement(xPos, yPos, nbRow, 1, themeTree.GetRowWidth(), themeTree.GetRowHeight(), ELEMENT_CHECKBOX, treeElementCheckbox, data);
					widthElementColumn2 = xPos + posElement->GetWidth() + themeTree.GetMargeX();					
				}
				else if(data->GetType() == TYPE_LISTBOX)
				{
					CTreeElementListBox * treeElementListbox = nullptr;
					treeElementListbox = CreateListBoxElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetMetadataValue(), data->GetIndex(), data->GetExifKey());
					treeElementListbox->SetVisible(isVisible);
					posElement = CreatePositionElement(xPos, yPos, nbRow, 1, themeTree.GetRowWidth(), themeTree.GetRowHeight(), ELEMENT_LISTBOX, treeElementListbox, data);
					widthElementColumn2 = xPos + posElement->GetWidth() + themeTree.GetMargeX();			
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
			int xPos = widthPosition *profondeur;
			int widthElement = 0;
			CTreeElementTexte * treeElementTexte = nullptr;
			CTreeElementTriangle * treeElementTriangle = nullptr;
			
			treeElementTriangle = CreateTriangleElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), true);
			treeElementTriangle->SetVisible(isVisible);
			posElement = CreatePositionElement(xPos, yPos, nbRow, 0, themeTree.GetRowWidth(), themeTree.GetRowHeight(), ELEMENT_TRIANGLE, treeElementTriangle, data);

			xPos += posElement->GetWidth() + themeTree.GetMargeX();
			treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey());
			treeElementTexte->SetVisible(isVisible);
			posElement = CreatePositionElement(xPos, yPos, nbRow, 0, themeTree.GetRowWidth(), themeTree.GetRowHeight(), ELEMENT_TEXTE, treeElementTexte, data, false);


			widthElement = xPos + posElement->GetWidth() + themeTree.GetMargeX();
			yPos += themeTree.GetRowHeight();

			nbRow++;
			if (rowWidth[0] < widthElement)
				rowWidth[0] = widthElement;

			CreateChildTree(it);
		}
		it++;

	}
}


void CFiltreEffect::UpdateChildTree(tree<CTreeData *>::sibling_iterator &parent)
{
	CPositionElement * posElement = nullptr;
	tree<CTreeData *>::sibling_iterator it = tr.begin(parent);
	//tree<CTreeData *>::iterator itend = tr.end(parent);
	bool isVisible = true;
	//int i = 

	for (int i = 0; i < parent.number_of_children(); i++)
	{
		int profondeur = tr.depth(it);
		CTreeDataEffect * data = (CTreeDataEffect *)*it;

		if (data->HasValue() || it.number_of_children() == 0)
		{
			int xPos = widthPosition * profondeur;
			int widthElementColumn1 = 0;
			int widthElementColumn2 = 0;
			CTreeElementSlide * treeElementSlide = nullptr;
			CTreeElementTexte * treeElementTexte = nullptr;
			CTreeElementCheckBox * treeElementCheck = nullptr;
			CTreeElementListBox * treeElementListBox = nullptr;

			posElement = GetElement(data, ELEMENT_TEXTE);
			if (posElement != nullptr)
			{
				CTreeElementTexte * treeElementTexte = (CTreeElementTexte *)posElement->GetTreeElement();
				treeElementTexte->SetVisible(isVisible);
				posElement->SetX(xPos);
				posElement->SetY(yPos);
			}
			else
			{
				treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey());
				treeElementTexte->SetVisible(isVisible);
				posElement = CreatePositionElement(xPos, yPos, nbRow, 0, themeTree.GetRowWidth(), themeTree.GetRowHeight(), ELEMENT_TEXTE, treeElementTexte, data, false);
			}
			widthElementColumn1 = xPos + posElement->GetWidth() + themeTree.GetMargeX();

			if (data->HasValue() > 0)
			{
				xPos = themeTree.GetMargeX();
				if(data->GetType() == TYPE_SLIDE)
					posElement = GetElement(data, ELEMENT_SLIDE);
				else if(data->GetType() == TYPE_CHECKBOX)
					posElement = GetElement(data, ELEMENT_CHECKBOX);
				else if(data->GetType() == TYPE_LISTBOX)
					posElement = GetElement(data, ELEMENT_LISTBOX);

				if (posElement != nullptr)
				{
					if(data->GetType() == TYPE_SLIDE)
					{
						treeElementSlide = (CTreeElementSlide *)posElement->GetTreeElement();
						treeElementSlide->SetVisible(isVisible);
						treeElementSlide->SetElementPos(xPos,yPos);
					}
					else if(data->GetType() == TYPE_CHECKBOX)
					{
						treeElementCheck = (CTreeElementCheckBox *)posElement->GetTreeElement();
						treeElementCheck->SetVisible(isVisible);
						treeElementCheck->SetElementPos(xPos,yPos);
					}
					else if(data->GetType() == TYPE_LISTBOX)
					{
						treeElementListBox = (CTreeElementListBox *)posElement->GetTreeElement();
						treeElementListBox->SetVisible(isVisible);
						treeElementListBox->SetElementPos(xPos,yPos);
					}

					posElement->SetX(xPos);
					posElement->SetY(yPos);
				}
				else if(data->GetType() == TYPE_SLIDE)
				{
					treeElementSlide = CreateSlideElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetInitValue(), data->GetVectorValue(), data->GetExifKey());
					treeElementSlide->SetVisible(isVisible);
					posElement = CreatePositionElement(xPos, yPos, nbRow, 1, themeTree.GetRowWidth(), themeTree.GetRowHeight(), ELEMENT_SLIDE, treeElementSlide, data);
				}
				else if(data->GetType() == TYPE_CHECKBOX)
				{
					treeElementCheck = CreateCheckBoxElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetBoolValue());
					treeElementCheck->SetVisible(isVisible);
					posElement = CreatePositionElement(xPos, yPos, nbRow, 1, themeTree.GetRowWidth(), themeTree.GetRowHeight(), ELEMENT_CHECKBOX, treeElementCheck, data);
				}
				else if(data->GetType() == TYPE_LISTBOX)
				{
					treeElementListBox = CreateListBoxElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetMetadataValue(), data->GetIndex(), data->GetExifKey());
					treeElementListBox->SetVisible(isVisible);
					posElement = CreatePositionElement(xPos, yPos, nbRow, 1, themeTree.GetRowWidth(), themeTree.GetRowHeight(), ELEMENT_LISTBOX, treeElementListBox, data);
				}

				widthElementColumn2 = xPos + posElement->GetWidth() + themeTree.GetMargeX();
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
			int xPos = widthPosition *profondeur;
			int widthElement = 0;
			CTreeElementTexte * treeElementTexte = nullptr;
			CTreeElementTriangle * treeElementTriangle = nullptr;

			posElement = GetElement(data, ELEMENT_TRIANGLE);
			if (posElement != nullptr)
			{
				treeElementTriangle = (CTreeElementTriangle *)posElement->GetTreeElement();
				treeElementTriangle->SetVisible(isVisible);
				posElement->SetX(xPos);
				posElement->SetY(yPos);
			}
			else
			{
				treeElementTriangle = CreateTriangleElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), true);
				treeElementTriangle->SetVisible(isVisible);
				posElement = CreatePositionElement(xPos, yPos, nbRow, 0, themeTree.GetRowWidth(), themeTree.GetRowHeight(), ELEMENT_TRIANGLE, treeElementTriangle, data);

			}

			xPos += posElement->GetWidth() + themeTree.GetMargeX();
			posElement = GetElement(data, ELEMENT_TEXTE);
			if (posElement != nullptr)
			{
				CTreeElementTexte * treeElementTexte = (CTreeElementTexte *)posElement->GetTreeElement();
				treeElementTexte->SetVisible(isVisible);
				posElement->SetX(xPos);
				posElement->SetY(yPos);
			}
			else
			{
				treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey());
				treeElementTexte->SetVisible(isVisible);
				posElement = CreatePositionElement(xPos, yPos, nbRow, 0, themeTree.GetRowWidth(), themeTree.GetRowHeight(), ELEMENT_TEXTE, treeElementTexte, data, false);
			}

			widthElement = xPos + posElement->GetWidth() + themeTree.GetMargeX();
			yPos += themeTree.GetRowHeight();

			nbRow++;
			if (rowWidth[0] < widthElement)
				rowWidth[0] = widthElement;

			bool isOpen = treeElementTriangle->GetOpen();
			if (isOpen)
				UpdateChildTree(it);
		}
		it++;

	}
}

void CFiltreEffect::UpdateElement()
{
	for (CPositionElement * value : vectorPosElement)
	{
		if (value != nullptr)
		{
			value->SetX(0);
			value->SetY(0);
			CTreeElement * treeElement = value->GetTreeElement();
			if (treeElement != nullptr)
			{
				treeElement->SetVisible(false);
				treeElement->SetElementPos(0,0);
			}
		}
	}

	tree<CTreeData *>::sibling_iterator it = tr.begin();
	tree<CTreeData *>::iterator itend = tr.end();
	yPos = 0;
	nbRow = 0;
	bool isVisible = true;

	while (it != itend) {
		CTreeData * data = *it;
		int profondeur = tr.depth(it);
		if (profondeur == 0)
		{
			int xPos = themeTree.GetMargeX();
			int widthElement = 0;
			CTreeElementTexte * treeElementTexte = nullptr;
			CTreeElementTriangle * treeElementTriangle = nullptr;
			CPositionElement * posElement = GetElement(data, ELEMENT_TRIANGLE);
			if (posElement != nullptr)
			{
				treeElementTriangle = (CTreeElementTriangle *)posElement->GetTreeElement();
				treeElementTriangle->SetVisible(isVisible);
				posElement->SetX(xPos);
				posElement->SetY(yPos);
			}
			else
			{
				treeElementTriangle = CreateTriangleElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), true);
				treeElementTriangle->SetVisible(isVisible);
				posElement = CreatePositionElement(xPos, yPos, nbRow, 0, themeTree.GetRowWidth(), themeTree.GetRowHeight(), ELEMENT_TRIANGLE, treeElementTriangle, data);
			}
			widthPosition = xPos + posElement->GetWidth() + themeTree.GetMargeX();
			xPos += posElement->GetWidth() + themeTree.GetMargeX();

			posElement = GetElement(data, ELEMENT_TEXTE);
			if (posElement != nullptr)
			{
				CTreeElementTexte * treeElementTexte = (CTreeElementTexte *)posElement->GetTreeElement();
				treeElementTexte->SetVisible(isVisible);
				posElement->SetX(xPos);
				posElement->SetY(yPos);
			}
			else
			{
				treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey());
				treeElementTexte->SetVisible(isVisible);
				posElement = CreatePositionElement(xPos, yPos, nbRow, 0, themeTree.GetRowWidth(), themeTree.GetRowHeight(), ELEMENT_TEXTE, treeElementTexte, data, false);
			}

			widthElement += xPos + posElement->GetWidth() + themeTree.GetMargeX();
			yPos += themeTree.GetRowHeight();
			nbRow++;
			if (rowWidth[0] < widthElement)
				rowWidth[0] = widthElement;

			bool isOpen = treeElementTriangle->GetOpen();
			if (isOpen)
				UpdateChildTree(it);
		}
		it++;
	}
}


void CFiltreEffect::CreateElement()
{
	tree<CTreeData *>::sibling_iterator it = tr.begin();
	tree<CTreeData *>::iterator itend = tr.end();
	yPos = 0;
	nbRow = 0;
	vectorPosElement.clear();
	vectorPosElementDynamic.clear();
	bool isVisible = true;

	while (it != itend) {
		CTreeData * data = *it;
		int profondeur = tr.depth(it);
		if (profondeur == 0)
		{
			int xPos = themeTree.GetMargeX();
			int widthElement = 0;
			CTreeElementTexte * treeElementTexte = nullptr;
			CTreeElementTriangle * treeElementTriangle = nullptr;
			CPositionElement * posElement = nullptr;
			treeElementTriangle = CreateTriangleElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), true);
			treeElementTriangle->SetVisible(isVisible);
			posElement = CreatePositionElement(xPos, yPos, nbRow, 0, themeTree.GetRowWidth(), themeTree.GetRowHeight(), ELEMENT_TRIANGLE, treeElementTriangle, data);
			widthPosition = xPos + posElement->GetWidth() + themeTree.GetMargeX();
			xPos += posElement->GetWidth() + themeTree.GetMargeX();
			
			treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey());
			treeElementTexte->SetVisible(isVisible);
			posElement = CreatePositionElement(xPos, yPos, nbRow, 0, themeTree.GetRowWidth(), themeTree.GetRowHeight(), ELEMENT_TEXTE, treeElementTexte, data, false);


			widthElement += xPos + posElement->GetWidth() + themeTree.GetMargeX();
			yPos += themeTree.GetRowHeight();
			nbRow++;
			if (rowWidth[0] < widthElement)
				rowWidth[0] = widthElement;

			CreateChildTree(it);
		}
		it++;
	}
}

void CFiltreEffect::UnclickOnElement(CPositionElement * element, wxWindow * window, const int &x, const int &y, const int& posLargeur, const int &posHauteur)
{
	CTreeElement * treeElement = element->GetTreeElement();
	if (element->GetType() == ELEMENT_SLIDE)
	{
		CTreeElementSlide * treeElementSlide = (CTreeElementSlide *)treeElement;

		int xPos = 0;
		if (treeElementSlide->GetRow() > 0)
			xPos = GetWidthRow(treeElementSlide->GetRow() - 1);

		treeElementSlide->UnclickElement(window, (x + posLargeur) - (element->GetX() + xPos), (y + posHauteur) - element->GetY());
	}

}

void CFiltreEffect::ClickOnElement(CPositionElement * element, wxWindow * window, const int &x, const int &y, const int& posLargeur, const int &posHauteur)
{
	bool update = false;
	CTreeElement * treeElement = element->GetTreeElement();
	if (element->GetType() == ELEMENT_SLIDE)
	{
		CTreeElementSlide * treeElementSlide = (CTreeElementSlide *)treeElement;

		int xPos = 0;
		if (treeElementSlide->GetRow() > 0)
			xPos = GetWidthRow(treeElementSlide->GetRow() - 1);

		//treeElementSlide->SetElementPos((x - xPos) - posLargeur, y - posHauteur);
		treeElementSlide->ClickElement(window, (x + posLargeur) - (element->GetX() + xPos), (y + posHauteur) - element->GetY());

		update = true;

	}
	else if(element->GetType() == ELEMENT_TRIANGLE)
	{
		CTreeElementTriangle * treeElementTriangle = (CTreeElementTriangle *)treeElement;
		treeElementTriangle->ClickElement(window, (x + posLargeur) - element->GetX(), (y + posHauteur) - element->GetY());
		update = true;
	}
	else if(element->GetType() == ELEMENT_CHECKBOX)		
	{
		CTreeElementCheckBox * treeElementCheckBox = (CTreeElementCheckBox *)treeElement;
		treeElementCheckBox->ClickElement(window, (x + posLargeur) - element->GetX(), (y + posHauteur) - element->GetY());

		if(filterEffect != nullptr)
		{
			CTreeDataEffect * data = (CTreeDataEffect *)element->GetTreeData();
			CTreeElementValueBool element(treeElementCheckBox->GetCheckState());
			filterEffect->FilterChangeParam(effectParameter, &element, data->GetExifKey());
		}

		update = true;

		if (bitmapViewer != nullptr)
			bitmapViewer->UpdateFiltre(effectParameter);
	}
	else if (element->GetType() == ELEMENT_LISTBOX)
	{
		CTreeElementListBox * treeElementListbox = (CTreeElementListBox *)treeElement;

		int xPos = 0;
		if (treeElementListbox->GetRow() > 0)
			xPos = GetWidthRow(treeElementListbox->GetRow() - 1);

		//treeElementSlide->SetElementPos((x - xPos) - posLargeur, y - posHauteur);
		treeElementListbox->ClickElement(window, (x + posLargeur) - (element->GetX() + xPos), (y + posHauteur) - element->GetY());


		update = true;
	}

	if(update)
	{
		UpdateElement();
		eventControl->UpdateTreeControl();
	}
}

void CFiltreEffect::MouseOver(wxDC * deviceContext, CPositionElement * element, const int &x, const int &y, const int& posLargeur, const int &posHauteur, bool & update)
{
	CTreeElement * treeElement = element->GetTreeElement();
	if (element->GetType() == ELEMENT_SLIDE)
	{
		CTreeElementSlide * treeElementSlide = (CTreeElementSlide *)treeElement;

		int xPos = 0;
		if (treeElementSlide->GetRow() > 0)
			xPos = GetWidthRow(treeElementSlide->GetRow() - 1);

		//
		if (treeElementSlide->MouseBlock())
		{
			treeElementSlide->MouseOver(deviceContext, (x + posLargeur) - (element->GetX() + xPos), (y + posHauteur) - element->GetY(), update);
			treeElementSlide->DrawElement(deviceContext, (element->GetX() + xPos) - posLargeur, element->GetY() - posHauteur);
		}
			
	}
}
