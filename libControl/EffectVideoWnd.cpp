#include "EffectVideoWnd.h"
#include <ConvertUtility.h>
#include <TreeDataEffect.h>
#include <ParamInit.h>
#include <ConfigParam.h>
#include <libResource.h>

using namespace Regards::Control;

#define TAILLEMAX 1024
#define SLIDECOMPONENT 1
#define CHECKBOXCOMPONENT 2

CEffectVideoWnd::CEffectVideoWnd(CVideoEffectParameter * videoEffectParameter, CThemeTree * theme, CTreeElementControlInterface * interfaceControl)
{
	this->videoEffectParameter = videoEffectParameter;
	rowWidth.push_back(0);
	rowWidth.push_back(0);
	themeTree = *theme;
	themeTree.themeTriangle.SetHeight(themeTree.GetRowHeight());
	themeTree.themeCheckbox.SetHeight(themeTree.GetRowHeight());
	themeTree.themeDelete.SetHeight(themeTree.GetRowHeight());
	themeTree.themeSlide.SetHeight(themeTree.GetRowHeight());
	themeTree.themeTexte.SetHeight(themeTree.GetRowHeight());
	eventControl = interfaceControl;
	effectMedianLevel = CLibResource::LoadStringFromResource("LBLEFFECTMEDIANLEVE",1);
	effectSharpenLevel = CLibResource::LoadStringFromResource("LBLEFFECTSHARPENLEVE", 1);
	toneConstrastLevel = CLibResource::LoadStringFromResource("LBLTONECONTRASTLEVE", 1);
	toneExposureLevel = CLibResource::LoadStringFromResource("LBLTONEEXPOSURELEVE", 1);
	toneBrightnessLevel = CLibResource::LoadStringFromResource("LBLTONEBRIGHTNESSLEVE", 1);
	colorBoostRed = CLibResource::LoadStringFromResource("LBLCOLORBOOSTRED", 1);
	colorBoostGreen = CLibResource::LoadStringFromResource("LBLCOLORBOOSTGREEN", 1);
	colorBoostBlue = CLibResource::LoadStringFromResource("LBLCOLORBOOSTBLUE", 1);
}

CEffectVideoWnd::~CEffectVideoWnd(void)
{
}

void CEffectVideoWnd::Init()
{
	int index = 0;
	tree<CTreeData *>::iterator top;
	tree<CTreeData *>::iterator child;

	top = tr.begin();
	child = top;
	//vector<int> element = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };


	vector<int> elementColor;
	for (int i = 0; i < 256; i++)
		elementColor.push_back(i);

	vector<int> element10;
	for (int i = 0; i < 11; i++)
		element10.push_back(i);

	vector<int> element100;
	for (int i = 0; i < 101; i++)
		element100.push_back(i);

	vector<int> element1000;
	for (int i = 0; i < 1001; i++)
		element1000.push_back(i);

	AddTreeInfos(effectMedianLevel, 100, element1000, index++, top, child, SLIDECOMPONENT);
	AddTreeInfos(effectSharpenLevel, 150, element1000, index++, top, child, SLIDECOMPONENT);

	AddTreeInfos(toneConstrastLevel, 50, element100, index++, top, child, SLIDECOMPONENT);
	AddTreeInfos(toneExposureLevel, 50, element100, index++, top, child, SLIDECOMPONENT);
	AddTreeInfos(toneBrightnessLevel, 25, element100, index++, top, child, SLIDECOMPONENT);

	AddTreeInfos(colorBoostRed, 0, elementColor, index++, top, child, SLIDECOMPONENT);
	AddTreeInfos(colorBoostGreen, 0, elementColor, index++, top, child, SLIDECOMPONENT);
	AddTreeInfos(colorBoostBlue, 0, elementColor, index++, top, child, SLIDECOMPONENT);

	CreateElement();
}


void CEffectVideoWnd::AddTreeInfos(const wxString &exifKey, const int &position,const vector<int> & value, const int &index, tree<CTreeData *>::iterator &top, tree<CTreeData *>::iterator &child, const int &type)
{
	wchar_t seps[] = L".";
	int item = 0;
	wchar_t informations[TAILLEMAX];
	wchar_t *next_token1 = nullptr;

	wcscpy(informations, exifKey.c_str());

#ifdef WIN32
	wchar_t * token = wcstok(informations, seps); // C4996
#else
	// Establish string and get the first token:
	wchar_t * token = wcstok(informations, seps, &next_token1); // C4996
#endif
	// Note: strtok is deprecated; consider using strtok_s instead
	while (token != nullptr)
	{
		CTreeDataEffect * treeData = new CTreeDataEffect();
		treeData->SetKey(token);
#ifdef WIN32
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
			treeData->SetPosition(position);
			treeData->SetValue(value);
			treeData->SetExifKey(exifKey);
			treeData->SetType(type);
			tr.append_child(child, treeData);
		}
		item++;
	}
}

