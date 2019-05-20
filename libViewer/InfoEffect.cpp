#include <header.h>
#include "InfoEffect.h"
#include "TreeElementTexte.h"
#include "TreeElementTriangle.h"
#include "ModificationManager.h"
#include <ImageLoadingFormat.h>
#include "ViewerTheme.h"
#include "ViewerThemeInit.h"
using namespace Regards::Window;
using namespace Regards::Viewer;


CInfoEffect::CInfoEffect(CBitmapWndViewer * bitmapViewer, CTreeElementControlInterface * interfaceControl, CModificationManager * modificationManager)
{
	widthPosition = 0;
	CViewerTheme * viewerTheme = CViewerThemeInit::getInstance();

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
	this->bitmapViewer = bitmapViewer;
	rowWidth.push_back(0);
};



CInfoEffect::~CInfoEffect()
{
	
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
void CInfoEffect::AddModification(CRegardsBitmap * bitmap, const wxString &libelle)
{
    int numModification = modificationManager->GetNumModification();
    int nbModification = modificationManager->GetNbModification();
    
    if(numModification < nbModification)
    {
        ClearData();
        
        InitTree(modificationManager->GetModificationLibelle(0), to_string(0));

        for(int i = 1;i < (numModification+1);i++)
        {
            wxString modificationString = modificationManager->GetModificationLibelle(i);
            modificationString.Replace(".","@99");
            AddEvent("Effect." + modificationString, to_string(i));
            index++;
        }
    }
    
	wxString localLibelle = libelle;
	localLibelle.Replace(".", "@99");
	modificationManager->AddModification(bitmap, libelle);
	AddEvent("Effect." + localLibelle, GetNumModification()); index++;
	SetActifElement(GetNumModification());
}

void CInfoEffect::InitTree(const wxString &libelle, const wxString &key)
{
    filename = libelle;
    numEvent = 1;
    index = 0;
    //Récupération des catégories principales
    /*
    top = tr.begin();
    CTreeData * treeData = new CTreeData();
    treeData->SetIsParent(true);
    treeData->SetKey(libelle);
    treeData->SetExifKey(key);
    child = tr.insert(top, treeData);
    */
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

void CInfoEffect::ClickOnElement(CPositionElement * element, wxWindow * window, const int &x, const int &y, const int& posLargeur, const int &posHauteur)
{
	CTreeElement * treeElement = element->GetTreeElement();
	CTreeData * treeData = (CTreeData *)element->GetTreeData();
	if (element->GetType() == ELEMENT_TEXTE)
	{
		if (bitmapViewer != nullptr)
		{
			wxString key = treeData->GetExifKey();
            if(key != "-1")
            {
                int modif = atoi(key.c_str());

				CImageLoadingFormat * imageLoad = new CImageLoadingFormat();
				imageLoad->SetPicture(modificationManager->GetModification(modif));
                bitmapViewer->SetBitmap(imageLoad);
                //bitmapViewer->Refresh();
                SetActifElement(key);
            }
		}
	}
	else if (element->GetType() == ELEMENT_TRIANGLE)
	{
		CTreeElementTriangle * treeElementTriangle = (CTreeElementTriangle *)treeElement;
		treeElementTriangle->ClickElement(window, (x + posLargeur) - element->GetX(), (y + posHauteur) - element->GetY());
		UpdateElement();
		eventControl->UpdateTreeControl();
	}
}

void CInfoEffect::MouseOver(wxDC * deviceContext, CPositionElement * element, const int &x, const int &y, const int& posLargeur, const int &posHauteur)
{
	int xPos = 0;
	if (element->GetRow() > 0)
		xPos = GetWidthRow(element->GetRow() - 1);

	element->GetTreeElement()->MouseOver(deviceContext, element->GetX() + xPos, element->GetY());
}

wxString CInfoEffect::GetFilename()
{
	return filename;
}

void CInfoEffect::Init(CRegardsBitmap * bitmap, const wxString &libelle, const wxString &key)
{
	modificationManager->Init(bitmap);
    InitTree(libelle, key);

}

void CInfoEffect::SetActifElement(const wxString &key)
{
	for (PositionElementVector::iterator fit = vectorPosElement.begin(); fit != vectorPosElement.end(); fit++)
	{
		CPositionElement * posElement = *fit;
		if (posElement != nullptr)
		{
			CTreeData * data = posElement->GetTreeData();
			if (data->GetExifKey() == key)
				data->SetActif(true);
			else
				data->SetActif(false);
		}
	}
	eventControl->UpdateTreeControl();
}

void CInfoEffect::AddEvent(const wxString &libelle, const wxString &key)
{
	//Récupération des catégories principales
	numEvent+=2;
	//tree<CTreeData *>::iterator child;
	//tree<CTreeData *>::iterator top = tr.begin();
	wxString localLibelle = libelle;
	localLibelle.Replace("@99", ".");
	CTreeData * treeData = new CTreeData();
	treeData->SetIsParent(true);
	treeData->SetKey(localLibelle);
	treeData->SetExifKey(key);

	//FindKey(const wxString & key)
	//child = tr.insert_after(child, treeData);

	wchar_t seps[] = L".";
	int item = 0;
	wchar_t *next_token1 = nullptr;
	wchar_t informations[1024];
	wcscpy(informations, libelle.c_str());

	// Establish string and get the first token:
#if defined(WIN32) && _MSC_VER < 1900
	wchar_t * token = wcstok(informations, seps); // C4996
#else
	wchar_t * token = wcstok(informations, seps, &next_token1); // C4996
#endif

	// Note: strtok is deprecated; consider using strtok_s instead
	while (token != nullptr)
	{
		CTreeData * treeData = new CTreeData();
		wxString value = token;
		value.Replace("@99", ".");
		treeData->SetKey(value);
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
                treeData->SetExifKey("-1");
				child = tr.insert(top, treeData);
			}
		}
		else
		{
			treeData->SetIsParent(false);
			treeData->SetValue(localLibelle);
            treeData->SetExifKey(key);
			tr.append_child(child, treeData);
		}
		item++;
	}
	
	UpdateElement();
	eventControl->UpdateTreeControl();
}

