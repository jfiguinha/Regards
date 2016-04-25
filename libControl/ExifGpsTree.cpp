#include "ExifGpsTree.h"
#include <ConvertUtility.h>
#include <TreeElementTexte.h>
#include <TreeElementTriangle.h>
#include "TreeElementTexteClick.h"
#include <SqlPhotos.h>
#include "TreeDataLink.h"
#include <PhotoCriteria.h>
#include <wx/filename.h>
#if defined(__WXMSW__)
#include "../include/window_id.h"
#else
#include <window_id.h>
#endif
#ifdef __APPLE__
#include <AppleReadExif.h>
#endif
using namespace Regards::Control;
using namespace Regards::Window;
using namespace Regards::Sqlite;
#define TAILLEMAX 4096

CExifGpsTree::CExifGpsTree(CThemeTree * theme, CTreeElementControlInterface * interfaceControl)
{
	rowWidth.push_back(0);
	rowWidth.push_back(0);
	themeTree = *theme;
	themeTree.themeTriangle.SetHeight(themeTree.GetRowHeight());
	themeTree.themeCheckbox.SetHeight(themeTree.GetRowHeight());
	themeTree.themeDelete.SetHeight(themeTree.GetRowHeight());
	themeTree.themeSlide.SetHeight(themeTree.GetRowHeight());
	themeTree.themeTexte.SetHeight(themeTree.GetRowHeight());
	eventControl = interfaceControl;
    
};



wxString CExifGpsTree::GetFilename()
{
	return filename;
}

void CExifGpsTree::MouseOver(wxDC * deviceContext, CPositionElement * element, const int &x, const int &y, const int& posLargeur, const int &posHauteur)
{
	if (element != nullptr)
	{
		CTreeElement * treeElement = element->GetTreeElement();
		if (treeElement != nullptr)
			treeElement->MouseOver(deviceContext, x, y);
	}
}


void CExifGpsTree::UpdateScreenRatio()
{
    //this->UpdateElement();
}

void CExifGpsTree::AddTreeInfos(const wxString &exifKey, const wxString &exifValue, const int &index, tree<CTreeData *>::iterator &top, tree<CTreeData *>::iterator &child)
{
	wchar_t seps[] = L".";
	int item = 0;
	wchar_t informations[TAILLEMAX];
	wcscpy(informations, exifKey.c_str());
    wchar_t * token1;
    
	// Establish string and get the first token:
#ifdef WIN32
	wchar_t * token = wcstok(informations, seps); // C4996
#else
	wchar_t * token = wcstok(informations, seps, &token1); // C4996
#endif
	// Note: strtok is deprecated; consider using strtok_s instead
	while (token != nullptr)
	{
		CTreeData * treeData = new CTreeData();
		treeData->SetKey(token);
#ifdef WIN32
		token = wcstok(nullptr, seps); // C4996
#else
		token = wcstok(nullptr, seps, &token1); // C4996
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
			treeData->SetValue(exifValue);
			treeData->SetExifKey(exifKey);
			tr.append_child(child, treeData);
		}
		item++;
	}
}

void CExifGpsTree::SetFile(const wxString & picture)
{
	if (filename == picture)
		return;
    
    top = tr.begin();
    tree<CTreeData *>::iterator child = top;
    
    CTreeData * treeDataPicture = new CTreeData();
    treeDataPicture->SetKey("Gps");
    child = tr.insert(top, treeDataPicture);
    
#ifdef __APPLE__
    CAppleReadExif appleReadExif;
    vector<CMetadata> listItem;
    
    listItem = appleReadExif.GetGpsMetadata(picture);
    
    if(listItem.size() > 0)
    {
        int index = 0;
        for(CMetadata meta : listItem)
        {
            AddTreeInfos(meta.key, meta.value, index, top, child);
            index++;
        }
    }
#endif
    CreateElement();
	
}

void CExifGpsTree::SetFile(const wxString & picture, const float &latitude, const float &longitude)
{
    if (filename == picture)
        return;
    
    top = tr.begin();
    tree<CTreeData *>::iterator child = top;
    
    CTreeData * treeDataPicture = new CTreeData();
    //treeDataPicture->SetKey("Gps");
    //child = tr.insert(top, treeDataPicture);
#ifdef __APPLE__  
    CAppleReadExif appleReadExif;
    vector<CMetadata> listItem;
    
    listItem = appleReadExif.GetPreformatGpsMetadata(picture, latitude, longitude);
    
    if(listItem.size() > 0)
    {
        int index = 0;
        
        for(CMetadata meta : listItem)
        {
            AddTreeInfos(meta.key, meta.value, index, top, child);
            index++;
        }
    }
#endif 
    CreateElement();
    
}


