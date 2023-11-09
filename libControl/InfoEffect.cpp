// ReSharper disable All
#include <header.h>
#include "InfoEffect.h"
#include "TreeElementTexte.h"
#include "TreeElementTriangle.h"
#include "ModificationManager.h"
#include <ImageLoadingFormat.h>
#include "MainTheme.h"
#include "MainThemeInit.h"
#include "BitmapWnd3d.h"
#include "BitmapWndViewer.h"
#include <TreeData.h>
#include <TreeElementControlInterface.h>
#include <ConvertUtility.h>
using namespace Regards::Window;
using namespace Regards::Control;


CInfoEffect::CInfoEffect(CTreeElementControlInterface* interfaceControl, CModificationManager* modificationManager,
                         int bitmapWindowId): numEvent(0), yPos(0), index(0)
{
	this->bitmapWindowId = bitmapWindowId;
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
	this->modificationManager = modificationManager;
	rowWidth.push_back(0);
}
;


CInfoEffect::~CInfoEffect()
{
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
void CInfoEffect::AddModification(CImageLoadingFormat* bitmap, const wxString& libelle)
{
	const int numModification = modificationManager->GetNumModification();
	const int nbModification = modificationManager->GetNbModification();

	if (numModification < nbModification)
	{
		ClearData();

		InitTree(modificationManager->GetModificationLibelle(0), to_string(0));

		for (int i = 1; i < (numModification + 1); i++)
		{
			wxString modificationString = modificationManager->GetModificationLibelle(i);
			modificationString.Replace(".", "@99");
			AddEvent("Effect." + modificationString, to_string(i));
			index++;
		}
	}

	wxString localLibelle = libelle;
	localLibelle.Replace(".", "@99");
	modificationManager->AddModification(bitmap, libelle);
	AddEvent("Effect." + localLibelle, GetNumModification());
	index++;
	SetActifElement(GetNumModification());
}

void CInfoEffect::InitTree(const wxString& libelle, const wxString& key)
{
	filename = libelle;
	numEvent = 1;
	index = 0;
	//Récupération des catégories principales

	//child = childStart;
	wxString localLibelle = libelle;
	localLibelle.Replace(".", "@99");
	AddEvent("Source." + localLibelle, key);
	//childStart = child;
	index++;
	CreateElement();
}

wxString CInfoEffect::GetNumModification()
{
	return to_string(modificationManager->GetNumModification());
}

void CInfoEffect::ClickOnElement(CPositionElement* element, wxWindow* window, const int& x, const int& y,
                                 const int& posLargeur, const int& posHauteur)
{
	CTreeElement* treeElement = element->GetTreeElement();
	auto treeData = element->GetTreeData();
	if (element->GetType() == ELEMENT_TEXTE)
	{
		CBitmapWndViewer* bitmapViewer = nullptr;
		auto bitmapWindow = dynamic_cast<IBitmapWnd*>(wxWindow::FindWindowById(bitmapWindowId));
		if (bitmapWindow != nullptr)
		{
			bitmapViewer = (CBitmapWndViewer*)bitmapWindow->GetWndPt();
		}
		if (bitmapViewer != nullptr)
		{
			wxString key = treeData->GetExifKey();
			if (key != "-1")
			{
				const int modif = atoi(CConvertUtility::ConvertToUTF8(key));
				bitmapViewer->SetBitmap(modificationManager->GetModification(modif));
				//bitmapViewer->Refresh();
				SetActifElement(key);
			}
		}
	}
	else if (element->GetType() == ELEMENT_TRIANGLE)
	{
		auto treeElementTriangle = dynamic_cast<CTreeElementTriangle*>(treeElement);
		treeElementTriangle->ClickElement(window, (x + posLargeur) - element->GetX(),
		                                  (y + posHauteur) - element->GetY());
		UpdateElement();
		eventControl->UpdateTreeControl();
	}
}

void CInfoEffect::MouseOver(CPositionElement* element, const int& x, const int& y,
                            const int& posLargeur, const int& posHauteur, bool& update)
{
	int xPos = 0;
	if (element->GetRow() > 0)
		xPos = GetWidthRow(element->GetRow() - 1);

	element->GetTreeElement()->MouseOver(element->GetX() + xPos, element->GetY(), update);
}

wxString CInfoEffect::GetFilename()
{
	return filename;
}

void CInfoEffect::Init(CImageLoadingFormat* bitmap, const wxString& libelle, const wxString& key)
{
	modificationManager->Init(bitmap);
	InitTree(libelle, key);
}

void CInfoEffect::SetActifElement(const wxString& key)
{
	for (auto posElement : vectorPosElement)
	{
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

void CInfoEffect::AddEvent(const wxString& libelle, const wxString& key)
{
	//Récupération des catégories principales
	numEvent += 2;
	wxString localLibelle = libelle;
	localLibelle.Replace("@99", ".");
	auto treeData = new CTreeData();
	treeData->SetIsParent(true);
	treeData->SetKey(localLibelle);
	treeData->SetExifKey(key);

	//FindKey(const wxString & key)
	//child = tr.insert_after(child, treeData);

	wchar_t seps[] = L".";
	int item = 0;
	wchar_t* next_token1 = nullptr;
	wchar_t informations[1024];
	wcscpy(informations, libelle.c_str());

	// Establish string and get the first token:
#if defined(WIN32) && _MSC_VER < 1900
	wchar_t * token = wcstok(informations, seps); // C4996
#else
	wchar_t* token = wcstok(informations, seps, &next_token1); // C4996
#endif

	// Note: strtok is deprecated; consider using strtok_s instead
	while (token != nullptr)
	{
		auto tree_data = new CTreeData();
		wxString value = token;
		value.Replace("@99", ".");
		tree_data->SetKey(value);
#if defined(WIN32) && _MSC_VER < 1900
		token = wcstok(nullptr, seps); // C4996
#else
		token = wcstok(nullptr, seps, &next_token1); // C4996
#endif

		if (token != nullptr)
		{
			tree_data->SetIsParent(true);

			if (index > 0)
			{
				if (item == 0)
				{
					tree<CTreeData*>::iterator it;
					//Recherche de la clé
					it = FindKey(tree_data->GetKey());
					if (it != nullptr)
					{
						child = it;
						item++;
						delete(tree_data);
						continue;
					}
				}
				else
				{
					tree<CTreeData*>::iterator it;
					//Recherche de la clé
					it = FindKey(tree_data->GetKey(), child);
					if (it != nullptr)
					{
						child = it;
						item++;
						delete(tree_data);
						continue;
					}
				}
			}

			if (item > 0)
			{
				child = tr.append_child(child, tree_data);
			}
			else
			{
				tree_data->SetExifKey("-1");
				child = tr.insert(top, tree_data);
			}
		}
		else
		{
			tree_data->SetIsParent(false);
			tree_data->SetValue(localLibelle);
			tree_data->SetExifKey(key);
			tr.append_child(child, tree_data);
		}
		item++;
	}

	UpdateElement();
	eventControl->UpdateTreeControl();
}

void CInfoEffect::CreateElement()
{
	vectorPosElement.clear();
	vectorPosElementDynamic.clear();
	tree<CTreeData*>::sibling_iterator it = tr.begin();
	auto itend = tr.end();
	yPos = 0;
	nbRow = 0;

	while (it != itend)
	{
		CTreeData* data = *it;
		const int profondeur = tr.depth(it);
		if (profondeur == 0)
		{
			int xPos = themeTree.GetMargeX();
			int widthElement = 0;
			CTreeElementTexte* treeElementTexte = nullptr;
			CPositionElement* posElement = nullptr;

			auto treeElementTriangle = CreateTriangleElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(),
			                                                 true);
			posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTriangle->GetWidth(),
			                                   treeElementTriangle->GetHeight(), ELEMENT_TRIANGLE, treeElementTriangle,
			                                   data);

			xPos += posElement->GetWidth() + themeTree.GetMargeX();
			widthPosition = posElement->GetWidth() + themeTree.GetMargeX();

			treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey());
			posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTexte->GetWidth(),
			                                   treeElementTexte->GetHeight(), ELEMENT_TEXTE, treeElementTexte, data);

			widthElement += xPos + posElement->GetWidth() + themeTree.GetMargeX();
			yPos += themeTree.GetRowHeight();
			nbRow++;
			if (rowWidth[0] < widthElement)
				rowWidth[0] = widthElement;
		}
		++it;
	}
}