void CEffectVideoWnd::SlidePosChange(CTreeElement * treeElement, const int &position, const int &valueData, const wxString &key)
{
	float value = valueData;
	//Video Parameter
	if (key == effectMedianLevel)
	{
		if (value == 0)
			videoEffectParameter->MedianEnable = 0;
		else
			videoEffectParameter->MedianEnable = 1;
		videoEffectParameter->median = value / 100.0f;
	}
	else if (key == effectSharpenLevel)
	{
		if (value == 0)
			videoEffectParameter->SharpenEnable = 0;
		else
			videoEffectParameter->SharpenEnable = 1;

		videoEffectParameter->sharpness = value / 100.0f;
	}
	else if (key == toneConstrastLevel)
	{
		//Tone mapping
		videoEffectParameter->contrast = 1.0 + value / 100.0f;
	}
	else if (key == toneExposureLevel)
	{
		videoEffectParameter->exposure = 1.0 + value / 100.0f;
	}
	else if (key == toneBrightnessLevel)
	{
		videoEffectParameter->brightness = 2 + value / 100.0f;
	}
	else if (key == colorBoostRed)
	{
		//Color Boost
		videoEffectParameter->color_boost[0] = 1.0 + value / 255.0f;;
	}
	else if (key == colorBoostGreen)
	{
		videoEffectParameter->color_boost[1] = 1.0 + value / 255.0f;;

	}
	else if (key == colorBoostBlue)
	{
		videoEffectParameter->color_boost[2] = 1.0 + value / 255.0f;;
	}
}

void CEffectVideoWnd::CreateChildTree(tree<CTreeData *>::sibling_iterator &parent)
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
			posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTexte->GetWidth(), treeElementTexte->GetHeight(), ELEMENT_TEXTE, treeElementTexte, data, false);

			widthElementColumn1 = xPos + posElement->GetWidth() + themeTree.GetMargeX();
			if (data->GetValue().size() > 0)
			{
				if (data->GetType() == SLIDECOMPONENT)
				{
					xPos = themeTree.GetMargeX();
					treeElementSlide = CreateSlideElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetPosition(), data->GetValue(), data->GetExifKey());
					treeElementSlide->SetVisible(isVisible);
					posElement = CreatePositionElement(xPos, yPos, nbRow, 1, treeElementSlide->GetWidth(), treeElementSlide->GetHeight(), ELEMENT_SLIDE, treeElementSlide, data);
					widthElementColumn2 = xPos + posElement->GetWidth() + themeTree.GetMargeX();
				}
			}

			if (isVisible)
				yPos += themeTree.GetRowHeight();

			nbRow++;
			if (rowWidth[0] < widthElementColumn1)
				rowWidth[0] = widthElementColumn1;

			if (rowWidth[1] < widthElementColumn2)
				rowWidth[1] = widthElementColumn2;
		}
		else
		{
			int widthElement = 0;
			int xPos = widthPosition *profondeur;
			CTreeElementTexte * treeElementTexte = nullptr;
			CTreeElementTriangle * treeElementTriangle = nullptr;
			treeElementTriangle = CreateTriangleElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), true);
			treeElementTriangle->SetVisible(isVisible);
			posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTriangle->GetWidth(), treeElementTriangle->GetHeight(), ELEMENT_TRIANGLE, treeElementTriangle, data);

			xPos += posElement->GetWidth() + themeTree.GetMargeX();
			treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey());
			treeElementTexte->SetVisible(isVisible);
			posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTexte->GetWidth(), treeElementTexte->GetHeight(), ELEMENT_TEXTE, treeElementTexte, data, false);

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