void CInfoEffect::CreateElement()
{
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
			CTreeElementTriangle * treeElementTriangle = nullptr;
			CTreeElementTexte * treeElementTexte = nullptr;
			CPositionElement * posElement = nullptr;

			treeElementTriangle = CreateTriangleElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), true);
			posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTriangle->GetWidth(), treeElementTriangle->GetHeight(), ELEMENT_TRIANGLE, treeElementTriangle, data);

			xPos += posElement->GetWidth() + themeTree.GetMargeX();
			widthPosition = posElement->GetWidth() + themeTree.GetMargeX();

			treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey());
			posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTexte->GetWidth(), treeElementTexte->GetHeight(), ELEMENT_TEXTE, treeElementTexte, data);

			widthElement += xPos + posElement->GetWidth() + themeTree.GetMargeX();
			yPos += themeTree.GetRowHeight();
			nbRow++;
			if (rowWidth[0] < widthElement)
				rowWidth[0] = widthElement;
		}
		it++;

	}
}


void CInfoEffect::UpdateElement()
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
			CTreeElementTriangle * treeElementTriangle = nullptr;
			CTreeElementTexte * treeElementTexte = nullptr;
			CPositionElement * posElement = GetElement(data, ELEMENT_TRIANGLE);
			if (posElement == nullptr)
			{
				treeElementTriangle = CreateTriangleElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), true);
				posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTriangle->GetWidth(), treeElementTriangle->GetHeight(), ELEMENT_TRIANGLE, treeElementTriangle, data);
			}
			else
			{
				treeElementTriangle = (CTreeElementTriangle *)posElement->GetTreeElement();
				treeElementTriangle->SetVisible(isVisible);
				posElement->SetX(xPos);
				posElement->SetY(yPos);
			}
			widthPosition = xPos + posElement->GetWidth() + themeTree.GetMargeX();
			xPos += posElement->GetWidth() + themeTree.GetMargeX();
			

			posElement = GetElement(data, ELEMENT_TEXTE);
			if (posElement == nullptr)
			{
				treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey());
				posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTexte->GetWidth(), treeElementTexte->GetHeight(), ELEMENT_TEXTE, treeElementTexte, data);
			}
			else
			{
				CTreeElementTexte * treeElementTexte = (CTreeElementTexte *)posElement->GetTreeElement();
				treeElementTexte->SetVisible(isVisible);
				posElement->SetX(xPos);
				posElement->SetY(yPos);
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

void CInfoEffect::UpdateScreenRatio()
{
    UpdateElement();
}

void CInfoEffect::UpdateChildTree(tree<CTreeData *>::sibling_iterator &parent)
{
	tree<CTreeData *>::sibling_iterator it = tr.begin(parent);
	//tree<CTreeData *>::iterator itend = tr.end(parent);
	bool isVisible = true;
	//int i = 

	for (auto i = 0; i < parent.number_of_children(); i++)
	{
		int profondeur = tr.depth(it);
		CTreeData * data = *it;
		int xPos = widthPosition * (profondeur);
		int widthElement = 0;
		CTreeElementTexte * treeElementTexte = nullptr;
		CPositionElement * posElement = GetElement(data, ELEMENT_TEXTE);
		if (posElement == nullptr)
		{
			treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey());
			treeElementTexte->SetVisible(isVisible);
			posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTexte->GetWidth(), treeElementTexte->GetHeight(), ELEMENT_TEXTE, treeElementTexte, data);
		}
		else
		{
			CTreeElementTexte * treeElementTexte = (CTreeElementTexte *)posElement->GetTreeElement();
			treeElementTexte->SetVisible(isVisible);
			posElement->SetX(xPos);
			posElement->SetY(yPos);
		}

		widthElement = xPos + posElement->GetWidth() + themeTree.GetMargeX();
		yPos += themeTree.GetRowHeight();

		nbRow++;
		if (rowWidth[0] < widthElement)
			rowWidth[0] = widthElement;

		it++;

	}
}