void CInfoEffect::UpdateElement()
{
	for (CPositionElement* value : vectorPosElement)


	{
		if (value != nullptr)
		{
			CTreeElement* treeElement = value->GetTreeElement();
			if (treeElement != nullptr)
				treeElement->SetVisible(false);
		}
	}
	tree<CTreeData*>::sibling_iterator it = tr.begin();
	auto itend = tr.end();
	yPos = 0;
	nbRow = 0;

	while (it != itend)
	{
		CTreeData* data = *it;
		const int profondeur = tr.depth(it);
		if (profondeur == 0)
		{
			bool isVisible = true;
			int xPos = themeTree.GetMargeX();
			int widthElement = 0;
			CTreeElementTriangle* tree_element_triangle;
			CPositionElement* posElement = GetElement(data, ELEMENT_TRIANGLE);
			if (posElement == nullptr)
			{
				tree_element_triangle = CreateTriangleElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), true);
				posElement = CreatePositionElement(xPos, yPos, nbRow, 0, tree_element_triangle->GetWidth(),
				                                   tree_element_triangle->GetHeight(), ELEMENT_TRIANGLE,
				                                   tree_element_triangle, data);
			}
			else
			{
				tree_element_triangle = dynamic_cast<CTreeElementTriangle*>(posElement->GetTreeElement());
				tree_element_triangle->SetVisible(isVisible);
				posElement->SetX(xPos);
				posElement->SetY(yPos);
			}
			widthPosition = xPos + posElement->GetWidth() + themeTree.GetMargeX();
			xPos += posElement->GetWidth() + themeTree.GetMargeX();


			posElement = GetElement(data, ELEMENT_TEXTE);
			if (posElement == nullptr)
			{
				CTreeElementTexte* treeElementTexte = CreateTexteElement(
					themeTree.GetRowWidth(), themeTree.GetRowHeight(),
					data->GetKey());
				posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTexte->GetWidth(),
				                                   treeElementTexte->GetHeight(), ELEMENT_TEXTE, treeElementTexte,
				                                   data);
			}
			else
			{
				auto treeElementTexte = dynamic_cast<CTreeElementTexte*>(posElement->GetTreeElement());
				treeElementTexte->SetVisible(isVisible);
				posElement->SetX(xPos);
				posElement->SetY(yPos);
			}

			widthElement += xPos + posElement->GetWidth() + themeTree.GetMargeX();
			yPos += themeTree.GetRowHeight();
			nbRow++;
			if (rowWidth[0] < widthElement)
				rowWidth[0] = widthElement;

			bool isOpen = false;
			if (tree_element_triangle != nullptr)
				isOpen = tree_element_triangle->GetOpen();

			if (isOpen)
				UpdateChildTree(it);
		}
		++it;
	}
}