void CEffectVideoWnd::CreateElement()
{
	bool isVisible = true;

	tree<CTreeData *>::sibling_iterator it = tr.begin();
	tree<CTreeData *>::iterator itend = tr.end();
	yPos = 0;
	nbRow = 0;

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
			posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTriangle->GetWidth(), treeElementTriangle->GetHeight(), ELEMENT_TRIANGLE, treeElementTriangle, data);
			widthPosition = xPos + posElement->GetWidth() + themeTree.GetMargeX();
			xPos += posElement->GetWidth() + themeTree.GetMargeX();
			treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey());
			treeElementTexte->SetVisible(isVisible);
			posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTexte->GetWidth(), treeElementTexte->GetHeight(), ELEMENT_TEXTE, treeElementTexte, data, false);

			widthElement += xPos + posElement->GetWidth() + themeTree.GetMargeX();

			xPos = themeTree.GetMargeX();
			CTreeElementCheckBox * treeElementCheckBox = CreateCheckBoxElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), false);
			treeElementCheckBox->SetVisible(isVisible);
			CreatePositionElement(xPos, yPos, nbRow, 1, treeElementCheckBox->GetWidth(), treeElementCheckBox->GetHeight(), ELEMENT_CHECKBOX, treeElementCheckBox, data);

			yPos += themeTree.GetRowHeight();
			nbRow++;

			if (rowWidth[0] < widthElement)
				rowWidth[0] = widthElement;

			CreateChildTree(it);
		}
		it++;
	}
}


void CEffectVideoWnd::UpdateChildTree(tree<CTreeData *>::sibling_iterator &parent)
{
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
			int xPos = themeTree.GetMargeX() * (profondeur + 1);
			int widthElementColumn1 = 0;
			int widthElementColumn2 = 0;
			CTreeElementSlide * treeElementSlide = nullptr;
			//CTreeElementTexte * treeElementTexte = nullptr;
			CPositionElement * posElement = GetElement(data, ELEMENT_TEXTE);
			if (posElement != nullptr)
			{
				CTreeElementTexte * treeElementTexte = (CTreeElementTexte *)posElement->GetTreeElement();
				treeElementTexte->SetVisible(isVisible);
				posElement->SetX(xPos);
				posElement->SetY(yPos);
			
                widthElementColumn1 = xPos + posElement->GetWidth() + themeTree.GetMargeX();
            }
            
            if (data->GetValue().size() > 0)
            {
                xPos = themeTree.GetMargeX();
                if (data->GetType() == SLIDECOMPONENT)
                    posElement = GetElement(data, ELEMENT_SLIDE);

                    //CTreeElement * treeElement = posElement->GetTreeElement();
                if(treeElementSlide != nullptr)
                    treeElementSlide->SetVisible(isVisible);
                
                if(posElement != nullptr)
                {
                    posElement->SetX(xPos);
                    posElement->SetY(yPos);
                    widthElementColumn2 = xPos + posElement->GetWidth() + themeTree.GetMargeX();
                }
            }

			if (isVisible)
				yPos += themeTree.GetRowHeight();

			nbRow++;
			if (rowWidth[0] < widthElementColumn1)
				rowWidth[0] = widthElementColumn1;

			if (rowWidth[1] < widthElementColumn2)
				rowWidth[1] = widthElementColumn2;
		}
		else
		{
			int widthElement = 0;
			int xPos = themeTree.GetMargeX() *profondeur;
			CTreeElementTexte * treeElementTexte = nullptr;
			CTreeElementTriangle * treeElementTriangle = nullptr;
			CPositionElement * posElement = GetElement(data, ELEMENT_TRIANGLE);
			if (posElement != nullptr)
			{
				treeElementTriangle = (CTreeElementTriangle *)posElement->GetTreeElement();
				treeElementTriangle->SetVisible(isVisible);
				posElement->SetX(xPos);
				posElement->SetY(yPos);
                xPos += posElement->GetWidth() + themeTree.GetMargeX();
			}
            
           
            posElement = GetElement(data, ELEMENT_TEXTE);
            if (posElement != nullptr)
            {
				treeElementTexte = (CTreeElementTexte *)posElement->GetTreeElement();
				treeElementTexte->SetVisible(isVisible);
				posElement->SetX(xPos);
				posElement->SetY(yPos);
                widthElement = xPos + posElement->GetWidth() + themeTree.GetMargeX();
			}

			
			yPos += themeTree.GetRowHeight();

			nbRow++;
			if (rowWidth[0] < widthElement)
				rowWidth[0] = widthElement;


            if(treeElementTriangle != nullptr)
            {
                bool isShow = treeElementTriangle->GetOpen();
                if (isShow)
                    CreateChildTree(it);
            }
		}
		it++;

	}
}

