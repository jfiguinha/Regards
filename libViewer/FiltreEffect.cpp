#include "FiltreEffect.h"
#include <effect.h>
#include <ConvertUtility.h>
#include <TreeDataEffect.h>
#include "LensFlareFilter.h"
#include "SwirlFilter.h"
#include "ColorBalanceFilter.h"
#include "CloudsFilter.h"
#include "RotateFreeFilter.h"
#include "SolarisationFilter.h"
#include "PosterisationFilter.h"
#include "PhotoFiltreFilter.h"
#include "BrightAndContrastFilter.h"
#include "MotionBlurFilter.h"
#include <BitmapWndViewer.h>
#include <libResource.h>
#include <RegardsBitmap.h>
#include <FilterData.h>
#define TAILLEMAX 1024
using namespace Regards::Viewer;

CFiltreEffect::CFiltreEffect(CBitmapWndViewer * bitmapViewer, CThemeTree * theme, CTreeElementControlInterface * interfaceControl)
{
	this->bitmapViewer = bitmapViewer;
	themeTree = *theme;
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

void CFiltreEffect::AddTreeInfos(const wxString &exifKey, const int &position, const vector<int> & value)
{
    AddTreeInfos(exifKey, position, value, index++, top, child);
}

void CFiltreEffect::Init(CEffectParameter * effectParameter, CRegardsBitmap * source, const int &filtre)
{
	this->filtre = filtre;
	this->effectParameter = effectParameter;
	this->source = source;
    
    index = 0;    
    top = tr.begin();
    child = top;
    
    if(filterEffect != nullptr)
        delete filterEffect;

	switch (filtre)
	{
	case IDM_FILTRE_MOTIONBLUR:
        filterEffect = new CMotionBlurFilter();
        filterEffect->Filter(effectParameter, source, this);
		break;

	case IDM_FILTRE_CLOUDS:
        filterEffect = new CCloudsFilter();
        filterEffect->Filter(effectParameter, source, this);
		break;

	case IDM_FILTRE_SWIRL:
        filterEffect = new CSwirlFilter();
        filterEffect->Filter(effectParameter, source, this);
		break;

	case IDM_COLOR_BALANCE:
        filterEffect = new CColorBalanceFilter();
        filterEffect->Filter(effectParameter, source, this);
		break;

	case ID_AJUSTEMENT_POSTERISATION:
        filterEffect = new CPosterisationFilter();
        filterEffect->Filter(effectParameter, source, this);
		break;

	case ID_AJUSTEMENT_PHOTOFILTRE:
        filterEffect = new CPhotoFiltreFilter();
        filterEffect->Filter(effectParameter, source, this);
		break;

	case IDM_IMAGE_LIGHTCONTRAST:
        filterEffect = new CBrightAndContrastFilter();
        filterEffect->Filter(effectParameter, source, this);
		break;

	case IDM_AJUSTEMENT_SOLARISATION:
        filterEffect = new CSolarisationFilter();
        filterEffect->Filter(effectParameter, source, this);
		break;

	case IDM_FILTRELENSFLARE:
        filterEffect = new CLensFlareFilter();
        filterEffect->Filter(effectParameter, source, this);
		break;

	case IDM_ROTATE_FREE:
        filterEffect = new CRotateFreeFilter();
        filterEffect->Filter(effectParameter, source, this);
		break;

	}

	CreateElement();
}


void CFiltreEffect::UpdateScreenRatio()
{
    UpdateElement();
}


void CFiltreEffect::SlidePosChange(CTreeElement * treeElement, const int &position, const int &valueData, const wxString &key)
{
    if(filterEffect != nullptr)
        filterEffect->FilterChangeParam(effectParameter, valueData, key);
    
	if (bitmapViewer != nullptr)
		bitmapViewer->Refresh();

	eventControl->UpdateElement(treeElement);
}


void CFiltreEffect::AddTreeInfos(const wxString &exifKey, const int &position,const vector<int> & value, const int &index, tree<CTreeData *>::iterator &top, tree<CTreeData *>::iterator &child)
{
	wchar_t seps[] = L".";
	int item = 0;
	wchar_t informations[TAILLEMAX];
	wchar_t *next_token1 = nullptr;

	wcscpy(informations, exifKey.c_str());

	// Establish string and get the first token:
#ifdef WIN32
#if _MSC_VER < 1900
	wchar_t * token = wcstok(informations, seps ); // C4996
#else
	wchar_t * token = wcstok(informations, seps, &next_token1); // C4996
#endif
#else
    wchar_t * token = wcstok(informations, seps, &next_token1); // C4996
#endif
	// Note: strtok is deprecated; consider using strtok_s instead
	while (token != nullptr)
	{
		CTreeDataEffect * treeData = new CTreeDataEffect();
		treeData->SetKey(token);
#ifdef WIN32
#if _MSC_VER < 1900
		token = wcstok(nullptr, seps); // C4996
#else
		token = wcstok(nullptr, seps, &next_token1); // C4996
#endif
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
			treeData->SetPosition(position);
			treeData->SetValue(value);
			treeData->SetExifKey(exifKey);
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

		if (data->GetValue().size() > 0 || it.number_of_children() == 0)
		{
			int xPos = widthPosition * profondeur;
			int widthElementColumn1 = 0;
			int widthElementColumn2 = 0;
			CTreeElementSlide * treeElementSlide = nullptr;
			CTreeElementTexte * treeElementTexte = nullptr;
			treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey());
			treeElementTexte->SetVisible(isVisible);
			posElement = CreatePositionElement(xPos, yPos, nbRow, 0, themeTree.GetRowWidth(), themeTree.GetRowHeight(), ELEMENT_TEXTE, treeElementTexte, data, false);

			widthElementColumn1 = xPos + posElement->GetWidth() + themeTree.GetMargeX();

			if (data->GetValue().size() > 0)
			{
				xPos = themeTree.GetMargeX();
				treeElementSlide = CreateSlideElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetPosition(), data->GetValue(), data->GetExifKey());
				treeElementSlide->SetVisible(isVisible);
				posElement = CreatePositionElement(xPos, yPos, nbRow, 1, themeTree.GetRowWidth(), themeTree.GetRowHeight(), ELEMENT_SLIDE, treeElementSlide, data);

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

		if (data->GetValue().size() > 0 || it.number_of_children() == 0)
		{
			int xPos = widthPosition * profondeur;
			int widthElementColumn1 = 0;
			int widthElementColumn2 = 0;
			CTreeElementSlide * treeElementSlide = nullptr;
			CTreeElementTexte * treeElementTexte = nullptr;
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

			if (data->GetValue().size() > 0)
			{
				xPos = themeTree.GetMargeX();
				posElement = GetElement(data, ELEMENT_SLIDE);
				if (posElement != nullptr)
				{
					CTreeElementSlide * treeElementSlide = (CTreeElementSlide *)posElement->GetTreeElement();
					treeElementSlide->SetVisible(isVisible);
					posElement->SetX(xPos);
					posElement->SetY(yPos);
				}
				else
				{
					treeElementSlide = CreateSlideElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetPosition(), data->GetValue(), data->GetExifKey());
					treeElementSlide->SetVisible(isVisible);
					posElement = CreatePositionElement(xPos, yPos, nbRow, 1, themeTree.GetRowWidth(), themeTree.GetRowHeight(), ELEMENT_SLIDE, treeElementSlide, data);
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
			CTreeElement * treeElement = value->GetTreeElement();
			if (treeElement != nullptr)
				treeElement->SetVisible(false);
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
	CTreeElement * treeElement = element->GetTreeElement();
	if (element->GetType() == ELEMENT_SLIDE)
	{
		CTreeElementSlide * treeElementSlide = (CTreeElementSlide *)treeElement;

		int xPos = 0;
		if (treeElementSlide->GetRow() > 0)
			xPos = GetWidthRow(treeElementSlide->GetRow() - 1);

		//treeElementSlide->SetElementPos((x - xPos) - posLargeur, y - posHauteur);
		treeElementSlide->ClickElement(window, (x + posLargeur) - (element->GetX() + xPos), (y + posHauteur) - element->GetY());

	}
	else if(element->GetType() == ELEMENT_TRIANGLE)
	{
		CTreeElementTriangle * treeElementTriangle = (CTreeElementTriangle *)treeElement;
		treeElementTriangle->ClickElement(window, (x + posLargeur) - element->GetX(), (y + posHauteur) - element->GetY());
		UpdateElement();
		eventControl->Update();
	}

}

void CFiltreEffect::MouseOver(wxDC * deviceContext, CPositionElement * element, const int &x, const int &y, const int& posLargeur, const int &posHauteur)
{
	CTreeElement * treeElement = element->GetTreeElement();
	if (element->GetType() == ELEMENT_SLIDE)
	{
		CTreeElementSlide * treeElementSlide = (CTreeElementSlide *)treeElement;

		int xPos = 0;
		if (treeElementSlide->GetRow() > 0)
			xPos = GetWidthRow(treeElementSlide->GetRow() - 1);

		treeElementSlide->MouseOver(deviceContext, (x + posLargeur) - (element->GetX() + xPos), (y + posHauteur) - element->GetY());
		if (treeElementSlide->MouseBlock())
			treeElementSlide->DrawElement(deviceContext, (element->GetX() + xPos) - posLargeur, element->GetY() - posHauteur);
	}
}