void CInfoEffect::UpdateScreenRatio()
{
	UpdateElement();
}

void CInfoEffect::UpdateChildTree(tree<CTreeData*>::sibling_iterator& parent)
{
	tree<CTreeData*>::sibling_iterator it = tree<CTreeData*>::begin(parent);

	for (auto i = 0; i < parent.number_of_children(); i++)
	{
		const bool isVisible = true;
		const int profondeur = tr.depth(it);
		CTreeData* data = *it;
		int xPos = widthPosition * (profondeur);
		CPositionElement* posElement = GetElement(data, ELEMENT_TEXTE);
		if (posElement == nullptr)
		{
			CTreeElementTexte* treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(),
			                                                         data->GetKey());
			treeElementTexte->SetVisible(isVisible);
			posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTexte->GetWidth(),
			                                   treeElementTexte->GetHeight(), ELEMENT_TEXTE, treeElementTexte, data);
		}
		else
		{
			auto treeElementTexte = static_cast<CTreeElementTexte*>(posElement->GetTreeElement());
			treeElementTexte->SetVisible(isVisible);
			posElement->SetX(xPos);
			posElement->SetY(yPos);
		}

		const int widthElement = xPos + posElement->GetWidth() + themeTree.GetMargeX();
		yPos += themeTree.GetRowHeight();

		nbRow++;
		if (rowWidth[0] < widthElement)
			rowWidth[0] = widthElement;

		++it;
	}
}