void CEffectVideoWnd::UpdateElement()
{
	for (PositionElementVector::iterator fit = vectorPosElement.begin(); fit != vectorPosElement.end(); fit++)
	{
		CPositionElement * value = *fit;
		CTreeElement * treeElement = value->GetTreeElement();
		treeElement->SetVisible(false);
	}

	bool isVisible = true;

	tree<CTreeData *>::sibling_iterator it = tr.begin();
	tree<CTreeData *>::iterator itend = tr.end();
	yPos = 0;
	nbRow = 0;

	while (it != itend) {
		CTreeData * data = *it;
		int profondeur = tr.depth(it);
		if (profondeur == 0)
		{
			int xPos = themeTree.GetMargeX();
			int widthElement = 0;
			//CTreeElementTexte * treeElementTexte = nullptr;
			CTreeElementTriangle * treeElementTriangle = nullptr;
			CPositionElement * posElement = GetElement(data, ELEMENT_TRIANGLE);
			if (posElement != nullptr)
			{
				treeElementTriangle = (CTreeElementTriangle *)posElement->GetTreeElement();
				treeElementTriangle->SetVisible(isVisible);
				posElement->SetX(xPos);
				posElement->SetY(yPos);
                xPos += posElement->GetWidth() + themeTree.GetMargeX();
			}
			
			posElement = GetElement(data, ELEMENT_TEXTE);
			if (posElement != nullptr)
			{
				CTreeElementTexte * treeElementTexte = (CTreeElementTexte *)posElement->GetTreeElement();
				treeElementTexte->SetVisible(isVisible);
				posElement->SetX(xPos);
				posElement->SetY(yPos);
                widthElement += xPos + posElement->GetWidth() + themeTree.GetMargeX();
			}

			
			yPos += themeTree.GetRowHeight();
			nbRow++;
			if (rowWidth[0] < widthElement)
				rowWidth[0] = widthElement;

            if(treeElementTriangle != nullptr)
            {
                bool isOpen = treeElementTriangle->GetOpen();
                if (isOpen)
                    CreateChildTree(it);
            }
		}
		it++;
	}
}

void CEffectVideoWnd::UnclickOnElement(CPositionElement * element, wxWindow * window, const int &x, const int &y, const int& posLargeur, const int &posHauteur)
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

void CEffectVideoWnd::ClickOnElement(CPositionElement * element, wxWindow * window, const int &x, const int &y, const int& posLargeur, const int &posHauteur)
{
	CTreeElement * treeElement = element->GetTreeElement();
	if (element->GetType() == ELEMENT_SLIDE)
	{
		CTreeElementSlide * treeElementSlide = (CTreeElementSlide *)treeElement;

		int xPos = 0;
		if (treeElementSlide->GetRow() > 0)
			xPos = GetWidthRow(treeElementSlide->GetRow() - 1);

		treeElementSlide->ClickElement(window, (x + posLargeur) - (element->GetX() + xPos), (y + posHauteur) - element->GetY());
	}
	else if(element->GetType() == ELEMENT_TRIANGLE)
	{
		CTreeElementTriangle * treeElementTriangle = (CTreeElementTriangle *)treeElement;
		treeElementTriangle->ClickElement(window, (x + posLargeur) - element->GetX(), (y + posHauteur) - element->GetY());
		UpdateElement();
		eventControl->Update();
	}
	else if (element->GetType() == ELEMENT_CHECKBOX)
	{
		CTreeData * treeData = element->GetTreeData();
		CTreeElementCheckBox * treeElementCheckBox = (CTreeElementCheckBox *)treeElement;
		treeElementCheckBox->ClickElement(window, (x + posLargeur) - element->GetX(), (y + posHauteur) - element->GetY());

		if (treeData->GetKey() == "Effect")
		{
			videoEffectParameter->effectEnable = treeElementCheckBox->GetCheckState();
		}
		else if (treeData->GetKey() == "Tone")
		{
			videoEffectParameter->ToneEnable = treeElementCheckBox->GetCheckState();
		}
		else if (treeData->GetKey() == "Color Boost")
		{
			videoEffectParameter->ColorBoostEnable = treeElementCheckBox->GetCheckState();
		}

		int xPos = 0;
		if (treeElementCheckBox->GetRow() > 0)
			xPos = GetWidthRow(treeElementCheckBox->GetRow() - 1);


		wxWindowDC winDC(window);
		treeElementCheckBox->DrawElement(&winDC, (element->GetX() + xPos) - posLargeur, element->GetY() - posHauteur);
	}

}

void CEffectVideoWnd::MouseOver(wxDC * deviceContext, CPositionElement * element, const int &x, const int &y, const int& posLargeur, const int &posHauteur)
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