void CExifGpsTree::CreateElement()
{
    bool isVisible = true;
    widthPosition = 0;
    
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
            
            xPos += posElement->GetWidth() + themeTree.GetMargeX();
            widthPosition = posElement->GetWidth() + themeTree.GetMargeX();
            
            
            if(data->GetType() == 2)
            {
                CTreeDataLink * dataLink = (CTreeDataLink *)data;
                treeElementTexte = CreateTexteLinkElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey(), dataLink->GetLinkPath(), dataLink->GetLinkType());
            }
            else
                treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey());
            
            treeElementTexte->SetVisible(isVisible);
            posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTexte->GetWidth(), treeElementTexte->GetHeight(), ELEMENT_TEXTE, treeElementTexte, data, false);
            
            widthElement += xPos + posElement->GetWidth() + themeTree.GetMargeX();
            yPos += themeTree.GetRowHeight();
            nbRow++;
            if (rowWidth[0] < widthElement)
                rowWidth[0] = widthElement;
            
            if (treeElementTriangle->GetOpen())
                CreateChildTree(it);
            
        }
        it++;
        
    }
}


void CExifGpsTree::CreateChildTree(tree<CTreeData *>::sibling_iterator &parent)
{
    CPositionElement * posElement = nullptr;
    tree<CTreeData *>::sibling_iterator it = tr.begin(parent);
    //tree<CTreeData *>::iterator itend = tr.end(parent);
    bool isVisible = true;
    //int i =
    
    for (int i = 0; i < parent.number_of_children(); i++)
    {
        int profondeur = tr.depth(it);
        CTreeData * data = *it;
        
        if (data->GetValue().size() > 0 || it.number_of_children() == 0)
        {
            int xPos = widthPosition * (profondeur + 1);
            int widthElementColumn1 = 0;
            int widthElementColumn2 = 0;
            CTreeElementTexte * treeElementTexte = nullptr;
            
            if(data->GetType() == 2)
            {
                CTreeDataLink * dataLink = (CTreeDataLink *)data;
                treeElementTexte = CreateTexteLinkElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey(), dataLink->GetLinkPath(), dataLink->GetLinkType());
            }
            else
                treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey());
            
            treeElementTexte->SetVisible(isVisible);
            posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTexte->GetWidth(), treeElementTexte->GetHeight(), ELEMENT_TEXTE, treeElementTexte, data, false);
            
            widthElementColumn1 = xPos + posElement->GetWidth() + themeTree.GetMargeX();
            
            if (data->GetValue() != "")
            {
                xPos = themeTree.GetMargeX();
                
                if(data->GetType() == 2)
                {
                    CTreeDataLink * dataLink = (CTreeDataLink *)data;
                    treeElementTexte = CreateTexteLinkElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetValue(), dataLink->GetLinkPath(), dataLink->GetLinkType());
                }
                else
                    treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetValue());
                
                treeElementTexte->SetVisible(isVisible);
                posElement = CreatePositionElement(xPos, yPos, nbRow, 1, treeElementTexte->GetWidth(), treeElementTexte->GetHeight(), ELEMENT_TEXTEVALUE, treeElementTexte, data, (data->GetType() == 2));
                widthElementColumn2 = xPos + posElement->GetWidth() + themeTree.GetMargeX();
            }
            
            yPos += themeTree.GetRowHeight();
            
            nbRow++;
            if (rowWidth[0] < widthElementColumn1)
                rowWidth[0] = widthElementColumn1;
            
            if (rowWidth[1] < widthElementColumn2)
                rowWidth[1] = widthElementColumn2;
        }
        else
        {
            int xPos = widthPosition * profondeur;
            int widthElement = 0;
            CTreeElementTexte * treeElementTexte = nullptr;
            CTreeElementTriangle * treeElementTriangle = nullptr;
            treeElementTriangle = CreateTriangleElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), true);
            treeElementTriangle->SetVisible(isVisible);
            posElement = CreatePositionElement(xPos, yPos, nbRow, 0, treeElementTriangle->GetWidth(), treeElementTriangle->GetHeight(), ELEMENT_TRIANGLE, treeElementTriangle, data);
            
            xPos += posElement->GetWidth() + themeTree.GetMargeX();
            
            if(data->GetType() == 2)
            {
                CTreeDataLink * dataLink = (CTreeDataLink *)data;
                treeElementTexte = CreateTexteLinkElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey(), dataLink->GetLinkPath(), dataLink->GetLinkType());
            }
            else
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